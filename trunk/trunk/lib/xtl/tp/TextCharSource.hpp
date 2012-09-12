#ifndef XTL__TEXT_CHAR_SOURCE_HPP__
#define XTL__TEXT_CHAR_SOURCE_HPP__ 1

#include <list>
#include <memory>
#include <string>

#include "CharSource.hpp"

namespace XTL
{
	class TextCharSource : public CharSource
	{
		public:

			explicit TextCharSource(std::auto_ptr<CharSource> charSource);

			virtual ~TextCharSource() throw();

			virtual bool NotAtEnd() const;

			virtual bool AtEnd() const;

			virtual char GetChar() const;

			virtual unsigned int GetCharIndex() const;

			virtual void Advance();

			virtual unsigned int Mark();

			virtual void Unmark();

			virtual unsigned int MarkIndex() const;

			virtual const std::string ReleaseString();

			virtual void ReleaseString(CharBuffer & buffer);

			virtual const TextCursor GetCursor() const;

			// TODO: make SaveCursor() or not?
			virtual const TextCursor ReleaseCursor();

			virtual void RestorePosition();

		private:

			std::auto_ptr<CharSource> charSource_;
			TextCursor textCursor_;
	};
}

#endif
