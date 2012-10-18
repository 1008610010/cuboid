#include "PrintStream.hpp"

#include <stdio.h>

#include <stdexcept>

namespace XTL
{
	void PrintStream::PrintSpaces(unsigned int count)
	{
		XTL::CharRepeater<' '>::Print(*this, count);
	}

	PrintStream & StdOut()
	{
		static StandardPrintStream instance(stdout);
		return instance;
	}

	PrintStream & StdErr()
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

	void StandardPrintStream::Print(const char * s, unsigned int size)
	{
		if (size == 0)
		{
			return;
		}

		if (::fwrite(s, 1, size, stream_) != size)
		{
			// TODO: replace this with XTL::RuntimeError
			throw std::runtime_error("Error in StandardPrintStream::Print()");
		}
	}
}
