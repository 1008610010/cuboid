#include "TcpSocketClient.hpp"

#include "../net/TcpClientSocket.hpp"

namespace XTL
{
	TcpSocketClient::TcpSocketClient(const std::string & host, unsigned int port)
		: serverAddress_(host, port),
		  clientSocket_(TcpClientSocket::Create(true))
	{
		;;
	}

	void TcpSocketClient::Connect()
	{
		clientSocket_.Connect(serverAddress_);
	}

	void TcpSocketClient::Send(const void * buffer, unsigned int size)
	{
		if (size == 0)
		{
			return;
		}

		clientSocket_.Send(buffer, size);
	}
}

