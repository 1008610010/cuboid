#ifndef XTL__TCP_SOCKET_CLIENT_HPP__
#define XTL__TCP_SOCKET_CLIENT_HPP__ 1

#include <string>

#include "../net/SocketAddressInet.hpp"
#include "../net/TcpClientSocket.hpp"

namespace XTL
{
	class TcpSocketClient
	{
		public:

			/**
			 * @throw SocketAddressError
			 */
			explicit TcpSocketClient(const std::string & host, unsigned int port);

			void SetBlocking(bool blocking);

			void Connect();

			void Send(const void * buffer, unsigned int size);

		private:

			SocketAddressInet serverAddress_;
			TcpClientSocket   clientSocket_;
	};
}

#endif

