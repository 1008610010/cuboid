#include "ServerSocket.hpp"

#include <sys/types.h>
#include <sys/socket.h>

#include "../UnixError.hpp"
#include "SocketAddress.hpp"

namespace XTL
{
	void ServerSocket::SetReuseAddr(bool value)
	{
		int on = (value ? 1 : 0);

		if (::setsockopt(Desc(), SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) != 0)
		{
			throw UnixError();
		}
	}

	void ServerSocket::Bind(const SocketAddress & address)
	{
		if (::bind(Desc(), address.SockAddr(), address.Size()) == 0)
		{
			return;
		}

		if (errno == EEXIST)
		{
			throw UnixError::AlreadyExists();
		}

		throw UnixError();
	}

	void ServerSocket::Listen(int backlog)
	{
		if (::listen(Desc(), backlog) == 0)
		{
			return;
		}

		throw UnixError();
	}

	ClientSocket ServerSocket::Accept()
	{
		int clientDesc = ::accept(Desc(), NULL, 0);

		if (clientDesc != -1)
		{
			return ClientSocket(Socket(clientDesc));
		}

		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			return ClientSocket();
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

