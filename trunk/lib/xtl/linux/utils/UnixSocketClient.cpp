#include "UnixSocketClient.hpp"

#include "../net/UnixSocket.hpp"

namespace XTL
{
	UnixSocketClient::UnixSocketClient(const std::string & unixSocketPath)
		: serverAddress_(unixSocketPath),
		  socket_(UnixSocket::Create(true))
	{
		;;
	}

	void UnixSocketClient::Connect()
	{
		socket_.Connect(serverAddress_);
	}

	void UnixSocketClient::Send(const void * buffer, unsigned int size)
	{
		if (size == 0)
		{
			return;
		}

		socket_.Send(buffer, size);
	}
}

