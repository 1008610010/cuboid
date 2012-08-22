#include "PrintStream.hpp"

#include <stdio.h>

#include <stdexcept>

namespace XTL
{
	PrintStream & StandardPrintStream::Out()
	{
		static StandardPrintStream instance(stdout);
		return instance;
	}

	PrintStream & StandardPrintStream::Err()
	{
		static StandardPrintStream instance(stderr);
		return instance;
	}

	StandardPrintStream::StandardPrintStream(FILE * stream)
		: stream_(stream)
	{
		;;
	}

	StandardPrintStream::~StandardPrintStream() throw()
	{
		;;
	}

	void StandardPrintStream::Print(const char * s)
	{
		::fprintf(stream_, "%s", s);
	}

	void StandardPrintStream::Print(const std::string & s)
	{
		if (::fwrite(s.data(), 1, s.size(), stream_) != s.size())
		{
			throw std::runtime_error("Error in StandardPrintStream::Print()");
		}
	}
}

