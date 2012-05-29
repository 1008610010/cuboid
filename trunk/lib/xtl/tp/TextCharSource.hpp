#ifndef XTL__TEXT_CHAR_SOURCE_HPP__
#define XTL__TEXT_CHAR_SOURCE_HPP__ 1

#include <list>
#include <string>

#include "CharClass.hpp"
#include "CharSource.hpp"

namespace XTL
{
	class TextCharSource : public CharSource
	{
		public:

			static const char NULL_CHAR = '\0';

			virtual ~TextCharSource() throw()
			{
				;;
			}

			virtual bool NotAtEnd() const
			{
				return GetChar() != NULL_CHAR;
			}

			virtual bool AtEnd() const
			{
				return GetChar() == NULL_CHAR;
			}

			class ConstCharPtr;

		private:

			TextCursor textCursor_;
	};

	class TextCharSource::ConstCharPtr : public TextCharSource
	{
		public:

			explicit ConstCharPtr(const char * ptr)
				: begin_(ptr),
				  ptr_(ptr),
				  markedList_()
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

			virtual void Advance()
			{
				++ptr_;
			}

			virtual const TextCursor GetCursor() const;

			virtual void Mark();

			virtual void Unmark();

			virtual const std::string ReleaseString();

		private:

			const char * begin_;
			const char * ptr_;
			std::list<const char *> markedList_;
	};
}

#endif

