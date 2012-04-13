#ifndef _XTL__TCP_CLIENT_HPP__
#define _XTL__TCP_CLIENT_HPP__ 1

#include "ByteBuffer.hpp"
#include "TcpSocket.hpp"

namespace XTL
{
	class SocketAddressInet;

	class TcpClient
	{
		public:

			static const int RECEIVE_BUFFER_SIZE = 4096;

			enum Status
			{
				CLOSED     = 0,
				CONNECTED  = 1,
				CONNECTING = 2
			};

			TcpClient();

			~TcpClient() throw();

			bool Connect(const SocketAddressInet & address);

			bool Connect(const char * host, int port);

			void Close();

			TcpSocket Socket() { return socket_; }

			int Status() const
			{
				return status_;
			}

			bool HasSendingData() const
			{
				return !sendBuffer_.Empty();
			}

			const char * ReceivedData() const
			{
				return recvBuffer_.Data();
			}

			void Push(const void * buffer, int size)
			{
				sendBuffer_.Append(buffer, size);
			}

			void Push(const char * buffer)
			{
				sendBuffer_.Append(buffer);
			}

			void Push(const ByteBuffer & buffer)
			{
				sendBuffer_.Append(buffer);
			}

			bool DoReceive()
			{
				switch (status_)
				{
					case CLOSED:     return false;
					case CONNECTING: return true;
				}

				char buffer[RECEIVE_BUFFER_SIZE];

				// TODO: replace buffer by recvBuffer_.Reserve(recvBuffer_.Size() + RECEIVE_BUFFER_SIZE)
				//       and recvBuffer_.SetSize(...);

				int result = socket_.Receive(buffer, RECEIVE_BUFFER_SIZE);
				if (result > 0)
				{
					recvBuffer_.Append(buffer, result);
				}
				else if (result < 0)
				{
					/*
					if (result == TcpSocket::CLOSED)
					{
						return false;
					}
					else
					{
						return false;
					}
					*/
				}
				return true;
			}

			bool DoSend()
			{
				switch (status_)
				{
					case CLOSED:     return false;
					case CONNECTING: return true;
				}

				if (sendBuffer_.Empty())
				{
					return true;
				}
				int result = socket_.Send(sendBuffer_.Data(), sendBuffer_.Size());
				if (result < 0)
				{
					return false;
				}
				sendBuffer_.Shift(result);
				return true;
			}

		protected:

			virtual bool OnConnected()
			{
				return true;
			}

			virtual void OnError()
			{
				;;
			}

			virtual bool OnReceived()
			{
				return true;
			}

			int        status_;
			TcpSocket  socket_;
			ByteBuffer recvBuffer_;
			ByteBuffer sendBuffer_;
	};
}


#endif

