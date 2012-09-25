#ifndef XTL__CLIENT_SOCKET_HPP__
#define XTL__CLIENT_SOCKET_HPP__ 1

#include "Socket.hpp"

namespace XTL
{
	class SocketAddress;

	class ClientSocket : public Socket
	{
		public:

			class ReceiveResult
			{
				public:

					explicit ReceiveResult(int wasRead)
						: wasRead_(wasRead)
					{
						;;
					}

					int WasRead() const
					{
						return wasRead_;
					}

					bool IsConnectionClosed() const
					{
						return wasRead_ < 0;
					}

				private:

					const int wasRead_;
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

			ReceiveResult Receive(void * buffer, int size) const;

			int Send(const void * buffer, int size) const;

		protected:

			bool Connect(const SocketAddress & address);
	};
}

#endif

