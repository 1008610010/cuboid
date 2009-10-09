#include "StringUtils.hpp"
#include <stdarg.h>
#include <stdlib.h>

namespace XTL
{
	bool FormatString(std::string & s, const char * format, ...)
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

