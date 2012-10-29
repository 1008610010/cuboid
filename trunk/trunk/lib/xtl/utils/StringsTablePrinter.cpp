#include "StringsTablePrinter.hpp"

#include "../Exception.hpp"

namespace XTL
{
	const CellAlignment CellAlignment::LEFT   (0);
	const CellAlignment CellAlignment::CENTER (1);
	const CellAlignment CellAlignment::RIGHT  (2);

	CellAlignment::CellAlignment()
		: id_(0)
	{
		;;
	}

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

		}
	}

	StringsTablePrinter::StringsTablePrinter(PrintStream & printStream)
		: printStream_(printStream),
		  columns_()
	{
		;;
	}

	StringsTablePrinter::~StringsTablePrinter() throw()
	{
		;;
	}

	CellAlignment StringsTablePrinter::GetAlignment(unsigned int columnIndex) const
	{
		return columns_[columnIndex];
	}

	StringsTablePrinter & StringsTablePrinter::SetAlignment(unsigned int columnIndex, CellAlignment alignment)
	{
		if (columnIndex >= columns_.size())
		{
			columns_.resize(columnIndex + 1);
		}

		columns_[columnIndex] = alignment;
		return *this;
	}

	void StringsTablePrinter::Print(const StringsTable & table)
	{
		Print(table, 0, table.RowsCount());
	}

	void StringsTablePrinter::Print(const StringsTable & table, unsigned int rowFrom, unsigned int rowTo)
	{
		if (!PrepareBeforePrint(table))
		{
			return;
		}

		for (unsigned int i = rowFrom; i < rowTo; ++i)
		{
			const StringsTable::Row & row = table.GetRow(i);

			for (unsigned int j = 0; j < table.ColumnsCount(); ++j)
			{
				if (j != 0)
				{
					PrintString(" ");
				}

				PrintStringAligned(row[j], GetAlignment(j), table.ColumnWidth(j));
			}

			PrintString("\n");
		}
	}

	bool StringsTablePrinter::PrepareBeforePrint(const StringsTable & table)
	{
		if (table.ColumnsCount() == 0)
		{
			return false;
		}

		if (table.ColumnsCount() > columns_.size())
		{
			columns_.resize(table.ColumnsCount());
		}

		return true;
	}

	void StringsTablePrinter::PrintString(const char * value)
	{
		printStream_.Print(value);
	}

	void StringsTablePrinter::PrintString(const std::string & value)
	{
		printStream_.Print(value);
	}

	void StringsTablePrinter::PrintStringAligned(const std::string & value, CellAlignment alignment, unsigned int width)
	{
		if (width <= value.size())
		{
			printStream_.Print(value);
			return;
		}

		unsigned int spaces = width - value.size();

		if (alignment == CellAlignment::RIGHT)
		{
			printStream_.PrintSpaces(spaces);
			printStream_.Print(value);
		}
		else if (alignment == CellAlignment::CENTER)
		{
			printStream_.PrintSpaces(spaces / 2);
			printStream_.Print(value);
			printStream_.PrintSpaces(spaces - spaces / 2);
		}
		else if (alignment == CellAlignment::LEFT)
		{
			printStream_.Print(value);
			printStream_.PrintSpaces(spaces);
		}
		else
		{
			throw ILLEGAL_ARGUMENT_ERROR("Invalid parameter 'alignment'");
		}
	}
}
