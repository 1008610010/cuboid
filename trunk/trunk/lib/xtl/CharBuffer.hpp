#ifndef XTL__CHAR_BUFFER_HPP__
#define XTL__CHAR_BUFFER_HPP__ 1

#include <string.h>

#include <string>
#include <vector>

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

			const std::string ToString() const
			{
				return std::string(chars_.data(), chars_.size());
			}

		private:

			std::vector<char> chars_;
	};
}

#endif
