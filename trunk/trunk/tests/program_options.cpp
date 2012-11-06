#include <stdio.h>
#include <string.h>

#include <list>
#include <map>
#include <stdexcept>
#include <string>

#include <xtl/Exception.hpp>
#include <xtl/FormatString.hpp>
#include <xtl/StringUtils.hpp>
#include <xtl/utils/AutoPtrVector.hpp>
#include <xtl/utils/StringsTable.hpp>
#include <xtl/utils/StringsTablePrinter.hpp>

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

namespace XTL
{
	class SimpleStringsTablePrinter : public StringsTablePrinter
	{
		public:

			SimpleStringsTablePrinter(PrintStream & printStream, const std::string & rowBegin, const std::string & rowEnd, const std::string & columnDivider)
				: StringsTablePrinter(printStream),
				  rowBegin_(rowBegin),
				  rowEnd_(rowEnd),
				  columnDivider_(columnDivider)
			{
				;;
			}

			virtual ~SimpleStringsTablePrinter() throw()
			{
				;;
			}

			virtual void Print(const StringsTable & table, unsigned int rowFrom, unsigned int rowTo)
			{
				if (!PrepareBeforePrint(table))
				{
					return;
				}

				for (unsigned int i = rowFrom; i < rowTo; ++i)
				{
					const StringsTable::Row & row = table.GetRow(i);

					PrintString(rowBegin_);

					for (unsigned int j = 0; j < table.ColumnsCount(); ++j)
					{
						if (j != 0)
						{
							PrintString(columnDivider_);
						}

						PrintStringAligned(row[j], GetAlignment(j), table.ColumnWidth(j));
					}

					PrintString(rowEnd_);
					PrintString("\n");
				}
			}

		private:

			const std::string rowBegin_;
			const std::string rowEnd_;
			const std::string columnDivider_;
	};
}

namespace XTL
{
namespace PO
{
	enum ProgramOptionFlags
	{
		REQUIRED   = 0x0001,
		PASSWORD   = 0x0002
	};
}
}

#include <xtl/PrintStream.hpp>

namespace XTL
{
	class ProgramOption
	{
		public:

			ProgramOption(const char * label, const char * text, unsigned int flags)
				: label_(label),
				  text_(text),
				  flags_(flags),
				  exists_(false)
			{
				;;
			}

			virtual ~ProgramOption() throw()
			{
				;;
			}

			const char * GetLabel() const
			{
				return label_;
			}

			const char * GetText() const
			{
				return text_;
			}

			const std::string GetTextWithRequired() const
			{
				static const std::string REQUIRED = "[Required] ";
				static const std::string OPTIONAL = "[Optional] ";

				return (IsRequired() ? REQUIRED : OPTIONAL) + GetText();
			}

			bool IsRequired() const
			{
				return (flags_ & XTL::PO::REQUIRED) != 0;
			}

			bool Exists() const
			{
				return exists_;
			}

			virtual bool IsArray() const = 0;

			virtual bool NeedValue() const = 0;

			virtual void Set(char * value)
			{
				exists_ = true;

				SetValue(value);

				if (value != 0 && (flags_ & XTL::PO::PASSWORD) != 0)
				{
					for (char * p = value; *p != '\0'; ++p)
					{
						*p = '*';
					}
				}
			}

		protected:

			virtual void SetValue(const char * value) = 0;

		private:

			const char * const label_;
			const char * const text_;
			const unsigned int flags_;
			bool               exists_;
	};

	class ProgramOptionsList
	{
		public:

			ProgramOptionsList()
				: optionsList_(),
				  currentIndex_(0)
			{
				;;
			}

			ProgramOptionsList & operator<< (std::auto_ptr<ProgramOption> option)
			{
				if (IsLastArray())
				{
					fprintf(stderr, "Internal error: Could not add list option, while array-option was added\n");
					throw TerminateProgram(1);
				}

				if (IsLastOptional() && option->IsRequired())
				{
					fprintf(stderr, "Internal error: Option #%u is required, while previous option is optional\n", static_cast<unsigned int>(optionsList_.Size() + 1));
					throw TerminateProgram(1);
				}

				optionsList_.PushBack(option);

				return *this;
			}

