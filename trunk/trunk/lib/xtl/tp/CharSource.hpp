#ifndef XTL__CHAR_SOURCE_HPP__
#define XTL__CHAR_SOURCE_HPP__ 1

#include <string>

#include "TextCursor.hpp"

namespace XTL
{
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

			virtual const TextCursor GetCursor() const = 0;

			virtual void Mark() = 0;

			virtual void Unmark() = 0;

			virtual const std::string ReleaseString() = 0;
	};
}

#endif

