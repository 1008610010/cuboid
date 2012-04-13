#include <stdio.h>

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

class ProgramOptionsPool
{
	public:
	private:
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

		ProgramOption(const char * label1, const char * label2)
			: label1_(label1 == 0 ? "" : label1),
			  label2_(label2 == 0 ? "" : label2)
		{
			;;
		}

		const std::string & Label1() const
		{
			return label1_;
		}

		const std::string & Label2() const
		{
			return label2_;
		}

	private:

		const std::string label1_;
		const std::string label2_;
};

class ProgramOptionsPool
{
	public:
	
		ProgramOptionsPool()
			: options_(),
			  optionsByLabel1_(),
			  optionsByLabel2_()
		{
			;;
		}

		ProgramOptionsPool & operator<< (std::auto_ptr<const ProgramOption> option)
		{
			const std::string & label1 = option->Label1();
			if (!label1.empty())
			{
				if (FindByLabel1(label1) != 0)
				{
					throw std::runtime_error("Duplicate program option");
				}
				optionsByLabel1_[label1] = option.get();
			}

			const std::string & label2 = option->Label2();
			if (!label2.empty())
			{
				if (FindByLabel2(label2) != 0)
				{
					throw std::runtime_error("Duplicate program option");
				}
				optionsByLabel2_[label2] = option.get();
			}

			options_.PushBack(option);

			return *this;
		}

		const ProgramOption * FindByLabel1(const std::string & label1) const
		{
			const std::map<std::string, const ProgramOption *>::const_iterator itr = optionsByLabel1_.find(label1);

			return itr != optionsByLabel1_.end() ? itr->second : 0;
		}

		const ProgramOption * FindByLabel2(const std::string & label2) const
		{
			const std::map<std::string, const ProgramOption *>::const_iterator itr = optionsByLabel2_.find(label2);

			return itr != optionsByLabel2_.end() ? itr->second : 0;
		}

	private:

		ProgramOptionsPool(const ProgramOptionsPool &);
		ProgramOptionsPool & operator= (const ProgramOptionsPool &);

		XTL::AutoPtrVector<const ProgramOption> options_;
		std::map<std::string, const ProgramOption *> optionsByLabel1_;
		std::map<std::string, const ProgramOption *> optionsByLabel2_;
};

namespace PO
{
	std::auto_ptr<const ProgramOption> Flag(const char * label1, const char * label2)
	{
		return std::auto_ptr<const ProgramOption>(new ProgramOption(label1, label2));
	}
/*
	std::auto_ptr<const ProgramOption> FlagMode()
	{
	}
*/
}

int main(int argc, char * argv[])
{
	ProgramOptionsPool globalPool;
	globalPool
		<< PO::Flag("f", "flag");

	printf("Ok!\n");
}


