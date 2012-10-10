#ifndef XTL__EXCEPTION_HPP__
#define XTL__EXCEPTION_HPP__ 1

#include <cstdio>
#include <string>

#define ILLEGAL_ARGUMENT_ERROR(WHAT)  XTL::IllegalArgumentError(__FILE__, __LINE__, WHAT)
#define ILLEGAL_OPERATION_ERROR(WHAT) XTL::IllegalOperationError(__FILE__, __LINE__, WHAT)

namespace XTL
{
	// TODO: subclass all this shit of std::exception
	class Exception
	{
		public:

			virtual ~Exception() throw();

			virtual const std::string What() const = 0;
	};

	class LogicError : public Exception
	{
		public:

			LogicError(const char * file, unsigned int line, const char * what);

			LogicError(const char * file, unsigned int line, const std::string & what);

			virtual ~LogicError() throw();

			virtual const std::string What() const;

		private:

			const char   *    file_;
			unsigned int      line_;
			const std::string what_;
	};

	class IllegalArgumentError : public LogicError
	{
		public:

			IllegalArgumentError(const char * file, unsigned int line, const char * what);

			IllegalArgumentError(const char * file, unsigned int line, const std::string & what);

			virtual ~IllegalArgumentError() throw();
	};

	class IllegalOperationError : public LogicError
	{
		public:

			IllegalOperationError(const char * file, unsigned int line, const char * what);

			virtual ~IllegalOperationError() throw();
	};

	class TerminateProgram
	{
		public:

			explicit TerminateProgram(int exitCode)
				: exitCode_(exitCode)
			{
				;;
			}

			int ExitCode() const
			{
				return exitCode_;
			}

		private:

			const int exitCode_;
	};
}

#endif

