#ifndef _XTL__SOCKET_ADDRESS_INET_HPP__
#define _XTL__SOCKET_ADDRESS_INET_HPP__ 1

#include <string>
#include <arpa/inet.h>

namespace XTL
{
	class SocketAddressInet
	{
		public:

			/**
			 * Create empty address.
			 */
			SocketAddressInet();

			/**
			 * Create address with any host and specified port.
			 */
			explicit SocketAddressInet(int port);

			/**
			 * Create address with any port and specified host.
			 */
			explicit SocketAddressInet(const char * host);

			/**
			 * Create address with any port and specified host.
			 */
			explicit SocketAddressInet(const std::string & host);

			/**
			 * Create address with specified host and port.
			 */
			SocketAddressInet(const char * host, int port);

			/**
			 * Create address with specified host and port.
			 */
			SocketAddressInet(const std::string & host, int port);

			void Assign(int port);

			void Assign(const char * host);

			void Assign(const std::string & host);

			void Assign(const char * host, int port);

			void Assign(const std::string & host, int port);

			const std::string Host() const;

			int Port() const;

			const ::sockaddr * Data() const;

			int Size() const;

			bool IsAny() const;

			bool IsNone() const;

			const std::string AsString() const;

		protected:

			SocketAddressInet(const struct ::sockaddr_in & addr);

			void Clear();

			friend class TcpSocket;

			sockaddr_in address_;
	};
}

#endif

