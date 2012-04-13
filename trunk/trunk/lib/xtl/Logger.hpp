#ifndef XTL__LOGGER_HPP__
#define XTL__LOGGER_HPP__ 1

#include "FormatString.hpp"

namespace XTL
{
	class Logger
	{
		public:

			virtual ~Logger() throw() { ;; }

			virtual void Log(const std::string & message) = 0;
	};

	class FileLogger : public Logger
	{
		public:

			virtual ~FileLogger() throw()
			{
				;;
			}

			static FileLogger * StdOut()
			{
				static FileLogger instance(stdout);

				return &instance;
			}

			static FileLogger * StdErr()
			{
				static FileLogger instance(stderr);

				return &instance;
			}

			virtual void Log(const std::string & message)
			{
				fprintf(file_, "%s", message.c_str());
			}

		private:

			explicit FileLogger(FILE * file)
				: file_(file)
			{
				;;
			}

			FILE * file_;
	};

	class Warner
	{
		public:

			static Logger * SetLogger(Logger * logger)
			{
				Logger * oldLogger = Instance().logger_;
				Instance().logger_ = logger;
				return oldLogger;
			}

			static void Log(const std::string & message)
			{
				Logger * logger = Instance().logger_;
				if (logger == 0)
				{
					return;
				}

				logger->Log("[WARN] ");
				logger->Log(message);
				if (message.length() == 0 || message[message.length() - 1] != '\n')
				{
					logger->Log("\n");
				}
			}

		private:

			Warner()
				: logger_(FileLogger::StdErr())
			{
				;;
			}

			static Warner & Instance()
			{
				static Warner instance;

				return instance;
			}

			Logger * logger_;
	};

	void Warn(const char * message)
	{
		Warner::Log(message);
	}

	template <typename T1>
	void Warn(const char * format, const T1 & t1)
	{
		Warner::Log(FormatString(format, t1));
	}

	template <typename T1, typename T2>
	void Warn(const char * format, const T1 & t1, const T2 & t2)
	{
		Warner::Log(FormatString(format, t1, t2));
	}

	template <typename T1, typename T2, typename T3>
	void Warn(const char * format, const T1 & t1, const T2 & t2, const T3 & t3)
	{
		Warner::Log(FormatString(format, t1, t2, t3));
	}

}

#endif

