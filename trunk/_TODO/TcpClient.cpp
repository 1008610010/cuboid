#include "TcpClient.hpp"
#include "SocketAddressInet.hpp"

namespace XTL
{
	TcpClient::TcpClient()
		: status_(CLOSED), socket_(), recvBuffer_(), sendBuffer_()
	{
		;;
	}

	TcpClient::~TcpClient() throw()
	{
		;;
	}

	bool TcpClient::Connect(const SocketAddressInet & address)
	{
		int result = socket_.Connect(address);
		/*
		if (result == TcpSocket::OK)
		{
			status_ = OPENED;
			return true;
		}
		else if (result == TcpSocket::IN_PROGRESS)
		{
			status_ = OPENING;
			return true;
		}
		else
		{
			Close();
			return false;
		}
		*/
		return true;
	}

	bool TcpClient::Connect(const char * host, int port)
	{
		return Connect(SocketAddressInet(host, port));
	}

	void TcpClient::Close()
	{
		if (status_ != CLOSED)
		{
			status_ = CLOSED;
			socket_.Close();
			recvBuffer_.Clear();
			sendBuffer_.Clear();
		}
	}
}

