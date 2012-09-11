#include "TcpSocketServer.hpp"

#include "../net/SocketAddressInet.hpp"
#include "../net/TcpServerSocket.hpp"

namespace XTL
{
	TcpSocketServer::TcpSocketServer(double selectTimeout)
		: SocketServer(selectTimeout)
	{
		;;
	}

	TcpSocketServer::~TcpSocketServer() throw()
	{
		;;
	}

	void TcpSocketServer::Listen(unsigned int listenPort, int listenBacklog)
	{
		SocketAddressInet serverAddress(listenPort);

		TcpServerSocket serverSocket(TcpServerSocket::Create(false, true));
		serverSocket.Bind(serverAddress);
		serverSocket.Listen(listenBacklog);

		AddListeningSocket(serverSocket);
	}
}
