#ifndef XTL_SQLITE__EXCEPTION_HPP__
#define XTL_SQLITE__EXCEPTION_HPP__ 1

#include <string>

namespace XTL
{
namespace SQLITE
{
	class Exception
	{
		public:

			Exception(int code, const char * message)
				: code_(code),
				  message_(message)
			{
				;;
			}

			int Code() const
			{
				return code_;
			}

			const std::string & Message() const
			{
				return message_;
			}

		private:

			const int         code_;
			const std::string message_;
	};

	class QueryError : public Exception
	{
		public:

			QueryError(int code, const char * message, const std::string & query)
				: Exception(code, message),
				  query_(query)
			{
				;;
			}

			const std::string & Query() const
			{
				return query_;
			}

		private:

			const std::string query_;
	};
}
}

#endif
