#include "TcpSocket.hpp"

#include <sys/types.h>
#include <sys/socket.h>

#include "../UnixError.hpp"

namespace XTL
{
	const TcpSocket TcpSocket::Create()
	{
		int desc = ::socket(PF_INET, SOCK_STREAM, 0);
		if (desc == -1)
		{
			throw UnixError();
		}

		return TcpSocket(desc);
	}

	const TcpSocket TcpSocket::Create(bool blocking)
	{
		TcpSocket socket = Create();
		socket.SetBlocking(blocking);
		return socket;
	}
}

