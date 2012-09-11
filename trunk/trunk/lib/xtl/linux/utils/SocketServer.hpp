#ifndef XTL__SOCKET_SERVER_HPP__
#define XTL__SOCKET_SERVER_HPP__ 1

#include <signal.h> // sig_atomic_t

#include <vector>

#include "../../utils/AutoPtrMap.hpp"
#include "../net/ClientSocket.hpp"
#include "../net/ServerSocket.hpp"
#include "../net/SocketSelector.hpp"

namespace XTL
{
	class SocketServer
	{
		public:

			class Client;

			class ClientHandler
			{
				public:

					explicit ClientHandler(Client & client);

					virtual ~ClientHandler() throw();

					virtual void OnClientConnected() = 0;

					virtual void OnClientDisconnected() = 0;

					virtual void OnReceiveError() = 0;

					virtual void OnDataReceived(const void * buffer, unsigned int size) = 0;

					/*
					 * TODO:
					 *   Create abstract class ClientSendData:
					 *
					 *   public:
					 *
					 *       void Send(std::auto_ptr<ClientSendData> data);
					 *
					 *   private:
					 *
					 *       AutoPtrQueue<ClientSendData> sendDataQueue_;
					 */

				protected:

					void Disconnect();

				private:

					ClientHandler(const ClientHandler &);
					ClientHandler & operator= (const ClientHandler &);

					Client & client_;
			};

			explicit SocketServer(double selectTimeout);

			virtual ~SocketServer() throw();

			virtual std::auto_ptr<ClientHandler> CreateClientHandler(Client & client) = 0;

			virtual void OnTerminated() = 0;

			void AddListeningSocket(ServerSocket serverSocket);

			void Run();

			void Terminate();

		private:

			SocketServer(const SocketServer &);
			SocketServer & operator= (const SocketServer &);

			void AcceptClients(const SocketSelector::SelectResult & selectResult);

			void Iterate();

			volatile sig_atomic_t     terminated_;
			SocketSelector            socketSelector_;
			SocketSelector::Timeout   selectTimeout_;
			std::vector<ServerSocket> serverSockets_;
			AutoPtrMap<ClientSocket, Client> clients_;
	};
}

#endif
