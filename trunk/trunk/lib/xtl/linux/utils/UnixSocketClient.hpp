#ifndef XTL__UNIX_SOCKET_CLIENT_HPP__
#define XTL__UNIX_SOCKET_CLIENT_HPP__ 1

#include <string>

#include "../net/SocketAddressUnix.hpp"
#include "../net/UnixClientSocket.hpp"

namespace XTL
{
	class UnixSocketClient
	{
		public:

			explicit UnixSocketClient(const std::string & unixSocketPath);

			void Connect();

			void Send(const void * buffer, unsigned int size);

		private:

			SocketAddressUnix serverAddress_;
			UnixClientSocket  socket_;
	};
}

#endif

