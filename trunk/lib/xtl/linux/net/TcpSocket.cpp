#include "TcpSocket.hpp"

#include "../UnixError.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

namespace XTL
{
	const TcpSocket TcpSocket::Create()
	{
		int fd = ::socket(PF_INET, SOCK_STREAM, 0);
		if (fd == -1)
		{
			throw UnixError();
		}

		return TcpSocket(fd);
	}

	const TcpSocket TcpSocket::Create(bool blocking)
	{
		TcpSocket socket = Create();
		socket.SetBlocking(blocking);
		return socket;
	}
}

