#ifndef XTL__NUMBER_BUILDER_HPP__
#define XTL__NUMBER_BUILDER_HPP__ 1

#include "../Types.hpp"
#include "../TypeTraits.hpp"

namespace XTL
{
	class Number
	{
		public:

			enum Type
			{
				SIGNED_INTEGER   = 0,
				UNSIGNED_INTEGER = 1,
				RATIONAL         = 2
			};

			explicit Number(INT_64 value)
				: value_(value),
				  type_(SIGNED_INTEGER) { ;; }

			explicit Number(UINT_64 value)
				: value_(value),
				  type_(UNSIGNED_INTEGER) { ;; }

			explicit Number(double value)
				: value_(value),
				  type_(RATIONAL) { ;; }

			static const Number ZERO()
			{
				static const Number instance(static_cast<INT_64>(0));

				return instance;
			}

			Type GetType() const              { return type_; }

			bool IsInteger() const            { return type_ != RATIONAL; }

			bool IsRational() const           { return type_ == RATIONAL; }

			INT_64 ToSignedInteger() const    { return value_.i; }

			UINT_64 ToUnsignedInteger() const { return value_.u; }

			double ToDouble() const           { return value_.f; }

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

			IntegerBuilder();

			const Number Release() const;

			/**
			 * @throw OverflowError
			 */
			void SetNegative();

			/**
			 * @throw OverflowError
			 */
			void OnDecimalDigit(unsigned int digit);

			void OnBinaryDigit(unsigned int digit);

			void OnOctalDigit(unsigned int digit);

			void OnHexadecimalDigit(unsigned int digit);

		private:

			bool CanAppendIntegerDigit(unsigned int digit) const;

			UINT_64 value_;
			bool    isNegative_;
	};

	class NumberBuilder
	{
		public:

			static const UINT_64 MAX_UNSIGNED = TypeTraits<UINT_64>::MaxValue;
			static const UINT_64 MAX_SIGNED   = MAX_UNSIGNED / 2 + 1;
			static const INT_32  MAX_EXPONENT = 1000000;

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

			const Number Release() const;

			/**
			 * При переполнении автоматически преобразуется в double
			 */
			void SetNegative();

			void OnIntegerDigit(unsigned int digit);

			void OnFractionalDigit(int digit);

			void SetNegativeExponent();

			void OnExponentDigit(int digit);

			void SetFloat();

		private:

			bool CanAppendIntegerDigit(unsigned int digit) const;

			UINT_64 i_;
			double  f_;
			INT_32  exponent_;
			INT_32  exponentDelta_;
			bool    isNegative_;
			bool    isNegativeExponent_;
			bool    isFloat_;
	};


}

#endif
