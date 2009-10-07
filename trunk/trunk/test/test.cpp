#include "xtl/TcpClient.hpp"
#include "xtl/ByteBuffer.hpp"
#include <netdb.h>

class NameResolveException
{
};

class IpAddress
{
	public:

		IpAddress()
		{
			address_.s_addr = INADDR_NONE;
		}

		IpAddress(const char * host)
		{
			Initialize(host);
		}

		static IpAddress None()
		{
			return IpAddress((unsigned long int) INADDR_NONE);
		}

		static IpAddress Any()
		{
			return IpAddress((unsigned long int) INADDR_ANY);
		}

		const std::string AsString() const
		{
			char buffer[INET_ADDRSTRLEN];
			return ::inet_ntop(AF_INET, &(address_.s_addr), buffer, sizeof(buffer)) == NULL
			       ? std::string()
			       : std::string(buffer);
		}

	protected:

		explicit IpAddress(unsigned long int address)
		{
			address_.s_addr = address;
		}

		void Initialize(const char * host)
		{
			/*
#if XTL_USE_GETADDRINFO
			struct ::addrinfo hints;
			struct ::addrinfo * result;

			::memset(&hints, 0, sizeof(hints));
			hints.ai_family = AF_INET;
			hints.ai_socktype = 0;
			hints.ai_protocol = 0;
			hints.ai_flags = 0;

			const char * portString[16];
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
			*/

			struct ::hostent * hostEntry = NULL;
#ifdef XTL_USE_GETHOSTBYNAME_R
			struct ::hostent hostData;
			char buffer[2048];
			int error = 0;
			if (::gethostbyname_r(host, &hostData, buffer, sizeof(buffer), &hostEntry, &error) != 0)
			{
				// throw SocketAddressError(error, std::string("gethostbyname_r(): ") + ::hstrerror(error));
				throw NameResolveException();
			}
#else
			hostEntry = ::gethostbyname(host);
			if (hostEntry == NULL || hostEntry->h_addrtype != AF_INET)
			{
				// throw SocketAddressError(h_errno, std::string("gethostbyname(): ") + ::hstrerror(h_errno));
				throw NameResolveException();
			}
#endif
			::memcpy(&address_, hostEntry->h_addr_list[0], hostEntry->h_length);

/*
			for (int i = 0; hostEntry->h_addr_list[i] != NULL; ++i)
			{
				printf("%08x\n", *((unsigned int *) (hostEntry->h_addr_list[i])));
			}
*/
//#endif
		}

		struct ::in_addr address_;
};

int main(int argc, const char * argv[])
{
	XTL::ByteBuffer bb(600000);
	bb << 1;
	XTL::ByteBuffer a = bb;
	return 0;

	const char * remoteHost = "www.microsoft.com";
	int remotePort = 80;

	XTL::TcpClient client;

	if (!client.Connect(remoteHost, remotePort))
	{
		fprintf(stderr, "Unable to connect to %s:%d\n", remoteHost, remotePort);
		return 1;
	}

	fprintf(stdout, "Status: %d\n", client.Status());

	return 0;

	fprintf(stderr, "Connected to %s:%d\n", remoteHost, remotePort);

	client.Push(
		"GET / HTTP/1.1\r\n"
		"Host: www.yandex.ru\r\n"
		"User-Agent: Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5\r\n"
		"Accept: text/html\r\n"
		"Connection: close\r\n\r\n"
	);

	while (client.HasSendingData())
	{
		if (!client.DoSend())
		{
			fprintf(stderr, "Error, while sending request\n");
			return 1;
		}
	}

	while (client.DoReceive())
	{
		;;
	}

	printf("%s\n", client.ReceivedData());

	client.Close();

	return 0;

//	TcpSocket cs;

//	int result = cs.connect(host.c_str(), port, true);
//	printf("connect = %d\n", result);

	/*
	const char * const q =
	"GET / HTTP/1.1\r\n"
	"Host: yandex.ru\r\n"
	"User-Agent: Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5\r\n"
	"Accept: text/html\r\n"
	"Connection: close\r\n\r\n";

	printf("sending\n");
	cs.sendAll(q, strlen(q));
	printf("ok!\n");

	char buffer[4096];
	int received;

	while (cs.opened())
	{
		received = cs.recv(buffer, sizeof(buffer) - 1);
		if (received > 0)
		{
			buffer[received] = '\0';
			printf("received: %d\n", received);
		}
	}
	*/

//	cs.close();
}

