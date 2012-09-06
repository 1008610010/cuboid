#ifndef XTL__NUMBER_LITERAL_PARSER_HPP__
#define XTL__NUMBER_LITERAL_PARSER_HPP__ 1

#include <math.h>
#include <stdio.h>

#include "../TypeTraits.hpp"
#include "Parser.hpp"

namespace XTL
{
	class Number
	{
		public:

			enum Type
			{
				SIGNED_INTEGER   = 0,
				UNSIGNED_INTEGER = 1,
				FLOAT            = 2
			};

			explicit Number(INT_64 value)
				: value_(value),
				  type_(SIGNED_INTEGER)
			{
				;;
			}

			explicit Number(UINT_64 value)
				: value_(value),
				  type_(UNSIGNED_INTEGER)
			{
				;;
			}

			explicit Number(double value)
				: value_(value),
				  type_(FLOAT)
			{
				;;
			}

			static const Number ZERO()
			{
				static const Number instance(static_cast<INT_64>(0));

				return instance;
			}

			Type GetType() const
			{
				return type_;
			}

			bool IsInteger() const
			{
				return type_ != FLOAT;
			}

			bool IsFloat() const
			{
				return type_ == FLOAT;
			}

			INT_64 ToSignedInteger() const
			{
				return value_.i;
			}

			UINT_64 ToUnsignedInteger() const
			{
				return value_.u;
			}

			double ToFloat() const
			{
				return value_.f;
			}

			void DebugPrint()
			{
				switch (GetType())
				{
					case SIGNED_INTEGER:   printf("signed %lld\n", static_cast<long long int>(ToSignedInteger())); break;
					case UNSIGNED_INTEGER: printf("unsigned %llu\n", static_cast<unsigned long long int>(ToUnsignedInteger())); break;
					case FLOAT:            printf("float %g\n", ToFloat()); break;
				}
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

			Value value_;
			Type  type_;
	};

	class IntegerBuilder
	{
		public:

			class OverflowError {};

			static const UINT_64 MAX_UNSIGNED = TypeTraits<UINT_64>::MaxValue;
			static const UINT_64 MAX_SIGNED   = MAX_UNSIGNED / 2 + 1;

			IntegerBuilder()
				: value_(0),
				  isNegative_(false)
			{
				;;
			}

			const Number Release() const
			{
				if (isNegative_)
				{
					return Number(- static_cast<INT_64>(value_));
				}
				else
				{
					return Number(value_);
				}
			}

			void SetNegative()
			{
				if (isNegative_)
				{
					return;
				}

				if (value_ > MAX_SIGNED)
				{
					throw OverflowError();
				}

				isNegative_ = true;
			}

			void OnDecimalDigit(unsigned int digit)
			{
				if (CanAppendIntegerDigit(digit))
				{
					value_ = 10 * value_ + digit;
				}
				else
				{
					throw OverflowError();
				}
			}

			void OnBinaryDigit(unsigned int digit)
			{
				if (value_ > (MAX_UNSIGNED >> 1) + 1)
				{
					throw OverflowError();
				}

				value_ = (value_ << 1) + digit;
			}

			void OnOctalDigit(unsigned int digit)
			{
				if (value_ > (MAX_UNSIGNED >> 3) + 1)
				{
					throw OverflowError();
				}

				value_ = (value_ << 3) + digit;
			}

			void OnHexadecimalDigit(unsigned int digit)
			{
				if (value_ > (MAX_UNSIGNED >> 4) + 1)
				{
					throw OverflowError();
				}

				value_ = (value_ << 4) + digit;
			}

		private:

			bool CanAppendIntegerDigit(unsigned int digit) const
			{
				if (isNegative_)
				{
					return value_ < MAX_SIGNED / 10 || (value_ == MAX_SIGNED / 10 && digit <= MAX_SIGNED % 10);
				}
				else
				{
					return value_ < MAX_UNSIGNED / 10 || (value_ == MAX_UNSIGNED / 10 && digit <= MAX_UNSIGNED % 10);
				}
			}

