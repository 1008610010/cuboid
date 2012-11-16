#include "ProgramOptions.hpp"

#include <stdio.h>
#include <string.h>

#include "Exception.hpp"
#include "StringUtils.hpp"
#include "utils/StringsTable.hpp"
#include "utils/StringsTablePrinter.hpp"

namespace XTL
{
	/********** ProgramOption **********/

	ProgramOption::ProgramOption(const char * label, const char * text, unsigned int flags)
		: label_(label),
		  text_(text),
		  flags_(flags),
		  exists_(false)
	{
		;;
	}

	ProgramOption::~ProgramOption() throw()
	{
		;;
	}

	const char * ProgramOption::GetLabel() const
	{
		return label_;
	}

	const char * ProgramOption::GetText() const
	{
		return text_;
	}

	const std::string ProgramOption::GetTextWithRequired() const
	{
		static const std::string REQUIRED = "[Required] ";
		static const std::string OPTIONAL = "[Optional] ";

		return (IsRequired() ? REQUIRED : OPTIONAL) + GetText();
	}

	bool ProgramOption::IsRequired() const
	{
		return (flags_ & XTL::PO::REQUIRED) != 0;
	}

	bool ProgramOption::Exists() const
	{
		return exists_;
	}

	void ProgramOption::Set(char * value)
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

	/********** ProgramOptionsList **********/

	ProgramOptionsList::ProgramOptionsList()
		: optionsList_(),
		  currentIndex_(0)
	{
		;;
	}

	bool ProgramOptionsList::IsEmpty() const
	{
		return optionsList_.IsEmpty();
	}

	unsigned int ProgramOptionsList::Size() const
	{
		return optionsList_.Size();
	}

	ProgramOptionsList & ProgramOptionsList::operator<< (std::auto_ptr<ProgramOption> option)
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

	bool ProgramOptionsList::IsLastArray() const
	{
		return optionsList_.Size() > 0 && optionsList_.Back()->IsArray();
	}

	bool ProgramOptionsList::IsLastOptional() const
	{
		return optionsList_.Size() > 0 && !optionsList_.Back()->IsRequired();
	}

