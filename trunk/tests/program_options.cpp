#include <stdio.h>
#include <string.h>

#include <map>
#include <stdexcept>
#include <string>

/*
struct ProgramOptionDesc
{
	const char   * label1;
	const char   * label2;
	const char   * value2;
	const char   * text;
	unsigned int   flags;
};

class ProgramOptions
{
	public:

		class Option
		{
			public:

		};

		void RegisterFlag(const char * label1, const char * label2, const char * value2)
		{
		}

		using namespace XTL::PO;

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


	private:

		std::list<Option> options_;
};
*/

#include <xtl/utils/AutoPtrVector.hpp>

class ProgramOption;

class ProgramOptionHandler
{
	public:

		virtual ~ProgramOptionHandler() throw() { ;; }

		virtual void Execute(const ProgramOption * option, const char * value) = 0;
};

class ProgramOptionHandler_String
{
	public:

		ProgramOptionHandler_String(std::string & dest)
			: dest_(dest)
		{
			;;
		}

		virtual ~ProgramOptionHandler_String() throw() { ;; }

		virtual void Execute(const ProgramOption * option, const char * value)
		{
			dest_.assign(value);
		}

	private:

		std::string & dest_;
};

class ProgramOption
{
	public:

		ProgramOption(const char * label, const char * text)
			: label_(label),
			  text_(text)
		{
			;;
		}

		const char * GetLabel() const
		{
			return label_;
		}

		bool IsRequired() const
		{
			return required_;
		}

		bool NeedValue() const
		{
			return false;
		}

	private:

		const char * const label_;
		const char * const text_;
		bool required_;
		unsigned int flags_;
};

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

class ProgramOptionsPool
{
	public:

		ProgramOptionsPool()
			: optionsList_(),
			  optionsMap_()
		{
			;;
		}

		ProgramOptionsPool & operator<< (std::auto_ptr<const ProgramOption> option)
		{
			StringSplitter splitter(option->GetLabel(), ',');
			std::string label;
			while (splitter.GetNext(label))
			{
				if (label.size() == 0 || label[0] != '-')
				{
					throw std::runtime_error("Invalid option's label");
				}

				if (optionsMap_.count(label) > 0)
				{
					throw std::runtime_error("Duplicate program option");
				}

				optionsMap_[label] = option.get();
				optionsList_.PushBack(option);
			}
		}

	private:

		ProgramOptionsPool(const ProgramOptionsPool &);
		ProgramOptionsPool & operator= (const ProgramOptionsPool &);

		const ProgramOption * FindOption(const std::string & key) const
		{
			std::map<std::string, const ProgramOption *>::const_iterator itr = optionsMap_.find(key);

			return itr != optionsMap_.end() ? itr->second : 0;
		}

		XTL::AutoPtrVector<const ProgramOption>      optionsList_;
		std::map<std::string, const ProgramOption *> optionsMap_;

};



namespace PO
{
/*
	std::auto_ptr<const ProgramOption> Flag(const char * label1, const char * label2)
	{
		return std::auto_ptr<const ProgramOption>(new ProgramOption(label1, label2));
	}
*/

/*
	std::auto_ptr<const ProgramOption> FlagMode()
	{
	}
*/
}

template <typename FUNC>
void SplitString(const char * source, char delimiter, FUNC func)
{
	const char * end;
	while ((end = ::strchr(source, delimiter)) != 0)
	{
		func(std::string(source, end - source));
		source = end + 1;
	}

	func(std::string(source));
}


void SplitLabels(const char * src)
{
	StringSplitter splitter(src, ',');
	std::string label;
	while (splitter.GetNext(label))
	{
		printf("[[[ %s\n", label.c_str());
	}
}

int main(int argc, char * argv[])
{
	SplitLabels("-f,--flag,--super-flag");
	ProgramOptionsPool globalPool;

	// PO::Flag("-f,--flag", "Some description of this flag", argFlag)

/*
	globalPool
		<< PO::Flag("f", "flag");
*/

	printf("Ok!\n");
}


