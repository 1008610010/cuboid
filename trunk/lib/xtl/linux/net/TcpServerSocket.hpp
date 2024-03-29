#ifndef XTL__TCP_SERVER_SOCKET_HPP__
#define XTL__TCP_SERVER_SOCKET_HPP__ 1

#include "ServerSocket.hpp"
#include "TcpClientSocket.hpp"

namespace XTL
{
	class SocketAddressInet;

	class TcpServerSocket : public ServerSocket
	{
		public:

			static TcpServerSocket Create(bool blocking, bool reuseAddr = false)
			{
				TcpServerSocket socket = TcpSocket::Create(blocking);
				socket.SetReuseAddr(reuseAddr);
				return socket;
			}

			TcpServerSocket()
				: ServerSocket()
			{
				;;
			}

			TcpServerSocket(TcpSocket socket)
				: ServerSocket(socket)
			{
				;;
			}

			TcpServerSocket & Bind(const SocketAddressInet & address)
			{
				ServerSocket::Bind(address);

				return *this;
			}

			TcpServerSocket & Listen(int backlog)
			{
				ServerSocket::Listen(backlog);

				return *this;
			}

			TcpClientSocket Accept()
			{
				return ServerSocket::Accept();
			}
	};
}

#endif

