#ifndef XTL__NUMBER_PARSER_HPP__
#define XTL__NUMBER_PARSER_HPP__ 1

#include <xtl/tp/Parser.hpp>

namespace XTL
{
	class NumberParser : public Parser
	{
		public:

			explicit NumberParser(CharSource & charSource)
				: Parser(charSource),
				  numberBuilder_()
			{
				;;
			}

			class Number
			{
				public:

					void OnIntegerDigit(int digit)
					{
						value.u = 10 * value.u + digit;
					}

				private:

					union Value
					{
						unsigned long long int u;
						long long int i;
						double d;

						Value()
							: u(0)
						{
							;;
						}
					};

					Value value;
			};

		protected:

			class NumberBuilder
			{
				public:

					void Reset()
					{
					}

					void SetNegative()
					{
					}

					void SetFloat()
					{
					}

					void SetNegativeExponent()
					{
					}

					void OnIntegerDigit(int digit)
					{
						;;
					}

					void OnFractionalDigit(int digit)
					{
						;;
					}

					void OnExponentDigit(int digit)
					{
						;;
					}

					void OnHexadecimalDigit(int digit)
					{
					}

					void OnOctalDigit(int digit)
					{
					}

					void OnBinaryDigit(int digit)
					{
						;;
					}
			};

			void Parse()
			{
				// Assert(NotAtEnd() && GetChar() =~ ["-", "0".."9"])

				numberBuilder_.Reset();

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

				if (c == '-')
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
			}

			void ParseBinary()
			{
				// Assert( NeedChar() == 'b' )

				Advance();
				if (AtEnd())
				{
					ThrowError("Binary digit expected");
				}

				char c = GetChar();
				if (!CharClass::BINARY.Contains(c))
				{
					ThrowError("Binary digit expected");
				}

				do
				{
					numberBuilder_.OnBinaryDigit(c - '0');
					Advance();
					if (AtEnd())
					{
						return;
					}
					c = GetChar();
				}
				while (CharClass::BINARY.Contains(c));

				if (CharClass::DECIMAL.Contains(c))
				{
					ThrowError("Binary digit expected");
				}
			}

			void ParseOctal()
			{
				// Assert( CharClass::DECIMAL.Contains(NeedChar()) )

				char c = GetChar();
				if (!CharClass::OCTAL.Contains(c))
				{
					ThrowError("Octal digit expected");
				}

				do
				{
					numberBuilder_.OnOctalDigit(c - '0');
					Advance();
					if (AtEnd())
					{
						return;
					}
					c = GetChar();
				}
				while (CharClass::OCTAL.Contains(c));

				if (CharClass::DECIMAL.Contains(c))
				{
					ThrowError("Octal digit expected");
				}
			}

			void ParseHexadecimal()
			{
				// Assert( NeedChar() == 'x' )

				char c = GetChar();
				if (!CharClass::HEXADECIMAL.Contains(c))
				{
					ThrowError("Hexadecimal digit expected");
				}

				do
				{
					numberBuilder_.OnHexadecimalDigit(HexToInt(c));
					Advance();
					if (AtEnd())
					{
						return;
					}
					c = GetChar();
				}
				while (CharClass::HEXADECIMAL.Contains(c));

				if (CharClass::LETTER.Contains(c))
				{
					ThrowError("Hexadecimal digit expected");
				}
			}

		private:

			int HexToInt(char c)
			{
				if (c >= 'a' && c <= 'f') return (c - 'a') + 10;
				if (c >= 'A' && c <= 'F') return (c - 'A') + 10;
				return c - '0';
			}

			void ParseFractional()
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

			void ParseExponent()
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

			NumberBuilder numberBuilder_;
	};
}

#endif
