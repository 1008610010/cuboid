#ifndef XTL__LOGGER_HPP__
#define XTL__LOGGER_HPP__ 1

#include <string>

#include "FormatString.hpp"
#include "linux/fs/File.hpp"

namespace XTL
{
	class LogLevel
	{
		public:

			LogLevel(int value, const char * name);

			int Value() const;

			const std::string & Name() const;

		private:

			const int          value_;
			const std::string  name_;
	};
}

extern const XTL::LogLevel LOG_ERROR;
extern const XTL::LogLevel LOG_WARN;
extern const XTL::LogLevel LOG_INFO;
extern const XTL::LogLevel LOG_DEBUG;
extern const XTL::LogLevel LOG_TRACE;

namespace XTL
{
	class Logger
	{
		public:

			Logger();

			virtual ~Logger() throw();

			void SetMinLogLevel(LogLevel level);

			void Log(LogLevel level, const std::string & message);

			template <typename T1>
			void Log(LogLevel level, const char * format, const T1 & t1);

			template <typename T1, typename T2>
			void Log(LogLevel level, const char * format, const T1 & t1, const T2 & t2);

			template <typename T1, typename T2, typename T3>
			void Log(LogLevel level, const char * format, const T1 & t1, const T2 & t2, const T3 & t3);

			template <typename T1, typename T2, typename T3, typename T4>
			void Log(LogLevel level, const char * format, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4);

			template <typename T1, typename T2, typename T3, typename T4, typename T5>
			void Log(LogLevel level, const char * format, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5);

			virtual void Flush() = 0;

		protected:

			void SetMinLogLevel(int level);

			void WriteMessage(const LogLevel & level, const std::string & message);

			void Write(const std::string & s);

			virtual void Write(const char * buffer, unsigned int size) = 0;

		private:

			Logger(const Logger &);
			Logger & operator= (const Logger &);

			int minLevel_;
	};

	class NullLogger : public Logger
	{
		public:

			NullLogger();

			virtual ~NullLogger() throw();

			virtual void Flush();

		protected:

			virtual void Write(const char * buffer, unsigned int size);
	};

	class StdOutLogger : public Logger
	{
		public:

			virtual ~StdOutLogger() throw();

			virtual void Flush();

		protected:

			virtual void Write(const char * buffer, unsigned int size);
	};

	class StdErrLogger : public Logger
	{
		public:

			virtual ~StdErrLogger() throw();

			virtual void Flush();

		protected:

			virtual void Write(const char * buffer, unsigned int size);
	};

	class FileLogger : public Logger
	{
		public:

			explicit FileLogger(const std::string & filePath);

			virtual ~FileLogger() throw();

			virtual void Flush();

		protected:

			virtual void Write(const char * buffer, unsigned int size);

		private:

			XTL::File file_;
	};
}

namespace XTL
{
	Logger & DefaultLogger();

	void SetDefaultLogger(Logger * logger);

	void Warn(const char * message);

	template <typename T1>
	void Warn(const char * format, const T1 & t1)
	{
		DefaultLogger().Log(LOG_WARN, format, t1);
	}

	template <typename T1, typename T2>
	void Warn(const char * format, const T1 & t1, const T2 & t2)
	{
		DefaultLogger().Log(LOG_WARN, format, t1, t2);
	}

	template <typename T1, typename T2, typename T3>
	void Warn(const char * format, const T1 & t1, const T2 & t2, const T3 & t3)
	{
		DefaultLogger().Log(LOG_WARN, format, t1, t2, t3);
	}

	template <typename T1, typename T2, typename T3, typename T4>
	void Warn(const char * format, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4)
	{
		DefaultLogger().Log(LOG_WARN, format, t1, t2, t3, t4);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5>
	void Warn(const char * format, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5)
	{
		DefaultLogger().Log(LOG_WARN, format, t1, t2, t3, t4, t5);
	}
}

namespace XTL
{
	template <typename T1>
	void Logger::Log(LogLevel level, const char * format, const T1 & t1)
	{
		if (level.Value() <= minLevel_)
		{
			WriteMessage(level, XTL::FormatString(format, t1));
		}
	}

	template <typename T1, typename T2>
	void Logger::Log(LogLevel level, const char * format, const T1 & t1, const T2 & t2)
	{
		if (level.Value() <= minLevel_)
		{
			WriteMessage(level, XTL::FormatString(format, t1, t2));
		}
	}

	template <typename T1, typename T2, typename T3>
	void Logger::Log(LogLevel level, const char * format, const T1 & t1, const T2 & t2, const T3 & t3)
	{
		if (level.Value() <= minLevel_);
		{
			WriteMessage(level, XTL::FormatString(format, t1, t2, t3));
		}
	}

	template <typename T1, typename T2, typename T3, typename T4>
	void Logger::Log(LogLevel level, const char * format, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4)
	{
		if (level.Value() <= minLevel_);
		{
			WriteMessage(level, XTL::FormatString(format, t1, t2, t3, t4));
		}
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5>
	void Logger::Log(LogLevel level, const char * format, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5)
	{
		if (level.Value() <= minLevel_);
		{
			WriteMessage(level, XTL::FormatString(format, t1, t2, t3, t4, t5));
		}
	}
}

#endif