			bool IsEmpty() const
			{
				return optionsList_.IsEmpty();
			}

			unsigned int Size() const
			{
				return optionsList_.Size();
			}

			void Print(PrintStream & printStream)
			{
				for (unsigned int i = 0; i < optionsList_.Size(); ++i)
				{
					const ProgramOption * option = optionsList_[i];
					if (option->IsArray())
					{
						printStream.PrintF(" [%s ...]", option->GetLabel());
					}
					else if (option->IsRequired())
					{
						printStream.PrintF(" %s", option->GetLabel());
					}
					else
					{
						printStream.PrintF(" [%s]", option->GetLabel());
					}
				}
			}

			void PrintToTable(StringsTable & table) const
			{
				for (unsigned int i = 0; i < optionsList_.Size(); ++i)
				{
					table
						.AddRow()
							.AddColumn(optionsList_[i]->GetLabel())
							.AddColumn(optionsList_[i]->GetTextWithRequired());
				}
			}

		private:

			friend class ProgramOptionsPool;

			bool IsLastArray() const
			{
				return optionsList_.Size() > 0 && optionsList_.Back()->IsArray();
			}

			bool IsLastOptional() const
			{
				return optionsList_.Size() > 0 && !optionsList_.Back()->IsRequired();
			}

			void SetOptionValue(char * value)
			{
				if (currentIndex_ >= optionsList_.Size())
				{
					return;

					/*
					TODO: Print error and terminate program
					fprintf(stderr, "");
					throw TerminateProgram(1);
					*/
				}

				ProgramOption * option = optionsList_[currentIndex_];

				option->Set(value);

				if (!option->IsArray())
				{
					++currentIndex_;
				}
			}

			void CheckRequired(std::list<const ProgramOption *> & missingRequired)
			{
				for (unsigned int i = 0; i < optionsList_.Size(); ++i)
				{
					if (optionsList_[i]->IsRequired() && !optionsList_[i]->Exists())
					{
						missingRequired.push_back(optionsList_[i]);
					}
				}
			}

			XTL::AutoPtrVector<ProgramOption> optionsList_;
			unsigned int currentIndex_;
	};

	class ProgramOptionsMap
	{
		public:

			ProgramOptionsMap()
				: optionsList_(),
				  optionsMap_(),
				  optionsRequired_()
			{
				;;
			}

			// - _ A..Z a..z 0..9 ? ! @ $
			bool IsLabelNameChar(char c) const
			{
				return (c >= 'a' && c <= 'z') ||
				       (c >= 'A' && c <= 'Z') ||
				       (c >= '0' && c <= '9') ||
				       (c == '-') || (c == '_') ||
				       (c == '?') || (c == '!') ||
				       (c == '@') || (c == '$');
			}

			const std::string GetLabelName(const std::string & label)
			{
				unsigned int i = 0;
				while (i < label.length() && IsLabelNameChar(label[i]))
				{
					++i;
				}

				return label.substr(0, i);
			}

			ProgramOptionsMap & operator<< (std::auto_ptr<ProgramOption> option)
			{
				if (option->IsArray())
				{
					fprintf(stderr, "Internal error: Array option '%s' in map\n", option->GetLabel());
					throw TerminateProgram(1);
				}

				StringSplitter splitter(option->GetLabel(), ',');
				std::string label;
				while (splitter.GetNext(label))
				{
					if (label.size() == 0 || label[0] != '-')
					{
						fprintf(stderr, "Internal error: Invalid option's label '%s'\n", label.c_str());
						throw TerminateProgram(1);
					}

					const std::string labelName(GetLabelName(label));

					if (optionsMap_.count(labelName) > 0)
					{
						fprintf(stderr, "Internal error: Duplicate program option '%s'\n", label.c_str());
						throw TerminateProgram(1);
					}

					optionsMap_[labelName] = option.get();
				}

				if (option->IsRequired())
				{
					optionsRequired_.push_back(option.get());
				}

				optionsList_.PushBack(option);

				return *this;
			}

			bool IsEmpty() const
			{
				return optionsMap_.empty();
			}

			bool Contains(const std::string & key) const
			{
				return optionsMap_.count(key) > 0;
			}

