#include "InitConfigParser.hpp"
#include "InitConfig.hpp"

namespace XTL
{
	InitConfigParser::InitConfigParser(const char * source, InitConfig & config)
		: Parser(source), config_(config)
	{
		Parse();
	}

	// *cur_ == ';'
	void InitConfigParser::SkipComments()
	{
		Advance();
		for (; *cur_ != '\0'; Advance())
		{
			if (IsNewLine(*cur_))
			{
				AdvanceNewLine();
				break;
			}
		}
	}

	void InitConfigParser::GotoNextLine()
	{
		SkipSpaces();

		if (*cur_ == ';')
		{
			SkipComments();
		}
		else if (IsNewLine(*cur_))
		{
			AdvanceNewLine();
		}
		else if (*cur_ != '\0')
		{
			throw Error(row_, column_, "Invalid symbol, while end of line expected");
		}
	}

	// before: *cur_ == '['
	// after:  begin of line or end of file
	void InitConfigParser::ReadSectionName(std::string & s)
	{
		Advance();
		SkipSpaces();

		const char * begin = cur_;
		for (; *cur_ != ']'; Advance())
		{
			if (*cur_ == '\0')
			{
				throw Error(row_, column_, "Unexpected end of file, while unclosed section");
			}
			else if (*cur_ == '\n')
			{
				throw Error(row_, column_, "Unexpected end of line, while unclosed section");
			}
		}

		const char * end = cur_ - 1;
		for (; IsSpace(*end) && begin < end; --end) { ;; }
		++end;
		if (begin == end)
		{
			throw Error(row_, column_, "Empty section name");
		}
		s.assign(begin, end - begin);

		Advance();

		GotoNextLine();
	}

	// IsAlpha(*cur)
	void InitConfigParser::ReadParameterPair(std::string & section)
	{
		if (section.empty())
		{
			throw Error(row_, column_, "Unexpected parameter, while section is not specified");
		}

		std::string key;
		ParseIdentifier(key);

		SkipSpaces();
		if (*cur_ == '=')
		{
			Advance();
			SkipSpaces();
		}

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
			throw Error(row_, column_, "Invalid symbol, while value expected");
		}

		GotoNextLine();
	}

	void InitConfigParser::Parse()
	{
		std::string section;

		while (*cur_ != '\0')
		{
			SkipSpaces();
			if (*cur_ == '\n')
			{
				AdvanceNewLine();
			}
			else if (*cur_ == '[')
			{
				ReadSectionName(section);
			}
			else if (*cur_ == ';')
			{
				SkipComments();
			}
			else if (IsAlpha(*cur_))
			{
				ReadParameterPair(section);
			}
			else
			{
				throw Error(row_, column_, "Section declaration or parameter key expected");
			}
		}
	}
}
