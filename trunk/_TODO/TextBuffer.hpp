#ifndef __XTL__TEXT_BUFFER_HPP__
#define __XTL__TEXT_BUFFER_HPP__ 1

#include "Exception.hpp"
#include "StringUtils.hpp"

namespace XTL
{
	class TextBuffer
	{
		public:

			typedef unsigned long int SizeType;

			static const SizeType MIN_CAPACITY   = 128;
			static const SizeType DELTA_CAPACITY =  64;

			TextBuffer()
				: data_(static_cast<char *>(::malloc(MIN_CAPACITY))),
				  capacity_(MIN_CAPACITY),
				  size_(0)
			{
				if (data_ == 0)
				{
					capacity_ = 0;
					ThrowRuntimeError(FormatString("Not enough memory (%lu bytes requested)", MIN_CAPACITY));
				}

				data_[size_] = '\0';
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

			TextBuffer & Append(const char * value, SizeType size)
			{
				if (size == 0)
				{
					return *this;
				}

				Reserve(size);

				::memcpy(data_ + size_, value, size);
				size_ += size;
				data_[size_] = '\0';
				return *this;
			}

			TextBuffer & Append(char ch)
			{
				Reserve(1);
				data_[size_] = ch;
				++size_;
				data_[size_] = '\0';
				return *this;
			}

			TextBuffer & Append(char ch, SizeType count)
			{
				if (count == 0)
				{
					return *this;
				}

				Reserve(count);

				::memset(data_ + size_, ch, count);
				size_ += count;
				data_[size_] = '\0';
				return *this;
			}

			TextBuffer & Append(const char * value)
			{
				return Append(value, ::strlen(value));
			}

			TextBuffer & Append(const std::string & value)
			{
				return Append(value.c_str(), value.size());
			}

			TextBuffer & operator<< (char ch)
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

			TextBuffer(const TextBuffer &);
			TextBuffer & operator= (const TextBuffer &);

			bool Realloc(SizeType capacity)
			{
				char * newData = static_cast<char *>(::realloc(data_, capacity));
				if (newData == 0)
				{
					return false;
				}
				capacity_ = capacity;
				data_ = newData;
				return true;
			}

			void ReallocThrow(SizeType capacity)
			{
				if (!Realloc(capacity))
				{
					ThrowRuntimeError(FormatString("Not enough memory (%lu bytes requested)", capacity));
				}
			}

			void Reserve(SizeType size)
			{
				// ">=" - for '\0' char-terminator
				if (size_ + size >= capacity_)
				{
					ReallocThrow(MIN_CAPACITY + DELTA_CAPACITY * ((size_ + size - MIN_CAPACITY) / DELTA_CAPACITY + 1));
				}
			}

			char     * data_;
			SizeType   capacity_;
			SizeType   size_;
	};
}

#endif
