#ifndef XTL__SOCKET_ADDRESS_UNIX_HPP__
#define XTL__SOCKET_ADDRESS_UNIX_HPP__ 1

#include <sys/un.h>

#include <string>

#include "SocketAddress.hpp"

namespace XTL
{
	class SocketAddressUnix : public SocketAddress
	{
		public:

			SocketAddressUnix();

			explicit SocketAddressUnix(const std::string & path);

			virtual ~SocketAddressUnix() throw();

			virtual const ::sockaddr * SockAddr() const;

			virtual int Size() const;

			const std::string Path() const;

			void Unlink();

		private:

			sockaddr_un address_;
	};
}

#endif

