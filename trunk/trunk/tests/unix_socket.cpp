#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdexcept>
#include <map>
#include <memory>
#include <vector>

#include <xtl/Types.hpp>
#include <xtl/utils/AutoPtrMap.hpp>
#include <xtl/linux/UnixError.hpp>
#include <xtl/linux/net/UnixServerSocket.hpp>
#include <xtl/linux/net/SocketAddressUnix.hpp>
#include <xtl/linux/net/SocketSelector.hpp>
#include <xtl/linux/net/SocketSet.hpp>

const char * const UNIX_SOCKET_PATH = "LJ_STATS_WRITER";

/*

class WriterClient
{
	public:

		class Disconnected
		{
		};

		explicit WriterClient(XTL::UnixClientSocket clientSocket)
			: clientSocket_(clientSocket)
		{
			;;
		}

		void Clear()
		{
			buffer_.resize(0);
		}

		void Receive()
		{
			char buffer[3];

			int wasRead = clientSocket_.Receive(buffer, sizeof(buffer));

			if (wasRead < 0)
			{
				throw Disconnected();
			}

			OnBufferReceived(buffer, wasRead);
		}

		void OnBufferReceived(const void * buffer, int size)
		{
			if (size <= 0)
			{
				return;
			}

			printf("OnBufferReceived(%d)\n", size);

			unsigned int oldSize = buffer_.size();
			buffer_.resize(oldSize + size);
			::memcpy(&(buffer_[oldSize]), buffer, size);

			if (IsComplete())
			{
				printf("Message received!\n");
				Clear();
			}
		}

		bool IsComplete() const
		{
			if (buffer_.size() < sizeof(MessageHeader))
			{
				return false;
			}

			const MessageHeader * header = reinterpret_cast<const MessageHeader *>(&(buffer_[0]));

			if (!header->IsMagicValid())
			{
				throw std::runtime_error("Message magic is invalid");
			}

			return buffer_.size() >= sizeof(MessageHeader) + header->Size();
		}

	private:

		XTL::UnixClientSocket clientSocket_;
		std::vector<char> buffer_;
};

void PackMessage(const void * messageBody, unsigned int messageSize, std::vector<char> & buffer)
{
	buffer.resize(sizeof(MessageHeader) + messageSize);

	MessageHeader header(messageSize);
	::memcpy(&(buffer[0]), &header, sizeof(MessageHeader));
	::memcpy(&(buffer[sizeof(MessageHeader)]), messageBody, messageSize);
}

void PackMessage(const std::string & messageBody, std::vector<char> & buffer)
{
	PackMessage(messageBody.data(), messageBody.size(), buffer);
}
*/

class UnixSocketClient
{
	public:

		explicit UnixSocketClient(const std::string & unixSocketPath)
			: serverAddress_(unixSocketPath),
			  socket_(XTL::UnixSocket::Create(true))
		{
			;;
		}

		void Connect()
		{
			socket_.Connect(serverAddress_);
		}

		void Send(const void * buffer, unsigned int size)
		{
			if (size == 0)
			{
				return;
			}

			socket_.Send(buffer, size);
		}

	private:

		XTL::SocketAddressUnix serverAddress_;
		XTL::UnixClientSocket socket_;
};

class UnixSocketServer
{
	public:

		static const int DEFAULT_LISTEN_BACKLOG = 5;
		static const int DEFAULT_SELECT_TIMEOUT = 1;

		float Frac(float f)
		{
			return f - static_cast<int>(f);
		}

