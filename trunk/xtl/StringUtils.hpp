#ifndef _XTL__STRING_UTILS_HPP__
#define _XTL__STRING_UTILS_HPP__ 1

#include <string>

namespace XTL
{
	template <typename _SourceType, typename _DestType>
	class ConverterStaticCast
	{
		public:

			typedef _SourceType SourceType;
			typedef _DestType   DestType;

			DestType operator() (const SourceType & v) const
			{
				return static_cast<DestType>(v);
			}
	};

	template <typename _SourceType, typename _DestType>
	class ConverterReinterpretCast
	{
		public:

			typedef _SourceType SourceType;
			typedef _DestType   DestType;

			DestType operator() (const SourceType & v) const
			{
				return reinterpret_cast<DestType>(v);
			}
	};

	class ConverterCharsToInteger
	{
		public:
	};

	template <typename SourceType>
	bool IntegerToString(const SourceType & src, char * dest)
	{
		return true;
	}


	template <typename T>
	const std::string IntegerToString(const T & v)
	{
		char buffer[64];
		::sprintf(buffer, "%lld", static_cast<long long >(v));
		return std::string(buffer);
	}

	template <typename T>
	const std::string RealToString(const T & v)
	{
		char buffer[64];
		::sprintf(buffer, "%.16g", static_cast<double>(v));
		return std::string(buffer);
	}

	template <typename T>
	T StringToInteger(const char * s)
	{
	}

	template <typename Parser>
	const char * Parse(const char * src, Parser & parser)
	{
		const char * p = src;
		for (; parser(*p); ++p) { ;; }
		return p;
	}

	enum CharClass
	{
		CHAR_SPACE   = 1,
		CHAR_NEWLINE = 2,
		CHAR_DIGIT   = 4,
		CHAR_ALPHA   = 8
	};

	class CharClassifier
	{
		public:

			static const int CHARS_COUNT = 256;

			enum
			{
				SPACE   = 1,
				NEWLINE = 2,
				DIGIT   = 4,
				ALPHA   = 8
			};

			CharClassifier()
			{
				Clear();
				Init('0', '9', DIGIT);
				Init('A', 'Z', ALPHA);
				Init('a', 'z', ALPHA);
				Init('_', ALPHA);
				Init(' ', SPACE);
				Init('\t', SPACE);
				Init('\v', SPACE);
				Init('\r', SPACE);
				Init('\n', NEWLINE);
			}

			bool Belongs(char ch, int charClass)
			{
				return (chars[ch] & charClass) != 0;
			}

		protected:

			void Clear()
			{
				for (int ch = 0; ch < CHARS_COUNT; ++ch)
				{
					chars[ch] = 0;
				}
			}

			void Init(unsigned char ch, char charClass)
			{
				chars[ch] |= charClass;
			}

			void Init(unsigned char begin, unsigned char end, char charClass)
			{
				for (unsigned char ch = begin; ch <= end; ++ch)
				{
					Init(ch, charClass);
				}
			}

			char chars[CHARS_COUNT];
	};

	class NumberParser
	{
		public:

		protected:
	};

/*
	bool ParseInteger(const char * src, long long & dest, const char ** end)
	{
		const char * p = src;
		long long i = 0;

		if (src == 0)
		{
			return false;
		}

		if (*p == '-')
		{
			if (*p < '0' || *p > '9')
			{
				return false;
			}

			for (++p; *p >= '0' && *p <= '9'; ++p)
			{
				i = (i << 3) + (i << 1) + ('0' - *p);
			}
		}
		else
		{
			if (*p < '0' || *p > '9')
			{
				return false;
			}

			for (; *p >= '0' && *p <= '9'; ++p)
			{
				i = (i << 3) + (i << 1) + (*p - '0');
			}
		}

		dest = i;

		return true;
	}
*/

	const long long FloatToInteger(const double & f);

	const long long StringToInteger(const std::string & s);

	const double IntegerToFloat(const long long & i);

	const double StringToFloat(const std::string & s);

	const std::string IntegerToString(const long long & i);

	const std::string FloatToString(const double & f);

	bool FormatString(std::string & s, const char * format, ...);

	const std::string FormatString(const char * format, ...);
}

#endif

