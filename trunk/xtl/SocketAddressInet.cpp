#include "SocketAddressInet.hpp"
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "Exception.hpp"

#define XTL_USE_GETADDRINFO     1
#define XTL_USE_GETHOSTBYNAME_R 0

namespace XTL
{
	SocketAddressInet::SocketAddressInet()
	{
		Clear();
		address_.sin_addr.s_addr = ::htonl(INADDR_NONE);
		address_.sin_port = 0;
	}

	SocketAddressInet::SocketAddressInet(int port)
	{
		Clear();
		address_.sin_addr.s_addr = ::htonl(INADDR_ANY);
		address_.sin_port = ::htons(port);
	}

	SocketAddressInet::SocketAddressInet(const char * host)
	{
		Assign(host, 0);
	}

	SocketAddressInet::SocketAddressInet(const std::string & host)
	{
		Assign(host.c_str(), 0);
	}

	SocketAddressInet::SocketAddressInet(const char * host, int port)
	{
		Assign(host, port);
	}

	SocketAddressInet::SocketAddressInet(const std::string & host, int port)
	{
		Assign(host.c_str(), port);
	}

	SocketAddressInet::SocketAddressInet(const struct ::sockaddr_in & addr)
	{
		if (addr.sin_family != AF_INET)
		{
			throw SocketAddressError("Invalid socket family");
		}

		::memcpy(&address_, &addr, sizeof(addr));
	}

	void SocketAddressInet::Clear()
	{
		::memset(&address_, 0, sizeof(address_));
		address_.sin_family = AF_INET;
	}

	void SocketAddressInet::Assign(int port)
	{
		Clear();
		address_.sin_addr.s_addr = ::htonl(INADDR_ANY);
		address_.sin_port = ::htons(port);
	}

	void SocketAddressInet::Assign(const char * host)
	{
		Assign(host, 0);
	}

	void SocketAddressInet::Assign(const std::string & host)
	{
		Assign(host.c_str(), 0);
	}

	void SocketAddressInet::Assign(const std::string & host, int port)
	{
		Assign(host.c_str(), port);
	}

	void SocketAddressInet::Assign(const char * host, int port)
	{
		Clear();

#if XTL_USE_GETADDRINFO
		struct ::addrinfo hints;
		struct ::addrinfo * result;

		::memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = 0;
		hints.ai_protocol = 0;
		hints.ai_flags = 0;

		char portString[16];
		::sprintf(portString, "%d", port);

		int error = ::getaddrinfo(host, portString, &hints, &result);
		if (error != 0)
		{
			throw SocketAddressError(error, std::string("getaddrinfo(): ") + ::gai_strerror(error));
		}

		for (struct ::addrinfo * current = result; current != NULL; current = current->ai_next)
		{
			if (current->ai_family == AF_INET)
			{
				::memcpy(&address_, current->ai_addr, current->ai_addrlen);
				::freeaddrinfo(result);
				return;
			}
		}
		::freeaddrinfo(result);

		throw SocketAddressError("Host not found");
#else
		struct ::hostent * hostEntry = NULL;
#if XTL_USE_GETHOSTBYNAME_R
		struct ::hostent hostData;
		char buffer[2048];
		int error = 0;
		if (::gethostbyname_r(host, &hostData, buffer, sizeof(buffer), &hostEntry, &error) != 0)
		{
			throw SocketAddressError(error, std::string("gethostbyname_r(): ") + ::hstrerror(error));
		}
#else
		hostEntry = ::gethostbyname(host);
		if (hostEntry == NULL || hostEntry->h_addrtype != AF_INET)
		{
			throw SocketAddressError(h_errno, std::string("gethostbyname(): ") + ::hstrerror(h_errno));
		}
#endif
		::memcpy(&address_.sin_addr, hostEntry->h_addr_list[0], hostEntry->h_length);
		address_.sin_port = ::htons(port);
/*
		for (int i = 0; hostEntry->h_addr_list[i] != NULL; ++i)
		{
			printf("%08x\n", *((unsigned int *) (hostEntry->h_addr_list[i])));
		}
*/
#endif
	}

	int SocketAddressInet::Family() const
	{
		return AF_INET;
	}

	const std::string SocketAddressInet::Host() const
	{
		char ipaddr[INET_ADDRSTRLEN];
		return ::inet_ntop(AF_INET, &(address_.sin_addr.s_addr), ipaddr, sizeof(ipaddr)) == NULL
		       ? std::string()
		       : std::string(ipaddr);
	}

	int SocketAddressInet::Port() const
	{
		return ::ntohs(address_.sin_port);
	}

	const ::sockaddr * SocketAddressInet::Data() const
	{
		return reinterpret_cast<const sockaddr *>(&address_);
	}

	int SocketAddressInet::Size() const
	{
		return sizeof(address_);
	}

	bool SocketAddressInet::IsAny() const
	{
		return address_.sin_addr.s_addr == ::htonl(INADDR_ANY);
	}

	bool SocketAddressInet::IsNone() const
	{
		return address_.sin_addr.s_addr == ::htonl(INADDR_NONE);
	}

	const std::string SocketAddressInet::AsString() const
	{
		char portString[16];
		sprintf(portString, "%d", Port());
		return Host() + ":" + portString;
	}
}

