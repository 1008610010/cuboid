#ifndef XTL__SOCKET_SET_HPP__
#define XTL__SOCKET_SET_HPP__ 1

#include "Socket.hpp"

#include <sys/select.h>

namespace XTL
{
	class SocketSet
	{
		public:

			SocketSet();

			SocketSet(const SocketSet & other);

			SocketSet & operator= (const SocketSet & other);

			void Clear();

			bool Insert(Socket socket);

			bool Delete(Socket socket);

			bool Contains(Socket socket) const;

			static bool IsDescInvalid(int fd)
			{
				return fd < 0 || fd >= FD_SETSIZE;
			}

		private:

			friend class SocketSelector;

			bool Contains(int fd) const;

			fd_set * Raw()
			{
				return &set_;
			}

			fd_set set_;
	};
}

#endif

