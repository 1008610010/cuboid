#include "CharSource.hpp"

#include "../CharBuffer.hpp"

namespace XTL
{
	CharSource::ConstCharPtr::ConstCharPtr(const char * ptr, unsigned int size)
		: ptr_(ptr),
		  begin_(ptr),
		  end_(ptr + size),
		  marked_()
	{
		;;
	}

	CharSource::ConstCharPtr::~ConstCharPtr() throw()
	{
		;;
	}

	bool CharSource::ConstCharPtr::NotAtEnd() const
	{
		return ptr_ < end_;
	}

	bool CharSource::ConstCharPtr::AtEnd() const
	{
		return ptr_ >= end_;
	}

	char CharSource::ConstCharPtr::GetChar() const
	{
		return *ptr_;
	}

	unsigned int CharSource::ConstCharPtr::GetCharIndex() const
	{
		return ptr_ - begin_;
	}

	void CharSource::ConstCharPtr::Advance()
	{
		++ptr_;
	}

	unsigned int CharSource::ConstCharPtr::Mark()
	{
		marked_.push(ptr_);
		return marked_.size();
	}

	void CharSource::ConstCharPtr::Unmark()
	{
		PopPtr();
	}

	unsigned int CharSource::ConstCharPtr::MarkIndex() const
	{
		return marked_.size();
	}

	const std::string CharSource::ConstCharPtr::ReleaseString()
	{
		const char * begin = PopPtr();

		return std::string(begin, ptr_ - begin);
	}

	void CharSource::ConstCharPtr::ReleaseString(CharBuffer & buffer)
	{
		const char * begin = PopPtr();

		buffer.Append(begin, ptr_);
	}

	const TextCursor CharSource::ConstCharPtr::GetCursor() const
	{
		return GetCursor(ptr_);
	}

	const TextCursor CharSource::ConstCharPtr::ReleaseCursor()
	{
		return GetCursor(PopPtr());
	}

	void CharSource::ConstCharPtr::RestorePosition()
	{
		ptr_ = PopPtr();
	}

	const char * CharSource::ConstCharPtr::PopPtr()
	{
		if (marked_.empty())
		{
			throw std::runtime_error("CharSource was not marked");
		}

		const char * p = marked_.top();
		marked_.pop();
		return p;
	}

	const TextCursor CharSource::ConstCharPtr::GetCursor(const char * to) const
	{
		TextCursor cursor;

		for (const char * p = begin_; p < to; ++p)
		{
			if (*p == '\n')
			{
				cursor.NextRow();
			}
			else
			{
				cursor.NextColumn();
			}
		}

		return cursor;
	}
}
