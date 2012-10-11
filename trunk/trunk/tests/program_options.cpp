#include <stdio.h>
#include <string.h>

#include <map>
#include <stdexcept>
#include <string>

#include <xtl/FormatString.hpp>
#include <xtl/StringUtils.hpp>

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

#include <xtl/utils/AutoPtrVector.hpp>

namespace XTL
{
	class TerminateProgram
	{
		public:

			explicit TerminateProgram(int exitCode)
				: exitCode_(exitCode)
			{
				;;
			}

			int ExitCode() const
			{
				return exitCode_;
			}

		private:

			const int exitCode_;
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

			bool IsRequired() const
			{
				return (flags_ & XTL::PO::REQUIRED) != 0;
			}

			bool Exists() const
			{
				return exists_;
			}

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
}

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

namespace XTL
{
	class ProgramOptionsPool
	{
		public:

			class ParseError
			{
				public:

					explicit ParseError(const std::string & key)
						: key_(key)
					{
						;;
					}

				private:

					const std::string key_;
			};

			ProgramOptionsPool()
				: optionsList_(),
				  optionsMap_(),
				  optionsRequired_()
			{
				;;
			}

			ProgramOptionsPool & operator<< (std::auto_ptr<ProgramOption> option)
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

			void Parse(int argc, char * argv[])
			{
				int i = 0;
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
								OnOption(arg, 0);
							}
							else
							{
								OnOption(std::string(arg, v), v + 1);
							}

							++i;
						}
						else
						{
							ProgramOption * option = FindOption(arg);
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

								OnOption(arg, option, argv[i]);
								++i;
							}
							else
							{
								OnOption(arg, option, 0);
							}
						}
					}
					else
					{
						++i;
					}
				}

				for (unsigned int i = 0; i < optionsRequired_.size(); )
				{
					if (optionsRequired_[i]->Exists())
					{
						optionsRequired_.erase(optionsRequired_.begin() + i);
					}
					else
					{
						++i;
					}
				}

				if (!optionsRequired_.empty())
				{
					fprintf(stderr, "Missing required parameters:\n");
					for (unsigned int i = 0; i < optionsRequired_.size(); ++i)
					{
						fprintf(stderr, "  %s\n", optionsRequired_[i]->GetLabel());
					}

					throw TerminateProgram(1);
				}
			}

		private:

			ProgramOptionsPool(const ProgramOptionsPool &);
			ProgramOptionsPool & operator= (const ProgramOptionsPool &);

			ProgramOption * FindOption(const std::string & key) const
			{
				std::map<std::string, ProgramOption *>::const_iterator itr = optionsMap_.find(key);

				return itr != optionsMap_.end() ? itr->second : 0;
			}

			void OnOption(const std::string & key, char * value)
			{
				ProgramOption * option = FindOption(key);

				if (option == 0)
				{
					fprintf(stderr, "Unknown option '%s'\n", key.c_str());
					throw TerminateProgram(1);
				}

				OnOption(key, option, value);
			}

			void OnOption(const std::string & key, ProgramOption * option, char * value)
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

			XTL::AutoPtrVector<ProgramOption>      optionsList_;
			std::map<std::string, ProgramOption *> optionsMap_;
			std::vector<const ProgramOption *>     optionsRequired_;
	};
}

namespace XTL
{
namespace PO
{
	ProgramOptionsPool & ProgramOptions()
	{
		static ProgramOptionsPool instance;
		return instance;
	}
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

int main(int argc, char * argv[])
{
	bool argFlag;
	std::string argString;
	int argInteger;

	XTL::PO::ProgramOptions()
		<< PO::Flag    ("-f,--flag",    "Some description of this flag", argFlag)
		<< PO::String  ("-s",           "String",                        argString, XTL::PO::REQUIRED | XTL::PO::PASSWORD)
		<< PO::Integer ("-i,--integer", "Integer",                       argInteger, XTL::PO::REQUIRED);

	try
	{
		XTL::PO::ProgramOptions().Parse(argc, argv);
	}
	catch (const XTL::TerminateProgram & e)
	{
		return e.ExitCode();
	}

	printf("flag    = %s\n", argFlag ? "ON" : "OFF");
	printf("string  = %s\n", argString.c_str());
	printf("integer = %i\n", argInteger);
}
