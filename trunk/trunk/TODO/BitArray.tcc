namespace STXX
{
	template <UINT_64 BYTES_COUNT>
	BitArray<BYTES_COUNT>::BitArray()
	{
		memset(data_, '\0', BYTES_COUNT);
	}

	template <UINT_64 BYTES_COUNT>
	UINT_8 & BitArray<BYTES_COUNT>::operator[] (UINT_32 index)
	{
		return data_[index];
	}

	template <UINT_64 BYTES_COUNT>
	bool BitArray<BYTES_COUNT>::GetBit(UINT_64 bitIndex)
	{
		static const UINT_8 MASK[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
		return (data_[bitIndex >> 3] & MASK[bitIndex & 7]) != 0;
	}

	template <UINT_64 BYTES_COUNT>
	bool BitArray<BYTES_COUNT>::SetBit(UINT_64 bitIndex)
	{
		static const UINT_8 MASK[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

		data_[bitIndex >> 3] |= MASK[bitIndex & 7];
		return data_[bitIndex >> 3] == (UINT_8) 0xff;
	}

	template <UINT_64 BYTES_COUNT>
	bool BitArray<BYTES_COUNT>::ClearBit(UINT_64 bitIndex)
	{
		static const UCHAR_8 MASK[] = { 0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f };

		data_[bitIndex >> 3] &= MASK[bitIndex & 7];
		return data_[bitIndex >> 3] == (UINT_8) 0x00;
	}

	template <UINT_64 BYTES_COUNT>
	INT_64 BitArray<BYTES_COUNT>::GetFirstZero(UINT_64 byteIndex)
	{
		// TODO: make static global function GetFirstZero(UINT_8)
		static const INT_64 ZERO[256] = {
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
		return ZERO[data_[byteIndex]];
	}
}
