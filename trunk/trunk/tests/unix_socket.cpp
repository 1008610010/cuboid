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

/*
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
*/

const char * const UNIX_SOCKET_PATH = "LJ_STATS_WRITER";

class MessageHeader
{
	public:

		static const XTL::UINT_32 MAGIC = 0x2a2a2a2a;

		MessageHeader(XTL::UINT_32 size)
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
			char buffer[1];

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

				virtual ~ClientHandler() throw()
				{
					;;
				}

				virtual void OnDataReceived(const void * buffer, unsigned int size)
				{
					
				}

			private:

				
		};

		class Client
		{
			public:

				static const unsigned int RECEIVE_BUFFER_SIZE = 1;

				explicit Client(XTL::UnixClientSocket socket)
					: socket_(socket),
					  buffer_()
				{
					;;
				}

				void Receive()
				{
					int wasRead = socket_.Receive(buffer_, sizeof(buffer_));

					if (wasRead < 0)
					{
						throw ClientDisconnected(socket_);
					}

					if (wasRead > 0)
					{
						// OnDataReceived(buffer_, wasRead);
					}
				}

			protected:

				void Disconnect()
				{
					throw ClientDisconnected(socket_);
				}

			private:

				const XTL::UnixClientSocket socket_;
				char buffer_[RECEIVE_BUFFER_SIZE];
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

int main(int argc, const char * argv[])
{
	pid_t pid = ::fork();

	if (pid == 0)
	{
		// Child process
		sleep(1);

		XTL::UnixClientSocket clientSocket = XTL::UnixSocket::Create(true);
		XTL::SocketAddressUnix serverAddress(UNIX_SOCKET_PATH);

		clientSocket.Connect(serverAddress);

		std::vector<char> buffer;

		PackMessage("This is test message...", buffer);
		clientSocket.Send(&(buffer[0]), buffer.size());

		sleep(1);

		PackMessage("This is test message...", buffer);
		clientSocket.Send(&(buffer[0]), buffer.size());
	}
	else
	{
		// Parent process

		try
		{
			UnixSocketServer server(UNIX_SOCKET_PATH);

			server.Run();
		}
		catch (const XTL::UnixError & e)
		{
			fprintf(stderr, "%d %s\n", e.Code(), e.What().c_str());
			return 1;
		}

/*
		int serverSocket = -1;
		if ((serverSocket = ::socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
		{
			perror("socket");
			return 1;
		}

		struct sockaddr_un serverAddress;

		::memset(&serverAddress, '\0', sizeof(serverAddress));
		serverAddress.sun_family = AF_UNIX;
		::strcpy(serverAddress.sun_path, "LJ_STATS_WRITER");
		::unlink(serverAddress.sun_path);

		int serverAddressLength = ::strlen(serverAddress.sun_path) + sizeof(serverAddress.sun_family);
		if (::bind(serverSocket, (struct sockaddr *) &serverAddress, serverAddressLength) == -1)
		{
			perror("bind");
			return 1;
		}

		const int SERVER_BACKLOG = 5;
		if (::listen(serverSocket, SERVER_BACKLOG) == -1)
		{
			perror("listen");
			return 1;
		}

		while (1)
		{
			
		}
*/
	}
}

