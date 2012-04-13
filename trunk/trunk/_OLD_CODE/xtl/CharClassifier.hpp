#ifndef _XTL__CHAR_CLASSIFIER_HPP__
#define _XTL__CHAR_CLASSIFIER_HPP__ 1

namespace XTL
{
	enum CharClass
	{
		CHAR_DIGIT   = 0x0001,
		CHAR_ALPHA   = 0x0002,
		CHAR_NEWLINE = 0x0004,
		CHAR_SPACE   = 0x0008,
	};

	class TextPosition
	{
		public:

			TextPosition()
				: row_(0), column_(0) { ;; }

			TextPosition(unsigned int row, unsigned int column)
				: row_(row), column_(column) { ;; }

			void Advance()
			{
				++column_;
			}

			void NewLine()
			{
				++row_;
				column_ = 0;
			}

			unsigned int Row() const
			{
				return row_;
			}

			unsigned int Column() const
			{
				return column_;
			}

		protected:

			unsigned int row_;
			unsigned int column_;
	};

	class CharClassifier
	{
		public:

			CharClassifier()
			{
				;;
			}

			unsigned int Process(const char *& v)
			{
				
			}

		protected:

			
	};
}

#endif
