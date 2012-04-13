#ifndef XTL__TCP_SOCKET_HPP__
#define XTL__TCP_SOCKET_HPP__ 1

namespace XTL
{
	class TcpSocket
	{
		public:

			TcpSocket()
				: fd_(-1)
			{
				;;
			}

			static TcpSocket Null()
			{
				static TcpSocket instance(-1);
				return instance;
			}

			bool IsNull() const
			{
				return fd_ == -1;
			}

			static const TcpSocket Create();

			static const TcpSocket Create(bool blocking);

			void Close();

			bool IsBlocking() const;

			void SetBlocking(bool blocking);

		protected:

			friend class TcpSocketSelector;
			friend class TcpSocketSet;
			friend class TcpServerSocket;

			explicit TcpSocket(int fd)
				: fd_(fd)
			{
				;;
			}

			int FileDesc() const
			{
				return fd_;
			}

			int fd_;
	};
}

#endif

