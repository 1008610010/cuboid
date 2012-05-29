#ifndef XTL__TEXT_CHAR_SOURCE_HPP__
#define XTL__TEXT_CHAR_SOURCE_HPP__ 1

#include <list>
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

			virtual TextCursor GetCursor() const = 0;

			virtual void Mark() = 0;

			virtual void Unmark() = 0;

			virtual const std::string ReleaseString() = 0;
	};

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

			virtual TextCursor GetCursor() const
			{
				TextCursor result;

				for (const char * p = begin_; p != ptr_; ++p)
				{
					if (*p == '\n')
					{
						result.NextRow();
					}
					else
					{
						result.NextColumn();
					}
				}

				// TODO: May be cache result for cur_ position?
				return result;
			}

			virtual void Mark()
			{
				markedList_.push_back(ptr_);
			}

			virtual void Unmark()
			{
				if (markedList_.empty())
				{
					throw std::runtime_error("Attempt to Unmark() not marked CharSource");
				}

				markedList_.pop_back();
			}

			virtual const std::string ReleaseString()
			{
				if (markedList_.empty())
				{
					throw std::runtime_error("Attempt to ReleaseString() from not marked CharSource");
				}

				const char * from = markedList_.back();
				markedList_.pop_back();

				return std::string(from, ptr_ - from);
			}

		private:

			const char * begin_;
			const char * ptr_;
			std::list<const char *> markedList_;
	};
}

#endif

