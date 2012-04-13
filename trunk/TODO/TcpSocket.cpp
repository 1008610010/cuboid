#include "TcpSocket.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "SocketAddressInet.hpp"

namespace XTL
{
	TcpSocket::TcpSocket()
		: fd_(-1)
	{
		;;
	}

	TcpSocket::TcpSocket(int fd)
		: fd_(fd)
	{
		;;
	}

	TcpSocket::~TcpSocket() throw()
	{
		;;
	}

	TcpSocket & TcpSocket::Create(bool blocking)
	{
		fd_ = ::socket(PF_INET, SOCK_STREAM, 0);

		if (fd_ < 0)
		{
			throw SocketError(errno, std::string("socket(): ") + strerror(errno));
		}

		SetBlocking(blocking);
		return *this;
	}

	bool TcpSocket::Created() const
	{
		return fd_ >= 0;
	}

	void TcpSocket::Close()
	{
		if (fd_ >= 0)
		{
			if (::close(fd_) < 0)
			{
				if (errno == EINTR)
				{
					throw InterruptError("close()");
				}
				else
				{
					fd_ = -1;
					throw SocketError(errno, std::string("close(): ") + strerror(errno));
				}
			}
			fd_ = -1;
		}
	}

	bool TcpSocket::GetBlocking() const
	{
		return (::fcntl(fd_, F_GETFL, 0) & O_NONBLOCK) == 0;
	}

	bool TcpSocket::SetBlocking(bool blocking)
	{
		if (blocking)
		{
			return ::fcntl(fd_, F_SETFL, ::fcntl(fd_, F_GETFL, 0) & ~O_NONBLOCK) >= 0;
		}
		else
		{
			return ::fcntl(fd_, F_SETFL, ::fcntl(fd_, F_GETFL, 0) | O_NONBLOCK) >= 0;
		}
	}

	int TcpSocket::Error() const
	{
		int result = 0;
		::socklen_t length = sizeof(result);
		return ::getsockopt(fd_, SOL_SOCKET, SO_ERROR, &result, &length) < 0 ? -1 : result;
	}

	const SocketAddressInet TcpSocket::LocalAddress() const
	{
		struct ::sockaddr_in addr;
		::socklen_t addrlen = sizeof(addr);
		if (::getsockname(fd_, reinterpret_cast<struct ::sockaddr *>(&addr), &addrlen) < 0)
		{
			throw SocketError(errno, std::string("getsockname(): ") + strerror(errno));
		}

		if (addr.sin_family != AF_INET)
		{
			throw SocketError("Internal socket error");
		}

		return SocketAddressInet(addr);
	}

	const SocketAddressInet TcpSocket::RemoteAddress() const
	{
		struct ::sockaddr_in addr;
		::socklen_t addrlen = sizeof(addr);
		if (::getpeername(fd_, reinterpret_cast<struct ::sockaddr *>(&addr), &addrlen) < 0)
		{
			throw SocketError(errno, std::string("getpeername(): ") + strerror(errno));
		}

		if (addr.sin_family != AF_INET)
		{
			throw SocketError("Internal socket error");
		}

		return SocketAddressInet(addr);
	}

	TcpSocket & TcpSocket::Bind(const SocketAddressInet & address)
	{
		if (::bind(fd_, address.Data(), address.Size()) < 0)
		{
			throw SocketError(errno, std::string("bind(): ") + strerror(errno));
		}
		return *this;
	}

	TcpSocket & TcpSocket::Bind(int port)
	{
		return Bind(SocketAddressInet(port));
	}

	TcpSocket & TcpSocket::Bind(const char * host)
	{
		return Bind(SocketAddressInet(host));
	}

	TcpSocket & TcpSocket::Bind(const std::string & host)
	{
		return Bind(SocketAddressInet(host));
	}

	TcpSocket & TcpSocket::Bind(const char * host, int port)
	{
		return Bind(SocketAddressInet(host, port));
	}

	TcpSocket & TcpSocket::Bind(const std::string & host, int port)
	{
		return Bind(SocketAddressInet(host, port));
	}

	bool TcpSocket::Connect(const SocketAddressInet & address)
	{
		if (::connect(fd_, address.Data(), address.Size()) < 0)
		{
			if (errno == EINPROGRESS)
			{
				return false;
			}
			else if (errno == EINTR)
			{
				throw InterruptError("connect()");
			}
			else
			{
				Close();
				throw SocketError(errno, std::string("connect(): ") + strerror(errno));
			}
		}

		return true;
	}

	bool TcpSocket::Connect(const char * host, int port)
	{
		return Connect(SocketAddressInet(host, port));
	}

	bool TcpSocket::Connect(const std::string & host, int port)
	{
		return Connect(SocketAddressInet(host, port));
	}

	TcpSocket & TcpSocket::Listen(int backlog)
	{
		if (::listen(fd_, backlog) < 0)
		{
			throw SocketError(errno, std::string("listen(): ") + strerror(errno));
		}
		return *this;
	}

	TcpSocket & TcpSocket::Listen(const SocketAddressInet & address, int backlog)
	{
		return Bind(address).Listen(backlog);
	}

	TcpSocket & TcpSocket::Listen(int port, int backlog)
	{
		return Bind(port).Listen(backlog);
	}

	TcpSocket & TcpSocket::Listen(const char * host, int port, int backlog)
	{
		return Bind(host, port).Listen(backlog);
	}

	TcpSocket & TcpSocket::Listen(const std::string & host, int port, int backlog)
	{
		return Bind(host, port).Listen(backlog);
	}

	int TcpSocket::Receive(void * buffer, int size)
	{
		int result = ::recv(fd_, buffer, size, 0);

		if (result > 0)
		{
			return result;
		}
		else if (result == 0)
		{
			return -1;
		}

		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			return 0;
		}
		else if (errno == EINTR)
		{
			throw InterruptError("recv()");
		}

		throw SocketError(errno, std::string("recv(): ") + strerror(errno));
	}

	int TcpSocket::Send(const void * buffer, int size)
	{
		int result = ::send(fd_, buffer, size, MSG_NOSIGNAL);
		if (result >= 0)
		{
			return result;
		}

		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			return 0;
		}
		else if (errno == EINTR)
		{
			throw InterruptError("send()");
		}

		throw SocketError(errno, std::string("send(): ") + strerror(errno));
	}
}

