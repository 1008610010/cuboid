#include "NumberLiteralParser.hpp"

namespace XTL
{
	const Number NumberLiteralParser::Parse()
	{
		// Assert( NotAtEnd() && InClass(CharClass::NUMBER_HEAD) )

		char c = GetChar();

		NumberBuilder numberBuilder;

		if (c == '0')
		{
			Advance();
			if (AtEnd())
			{
				return Number(static_cast<INT_64>(0));
			}

			switch (GetChar())
			{
				case 'b': return ParseBinary();
				case 'x': return ParseHexadecimal();
				case '.': return ParseFractional(numberBuilder);

				case 'e':
				case 'E': return ParseExponent(numberBuilder);
			}

			if (InClass(CharClass::DECIMAL))
			{
				return ParseOctal();
			}

			return numberBuilder.Release();
		}
		else if (c == '-')
		{
			numberBuilder.SetNegative();
			Advance();
			if (AtEnd() || NotInClass(CharClass::DECIMAL))
			{
				ThrowError("Decimal digit expected");
			}
		}

		// Assert( NotAtEnd() && InClass(CharClass::DECIMAL) )
		do
		{
			numberBuilder.OnIntegerDigit(GetChar() - '0');

			Advance();
			if (AtEnd())
			{
				return numberBuilder.Release();
			}
		}
		while (InClass(CharClass::DECIMAL));

		if (GetChar() == '.')
		{
			return ParseFractional(numberBuilder);
		}
		else if (GetChar() == 'e' || GetChar() == 'E')
		{
			return ParseExponent(numberBuilder);
		}

		return numberBuilder.Release();
	}

	const Number NumberLiteralParser::ParseBinary()
	{
		// Assert( NeedChar() == 'b' )

		// TODO: replace with IntegerBuilder class
		NumberBuilder numberBuilder;

		Advance();
		if (AtEnd() || NotInClass(CharClass::BINARY))
		{
			ThrowError("Binary digit expected");
		}

		do
		{
			numberBuilder.OnBinaryDigit(GetChar() - '0');
			Advance();
			if (AtEnd())
			{
				return numberBuilder.Release();
			}
		}
		while (InClass(CharClass::BINARY));

		if (InClass(CharClass::DECIMAL))
		{
			ThrowError("Binary digit expected");
		}

		return numberBuilder.Release();
	}

	const Number NumberLiteralParser::ParseOctal()
	{
		// Assert( CharClass::DECIMAL.Contains(NeedChar()) )

		NumberBuilder numberBuilder;

		if (NotInClass(CharClass::OCTAL))
		{
			ThrowError("Octal digit expected");
		}

		do
		{
			numberBuilder.OnOctalDigit(GetChar() - '0');
			Advance();
			if (AtEnd())
			{
				return numberBuilder.Release();
			}
		}
		while (InClass(CharClass::OCTAL));

		if (InClass(CharClass::DECIMAL))
		{
			ThrowError("Octal digit expected");
		}

		return numberBuilder.Release();
	}

	const Number NumberLiteralParser::ParseHexadecimal()
	{
		// Assert( NeedChar() == 'x' )

		NumberBuilder numberBuilder;

		Advance();
		if (AtEnd() || NotInClass(CharClass::HEXADECIMAL))
		{
			ThrowError("Hexadecimal digit expected");
		}

		do
		{
			numberBuilder.OnHexadecimalDigit(HexToInt(GetChar()));
			Advance();
			if (AtEnd())
			{
				return numberBuilder.Release();
			}
		}
		while (InClass(CharClass::HEXADECIMAL));

		if (InClass(CharClass::LETTER))
		{
			ThrowError("Hexadecimal digit expected");
		}

		return numberBuilder.Release();
	}

	const Number NumberLiteralParser::ParseFractional(NumberBuilder & numberBuilder)
	{
		// Assert( NeedChar() == '.' )

		numberBuilder.SetFloat();
		Advance();
		if (AtEnd() || NotInClass(CharClass::DECIMAL))
		{
			ThrowError("Decimal digit expected");
		}

		do
		{
			numberBuilder.OnFractionalDigit(GetChar() - '0');

			Advance();
			if (AtEnd())
			{
				return numberBuilder.Release();
			}
		}
		while (InClass(CharClass::DECIMAL));

		char c = GetChar();
		if (c == 'e' || c == 'E')
		{
			return ParseExponent(numberBuilder);
		}

		return numberBuilder.Release();
	}

	const Number NumberLiteralParser::ParseExponent(NumberBuilder & numberBuilder)
	{
		// Assert( NeedChar() == 'e' || NeedChar() == 'E' )

		numberBuilder.SetFloat();
		Advance();
		if (AtEnd())
		{
			ThrowError("Decimal digit expected");
		}

		switch (GetChar())
		{
			case '-':
				numberBuilder.SetNegativeExponent();
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
			numberBuilder.OnExponentDigit(GetChar() - '0');
			Advance();
		}
		while (NotAtEnd() && InClass(CharClass::DECIMAL));

		return numberBuilder.Release();
	}
}
