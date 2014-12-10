#ifndef _XTL__BIT_UTILS_HPP__
#define _XTL__BIT_UTILS_HPP__ 1

namespace XTL
{
	inline const unsigned int GetBitByte(unsigned int index)
	{
		return index >> 3;
	}

	inline const char GetBitMask(unsigned int index)
	{
		static char MASK[] = {
			'\x01', '\x02', '\x04', '\x08', '\x10', '\x20', '\x40', '\x80' 
		};

		return MASK[index & 0x07];
	}

	inline const char GetBitNotMask(unsigned int index)
	{
		static char MASK[] = {
			'\xfe', '\xfd', '\xfb', '\xf7', '\xef', '\xdf', '\xbf', '\x7f'
		};

		return MASK[index & 0x07];
	}

	inline const bool GetBit(const char * bits, unsigned int index)
	{
		return (bits[GetBitByte(index)] & GetBitMask(index)) != 0;
	}

	inline void SetBit(char * bits, unsigned int index)
	{
		bits[GetBitByte(index)] |= GetBitMask(index);
	}

	inline void ClearBit(char * bits, unsigned int index)
	{
		bits[GetBitByte(index)] &= GetBitNotMask(index);
	}

	inline void InvertBit(char * bits, unsigned int index)
	{
		bits[GetBitByte(index)] ^= GetBitMask(index);
	}

	void SetBits(char * bits, unsigned int indexFrom, unsigned int indexTo);

	void ClearBits(char * bits, unsigned int indexFrom, unsigned int indexTo);

	void InvertBits(char * bits, unsigned int indexFrom, unsigned int indexTo);

	int GetLowestBit0(UINT_8 byte);

	int GetLowestBit1(UINT_8 byte);

	int GetCountBits0(UINT_8 byte);

	int GetCountBits1(UINT_8 byte);

	UINT_32 GetPackedBits0(UINT_8 byte);

	UINT_32 GetPackedBits1(UINT_8 byte);

	/*
	int GetRandomBit0(UINT_8 byte)
	{
		const int n = GetCountBits0(byte);
		if (n == 0)
		{
			return -1;
		}

		const UINT_32 b = GetPackedBits0(byte);
		return (b >> ((Random::Generate() % n) << 2)) & 0x0f;
	}

	int GetRandomBit1(UINT_8 byte)
	{
		const int n = GetCountBits1(byte);
		if (n == 0)
		{
			return -1;
		}

		const UINT_32 b = GetPackedBits1(byte);
		return (b >> ((Random::Generate() % n) << 2)) & 0x0f;
	}
	*
	UINT_32 Div8Ceil(UINT_32 x)
	{
		return ((x - 1) >> 3) + 1;
	}

	UINT_32 Log8Ceil(UINT_32 x)
	{
		UINT_32 result = 1;
		for (UINT_32 i = x; i > 8; i = Div8Ceil(i))
		{
			result++;
		}
		return result;
	}

	bool GetByteBit(UINT_8 byte, UINT_32 index)
	{
		static const UINT_8 MASK[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
		return (byte & MASK[index]) != 0;
	}

	void SetByteBit(UINT_8 & byte, UINT_32 index)
	{
		static const UINT_8 MASK[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
		byte |= MASK[index];
	}

	void ClearByteBit(UINT_8 & byte, UINT_32 index)
	{
		static const UINT_8 MASK[] = { 0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f };
		byte &= MASK[index];
	}
	*/

}

#endif
