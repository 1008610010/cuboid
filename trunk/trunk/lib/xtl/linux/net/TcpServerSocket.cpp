#include "TcpServerSocket.hpp"

#include "SocketAddressInet.hpp"
#include "../UnixError.hpp"

#include <sys/types.h>
#include <sys/socket.h>

namespace XTL
{
	TcpServerSocket & TcpServerSocket::Bind(const SocketAddressInet & address)
	{
		if (::bind(Desc(), address.SockAddr(), address.Size()) == 0)
		{
			return *this;
		}

		throw UnixError();
	}

	TcpServerSocket & TcpServerSocket::Listen(int backlog)
	{
		if (::listen(Desc(), backlog) == 0)
		{
			return *this;
		}

		throw UnixError();
	}

	TcpClientSocket TcpServerSocket::Accept()
	{
		int clientDesc = ::accept(Desc(), NULL, 0);

		if (clientDesc != -1)
		{
			return TcpSocket(clientDesc);
		}

		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			return TcpSocket::Null();
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

