#include "Socket.hpp"

#include "../UnixError.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

namespace XTL
{
	void Socket::Close()
	{
		if (desc_ < 0)
		{
			return;
		}

		if (::close(desc_) != -1)
		{
			desc_ = -1;
			return;
		}

		if (errno == EINTR)
		{
			throw UnixError::Interrupted();
		}
		else
		{
			desc_ = -1;
			throw UnixError();
		}
	}

	bool Socket::IsBlocking() const
	{
		int flags = ::fcntl(desc_, F_GETFL, 0);

		if (flags == -1)
		{
			throw UnixError();
		}

		return (flags & O_NONBLOCK) == 0;
	}

	void Socket::SetBlocking(bool blocking)
	{
		int flags = ::fcntl(desc_, F_GETFL, 0);

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

		if (::fcntl(desc_, F_SETFL, flags) == -1)
		{
			throw UnixError();
		}
	}
}

