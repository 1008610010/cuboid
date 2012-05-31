#ifndef XTL__CLIENT_SOCKET_HPP__
#define XTL__CLIENT_SOCKET_HPP__ 1

#include "Socket.hpp"

namespace XTL
{
	class SocketAddress;

	class ClientSocket : public Socket
	{
		public:

			ClientSocket()
				: Socket()
			{
				;;
			}

			ClientSocket(Socket other)
				: Socket(other)
			{
				;;
			}

			int Receive(void * buffer, int size) const;

			int Send(const void * buffer, int size) const;

		protected:

			bool Connect(const SocketAddress & address);
	};
}

#endif

