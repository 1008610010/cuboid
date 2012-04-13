

#include <map>

/*
    SP* [ SP* \w+ SP* ] SP* [ # .* ]
[gui.accessibility]
visual_bell=yes

is equivalent to

gui.accessibility.visual_bell=yes
*/

class IniConfigSection
{
	public:

	private:
};


class IniConfig
{
	public:

		const IniConfigSection * GetSection(const std::string & name) const
		{
			return 0;
		}

	private:

		std::map<std::string, IniConfigSection *> sections_;
};




class CharSourceText
{
	public:

		typedef const char * Position;

		static const char CHAR_NULL = '\0';

		explicit CharSourceText(const char * source)
			: ptr_(source)
		{
			;;
		}

		bool AtEnd() const
		{
			return *ptr_ == CHAR_NULL;
		}

		bool NotAtEnd() const
		{
			return *ptr_ != CHAR_NULL;
		}

		void Advance()
		{
			++ptr_;
		}

		Position GetPosition() const
		{
			return ptr_;
		}

		char GetChar() const
		{
			return *ptr_;
		}

		const std::string GetString(Position begin) const
		{
			return ptr_ < begin ? std::string(begin, ptr_ - begin) : std::string();
		}

	private:

		const char * ptr_;
};

class IniConfigParser
{
	public:

		explicit IniConfigParser(CharSourceText & charSource)
			: charSource_(charSource),
			  currentSection_()
		{
			;;
		}

		void Parse()
		{
			while (charSource_.NotAtEnd())
			{
				SkipSpaces();

				if (charSource_.GetChar() == '\n')
				{
					charSource_.Advance();
				}
				else if (charSource_.GetChar() == '[')
				{
					ReadSectionName();
				}
				else if (charSource_.GetChar() == '#')
				{
					SkipComments();
				}
				else if (IsAlpha(charSource_.GetChar()))
				{
					// ReadParameter();
				}
				else
				{
					throw std::runtime_error("Unexpected token");
				}
			}
		}

	private:

		static bool IsSpace(char c)
		{
			return c == ' ' || c == '\t' || c == '\r';
		}

		static bool IsAlpha(char c)
		{
			return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
		}

		static bool IsDigit(char c)
		{
			return (c >= '0' && c <= '9');
		}

		void SkipSpaces()
		{
			while (IsSpace(charSource_.GetChar()))
			{
				charSource_.Advance();
			}
		}

		void SkipComments()
		{
			// ASSERT(charSource_.GetChar() == '#')
			do
			{
				charSource_.Advance();
			}
			while (charSource_.NotAtEnd() && charSource_.GetChar() != '\n');
		}

		const std::string ReadIdentifier()
		{
			// ASSERT(IsAlpha(charSource_.GetChar()))

			CharSourceText::Position begin = charSource_.GetPosition();

			do
			{
				charSource_.Advance();
			}
			while (IsAlpha(charSource_.GetChar()));

			return charSource_.GetString(begin);
		}

		void ReadSectionName()
		{
			// ASSERT(charSource_.GetChar() == '[')
			charSource_.Advance();
			SkipSpaces();

			if (!IsAlpha(charSource_.GetChar()))
			{
				throw std::runtime_error("Invalid section name: not an english letter or underscore");
			}

			currentSection_ = ReadIdentifier();

			SkipSpaces();

			if (charSource_.GetChar() != ']')
			{
				throw std::runtime_error("Invalid section name: unclosed section, ']' expected");
			}

			charSource_.Advance();
			SkipSpaces();

			if (charSource_.AtEnd())
			{
				return;
			}
			else if (charSource_.GetChar() == '#')
			{
				SkipComments();
			}
			else if (charSource_.GetChar() == '\n')
			{
				charSource_.Advance();
			}
			else
			{
				throw std::runtime_error("Unexpected token");
			}
		}

		Variant ReadParameterValue()
		{
			if (charSource_.GetChar() == '"')
			{
				
			}
			else if (IsDigit(charSource_.GetChar()) || charSource_.GetChar() == '-')
			{
				
			}
			else
			{
				throw std::runtime_error("Unexpected token, while parameter value expected");
			}
		}
/*
		void ReadParameter()
		{
			// ASSERT(IsAlpha(charSource_.GetChar()))

			if (currentSection_.empty())
			{
				throw std::runtime_error("Unexpected parameter, while section is not specified");
			}

			std::string key = ReadIdentifier();

			SkipSpaces();
			if (charSource_.GetChar() == '=' || charSource_.GetChar() == ':')
			{
				charSource_.Advance();
				SkipSpaces();
			}

			ReadParameterValue()
			if (charSource_.GetChar() == '"')
			{
			}
			else if (IsDigit(charSource_.GetC))
		if (*cur_ == '"')
		{
			std::string value;
			ParseStringLiteral(value);
			config_.Set(section, key, value);
		}
		else if (IsDigit(*cur_) || *cur_ == '-')
		{
			long long integer;
			double real;
			if (ParseNumberLiteral(integer, real))
			{
				config_.Set(section, key, integer);
			}
			else
			{
				config_.Set(section, key, real);
			}
		}
		else
		{
			RaiseError("Invalid symbol, while value expected");
		}

		GotoNextLine();
	}
		}
*/

		CharSourceText & charSource_;
		std::string      currentSection_;
};


