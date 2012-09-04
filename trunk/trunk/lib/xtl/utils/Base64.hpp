#ifndef XTL__BASE_64_HPP__
#define XTL__BASE_64_HPP__ 1

#include <stdexcept>
#include <string>

#include "../CharBuffer.hpp"

namespace XTL
{
	class Base64
	{
		public:

			/**
			 * @throw std::runtime_error
			 */
			static void Decode(const char * source, unsigned int sourceSize, CharBuffer & dest);

			static void Decode(const std::string & source, CharBuffer & dest);

			static void DecodeUrl(const char * source, unsigned int sourceSize, CharBuffer & dest);

			static void DecodeUrl(const std::string & source, CharBuffer & dest);

			static void Encode(const char * source, unsigned int sourceSize, CharBuffer & dest);

			static void Encode(const std::string & source, CharBuffer & dest);

			static void EncodeUrl(const char * source, unsigned int sourceSize, CharBuffer & dest);

			static void EncodeUrl(const std::string & source, CharBuffer & dest);
	};
}

#endif
