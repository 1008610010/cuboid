#ifndef XTL__BIT_SET_HPP__
#define XTL__BIT_SET_HPP__ 1

#include <string>
#include <vector>

#include "utils/BitUtils.hpp"

namespace XTL
{
	template <typename T>
	class BitSet
	{
		public:

			static const unsigned int CAPACITY = (sizeof(T) << 3);

			BitSet()
				: bits_(0)
			{
				;;
			}

			explicit BitSet(bool setAll)
				: bits_(setAll ? ~T(0) : 0)
			{
				;;
			}

			explicit BitSet(T bits)
				: bits_(bits)
			{
				;;
			}

			static unsigned int Capacity()
			{
				return CAPACITY;
			}

			void Add(unsigned int bit)
			{
				bits_ |= OnlyOneBit<T>(bit);
			}

			void Remove(unsigned int bit)
			{
				bits_ &= OnlyZeroBit<T>(bit);
			}

			bool Includes(unsigned int bit) const
			{
				return (bits_ & OnlyOneBit<T>(bit)) != 0;
			}

			unsigned int operator[] (unsigned int bit) const
			{
				return Includes(bit) ? 1 : 0;
			}

			unsigned int Size() const
			{
				return NumberOfOneBits(bits_);
			}

			unsigned int Index(unsigned int bit) const
			{
				return NumberOfOneBits(bits_ & (OnlyOneBit<T>(bit) - T(1)));
			}

			const std::string ToString() const
			{
				std::vector<char> buffer(CAPACITY + 1);

				for (unsigned int i = 0; i < CAPACITY; ++i)
				{
					buffer[i] = '0' + (*this)[i];
				}

				buffer[CAPACITY] = '\0';

				return &buffer[0];
			}

			template <typename Container_>
			unsigned int Items(Container_ c)
			{
				unsigned int count = 0;

				for (unsigned int i = 0; i < CAPACITY; ++i)
				{
					if (Includes(i))
					{
						c[count] = i;
						++count;
					}
				}

				return count;
			}

			class ConstIterator
			{
				public:

					ConstIterator(const BitSet & set)
						: set_(set), current_(-1) { ;; }

					bool Next()
					{
						while (current_ < static_cast<INT_32>(BitSet::BITS_COUNT) - 1)
						{
							++current_;
							INT_32 bit = current_ & 0x07;
							INT_32 next = GetFirstBit1(set_.GetByte(current_) & ~((1 << bit) - 1));
							if (next >= 0)
							{
								current_ = (current_ & 0xf8) + next;
								return true;
							}
							current_ |= 0x07;
						}
						return false;
					}

					INT_32 operator* () const { return current_; }

				private:

					const BitSet & set_;
					INT_32 current_;
			};


		protected:

			T bits_;
	};

	typedef BitSet<UINT_8>  BitSet8;
	typedef BitSet<UINT_16> BitSet16;
	typedef BitSet<UINT_32> BitSet32;
	typedef BitSet<UINT_64> BitSet64;
}

#endif

