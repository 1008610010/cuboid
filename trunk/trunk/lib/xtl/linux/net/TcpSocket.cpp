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

	void TcpSocket::Close()
	{
		if (fd_ < 0)
		{
			return;
		}

		if (::close(fd_) != -1)
		{
			fd_ = -1;
			return;
		}

		if (errno == EINTR)
		{
			throw UnixError::Interrupted();
		}
		else
		{
			fd_ = -1;
			throw UnixError();
		}
	}

	bool TcpSocket::IsBlocking() const
	{
		int flags = ::fcntl(fd_, F_GETFL, 0);

		if (flags == -1)
		{
			throw UnixError();
		}

		return (flags & O_NONBLOCK) == 0;
	}

	void TcpSocket::SetBlocking(bool blocking)
	{
		int flags = ::fcntl(fd_, F_GETFL, 0);

		if (flags == -1)
		{
			throw UnixError();
		}

		if (blocking)
		{
			flags &= ~O_NONBLOCK;
		}
		else
		{
			flags |= O_NONBLOCK;
		}

		if (::fcntl(fd_, F_SETFL, flags) == -1)
		{
			throw UnixError();
		}
	}
}

