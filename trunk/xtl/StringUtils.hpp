#ifndef _XTL__STRING_UTILS_HPP__
#define _XTL__STRING_UTILS_HPP__ 1

#include <string>

namespace XTL
{
	bool FormatString(std::string & s, const char * format, ...);

	const std::string FormatString(const char * format, ...);
}

#endif

