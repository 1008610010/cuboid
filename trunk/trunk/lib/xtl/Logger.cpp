#include "Logger.hpp"

#include <stdio.h>

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
			"WARN ",
			"INFO ",
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
		Write("\n", 1);
	}

	void Logger::Write(const std::string & s)
	{
		Write(s.data(), s.size());
	}


	NullLogger::NullLogger()
		: Logger()
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
