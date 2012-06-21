#ifndef XTL__UNIX_SOCKET_SERVER_HPP__
#define XTL__UNIX_SOCKET_SERVER_HPP__ 1

#include <signal.h>

#include <memory>
#include <string>

#include "../../utils/AutoPtrMap.hpp"
#include "../net/SocketAddressUnix.hpp"
#include "../net/SocketSelector.hpp"
#include "../net/UnixClientSocket.hpp"
#include "../net/UnixServerSocket.hpp"

namespace XTL
{
	class UnixSocketServer
	{
		public:

			static const int DEFAULT_LISTEN_BACKLOG = 5;
			static const int DEFAULT_SELECT_TIMEOUT = 1;

			explicit UnixSocketServer(const std::string & unixSocketPath, int listenBacklog = DEFAULT_LISTEN_BACKLOG, float selectTimeout = DEFAULT_SELECT_TIMEOUT);

			~UnixSocketServer() throw();

			void Run();

			void Terminate()
			{
				terminated_ = 1;
			}

			virtual void OnTerminated() = 0;

			class Client;
			class ClientHandler;

			virtual std::auto_ptr<ClientHandler> CreateClientHandler(Client & client) = 0;

			class ClientDisconnected
			{
				public:

					ClientDisconnected(UnixClientSocket socket)
						: socket_(socket) { ;; }

					UnixClientSocket Socket() const  { return socket_; }

				private:

					const UnixClientSocket socket_;
			};

			class ClientHandler
			{
				public:

					explicit ClientHandler(Client & client)
						: client_(client)
					{
						;;
					}

					virtual ~ClientHandler() throw()
					{
						;;
					}

					virtual void OnClientConnected() = 0;

					virtual void OnClientDisconnected() = 0;

					virtual void OnDataReceived(const void * buffer, unsigned int size) = 0;

				protected:

					void Disconnect()
					{
						client_.Disconnect();
					}

				private:

					ClientHandler(const ClientHandler &);
					ClientHandler & operator= (const ClientHandler &);

					Client & client_;
			};

			class Client
			{
				public:

					static const unsigned int RECEIVE_BUFFER_SIZE = 4096;

					explicit Client(UnixClientSocket socket)
						: socket_(socket),
						  buffer_(),
						  handler_()
					{
						;;
					}

					void Disconnect()
					{
						if (handler_.get() != 0)
						{
							handler_->OnClientDisconnected();
						}

						throw ClientDisconnected(socket_);
					}

				protected:

					friend class UnixSocketServer;

					void SetHandler(std::auto_ptr<ClientHandler> handler)
					{
						handler_ = handler;

						if (handler_.get() != 0)
						{
							handler_->OnClientConnected();
						}
					}

					void Receive()
					{
						int wasRead = socket_.Receive(buffer_, sizeof(buffer_));

						if (wasRead < 0)
						{
							Disconnect();
						}

						if (wasRead > 0 && handler_.get() != 0)
						{
							handler_->OnDataReceived(buffer_, wasRead);
						}
					}

				private:

					const UnixClientSocket socket_;
					char buffer_[RECEIVE_BUFFER_SIZE];
					std::auto_ptr<ClientHandler> handler_;
			};

		private:

			void Iterate();

			volatile sig_atomic_t   terminated_;
			const std::string       unixSocketPath_;
			SocketAddressUnix       serverAddress_;
			UnixServerSocket        serverSocket_;
			SocketSelector          socketSelector_;
			SocketSelector::Timeout selectTimeout_;
			AutoPtrMap<UnixClientSocket, Client> clients_;
	};
}

#endif

