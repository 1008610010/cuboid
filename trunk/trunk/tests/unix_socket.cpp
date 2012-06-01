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

		static float Frac(float f)
		{
			return f - static_cast<int>(f);
		}

		explicit UnixSocketServer(const std::string & unixSocketPath, int listenBacklog = DEFAULT_LISTEN_BACKLOG, float selectTimeout = DEFAULT_SELECT_TIMEOUT)
			: unixSocketPath_(unixSocketPath),
			  serverAddress_(unixSocketPath),
			  serverSocket_(XTL::UnixSocket::Create(false)),
			  socketSelector_(),
			  selectTimeout_(selectTimeout, 1000000 * Frac(selectTimeout)),
			  clients_()
		{
			try
			{
				serverSocket_.Bind(serverAddress_);
			}
			catch (const XTL::UnixError & e)
			{
				if (e.Code() == EADDRINUSE)
				{
					serverAddress_.Unlink();
					serverSocket_.Bind(serverAddress_);
				}
				else
				{
					throw;
				}
			}

			serverSocket_.Listen(listenBacklog);

			socketSelector_.Insert(serverSocket_, true, false);
		}

		~UnixSocketServer() throw()
		{
			try
			{
				serverAddress_.Unlink();
			}
			catch (const XTL::UnixError & e)
			{
				fprintf(stderr, "Could not unlink unix socket '%s': %s\n", unixSocketPath_.c_str(), e.What().c_str());
			}
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

		virtual std::auto_ptr<ClientHandler> CreateClientHandler(Client & client) = 0;

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

		const std::string      unixSocketPath_;
		XTL::SocketAddressUnix serverAddress_;
		XTL::UnixServerSocket  serverSocket_;
		XTL::SocketSelector    socketSelector_;
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

#include <xtl/io/FileOutputStreamBuffered.hpp>
#include <xtl/linux/fs/FileUtils.hpp>
#include <xtl/FormatString.hpp>

namespace XTL
{
	class FileTransaction : public OutputStream
	{
		public:

			static const unsigned int DEFAULT_BUFFER_CAPACITY = (1 << 20);

			FileTransaction(const std::string & filePath, unsigned int bufferCapacity= DEFAULT_BUFFER_CAPACITY)
				: filePath_(filePath),
				  outputStream_(TempFilePath(filePath), bufferCapacity)
			{
				;;
			}

			virtual ~FileTransaction() throw()
			{
				;;
			}

			virtual void Write(const void * buffer, unsigned int size)
			{
				outputStream_.Write(buffer, size);
			}

			FileSize Position() const
			{
				return outputStream_.Position();
			}

			void Commit()
			{
				outputStream_.Flush();
				outputStream_.Close();

				FileUtils::Rename(TempFilePath(filePath_), filePath_);
			}

		private:

			static const std::string TempFilePath(const std::string & filePath)
			{
				return filePath + ".tmp";
			}

			const std::string filePath_;
			FileOutputStreamBuffered outputStream_;
	};
}

class StatsWriterServer : public UnixSocketServer
{
	public:

		class MessageBuffer
		{
			public:

				MessageBuffer()
					: buffer_()
				{
					;;
				}

				void Write(const void * ptr, unsigned int size)
				{
					unsigned int oldSize = buffer_.size();
					buffer_.resize(oldSize + size);
					::memcpy(&(buffer_[oldSize]), ptr, size);
				}

				bool HasHeader() const
				{
					return buffer_.size() >= sizeof(StatsMessageHeader);
				}

				const StatsMessageHeader * Header() const
				{
					return reinterpret_cast<const StatsMessageHeader *>(&(buffer_[0]));
				}

				unsigned int TotalSize() const
				{
					return sizeof(StatsMessageHeader) + Header()->Size();
				}

				bool IsMagicValid() const
				{
					return Header()->IsMagicValid();
				}

				bool IsComplete() const
				{
					return HasHeader() && buffer_.size() >= TotalSize();
				}

				void Remove()
				{
					buffer_.erase(buffer_.begin(), buffer_.begin() + TotalSize());
				}

			private:

				std::vector<char> buffer_;
		};

		class WriterHandler : public UnixSocketServer::ClientHandler
		{
			public:

				explicit WriterHandler(StatsWriterServer & server, UnixSocketServer::Client & client)
					: UnixSocketServer::ClientHandler(client),
					  server_(server),
					  messageBuffer_()
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

					messageBuffer_.Write(buffer, size);

					while (messageBuffer_.IsComplete())
					{
						if (!messageBuffer_.Header()->IsMagicValid())
						{
							fprintf(stderr, "Message magic is invalid");
							Disconnect();
						}

						server_.WriteMessage(messageBuffer_);
						messageBuffer_.Remove();
					}
				}

			private:

				StatsWriterServer & server_;
				MessageBuffer messageBuffer_;
		};

		explicit StatsWriterServer(const std::string & unixSocketPath, const std::string & outputDir)
			: UnixSocketServer(unixSocketPath),
			  outputDir_(outputDir),
			  outputStream_()
		{
			;;
		}

		virtual ~StatsWriterServer() throw()
		{
			;;
		}

		virtual std::auto_ptr<UnixSocketServer::ClientHandler> CreateClientHandler(Client & client)
		{
			return std::auto_ptr<UnixSocketServer::ClientHandler>(new WriterHandler(*this, client));
		}

		void WriteMessage(const MessageBuffer & messageBuffer)
		{
			if (outputStream_.get() == 0)
			{
				const std::string filePath = XTL::FormatString("%s/binlog.%s", outputDir_, NowString());
				fprintf(stderr, "Creating %s\n", filePath.c_str());
				outputStream_.reset(new XTL::FileTransaction(filePath));
			}

			outputStream_->Write(messageBuffer.Header(), messageBuffer.TotalSize());

			if (outputStream_->Position() >= 12 /*100 * (1 << 20)*/)
			{
				outputStream_->Commit();
				outputStream_.reset();
			}
		}

	private:

		static const std::string NowString()
		{
			time_t now = ::time(0);

			struct tm * t = ::gmtime(&now);

			return XTL::FormatString("%04d-%02d-%02d-%02d-%02d-%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
		}

		const std::string outputDir_;
		std::auto_ptr<XTL::FileTransaction> outputStream_;
};

int main(int argc, const char * argv[])
{
	int i;
	printf("%s\n", i);

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
			StatsWriterServer server(UNIX_SOCKET_PATH, "/home/dnikolaev/prj/cuboid/trunk/build");

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

