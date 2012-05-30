#ifndef XTL__SERVER_SOCKET_HPP__
#define XTL__SERVER_SOCKET_HPP__ 1

#include "Socket.hpp"
#include "SocketAddress.hpp"

namespace XTL
{
	class ServerSocket : public Socket
	{
		public:

			static TcpServerSocket Create()
			{
				return TcpSocket::Create();
			}

			ServerSocket()
				: Socket()
			{
				;;
			}

			ServerSocket(Socket other)
				: Socket(other)
			{
				;;
			}

			ServerSocket & Bind(const SocketAddress & address)
			{
				if (::bind(Desc(), address.SockAddr(), address.Size()) == 0)
				{
					return *this;
				}

				if (errno == EEXIST)
				{
					throw UnixError::AlreadyExists();
				}

				throw UnixError();
			}

			ServerSocket & Listen(int backlog)
			{
				if (::listen(Desc(), backlog) == 0)
				{
					return *this;
				}

				throw UnixError();
			}

			ClientSocket Accept()
			{
				int clientDesc = ::accept(Desc(), NULL, 0);

				if (clientDesc != -1)
				{
					return ClientSocket(clientDesc);
				}

				if (errno == EAGAIN || errno == EWOULDBLOCK)
				{
					return ClientSocket();
				}
				else if (errno == EINTR)
				{
					throw UnixError::Interrupted();
				}
				else
				{
					throw UnixError();
				}
			}
	};
}

#endif

