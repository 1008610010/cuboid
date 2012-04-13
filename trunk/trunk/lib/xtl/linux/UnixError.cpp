#include "UnixError.hpp"

#include <errno.h>
#include <string.h>

#include <vector>

namespace XTL
{
	UnixError::UnixError()
		: errno_(errno)
	{
		;;
	}

	UnixError::UnixError(int e)
		: errno_(e)
	{
		;;
	}

	UnixError::~UnixError() throw()
	{
		;;
	}

	const std::string UnixError::What() const
	{
		return ToString();
	}

	int UnixError::Code() const
	{
		return errno_;
	}

	namespace
	{
		static const int STRERROR_BUFFER_SIZE = 256;
	}

	const std::string UnixError::ToString() const
	{
		std::vector<char> buffer(STRERROR_BUFFER_SIZE);

#if (_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && ! _GNU_SOURCE
		while (true)
		{
			if (::strerror_r(errno_, &buffer[0], buffer.size()) != 0)
			{
				return std::string(&buffer[0]);
			}

			if (errno != ERANGE)
			{
				return "";
			}

			buffer.resize(2 * buffer.size());
		}
#else
		return ::strerror_r(errno_, &buffer[0], buffer.size());
#endif
	}



	UnixError::Interrupted::Interrupted()
		: UnixError(EINTR)
	{
		;;
	}

	UnixError::Interrupted::~Interrupted() throw()
	{
		;;
	}

	UnixError::AlreadyExists::AlreadyExists()
		: UnixError(EEXIST)
	{
		;;
	}

	UnixError::AlreadyExists::~AlreadyExists() throw()
	{
		;;
	}
}

