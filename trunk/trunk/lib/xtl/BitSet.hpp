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

			void Assign(T bits)
			{
				bits_ = bits;
			}

			static unsigned int Capacity()
			{
				return CAPACITY;
			}

			bool IsEmpty() const
			{
				return bits_ == 0;
			}

			void Clear()
			{
				bits_ = 0;
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

			int MinBit() const
			{
				return LeastOneBit(bits_);
			}

			int MaxBit() const
			{
				return GreatestOneBit(bits_);
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
						: set_(set),
						  current_(0)
					{
						FindNextBit();
					}

					void Reset()
					{
						current_ = 0;
						FindNextBit();
					}

					bool AtEnd() const
					{
						return current_ == -1;
					}

					void Advance()
					{
						if (AtEnd())
						{
							return;
						}

						++current_;

						FindNextBit();
					}

					int operator* () const
					{
						return current_;
					}

				private:

					void FindNextBit()
					{
						while (current_ < static_cast<int>(CAPACITY))
						{
							if (set_.Includes(current_))
							{
								return;
							}
							++current_;
						}

						current_ = -1;
					}

					const BitSet & set_;
					int current_;
			};

			template <typename Func_>
			void ForEach(Func_ func)
			{
				const const_iterator theEnd = end();
				for (const_iterator itr(begin()); itr != theEnd; ++itr)
				{
					func(*itr);
				}
			}

			typedef unsigned int value_type;

			class const_iterator
			{
				public:

					const_iterator(const BitSet & bitSet, unsigned int bitFrom)
						: bitSet_(bitSet),
						  currentBit_(bitFrom)
					{
						FindNextBit();
					}

					unsigned int operator* ()
					{
						return currentBit_;
					}

					const_iterator & operator++ ()
					{
						if (currentBit_ < CAPACITY)
						{
							++currentBit_;
							FindNextBit();
						}
						return *this;
					}

					const_iterator & operator++ (int)
					{
						const_iterator temp(*this);
						++(*this);
						return temp;
					}

					bool operator== (const const_iterator & other) const
					{
						// WARNING: We does not test, if bitSet_ members are equal.
						return currentBit_ == other.currentBit_;
					}

					bool operator!= (const const_iterator & other) const
					{
						return currentBit_ != other.currentBit_;
					}

				private:

					void FindNextBit()
					{
						while (currentBit_ < CAPACITY)
						{
							if (bitSet_.Includes(currentBit_))
							{
								return;
							}
							++currentBit_;
						}
						currentBit_ = CAPACITY;
					}

					const BitSet & bitSet_;
					unsigned int   currentBit_;
			};

			const_iterator begin() const
			{
				return const_iterator(*this, 0);
			}

			const_iterator end() const
			{
				return const_iterator(*this, CAPACITY);
			}

		protected:

			T bits_;
	};

	typedef BitSet<UINT_8>  BitSet8;
	typedef BitSet<UINT_16> BitSet16;
	typedef BitSet<UINT_32> BitSet32;
	typedef BitSet<UINT_64> BitSet64;
}

#endif

