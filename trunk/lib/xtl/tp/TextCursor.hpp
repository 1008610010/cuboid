#ifndef XTL__TEXT_CURSOR_HPP__
#define XTL__TEXT_CURSOR_HPP__ 1

namespace XTL
{
	class TextCursor
	{
		public:

			TextCursor()
				: row_(0),
				  column_(0)
			{
				;;
			}

			unsigned int Row() const
			{
				return row_;
			}

			unsigned int Column() const
			{
				return column_;
			}

			void NextChar()
			{
				++column_;
			}

			void NextLine()
			{
				++row_;
				column_ = 0;
			}

		private:

			unsigned int row_;
			unsigned int column_;
	};
}

#endif

