#ifndef _XTL__TYPES_HPP__
#define _XTL__TYPES_HPP__ 1

#include <stdint.h>

namespace XTL
{
	template <int BYTE_SIZE>
	struct SignedInt {};

	template <> struct SignedInt<1>
	{
		typedef int8_t Type;
	};

	template <> struct SignedInt<2>
	{
		typedef int16_t Type;
	};

	template <> struct SignedInt<4>
	{
		typedef int32_t Type;
	};

	template <> struct SignedInt<8>
	{
		typedef int64_t Type;
	};

	template <int BYTE_SIZE>
	struct UnsignedInt {};

	template <> struct UnsignedInt<1>
	{
		typedef uint8_t Type;
	};

	template <> struct UnsignedInt<2>
	{
		typedef uint16_t Type;
	};

	template <> struct UnsignedInt<4>
	{
		typedef uint32_t Type;
	};

	template <> struct UnsignedInt<8>
	{
		typedef uint64_t Type;
	};

	template <typename T, T INITIAL_VALUE>
	class Initialized
	{
		public:

			Initialized()
				: value_(INITIAL_VALUE) { ;; }

			operator const T & () const
			{
				return value_;
			}

			operator T & ()
			{
				return value_;
			}

			Initialized & operator= (const T & value)
			{
				value_ = value;
				return *this;
			}

		protected:

			T value_;
	};
}

#endif
