#include "SocketAddressUnix.hpp"

#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include "../../Exception.hpp"

namespace XTL
{
	namespace
	{
		void Init(sockaddr_un & address)
		{
			::memset(&address, 0, sizeof(address));
			address.sun_family = AF_UNIX;
		}
	}

	SocketAddressUnix::SocketAddressUnix()
	{
		Init(address_);
	}

	SocketAddressUnix::SocketAddressUnix(const std::string & path)
	{
		Init(address_);

		if (path.length() < sizeof(address_.sun_path))
		{
			::strcpy(address_.sun_path, path.c_str());
		}
		else
		{
			throw ILLEGAL_ARGUMENT_ERROR("Too big path to unix socket");
		}
	}

	SocketAddressUnix::~SocketAddressUnix() throw()
	{
		;;
	}

	const ::sockaddr * SocketAddressUnix::SockAddr() const
	{
		return reinterpret_cast<const sockaddr *>(&address_);
	}

	int SocketAddressUnix::Size() const
	{
		return ::strlen(address_.sun_path) + sizeof(address_.sun_family);
	}

	const std::string SocketAddressUnix::Path() const
	{
		return std::string(address_.sun_path);
	}

	void SocketAddressUnix::Unlink()
	{
		if (address_.sun_path[0] != '\0')
		{
			::unlink(address_.sun_path);
		}
	}
}
