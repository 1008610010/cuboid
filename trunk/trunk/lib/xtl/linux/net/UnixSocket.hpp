#ifndef XTL__UNIX_SOCKET_HPP__
#define XTL__UNIX_SOCKET_HPP__ 1

#include "Socket.hpp"

namespace XTL
{
	class UnixSocket : public Socket
	{
		public:

			static const UnixSocket Create();

			static const UnixSocket Create(bool blocking);

		protected:

			explicit UnixSocket(int desc)
				: Socket(desc)
			{
				;;
			}
	};
}

#endif

