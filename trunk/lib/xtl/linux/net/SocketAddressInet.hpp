#ifndef XTL__SOCKET_ADDRESS_INET_HPP__
#define XTL__SOCKET_ADDRESS_INET_HPP__ 1

#include "SocketAddressError.hpp"
#include "../UnixError.hpp"

#include <string>

#include <arpa/inet.h>

namespace XTL
{
	/**
	 * @class SocketAddressInet
	 * @brief Implementation of IPv4 internet address.
	 */
	class SocketAddressInet
	{
		public:

			SocketAddressInet();

			/**
			 * @brief Create address with any host and specified port.
			 * @param port  Port of address.
			 */
			explicit SocketAddressInet(int port);

			/**
			 * @throw SocketAddressError
			 */
			SocketAddressInet(const char * host, int port);

			/**
			 * @throw SocketAddressError
			 */
			SocketAddressInet(const std::string & host, int port);

			/**
			 * @throw UnixError
			 */
			const std::string Host() const;

			int Port() const;

			/**
			 * @throw SocketAddressError
			 */
			void Set(const char * host, int port);

		private:

			friend class TcpClientSocket;
			friend class TcpServerSocket;

			const ::sockaddr * SockAddr() const;

			int Size() const;

			sockaddr_in address_;
	};
}

#endif

