#ifndef XTL__UNIX_SOCKET_SERVER_HPP__
#define XTL__UNIX_SOCKET_SERVER_HPP__ 1

#include <memory>
#include <string>

#include "../../utils/AutoPtrVector.hpp"
#include "../UnixError.hpp"
#include "../net/SocketAddressUnix.hpp"
#include "../net/UnixServerSocket.hpp"
#include "SocketServer.hpp"

namespace XTL
{
	class UnixSocketServer : public SocketServer
	{
		public:

			class UnixSocketHolder
			{
				public:

					explicit UnixSocketHolder(const std::string & unixSocketPath)
						: socketAddress_(unixSocketPath)
					{
						;;
					}

					~UnixSocketHolder() throw()
					{
						try
						{
							Unlink();
						}
						catch (const UnixError & e)
						{
							::fprintf(stderr, "Could not unlink unix socket '%s': %s\n", socketAddress_.Path().c_str(), e.What().c_str());
						}
					}

					const SocketAddressUnix & Address() const throw()
					{
						return socketAddress_;
					}

					void Unlink()
					{
						socketAddress_.Unlink();
					}

				private:

					SocketAddressUnix socketAddress_;
			};

			static const int DEFAULT_LISTEN_BACKLOG = 5;
			static const int DEFAULT_SELECT_TIMEOUT = 1;

			explicit UnixSocketServer(double selectTimeout = DEFAULT_SELECT_TIMEOUT);

			virtual ~UnixSocketServer() throw();

			void Listen(const std::string & unixSocketPath, bool recreate, int listenBacklog = DEFAULT_LISTEN_BACKLOG);

		private:

			AutoPtrVector<UnixSocketHolder> unixSockets_;
	};
}

#endif

