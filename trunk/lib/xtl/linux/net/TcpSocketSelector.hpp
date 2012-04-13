#ifndef XTL__TCP_SOCKET_SELECTOR_HPP__
#define XTL__TCP_SOCKET_SELECTOR_HPP__ 1

#include "TcpSocketSet.hpp"

namespace XTL
{
	class TcpSocketSelector
	{
		public:

			class Timeout
			{
				public:

					Timeout();

					Timeout(int secs, int microsecs);

					explicit Timeout(int secs);

					explicit Timeout(double secs);

					Timeout(const Timeout & other);

					~Timeout() throw();

					Timeout & operator= (const Timeout & other);

					bool IsNull() const
					{
						return t_ == 0;
					}

					int Secs() const
					{
						return t_->tv_sec;
					}

					int MicroSecs() const
					{
						return t_->tv_usec;
					}

				private:

					friend class TcpSocketSelector;

					void Init(int secs, int microsecs);

					struct timeval * Raw()
					{
						return t_;
					}

					struct timeval * t_;
			};

			class SelectResult
			{
				public:

					int SelectedCount() const
					{
						return selectedCount_;
					}

					bool IsReadable(TcpSocket socket) const
					{
						return readSet_.Contains(socket);
					}

					bool IsWritable(TcpSocket socket) const
					{
						return writeSet_.Contains(socket);
					}

					const TcpSocketSet & ReadableSet()
					{
						return readSet_;
					}

					const TcpSocketSet & WritableSet()
					{
						return writeSet_;
					}

				private:

					friend class TcpSocketSelector;

					void Select(int maxfd, const TcpSocketSet & readSet, const TcpSocketSet & writeSet, const Timeout & timeout);

					TcpSocketSet readSet_;
					TcpSocketSet writeSet_;
					Timeout      timeout_;
					int          selectedCount_;
			};

			TcpSocketSelector();

			void Clear();

			bool Insert(TcpSocket socket, bool checkRead, bool checkWrite);

			bool Delete(TcpSocket socket);

			void Select(SelectResult & result, const Timeout & timeout = Timeout());

		private:

			int          maxfd_;
			TcpSocketSet readSet_;
			TcpSocketSet writeSet_;
	};
}

#endif