		explicit UnixSocketServer(const std::string & unixSocketPath, int listenBacklog = DEFAULT_LISTEN_BACKLOG, float selectTimeout = DEFAULT_SELECT_TIMEOUT)
			: unixSocketPath_(unixSocketPath),
			  serverSocket_(XTL::UnixSocket::Create(false)),
			  socketSelector_(),
			  selectTimeout_(selectTimeout, 1000000 * Frac(selectTimeout)),
			  clients_()
		{
			XTL::SocketAddressUnix serverAddress(UNIX_SOCKET_PATH);

			try
			{
				serverSocket_.Bind(serverAddress);
			}
			catch (const XTL::UnixError & e)
			{
				if (e.Code() == EADDRINUSE)
				{
					serverAddress.Unlink();
					serverSocket_.Bind(serverAddress);
				}
				else
				{
					throw;
				}
			}

			serverSocket_.Listen(listenBacklog);

			socketSelector_.Insert(serverSocket_, true, false);
		}

		class Client;

		void Run()
		{
			while (true)
			{
				Iterate();
			}
		}

		class ClientDisconnected
		{
			public:

				ClientDisconnected(XTL::UnixClientSocket socket)
					: socket_(socket)
				{
					;;
				}

				XTL::UnixClientSocket Socket() const
				{
					return socket_;
				}

			private:

				const XTL::UnixClientSocket socket_;
		};

		class ClientHandler
		{
			public:

				explicit ClientHandler(Client & client)
					: client_(client)
				{
					;;
				}

				virtual ~ClientHandler() throw()
				{
					;;
				}

				virtual void OnDataReceived(const void * buffer, unsigned int size) = 0;

			protected:

				void Disconnect()
				{
					client_.Disconnect();
				}

			private:

				ClientHandler(const ClientHandler &);
				ClientHandler & operator= (const ClientHandler &);

				Client & client_;
		};

		virtual std::auto_ptr<ClientHandler> CreateClientHandler(Client & client) const = 0;

		class Client
		{
			public:

				static const unsigned int RECEIVE_BUFFER_SIZE = 3;

				explicit Client(XTL::UnixClientSocket socket)
					: socket_(socket),
					  buffer_(),
					  handler_()
				{
					;;
				}

				void Disconnect()
				{
					throw ClientDisconnected(socket_);
				}

			protected:

				friend class UnixSocketServer;

				void SetHandler(std::auto_ptr<ClientHandler> handler)
				{
					handler_ = handler;
				}

				void Receive()
				{
					int wasRead = socket_.Receive(buffer_, sizeof(buffer_));

					if (wasRead < 0)
					{
						Disconnect();
					}

					if (wasRead > 0 && handler_.get() != 0)
					{
						handler_->OnDataReceived(buffer_, wasRead);
					}
				}

			private:

				const XTL::UnixClientSocket socket_;
				char buffer_[RECEIVE_BUFFER_SIZE];
				std::auto_ptr<ClientHandler> handler_;
		};

	private:

		void Iterate()
		{
			XTL::SocketSelector::SelectResult selectResult;

			socketSelector_.Select(selectResult, selectTimeout_);

			if (selectResult.SelectedCount() == 0)
			{
				return;
			}

			if (selectResult.IsReadable(serverSocket_))
			{
				XTL::UnixClientSocket clientSocket = serverSocket_.Accept();

				if (!clientSocket.IsNull())
				{
					std::auto_ptr<Client> client(new Client(clientSocket));
					std::auto_ptr<ClientHandler> clientHandler(CreateClientHandler(*client));
					client->SetHandler(clientHandler);

					socketSelector_.Insert(clientSocket, true, true);

					clients_.Set(clientSocket, client);
				}
			}


			const XTL::SocketSet readable = selectResult.ReadableSet();

			std::vector<XTL::UnixClientSocket> disconnected;

			for (XTL::AutoPtrMap<XTL::UnixClientSocket, Client>::const_iterator itr = clients_.begin(); itr != clients_.end(); ++itr)
			{
				if (readable.Contains(itr->first))
				{
					try
					{
						itr->second->Receive();
					}
					catch (const ClientDisconnected & e)
					{
						disconnected.push_back(e.Socket());
					}
				}
			}

			for (std::vector<XTL::UnixClientSocket>::iterator itr = disconnected.begin(); itr != disconnected.end(); ++itr)
			{
				fprintf(stderr, "Client disconnected\n");
				socketSelector_.Delete(*itr);
				clients_.Erase(*itr);
			}
		}

