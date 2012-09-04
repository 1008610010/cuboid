#include "NumberLiteralParser.hpp"

namespace XTL
{
	void NumberLiteralParser::Parse()
	{
		// Assert( NotAtEnd() && InClass(CharClass::NUMBER_HEAD) )

		numberBuilder_.Clear();

		char c = GetChar();

		if (c == '0')
		{
			Advance();
			if (AtEnd())
			{
				return;
			}

			c = GetChar();
			switch (c)
			{
				case 'b':
					ParseBinary(); break;

				case 'x':
					ParseHexadecimal(); break;

				case '.':
					ParseFractional(); break;

				case 'e':
				case 'E':
					ParseExponent(); break;

				default:
					if (CharClass::DECIMAL.Contains(c))
					{
						ParseOctal();
					}
			}

			return;
		}
		else if (c == '-')
		{
			numberBuilder_.SetNegative();
			Advance();
			if (AtEnd() || NotInClass(CharClass::DECIMAL))
			{
				ThrowError("Decimal digit expected");
			}
		}

		// Assert( NotAtEnd() && InClass(CharClass::DECIMAL) )
		do
		{
			numberBuilder_.OnIntegerDigit(GetChar() - '0');

			Advance();
			if (AtEnd())
			{
				return;
			}
		}
		while (InClass(CharClass::DECIMAL));

		if (GetChar() == '.')
		{
			ParseFractional();
		}
		else if (GetChar() == 'e' || GetChar() == 'E')
		{
			ParseExponent();
		}
	}

	void NumberLiteralParser::ParseBinary()
	{
		// Assert( NeedChar() == 'b' )

		Advance();
		if (AtEnd() || NotInClass(CharClass::BINARY))
		{
			ThrowError("Binary digit expected");
		}

		do
		{
			numberBuilder_.OnBinaryDigit(GetChar() - '0');
			Advance();
			if (AtEnd())
			{
				return;
			}
		}
		while (InClass(CharClass::BINARY));

		if (InClass(CharClass::DECIMAL))
		{
			ThrowError("Binary digit expected");
		}
	}

	void NumberLiteralParser::ParseOctal()
	{
		// Assert( CharClass::DECIMAL.Contains(NeedChar()) )

		if (NotInClass(CharClass::OCTAL))
		{
			ThrowError("Octal digit expected");
		}

		do
		{
			numberBuilder_.OnOctalDigit(GetChar() - '0');
			Advance();
			if (AtEnd())
			{
				return;
			}
		}
		while (InClass(CharClass::OCTAL));

		if (InClass(CharClass::DECIMAL))
		{
			ThrowError("Octal digit expected");
		}
	}

	void NumberLiteralParser::ParseHexadecimal()
	{
		// Assert( NeedChar() == 'x' )

		Advance();
		if (AtEnd() || NotInClass(CharClass::HEXADECIMAL))
		{
			ThrowError("Hexadecimal digit expected");
		}

		do
		{
			numberBuilder_.OnHexadecimalDigit(HexToInt(GetChar()));
			Advance();
			if (AtEnd())
			{
				return;
			}
		}
		while (InClass(CharClass::HEXADECIMAL));

		if (InClass(CharClass::LETTER))
		{
			ThrowError("Hexadecimal digit expected");
		}
	}

	void NumberLiteralParser::ParseFractional()
	{
		// Assert( NeedChar() == '.' )

		numberBuilder_.SetFloat();
		Advance();
		if (AtEnd() || NotInClass(CharClass::DECIMAL))
		{
			ThrowError("Decimal digit expected");
		}

		do
		{
			numberBuilder_.OnFractionalDigit(GetChar() - '0');

			Advance();
			if (AtEnd())
			{
				return;
			}
		}
		while (InClass(CharClass::DECIMAL));

		char c = GetChar();
		if (c == 'e' || c == 'E')
		{
			ParseExponent();
		}
	}

	void NumberLiteralParser::ParseExponent()
	{
		// Assert( NeedChar() == 'e' || NeedChar() == 'E' )

		Advance();
		if (AtEnd())
		{
			ThrowError("Decimal digit expected");
		}

		switch (GetChar())
		{
			case '-':
				numberBuilder_.SetNegativeExponent();
			case '+':
				Advance();
				if (AtEnd())
				{
					ThrowError("Decimal digit expected");
				}
				break;
		}

		if (NotInClass(CharClass::DECIMAL))
		{
			ThrowError("Decimal digit expected");
		}

		do
		{
			numberBuilder_.OnExponentDigit(GetChar() - '0');
			Advance();
		}
		while (NotAtEnd() && InClass(CharClass::DECIMAL));
	}
}