			void PrintToTable(StringsTable & table) const
			{
				for (unsigned int i = 0; i < optionsList_.Size(); ++i)
				{
					table
						.AddRow()
							.AddColumn(optionsList_[i]->GetLabel())
							.AddColumn(optionsList_[i]->GetTextWithRequired());
				}
			}

		private:

			friend class ProgramOptionsPool;

			ProgramOption * FindOption(const std::string & key) const
			{
				std::map<std::string, ProgramOption *>::const_iterator itr = optionsMap_.find(key);

				return itr != optionsMap_.end() ? itr->second : 0;
			}

			void SetOptionValue(const std::string & key, char * value)
			{
				ProgramOption * option = FindOption(key);

				if (option == 0)
				{
					fprintf(stderr, "Unknown option '%s'\n", key.c_str());
					throw TerminateProgram(1);
				}

				SetOptionValue(key, option, value);
			}

			void SetOptionValue(const std::string & key, ProgramOption * option, char * value)
			{
				if (option->NeedValue())
				{
					if (value == 0)
					{
						fprintf(stderr, "Option '%s' requires value\n", key.c_str());
						throw TerminateProgram(1);
					}
				}
				else
				{
					if (value != 0)
					{
						fprintf(stderr, "Option '%s' does not require value\n", key.c_str());
						throw TerminateProgram(1);
					}
				}

				option->Set(value);
			}

			void CheckRequired(std::list<const ProgramOption *> & missingRequired)
			{
				for (unsigned int i = 0; i < optionsRequired_.size(); ++i)
				{
					if (!optionsRequired_[i]->Exists())
					{
						missingRequired.push_back(optionsRequired_[i]);
					}
				}
			}

			XTL::AutoPtrVector<ProgramOption>      optionsList_;
			std::map<std::string, ProgramOption *> optionsMap_;
			std::vector<const ProgramOption *>     optionsRequired_;
	};

	class ProgramOptionsPool
	{
		public:

			ProgramOptionsList & List()
			{
				return optionsList_;
			}

			ProgramOptionsMap & Map()
			{
				return optionsMap_;
			}

			void PrintUsage(PrintStream & printStream)
			{
				printStream.Print("\n");
				printStream.Print("Usage:\n");
				printStream.Print("\n");
				printStream.PrintF("  %s", programName_);

				if (!optionsMap_.IsEmpty())
				{
					printStream.Print(" {OPTIONS}");
				}

				optionsList_.Print(printStream);
				printStream.Print("\n");

				StringsTable optionsTable;

				optionsList_.PrintToTable(optionsTable);
				optionsMap_.PrintToTable(optionsTable);

				XTL::SimpleStringsTablePrinter tablePrinter(printStream, "  ", "", "  ");

				tablePrinter
					.SetAlignment(0, XTL::CellAlignment::LEFT)
					.SetAlignment(1, XTL::CellAlignment::LEFT);

				if (!optionsList_.IsEmpty())
				{
					printStream.Print("\n");
					printStream.Print("where:\n");

					tablePrinter.Print(optionsTable, 0, optionsList_.Size());
				}

				if (!optionsMap_.IsEmpty())
				{
					printStream.Print("\n");
					printStream.Print("where OPTIONS can be:\n");

					tablePrinter.Print(optionsTable, optionsList_.Size(), optionsTable.RowsCount());
				}

				printStream.Print("\n");

				throw TerminateProgram(0);
			}

