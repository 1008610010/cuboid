#ifndef XTL__SOCKET_SELECTOR_HPP__
#define XTL__SOCKET_SELECTOR_HPP__ 1

#include "SocketSet.hpp"

namespace XTL
{
	class SocketSelector
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

					friend class SocketSelector;

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

					bool IsReadable(Socket socket) const
					{
						return readSet_.Contains(socket);
					}

					bool IsWritable(Socket socket) const
					{
						return writeSet_.Contains(socket);
					}

					const SocketSet & ReadableSet() const
					{
						return readSet_;
					}

					const SocketSet & WritableSet() const
					{
						return writeSet_;
					}

				private:

					friend class SocketSelector;

					void Select(int maxfd, const SocketSet & readSet, const SocketSet & writeSet, const Timeout & timeout);

					SocketSet readSet_;
					SocketSet writeSet_;
					Timeout   timeout_;
					int       selectedCount_;
			};

			SocketSelector();

			void Clear();

			bool Insert(Socket socket, bool checkRead, bool checkWrite);

			bool Delete(Socket socket);

			void Select(SelectResult & result, const Timeout & timeout = Timeout());

		private:

			int       maxfd_;
			SocketSet readSet_;
			SocketSet writeSet_;
	};
}

#endif

