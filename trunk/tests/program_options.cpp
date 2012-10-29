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
	class StringSplitter
	{
		public:

			StringSplitter(const char * source, char delimiter)
				: source_(source),
				  begin_(source),
				  delimiter_(delimiter)
			{
				;;
			}

			bool GetNext(std::string & result)
			{
				if (begin_ == 0)
				{
					return false;
				}

				const char * end = ::strchr(begin_, delimiter_);
				if (end != 0)
				{
					result.assign(begin_, end - begin_);
					begin_ = end + 1;
				}
				else
				{
					result.assign(begin_);
					begin_ = 0;
				}

				return true;
			}

		private:

			const char * const source_;
			const char * begin_;
			const char delimiter_;
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
					fprintf(stderr, "Internal error: could not add list option, while array-option was added");
					throw TerminateProgram(1);
				}

				if (IsLastOptional() && option->IsRequired())
				{
					fprintf(stderr, "Internal error: option #%u is required, while previous option is optional\n", static_cast<unsigned int>(optionsList_.Size() + 1));
					throw TerminateProgram(1);
				}

				optionsList_.PushBack(option);

				return *this;
			}

			void Print()
			{
				for (unsigned int i = 0; i < optionsList_.Size(); ++i)
				{
					const ProgramOption * option = optionsList_[i];
					if (option->IsArray())
					{
						fprintf(stderr, " [%s ...]", option->GetLabel());
					}
					else if (option->IsRequired())
					{
						fprintf(stderr, " %s", option->GetLabel());
					}
					else
					{
						fprintf(stderr, " [%s]", option->GetLabel());
					}
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
				for (unsigned int i = currentIndex_; i < optionsList_.Size() && optionsList_[i]->IsRequired(); ++i)
				{
					missingRequired.push_back(optionsList_[i]);
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

			ProgramOptionsMap & operator<< (std::auto_ptr<ProgramOption> option)
			{
				StringSplitter splitter(option->GetLabel(), ',');
				std::string label;
				while (splitter.GetNext(label))
				{
					if (label.size() == 0 || label[0] != '-')
					{
						fprintf(stderr, "Internal error: Invalid option's label '%s'\n", label.c_str());
						throw TerminateProgram(1);
					}

					if (optionsMap_.count(label) > 0)
					{
						fprintf(stderr, "Internal error: Duplicate program option '%s'\n", label.c_str());
						throw TerminateProgram(1);
					}

					optionsMap_[label] = option.get();
				}

				if (option->IsRequired())
				{
					optionsRequired_.push_back(option.get());
				}

				optionsList_.PushBack(option);

				return *this;
			}

			bool Empty() const
			{
				return optionsMap_.empty();
			}

			bool Contains(const std::string & key) const
			{
				return optionsMap_.count(key) > 0;
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

			void PrintUsage()
			{
				fprintf(stderr, "Usage:\n");
				fprintf(stderr, "\n");
				fprintf(stderr, "%s", programName_.c_str());

				if (!optionsMap_.Empty())
				{
					fprintf(stderr, " {OPTIONS}");
				}

				optionsList_.Print();
				fprintf(stderr, "\n");
				fprintf(stderr, "\n");

				fprintf(stderr, "where {OPTIONS} can be:\n");

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
								fprintf(stderr, "Unknown option '%s'\n", arg);
								throw TerminateProgram(1);
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

					for (std::list<const ProgramOption *>::const_iterator itr = missingRequired.begin(); itr != missingRequired.end(); ++itr)
					{
						fprintf(stderr, "  %s\n", (*itr)->GetLabel());
					}

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

	class ProgramOption_Handler : public XTL::ProgramOption
	{
		public:

			explicit ProgramOption_Handler(const char * label, const char * text, unsigned int flags, Handler handler)
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

	std::auto_ptr<XTL::ProgramOption> Flag(const char * label, const char * text, bool & result)
	{
		return std::auto_ptr<XTL::ProgramOption>(new ProgramOption_Boolean(label, text, 0, result, false));
	}

	std::auto_ptr<XTL::ProgramOption> String(const char * label, const char * text, std::string & result, unsigned int flags = 0)
	{
		return std::auto_ptr<XTL::ProgramOption>(new ProgramOption_String(label, text, flags, result, ""));
	}

	template <typename T>
	std::auto_ptr<XTL::ProgramOption> Integer(const char * label, const char * text, T & result, unsigned int flags = 0)
	{
		return std::auto_ptr<XTL::ProgramOption>(new ProgramOption_Integer<T>(label, text, flags, result, 0));
	}

/*
	std::auto_ptr<ProgramOption> FlagMode()
	{
	}
*/
}

/*

 xxxxxx | xxxx | x
--------+------+---
 xxxxxx | xxxx | x
 xxxxxx | xxxx | x
 xxxxxx | xxxx | x

*/

#include <vector>
#include <xtl/utils/StringsTablePrinter.hpp>

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

			virtual void Print(const StringsTable & table)
			{
				if (!PrepareBeforePrint(table))
				{
					return;
				}

				for (unsigned int i = 0; i < table.RowsCount(); ++i)
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

	/*
	struct ColumnDesc
	{
		ColumnDesc();

		void UpdateWidth(unsigned int newWidth);

		StringAlignment alignment;
		unsigned int    width;
		std::string     title;
	};

	StringsTable & SetColumn(unsigned int columnIndex, StringAlignment alignment, const std::string & title)
	{
		ColumnDesc & columnDesc = CreateColumn(columnIndex);
		columnDesc.alignment = alignment;
		columnDesc.title = title;

		columnDesc.UpdateWidth(title.size());

		return *this;
	}

	class StringTablePrinter
	{
		public:

			explicit StringTablePrinter(const StringsTable & table, PrintStream & printStream)
				: table_(table),
				  printStream_(printStream)
			{
				;;
			}

			void Print()
			{
				if (table_.ColumnsCount() == 0)
				{
					return;
				}

				PrintDivider();

				printStream_.Print(" ");
				table_.PrintColumnTitle(printStream_, 0);
				for (unsigned int j = 1; j < table_.ColumnsCount(); ++j)
				{
					printStream_.Print(" | ");
					table_.PrintColumnTitle(printStream_, j);
				}
				printStream_.Print("\n");

				PrintDivider();

				for (unsigned int i = 0; i < table_.RowsCount(); ++i)
				{
					printStream_.Print(" ");

					table_.PrintAligned(printStream_, i, 0);

					for (unsigned j = 1; j < table_.ColumnsCount(); ++j)
					{
						printStream_.Print(" | ");
						table_.PrintAligned(printStream_, i, j);
					}

					printStream_.Print("\n");
				}

				PrintDivider();
			}

		private:

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

			const StringsTable & table_;
			PrintStream        & printStream_;
	};
	*/
}

int main(int argc, char * argv[])
{
	{
		XTL::StringsTable table;

		table
			/*
			.SetColumn(0, XTL::StringsTable::RIGHT,  "id")
			.SetColumn(1, XTL::StringsTable::CENTER, "description")
			.SetColumn(2, XTL::StringsTable::LEFT,   "flag")
			*/
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

		/*
		XTL::StringTablePrinter(table, XTL::StdOut()).Print();
		*/
		return 0;
	}

	bool argFlag;
	std::string argString;
	int argInteger;
	unsigned int argPeriodId;

	try
	{
		XTL::ProgramOptions().Map()
			<< PO::Flag    ("-f,--flag",    "Some description of this flag", argFlag)
			<< PO::String  ("-s",           "String",                        argString,  XTL::PO::REQUIRED | XTL::PO::PASSWORD)
			<< PO::Integer ("-i,--integer", "Integer",                       argInteger, XTL::PO::REQUIRED);

		XTL::ProgramOptions().List()
			<< PO::Integer ("PERIOD_ID",    "Period number",                 argPeriodId, XTL::PO::REQUIRED);

		XTL::ProgramOptions().Parse(argc, argv);

		XTL::ProgramOptions().PrintUsage();
	}
	catch (const XTL::TerminateProgram & e)
	{
		return e.ExitCode();
	}

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


	printf("flag    = %s\n", argFlag ? "ON" : "OFF");
	printf("string  = %s\n", argString.c_str());
	printf("integer = %i\n", argInteger);
}
