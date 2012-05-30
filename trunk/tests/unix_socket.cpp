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
			char buffer[1024];

			int wasRead = clientSocket_.Receive(buffer, sizeof(buffer));
			printf("Receive %d\n", wasRead);

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

		const std::string data = "This is test message...";

		std::vector<char> buffer;
		PackMessage(data.data(), data.size(), buffer);

		clientSocket.Send(&(buffer[0]), buffer.size());

		sleep(1);
	}
	else
	{
		// Parent process

		try
		{
			static const int SERVER_LISTEN_BACKLOG = 5;

			XTL::UnixServerSocket serverSocket = XTL::UnixSocket::Create(false);

			XTL::SocketAddressUnix serverAddress(UNIX_SOCKET_PATH);

			try
			{
				serverSocket.Bind(serverAddress);
			}
			catch (const XTL::UnixError & e)
			{
				if (e.Code() == EADDRINUSE)
				{
					serverAddress.Unlink();
					serverSocket.Bind(serverAddress);
				}
				else
				{
					throw;
				}
			}

			serverSocket.Listen(SERVER_LISTEN_BACKLOG);

			XTL::SocketSelector socketSelector;
			XTL::SocketSelector::SelectResult selectResult;

			socketSelector.Insert(serverSocket, true, false);

			XTL::AutoPtrMap<XTL::UnixClientSocket, WriterClient> clients;

			while (1)
			{
				socketSelector.Select(selectResult, XTL::SocketSelector::Timeout(1, 0));

				if (selectResult.SelectedCount() > 0)
				{
					if (selectResult.IsReadable(serverSocket))
					{
						XTL::UnixClientSocket clientSocket = serverSocket.Accept();
						if (!clientSocket.IsNull())
						{
							socketSelector.Insert(clientSocket, true, false);
							clients.Set(clientSocket, std::auto_ptr<WriterClient>(new WriterClient(clientSocket)));
							printf("Client accepted\n");
						}
					}

					const XTL::SocketSet readable = selectResult.ReadableSet();

					for (XTL::AutoPtrMap<XTL::UnixClientSocket, WriterClient>::const_iterator itr = clients.begin(); itr != clients.end(); ++itr)
					{
						if (readable.Contains(itr->first))
						{
							printf("Readable\n");
							itr->second->Receive();
						}
					}
				}
			}
		}
		catch (const XTL::UnixError::AlreadyExists & e)
		{
			fprintf(stderr, "Already exists\n");
			return 1;
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

