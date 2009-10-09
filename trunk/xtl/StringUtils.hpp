#ifndef _XTL__STRING_UTILS_HPP__
#define _XTL__STRING_UTILS_HPP__ 1

#include <string>

namespace XTL
{
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

