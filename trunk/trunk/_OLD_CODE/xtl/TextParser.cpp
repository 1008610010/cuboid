#include "TextParser.hpp"

namespace XTL
{
	const CharClass TextParser::DEFAULT_IDENTIFIER_HEAD(CommonCharClassifier::Instance(), CHAR_LETTER | CHAR_UNDERSCORE);

	const CharClass TextParser::DEFAULT_IDENTIFIER_TAIL(CommonCharClassifier::Instance(), CHAR_LETTER | CHAR_UNDERSCORE | CHAR_DIGIT);

	TextParser::TextParser(TextCharSource & charSource)
		: charSource_(charSource)
	{
		;;
	}

	const std::string TextParser::ReadIdentifier(const CharClass & headClass, const CharClass & tailClass)
	{
		if (IsInClass(headClass))
		{
			Mark();
			Advance();
			SkipCharClass(tailClass);

			return ReleaseString();
		}

		return std::string();
	}

	TextParser::CommonCharClassifier::CommonCharClassifier()
		: CharClassifier()
	{
		Add(CHAR_DIGIT,            '0', '9');
		Add(CHAR_LETTER_UPPERCASE, 'A', 'Z');
		Add(CHAR_LETTER_LOWERCASE, 'a', 'z');
		Add(CHAR_HEX_UPPERCASE,    'A', 'F');
		Add(CHAR_HEX_LOWERCASE,    'a', 'f');
		Add(CHAR_SPACE,            ' ');
		Add(CHAR_TAB,              '\t');
		Add(CHAR_CARRIAGE_RETURN,  '\r');
		Add(CHAR_LINE_FEED,        '\n');
		Add(CHAR_PLUS,             '+');
		Add(CHAR_MINUS,            '-');
		Add(CHAR_UNDERSCORE,       '_');
	}
}

