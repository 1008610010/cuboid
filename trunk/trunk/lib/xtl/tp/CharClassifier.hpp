#ifndef XTL__CHAR_CLASSIFIER_HPP__
#define XTL__CHAR_CLASSIFIER_HPP__ 1

#include "../Types.hpp"
#include "CharClass.hpp"

namespace XTL
{
	class CharClassifier
	{
		public:

			static const unsigned int CHARS_COUNT = (1 << (sizeof(char) << 3));

			CharClassifier();

			CharClassBits operator[] (char c) const
			{
				return charBits_[static_cast<unsigned char>(c)];
			}

			CharClass CreateClass(CharClassBits classBits) const
			{
				return CharClass(*this, classBits);
			}

		protected:

			CharClassifier & Add(CharClassBits bits, char c);

			CharClassifier & Add(CharClassBits bits, char from, char to);

		private:

			CharClassBits & operator[] (char c)
			{
				return charBits_[static_cast<unsigned char>(c)];
			}

			CharClassBits charBits_[CHARS_COUNT];
	};
/*
	template <typename T>
	class CharClassifierSingleton : public CharClassifier
	{
		public:

			static const CharClassifier & Instance()
			{
				static T instance;

				return instance;
			}

			static CharClass CreateClass(CharClassBits classBits) const
			{
				return CharClass(*this, classBits);
			}
	};
*/
}

#endif

