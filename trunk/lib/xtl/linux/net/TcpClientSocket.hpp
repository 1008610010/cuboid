#ifndef XTL__TCP_CLIENT_SOCKET_HPP__
#define XTL__TCP_CLIENT_SOCKET_HPP__ 1

#include "ClientSocket.hpp"
#include "SocketAddressInet.hpp"
#include "TcpSocket.hpp"

namespace XTL
{
	class TcpClientSocket : public ClientSocket
	{
		public:

			static TcpClientSocket Create(bool blocking)
			{
				return TcpSocket::Create(blocking);
			}

			TcpClientSocket()
				: ClientSocket()
			{
				;;
			}

			bool Connect(const SocketAddressInet & address)
			{
				return ClientSocket::Connect(address);
			}

		protected:

			friend class TcpServerSocket;

			TcpClientSocket(TcpSocket other)
				: ClientSocket(other)
			{
				;;
			}

			TcpClientSocket(ClientSocket other)
				: ClientSocket(other)
			{
				;;
			}
	};
}

#endif

