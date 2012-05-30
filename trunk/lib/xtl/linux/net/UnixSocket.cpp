#include "UnixSocket.hpp"

#include <sys/types.h>
#include <sys/socket.h>

#include "../UnixError.hpp"

namespace XTL
{
	const UnixSocket UnixSocket::Create()
	{
		int desc = ::socket(PF_UNIX, SOCK_STREAM, 0);

		if (desc == -1)
		{
			throw UnixError();
		}

		return UnixSocket(desc);
	}

	const UnixSocket UnixSocket::Create(bool blocking)
	{
		UnixSocket socket = Create();
		socket.SetBlocking(blocking);
		return socket;
	}
}

