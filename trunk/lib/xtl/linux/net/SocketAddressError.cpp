#include "SocketAddressError.hpp"

namespace XTL
{
	SocketAddressError::SocketAddressError(int code, const std::string & what)
		: code_(code),
		  what_(what)
	{
		;;
	}

	SocketAddressError::~SocketAddressError() throw()
	{
		;;
	}

	const std::string SocketAddressError::What() const
	{
		return what_;
	}
}

