#include "Logger.hpp"

#include <stdio.h>

namespace XTL
{
	Logger::~Logger() throw()
	{
		;;
	}


	FileLogger::FileLogger(FILE * file)
		: file_(file)
	{
		;;
	}

	FileLogger::~FileLogger() throw()
	{
		;;
	}

	FileLogger * FileLogger::StdOut()
	{
		static FileLogger instance(stdout);

		return &instance;
	}

	FileLogger * FileLogger::StdErr()
	{
		static FileLogger instance(stderr);

		return &instance;
	}

	void FileLogger::Log(const std::string & message)
	{
		fprintf(file_, "%s", message.c_str());
	}


	Warner & Warner::Instance()
	{
		static Warner instance;

		return instance;
	}

	Warner::Warner()
		: logger_(FileLogger::StdErr())
	{
		;;
	}

	Logger * Warner::SetLogger(Logger * logger)
	{
		Logger * oldLogger = Instance().logger_;
		Instance().logger_ = logger;
		return oldLogger;
	}

	void Warner::Log(const std::string & message)
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


	void Warn(const char * message)
	{
		Warner::Log(message);
	}
}
