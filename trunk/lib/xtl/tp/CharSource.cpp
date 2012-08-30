#include "CharSource.hpp"

#include "../CharBuffer.hpp"

namespace XTL
{
	CharSource::ConstCharPtr::ConstCharPtr(const char * ptr, unsigned int size)
		: ptr_(ptr),
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

	void CharSource::ConstCharPtr::Advance()
	{
		++ptr_;
	}

	void CharSource::ConstCharPtr::Mark()
	{
		marked_.push(ptr_);
	}

	void CharSource::ConstCharPtr::Unmark()
	{
		PopPtr();
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
}
