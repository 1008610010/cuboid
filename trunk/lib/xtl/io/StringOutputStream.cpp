#include "StringOutputStream.hpp"

namespace XTL
{
	StringOutputStream::StringOutputStream()
		: chars_() { ;; }

	void StringOutputStream::Write(const void * buffer, unsigned int size)
	{
		chars_.insert(chars_.end(), static_cast<const char *>(buffer), static_cast<const char *>(buffer) + size);
	}

	const std::string StringOutputStream::ToString() const
	{
		return std::string(chars_.begin(), chars_.end());
	}
}

