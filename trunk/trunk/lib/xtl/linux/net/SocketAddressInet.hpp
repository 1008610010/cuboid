#ifndef XTL__SOCKET_ADDRESS_INET_HPP__
#define XTL__SOCKET_ADDRESS_INET_HPP__ 1

#include <arpa/inet.h>

#include <string>

#include "SocketAddress.hpp"

namespace XTL
{
	/**
	 * @class SocketAddressInet
	 * @brief Implementation of IPv4 internet address.
	 */
	class SocketAddressInet : public SocketAddress
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

			virtual ~SocketAddressInet() throw();

			virtual const ::sockaddr * SockAddr() const;

			virtual int Size() const;

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

			sockaddr_in address_;
	};
}

#endif

