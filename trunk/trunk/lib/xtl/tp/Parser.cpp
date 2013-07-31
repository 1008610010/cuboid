#include "Parser.hpp"

namespace XTL
{
	Parser::Error::Error(const TextCursor & cursor, const std::string & what)
		: cursor_(cursor),
		  what_(what)
	{
		;;
	}

	Parser::Error::~Error() throw()
	{
		;;
	}

	const TextCursor & Parser::Error::Cursor() const throw()
	{
		return cursor_;
	}

	const char * Parser::Error::What() const throw()
	{
		return what_.c_str();
	}


	Parser::EndOfFile::~EndOfFile() throw()
	{
		;;
	}


	Parser::Parser(CharSource & charSource)
		: charSource_(charSource)
	{
		;;
	}

	char Parser::NeedChar() const
	{
		if (AtEnd())
		{
			throw EndOfFile();
		}

		return GetChar();
	}

	void Parser::WaitChar(char c)
	{
		while (NotAtEnd())
		{
			if (GetChar() == c)
			{
				return;
			}

			Advance();
		}

		throw EndOfFile();
	}

	bool Parser::SkipChar(char c)
	{
		if (AtEnd() || GetChar() != c)
		{
			return false;
		}

		Advance();
		return true;
	}

	bool Parser::SkipString(const char * s)
	{
		for (; *s != '\0'; ++s)
		{
			if (!SkipChar(*s))
			{
				return false;
			}
		}

		return true;
	}

	void Parser::SkipChars(char c)
	{
		while (NotAtEnd())
		{
			if (GetChar() != c)
			{
				return;
			}

			Advance();
		}

		throw EndOfFile();
	}

	void Parser::SkipCharClass2(const CharClass & charClass)
	{
		while (NotAtEnd())
		{
			if (!charClass.Contains(GetChar()))
			{
				return;
			}

			Advance();
		}

		throw EndOfFile();
	}

	const std::string Parser::ReadIdentifier()
	{
		// Assert( NotAtEnd() && CharClass::IDENTIFIER_HEAD.Contains(GetChar()) )

		Mark();

		do
		{
			Advance();
		}
		while (NotAtEnd() && InClass(CharClass::IDENTIFIER_TAIL));

		return ReleaseString();
	}

	void Parser::SkipLinearSpaces()
	{
		while (NotAtEnd() && InClass(CharClass::LINEAR_SPACE))
		{
			Advance();
		}
	}
}
