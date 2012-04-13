#ifndef XTL__TCP_SOCKET_SET_HPP__
#define XTL__TCP_SOCKET_SET_HPP__ 1

#include "TcpSocket.hpp"

#include <sys/select.h>

namespace XTL
{
	class TcpSocketSet
	{
		public:

			TcpSocketSet();

			TcpSocketSet(const TcpSocketSet & other);

			TcpSocketSet & operator= (const TcpSocketSet & other);

			void Clear();

			bool Insert(TcpSocket socket);

			bool Delete(TcpSocket socket);

			bool Contains(TcpSocket socket) const;

			static bool IsDescInvalid(int fd)
			{
				return fd < 0 || fd >= FD_SETSIZE;
			}

		private:

			friend class TcpSocketSelector;

			bool Contains(int fd) const;

			fd_set * Raw()
			{
				return &set_;
			}

			fd_set set_;
	};
}

#endif

