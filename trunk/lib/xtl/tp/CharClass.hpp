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

			CharClass(const CharClassifier & classifier, CharClassBits classBits);

			CharClassBits Bits() const
			{
				return classBits_;
			}

			bool Contains(char c) const;

			static const CharClass DECIMAL;
			static const CharClass HEXADECIMAL;
			static const CharClass OCTAL;
			static const CharClass BINARY;
			static const CharClass IDENTIFIER_HEAD;
			static const CharClass IDENTIFIER_TAIL;

		private:

			const CharClassifier & classifier_;
			CharClassBits classBits_;
	};
}

#endif

