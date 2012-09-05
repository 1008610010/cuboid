#ifndef XTL__NUMBER_LITERAL_PARSER_HPP__
#define XTL__NUMBER_LITERAL_PARSER_HPP__ 1

#include <math.h>

#include "../TypeTraits.hpp"
#include "Parser.hpp"

namespace XTL
{
	class Number
	{
		public:

			enum Type
			{
				UNKNOWN  = 0,
				SIGNED   = 1,
				UNSIGNED = 2,
				FLOAT    = 3
			};

			Number()
				: type_(UNKNOWN)
			{
				;;
			}

			explicit Number(INT_64 value)
				: type_(SIGNED),
				  value_(value)
			{
				;;
			}

			explicit Number(UINT_64 value)
				: type_(UNSIGNED),
				  value_(value)
			{
				;;
			}

			explicit Number(double value)
				: type_(FLOAT),
				  value_(value)
			{
				;;
			}

		private:

			union Value
			{
				INT_64  i;
				UINT_64 u;
				double  f;

				Value() : i(0) { ;; }

				explicit Value(INT_64 i_) : i(i_) { ;; }
				explicit Value(UINT_64 u_) : u(u_) { ;; }
				explicit Value(double f_) : f(f_) { ;; }
			};

			Type  type_;
			Value value_;
	};

	class IntegerBuilder
	{
		public:

			class OverflowError {};

			static const UINT_64 MAX_UNSIGNED = TypeTraits<UINT_64>::MaxValue;
			static const UINT_64 MAX_SIGNED   = MAX_UNSIGNED / 2 + 1;

			IntegerBuilder()
				: value_(0),
				  negative_(false)
			{
				;;
			}

			bool IsSigned() const
			{
				return negative_ || value_ < MAX_SIGNED;
			}

			INT_64 ToSigned() const
			{
				return negative_ ? -value_ : value_;
			}

			UINT_64 ToUnsigned() const
			{
				return value_;
			}

			void Clear()
			{
				value_ = 0;
				negative_ = false;
			}

			void SetNegative()
			{
				if (!negative_)
				{
					if (value_ > MAX_SIGNED)
					{
						throw OverflowError();
					}

					negative_ = true;
				}
			}

			void AppendDecimal(unsigned int digit)
			{
				if (!CanAppendDecimal(digit))
				{
					throw OverflowError();
				}

				value_ = 10 * value_ + digit;
			}

			void AppendBinary(unsigned int digit)
			{
				if (!CanAppendBinary(digit))
				{
					throw OverflowError();
				}

				value_ = (value_ << 1) + digit;
			}

			void AppendOctal(unsigned int digit)
			{
				if (!CanAppendOctal(digit))
				{
					throw OverflowError();
				}

				value_ = (value_ << 3) + digit;
			}

			void AppendHexadecimal(unsigned int digit)
			{
				if (!CanAppendHexadecimal(digit))
				{
					throw OverflowError();
				}

				value_ = (value_ << 4) + digit;
			}

		private:

			bool CanAppendDecimal(unsigned int digit) const
			{
				if (negative_)
				{
					return value_ < MAX_SIGNED / 10 || (value_ == MAX_SIGNED / 10 && digit <= MAX_SIGNED % 10);
				}
				else
				{
					return value_ < MAX_UNSIGNED / 10 || (value_ == MAX_UNSIGNED / 10 && digit <= MAX_UNSIGNED % 10);
				}
			}

			bool CanAppendBinary(unsigned int digit) const
			{
				return value_ <= (MAX_UNSIGNED >> 1) + 1;
			}

			bool CanAppendOctal(unsigned int digit) const
			{
				return value_ <= (MAX_UNSIGNED >> 3) + 1;
			}

			bool CanAppendHexadecimal(unsigned int digit) const
			{
				return value_ <= (MAX_UNSIGNED >> 4) + 1;
			}

			UINT_64 value_;
			bool    negative_;
	};

	template <typename FloatType_>
	class FloatBuilder
	{
		public:

			typedef FloatType_ FloatType;

			FloatBuilder()
				: value_(0),
				  exponent_(0)
			{
				;;
			}

			void AppendInteger(int digit)
			{
				value_ = value_ * 10 + digit;
			}

			void AppendFractional(int digit)
			{
				AppendInteger(digit);
				--exponent_;
			}

			FloatType ToFloat() const
			{
				return value_ * ::pow(10.0, exponent_);
			}

		private:

			FloatType value_;
			int exponent_;
	};

	class NumberLiteralParser : public Parser
	{
		public:

			explicit NumberLiteralParser(CharSource & charSource)
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
					IntegerBuilder integer_;
					IntegerBuilder exponent_;
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
