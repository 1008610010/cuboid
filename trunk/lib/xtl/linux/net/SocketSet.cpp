#include "SocketSet.hpp"

#include <string.h>

namespace XTL
{
	SocketSet::SocketSet()
	{
		FD_ZERO(&set_);
	}

	SocketSet::SocketSet(const SocketSet & other)
	{
		::memcpy(&set_, &other.set_, sizeof(set_));
	}

	SocketSet & SocketSet::operator= (const SocketSet & other)
	{
		if (this == &other)
		{
			return *this;
		}

		::memcpy(&set_, &other.set_, sizeof(set_));
		return *this;
	}

	void SocketSet::Clear()
	{
		FD_ZERO(&set_);
	}

	bool SocketSet::Insert(Socket socket)
	{
		if (IsDescInvalid(socket.Desc()))
		{
			return false;
		}

		FD_SET(socket.Desc(), &set_);
		return true;
	}

	bool SocketSet::Delete(Socket socket)
	{
		if (IsDescInvalid(socket.Desc()))
		{
			return false;
		}

		FD_CLR(socket.Desc(), &set_);
		return true;
	}

	bool SocketSet::Contains(int fd) const
	{
		if (IsDescInvalid(fd))
		{
			return false;
		}

		return FD_ISSET(fd, &set_);
	}

	bool SocketSet::Contains(Socket socket) const
	{
		return Contains(socket.Desc());
	}
}

