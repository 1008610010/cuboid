#ifndef XTL__TEXT_CHAR_SOURCE_HPP__
#define XTL__TEXT_CHAR_SOURCE_HPP__ 1

#include <string>

#include "TextCursor.hpp"

namespace XTL
{
	class TextCharSource
	{
		public:

			static const char CHAR_NULL = '\0';

			TextCharSource()
				: textCursor_()
			{
				;;
			}

			virtual ~TextCharSource() throw()
			{
				;;
			}

			virtual bool NotAtEnd() const
			{
				return GetChar() != CHAR_NULL;
			}

			bool AtEnd() const
			{
				return !NotAtEnd();
			}

			virtual char GetChar() const = 0;

			virtual void Mark() = 0;

			virtual void Unmark() = 0;

			virtual const std::string GetString() const = 0;

			void Advance()
			{
				if (GetChar() == '\n')
				{
					textCursor_.NextLine();
				}
				else
				{
					textCursor_.NextChar();
				}

				DoAdvance();
			}

			const TextCursor & GetCursor() const
			{
				return textCursor_;
			}

			class ConstCharPtr;

		protected:

			virtual void DoAdvance() = 0;

		private:

			TextCursor textCursor_;
	};

	class TextCharSource::ConstCharPtr : public TextCharSource
	{
		public:

			explicit ConstCharPtr(const char * ptr)
				: ptr_(ptr),
				  marked_(0)
			{
				;;
			}

			virtual ~ConstCharPtr() throw()
			{
				;;
			}

			virtual char GetChar() const
			{
				return *ptr_;
			}

			virtual void Mark()
			{
				if (marked_ != 0)
				{
					throw std::runtime_error("");
				}

				marked_ = ptr_;
			}

			virtual void Unmark()
			{
				if (marked_ == 0)
				{
					throw std::runtime_error("");
				}

				marked_ = 0;
			}

			virtual const std::string GetString() const
			{
				if (marked_ == 0)
				{
					throw std::runtime_error("");
				}

				return std::string(marked_, ptr_ - marked_);
			}

		protected:

			virtual void DoAdvance()
			{
				++ptr_;
			}

		private:

			const char * ptr_;
			const char * marked_;
	};
}

#endif

