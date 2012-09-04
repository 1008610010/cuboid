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

		protected:

			class NumberBuilder
			{
				public:

					void Clear()
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
						exponent_.AppendDecimal(digit);
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

				private:

					bool isInteger_;
					IntegerBuilder<long long int> integer_;
					IntegerBuilder<long long int> exponent_;
			};

			void Parse();

		private:

			static int HexToInt(char c)
			{
				if (c >= 'a' && c <= 'f') return (c - 'a') + 10;
				if (c >= 'A' && c <= 'F') return (c - 'A') + 10;
				return c - '0';
			}

			void ParseBinary();

			void ParseOctal();

			void ParseHexadecimal();

			void ParseFractional();

			void ParseExponent();

			NumberBuilder numberBuilder_;
	};
}

#endif
