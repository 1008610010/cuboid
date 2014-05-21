#ifndef XTL__MYSQL__EXCEPTION_HPP__
#define XTL__MYSQL__EXCEPTION_HPP__ 1

#include <memory>
#include <string>

// #include "QueryState.hpp"
#include "../Exception.hpp"

namespace XTL
{
namespace MYSQL
{
	class Exception : public XTL::Exception
	{
		public:

			explicit Exception(const char * what, unsigned int code);

			virtual ~Exception() throw();

			virtual const std::string What() const;

			unsigned int Code() const;

		private:

			const std::string  what_;
			const unsigned int code_;
	};

	class ConnectError : public Exception
	{
		public:

			explicit ConnectError(const char * what, unsigned int code);

			virtual ~ConnectError() throw();
	};

	class QueryError : public Exception
	{
		public:

			QueryError(const char * what, unsigned int code, const std::string & query);

			virtual ~QueryError() throw();

			const std::string Query() const;

		private:

			const std::string query_;
	};
}
}

#endif

