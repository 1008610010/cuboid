#ifndef XTL__UNIX_CLIENT_SOCKET_HPP__
#define XTL__UNIX_CLIENT_SOCKET_HPP__ 1

#include "ClientSocket.hpp"
#include "SocketAddressUnix.hpp"
#include "UnixSocket.hpp"

namespace XTL
{
	class UnixClientSocket : public ClientSocket
	{
		public:

			static UnixClientSocket Create(bool blocking)
			{
				return UnixSocket::Create(blocking);
			}

			UnixClientSocket()
				: ClientSocket()
			{
				;;
			}

			UnixClientSocket(UnixSocket other)
				: ClientSocket(other)
			{
				;;
			}

			bool Connect(const SocketAddressUnix & address)
			{
				return ClientSocket::Connect(address);
			}

		protected:

			friend class UnixServerSocket;

			UnixClientSocket(ClientSocket other)
				: ClientSocket(other)
			{
				;;
			}
	};
}

#endif

