#ifndef XTL__TYPES_HPP__
#define XTL__TYPES_HPP__ 1

#include <stdint.h>
#include <fcntl.h>

namespace XTL
{
	typedef int8_t   INT_8;
	typedef int16_t  INT_16;
	typedef int32_t  INT_32;
	typedef int64_t  INT_64;

	typedef uint8_t  UINT_8;
	typedef uint16_t UINT_16;
	typedef uint32_t UINT_32;
	typedef uint64_t UINT_64;

	/**
	 * @typedef FileSize
	 * @brief Числовой знаковый тип для хранения размера файла и смещения указателя в файле.
	 */
	typedef off64_t FileSize;
}

#endif