			void Parse(int argc, char * argv[])
			{
				programName_ = argv[0];

				int i = 1;
				while (i < argc)
				{
					char * arg = argv[i];
					if (arg[0] == '-')
					{
						if (arg[1] == '-')
						{
							char * v = ::strchr(arg + 2, '=');

							if (v == 0)
							{
								optionsMap_.SetOptionValue(arg, 0);
							}
							else
							{
								optionsMap_.SetOptionValue(std::string(arg, v), v + 1);
							}

							++i;
						}
						else
						{
							ProgramOption * option = optionsMap_.FindOption(arg);
							if (option == 0)
							{
								optionsList_.SetOptionValue(arg);
								++i;
								continue;

								/*
								fprintf(stderr, "Unknown option '%s'\n", arg);
								throw TerminateProgram(1);
								*/
							}

							++i;

							if (option->NeedValue())
							{
								if (i == argc)
								{
									fprintf(stderr, "Option '%s' requires value\n", arg);
									throw TerminateProgram(1);
								}

								optionsMap_.SetOptionValue(arg, option, argv[i]);
								++i;
							}
							else
							{
								optionsMap_.SetOptionValue(arg, option, 0);
							}
						}
					}
					else
					{
						optionsList_.SetOptionValue(arg);
						++i;
					}
				}

				std::list<const ProgramOption *> missingRequired;

				optionsList_.CheckRequired(missingRequired);
				optionsMap_.CheckRequired(missingRequired);

				if (!missingRequired.empty())
				{
					fprintf(stderr, "Missing required parameters:\n");

					StringsTable table;
					for (std::list<const ProgramOption *>::const_iterator itr = missingRequired.begin(); itr != missingRequired.end(); ++itr)
					{
						table
							.AddRow()
								.AddColumn((*itr)->GetLabel())
								.AddColumn((*itr)->GetText());
					}

					XTL::SimpleStringsTablePrinter tablePrinter(XTL::StdErr(), "  ", "", "  ");

					tablePrinter
						.SetAlignment(0, XTL::CellAlignment::LEFT)
						.SetAlignment(1, XTL::CellAlignment::LEFT);

					tablePrinter.Print(table, 0, table.RowsCount());

					throw TerminateProgram(1);
				}
			}

		private:

			friend ProgramOptionsPool & ProgramOptions();

			ProgramOptionsPool()
				: programName_(),
				  optionsList_(),
				  optionsMap_()
			{
			}

			ProgramOptionsPool(const ProgramOptionsPool &);
			ProgramOptionsPool & operator= (const ProgramOptionsPool &);

			std::string        programName_;
			ProgramOptionsList optionsList_;
			ProgramOptionsMap  optionsMap_;
	};
}

namespace XTL
{
	ProgramOptionsPool & ProgramOptions()
	{
		static ProgramOptionsPool instance;
		return instance;
	}
}

namespace PO
{
	class ProgramOption_Boolean : public XTL::ProgramOption
	{
		public:

			ProgramOption_Boolean(const char * label, const char * text, unsigned int flags, bool & ref, bool defaultValue)
				: ProgramOption(label, text, flags),
				  ref_(ref)
			{
				ref_ = defaultValue;
			}

			virtual ~ProgramOption_Boolean() throw()
			{
				;;
			}

			virtual bool IsArray() const
			{
				return false;
			}

			virtual bool NeedValue() const
			{
				return false;
			}

			virtual void SetValue(const char * value)
			{
				ref_ = true;
			}

		private:

			bool & ref_;
	};

	typedef void (*Handler) (const char *);

	class ProgramOption_Function : public XTL::ProgramOption
	{
		public:

			explicit ProgramOption_Function(const char * label, const char * text, unsigned int flags, Handler handler)
				: ProgramOption(label, text, flags),
				  handler_(handler)
			{
				;;
			}

			virtual bool IsArray() const
			{
				return false;
			}

			virtual bool NeedValue() const
			{
				return false;
			}

			virtual void SetValue(const char * value)
			{
				handler_(value);
			}

		private:

			Handler handler_;
	};

	class ProgramOption_String : public XTL::ProgramOption
	{
		public:

			ProgramOption_String(const char * label, const char * text, unsigned int flags, std::string & ref, const char * defaultValue)
				: ProgramOption(label, text, flags),
				  ref_(ref)
			{
				ref_ = defaultValue;
			}

			virtual bool IsArray() const
			{
				return false;
			}

			virtual bool NeedValue() const
			{
				return true;
			}

			virtual void SetValue(const char * value)
			{
				ref_.assign(value);
			}

		private:

			std::string & ref_;
	};

	template <typename T>
	class ProgramOption_Integer : public XTL::ProgramOption
	{
		public:

			ProgramOption_Integer(const char * label, const char * text, unsigned int flags, T & ref, const T & defaultValue)
				: ProgramOption(label, text, flags),
				  ref_(ref)
			{
				ref_ = defaultValue;
			}

			virtual bool IsArray() const
			{
				return false;
			}

