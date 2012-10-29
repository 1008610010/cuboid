#include "FormatString.hpp"

#include <stdarg.h>
#include <stdio.h>

#include <vector>

namespace XTL
{
	static const int VSNPRINTF_BUFFER_SIZE = 256;

	const std::string StringPrintFormatted(const char * format, ...)
	{
		va_list ap;
		std::vector<char> buffer(VSNPRINTF_BUFFER_SIZE);
		int size;

		while (true)
		{
			va_start(ap, format);
			size = ::vsnprintf(&buffer[0], buffer.size(), format, ap);
			va_end(ap);

			if (size >= 0)
			{
				if (static_cast<std::size_t>(size) < buffer.size())
				{
					break;
				}
				else
				{
					buffer.resize(size + 1);
				}
			}
			else
			{
				buffer.resize(buffer.size() << 1);
			}
		}

		return std::string(&buffer[0], size);
	}
}
