#include <stdio.h>
//#include <xtl/SocketAddressInet.hpp>
//#include <xtl/TcpSocket.hpp>

static const int SERVER_PORT    = 1133;
static const int SERVER_BACKLOG =   64;
/*
	template <typename _DataType> class SocketMap
	{
		public:

			typedef std::map<int, _DataType, std::greater<int> > MapType;

			SocketMap() : items_(), maxFileDesc_(0) { ;; }

			_DataType * Find(int fd) const
			{
				typename MapType::iterator itr = items_.find(fd);
				return itr == items_.end() ? NULL : &(itr->second);
			}

			_DataType * Insert(int fd)
			{
				return &(items_[fd]);
			}

			void Delete(int fd)
			{
				items_.erase(fd);
			}

			inline void Clear()
			{
				items_.clear();
			}

			int Size() const
			{
				return items_.Size();
			}

			int MaxFileDesc() const
			{
				return items_.empty() ? 0 : items_.begin()->first;
			}

		protected:

			MapType items_;
			int maxFileDesc_;
	};

	class SocketSelector
	{
		public:

			SocketSelector() : maxfd_(-1)
			{
				FD_ZERO(&rdSet_);
				FD_ZERO(&wrSet_);
			}

			void Clear()
			{
				maxfd_ = -1;
				FD_ZERO(&rdSet_);
				FD_ZERO(&wrSet_);
			}

			bool Add(int fd, bool checkRead, bool checkWrite)
			{
				if (Invalid(fd))
				{
					return false;
				}
				if (checkRead)
				{
					FD_SET(fd, &rdSet_);
				}
				if (checkWrite)
				{
					FD_SET(fd, &wrSet_);
				}
				if ((checkRead || checkWrite) && fd > maxfd_)
				{
					maxfd_ = fd;
				}
				return true;
			}

			bool Delete(int fd)
			{
				if (Invalid(fd))
				{
					return false;
				}
				FD_CLR(fd, &rdSet_);
				FD_CLR(fd, &wrSet_);
				if (fd == maxfd_)
				{
					for (--maxfd_; maxfd_ >= 0; --maxfd_)
					{
						if (FD_ISSET(maxfd_, &rdSet_) || FD_ISSET(maxfd_, &wrSet_))
						{
							break;
						}
					}
				}
				return true;
			}

			int Size() const
			{
				return maxfd_ + 1;
			}

			bool Select(long timeout)
			{
				timeval t = { timeout / 1000000, timeout % 1000000 };
				fd_set rdSet;
				fd_set wrSet;
				memcpy(&rdSet, &rdSet_, sizeof(rdSet_));
				memcpy(&wrSet, &wrSet_, sizeof(wrSet_));

				int result = select(maxfd_ + 1, &rdSet, &wrSet, NULL, &t);

				if (result > 0)
				{
					return true;
				}
				else if (result < 0)
				{
					if (errno == EINTR)
					{
						return true;
					}
					else
					{
						// check errno == EBADF and remove error fd
						return false;
					}
				}
			}

			bool CanRead(int fd) const
			{
				return !Invalid(fd) && FD_ISSET(fd, &rdSet_);
			}

			bool CanWrite(int fd) const
			{
				return !Invalid(fd) && FD_ISSET(fd, &wrSet_);
			}

		protected:

			bool Invalid(int fd) const
			{
				return fd < 0 || fd >= FD_SETSIZE;
			}

			fd_set rdSet_;
			fd_set wrSet_;
			int    maxfd_;
	};
*/

class Ex {};

int main(int argc, const char * argv[])
{
	try
	{
		throw Ex();
	}
	catch (...)
	{
		fprintf(stderr, "Error\n");
	}

/*
	XTL::SocketAddressInet sa("77.88.21.11");

	printf("%s\n", sa.Host().c_str());

	return 0;

	XTL::TcpSocket socket;

	socket.Create();
	socket.SetBlocking(false);

	printf("%d\n", socket.GetBlocking());
*/
	return 0;

	/*
	int serverSocket = ::socket(PF_INET, SOCK_STREAM, 0);

	if (serverSocket < 0)
	{
		fprintf(stderr, "Error: socket()\n");
		return 1;
	}

	struct sockaddr_in serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(SERVER_PORT);

	if (::bind(serverSocket, reinterpret_cast<struct sockaddr *>(&serverAddr), sizeof(serverAddr)) < 0)
	{
		fprintf(stderr, "Error: bind()\n");
		return 1;
	}

	if (::listen(serverSocket, SERVER_BACKLOG) < 0)
	{
		fprintf(stderr, "Error: listen()\n");
		return 1;
	}

	fprintf(stdout, "Server started\n");

	getc(stdin);

	::close(serverSocket);
	*/
}
