#include <stdio.h>

#include <xtl/TcpSocket.hpp>
#include <xtl/SocketAddressInet.hpp>

namespace XTL
{
	class SocketSet
	{
		public:

			SocketSet()
			{
				Clear();
			}

			void Clear()
			{
				FD_ZERO(&set_);
			}

			void Add(int desc)
			{
				FD_SET(desc, &set_);
			}

			void Remove(int desc)
			{
				FD_CLR(desc, &set_);
			}

			bool Contains(int desc) const
			{
				return FD_ISSET(desc, &set_);
			}

			::fd_set * Data()
			{
				return &set_;
			}

			static bool InvalidDesc(int desc)
			{
				return desc < 0 || desc >= FD_SETSIZE;
			}

		protected:

			::fd_set set_;
	};

	class SocketMultiplexor
	{
		public:

			SocketMultiplexor()
				: maxDesc_(-1), set_(), readSet_(), writeSet_(), errorSet_() { ;; }

			void Clear()
			{
				maxDesc_ = -1;
				set_.Clear();
				readSet_.Clear();
				writeSet_.Clear();
				errorSet_.Clear();
			}

			int Count() const
			{
				return maxDesc_ + 1;
			}

			bool Add(int desc)
			{
				if (SocketSet::InvalidDesc(desc))
				{
					return false;
				}
				set_.Add(desc);
				if (desc > maxDesc_)
				{
					maxDesc_ = desc;
				}
				return true;
			}

			bool Remove(int desc)
			{
				if (SocketSet::InvalidDesc(desc))
				{
					return false;
				}
				set_.Remove(desc);
				readSet_.Remove(desc);
				writeSet_.Remove(desc);
				errorSet_.Remove(desc);

				if (desc == maxDesc_)
				{
					for (--maxDesc_; maxDesc_ >= 0; --maxDesc_)
					{
						if (set_.Contains(maxDesc_))
						{
							return true;
						}
					}
				}
				return true;
			}

			void Select(long timeoutSec, long timeoutMicrosec)
			{
				readSet_ = set_;
				writeSet_ = set_;
				errorSet_ = set_;
				::timeval t = { timeoutSec, timeoutMicrosec };

				int result = ::select(maxDesc_ + 1, readSet_.Data(), writeSet_.Data(), errorSet_.Data(), &t);

				if (result < 0)
				{
					if (errno == EINTR)
					{
						throw InterruptError("select()");
					}
					else
					{
						throw SocketError(errno, std::string("select(): ") + strerror(errno));
					}
				}
			}

			bool CanRead(int desc) const
			{
				return readSet_.Contains(desc);
			}

			bool CanWrite(int desc) const
			{
				return readSet_.Contains(desc);
			}

			bool Error(int desc) const
			{
				return readSet_.Contains(desc);
			}

		protected:

			int       maxDesc_;
			SocketSet set_;
			SocketSet readSet_;
			SocketSet writeSet_;
			SocketSet errorSet_;
	};
}

int main(int argc, const char * argv[])
{
	const char * const request = "GET / HTTP/1.1\r\nHost: ya.ru\r\n\r\n";

	try
	{
		char buffer[65536];
		XTL::TcpSocket socket;
		XTL::SocketMultiplexor mux;
		socket.Create(false);

		mux.Add(socket);
		socket.Connect("yandex.ru", 80);

		mux.Select(1, 0);

		int r;
		r = socket.Send(request, strlen(request));
		printf("send: %d\n", r);
		while (1)
		{
			r = socket.Receive(buffer, sizeof(buffer));
			if (r < 0)
			{
				break;
			}
			buffer[r] = '\0';
			printf("receive: %d\n", r);
//			printf("%s", buffer);
		}
		printf("\n");

//		socket.Listen("172.16.10.52", 1024, 20);
		socket.Close();

		//fprintf(stdout, "%s\n", connected ? "connected" : "connecting...");

//		printf("%s\n", socket.RemoteAddress().AsString().c_str());
	}
	catch (const XTL::Exception & e)
	{
		fprintf(stderr, "%s\n", e.What());
	}
	return 0;

	XTL::TcpSocket socket;

	socket.Create(true);

	printf("Blocking...%d\n", socket.GetBlocking());

	try
	{
		/*
		if (!socket.Bind(XTL::SocketAddressInet("127.0.0.1")))
		{
			printf("Bind error\n");
		}
		*/

		if (socket.Connect("www.microsoft.com", 80))
		{
			printf("Connected\n");
		}
		else
		{
			printf("Not connected\n");
		}

		printf("%s -> %s\n", socket.LocalAddress().AsString().c_str(), socket.RemoteAddress().AsString().c_str());
	}
	catch (const XTL::Exception & e)
	{
		printf("Exception: %s\n", e.What());
	}
}

