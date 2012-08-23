#include "UnixSocketServer.hpp"

#include <sys/types.h>
#include <sys/stat.h>

#include <vector>

#include "../UnixError.hpp"

namespace XTL
{
	namespace
	{
		float Frac(float f)
		{
			return f - static_cast<int>(f);
		}

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

	UnixSocketServer::UnixSocketServer(const std::string & unixSocketPath, int listenBacklog, float selectTimeout)
		: terminated_(0),
		  unixSocketPath_(unixSocketPath),
		  serverAddress_(unixSocketPath),
		  serverSocket_(UnixSocket::Create(false)),
		  socketSelector_(),
		  selectTimeout_(static_cast<int>(selectTimeout), static_cast<int>(1000000 * Frac(selectTimeout))),
		  clients_()
	{
		UmaskHolder umaskHolder;

		try
		{

			serverSocket_.Bind(serverAddress_);
		}
		catch (const UnixError & e)
		{
			if (e.Code() == EADDRINUSE)
			{
				serverAddress_.Unlink();
				serverSocket_.Bind(serverAddress_);
			}
			else
			{
				// FormatString("Could not bind unix socket '%s': %s", unixSocketPath, e.What());
				throw;
			}
		}

		serverSocket_.Listen(listenBacklog);

		socketSelector_.Insert(serverSocket_, true, false);
	}

	UnixSocketServer::~UnixSocketServer() throw()
	{
		try
		{
			serverAddress_.Unlink();
		}
		catch (const UnixError & e)
		{
			fprintf(stderr, "Could not unlink unix socket '%s': %s\n", unixSocketPath_.c_str(), e.What().c_str());
		}
	}

	void UnixSocketServer::Run()
	{
		while (!terminated_)
		{
			Iterate();
		}

		OnTerminated();
	}

	void UnixSocketServer::Iterate()
	{
		SocketSelector::SelectResult selectResult;

		socketSelector_.Select(selectResult, selectTimeout_);

		if (selectResult.SelectedCount() == 0)
		{
			return;
		}

		if (selectResult.IsReadable(serverSocket_))
		{
			UnixClientSocket clientSocket = serverSocket_.Accept();

			if (!clientSocket.IsNull())
			{
				std::auto_ptr<Client> client(new Client(clientSocket));
				std::auto_ptr<ClientHandler> clientHandler(CreateClientHandler(*client));
				client->SetHandler(clientHandler);

				// TODO: select client for writing, if it has non-empty sendBuffer
				socketSelector_.Insert(clientSocket, true, false);

				clients_.Set(clientSocket, client);
			}
		}

		const SocketSet readable = selectResult.ReadableSet();

		std::vector<UnixClientSocket> disconnected;

		for (AutoPtrMap<UnixClientSocket, Client>::const_iterator itr = clients_.begin(); itr != clients_.end(); ++itr)
		{
			if (readable.Contains(itr->first))
			{
				try
				{
					itr->second->Receive();
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

		for (std::vector<UnixClientSocket>::iterator itr = disconnected.begin(); itr != disconnected.end(); ++itr)
		{
			socketSelector_.Delete(*itr);
			clients_.Erase(*itr);
		}
	}
}
