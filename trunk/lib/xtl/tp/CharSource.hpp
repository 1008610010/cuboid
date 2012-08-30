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

			virtual void Mark() = 0;

			virtual void Unmark() = 0;

			virtual const std::string ReleaseString() = 0;

			virtual void ReleaseString(CharBuffer & buffer) = 0;

			class ConstCharPtr;
	};

	class Position
	{
		public:

			explicit Position(const CharSource & charSource)
				: charSource_(&charSource)
			{
				;;
			}

			bool FromCharSource(const CharSource & charSource) const
			{
				return charSource_ == &charSource;
			}

		private:

			const CharSource * const charSource_;
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

			virtual void Mark();

			virtual void Unmark();

			virtual const std::string ReleaseString();

			virtual void ReleaseString(CharBuffer & buffer);

		private:

			ConstCharPtr(const ConstCharPtr &);
			ConstCharPtr & operator= (const ConstCharPtr &);

			const char * PopPtr();

			const char * ptr_;
			const char * const end_;
			std::stack<const char *> marked_;
	};
}

#endif

