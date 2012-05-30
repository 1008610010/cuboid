#include "TcpSocketSet.hpp"

#include <string.h>

namespace XTL
{
	TcpSocketSet::TcpSocketSet()
	{
		FD_ZERO(&set_);
	}

	TcpSocketSet::TcpSocketSet(const TcpSocketSet & other)
	{
		::memcpy(&set_, &other.set_, sizeof(set_));
	}

	TcpSocketSet & TcpSocketSet::operator= (const TcpSocketSet & other)
	{
		if (this == &other)
		{
			return *this;
		}

		::memcpy(&set_, &other.set_, sizeof(set_));
		return *this;
	}

	void TcpSocketSet::Clear()
	{
		FD_ZERO(&set_);
	}

	bool TcpSocketSet::Insert(TcpSocket socket)
	{
		if (IsDescInvalid(socket.Desc()))
		{
			return false;
		}

		FD_SET(socket.Desc(), &set_);
		return true;
	}

	bool TcpSocketSet::Delete(TcpSocket socket)
	{
		if (IsDescInvalid(socket.Desc()))
		{
			return false;
		}

		FD_CLR(socket.Desc(), &set_);
		return true;
	}

	bool TcpSocketSet::Contains(int fd) const
	{
		if (IsDescInvalid(fd))
		{
			return false;
		}

		return FD_ISSET(fd, &set_);
	}

	bool TcpSocketSet::Contains(TcpSocket socket) const
	{
		return Contains(socket.Desc());
	}
}

