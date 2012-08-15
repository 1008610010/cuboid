#ifndef XTL__CHAR_CLASS_HPP__
#define XTL__CHAR_CLASS_HPP__ 1

#include "../Types.hpp"

namespace XTL
{
	typedef XTL::UINT_32 CharClassBits;

	class CharClassifier;

	class CharClass
	{
		public:

			CharClass(const CharClassifier & classifier, XTL::UINT_32 classBits);

			bool Contains(char c) const;

		private:

			const CharClassifier & classifier_;
			CharClassBits classBits_;
	};
}

#endif

