#include "FileInputStream.hpp"
#include <stdlib.h>
#include <string.h>

namespace STXX
{
	FileInputStream::Buffer::Buffer(UINT_64 capacity)
		: capacity_(capacity),
		  size_(0),
		  position_(0),
		  data_(capacity == 0 ? 0 : static_cast<CHAR_8 *>(malloc(capacity)))
	{
		;;
	}

	FileInputStream::Buffer::~Buffer() throw()
	{
		free(data_);
	}

	void FileInputStream::Buffer::Clear()
	{
		position_ = 0;
		size_ = 0;
	}

	bool FileInputStream::Buffer::Read(File & file, void * buffer, UINT_64 size)
	{
		if (size <= size_ - position_)
		{
			memcpy(buffer, data_ + position_, size);
			position_ += size;
		}
		else
		{
			if (size < capacity_)
			{
				if (position_ < size_)
				{
					UINT_64 part = size_ - position_;
					memcpy(buffer, data_ + position_, part);
					if (!Load(file) || size - part > size_)
					{
						return false;
					}
					memcpy(static_cast<CHAR_8 *>(buffer) + part, data_, size - part);
					position_ = size - part;
				}
				else
				{
					if (!Load(file) || size > size_)
					{
						return false;
					}
					memcpy(buffer, data_, size);
					position_ = size;
				}
			}
			else
			{
				if (position_ < size_)
				{
					UINT_64 part = size_ - position_;
					memcpy(buffer, data_ + position_, part);
					if (!file.ReadBuffer(static_cast<CHAR_8 *>(buffer) + part, size - part))
					{
						return false;
					}
				}
				else
				{
					if (!file.ReadBuffer(buffer, size))
					{
						return false;
					}
				}
				position_ = 0;
				size_ = 0;
			}
		}
		return true;
	}

	bool FileInputStream::Buffer::Load(File & file)
	{
		size_ = file.Size() - file.Position();
		if (size_ > capacity_)
		{
			size_ = capacity_;
		}
		position_ = 0;
		return file.ReadBuffer(data_, size_);
	}
}
