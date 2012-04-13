#ifndef _XTL__BIT_SET_HPP__
#define _XTL__BIT_SET_HPP__ 1

#include <string.h>
#include "BitUtils.hpp"

namespace XTL
{
	template <int BYTE_SIZE>
	class StaticBitSet
	{
		public:

			class Adapter
			{
				public:

					operator const bool () const
					{
						return set_.Get(bit_);
					}

					Adapter & Set()
					{
						set_.Set(bit_);
						return *this;
					}

					Adapter & Clear()
					{
						set_.Clear(bit_);
						return *this;
					}

					Adapter & Invert()
					{
						set_.Invert(bit_);
						return *this;
					}

					Adapter & operator= (bool value)
					{
						return value ? Set() : Clear();
					}

				protected:

					friend class StaticBitSet;

					Adapter(StaticBitSet & set, unsigned int bit)
						: set_(set), bit_(bit) { ;; }

					StaticBitSet & set_;
					unsigned int   bit_;
			};

			static const unsigned int CAPACITY = (BYTE_SIZE << 3);

			StaticBitSet()
			{
				Clear();
			}

			const unsigned int Capacity() const
			{
				return CAPACITY;
			}

			const bool Get(unsigned int bit) const
			{
				return GetBit(bytes_, bit);
			}

			const bool operator[] (unsigned int bit) const
			{
				return Get(bit);
			}

			Adapter operator[] (unsigned int bit)
			{
				return Adapter(*this, bit);
			}

			void Set(unsigned int bit)
			{
				SetBit(bytes_, bit);
			}

			void Clear(unsigned int bit)
			{
				ClearBit(bytes_, bit);
			}

			void Invert(unsigned int bit)
			{
				InvertBit(bytes_, bit);
			}

			void Set(unsigned int bitFrom, unsigned int bitTo)
			{
				SetBits(bytes_, bitFrom, bitTo);
			}

			void Clear(unsigned int bitFrom, unsigned int bitTo)
			{
				ClearBits(bytes_, bitFrom, bitTo);
			}

			void Invert(unsigned int bitFrom, unsigned int bitTo)
			{
				InvertBits(bytes_, bitFrom, bitTo);
			}

			void Set()
			{
				memset(bytes_, '\xff', BYTE_SIZE);
			}

			void Clear()
			{
				memset(bytes_, '\x00', BYTE_SIZE);
			}

			void Invert()
			{
				for (unsigned int i = 0; i < BYTE_SIZE; ++i)
				{
					bytes_[i] = ~bytes_[i];
				}
			}

		protected:

			char bytes_[BYTE_SIZE];
	};
}

#endif