	void ProgramOptionsList::SetOptionValue(char * value)
	{
		if (currentIndex_ >= optionsList_.Size())
		{
			return;

			/*
				MAYBE: Print error and terminate program?
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

	void ProgramOptionsList::CheckRequired(std::list<const ProgramOption *> & missingRequired)
	{
		for (unsigned int i = 0; i < optionsList_.Size(); ++i)
		{
			if (optionsList_[i]->IsRequired() && !optionsList_[i]->Exists())
			{
				missingRequired.push_back(optionsList_[i]);
			}
		}
	}

	void ProgramOptionsList::Print(PrintStream & printStream)
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

	void ProgramOptionsList::PrintToTable(StringsTable & table) const
	{
		for (unsigned int i = 0; i < optionsList_.Size(); ++i)
		{
			table
				.AddRow()
					.AddColumn(optionsList_[i]->GetLabel())
					.AddColumn(optionsList_[i]->GetTextWithRequired());
		}
	}

	/********** ProgramOptionsMap **********/

	ProgramOptionsMap::ProgramOptionsMap()
		: optionsList_(),
		  optionsMap_(),
		  optionsRequired_()
	{
		;;
	}

	bool ProgramOptionsMap::IsEmpty() const
	{
		return optionsMap_.empty();
	}

	namespace
	{
		// - _ A..Z a..z 0..9 ? ! @ $
		bool IsLabelNameChar(char c)
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
	}

	ProgramOptionsMap & ProgramOptionsMap::operator<< (std::auto_ptr<ProgramOption> option)
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

	ProgramOption * ProgramOptionsMap::FindOption(const std::string & key) const
	{
		std::map<std::string, ProgramOption *>::const_iterator itr = optionsMap_.find(key);

		return itr != optionsMap_.end() ? itr->second : 0;
	}

	void ProgramOptionsMap::SetOptionValue(const std::string & key, char * value)
	{
		ProgramOption * option = FindOption(key);

		if (option == 0)
		{
			fprintf(stderr, "Unknown option '%s'\n", key.c_str());
			throw TerminateProgram(1);
		}

		SetOptionValue(key, option, value);
	}

	void ProgramOptionsMap::SetOptionValue(const std::string & key, ProgramOption * option, char * value)
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

	void ProgramOptionsMap::CheckRequired(std::list<const ProgramOption *> & missingRequired)
	{
		for (unsigned int i = 0; i < optionsRequired_.size(); ++i)
		{
			if (!optionsRequired_[i]->Exists())
			{
				missingRequired.push_back(optionsRequired_[i]);
			}
		}
	}

	void ProgramOptionsMap::PrintToTable(StringsTable & table) const
	{
		for (unsigned int i = 0; i < optionsList_.Size(); ++i)
		{
			table
				.AddRow()
					.AddColumn(optionsList_[i]->GetLabel())
					.AddColumn(optionsList_[i]->GetTextWithRequired());
		}
	}

	/********** ProgramOptionsPool **********/

	ProgramOptionsPool::ProgramOptionsPool()
		: programName_(),
		  optionsList_(),
		  optionsMap_()
	{
		;;
	}

	ProgramOptionsList & ProgramOptionsPool::List()
	{
		return optionsList_;
	}

	ProgramOptionsMap & ProgramOptionsPool::Map()
	{
		return optionsMap_;
	}

	namespace
	{
		class ProgramOptionsTablePrinter : public StringsTablePrinter
		{
			public:

				ProgramOptionsTablePrinter(PrintStream & printStream, const std::string & rowBegin, const std::string & rowEnd, const std::string & columnDivider)
					: StringsTablePrinter(printStream),
					  rowBegin_(rowBegin),
					  rowEnd_(rowEnd),
					  columnDivider_(columnDivider)
				{
					;;
				}

				virtual ~ProgramOptionsTablePrinter() throw()
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

	void ProgramOptionsPool::Parse(int argc, char * argv[])
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
						if (optionsList_.IsEmpty())
						{
							fprintf(stderr, "Unknown option '%s'\n", arg);
							throw TerminateProgram(1);
						}

						optionsList_.SetOptionValue(arg);
						++i;
						continue;
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

			XTL::ProgramOptionsTablePrinter tablePrinter(XTL::StdErr(), "  ", "", "  ");

			tablePrinter
				.SetAlignment(0, XTL::CellAlignment::LEFT)
				.SetAlignment(1, XTL::CellAlignment::LEFT);

			tablePrinter.Print(table, 0, table.RowsCount());

			throw TerminateProgram(1);
		}
	}

	void ProgramOptionsPool::PrintUsage(PrintStream & printStream)
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

		XTL::ProgramOptionsTablePrinter tablePrinter(printStream, "  ", "", "  ");

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

	/********** ProgramOption Subclasses **********/

	namespace
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

		class ProgramOption_String : public XTL::ProgramOption
		{
			public:

				ProgramOption_String(const char * label, const char * text, unsigned int flags, std::string & ref, const char * defaultValue = 0)
					: ProgramOption(label, text, flags),
					  ref_(ref)
				{
					if (defaultValue != 0)
					{
						ref_ = defaultValue;
					}
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

		class ProgramOption_Choice : public XTL::ProgramOption
		{
			public:

				ProgramOption_Choice(const char * label, const char * text, unsigned int flags, unsigned int & ref, unsigned int defaultValue, const char * const * options)
					: ProgramOption(label, text, flags),
					  ref_(ref),
					  options_(options)
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
					unsigned int i = 0;

					for (const char * const * ptr = options_; *ptr != 0; ++ptr)
					{
						if (::strcmp(value, *ptr) == 0)
						{
							ref_ = i;
							return;
						}

						++i;
					}

					std::string allOptions;
					for (const char * const * ptr = options_; *ptr != 0; ++ptr)
					{
						if (!allOptions.empty())
						{
							allOptions += "|";
						}

						allOptions += *ptr;
					}

					fprintf(stderr, "Invalid value of parameter '%s' (%s)\n", GetLabel(), allOptions.c_str());
					throw XTL::TerminateProgram(1);
				}

			private:

				unsigned int & ref_;
				const char * const * const options_;
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

		class ProgramOption_Function : public XTL::ProgramOption
		{
			public:

				explicit ProgramOption_Function(const char * label, const char * text, unsigned int flags, XTL::PO::Handler handler)
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

				XTL::PO::Handler handler_;
		};
	}

	/******************************************************************/

	ProgramOptionsPool & ProgramOptions()
	{
		static ProgramOptionsPool instance;
		return instance;
	}

	namespace PO
	{
		std::auto_ptr<XTL::ProgramOption> Flag(const char * label, const char * text, bool & result)
		{
			return std::auto_ptr<XTL::ProgramOption>(new ProgramOption_Boolean(label, text, 0, result, false));
		}

		std::auto_ptr<XTL::ProgramOption> String(const char * label, const char * text, std::string & result, unsigned int flags)
		{
			return std::auto_ptr<XTL::ProgramOption>(new ProgramOption_String(label, text, flags, result));
		}

		std::auto_ptr<XTL::ProgramOption> Integer(const char * label, const char * text, int & result, unsigned int flags)
		{
			return std::auto_ptr<XTL::ProgramOption>(new ProgramOption_Integer<int>(label, text, flags, result, 0));
		}

		std::auto_ptr<XTL::ProgramOption> Integer(const char * label, const char * text, unsigned int & result, unsigned int flags)
		{
			return std::auto_ptr<XTL::ProgramOption>(new ProgramOption_Integer<unsigned int>(label, text, flags, result, 0));
		}

		std::auto_ptr<XTL::ProgramOption> LongLong(const char * label, const char * text, long long int & result, unsigned int flags)
		{
			return std::auto_ptr<XTL::ProgramOption>(new ProgramOption_Integer<long long int>(label, text, flags, result, 0));
		}

		std::auto_ptr<XTL::ProgramOption> LongLong(const char * label, const char * text, unsigned long long int & result, unsigned int flags)
		{
			return std::auto_ptr<XTL::ProgramOption>(new ProgramOption_Integer<unsigned long long int>(label, text, flags, result, 0));
		}

		std::auto_ptr<XTL::ProgramOption> Choice(const char * label, const char * text, unsigned int & result, const char * const * options, unsigned int flags)
		{
			return std::auto_ptr<XTL::ProgramOption>(new ProgramOption_Choice(label, text, flags, result, 0, options));
		}

		std::auto_ptr<XTL::ProgramOption> Function(const char * label, const char * text, Handler handler, unsigned int flags)
		{
			return std::auto_ptr<XTL::ProgramOption>(new ProgramOption_Function(label, text, flags, handler));
		}

		std::auto_ptr<XTL::ProgramOption> Array(const char * label, const char * text, std::vector<std::string> & result, unsigned int flags)
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
	}
}
