#include "Parser.hpp"

namespace XTL
{
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

	void Parser::SkipCharClass(const CharClass & charClass)
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
