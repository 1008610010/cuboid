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
}

#endif
