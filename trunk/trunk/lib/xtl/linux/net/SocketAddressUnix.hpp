#ifndef XTL__SOCKET_ADDRESS_UNIX_HPP__
#define XTL__SOCKET_ADDRESS_UNIX_HPP__ 1

#include <sys/un.h>

#include <string>

namespace XTL
{
	class SocketAddressUnix
	{
		public:

			SocketAddressUnix();

			explicit SocketAddressUnix(const std::string & path);

			void Unlink();

		private:

			const ::sockaddr * SockAddr() const;

			int Size() const;

			sockaddr_un address_;
	};
}

#endif

