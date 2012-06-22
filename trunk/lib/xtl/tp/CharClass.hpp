#ifndef XTL__CHAR_CLASS_HPP__
#define XTL__CHAR_CLASS_HPP__ 1

#include "../Types.hpp"
#include "CharClassifier.hpp"

namespace XTL
{
	class CharClass
	{
		public:

			CharClass(const CharClassifier & classifier, XTL::UINT_32 classBits)
				: classifier_(classifier),
				  classBits_(classBits)
			{
				;;
			}

			bool Contains(char c) const
			{
				return (classifier_[c] & classBits_) != 0;
			}

		private:

			const CharClassifier & classifier_;
			XTL::UINT_32 classBits_;
	};
}

#endif

