#ifndef XTL__SQLITE__EXCEPTION_HPP__
#define XTL__SQLITE__EXCEPTION_HPP__ 1

#include <string>

namespace XTL
{
namespace SQLITE
{
	class Exception
	{
		public:

			Exception(int code, const char * message);

			virtual ~Exception() throw();

			int Code() const;

			const std::string & Message() const;

			bool IsDatabaseLocked() const;

		private:

			const int         code_;
			const std::string message_;
	};

	class QueryError : public Exception
	{
		public:

			QueryError(int code, const char * message, const std::string & query);

			virtual ~QueryError() throw();

			const std::string & Query() const;

		private:

			const std::string query_;
	};
}
}

#endif
