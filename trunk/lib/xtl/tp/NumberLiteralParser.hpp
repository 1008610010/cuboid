#ifndef XTL__NUMBER_PARSER_HPP__
#define XTL__NUMBER_PARSER_HPP__ 1

#include "../TypeTraits.hpp"
#include "Parser.hpp"

namespace XTL
{
	template <typename T>
	class IntegerBuilder
	{
		public:

			typedef typename TypeTraits<T>::Signed   Signed;
			typedef typename TypeTraits<T>::Unsigned Unsigned;

			static const Unsigned MAX_UNSIGNED = TypeTraits<Unsigned>::MaxValue;
			static const Unsigned MAX_SIGNED   = MAX_UNSIGNED / 2 + 1;

			IntegerBuilder()
				: value_(0),
				  negative_(false)
			{
				;;
			}

			Signed ToSigned() const
			{
				return negative_ ? -value_ : value_;
			}

			Unsigned ToUnsigned() const
			{
				return value_;
			}

			bool IsNegative() const
			{
				return negative_;
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
						throw std::runtime_error("Overflow");
					}

					negative_ = true;
				}
			}

			void AppendDecimal(unsigned int digit)
			{
				if (!CanAppendDecimal(digit))
				{
					throw std::runtime_error("Overflow");
				}

				value_ = 10 * value_ + digit;
			}

			void AppendBinary(unsigned int digit)
			{
				if (!CanAppendBinary(digit))
				{
					throw std::runtime_error("Overflow");
				}

				value_ = (value_ << 1) + digit;
			}

			void AppendOctal(unsigned int digit)
			{
				if (!CanAppendOctal(digit))
				{
					throw std::runtime_error("Overflow");
				}

				value_ = (value_ << 3) + digit;
			}

			void AppendHexadecimal(unsigned int digit)
			{
				if (!CanAppendHexadecimal(digit))
				{
					throw std::runtime_error("Overflow");
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

			Unsigned value_;
			bool     negative_;
	};

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
