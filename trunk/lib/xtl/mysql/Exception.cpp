#include "Exception.hpp"

// #include "QueryResult.hpp"

namespace XTL
{
namespace MYSQL
{
	Exception::Exception(const char * what, unsigned int code)
		: what_(what),
		  code_(code)
	{
		;;
	}

	Exception::~Exception() throw()
	{
		;;
	}

	const std::string Exception::What() const
	{
		return what_;
	}

	unsigned int Exception::Code() const
	{
		return code_;
	}




	ConnectError::ConnectError(const char * what, unsigned int code)
		: Exception(what, code)
	{
		;;
	}

	ConnectError::~ConnectError() throw()
	{
		;;
	}



	QueryError::QueryError(const char * what, unsigned int code, const std::string & query)
		: Exception(what, code),
		  query_(query)
	{
		;;
	}

	QueryError::~QueryError() throw()
	{
		;;
	}

	const std::string QueryError::Query() const
	{
		return query_;
	}
}
}

