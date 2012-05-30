#ifndef XTL__TCP_SERVER_SOCKET_HPP__
#define XTL__TCP_SERVER_SOCKET_HPP__ 1

#include "ServerSocket.hpp"
#include "TcpClientSocket.hpp"
#include "SocketAddressInet.hpp"

namespace XTL
{
	class SocketAddressInet;

	class TcpServerSocket : public ServerSocket
	{
		public:

			static TcpServerSocket Create(bool blocking)
			{
				return TcpSocket::Create(blocking);
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

