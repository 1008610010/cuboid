#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include <xtl/linux/UnixError.hpp>
#include <xtl/linux/net/UnixSocket.hpp>
#include <xtl/linux/net/SocketAddressUnix.hpp>

/*
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

*/

int main(int argc, const char * argv[])
{
	pid_t pid = ::fork();

	if (pid != 0)
	{
		// Parent process
	}
	else
	{
		// Child process

		try
		{
			XTL::UnixSocket serverSocket = XTL::UnixSocket::Create(false);

			XTL::SocketAddressUnix serverAddress("LJ_STATS_WRITER");
			
		}
		catch (const XTL::UnixError & e)
		{
			fprintf(stderr, "%s\n", e.What().c_str());
			return 1;
		}

/*
		int serverSocket = -1;
		if ((serverSocket = ::socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
		{
			perror("socket");
			return 1;
		}

		struct sockaddr_un serverAddress;

		::memset(&serverAddress, '\0', sizeof(serverAddress));
		serverAddress.sun_family = AF_UNIX;
		::strcpy(serverAddress.sun_path, "LJ_STATS_WRITER");
		::unlink(serverAddress.sun_path);

		int serverAddressLength = ::strlen(serverAddress.sun_path) + sizeof(serverAddress.sun_family);
		if (::bind(serverSocket, (struct sockaddr *) &serverAddress, serverAddressLength) == -1)
		{
			perror("bind");
			return 1;
		}

		const int SERVER_BACKLOG = 5;
		if (::listen(serverSocket, SERVER_BACKLOG) == -1)
		{
			perror("listen");
			return 1;
		}

		while (1)
		{
			
		}
*/
	}
}

