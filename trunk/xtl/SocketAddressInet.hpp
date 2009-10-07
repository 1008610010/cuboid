#ifndef _XTL__SOCKET_ADDRESS_INET_HPP__
#define _XTL__SOCKET_ADDRESS_INET_HPP__ 1

#include <string>
#include <arpa/inet.h>

/**
 * @namespace XTL Extended Template Library
 */
namespace XTL
{
	/**
	 * @class SocketAddressInet
	 * @brief Implementation of IPv4 internet address.
	 */
	class SocketAddressInet
	{
		public:

			/**
			 * @brief Default constructor.
			 * @details Create empty address: IsNone() && Port() == 0
			 */
			SocketAddressInet();

			/**
			 * @brief Create address with any host and specified port.
			 * @param port Port of address.
			 */
			explicit SocketAddressInet(int port);

			/**
			 * @brief Create address with any port and specified host.
			 * @param host Zero-ended IP address in dot-decimal notation or domain name.
			 */
			explicit SocketAddressInet(const char * host);

			/**
			 * @brief Create address with any port and specified host.
			 * @param host IP address in dot-decimal notation or domain name.
			 */
			explicit SocketAddressInet(const std::string & host);

			/**
			 * @brief Create address with specified host and port.
			 * @param host Zero-ended IP address in dot-decimal notation or domain name.
			 * @param port Port of address.
			 */
			SocketAddressInet(const char * host, int port);

			/**
			 * @brief Create address with specified host and port.
			 * @param host IP address in dot-decimal notation or domain name.
			 * @param port Port of address.
			 */
			SocketAddressInet(const std::string & host, int port);

			void Assign(int port);

			void Assign(const char * host);

			void Assign(const std::string & host);

			void Assign(const char * host, int port);

			void Assign(const std::string & host, int port);

			int Family() const;

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

