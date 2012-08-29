#include "CharClass.hpp"

#include "CharClassifier.hpp"
#include "Parser.hpp"

namespace XTL
{
	CharClass::CharClass(const CharClassifier & classifier, XTL::UINT_32 classBits)
		: classifier_(classifier),
		  classBits_(classBits)
	{
		;;
	}

	bool CharClass::Contains(char c) const
	{
		return (classifier_[c] & classBits_) != 0;
	}

	const CharClass CharClass::DECIMAL = Parser::CreateCommonClass(
		Parser::CHAR_DECIMAL
	);

	const CharClass CharClass::OCTAL = Parser::CreateCommonClass(
		Parser::CHAR_OCTAL
	);

	const CharClass CharClass::BINARY = Parser::CreateCommonClass(
		Parser::CHAR_BINARY
	);

	const CharClass CharClass::HEXADECIMAL = Parser::CreateCommonClass(
		Parser::CHAR_DECIMAL  |
		Parser::CHAR_HEX_UC   |
		Parser::CHAR_HEX_LC
	);

	const CharClass CharClass::IDENTIFIER_HEAD = Parser::CreateCommonClass(
		Parser::CHAR_LETTER_UC  |
		Parser::CHAR_LETTER_LC  |
		Parser::CHAR_UNDERSCORE
	);

	const CharClass CharClass::IDENTIFIER_TAIL = Parser::CreateCommonClass(
		Parser::CHAR_LETTER_UC  |
		Parser::CHAR_LETTER_LC  |
		Parser::CHAR_UNDERSCORE |
		Parser::CHAR_DECIMAL
	);


}

