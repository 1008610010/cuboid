#include "SocketAddressInet.hpp"

#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

#include "../../FormatString.hpp"

namespace XTL
{
	namespace
	{
		class AddrInfoList
		{
			public:

				AddrInfoList(const char * host, int port)
					: addrInfoList_(0)
				{
					struct ::addrinfo hints;
					::memset(&hints, 0, sizeof(hints));
					hints.ai_family = AF_INET;
					hints.ai_socktype = 0;
					hints.ai_protocol = 0;
					hints.ai_flags = 0;

					char portString[16];
					::sprintf(portString, "%d", port);

					int error = ::getaddrinfo(host, portString, &hints, &addrInfoList_);
					if (error != 0)
					{
						throw SocketAddressError(error, std::string("getaddrinfo(): ") + ::gai_strerror(error));
					}
				}

				~AddrInfoList() throw()
				{
					if (addrInfoList_ != 0)
					{
						::freeaddrinfo(addrInfoList_);
					}
				}

				class Iterator
				{
					public:

						Iterator(const AddrInfoList & addrInfoList)
							: current_(addrInfoList.addrInfoList_)
						{
							;;
						}

						bool AtEnd() const
						{
							return current_ == 0;
						}

						void Advance()
						{
							current_ = current_->ai_next;
						}

						struct ::addrinfo * operator->() const
						{
							return current_;
						}

					private:

						struct ::addrinfo * current_;
				};

			private:

				struct ::addrinfo * addrInfoList_;
		};

		void BuildAddress_GetAddrInfo(const char * host, int port, sockaddr_in & address)
		{
			AddrInfoList addrInfo(host, port);
			for (AddrInfoList::Iterator itr(addrInfo); !itr.AtEnd(); itr.Advance())
			{
				if (itr->ai_family == AF_INET)
				{
					::memcpy(&address, itr->ai_addr, itr->ai_addrlen);
					return;
				}
			}

			throw SocketAddressError(0, "Host not found");
		}

		void BuildAddress_GetHostByName(const char * host, int port, sockaddr_in & address)
		{
			struct ::hostent * hostEntry = ::gethostbyname(host);

			if (hostEntry == 0 || hostEntry->h_addrtype != AF_INET)
			{
				throw SocketAddressError(h_errno, FormatString("gethostbyname(\"%s\"): %s", host, ::hstrerror(h_errno)));
			}

			::memcpy(&address.sin_addr, hostEntry->h_addr_list[0], hostEntry->h_length);
			address.sin_port = htons(port);

			/*
			for (int i = 0; hostEntry->h_addr_list[i] != NULL; ++i)
			{
				printf("%08x\n", *((unsigned int *) (hostEntry->h_addr_list[i])));
			}
			*/
		}

		void BuildAddress_GetHostByNameReentrant(const char * host, int port, sockaddr_in & address)
		{
			struct ::hostent * hostEntry = 0;
			struct ::hostent hostData;

			static const unsigned int BUFFER_SIZE = 2048;
			char buffer[BUFFER_SIZE];
			int error = 0;

			if (::gethostbyname_r(host, &hostData, buffer, sizeof(buffer), &hostEntry, &error) != 0)
			{
				throw SocketAddressError(error, FormatString("gethostbyname_r(\"%s\", ...): %s", host, ::hstrerror(error)));
			}

			::memcpy(&address.sin_addr, hostEntry->h_addr_list[0], hostEntry->h_length);
			address.sin_port = htons(port);
		}
	}

	SocketAddressInet::SocketAddressInet()
	{
		Init();
		address_.sin_addr.s_addr = htonl(INADDR_NONE);
		address_.sin_port = 0;
	}

	SocketAddressInet::SocketAddressInet(int port)
	{
		Init();
		address_.sin_addr.s_addr = htonl(INADDR_ANY);
		address_.sin_port = htons(port);
	}

	SocketAddressInet::SocketAddressInet(const char * host, int port)
	{
		Set(host, port);
	}

	SocketAddressInet::SocketAddressInet(const std::string & host, int port)
	{
		Set(host.c_str(), port);
	}

	void SocketAddressInet::Init()
	{
		::memset(&address_, 0, sizeof(address_));
		address_.sin_family = AF_INET;
	}

	void SocketAddressInet::Set(const char * host, int port)
	{
		Init();
		// BuildAddress_GetAddrInfo(host, port, address_);
		// BuildAddress_GetHostByName(host, port, address_);
		BuildAddress_GetHostByNameReentrant(host, port, address_);
	}

	const std::string SocketAddressInet::Host() const
	{
		char ipaddr[INET_ADDRSTRLEN];

		if (::inet_ntop(AF_INET, &(address_.sin_addr.s_addr), ipaddr, sizeof(ipaddr)) == NULL)
		{
			throw UnixError();
		}

		return std::string(ipaddr);
	}

	int SocketAddressInet::Port() const
	{
		return ntohs(address_.sin_port);
	}

	const ::sockaddr * SocketAddressInet::SockAddr() const
	{
		return reinterpret_cast<const sockaddr *>(&address_);
	}

	int SocketAddressInet::Size() const
	{
		return sizeof(address_);
	}
}

