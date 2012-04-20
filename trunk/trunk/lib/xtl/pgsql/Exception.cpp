#include "Exception.hpp"

#include "QueryResult.hpp"

namespace XTL
{
namespace PGSQL
{
	Exception::Exception(const char * what)
		: what_(what == 0 ? "(nil)" : what)
	{
		;;
	}

	Exception::~Exception() throw()
	{
		;;
	}

	const std::string Exception::What() const
	{
		return what_.c_str();
	}



	ConnectError::ConnectError(const char * what)
		: Exception(what)
	{
		;;
	}

	ConnectError::~ConnectError() throw()
	{
		;;
	}



	QueryError::QueryError(const char * what, const char * query, const QueryResult & result)
		: Exception(what),
		  query_(query),
		  state_(result.GetSqlState())
	{
		;;
	}

	QueryError::~QueryError() throw()
	{
		;;
	}

	const std::string QueryError::GetQuery() const
	{
		return query_;
	}

	const QueryState QueryError::GetState() const
	{
		return state_;
	}
}
}

