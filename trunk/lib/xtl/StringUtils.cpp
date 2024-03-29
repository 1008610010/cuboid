#include "StringUtils.hpp"

#include <stdio.h>
#include <string.h>

namespace XTL
{
	namespace
	{
		void ReadDecimalDigits(const char *& ptr, long long int & result, int maxDigits, int & wasRead)
		{
			result = 0;
			wasRead = 0;
			while (*ptr >= '0' && *ptr <= '9' && maxDigits > 0) {
				result = 10 * result + (*ptr - '0');
				--maxDigits;
				++wasRead;
				++ptr;
			}
		}
	}

	void ToLowerCase(std::string & s)
	{
		const std::string::iterator end = s.end();
		for (std::string::iterator itr = s.begin(); itr != end; ++itr)
		{
			if (*itr >= 'A' && *itr <= 'Z')
			{
				*itr += 'a' - 'A';
			}
		}
	}

	void ToUpperCase(std::string & s)
	{
		const std::string::iterator end = s.end();
		for (std::string::iterator itr = s.begin(); itr != end; ++itr)
		{
			if (*itr >= 'a' && *itr <= 'z')
			{
				*itr += 'A' - 'a';
			}
		}
	}

	const std::string LowerCased(const std::string & s)
	{
		std::string result(s);

		ToLowerCase(result);

		return result;
	}

	const std::string UpperCased(const std::string & s)
	{
		std::string result(s);

		ToUpperCase(result);

		return result;
	}

	bool IsDigit(const char * s)
	{
		return *s >= '0' && *s <= '9';
	}

	bool IsInteger(const char * s)
	{
		if (s == 0 || *s == '\0')
		{
			return false;
		}

		if (*s == '-')
		{
			++s;
		}

		if (!IsDigit(s))
		{
			return false;
		}

		do
		{
			++s;
		}
		while (IsDigit(s));

		return *s == '\0';
	}

	bool IsInteger(const std::string & s)
	{
		return IsInteger(s.c_str());
	}

	bool IsNumber(const char * s)
	{
		if (s == 0 || *s == '\0')
		{
			return false;
		}

		if (*s == '-')
		{
			++s;
		}

		if (!IsDigit(s))
		{
			return false;
		}

		do
		{
			++s;
		}
		while (IsDigit(s));

		if (*s == '.')
		{
			++s;

			if (!IsDigit(s))
			{
				return false;
			}

			do
			{
				++s;
			}
			while (IsDigit(s));
		}

		if (*s == 'e' || *s == 'E')
		{
			++s;

			if (*s == '+' || *s == '-')
			{
				++s;
			}

			if (!IsDigit(s))
			{
				return false;
			}

			do
			{
				++s;
			}
			while (IsDigit(s));
		}

		return *s == '\0';
	}

	bool IsNumber(const std::string & s)
	{
		return IsNumber(s.c_str());
	}

	/********** FloatStringifier **********/

	FloatStringifier::FloatStringifier(double value)
		: size_(0)
	{
		size_ = ::snprintf(buffer_, BUFFER_SIZE, "%g", value);
		if (size_ >= BUFFER_SIZE)
		{
			size_ = BUFFER_SIZE - 1;
			buffer_[size_] = '\0';
		}
	}

	/********** StringSplitter **********/

	StringSplitter::StringSplitter(const char * source, char delimiter)
		: source_(source),
		  begin_(source),
		  delimiter_(delimiter)
	{
		;;
	}

	bool StringSplitter::GetNext(std::string & result)
	{
		if (begin_ == 0)
		{
			return false;
		}

		const char * end = ::strchr(begin_, delimiter_);
		if (end != 0)
		{
			result.assign(begin_, end - begin_);
			begin_ = end + 1;
		}
		else
		{
			result.assign(begin_);
			begin_ = 0;
		}

		return true;
	}

/*
	TODO: read 9 digits to UINT_32
	double FloatToInteger(const char * value)
	{
		const char * p = value;
		bool negative = false;

		if (*p == '-') {
			negative = true;
			++p;
		}

		while (*p == '0') {
			++p;
		}

		long long i = 0;
		for (; *p >= '0' && *p <= '9'; ++p) {
			i = 10 * i + (*p - '0');
		}

		double f = static_cast<double>(i);

		if (*p == '.') {
			++p;
			INT_32 n = 0;
			INT_32 d = 0;
			double d = 1.0;
			for (; *p >= '0' && *p <= '9'; ++p) {
				d /= 10.0;
				f += (*p - '0') * d;
			}
		}

		if (*p == 'e' || *p == 'E')
		{
			++p;

			bool expneg = true;
			if (*p == '+' || *p == '-')
			{
				expneg = (*p == '-');
				++p;
			}

			long long e = 0;
			for (; *p >= '0' && *p <= '9'; ++p)
			{
				e = 10 * e + (*p - '0');
			}

			if (expneg)
			{
				for (; e > 0; --e, f /= 10) { ;; }
			}
			else
			{
				for (; e > 0; --e, f *= 10) { ;; }
			}
		}

		return neg ? -f : f;

	}

	double FloatToInteger(const std::string & value)
	{
		return FloatToInteger(value.c_str());
	}
*/
}

