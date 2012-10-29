#include "StringsTablePrinter.hpp"

namespace XTL
{
	const CellAlignment CellAlignment::LEFT   (0);
	const CellAlignment CellAlignment::CENTER (1);
	const CellAlignment CellAlignment::RIGHT  (2);

	CellAlignment::CellAlignment()
		: id_(0)
	{
		;;
	};

	bool CellAlignment::operator== (const CellAlignment & other) const
	{
		return id_ == other.id_;
	}

	CellAlignment::CellAlignment(unsigned int id)
		: id_(id)
	{
		;;
	}

	namespace
	{
		void PrintStringAligned(PrintStream & printStream, const std::string & value, CellAlignment alignment, unsigned int width)
		{
			if (width <= value.size())
			{
				printStream.Print(value);
				return;
			}

			unsigned int spaces = width - value.size();

			if (alignment == CellAlignment::RIGHT)
			{
				printStream.PrintSpaces(spaces);
				printStream.Print(value);
			}
			else if (alignment == CellAlignment::CENTER)
			{
				printStream.PrintSpaces(spaces / 2);
				printStream.Print(value);
				printStream.PrintSpaces(spaces - spaces / 2);
			}
			else if (alignment == CellAlignment::LEFT)
			{
				printStream.Print(value);
				printStream.PrintSpaces(spaces);
			}
			else
			{
				throw ILLEGAL_ARGUMENT_ERROR("Invalid parameter 'alignment'");
			}
		}
	}
}
