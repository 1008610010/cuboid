#include "CharClassifier.hpp"

#include <string.h>

#include <algorithm> // for std::swap

namespace XTL
{
	CharClassifier::CharClassifier()
	{
		::memset(charBits_, '\0', CHARS_COUNT * sizeof(charBits_[0]));
	}

	CharClassifier & CharClassifier::Add(XTL::UINT_32 bits, char c)
	{
		(*this)[c] |= bits;

		return *this;
	}

	CharClassifier & CharClassifier::Add(XTL::UINT_32 bits, char from, char to)
	{
		unsigned int indexFrom = static_cast<unsigned char>(from);
		unsigned int indexTo = static_cast<unsigned char>(to);

		if (indexFrom > indexTo)
		{
			std::swap(indexFrom, indexTo);
		}

		for (unsigned int i = indexFrom; i <= indexTo; ++i)
		{
			charBits_[i] |= bits;
		}

		return *this;
	}
}

