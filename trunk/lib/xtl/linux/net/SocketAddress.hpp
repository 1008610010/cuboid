#ifndef XTL__SOCKET_ADDRESS_HPP__
#define XTL__SOCKET_ADDRESS_HPP__ 1

#include <arpa/inet.h>

namespace XTL
{
	class SocketAddress
	{
		public:

			virtual ~SocketAddress() throw()
			{
				;;
			}

			virtual const ::sockaddr * SockAddr() const = 0;

			virtual int Size() const = 0;
	};
}

#endif

