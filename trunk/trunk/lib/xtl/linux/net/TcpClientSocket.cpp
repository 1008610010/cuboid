#include "TcpClientSocket.hpp"

#include <sys/types.h>
#include <sys/socket.h>

#include "SocketAddressInet.hpp"
#include "../UnixError.hpp"

namespace XTL
{
	bool TcpClientSocket::Connect(const SocketAddressInet & address)
	{
		if (::connect(fd_, address.SockAddr(), address.Size()) == 0)
		{
			return true;
		}

		if (errno == EINPROGRESS)
		{
			return false;
		}
		else if (errno == EINTR)
		{
			throw UnixError::Interrupted();
		}
		else
		{
			throw UnixError();
		}
	}

	int TcpClientSocket::Receive(void * buffer, int size)
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
			throw UnixError::Interrupted();
		}
		else
		{
			throw UnixError();
		}
	}

	int TcpClientSocket::Send(const void * buffer, int size)
	{
		int result = ::send(fd_, buffer, size, MSG_NOSIGNAL);

		if (result != -1)
		{
			return result;
		}

		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			return 0;
		}
		else if (errno == EINTR)
		{
			throw UnixError::Interrupted();
		}
		else
		{
			throw UnixError();
		}
	}
}

