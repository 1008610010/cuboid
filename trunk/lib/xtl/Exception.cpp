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

	IllegalArgumentError::IllegalArgumentError(const char * file, unsigned int line, const char * what)
		: LogicError(file, line, what)
	{
		;;
	}

	IllegalArgumentError::IllegalArgumentError(const char * file, unsigned int line, const std::string & what)
		: LogicError(file, line, what)
	{
		;;
	}

	IllegalArgumentError::~IllegalArgumentError() throw()
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
}

