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
}

