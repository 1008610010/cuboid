#include "ProgramOptionsPrinter.hpp"
#include <stdio.h>

namespace XTL
{
	ProgramOptionsPrinter::ProgramOptionsPrinter(FILE * file, int columnSize)
		: f_(file), column_(columnSize, ' ')
	{
		;;
	}

	void ProgramOptionsPrinter::OnModeSelector(const OptionDesc * desc, const Option * option, int depth)
	{
		fprintf(f_, "  %s:\n", (Space(depth * 2) + desc->text).c_str());
	}

	void ProgramOptionsPrinter::OnModeValue(const OptionDesc * desc, const Option * option, int depth)
	{
		Print(std::string("  ") + Space(depth * 2) + desc->AsString(), desc->text);
	}

	void ProgramOptionsPrinter::OnOption(const OptionDesc * desc, const Option * option, int depth)
	{
		Print(std::string("  ") + Space(depth * 2) + desc->AsString(), desc->text);
	}

	void ProgramOptionsPrinter::Indent(int depth)
	{
		for (; depth > 0; --depth)
		{
			fprintf(f_, "  ");
		}
	}

	const std::string ProgramOptionsPrinter::Space(int size) const
	{
		return std::string(size, ' ');
	}

	void ProgramOptionsPrinter::Print(const std::string & first, const std::string & second)
	{
		if (first.length() >= column_.length())
		{
			fprintf(
				f_,
				"%s\n%s%s\n",
				first.c_str(),
				column_.c_str(),
				second.c_str()
			);
		}
		else
		{
			fprintf(
				f_,
				"%s%s\n",
				(first + Space(column_.length() - first.length())).c_str(),
				second.c_str()
			);
		}
	}
}
