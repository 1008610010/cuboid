#include "FileOutputStream.hpp"
#include <stdlib.h>
#include <string.h>

namespace STXX
{
	FileOutputStream::Buffer::Buffer(UINT_64 capacity)
		: capacity_(capacity),
		  size_(0),
		  data_(capacity == 0 ? NULL : static_cast<CHAR_8 *>(malloc(capacity)))
	{
		;;
	}

	FileOutputStream::Buffer::~Buffer() throw()
	{
		free(data_);
	}

	void FileOutputStream::Buffer::Clear()
	{
		size_ = 0;
	}

	bool FileOutputStream::Buffer::Write(File & file, const void * buffer, UINT_64 size)
	{
		if (size <= capacity_ - size_)
		{
			memcpy(data_ + size_, buffer, size);
			size_ += size;
		}
		else
		{
			if (!Flush(file))
			{
				return false;
			}
			if (size > capacity_)
			{
				return file.WriteBuffer(buffer, size);
			}
			else
			{
				memcpy(data_, buffer, size);
				size_ = size;
			}
		}
		return true;
	}

	bool FileOutputStream::Buffer::Flush(File & file)
	{
		if (size_ > 0)
		{
			if (!file.WriteBuffer(data_, size_))
			{
				return false;
			}
			size_ = 0;
		}
		return true;
	}
}
