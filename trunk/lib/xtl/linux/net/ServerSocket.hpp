#ifndef XTL__SERVER_SOCKET_HPP__
#define XTL__SERVER_SOCKET_HPP__ 1

#include "Socket.hpp"
#include "ClientSocket.hpp"

namespace XTL
{
	class SocketAddress;

	class ServerSocket : public Socket
	{
		public:

			ServerSocket()
				: Socket()
			{
				;;
			}

			ServerSocket(Socket other)
				: Socket(other)
			{
				;;
			}

			void SetReuseAddr(bool value);

			ClientSocket Accept();

		protected:

			void Bind(const SocketAddress & address);

			void Listen(int backlog);
	};
}

#endif

