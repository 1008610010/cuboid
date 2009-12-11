#include "BitUtils.hpp"

namespace XTL
{
	void SetBits(char * bits, unsigned int indexFrom, unsigned int indexTo)
	{
		/*
		 * HIGHER[i] = 1 1 ... 1(i) 0 0 ... 0
		 */
		static const char HIGHER[] = {
			'\xff', '\xfe', '\xfc', '\xf8', '\xf0', '\xe0', '\xc0', '\x80'
		};

		/*
		 * LOWER[i] = 0 0 ... 0 1(i) 1 ... 1
		 */
		static const char LOWER[] = {
			'\x01', '\x03', '\x07', '\x0f', '\x1f', '\x3f', '\x7f', '\xff'
		};

		unsigned int byteFrom = GetBitByte(indexFrom);
		unsigned int byteTo = GetBitByte(indexTo);

		if (byteFrom < byteTo)
		{
			bits[byteFrom] |= HIGHER[indexFrom & 0x07];
			for (++byteFrom; byteFrom < byteTo; ++byteFrom)
			{
				bits[byteFrom] |= '\xff';
			}
			bits[byteTo] |= LOWER[indexTo & 0x07];
		}
		else
		{
			bits[byteFrom] |= (HIGHER[indexFrom & 0x07] & LOWER[indexTo & 0x07]);
		}
	}

	void ClearBits(char * bits, unsigned int indexFrom, unsigned int indexTo)
	{
		/*
		 * HIGHER[i] = 0 0 ... 0(i) 1 1 ... 1
		 */
		static const char HIGHER[] = {
			'\x00', '\x01', '\x03', '\x07', '\x0f', '\x1f', '\x3f', '\x7f'
		};

		/*
		 * LOWER[i] = 1 1 ... 1 0(i) 0 ... 0
		 */
		static const char LOWER[] = {
			'\xfe', '\xfc', '\xf8', '\xf0', '\xe0', '\xc0', '\x80', '\x00'
		};

		unsigned int byteFrom = GetBitByte(indexFrom);
		unsigned int byteTo = GetBitByte(indexTo);

		if (byteFrom < byteTo)
		{
			bits[byteFrom] &= HIGHER[indexFrom & 0x07];
			for (++byteFrom; byteFrom < byteTo; ++byteFrom)
			{
				bits[byteFrom] &= '\x00';
			}
			bits[byteTo] &= LOWER[indexTo & 0x07];
		}
		else
		{
			bits[byteFrom] &= ~(~HIGHER[indexFrom & 0x07] & ~LOWER[indexTo & 0x07]);
		}
	}

	void InvertBits(char * bits, unsigned int indexFrom, unsigned int indexTo)
	{
		/*
		 * HIGHER[i] = 1 1 ... 1(i) 0 0 ... 0
		 */
		static const char HIGHER[] = {
			'\xff', '\xfe', '\xfc', '\xf8', '\xf0', '\xe0', '\xc0', '\x80'
		};

		/*
		 * LOWER[i] = 0 0 ... 0 1(i) 1 ... 1
		 */
		static const char LOWER[] = {
			'\x01', '\x03', '\x07', '\x0f', '\x1f', '\x3f', '\x7f', '\xff'
		};

		unsigned int byteFrom = GetBitByte(indexFrom);
		unsigned int byteTo = GetBitByte(indexTo);

		if (byteFrom < byteTo)
		{
			bits[byteFrom] ^= HIGHER[indexFrom & 0x07];
			for (++byteFrom; byteFrom < byteTo; ++byteFrom)
			{
				bits[byteFrom] ^= '\xff';
			}
			bits[byteTo] ^= LOWER[indexTo & 0x07];
		}
		else
		{
			bits[byteFrom] ^= (HIGHER[indexFrom & 0x07] & LOWER[indexTo & 0x07]);
		}
	}
}
