#include "ClientSocket.hpp"

#include <sys/types.h>
#include <sys/socket.h>

#include "../UnixError.hpp"
#include "SocketAddress.hpp"

namespace XTL
{
	bool ClientSocket::Connect(const SocketAddress & address)
	{
		if (::connect(Desc(), address.SockAddr(), address.Size()) == 0)
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

	int ClientSocket::Receive(void * buffer, int size)
	{
		int result = ::recv(Desc(), buffer, size, 0);

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

	int ClientSocket::Send(const void * buffer, int size)
	{
		int result = ::send(Desc(), buffer, size, MSG_NOSIGNAL);

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