			UINT_64 value_;
			bool    isNegative_;
	};

	class NumberBuilder
	{
		public:

			class OverflowError {};

			static const UINT_64 MAX_UNSIGNED = TypeTraits<UINT_64>::MaxValue;
			static const UINT_64 MAX_SIGNED   = MAX_UNSIGNED / 2 + 1;
			static const INT_32  MAX_EXPONENT_VALUE = 1000000;

			NumberBuilder()
				: i_(0),
				  f_(0.0),
				  exponent_(0),
				  exponentDelta_(0),
				  isNegative_(false),
				  isNegativeExponent_(false),
				  isFloat_(false)
			{
				;;
			}

			const Number Release() const
			{
				if (isFloat_)
				{
					INT_64 e = (isNegativeExponent_ ? -exponent_ : exponent_) + exponentDelta_;
					return Number((isNegative_ ? -f_ : f_) * ::pow(10.0, static_cast<double>(e)));
				}
				else if (isNegative_)
				{
					return Number(- static_cast<INT_64>(i_));
				}
				else
				{
					return Number(i_);
				}
			}

			void SetNegative()
			{
				if (isNegative_)
				{
					return;
				}

				isNegative_ = true;

				if (!isFloat_ && i_ > MAX_SIGNED)
				{
					// TODO: check config AUTOCAST_TO_FLOAT
					// throw OverflowError();
					SetFloat();
				}
			}

			bool CanAppendIntegerDigit(unsigned int digit) const
			{
				if (isNegative_)
				{
					return i_ < MAX_SIGNED / 10 || (i_ == MAX_SIGNED / 10 && digit <= MAX_SIGNED % 10);
				}
				else
				{
					return i_ < MAX_UNSIGNED / 10 || (i_ == MAX_UNSIGNED / 10 && digit <= MAX_UNSIGNED % 10);
				}
			}

			void OnIntegerDigit(unsigned int digit)
			{
				if (!isFloat_)
				{
					if (CanAppendIntegerDigit(digit))
					{
						i_ = 10 * i_ + digit;
						return;
					}

					SetFloat();
				}

				f_ = 10 * f_ + digit;
			}

			void OnFractionalDigit(int digit)
			{
				SetFloat();
				f_ = 10 * f_ + digit;
				--exponentDelta_;
			}

			void SetNegativeExponent()
			{
				isNegativeExponent_ = true;
			}

			void OnExponentDigit(int digit)
			{
				if (exponent_ < MAX_EXPONENT_VALUE)
				{
					exponent_ = 10 * exponent_ + digit;
				}
			}

			void SetFloat()
			{
				if (!isFloat_)
				{
					f_ = static_cast<double>(i_);
					isFloat_ = true;
				}
			}

		private:

			UINT_64 i_;
			double  f_;
			INT_32  exponent_;
			INT_32  exponentDelta_;
			bool    isNegative_;
			bool    isNegativeExponent_;
			bool    isFloat_;
	};

	/*
	 * Config:
	 *   ALLOW_FOLLOWING_LETTERS
	 *   ALLOW_BINARY
	 *   ALLOW_OCTAL
	 *   ALLOW_HEXADECIMAL
	*/
	class NumberLiteralParser : public Parser
	{
		public:

			explicit NumberLiteralParser(CharSource & charSource)
				: Parser(charSource)
			{
				;;
			}

			const Number Parse();

		private:

			static int HexToInt(char c)
			{
				if (c >= 'a' && c <= 'f') return (c - 'a') + 10;
				if (c >= 'A' && c <= 'F') return (c - 'A') + 10;
				return c - '0';
			}

			const Number ParseBinary();

			const Number ParseOctal();

			const Number ParseHexadecimal();

			const Number ParseFractional(NumberBuilder & numberBuilder);

			const Number ParseExponent(NumberBuilder & numberBuilder);
	};
}

#endif
