#ifndef XTL__SOCKET_HPP__
#define XTL__SOCKET_HPP__ 1

namespace XTL
{
	class Socket
	{
		public:

			Socket()
				: desc_(-1)
			{
				;;
			}

			explicit Socket(int desc)
				: desc_(desc)
			{
				;;
			}

			bool IsNull() const
			{
				return desc_ == -1;
			}

			/**
			 * @throw XTL::UnixError::Interrupted;
			 * @throw XTL::UnixError;
			 */
			void Close();

			/**
			 * @throw XTL::UnixError
			 */
			bool IsBlocking() const;

			/**
			 * @throw XTL::UnixError
			 */
			void SetBlocking(bool blocking);

		protected:

			friend class TcpSocketSelector;
			friend class TcpSocketSet;

			int Desc() const
			{
				return desc_;
			}

		private:

			int desc_;
	};
}

#endif

