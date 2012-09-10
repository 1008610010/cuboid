#ifndef XTL__NUMBER_LITERAL_PARSER_HPP__
#define XTL__NUMBER_LITERAL_PARSER_HPP__ 1

#include "Parser.hpp"
#include "NumberBuilder.hpp"

namespace XTL
{
	class NumberLiteralParser : protected Parser
	{
		public:

			explicit NumberLiteralParser(CharSource & charSource)
				: Parser(charSource)
			{
				;;
			}

			virtual ~NumberLiteralParser() throw() { ;; }

			virtual const Number Parse() = 0;

		protected:

			static int HexToInt(char c)
			{
				if (c >= 'a' && c <= 'f') return (c - 'a') + 10;
				if (c >= 'A' && c <= 'F') return (c - 'A') + 10;
				return c - '0';
			}
	};

	class IntegerLiteralParser : public NumberLiteralParser
	{
		public:

			explicit IntegerLiteralParser(CharSource & charSource);

			virtual ~IntegerLiteralParser() throw() { ;; }

			virtual const Number Parse();

		protected:

			const Number ParseBinary();

			const Number ParseOctal();

			const Number ParseHexadecimal();
	};

	class FloatLiteralParser : public IntegerLiteralParser
	{
		public:

			enum
			{
				ALLOW_TRAILING_POINT = 0x00000001,
				PARSE_BINARY         = 0x00000002,
				PARSE_OCTAL          = 0x00000004,
				PARSE_HEXADECIMAL    = 0x00000008
			};

			struct Config
			{
				const bool allow_trailing_point;
				const bool parse_binary;
				const bool parse_octal;
				const bool parse_hexadecimal;

				explicit Config(unsigned int flags)
					: allow_trailing_point ((flags & ALLOW_TRAILING_POINT) != 0),
					  parse_binary         ((flags & PARSE_BINARY) != 0),
					  parse_octal          ((flags & PARSE_OCTAL) != 0),
					  parse_hexadecimal    ((flags & PARSE_HEXADECIMAL) != 0)
				{
					;;
				}
			};

			explicit FloatLiteralParser(CharSource & charSource);

			virtual ~FloatLiteralParser() throw() { ;; }

			virtual const Number Parse();

		private:

			const Number ParseFractional(NumberBuilder & numberBuilder);

			const Number ParseExponent(NumberBuilder & numberBuilder);

			Config config_;
	};
}

#endif
