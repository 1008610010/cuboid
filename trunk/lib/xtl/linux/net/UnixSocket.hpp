#ifndef XTL__UNIX_SOCKET_HPP__
#define XTL__UNIX_SOCKET_HPP__ 1

#include "Socket.hpp"

namespace XTL
{
	class UnixSocket : public Socket
	{
		public:

			UnixSocket()
				: Socket()
			{
				;;
			}

			/*
			static UnixSocket Null()
			{
				static TcpSocket instance;

				return instance;
			}
			*/

			static const UnixSocket Create();

			static const UnixSocket Create(bool blocking);

		protected:

			// friend class TcpServerSocket;

			explicit UnixSocket(int desc)
				: Socket(desc)
			{
				;;
			}
	};
}

#endif

