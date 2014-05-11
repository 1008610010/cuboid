#include "Logger.hpp"

#include <stdio.h>
#include <sys/time.h>

#include <map>
#include <memory>
#include <stdexcept>

#include "StringUtils.hpp"

namespace XTL
{
	namespace
	{
		const std::string LOG_LEVEL_NAME[] = {
			"ERROR",
			" WARN",
			" INFO",
			"DEBUG",
			"TRACE"
		};
	}

	LogLevel::LogLevel(int value)
		: value_(value)
	{
		;;
	}

	int LogLevel::Value() const
	{
		return value_;
	}

	const std::string & LogLevel::Name() const
	{
		return LOG_LEVEL_NAME[value_];
	}

	bool LogLevel::operator== (const LogLevel & other) const
	{
		return Value() == other.Value();
	}

	bool LogLevel::operator<= (const LogLevel & other) const
	{
		return Value() <= other.Value();
	}

	bool LogLevel::operator>= (const LogLevel & other) const
	{
		return Value() >= other.Value();
	}
}

const XTL::LogLevel LOG_NOTHING (-1);
const XTL::LogLevel LOG_ERROR   ( 0);
const XTL::LogLevel LOG_WARN    ( 1);
const XTL::LogLevel LOG_INFO    ( 2);
const XTL::LogLevel LOG_DEBUG   ( 3);
const XTL::LogLevel LOG_TRACE   ( 4);

namespace XTL
{
	Logger::Logger()
		: minLevel_(LOG_TRACE)
	{
		;;
	}

	Logger::~Logger() throw()
	{
		;;
	}

	LogLevel Logger::GetMinLogLevel() const
	{
		return minLevel_;
	}

	void Logger::SetMinLogLevel(LogLevel level)
	{
		minLevel_ = level;
	}

	void Logger::Log(LogLevel level, const std::string & message)
	{
		if (level <= minLevel_)
		{
			WriteMessage(level, message);
		}
	}

	namespace
	{
		const std::string Now()
		{
			struct timeval tv;
			::gettimeofday(&tv, 0);

			struct tm t;
			::gmtime_r(&tv.tv_sec, &t);

			return XTL::FormatString(
				"[%02u/%02u/%04u %02u:%02u:%02u.%06u]",
				t.tm_mday, t.tm_mon + 1, t.tm_year + 1900,
				t.tm_hour, t.tm_min, t.tm_sec,
				static_cast<unsigned int>(tv.tv_usec)
			);
		}
	}


	SimpleLogger::~SimpleLogger() throw()
	{
		;;
	}

	void SimpleLogger::WriteMessage(const LogLevel & level, const std::string & message)
	{
		Write(Now());
		Write(" ", 1);
		Write(level.Name());
		Write(" ", 1);
		Write(message);
		Write("\n", 1);
	}

	void SimpleLogger::Write(const std::string & s)
	{
		Write(s.data(), s.size());
	}


	NullLogger::NullLogger()
		: SimpleLogger()
	{
		SetMinLogLevel(LOG_NOTHING);
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


	ConsoleLogger::~ConsoleLogger() throw()
	{
		;;
	}

	void ConsoleLogger::Flush()
	{
		if (::fflush(stdout) != 0)
		{
			throw std::runtime_error("Could not fflush(stdout)");
		}

		if (::fflush(stderr) != 0)
		{
			throw std::runtime_error("Could not fflush(stderr)");
		}
	}

	void ConsoleLogger::WriteMessage(const LogLevel & level, const std::string & message)
	{
		FILE * const stream = (level == LOG_ERROR || level == LOG_WARN) ? stderr : stdout;
		Write(stream, Now());
		Write(stream, " ", 1);
		Write(stream, level.Name());
		Write(stream, " ", 1);
		Write(stream, message);
		Write(stream, "\n", 1);
	}

	void ConsoleLogger::Write(FILE * stream, const char * buffer, unsigned int size)
	{
		const size_t written = ::fwrite(buffer, 1, size, stream);
		if (written < size)
		{
			throw std::runtime_error("Error in ConsoleLogger::Write");
		}
	}

	void ConsoleLogger::Write(FILE * stream, const std::string & buffer)
	{
		Write(stream, buffer.data(), buffer.size());
	}


	FileLogger::FileLogger(const std::string & filePath)
		: SimpleLogger(),
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
		class LevelsMap
		{
			public:

				static const LevelsMap & Instance()
				{
					static LevelsMap instance;
					return instance;
				}

				LogLevel Get(const std::string & level, LogLevel defaultValue) const
				{
					std::map<std::string, LogLevel>::const_iterator itr = levels_.find(XTL::LowerCased(level));
					return itr != levels_.end() ? itr->second : defaultValue;
				}

			private:

				LevelsMap()
				{
					levels_.insert(std::pair<std::string, LogLevel>("error",    LOG_ERROR));
					levels_.insert(std::pair<std::string, LogLevel>("err",      LOG_ERROR));
					levels_.insert(std::pair<std::string, LogLevel>("warnings", LOG_WARN));
					levels_.insert(std::pair<std::string, LogLevel>("warning",  LOG_WARN));
					levels_.insert(std::pair<std::string, LogLevel>("warn",     LOG_WARN));
					levels_.insert(std::pair<std::string, LogLevel>("info",     LOG_INFO));
					levels_.insert(std::pair<std::string, LogLevel>("debug",    LOG_DEBUG));
					levels_.insert(std::pair<std::string, LogLevel>("trace",    LOG_TRACE));
				}

				std::map<std::string, LogLevel> levels_;
		};
	}

	LogLevel LogLevelFromString(const std::string & level, LogLevel defaultValue)
	{
		return LevelsMap::Instance().Get(level, defaultValue);
	}

	namespace
	{
		std::auto_ptr<Logger> & DefaultLoggerPtr()
		{
			static std::auto_ptr<Logger> instance(new ConsoleLogger());
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

	LogLevel CurrentLogLevel()
	{
		return DefaultLogger().GetMinLogLevel();
	}

	void Log(LogLevel level, const std::string & message)
	{
		DefaultLogger().Log(level, message);
	}

	void Warn(const std::string & message)
	{
		Log(LOG_WARN, message);
	}
}
