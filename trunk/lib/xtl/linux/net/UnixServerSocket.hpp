#ifndef XTL__UNIX_SERVER_SOCKET_HPP__
#define XTL__UNIX_SERVER_SOCKET_HPP__ 1

#include "ServerSocket.hpp"
#include "SocketAddressUnix.hpp"
#include "UnixClientSocket.hpp"

namespace XTL
{
	class SocketAddressInet;

	class UnixServerSocket : public ServerSocket
	{
		public:

			static UnixServerSocket Create(bool blocking)
			{
				return UnixSocket::Create(blocking);
			}

			UnixServerSocket()
				: ServerSocket()
			{
				;;
			}

			UnixServerSocket(UnixSocket socket)
				: ServerSocket(socket)
			{
				;;
			}

			UnixServerSocket & Bind(const SocketAddressUnix & address)
			{
				ServerSocket::Bind(address);

				return *this;
			}

			UnixServerSocket & Listen(int backlog)
			{
				ServerSocket::Listen(backlog);

				return *this;
			}

			UnixClientSocket Accept()
			{
				return ServerSocket::Accept();
			}
	};
}

#endif

