#ifndef XTL__TCP_SOCKET_HPP__
#define XTL__TCP_SOCKET_HPP__ 1

#include "Socket.hpp"

namespace XTL
{
	class TcpSocket : public Socket
	{
		public:

			static const TcpSocket Create();

			static const TcpSocket Create(bool blocking);

		protected:

			friend class TcpServerSocket;

			explicit TcpSocket(int desc)
				: Socket(desc)
			{
				;;
			}
	};
}

#endif

