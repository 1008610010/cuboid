#ifndef XTL__CHAR_CLASS_HPP__
#define XTL__CHAR_CLASS_HPP__ 1

#include "../Types.hpp"
#include "CharClassifier.hpp"

namespace XTL
{
	class CharClass
	{
		public:

			CharClass(const CharClassifier & classifier, XTL::UINT_32 bits)
				: classifier_(classifier),
				  bits_(bits)
			{
				;;
			}

			bool Contains(char c) const
			{
				return (classifier_[c] & bits_) != 0;
			}

		private:

			const CharClassifier & classifier_;
			XTL::UINT_32 bits_;
	};
}

#endif

