#ifndef XTL__CHAR_BUFFER_HPP__
#define XTL__CHAR_BUFFER_HPP__ 1

#include <string.h>

#include <string>
#include <vector>

#include "io/InputStream.hpp"

namespace XTL
{
	class CharBuffer
	{
		public:

			CharBuffer()
				: chars_()
			{
				;;
			}

			explicit CharBuffer(unsigned int capacity)
				: chars_()
			{
				chars_.reserve(capacity);
			}

			const char * Data() const
			{
				return chars_.data();
			}

			unsigned int Size() const
			{
				return chars_.size();
			}

			void Reserve(unsigned int capacity)
			{
				chars_.reserve(capacity);
			}

			CharBuffer & Append(char c)
			{
				chars_.push_back(c);
				return *this;
			}

			CharBuffer & Append(const char * begin, const char * end)
			{
				chars_.insert(chars_.end(), begin, end);
				return *this;
			}

			CharBuffer & Append(const char * ptr, unsigned int size)
			{
				return Append(ptr, ptr + size);
			}

			CharBuffer & Append(const std::string & s)
			{
				return Append(s.data(), s.size());
			}

			CharBuffer & Append(const char * ptr)
			{
				return Append(ptr, ::strlen(ptr));
			}

			CharBuffer & Append(InputStream & inputStream, unsigned int size)
			{
				if (size == 0)
				{
					return *this;
				}

				const unsigned int currentSize = chars_.size();

				try
				{
					chars_.resize(currentSize + size);
					inputStream.Read(&(chars_[currentSize]), size);
				}
				catch (...)
				{
					chars_.resize(currentSize);
					throw;
				}

				return *this;
			}

			const std::string ToString() const
			{
				return std::string(chars_.data(), chars_.size());
			}

			void Clear()
			{
				chars_.clear();
			}

			void Shift(unsigned int size)
			{
				if (size == 0)
				{
					return;
				}
				else if (size >= chars_.size())
				{
					chars_.clear();
				}
				else
				{
					chars_.erase(chars_.begin(), chars_.begin() + size);
				}
			}

		private:

			std::vector<char> chars_;
	};

	inline bool operator== (const CharBuffer & charBuffer, const std::string & s)
	{
		return charBuffer.Size() == s.size() &&
		       ::memcmp(s.data(), charBuffer.Data(), charBuffer.Size()) == 0;
	}

	inline bool operator== (const std::string & s, const CharBuffer & charBuffer)
	{
		return charBuffer == s;
	}

	inline bool operator!= (const CharBuffer & charBuffer, const std::string & s)
	{
		return !(charBuffer == s);
	}

	inline bool operator!= (const std::string & s, const CharBuffer & charBuffer)
	{
		return charBuffer != s;
	}
}

#endif
