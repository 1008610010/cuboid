#ifndef XTL__TCP_SOCKET_SERVER_HPP__
#define XTL__TCP_SOCKET_SERVER_HPP__ 1

#include "SocketServer.hpp"
#include "../net/TcpServerSocket.hpp"

namespace XTL
{
	class TcpSocketServer : public SocketServer
	{
		public:

			static const int DEFAULT_LISTEN_BACKLOG = 5;
			static const int DEFAULT_SELECT_TIMEOUT = 1;

			explicit TcpSocketServer(double selectTimeout = DEFAULT_SELECT_TIMEOUT)
				: SocketServer(selectTimeout)
			{
				;;
			}

			void Listen(unsigned int listenPort, int listenBacklog = DEFAULT_LISTEN_BACKLOG)
			{
				SocketAddressInet serverAddress(listenPort);

				TcpServerSocket serverSocket(TcpServerSocket::Create(false, true));
				serverSocket.Bind(serverAddress);
				serverSocket.Listen(listenBacklog);

				AddListeningSocket(serverSocket);
			}

			virtual ~TcpSocketServer() throw()
			{
				;;
			}
	};
}

#endif
