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

		if (errno == EINPROGRESS || errno == EALREADY)
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

	ClientSocket::ReceiveResult ClientSocket::Receive(void * buffer, int size) const
	{
		int result = ::recv(Desc(), buffer, size, 0);

		if (result > 0)
		{
			return ReceiveResult(result);
		}
		else if (result == 0)
		{
			return ReceiveResult(-1);
		}

		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			return ReceiveResult(0);
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

	int ClientSocket::Send(const void * buffer, int size) const
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

