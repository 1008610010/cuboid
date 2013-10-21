#include "Exception.hpp"

#include "FormatString.hpp"

namespace XTL
{
	Exception::~Exception() throw()
	{
		;;
	}


	LogicError::LogicError(const char * file, unsigned int line, const char * what)
		: file_(file),
		  line_(line),
		  what_(what)
	{
		;;
	}

	LogicError::LogicError(const char * file, unsigned int line, const std::string & what)
		: file_(file),
		  line_(line),
		  what_(what)
	{
		;;
	}

	LogicError::~LogicError() throw()
	{
		;;
	}

	const std::string LogicError::What() const
	{
		return XTL::FormatString("%s (file: %s, line: %u)", what_, file_, line_);
	}


	InvalidArgumentError::InvalidArgumentError(const char * file, unsigned int line, const char * what)
		: LogicError(file, line, what)
	{
		;;
	}

	InvalidArgumentError::InvalidArgumentError(const char * file, unsigned int line, const std::string & what)
		: LogicError(file, line, what)
	{
		;;
	}

	InvalidArgumentError::~InvalidArgumentError() throw()
	{
		;;
	}


	IllegalOperationError::IllegalOperationError(const char * file, unsigned int line, const char * what)
		: LogicError(file, line, what)
	{
		;;
	}

	IllegalOperationError::~IllegalOperationError() throw()
	{
		;;
	}


	TerminateProgram::TerminateProgram(int exitCode)
		: exitCode_(exitCode)
	{
		;;
	}

	TerminateProgram::~TerminateProgram() throw()
	{
		;;
	}

	int TerminateProgram::ExitCode() const
	{
		return exitCode_;
	}
}

