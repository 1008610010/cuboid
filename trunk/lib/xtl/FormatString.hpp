#ifndef XTL__FORMAT_STRING_HPP__
#define XTL__FORMAT_STRING_HPP__ 1

#include <string>

namespace XTL
{
	const std::string StringPrintFormatted(const char * format, ...);


	template <typename T1>
	inline const std::string FormatString(const char * format,
	                                      const T1 & t1);

	template <typename T1,
	          typename T2>
	inline const std::string FormatString(const char * format,
	                                      const T1 & t1,
	                                      const T2 & t2);

	template <typename T1,
	          typename T2,
	          typename T3>
	inline const std::string FormatString(const char * format,
	                                      const T1 & t1,
	                                      const T2 & t2,
	                                      const T3 & t3);

	template <typename T1,
	          typename T2,
	          typename T3,
	          typename T4>
	inline const std::string FormatString(const char * format,
	                                      const T1 & t1,
	                                      const T2 & t2,
	                                      const T3 & t3,
	                                      const T4 & t4);

	template <typename T1,
	          typename T2,
	          typename T3,
	          typename T4,
	          typename T5>
	inline const std::string FormatString(const char * format,
	                                      const T1 & t1,
	                                      const T2 & t2,
	                                      const T3 & t3,
	                                      const T4 & t4,
	                                      const T5 & t5);

	template <typename T1,
	          typename T2,
	          typename T3,
	          typename T4,
	          typename T5,
	          typename T6>
	inline const std::string FormatString(const char * format,
	                                      const T1 & t1,
	                                      const T2 & t2,
	                                      const T3 & t3,
	                                      const T4 & t4,
	                                      const T5 & t5,
	                                      const T6 & t6);

	template <typename T1,
	          typename T2,
	          typename T3,
	          typename T4,
	          typename T5,
	          typename T6,
	          typename T7>
	inline const std::string FormatString(const char * format,
	                                      const T1 & t1,
	                                      const T2 & t2,
	                                      const T3 & t3,
	                                      const T4 & t4,
	                                      const T5 & t5,
	                                      const T6 & t6,
	                                      const T7 & t7);

	template <typename T1,
	          typename T2,
	          typename T3,
	          typename T4,
	          typename T5,
	          typename T6,
	          typename T7,
	          typename T8>
	inline const std::string FormatString(const char * format,
	                                      const T1 & t1,
	                                      const T2 & t2,
	                                      const T3 & t3,
	                                      const T4 & t4,
	                                      const T5 & t5,
	                                      const T6 & t6,
	                                      const T7 & t7,
	                                      const T8 & t8);
}

#include "FormatString.inc"

#endif

