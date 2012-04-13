#include "StringUtils.hpp"
#include <stdlib.h>

namespace XTL
{
	const char * const IntegerTypeTraits<int>::PrintPattern = "%d";
	const char * const IntegerTypeTraits<unsigned int>::PrintPattern = "%u";
	const char * const IntegerTypeTraits<long int>::PrintPattern = "%ld";
	const char * const IntegerTypeTraits<unsigned long int>::PrintPattern = "%lu";
	const char * const IntegerTypeTraits<long long int>::PrintPattern = "%lld";
	const char * const IntegerTypeTraits<unsigned long long int>::PrintPattern = "%llu";

	const double IntegerToFloat(const long long & i)
	{
		return static_cast<const double>(i);
	}

	const std::string IntegerToString(const long long & i)
	{
		char buffer[64];
		::sprintf(buffer, "%lld", i);
		return std::string(buffer);
	}

	const long long FloatToInteger(const double & f)
	{
		return static_cast<const long long>(f);
	}

	const std::string FloatToString(const double & f)
	{
		char buffer[64];
		::sprintf(buffer, "%.16g", f);
		return std::string(buffer);
	}

	const long long StringToInteger(const std::string & s)
	{
		const char * p = s.c_str();
		long long i = 0;
		if (*p != '\0')
		{
			if (*p == '-')
			{
				++p;
				if (*p != '\0')
				{
					i = ('0' - *p);
					++p;
					for (; *p != '\0'; ++p)
					{
						i = (i << 3) + (i << 1) + ('0' - *p);
					}
				}
			}
			else
			{
				i = (*p - '0');
				++p;
				for (; *p != '\0'; ++p)
				{
					i = (i << 3) + (i << 1) + (*p - '0');
				}
			}
		}
		return i;
	}

	const double StringToFloat(const std::string & s)
	{
		const char * p = s.c_str();
		bool neg = false;

		if (*p == '-')
		{
			neg = true;
			++p;
		}

		long long i = 0;
		for (; *p >= '0' && *p <= '9'; ++p)
		{
			i = 10 * i + (*p - '0');
		}

		double f = static_cast<double>(i);

		if (*p == '.')
		{
			++p;

			double d = 1.0;
			for (; *p >= '0' && *p <= '9'; ++p)
			{
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

	const bool FormatString(std::string & s, const char * format, ...)
	{
		va_list ap;

		int size = 128;
		char * buffer = static_cast<char *>(::malloc(size));
		if (buffer == 0)
		{
			return false;
		}

		while (true)
		{
			va_start(ap, format);
			int result = ::vsnprintf(buffer, size, format, ap);
			va_end(ap);

			if (result >= 0)
			{
				if (result < size)
				{
					break;
				}
				else
				{
					size = result + 1;
				}
			}
			else
			{
				size <<= 1;
			}

			char * newBuffer = static_cast<char *>(::realloc(buffer, size));
			if (newBuffer == 0)
			{
				::free(buffer);
				return false;
			}

			buffer = newBuffer;
		}

		s.assign(buffer);
		::free(buffer);
		return true;
	}

	const std::string FormatString(const char * format, ...)
	{
		va_list ap;

		int size = 128;
		char * buffer = static_cast<char *>(::malloc(size));
		if (buffer == 0)
		{
			return std::string();
		}

		while (true)
		{
			va_start(ap, format);
			int result = ::vsnprintf(buffer, size, format, ap);
			va_end(ap);

			if (result >= 0)
			{
				if (result < size)
				{
					break;
				}
				else
				{
					size = result + 1;
				}
			}
			else
			{
				size <<= 1;
			}

			char * newBuffer = static_cast<char *>(::realloc(buffer, size));
			if (newBuffer == 0)
			{
				::free(buffer);
				return std::string();
			}

			buffer = newBuffer;
		}

		std::string s(buffer);
		::free(buffer);
		return s;
	}
}

