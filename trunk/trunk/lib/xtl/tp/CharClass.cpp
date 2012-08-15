#include "CharClass.hpp"

#include "CharClassifier.hpp"

namespace XTL
{
	CharClass::CharClass(const CharClassifier & classifier, XTL::UINT_32 classBits)
		: classifier_(classifier),
		  classBits_(classBits)
	{
		;;
	}

	bool CharClass::Contains(char c) const
	{
		return (classifier_[c] & classBits_) != 0;
	}
}

