#ifndef XTL__BIT_UTILS_HPP__
#define XTL__BIT_UTILS_HPP__ 1

#include "../Types.hpp"

namespace XTL
{
	unsigned int NumberOfOneBits(UINT_8 value);

	template <typename T>
	unsigned int NumberOfOneBits(T value)
	{
		unsigned int count = 0;
		for (unsigned int i = 0; i < sizeof(value) / sizeof(UINT_8); ++i)
		{
			count += NumberOfOneBits(*(reinterpret_cast<const UINT_8 *>(&value) + i));
		}
		return count;
	}

	template <typename T>
	unsigned int NumberOfZeroBits(T value)
	{
		return (sizeof(T) << 3) - NumberOfOneBits(value);
	}

	/**
	 * @return -1  если value равно нулю (все биты нулевые).
	 */
	int LeastOneBit(UINT_8 value);

	template <typename T>
	int LeastOneBit(T value)
	{
		const UINT_8 * const begin = reinterpret_cast<const UINT_8 *>(&value);
		const UINT_8 * const end = begin + sizeof(value) / sizeof(UINT_8);

		for (const UINT_8 * p = begin; p < end; ++p)
		{
			if (*p != static_cast<UINT_8>(0))
			{
				return ((p - begin) << 3) + LeastOneBit(*p);
			}
		}

		return -1;
	}

	int GreatestOneBit(UINT_8 value);

	template <typename T>
	int GreatestOneBit(T value)
	{
		const UINT_8 * const begin = reinterpret_cast<const UINT_8 *>(&value);
		const UINT_8 * const end = begin + sizeof(value) / sizeof(UINT_8);

		for (const UINT_8 * p = end - 1; p >= begin; --p)
		{
			if (*p != static_cast<UINT_8>(0))
			{
				return ((p - begin) << 3) + LeastOneBit(*p);
			}
		}

		return -1;
	}

	template <typename T>
	class OnlyOneBits
	{
		public:

			static const unsigned int CAPACITY = (sizeof(T) << 3);

			static const OnlyOneBits & Instance()
			{
				static OnlyOneBits instance;
				return instance;
			}

			const T operator[] (unsigned int bitIndex) const
			{
				return items_[bitIndex];
			}

		private:

			OnlyOneBits()
			{
				for (unsigned int i = 0; i < CAPACITY; ++i)
				{
					items_[i] = T(1) << i;
				}
			}

			T items_[CAPACITY];
	};

	template <typename T>
	class OnlyZeroBits
	{
		public:

			static const unsigned int CAPACITY = (sizeof(T) << 3);

			static const OnlyZeroBits & Instance()
			{
				static OnlyZeroBits instance;
				return instance;
			}

			const T operator[] (unsigned int bitIndex) const
			{
				return items_[bitIndex];
			}

		private:

			OnlyZeroBits()
			{
				for (unsigned int i = 0; i < CAPACITY; ++i)
				{
					items_[i] = ~ OnlyOneBits<T>::Instance()[i];
				}
			}

			T items_[CAPACITY];
	};

	template <typename T> const T OnlyOneBit(unsigned int bitIndex)
	{
		return OnlyOneBits<T>::Instance()[bitIndex];
	}

	template <typename T> const T OnlyZeroBit(unsigned int bitIndex)
	{
		return OnlyZeroBits<T>::Instance()[bitIndex];
	}
}

#endif

