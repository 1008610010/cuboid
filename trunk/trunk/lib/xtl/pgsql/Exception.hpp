#ifndef XTL__PGSQL__EXCEPTION_HPP__
#define XTL__PGSQL__EXCEPTION_HPP__ 1

#include <memory>

#include "QueryState.hpp"
#include "../Exception.hpp"

namespace XTL
{
namespace PGSQL
{
	class QueryResult;

	class Exception : public XTL::Exception
	{
		public:

			explicit Exception(const char * what);

			virtual ~Exception() throw();

			virtual const std::string What() const;

		private:

			const std::string what_;
	};

	class ConnectError : public Exception
	{
		public:

			explicit ConnectError(const char * what);

			virtual ~ConnectError() throw();
	};

	class QueryError : public Exception
	{
		public:

			QueryError(const char * what, const char * query, const QueryResult & result);

			virtual ~QueryError() throw();

			const std::string GetQuery() const;

			const QueryState GetState() const;

		private:

			const std::string query_;
			const QueryState  state_;
	};
}
}

#endif

