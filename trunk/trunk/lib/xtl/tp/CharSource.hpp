#ifndef XTL__CHAR_SOURCE_HPP__
#define XTL__CHAR_SOURCE_HPP__ 1

#include <stack>
#include <stdexcept>
#include <string>

#include "TextCursor.hpp"

namespace XTL
{
	class CharBuffer;

	class CharSource
	{
		public:

			virtual ~CharSource() throw()
			{
				;;
			}

			virtual bool NotAtEnd() const = 0;

			virtual bool AtEnd() const
			{
				return !NotAtEnd();
			}

			virtual char GetChar() const = 0;

			virtual void Advance() = 0;

			virtual unsigned int Mark() = 0;

			virtual void Unmark() = 0;

			virtual unsigned int MarkIndex() const = 0;

			virtual const std::string ReleaseString() = 0;

			virtual void ReleaseString(CharBuffer & buffer) = 0;

			virtual const TextCursor GetCursor() const = 0;

			virtual const TextCursor ReleaseCursor() = 0;

			class ConstCharPtr;
	};

	class CharSource::ConstCharPtr : public CharSource
	{
		public:

			ConstCharPtr(const char * ptr, unsigned int size);

			virtual ~ConstCharPtr() throw();

			virtual bool NotAtEnd() const;

			virtual bool AtEnd() const;

			virtual char GetChar() const;

			virtual void Advance();

			virtual unsigned int Mark();

			virtual void Unmark();

			virtual unsigned int MarkIndex() const;

			virtual const std::string ReleaseString();

			virtual void ReleaseString(CharBuffer & buffer);

			virtual const TextCursor GetCursor() const;

			virtual const TextCursor ReleaseCursor();

		private:

			ConstCharPtr(const ConstCharPtr &);
			ConstCharPtr & operator= (const ConstCharPtr &);

			const char * PopPtr();

			const TextCursor GetCursor(const char * to) const;

			const char * ptr_;
			const char * const begin_;
			const char * const end_;
			std::stack<const char *> marked_;
	};
}

#endif

