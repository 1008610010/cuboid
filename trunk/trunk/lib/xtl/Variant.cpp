#include "Variant.hpp"

#include "stdio.h"

#include "PrintStream.hpp"

#include "VariantScalar.hpp"

namespace XTL
{
	Variant::~Variant() throw()
	{
		;;
	}

	VariantPtr Variant::Null()
	{
		return Variant::NullValue::Instance();
	}

	VariantPtr Variant::True()
	{
		return Variant::Boolean::True();
	}

	VariantPtr Variant::False()
	{
		return Variant::Boolean::False();
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
