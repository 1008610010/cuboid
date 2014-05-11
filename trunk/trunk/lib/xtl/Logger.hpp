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

			explicit LogLevel(int value);

			const std::string & Name() const;

			bool operator== (const LogLevel & other) const;

			bool operator<= (const LogLevel & other) const;

			bool operator>= (const LogLevel & other) const;

		private:

			int Value() const;

			int value_;
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

			LogLevel GetMinLogLevel() const;

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

			template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
			void Log(LogLevel level, const char * format, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6);

			template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
			void Log(LogLevel level, const char * format, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7);

			template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
			void Log(LogLevel level, const char * format, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8);

			virtual void Flush() = 0;

		protected:

			virtual void WriteMessage(const LogLevel & level, const std::string & message) = 0;

		private:

			Logger(const Logger &);
			Logger & operator= (const Logger &);

			LogLevel minLevel_;
	};

	class SimpleLogger : public Logger
	{
		public:

			virtual ~SimpleLogger() throw();

		protected:

			virtual void WriteMessage(const LogLevel & level, const std::string & message);

			void Write(const std::string & s);

			virtual void Write(const char * buffer, unsigned int size) = 0;
	};

	class NullLogger : public SimpleLogger
	{
		public:

			NullLogger();

			virtual ~NullLogger() throw();

			virtual void Flush();

		protected:

			virtual void Write(const char * buffer, unsigned int size);
	};

	class StdOutLogger : public SimpleLogger
	{
		public:

			virtual ~StdOutLogger() throw();

			virtual void Flush();

		protected:

			virtual void Write(const char * buffer, unsigned int size);
	};

	class StdErrLogger : public SimpleLogger
	{
		public:

			virtual ~StdErrLogger() throw();

			virtual void Flush();

		protected:

			virtual void Write(const char * buffer, unsigned int size);
	};

	class ConsoleLogger : public Logger
	{
		public:

			virtual ~ConsoleLogger() throw();

			virtual void Flush();

		protected:

			virtual void WriteMessage(const LogLevel & level, const std::string & message);

			void Write(FILE * stream, const char * buffer, unsigned int size);

			void Write(FILE * stream, const std::string & buffer);
	};

	class FileLogger : public SimpleLogger
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
	LogLevel LogLevelFromString(const std::string & level, LogLevel defaultValue);

	Logger & DefaultLogger();

	void SetDefaultLogger(Logger * logger);

	LogLevel CurrentLogLevel();

	void Log(LogLevel level, const std::string & message);

	template <typename T1>
	void Log(LogLevel level, const char * format, const T1 & t1)
	{
		DefaultLogger().Log(level, format, t1);
	}

	template <typename T1, typename T2>
	void Log(LogLevel level, const char * format, const T1 & t1, const T2 & t2)
	{
		DefaultLogger().Log(level, format, t1, t2);
	}

	template <typename T1, typename T2, typename T3>
	void Log(LogLevel level, const char * format, const T1 & t1, const T2 & t2, const T3 & t3)
	{
		DefaultLogger().Log(level, format, t1, t2, t3);
	}

	template <typename T1, typename T2, typename T3, typename T4>
	void Log(LogLevel level, const char * format, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4)
	{
		DefaultLogger().Log(level, format, t1, t2, t3, t4);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5>
	void Log(LogLevel level, const char * format, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5)
	{
		DefaultLogger().Log(level, format, t1, t2, t3, t4, t5);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	void Log(LogLevel level, const char * format, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6)
	{
		DefaultLogger().Log(level, format, t1, t2, t3, t4, t5, t6);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	void Log(LogLevel level, const char * format, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7)
	{
		DefaultLogger().Log(level, format, t1, t2, t3, t4, t5, t6, t7);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	void Log(LogLevel level, const char * format, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8)
	{
		DefaultLogger().Log(level, format, t1, t2, t3, t4, t5, t6, t7, t8);
	}

	void Warn(const std::string & message);

	template <typename T1>
	void Warn(const char * format, const T1 & t1)
	{
		Log(LOG_WARN, format, t1);
	}

	template <typename T1, typename T2>
	void Warn(const char * format, const T1 & t1, const T2 & t2)
	{
		Log(LOG_WARN, format, t1, t2);
	}

	template <typename T1, typename T2, typename T3>
	void Warn(const char * format, const T1 & t1, const T2 & t2, const T3 & t3)
	{
		Log(LOG_WARN, format, t1, t2, t3);
	}

	template <typename T1, typename T2, typename T3, typename T4>
	void Warn(const char * format, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4)
	{
		Log(LOG_WARN, format, t1, t2, t3, t4);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5>
	void Warn(const char * format, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5)
	{
		Log(LOG_WARN, format, t1, t2, t3, t4, t5);
	}
}

namespace XTL
{
	template <typename T1>
	void Logger::Log(LogLevel level, const char * format, const T1 & t1)
	{
		if (level <= minLevel_)
		{
			WriteMessage(level, XTL::FormatString(format, t1));
		}
	}

	template <typename T1, typename T2>
	void Logger::Log(LogLevel level, const char * format, const T1 & t1, const T2 & t2)
	{
		if (level <= minLevel_)
		{
			WriteMessage(level, XTL::FormatString(format, t1, t2));
		}
	}

	template <typename T1, typename T2, typename T3>
	void Logger::Log(LogLevel level, const char * format, const T1 & t1, const T2 & t2, const T3 & t3)
	{
		if (level <= minLevel_);
		{
			WriteMessage(level, XTL::FormatString(format, t1, t2, t3));
		}
	}

	template <typename T1, typename T2, typename T3, typename T4>
	void Logger::Log(LogLevel level, const char * format, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4)
	{
		if (level <= minLevel_);
		{
			WriteMessage(level, XTL::FormatString(format, t1, t2, t3, t4));
		}
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5>
	void Logger::Log(LogLevel level, const char * format, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5)
	{
		if (level <= minLevel_);
		{
			WriteMessage(level, XTL::FormatString(format, t1, t2, t3, t4, t5));
		}
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	void Logger::Log(LogLevel level, const char * format, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6)
	{
		if (level <= minLevel_);
		{
			WriteMessage(level, XTL::FormatString(format, t1, t2, t3, t4, t5, t6));
		}
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	void Logger::Log(LogLevel level, const char * format, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7)
	{
		if (level <= minLevel_);
		{
			WriteMessage(level, XTL::FormatString(format, t1, t2, t3, t4, t5, t6, t7));
		}
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	void Logger::Log(LogLevel level, const char * format, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8)
	{
		if (level <= minLevel_);
		{
			WriteMessage(level, XTL::FormatString(format, t1, t2, t3, t4, t5, t6, t7, t8));
		}
	}
}

#endif
