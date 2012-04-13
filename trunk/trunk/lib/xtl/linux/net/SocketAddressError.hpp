#ifndef XTL__SOCKET_ADDRESS_ERROR_HPP__
#define XTL__SOCKET_ADDRESS_ERROR_HPP__ 1

#include "../../Exception.hpp"

namespace XTL
{
	class SocketAddressError : public Exception
	{
		public:

			 SocketAddressError(int code, const std::string & what);

			virtual ~SocketAddressError() throw();

			virtual const std::string What() const;

		private:

			const int code_;
			const std::string what_;
	};
}

#endif

