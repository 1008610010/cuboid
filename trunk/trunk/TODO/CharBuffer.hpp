#ifndef __XTL__CHAR_BUFFER_HPP__
#define __XTL__CHAR_BUFFER_HPP__ 1

#include "Exception.hpp"
#include "StringUtils.hpp"
#include "DynamicArray.hpp"

#define ThrowNotEnoughMemory(SIZE) ThrowRuntimeError(FormatString("Not enough memory (%lu bytes requested)", static_cast<unsigned long int>(SIZE)))

namespace XTL
{
	template <typename _MemoryBuffer>
	class CharBuffer
	{
		public:

			typedef unsigned long int SizeType;

			CharBuffer()
				: data_(static_cast<char *>(::malloc(MIN_CAPACITY))),
				  capacity_(MIN_CAPACITY),
				  size_(0)
			{
				::Initial
				if (data_ == 0)
				{
					capacity_ = 0;
					ThrowRuntimeError(FormatString("Not enough memory (%lu bytes requested)", MIN_CAPACITY));
				}
			}

			~TextBuffer() throw()
			{
				::free(data_);
			}

			void Clear()
			{
				if (capacity_ > MIN_CAPACITY)
				{
					Realloc(MIN_CAPACITY);
				}

				size_ = 0;
				data_[size_] = '\0';
			}

			CharBuffer & Append(char value)
			{
				Reserve(1);

				data_[size_] = value;
				++size_;

				return *this;
			}

			CharBuffer & Append(char value, SizeType count)
			{
				if (count == 0)
				{
					return *this;
				}

				Reserve(count);

				::memset(data_ + size_, value, count);
				size_ += count;

				return *this;
			}

			CharBuffer & Append(const char * value, SizeType size)
			{
				if (size == 0)
				{
					return *this;
				}

				Reserve(size);

				::memcpy(data_ + size_, value, size);
				size_ += size;

				return *this;
			}



			CharBuffer & Append(const char * value)
			{
				return Append(value, ::strlen(value));
			}

			CharBuffer & Append(const std::string & value)
			{
				return Append(value.c_str(), value.size());
			}

			CharBuffer & operator<< (char value)
			{
				return Append(ch);
			}

			TextBuffer & operator<< (const char * value)
			{
				return Append(value);
			}

			TextBuffer & operator<< (const std::string & value)
			{
				return Append(value);
			}

		protected:

			CharBuffer(const CharBuffer &);
			CharBuffer & operator= (const CharBuffer &);

			void ReallocThrow(SizeType capacity)
			{
				if (!Realloc(capacity))
				{
					ThrowRuntimeError(FormatString("Not enough memory (%lu bytes requested)", capacity));
				}
			}

			void Reserve(SizeType size)
			{
				if (size_ + size > capacity_)
				{
					ReallocThrow(MIN_CAPACITY + DELTA_CAPACITY * ((size_ + size - MIN_CAPACITY) / DELTA_CAPACITY + 1));
				}
			}

			char     * data_;
			SizeType   capacity_;
			SizeType   size_;
	};


	template <unsigned int MIN_CAPACITY,
	          unsigned int DELTA_CAPACITY>
	class DeltaResizer
	{
		public:

			typedef unsigned long int SizeType;

			static const SizeType Initial()
			{
				return MIN_CAPACITY;
			}

			static const bool Resize(SizeType oldCapacity, SizeType needSize, SizeType & newCapacity)
			{
				if (needSize <= oldCapacity)
				{
					return false;
				}

				newCapacity = (needSize <= MIN_CAPACITY ?
				               MIN_CAPACITY :
				               MIN_CAPACITY + DELTA_CAPACITY * ((needSize - MIN_CAPACITY - 1) / DELTA_CAPACITY + 1));
				return true;
			}
	};

	template <typename _ItemType,
	          typename _Resizer>
	class DynamicArray
	{
		public:

			void Clear()
			{
				if (capacity_ > MIN_CAPACITY)
				{
					Realloc(MIN_CAPACITY);
				}

				size_ = 0;
				data_[size_] = '\0';
			}

			

			bool Reserve(SizeType capacity)
			{
			}

			void ReserveThrow(SizeType capacity)
			{
			}

		protected:





			void ReallocThrow(SizeType capacity)
			{
				if (
			}





			ItemType * items_;
			SizeType   capacity_;
	};
}

#endif
