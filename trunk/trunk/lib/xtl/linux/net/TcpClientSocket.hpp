#ifndef XTL__TCP_CLIENT_SOCKET_HPP__
#define XTL__TCP_CLIENT_SOCKET_HPP__ 1

#include "TcpSocket.hpp"

namespace XTL
{
	class SocketAddressInet;

	class TcpClientSocket : public TcpSocket
	{
		public:

			static TcpClientSocket Create()
			{
				return TcpSocket::Create();
			}

			static TcpClientSocket Create(bool blocking)
			{
				return TcpSocket::Create(blocking);
			}

			TcpClientSocket()
				: TcpSocket()
			{
				;;
			}

			TcpClientSocket(TcpSocket other)
				: TcpSocket(other)
			{
				;;
			}

			bool Connect(const SocketAddressInet & address);

			int Receive(void * buffer, int size);

			int Send(const void * buffer, int size);
	};
}

#endif

