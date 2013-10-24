#ifndef XTL__LOGGER_HPP__
#define XTL__LOGGER_HPP__ 1

#include "FormatString.hpp"

namespace XTL
{
	class Logger
	{
		public:

			virtual ~Logger() throw();

			virtual void Log(const std::string & message) = 0;
	};

	class FileLogger : public Logger
	{
		public:

			virtual ~FileLogger() throw();

			static FileLogger * StdOut();

			static FileLogger * StdErr();

			virtual void Log(const std::string & message);

		private:

			explicit FileLogger(FILE * file);

			FILE * file_;
	};

	class Warner
	{
		public:

			static Logger * SetLogger(Logger * logger);

			static void Log(const std::string & message);

		private:

			Warner();

			static Warner & Instance();

			Logger * logger_;
	};

	void Warn(const char * message);

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

	template <typename T1, typename T2, typename T3, typename T4>
	void Warn(const char * format, const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4)
	{
		Warner::Log(FormatString(format, t1, t2, t3, t4));
	}
}

#endif
