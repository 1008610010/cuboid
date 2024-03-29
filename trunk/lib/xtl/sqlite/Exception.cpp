#include "Exception.hpp"

#include <sqlite3.h>

namespace XTL
{
namespace SQLITE
{
	Exception::Exception(int code, const char * message)
		: code_(code),
		  message_(message)
	{
		;;
	}

	Exception::~Exception() throw()
	{
		;;
	}

	int Exception::Code() const
	{
		return code_;
	}

	const std::string & Exception::Message() const
	{
		return message_;
	}

	bool Exception::IsDatabaseLocked() const
	{
		return code_ == SQLITE_BUSY;
	}


	QueryError::QueryError(int code, const char * message, const std::string & query)
		: Exception(code, message),
		  query_(query)
	{
		;;
	}

	QueryError::~QueryError() throw()
	{
		;;
	}

	const std::string & QueryError::Query() const
	{
		return query_;
	}
}
}
