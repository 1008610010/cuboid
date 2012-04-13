#ifndef _STXX__BIT_ARRAY_HPP__
#define _STXX__BIT_ARRAY_HPP__ 1

#include <sup/Types.h>
#include <string.h>

namespace STXX
{
	template <UINT_64 BYTES_COUNT> class BitArray
	{
		public:

			BitArray();

			UINT_8 & operator[] (UINT_32 index);

			bool GetBit(UINT_64 bitIndex);

			bool SetBit(UINT_64 bitIndex);

			bool ClearBit(UINT_64 bitIndex);

			INT_64 GetFirstZero(UINT_64 byteIndex);

		private:

			UINT_8 data_[BYTES_COUNT];
	};
}

#include "BitArray.tcc"

#endif
