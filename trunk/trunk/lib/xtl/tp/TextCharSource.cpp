#include "TextCharSource.hpp"

#include <stdexcept>

namespace XTL
{
	const TextCursor TextCharSource::ConstCharPtr::GetCursor() const
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

	unsigned int TextCharSource::ConstCharPtr::Mark()
	{
		markedList_.push_back(ptr_);
		return markedList_.size();
	}

	void TextCharSource::ConstCharPtr::Unmark()
	{
		if (markedList_.empty())
		{
			throw std::runtime_error("Attempt to Unmark() not marked CharSource");
		}

		markedList_.pop_back();
	}

	const std::string TextCharSource::ConstCharPtr::ReleaseString()
	{
		if (markedList_.empty())
		{
			throw std::runtime_error("Attempt to ReleaseString() from not marked CharSource");
		}

		const char * from = markedList_.back();
		markedList_.pop_back();

		return std::string(from, ptr_ - from);
	}
}

