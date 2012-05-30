#ifndef XTL__CLIENT_SOCKET_HPP__
#define XTL__CLIENT_SOCKET_HPP__ 1

namespace XTL
{
	class SocketAddressInet;

	class ClientSocket : public Socket
	{
		public:

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

			bool Connect(const SocketAddress & address);

			int Receive(void * buffer, int size);

			int Send(const void * buffer, int size);
	};
}

#endif

