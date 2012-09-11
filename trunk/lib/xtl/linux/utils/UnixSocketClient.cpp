#include "UnixSocketClient.hpp"

#include "../net/UnixClientSocket.hpp"

namespace XTL
{
	UnixSocketClient::UnixSocketClient(const std::string & unixSocketPath)
		: serverAddress_(unixSocketPath),
		  clientSocket_(UnixClientSocket::Create(true))
	{
		;;
	}

	void UnixSocketClient::Connect()
	{
		clientSocket_.Connect(serverAddress_);
	}

	void UnixSocketClient::Send(const void * buffer, unsigned int size)
	{
		if (size == 0)
		{
			return;
		}

		clientSocket_.Send(buffer, size);
	}
}

