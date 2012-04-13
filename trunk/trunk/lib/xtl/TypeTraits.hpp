#ifndef XTL__TYPE_TRAITS_HPP__
#define XTL__TYPE_TRAITS_HPP__ 1

#include "Types.hpp"

namespace XTL
{
	template <unsigned int VALUE, unsigned int ALIGNMENT>
	struct AlignedCeil
	{
		enum
		{
			Value = ((VALUE - 1) / ALIGNMENT + 1) * ALIGNMENT
		};
	};

	template <unsigned int BYTE_SIZE>
	struct IntegerTypeTraits
	{
	};

	template <> struct IntegerTypeTraits<1>
	{
		typedef INT_8  Signed;
		typedef UINT_8 Unsigned;

		static const int ByteSize     = 1;
		static const int BitSize      = 8;
		static const int MaxDecDigits = 3;
		static const int MaxHexDigits = 2;

		static const Signed   MaxSignedValue   = 127;
		static const Signed   MinSignedValue   = - MaxSignedValue - 1;
		static const Unsigned MaxUnsignedValue = 255;
		static const Unsigned MinUnsignedValue = 0;
	};

	template <> struct IntegerTypeTraits<2>
	{
		typedef INT_16  Signed;
		typedef UINT_16 Unsigned;

		static const int ByteSize     =  2;
		static const int BitSize      = 16;
		static const int MaxDecDigits =  5;
		static const int MaxHexDigits =  4;

		static const Signed   MaxSignedValue   = 32767;
		static const Signed   MinSignedValue   = - MaxSignedValue - 1;
		static const Unsigned MaxUnsignedValue = 65535;
		static const Unsigned MinUnsignedValue = 0;
	};

	template <> struct IntegerTypeTraits<4>
	{
		typedef INT_32  Signed;
		typedef UINT_32 Unsigned;

		static const int ByteSize     =  4;
		static const int BitSize      = 32;
		static const int MaxDecDigits = 10;
		static const int MaxHexDigits =  8;

		static const Signed   MaxSignedValue   = 2147483647l;
		static const Signed   MinSignedValue   = - MaxSignedValue - 1;
		static const Unsigned MaxUnsignedValue = 4294967295lu;
		static const Unsigned MinUnsignedValue = 0;
	};

	template <> struct IntegerTypeTraits<8>
	{
		typedef INT_64  Signed;
		typedef UINT_64 Unsigned;

		static const int ByteSize     =  8;
		static const int BitSize      = 64;
		static const int MaxDecDigits = 20;
		static const int MaxHexDigits = 16;

		static const Signed   MaxSignedValue   = 9223372036854775807ll;
		static const Signed   MinSignedValue   = - MaxSignedValue - 1;
		static const Unsigned MaxUnsignedValue = 18446744073709551615llu;
		static const Unsigned MinUnsignedValue = 0;
	};

	template <typename IntegerType_>
	struct TypeTraits_UnsignedInteger
	{
		typedef IntegerTypeTraits<sizeof(IntegerType_)> Base;

		typedef typename Base::Signed   Signed;
		typedef typename Base::Unsigned Unsigned;

		static const bool IsSigned   = false;
		static const bool IsUnsigned = true;

		static const IntegerType_ MinValue = Base::MinUnsignedValue;
		static const IntegerType_ MaxValue = Base::MaxUnsignedValue;

		static const unsigned int MaxDecChars = Base::MaxDecDigits;
	};

	template <typename IntegerType_> const IntegerType_ TypeTraits_UnsignedInteger<IntegerType_>::MinValue;
	template <typename IntegerType_> const IntegerType_ TypeTraits_UnsignedInteger<IntegerType_>::MaxValue;

	template <typename IntegerType_>
	struct TypeTraits_SignedInteger
	{
		typedef IntegerTypeTraits<sizeof(IntegerType_)> Base;

		typedef typename Base::Signed   Signed;
		typedef typename Base::Unsigned Unsigned;

		static const bool IsSigned   = true;
		static const bool IsUnsigned = false;

		static const IntegerType_ MinValue = Base::MinSignedValue;
		static const IntegerType_ MaxValue = Base::MaxSignedValue;

		static const unsigned int MaxDecChars = Base::MaxDecDigits + 1; // "+ 1" for 'minus' char
	};

	template <typename IntegerType_> const IntegerType_ TypeTraits_SignedInteger<IntegerType_>::MinValue;
	template <typename IntegerType_> const IntegerType_ TypeTraits_SignedInteger<IntegerType_>::MaxValue;

	template <typename T> struct TypeTraits
	{
	};

	/*** Signed Integer Type Traits ***/

	template <> struct TypeTraits<signed char> : public TypeTraits_SignedInteger<signed char>
	{
	};

	template <> struct TypeTraits<short int> : public TypeTraits_SignedInteger<short int>
	{
	};

	template <> struct TypeTraits<int> : public TypeTraits_SignedInteger<int>
	{
	};

	template <> struct TypeTraits<long int> : public TypeTraits_SignedInteger<long int>
	{
	};

	template <> struct TypeTraits<long long int> : public TypeTraits_SignedInteger<long long int>
	{
	};

	/*** Unsigned Integer Type Traits ***/

	template <> struct TypeTraits<unsigned char> : public TypeTraits_UnsignedInteger<unsigned char>
	{
	};

	template <> struct TypeTraits<unsigned short int> : public TypeTraits_UnsignedInteger<unsigned short int>
	{
	};

	template <> struct TypeTraits<unsigned int> : public TypeTraits_UnsignedInteger<unsigned int>
	{
	};

	template <> struct TypeTraits<unsigned long int> : public TypeTraits_UnsignedInteger<unsigned long int>
	{
	};

	template <> struct TypeTraits<unsigned long long int> : public TypeTraits_UnsignedInteger<unsigned long long int>
	{
	};
}

#endif
