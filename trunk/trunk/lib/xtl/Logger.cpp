#include "Logger.hpp"

#include <stdio.h>

#include <memory>
#include <stdexcept>

namespace XTL
{
	LogLevel::LogLevel(int value, const char * name)
		: value_(value),
		  name_(name)
	{
		;;
	}

	int LogLevel::Value() const
	{
		return value_;
	}

	const std::string & LogLevel::Name() const
	{
		return name_;
	}
}

const XTL::LogLevel LOG_ERROR (0, "ERROR");
const XTL::LogLevel LOG_WARN  (1, "WARN ");
const XTL::LogLevel LOG_INFO  (2, "INFO ");
const XTL::LogLevel LOG_DEBUG (3, "DEBUG");
const XTL::LogLevel LOG_TRACE (4, "TRACE");

namespace XTL
{
	Logger::Logger()
		: minLevel_(LOG_TRACE.Value())
	{
		;;
	}

	Logger::~Logger() throw()
	{
		;;
	}

	void Logger::SetMinLogLevel(int level)
	{
		minLevel_ = level;
	}

	void Logger::SetMinLogLevel(LogLevel level)
	{
		SetMinLogLevel(level.Value());
	}

	void Logger::Log(LogLevel level, const std::string & message)
	{
		if (level.Value() <= minLevel_)
		{
			WriteMessage(level, message);
		}
	}

	namespace
	{
		const std::string Now()
		{
			time_t ts_now = time(0);
			struct tm * now = ::gmtime(&ts_now);

			return XTL::FormatString(
				"%04u-%02u-%02u %02u:%02u:%02u",
				now->tm_year + 1900, now->tm_mon + 1, now->tm_mday,
				now->tm_hour, now->tm_min, now->tm_sec
			);
		}
	}

	void Logger::WriteMessage(const LogLevel & level, const std::string & message)
	{
		Write(Now());
		Write(" [", 2);
		Write(level.Name());
		Write("] ", 2);
		Write(message);
	}

	void Logger::Write(const std::string & s)
	{
		Write(s.data(), s.size());
	}


	NullLogger::NullLogger()
		: Logger()
	{
		SetMinLogLevel(-1);
	}

	NullLogger::~NullLogger() throw()
	{
		;;
	}

	void NullLogger::Write(const char * buffer, unsigned int size)
	{
		;;
	}

	void NullLogger::Flush()
	{
		;;
	}


	StdOutLogger::~StdOutLogger() throw()
	{
		;;
	}

	void StdOutLogger::Write(const char * buffer, unsigned int size)
	{
		const size_t written = ::fwrite(buffer, 1, size, stdout);
		if (written < size)
		{
			throw std::runtime_error("Error in StdOutLogger::Write");
		}
	}

	void StdOutLogger::Flush()
	{
		if (::fflush(stdout) != 0)
		{
			throw std::runtime_error("Could not fflush(stdout)");
		}
	}


	StdErrLogger::~StdErrLogger() throw()
	{
		;;
	}

	void StdErrLogger::Write(const char * buffer, unsigned int size)
	{
		const size_t written = ::fwrite(buffer, 1, size, stderr);
		if (written < size)
		{
			throw std::runtime_error("Error in StdErrLogger::Write");
		}
	}

	void StdErrLogger::Flush()
	{
		if (::fflush(stderr) != 0)
		{
			throw std::runtime_error("Could not fflush(stderr)");
		}
	}


	FileLogger::FileLogger(const std::string & filePath)
		: Logger(),
		  file_(filePath)
	{
		file_.Create(XTL::File::CREATE_APPEND);
	}

	FileLogger::~FileLogger() throw()
	{
		file_.Close();
	}

	void FileLogger::Write(const char * buffer, unsigned int size)
	{
		file_.Write(buffer, size);
	}

	void FileLogger::Flush()
	{
		file_.Flush();
	}
}

namespace XTL
{
	namespace
	{
		std::auto_ptr<Logger> & DefaultLoggerPtr()
		{
			static std::auto_ptr<Logger> instance(new StdErrLogger());
			return instance;
		}
	}

	Logger & DefaultLogger()
	{
		return *(DefaultLoggerPtr());
	}

	void SetDefaultLogger(Logger * logger)
	{
		DefaultLoggerPtr().reset(logger);
	}

	void Warn(const char * message)
	{
		DefaultLogger().Log(LOG_WARN, message);
	}
}
