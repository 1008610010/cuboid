#include "Variant.hpp"

#include "stdio.h"

#include "PrintStream.hpp"

namespace XTL
{
	Variant::~Variant() throw()
	{
		;;
	}

	void Variant::DebugPrint(PrintStream & stream) const
	{
		DebugPrintValue(stream, 0);
		stream.Print("\n");
	}

	void Variant::PrintIndent(PrintStream & stream, unsigned int indent)
	{
		static const std::string SPACE("    ");

		while (indent > 0)
		{
			stream.Print(SPACE);
			--indent;
		}
	}
}
