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
				return Number::ZERO();
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

		IntegerBuilder integerBuilder;

		Advance();
		if (AtEnd() || NotInClass(CharClass::BINARY))
		{
			ThrowError("Binary digit expected");
		}

		try
		{
			do
			{
				integerBuilder.OnBinaryDigit(GetChar() - '0');

				Advance();
				if (AtEnd())
				{
					return integerBuilder.Release();
				}
			}
			while (InClass(CharClass::BINARY));
		}
		catch (const IntegerBuilder::OverflowError &)
		{
			ThrowError("Binary number is too big");
		}

		if (InClass(CharClass::DECIMAL))
		{
			ThrowError("Binary digit expected");
		}

		return integerBuilder.Release();
	}

	const Number NumberLiteralParser::ParseOctal()
	{
		// Assert( CharClass::DECIMAL.Contains(NeedChar()) )

		IntegerBuilder integerBuilder;

		if (NotInClass(CharClass::OCTAL))
		{
			ThrowError("Octal digit expected");
		}

		try
		{
			do
			{
				integerBuilder.OnOctalDigit(GetChar() - '0');

				Advance();
				if (AtEnd())
				{
					return integerBuilder.Release();
				}
			}
			while (InClass(CharClass::OCTAL));
		}
		catch (const IntegerBuilder::OverflowError &)
		{
			ThrowError("Octal number is too big");
		}

		if (InClass(CharClass::DECIMAL))
		{
			ThrowError("Octal digit expected");
		}

		return integerBuilder.Release();
	}

	const Number NumberLiteralParser::ParseHexadecimal()
	{
		// Assert( NeedChar() == 'x' )

		IntegerBuilder integerBuilder;

		Advance();
		if (AtEnd() || NotInClass(CharClass::HEXADECIMAL))
		{
			ThrowError("Hexadecimal digit expected");
		}

		try
		{
			do
			{
				integerBuilder.OnHexadecimalDigit(HexToInt(GetChar()));

				Advance();
				if (AtEnd())
				{
					return integerBuilder.Release();
				}
			}
			while (InClass(CharClass::HEXADECIMAL));
		}
		catch (const IntegerBuilder::OverflowError &)
		{
			ThrowError("Hexadecimal number is too big");
		}

		if (InClass(CharClass::LETTER))
		{
			ThrowError("Hexadecimal digit expected");
		}

		return integerBuilder.Release();
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
