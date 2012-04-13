#ifndef XTL__TCP_SERVER_SOCKET_HPP__
#define XTL__TCP_SERVER_SOCKET_HPP__ 1

#include "TcpSocket.hpp"
#include "TcpClientSocket.hpp"

namespace XTL
{
	class SocketAddressInet;

	class TcpServerSocket : public TcpSocket
	{
		public:

			static TcpServerSocket Create()
			{
				return TcpSocket::Create();
			}

			static TcpServerSocket Create(bool blocking)
			{
				return TcpSocket::Create(blocking);
			}

			TcpServerSocket()
				: TcpSocket()
			{
				;;
			}

			TcpServerSocket(TcpSocket other)
				: TcpSocket(other)
			{
				;;
			}

			/*
				TODO: move it to parent class (TcpSocket)
			 */
			TcpServerSocket & Bind(const SocketAddressInet & address);

			TcpServerSocket & Listen(int backlog);

			TcpClientSocket Accept();
	};
}

#endif

