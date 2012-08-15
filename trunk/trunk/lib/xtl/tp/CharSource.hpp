#ifndef XTL__CHAR_SOURCE_HPP__
#define XTL__CHAR_SOURCE_HPP__ 1

#include <stack>
#include <stdexcept>
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

			virtual void Mark() = 0;

			virtual void Unmark() = 0;

			virtual const std::string ReleaseString() = 0;

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

			bool IsInCharSource(const CharSource & charSource) const
			{
				return charSource_ == &charSource;
			}

		private:

			const CharSource * const charSource_;
	};

	class CharSource::ConstCharPtr : public CharSource
	{
		public:

			ConstCharPtr(const char * ptr, unsigned int size)
				: ptr_(ptr),
				  end_(ptr + size),
				  marked_()
			{
				;;
			}

			virtual ~ConstCharPtr() throw()
			{
				;;
			}

			virtual bool NotAtEnd() const
			{
				return ptr_ < end_;
			}

			virtual bool AtEnd() const
			{
				return ptr_ >= end_;
			}

			virtual char GetChar() const
			{
				return *ptr_;
			}

			virtual void Advance()
			{
				++ptr_;
			}

			virtual void Mark()
			{
				marked_.push(ptr_);
			}

			virtual void Unmark()
			{
				PopPtr();
			}

			virtual const std::string ReleaseString()
			{
				const char * begin = PopPtr();

				return std::string(begin, ptr_ - begin);
			}

		private:

			ConstCharPtr(const ConstCharPtr &);
			ConstCharPtr & operator= (const ConstCharPtr &);

			const char * PopPtr()
			{
				if (marked_.empty())
				{
					throw std::runtime_error("CharSource was not marked");
				}

				const char * p = marked_.top();
				marked_.pop();
				return p;
			}

			const char * ptr_;
			const char * const end_;
			std::stack<const char *> marked_;
	};
}

#endif