		const std::string     unixSocketPath_;
		XTL::UnixServerSocket serverSocket_;
		XTL::SocketSelector   socketSelector_;
		XTL::SocketSelector::Timeout selectTimeout_;
		XTL::AutoPtrMap<XTL::UnixClientSocket, Client> clients_;
};

class StatsMessageHeader
{
	public:

		static const XTL::UINT_32 MAGIC = 0x2a2a2a2a;

		explicit StatsMessageHeader(XTL::UINT_32 size)
			: magic_(MAGIC),
			  size_(size)
		{
			;;
		}

		bool IsMagicValid() const
		{
			return magic_ == MAGIC;
		}

		XTL::UINT_32 Size() const
		{
			return size_;
		}

	private:

		XTL::UINT_32 magic_;
		XTL::UINT_32 size_;
};

class StatsWriterClient
{
	public:

		explicit StatsWriterClient(const std::string & unixSocketPath)
			: client_(unixSocketPath)
		{
			client_.Connect();
		}

		void Send(const std::string & message)
		{
			StatsMessageHeader header(message.size());

			client_.Send(&header, sizeof(header));
			client_.Send(message.data(), message.size());
		}

	private:

		UnixSocketClient client_;
};

class StatsWriterServer : public UnixSocketServer
{
	public:

		class WriterHandler : public UnixSocketServer::ClientHandler
		{
			public:

				explicit WriterHandler(UnixSocketServer::Client & client)
					: UnixSocketServer::ClientHandler(client),
					  buffer_()
				{
					;;
				}

				virtual ~WriterHandler() throw()
				{
					;;
				}

				virtual void OnDataReceived(const void * buffer, unsigned int size)
				{
					fprintf(stderr, "OnDataReceived(%u)\n", size);

					unsigned int oldSize = buffer_.size();
					buffer_.resize(oldSize + size);
					::memcpy(&(buffer_[oldSize]), buffer, size);

					if (IsMessageReceived())
					{
						fprintf(stderr, "Message received!\n");
						Clear();
					}
				}

			private:

				void Clear()
				{
					buffer_.resize(0);
				}

				bool IsMessageReceived()
				{
					if (buffer_.size() < sizeof(StatsMessageHeader))
					{
						return false;
					}

					const StatsMessageHeader * header = reinterpret_cast<const StatsMessageHeader *>(&(buffer_[0]));

					if (!header->IsMagicValid())
					{
						fprintf(stderr, "Message magic is invalid");
						Disconnect();
					}

					return buffer_.size() >= sizeof(StatsMessageHeader) + header->Size();
				}

				std::vector<char> buffer_;
		};

		explicit StatsWriterServer(const std::string & unixSocketPath)
			: UnixSocketServer(unixSocketPath)
		{
			;;
		}

		virtual ~StatsWriterServer() throw()
		{
			;;
		}

		virtual std::auto_ptr<UnixSocketServer::ClientHandler> CreateClientHandler(Client & client) const
		{
			return std::auto_ptr<UnixSocketServer::ClientHandler>(new WriterHandler(client));
		}
};

int main(int argc, const char * argv[])
{
	pid_t pid = ::fork();

	if (pid == 0)
	{
		// Child process
		try
		{
			sleep(1);

			StatsWriterClient client(UNIX_SOCKET_PATH);

			client.Send("This is test message...");

			sleep(1);

			client.Send("This is test message 2...");
		}
		catch (const XTL::UnixError & e)
		{
			fprintf(stderr, "Client Error: %s\n", e.What().c_str());
			return 1;
		}
	}
	else
	{
		// Parent process

		try
		{
			StatsWriterServer server(UNIX_SOCKET_PATH);

			server.Run();

			fprintf(stderr, "Server is shutting down.\n");
		}
		catch (const XTL::UnixError & e)
		{
			fprintf(stderr, "%d %s\n", e.Code(), e.What().c_str());
			return 1;
		}
	}
}

