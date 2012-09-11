#include "UnixSocketServer.hpp"

#include <sys/types.h>
#include <sys/stat.h>

#include <vector>

#include "../UnixError.hpp"

namespace XTL
{
	namespace
	{
		class UmaskHolder
		{
			public:

				UmaskHolder()
					: oldMode_(::umask(0))
				{
					;;
				}

				~UmaskHolder() throw()
				{
					::umask(oldMode_);
				}

			private:

				mode_t oldMode_;
		};
	}

	UnixSocketServer::UnixSocketServer(double selectTimeout)
		: SocketServer(selectTimeout),
		  unixSockets_()
	{
		;;
	}

	UnixSocketServer::~UnixSocketServer() throw()
	{
		;;
	}

	void UnixSocketServer::Listen(const std::string & unixSocketPath, bool recreate, int listenBacklog)
	{
		UmaskHolder umaskHolder;

		std::auto_ptr<UnixSocketHolder> unixSocket(new UnixSocketHolder(unixSocketPath));

		UnixServerSocket serverSocket(UnixServerSocket::Create(false));

		try
		{
			serverSocket.Bind(unixSocket->Address());
		}
		catch (const UnixError & e)
		{
			if (recreate && e.Code() == EADDRINUSE)
			{
				unixSocket->Unlink();
				serverSocket.Bind(unixSocket->Address());
			}
			else
			{
				// FormatString("Could not bind unix socket '%s': %s", unixSocketPath, e.What());
				throw;
			}
		}

		serverSocket.Listen(listenBacklog);

		AddListeningSocket(serverSocket);
		unixSockets_.PushBack(unixSocket);
	}
}
