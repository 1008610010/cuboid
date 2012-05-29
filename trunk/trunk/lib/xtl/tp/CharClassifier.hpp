#ifndef XTL__CHAR_CLASSIFIER_HPP__
#define XTL__CHAR_CLASSIFIER_HPP__ 1

#include "../Types.hpp"

namespace XTL
{
	class CharClassifier
	{
		public:

			static const unsigned int CHARS_COUNT = sizeof(char);

			CharClassifier();

			XTL::UINT_32 operator[] (char c) const
			{
				return charBits_[static_cast<unsigned char>(c)];
			}

		protected:

			CharClassifier & Add(XTL::UINT_32 bits, char c);

			CharClassifier & Add(XTL::UINT_32 bits, char from, char to);

		private:

			XTL::UINT_32 & operator[] (char c)
			{
				return charBits_[static_cast<unsigned char>(c)];
			}

			XTL::UINT_32 charBits_[CHARS_COUNT];
	};
}

#endif

