#include "Parser.hpp"

namespace XTL
{
	const CharClass Parser::IDENTIFIER_HEAD = Parser::CommonCharClassifier::Instance().CreateClass(
		Parser::CHAR_LETTER_UC  |
		Parser::CHAR_LETTER_LC  |
		Parser::CHAR_UNDERSCORE
	);

	const CharClass Parser::IDENTIFIER_TAIL = Parser::CommonCharClassifier::Instance().CreateClass(
		Parser::CHAR_LETTER_UC  |
		Parser::CHAR_LETTER_LC  |
		Parser::CHAR_UNDERSCORE |
		Parser::CHAR_DIGIT
	);

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

