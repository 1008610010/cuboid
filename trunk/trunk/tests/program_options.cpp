#include <stdio.h>
#include <string.h>

#include <list>
#include <map>
#include <stdexcept>
#include <string>

#include <xtl/FormatString.hpp>
#include <xtl/ProgramOptions.hpp>

/*
class ProgramOptions
{
	public:

		void PrintHelp()
		{
			XTL::PO::GlobalPool().Print(stderr);
		}

		struct Arguments
		{
			bool is_color;
			int  red;
			int  green;
			int  blue;
		} arguments;

* REQUIRED
* PASSWORD
* DEFAULT_VALUE

		PO::GlobalOptions()
			<< PO::Flag     ( OPTIONAL, "h", "help", 0, "Print this message",  PrintHelp )
			<< PO::FlagMode ( OPTIONAL, 0, "color", "Color of foreground text",  arguments.is_color)
				<< PO::Integer(REQUIRED, "r", "red",   "#", "Red component",   arguments.red)
				<< PO::Integer(REQUIRED, "g", "green", "#", "Green component", arguments.green)
				<< PO::Integer(REQUIRED, "b", "blue",  "#", "Blue component",  arguments.blue)
				<< PO::End()
			<< PO::String(REQUIRED, "f", "file", "FILEPATH", "File to write to")
			<< PO::String(OPTIONAL | PASSWORD, "p", "password", "S", password) // flag-string как в mysqldump - может иметь значение, а может не иметь, но присутствовать
			<< PO::SelectInteger(REQUIRED, "m", "month", 0, "Month of the year")
				<< PO::SelectChoice("jan", 1)
				<< PO::SelectChoice("Jan", 1)
				<< PO::SelectChoice("Dec", 12)
				<< PO::SelectEnd()
};
*/


/*
--------+------+---
 xxxxxx | xxxx | x  <-- First row - Alignment::CENTER
--------+------+---
 xxxxxx | xxxx | x
 xxxxxx | xxxx | x
 xxxxxx | xxxx | x
--------+------+---

void PrintDivider()
{
	printStream_.Print("-");
	PrintDashes(0);
	for (unsigned int j = 1; j < table_.ColumnsCount(); ++j)
	{
		printStream_.Print("-+-");
		PrintDashes(j);
	}
	printStream_.Print("-\n");
}

void PrintDashes(unsigned int columnIndex)
{
	XTL::CharRepeater<'-'>::Print(printStream_, table_.ColumnWidth(columnIndex));
}
*/

#include <xtl/utils/StringsTablePrinter.hpp>

#include <xtl/utils/BitUtils.hpp>
#include <xtl/linux/fs/FileUtils.hpp>


void ParseTablePath(const std::string & tablePath, std::string & tableDir, std::string & tableName)
{
	std::string::size_type solidus = tablePath.find_last_of('/');
	if (solidus != std::string::npos)
	{
		tableDir = tablePath.substr(0, solidus);
		if (tableDir.empty())
		{
			tableDir = "/";
		}
		else
		{
			tableDir = XTL::FileUtils::NormalizeFilePath(tableDir);
		}

		tableName = tablePath.substr(solidus + 1);
	}
	else
	{
		tableDir = ".";
		tableName = tablePath;
	}

	std::string::size_type dot = tableName.find_last_of('.');
	if (dot != std::string::npos && dot != 0)
	{
		tableName = tableName.substr(0, dot);
	}
}

int main(int argc, char * argv[])
{
	/*
	{
		XTL::StringsTable table;

		table
			.AddRow()
				.AddColumn("1")
				.AddColumn("abc asdf asdlkfj asdf a;lskdjf")
				.AddColumn("YES")
			.AddRow()
				.AddColumn("65536")
				.AddColumn("xyzm")
				.AddColumn("YES")
			.AddRow()
			.AddRow()
				.AddColumn("16")
				.AddColumn("R G B")
				.AddColumn("NO")
		;

		// XTL::StringsTablePrinter tablePrinter(XTL::StdOut());
		XTL::SimpleStringsTablePrinter tablePrinter(XTL::StdOut(), "  ", "", "  ");
		tablePrinter.SetAlignment(0, XTL::CellAlignment::RIGHT);

		tablePrinter.Print(table);

		// return 0;
	}
	*/

	bool argFlag;
	std::string argString;
	int argInteger;
	std::string argTablePath;
	unsigned int argJournalId;
	std::vector<std::string> argDitemIds;

	try
	{

		XTL::ProgramOptions().Map()
			// << PO::Flag       ("-f,--flag",      "Some description of this flag", argFlag)
			// << PO::String     ("-s NAME",        "String",                        argString,  XTL::PO::REQUIRED | XTL::PO::PASSWORD)
			// << PO::Integer    ("-i,--integer=#", "Integer value of argument",     argInteger, XTL::PO::REQUIRED)
			<< XTL::PO::PrintUsage ("-h,--help",      "Show this help")
		;

		XTL::ProgramOptions().List()
			<< XTL::PO::String     ("TABLE_PATH",     "Path to journal_stats table",   argTablePath, XTL::PO::REQUIRED | XTL::PO::PASSWORD)
			<< XTL::PO::Integer    ("JOURNAL_ID",     "Journal id",                    argJournalId, XTL::PO::REQUIRED)
			<< XTL::PO::Array      ("DITEM_ID",       "List of entries ditem_id",      argDitemIds,  XTL::PO::REQUIRED | XTL::PO::PASSWORD)
		;

		XTL::ProgramOptions().Parse(argc, argv);

		getc(stdin);
		std::string tableDir;
		std::string tableName;

		ParseTablePath(argTablePath, tableDir, tableName);

		printf("%s --- %s\n", tableDir.c_str(), tableName.c_str());
		printf("%s\n", XTL::FileUtils::JoinFilePath(tableDir, tableName).c_str());

		// XTL::ProgramOptions().PrintUsage(XTL::StdOut());
	}
	catch (const XTL::TerminateProgram & e)
	{
		return e.ExitCode();
	}

/*
	printf("flag    = %s\n", argFlag ? "ON" : "OFF");
	printf("string  = %s\n", argString.c_str());
	printf("integer = %i\n", argInteger);
*/

	/*
		try
		{
			XTL::PO::ProgramOptions().Map()
				<< PO::Flag    ("-f,--flag",    "Some description of this flag", argFlag)
				<< PO::String  ("-s",           "String",                        argString,    XTL::PO::REQUIRED | XTL::PO::PASSWORD)
				<< PO::Integer ("-i,--integer", "Integer",                       argInteger,   XTL::PO::REQUIRED);

			XTL::PO::ProgramOptions().List()
				<< PO::String  ("DIRECTORY",    "Path to events directory",      argDirectory, XTL::PO::REQUIRED)
				<< PO::String  ("FILE_NAME",    "Event file name",               argFileName,  XTL::PO::REQUIRED)
				<< PO::Integer ("POSITION",     "Position in event file",        argFilePos)
				<< PO::IntegerList ("JOURNAL_ID", argJournalIds)

			XTL::PO::ProgramOptions().Parse(argc, argv);

			program_name -s STRING -i INTEGER DIRECTORY FILE_NAME [POSITION] [JOURNAL_ID ...]

		}
		catch (XTL::TerminateProgram & e)
		{
			return e.ExitCode();
		}
	*/

}
