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

	int GetLowestBit0(UINT_8 byte)
	{
		static const int BIT[256] = {
			0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
			0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,
			0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
			0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 6,
			0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
			0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,
			0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
			0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 7,
			0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
			0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,
			0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
			0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 6,
			0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
			0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5,
			0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4,
			0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,-1
		};
		return BIT[byte];
	}

	int GetLowestBit1(UINT_8 byte)
	{
		static const int BIT[256] = {
		   -1, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		    7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
		    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
		};
		return BIT[byte];
	}

	int GetCountBits0(UINT_8 byte)
	{
		return 8 - GetBit1Count(byte);
	}

	int GetCountBits1(UINT_8 byte)
	{
		static const int COUNT[256] = {
		    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
		    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
		    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
		    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
		    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
		    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
		    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
		    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
		    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
		    4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
		};
		return COUNT[byte];
	}

	UINT_32 GetPackedBits0(UINT_8 byte)
	{
		static const UINT_32 BITS[256] = {
			0x76543210, 0xf7654321, 0xf7654320, 0xff765432, 0xf7654310, 0xff765431, 0xff765430, 0xfff76543,
			0xf7654210, 0xff765421, 0xff765420, 0xfff76542, 0xff765410, 0xfff76541, 0xfff76540, 0xffff7654,
			0xf7653210, 0xff765321, 0xff765320, 0xfff76532, 0xff765310, 0xfff76531, 0xfff76530, 0xffff7653,
			0xff765210, 0xfff76521, 0xfff76520, 0xffff7652, 0xfff76510, 0xffff7651, 0xffff7650, 0xfffff765,
			0xf7643210, 0xff764321, 0xff764320, 0xfff76432, 0xff764310, 0xfff76431, 0xfff76430, 0xffff7643,
			0xff764210, 0xfff76421, 0xfff76420, 0xffff7642, 0xfff76410, 0xffff7641, 0xffff7640, 0xfffff764,
			0xff763210, 0xfff76321, 0xfff76320, 0xffff7632, 0xfff76310, 0xffff7631, 0xffff7630, 0xfffff763,
			0xfff76210, 0xffff7621, 0xffff7620, 0xfffff762, 0xffff7610, 0xfffff761, 0xfffff760, 0xffffff76,
			0xf7543210, 0xff754321, 0xff754320, 0xfff75432, 0xff754310, 0xfff75431, 0xfff75430, 0xffff7543,
			0xff754210, 0xfff75421, 0xfff75420, 0xffff7542, 0xfff75410, 0xffff7541, 0xffff7540, 0xfffff754,
			0xff753210, 0xfff75321, 0xfff75320, 0xffff7532, 0xfff75310, 0xffff7531, 0xffff7530, 0xfffff753,
			0xfff75210, 0xffff7521, 0xffff7520, 0xfffff752, 0xffff7510, 0xfffff751, 0xfffff750, 0xffffff75,
			0xff743210, 0xfff74321, 0xfff74320, 0xffff7432, 0xfff74310, 0xffff7431, 0xffff7430, 0xfffff743,
			0xfff74210, 0xffff7421, 0xffff7420, 0xfffff742, 0xffff7410, 0xfffff741, 0xfffff740, 0xffffff74,
			0xfff73210, 0xffff7321, 0xffff7320, 0xfffff732, 0xffff7310, 0xfffff731, 0xfffff730, 0xffffff73,
			0xffff7210, 0xfffff721, 0xfffff720, 0xffffff72, 0xfffff710, 0xffffff71, 0xffffff70, 0xfffffff7,
			0xf6543210, 0xff654321, 0xff654320, 0xfff65432, 0xff654310, 0xfff65431, 0xfff65430, 0xffff6543,
			0xff654210, 0xfff65421, 0xfff65420, 0xffff6542, 0xfff65410, 0xffff6541, 0xffff6540, 0xfffff654,
			0xff653210, 0xfff65321, 0xfff65320, 0xffff6532, 0xfff65310, 0xffff6531, 0xffff6530, 0xfffff653,
			0xfff65210, 0xffff6521, 0xffff6520, 0xfffff652, 0xffff6510, 0xfffff651, 0xfffff650, 0xffffff65,
			0xff643210, 0xfff64321, 0xfff64320, 0xffff6432, 0xfff64310, 0xffff6431, 0xffff6430, 0xfffff643,
			0xfff64210, 0xffff6421, 0xffff6420, 0xfffff642, 0xffff6410, 0xfffff641, 0xfffff640, 0xffffff64,
			0xfff63210, 0xffff6321, 0xffff6320, 0xfffff632, 0xffff6310, 0xfffff631, 0xfffff630, 0xffffff63,
			0xffff6210, 0xfffff621, 0xfffff620, 0xffffff62, 0xfffff610, 0xffffff61, 0xffffff60, 0xfffffff6,
			0xff543210, 0xfff54321, 0xfff54320, 0xffff5432, 0xfff54310, 0xffff5431, 0xffff5430, 0xfffff543,
			0xfff54210, 0xffff5421, 0xffff5420, 0xfffff542, 0xffff5410, 0xfffff541, 0xfffff540, 0xffffff54,
			0xfff53210, 0xffff5321, 0xffff5320, 0xfffff532, 0xffff5310, 0xfffff531, 0xfffff530, 0xffffff53,
			0xffff5210, 0xfffff521, 0xfffff520, 0xffffff52, 0xfffff510, 0xffffff51, 0xffffff50, 0xfffffff5,
			0xfff43210, 0xffff4321, 0xffff4320, 0xfffff432, 0xffff4310, 0xfffff431, 0xfffff430, 0xffffff43,
			0xffff4210, 0xfffff421, 0xfffff420, 0xffffff42, 0xfffff410, 0xffffff41, 0xffffff40, 0xfffffff4,
			0xffff3210, 0xfffff321, 0xfffff320, 0xffffff32, 0xfffff310, 0xffffff31, 0xffffff30, 0xfffffff3,
			0xfffff210, 0xffffff21, 0xffffff20, 0xfffffff2, 0xffffff10, 0xfffffff1, 0xfffffff0, 0xffffffff
		};

		return BITS[byte];
	}

	UINT_32 GetPackedBits1(UINT_8 byte)
	{
		static const UINT_32 BITS[256] = {
			0xffffffff, 0xfffffff0, 0xfffffff1, 0xffffff10, 0xfffffff2, 0xffffff20, 0xffffff21, 0xfffff210,
			0xfffffff3, 0xffffff30, 0xffffff31, 0xfffff310, 0xffffff32, 0xfffff320, 0xfffff321, 0xffff3210,
			0xfffffff4, 0xffffff40, 0xffffff41, 0xfffff410, 0xffffff42, 0xfffff420, 0xfffff421, 0xffff4210,
			0xffffff43, 0xfffff430, 0xfffff431, 0xffff4310, 0xfffff432, 0xffff4320, 0xffff4321, 0xfff43210,
			0xfffffff5, 0xffffff50, 0xffffff51, 0xfffff510, 0xffffff52, 0xfffff520, 0xfffff521, 0xffff5210,
			0xffffff53, 0xfffff530, 0xfffff531, 0xffff5310, 0xfffff532, 0xffff5320, 0xffff5321, 0xfff53210,
			0xffffff54, 0xfffff540, 0xfffff541, 0xffff5410, 0xfffff542, 0xffff5420, 0xffff5421, 0xfff54210,
			0xfffff543, 0xffff5430, 0xffff5431, 0xfff54310, 0xffff5432, 0xfff54320, 0xfff54321, 0xff543210,
			0xfffffff6, 0xffffff60, 0xffffff61, 0xfffff610, 0xffffff62, 0xfffff620, 0xfffff621, 0xffff6210,
			0xffffff63, 0xfffff630, 0xfffff631, 0xffff6310, 0xfffff632, 0xffff6320, 0xffff6321, 0xfff63210,
			0xffffff64, 0xfffff640, 0xfffff641, 0xffff6410, 0xfffff642, 0xffff6420, 0xffff6421, 0xfff64210,
			0xfffff643, 0xffff6430, 0xffff6431, 0xfff64310, 0xffff6432, 0xfff64320, 0xfff64321, 0xff643210,
			0xffffff65, 0xfffff650, 0xfffff651, 0xffff6510, 0xfffff652, 0xffff6520, 0xffff6521, 0xfff65210,
			0xfffff653, 0xffff6530, 0xffff6531, 0xfff65310, 0xffff6532, 0xfff65320, 0xfff65321, 0xff653210,
			0xfffff654, 0xffff6540, 0xffff6541, 0xfff65410, 0xffff6542, 0xfff65420, 0xfff65421, 0xff654210,
			0xffff6543, 0xfff65430, 0xfff65431, 0xff654310, 0xfff65432, 0xff654320, 0xff654321, 0xf6543210,
			0xfffffff7, 0xffffff70, 0xffffff71, 0xfffff710, 0xffffff72, 0xfffff720, 0xfffff721, 0xffff7210,
			0xffffff73, 0xfffff730, 0xfffff731, 0xffff7310, 0xfffff732, 0xffff7320, 0xffff7321, 0xfff73210,
			0xffffff74, 0xfffff740, 0xfffff741, 0xffff7410, 0xfffff742, 0xffff7420, 0xffff7421, 0xfff74210,
			0xfffff743, 0xffff7430, 0xffff7431, 0xfff74310, 0xffff7432, 0xfff74320, 0xfff74321, 0xff743210,
			0xffffff75, 0xfffff750, 0xfffff751, 0xffff7510, 0xfffff752, 0xffff7520, 0xffff7521, 0xfff75210,
			0xfffff753, 0xffff7530, 0xffff7531, 0xfff75310, 0xffff7532, 0xfff75320, 0xfff75321, 0xff753210,
			0xfffff754, 0xffff7540, 0xffff7541, 0xfff75410, 0xffff7542, 0xfff75420, 0xfff75421, 0xff754210,
			0xffff7543, 0xfff75430, 0xfff75431, 0xff754310, 0xfff75432, 0xff754320, 0xff754321, 0xf7543210,
			0xffffff76, 0xfffff760, 0xfffff761, 0xffff7610, 0xfffff762, 0xffff7620, 0xffff7621, 0xfff76210,
			0xfffff763, 0xffff7630, 0xffff7631, 0xfff76310, 0xffff7632, 0xfff76320, 0xfff76321, 0xff763210,
			0xfffff764, 0xffff7640, 0xffff7641, 0xfff76410, 0xffff7642, 0xfff76420, 0xfff76421, 0xff764210,
			0xffff7643, 0xfff76430, 0xfff76431, 0xff764310, 0xfff76432, 0xff764320, 0xff764321, 0xf7643210,
			0xfffff765, 0xffff7650, 0xffff7651, 0xfff76510, 0xffff7652, 0xfff76520, 0xfff76521, 0xff765210,
			0xffff7653, 0xfff76530, 0xfff76531, 0xff765310, 0xfff76532, 0xff765320, 0xff765321, 0xf7653210,
			0xffff7654, 0xfff76540, 0xfff76541, 0xff765410, 0xfff76542, 0xff765420, 0xff765421, 0xf7654210,
			0xfff76543, 0xff765430, 0xff765431, 0xf7654310, 0xff765432, 0xf7654320, 0xf7654321, 0x76543210
		};

		return BITS[byte];
	}
}
