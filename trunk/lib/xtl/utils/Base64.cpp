#include "Base64.hpp"

namespace XTL
{
	namespace
	{
		class Base64Codec
		{
			public:

				static const unsigned int CHAR_TO_INT_SIZE = 128;

				Base64Codec(const int * charToInt, const char * intToChar)
					: charToInt_(charToInt),
					  intToChar_(intToChar)
				{
					;;
				}

				int CharToInt(unsigned char c) const
				{
					int i = (c < CHAR_TO_INT_SIZE ? charToInt_[c] : -1);

					if (i < 0)
					{
						throw std::runtime_error("CharToInt");
					}

					return i;
				}

				char IntToChar(unsigned char i) const
				{
					return intToChar_[i & 0x3f];
				}

			private:

				const int * const charToInt_;
				const char * const intToChar_;
		};

		static const int BASE_64_CHAR_TO_INT[] = {
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
			52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
			-1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
			15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
			-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
			41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1
		};

		static const char BASE_64_INT_TO_CHAR[] = {
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
			'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
			'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
			'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
			'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
			'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
			'w', 'x', 'y', 'z', '0', '1', '2', '3',
			'4', '5', '6', '7', '8', '9', '+', '/'
		};

		static const int BASE_64_URL_CHAR_TO_INT[] = {
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1,
			52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
			-1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
			15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, 63,
			-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
			41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1
		};

		static const char BASE_64_URL_INT_TO_CHAR[] = {
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
			'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
			'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
			'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
			'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
			'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
			'w', 'x', 'y', 'z', '0', '1', '2', '3',
			'4', '5', '6', '7', '8', '9', '-', '_'
		};

		static const Base64Codec BASE64_COMMON (BASE_64_CHAR_TO_INT,     BASE_64_INT_TO_CHAR);
		static const Base64Codec BASE64_URL    (BASE_64_URL_CHAR_TO_INT, BASE_64_URL_INT_TO_CHAR);

		void Base64_Decode(const Base64Codec & codec, const char * source, unsigned int sourceSize, CharBuffer & dest)
		{
			if (sourceSize == 0)
			{
				return;
			}

			if (sourceSize % 4 != 0)
			{
				throw std::runtime_error("Invalid message length");
			}

			// int destSize = 3 * (source.length() / 4);
			const unsigned char * p = reinterpret_cast<const unsigned char *>(source);
			const unsigned char * end = p + sourceSize;

			int tail = 0;
			if (source[sourceSize - 1] == '=')
			{
				// --destSize;
				++tail;

				end -= 4;

				if (source[sourceSize - 2] == '=')
				{
					// --destSize;
					++tail;
				}
			}

			while (p < end)
			{
				int a = codec.CharToInt(*(p++));
				int b = codec.CharToInt(*(p++));
				int c = codec.CharToInt(*(p++));
				int d = codec.CharToInt(*(p++));

				dest.Append((a << 2) | (b >> 4));
				dest.Append((b << 4) | (c >> 2));
				dest.Append((c << 6) | d);
			}

			if (tail > 0)
			{
				int a = codec.CharToInt(*(p++));
				int b = codec.CharToInt(*(p++));

				dest.Append((a << 2) | (b >> 4));

				if (tail == 1)
				{
					int c = codec.CharToInt(*(p++));
					dest.Append((b << 4) | (c >> 2));
				}
			}
		}

		void Base64_Encode(const Base64Codec & codec, const char * source, unsigned int sourceSize, CharBuffer & dest)
		{
			if (sourceSize == 0)
			{
				return;
			}

			int tail = sourceSize % 3;

			const unsigned char * p = reinterpret_cast<const unsigned char *>(source);

			unsigned char a;
			unsigned char b;
			unsigned char c;

			const unsigned char * end = p + (sourceSize - tail);
			while (p < end)
			{
				a = *(p++);
				b = *(p++);
				c = *(p++);
				dest.Append(codec.IntToChar(a >> 2));
				dest.Append(codec.IntToChar(((a & 0x03) << 4) | ((b >> 4) & 0x0f)));
				dest.Append(codec.IntToChar(((b & 0x0f) << 2) | ((c >> 6) & 0x03)));
				dest.Append(codec.IntToChar(c & 0x3f));
			}

			switch (tail)
			{
				case 1:
					a = *p;
					dest.Append(codec.IntToChar(a >> 2));
					dest.Append(codec.IntToChar((a & 0x03) << 4));
					dest.Append("==");
					break;

				case 2:
					a = *(p++);
					b = *p;
					dest.Append(codec.IntToChar(a >> 2));
					dest.Append(codec.IntToChar(((a & 0x03) << 4) | ((b >> 4) & 0x0f)));
					dest.Append(codec.IntToChar((b & 0x0f) << 2));
					dest.Append("=");
					break;
			}
		}
	}

	void Base64::Decode(const char * source, unsigned int sourceSize, CharBuffer & dest)
	{
		Base64_Decode(BASE64_COMMON, source, sourceSize, dest);
	}

	void Base64::Decode(const std::string & source, CharBuffer & dest)
	{
		Decode(source.data(), source.size(), dest);
	}

	void Base64::DecodeUrl(const char * source, unsigned int sourceSize, CharBuffer & dest)
	{
		Base64_Decode(BASE64_URL, source, sourceSize, dest);
	}

	void Base64::DecodeUrl(const std::string & source, CharBuffer & dest)
	{
		DecodeUrl(source.data(), source.size(), dest);
	}

	void Base64::Encode(const char * source, unsigned int sourceSize, CharBuffer & dest)
	{
		Base64_Encode(BASE64_COMMON, source, sourceSize, dest);
	}

	void Base64::Encode(const std::string & source, CharBuffer & dest)
	{
		Encode(source.data(), source.size(), dest);
	}

	void Base64::EncodeUrl(const char * source, unsigned int sourceSize, CharBuffer & dest)
	{
		Base64_Encode(BASE64_URL, source, sourceSize, dest);
	}

	void Base64::EncodeUrl(const std::string & source, CharBuffer & dest)
	{
		EncodeUrl(source.data(), source.size(), dest);
	}
}
