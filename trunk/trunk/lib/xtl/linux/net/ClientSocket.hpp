#ifndef XTL__CLIENT_SOCKET_HPP__
#define XTL__CLIENT_SOCKET_HPP__ 1

#include "Socket.hpp"

namespace XTL
{
	class SocketAddress;

	class ClientSocket : public Socket
	{
		public:

			class Disconnected
			{
			};

			ClientSocket()
				: Socket()
			{
				;;
			}

			ClientSocket(Socket other)
				: Socket(other)
			{
				;;
			}

			/**
			 * @throw XTL::ClientSocket::Disconnected
			 * @throw XTL::UnixError::Interrupted
			 * @throw XTL::UnixError
			 */
			unsigned int Receive(void * buffer, int size) const;

			/**
			 * @throw XTL::UnixError::Interrupted
			 * @throw XTL::UnixError
			 */
			unsigned int Send(const void * buffer, int size) const;

		protected:

			bool Connect(const SocketAddress & address);
	};
}

#endif

