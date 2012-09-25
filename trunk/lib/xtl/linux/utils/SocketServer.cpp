#include "SocketServer.hpp"

#include <memory>
#include <stdexcept>

#include "../UnixError.hpp"

namespace XTL
{
	namespace
	{
		class ClientDisconnected
		{
			public:

				ClientDisconnected(ClientSocket socket)
					: socket_(socket) { ;; }

				ClientSocket Socket() const throw() { return socket_; }

			private:

				const ClientSocket socket_;
		};
	}

	class SocketServer::Client
	{
		public:

			static const unsigned int RECEIVE_BUFFER_SIZE = 4096;

			explicit Client(ClientSocket socket)
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

			friend class SocketServer;
			friend class TcpSocketServer;

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
				const ClientSocket::ReceiveResult result = socket_.Receive(buffer_, sizeof(buffer_));

				if (result.IsConnectionClosed())
				{
					Disconnect();
				}

				if (handler_.get() != 0 && result.WasRead() > 0)
				{
					handler_->OnDataReceived(buffer_, result.WasRead());
				}
			}

		private:

			const ClientSocket socket_;
			char buffer_[RECEIVE_BUFFER_SIZE];
			std::auto_ptr<ClientHandler> handler_;
	};

	/********** SocketServer::ClientHandler **********/

	SocketServer::ClientHandler::ClientHandler(Client & client)
		: client_(client)
	{
		;;
	}

	SocketServer::ClientHandler::~ClientHandler() throw()
	{
		;;
	}

	void SocketServer::ClientHandler::Disconnect()
	{
		client_.Disconnect();
	}

	/********** SocketServer **********/

	SocketServer::SocketServer(double selectTimeout)
		: terminated_(0),
		  socketSelector_(),
		  selectTimeout_(selectTimeout),
		  serverSockets_(),
		  clients_()
	{
		;;
	}

	SocketServer::~SocketServer() throw()
	{
		;;
	}

	void SocketServer::AddListeningSocket(ServerSocket serverSocket)
	{
		serverSockets_.push_back(serverSocket);
		socketSelector_.Insert(serverSocket, true, false);
	}

	void SocketServer::Run()
	{
		if (serverSockets_.empty())
		{
			throw std::runtime_error("SocketServer::Run(), while no listening sockets");
		}

		while (!terminated_)
		{
			Iterate();
		}

		OnTerminated();
	}

	void SocketServer::Terminate()
	{
		terminated_ = 1;
	}

	void SocketServer::Iterate()
	{
		SocketSelector::SelectResult selectResult;

		socketSelector_.Select(selectResult, selectTimeout_);

		if (selectResult.SelectedCount() == 0)
		{
			return;
		}

		AcceptClients(selectResult);

		const SocketSet readable = selectResult.ReadableSet();

		std::vector<ClientSocket> disconnected;

		const AutoPtrMap<ClientSocket, Client>::Iterator end = clients_.End();
		for (AutoPtrMap<ClientSocket, Client>::Iterator itr = clients_.Begin(); itr != end; ++itr)
		{
			if (readable.Contains(itr.Key()))
			{
				try
				{
					itr->Receive();
				}
				catch (const ClientDisconnected & e)
				{
					disconnected.push_back(e.Socket());
				}
			}

			if (terminated_)
			{
				return;
			}
		}

		for (std::vector<ClientSocket>::iterator itr = disconnected.begin(); itr != disconnected.end(); ++itr)
		{
			socketSelector_.Delete(*itr);

			itr->Close();

			clients_.Erase(*itr);
		}
	}

	void SocketServer::AcceptClients(const SocketSelector::SelectResult & selectResult)
	{
		const std::vector<ServerSocket>::iterator end = serverSockets_.end();
		for (std::vector<ServerSocket>::iterator itr = serverSockets_.begin(); itr != end; ++itr)
		{
			if (!selectResult.IsReadable(*itr))
			{
				continue;
			}

			ClientSocket clientSocket = itr->Accept();

			if (clientSocket.IsNull())
			{
				continue;
			}

			std::auto_ptr<Client> client(new Client(clientSocket));
			std::auto_ptr<ClientHandler> clientHandler(CreateClientHandler(*client));
			client->SetHandler(clientHandler);

			// TODO: select client for writing, if it has non-empty sendBuffer
			if (!socketSelector_.Insert(clientSocket, true, false))
			{
				throw std::runtime_error("SocketServer::AcceptClients(), could not insert socket descriptor to fd_set");
			}

			clients_.Set(clientSocket, client);
		}
	}
}