			virtual bool NeedValue() const
			{
				return true;
			}

			virtual void SetValue(const char * value)
			{
				if (!XTL::IsInteger(value))
				{
					fprintf(stderr, "Invalid value of parameter '%s' (integer expected)\n", GetLabel());
					throw XTL::TerminateProgram(1);
				}

				ref_ = XTL::StringToInteger<T>(value);
			}

		private:

			T & ref_;
	};

	class ProgramOption_StringArray : public XTL::ProgramOption
	{
		public:

			ProgramOption_StringArray(const char * label, const char * text, unsigned int flags, std::vector<std::string> & ref)
				: ProgramOption(label, text, flags),
				  ref_(ref)
			{
				;;
			}

			virtual bool IsArray() const
			{
				return true;
			}

			virtual bool NeedValue() const
			{
				return true;
			}

			virtual void SetValue(const char * value)
			{
				ref_.push_back(value);
			}

		private:

			std::vector<std::string> & ref_;
	};

	std::auto_ptr<XTL::ProgramOption> Flag(const char * label, const char * text, bool & result)
	{
		return std::auto_ptr<XTL::ProgramOption>(new ProgramOption_Boolean(label, text, 0, result, false));
	}

	std::auto_ptr<XTL::ProgramOption> String(const char * label, const char * text, std::string & result, unsigned int flags = 0)
	{
		return std::auto_ptr<XTL::ProgramOption>(new ProgramOption_String(label, text, flags, result, ""));
	}

	std::auto_ptr<XTL::ProgramOption> Integer(const char * label, const char * text, int & result, unsigned int flags = 0)
	{
		return std::auto_ptr<XTL::ProgramOption>(new ProgramOption_Integer<int>(label, text, flags, result, 0));
	}

	std::auto_ptr<XTL::ProgramOption> Integer(const char * label, const char * text, unsigned int & result, unsigned int flags = 0)
	{
		return std::auto_ptr<XTL::ProgramOption>(new ProgramOption_Integer<unsigned int>(label, text, flags, result, 0));
	}

	std::auto_ptr<XTL::ProgramOption> LongLong(const char * label, const char * text, long long int & result, unsigned int flags = 0)
	{
		return std::auto_ptr<XTL::ProgramOption>(new ProgramOption_Integer<long long int>(label, text, flags, result, 0));
	}

	std::auto_ptr<XTL::ProgramOption> LongLong(const char * label, const char * text, unsigned long long int & result, unsigned int flags = 0)
	{
		return std::auto_ptr<XTL::ProgramOption>(new ProgramOption_Integer<unsigned long long int>(label, text, flags, result, 0));
	}

	std::auto_ptr<XTL::ProgramOption> Function(const char * label, const char * text, Handler handler, unsigned int flags = 0)
	{
		return std::auto_ptr<XTL::ProgramOption>(new ProgramOption_Function(label, text, flags, handler));
	}

	std::auto_ptr<XTL::ProgramOption> Array(const char * label, const char * text, std::vector<std::string> & result, unsigned int flags = 0)
	{
		return std::auto_ptr<XTL::ProgramOption>(new ProgramOption_StringArray(label,text, flags, result));
	}

	void ProgramOptionsPrintUsage(const char * value)
	{
		XTL::ProgramOptions().PrintUsage(XTL::StdErr());
	}

	std::auto_ptr<XTL::ProgramOption> PrintUsage(const char * label, const char * text)
	{
		return Function(label, text, ProgramOptionsPrintUsage);
	}

/*
	std::auto_ptr<ProgramOption> FlagMode()
	{
	}
*/
}

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
			<< PO::PrintUsage ("-h,--help",      "Show this help")
		;

		XTL::ProgramOptions().List()
			<< PO::String     ("TABLE_PATH",     "Path to journal_stats table",   argTablePath, XTL::PO::REQUIRED | XTL::PO::PASSWORD)
			<< PO::Integer    ("JOURNAL_ID",     "Journal id",                    argJournalId, XTL::PO::REQUIRED)
			<< PO::Array      ("DITEM_ID",       "List of entries ditem_id",      argDitemIds,  XTL::PO::REQUIRED | XTL::PO::PASSWORD)
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
