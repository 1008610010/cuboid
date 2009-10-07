#ifndef _XTL__TCP_SOCKET_HPP__
#define _XTL__TCP_SOCKET_HPP__ 1

#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "Exception.hpp"
#include "SocketAddressInet.hpp"

namespace XTL
{
	class SocketAddressInet;

	class TcpSocket
	{
		public:

			TcpSocket();

			explicit TcpSocket(int fd);

			~TcpSocket() throw();

			operator int() const
			{
				return fd_;
			}

			/**
			 * @throw SocketError
			 */
			TcpSocket & Create(bool blocking = true);

			bool Created() const;

			void Close();

			bool GetBlocking() const;

			bool SetBlocking(bool blocking);

			int Error() const;

			const SocketAddressInet LocalAddress() const;

			const SocketAddressInet RemoteAddress() const;

			// TODO: make template by _SocketAddress
			TcpSocket & Bind(const SocketAddressInet & address);

			TcpSocket & Bind(int port);

			TcpSocket & Bind(const char * host);

			TcpSocket & Bind(const std::string & host);

			TcpSocket & Bind(const char * host, int port);

			TcpSocket & Bind(const std::string & host, int port);

			bool Connect(const SocketAddressInet & address);

			bool Connect(const char * host, int port);

			bool Connect(const std::string & host, int port);

			TcpSocket & Listen(int backlog);

			TcpSocket & Listen(const SocketAddressInet & address, int backlog);

			TcpSocket & Listen(int port, int backlog);

			TcpSocket & Listen(const char * host, int port, int backlog);

			TcpSocket & Listen(const std::string & host, int port, int backlog);

			int Receive(void * buffer, int size);

			int Send(const void * buffer, int size);

		protected:

			int fd_;
	};
}

#endif

