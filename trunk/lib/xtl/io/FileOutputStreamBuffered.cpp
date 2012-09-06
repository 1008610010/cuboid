#include "FileOutputStreamBuffered.hpp"

#include <string.h>

namespace XTL
{
	FileOutputStreamBuffered::FileOutputStreamBuffered(const std::string & filePath, unsigned int bufferCapacity)
		: outputStream_(filePath),
		  buffer_(bufferCapacity),
		  position_(0)
	{
		;;
	}

	FileOutputStreamBuffered::~FileOutputStreamBuffered() throw()
	{
		Flush();
	}

	void FileOutputStreamBuffered::Write(const void * buffer, unsigned int size)
	{
		if (size <= buffer_.FreeSize())
		{
			buffer_.Write(buffer, size);
		}
		else
		{
			buffer_.Flush(outputStream_);

			if (size < buffer_.Capacity())
			{
				buffer_.Write(buffer, size);
			}
			else
			{
				outputStream_.Write(buffer, size);
			}
		}

		position_ += size;
	}

	FileSize FileOutputStreamBuffered::Position() const throw()
	{
		return outputStream_.Position() + buffer_.Size();
	}

	void FileOutputStreamBuffered::Flush()
	{
		buffer_.Flush(outputStream_);
	}

	void FileOutputStreamBuffered::Close()
	{
		Flush();
		outputStream_.Close();
	}



	FileOutputStreamBuffered::Buffer::Buffer(unsigned int capacity)
		: data_(capacity > 0 ? new char[capacity] : 0),
		  capacity_(capacity),
		  size_(0)
	{
		;;
	}

	FileOutputStreamBuffered::Buffer::~Buffer() throw()
	{
		delete [] data_;
	}

	unsigned int FileOutputStreamBuffered::Buffer::Capacity() const
	{
		return capacity_;
	}

	unsigned int FileOutputStreamBuffered::Buffer::Size() const
	{
		return size_;
	}

	unsigned int FileOutputStreamBuffered::Buffer::FreeSize() const
	{
		return capacity_ - size_;
	}

	void FileOutputStreamBuffered::Buffer::Write(const void * buffer, unsigned int size)
	{
		// TODO: Assert(size < FreeSize())
		::memcpy(data_ + size_, buffer, size);
		size_ += size;
	}

	void FileOutputStreamBuffered::Buffer::Flush(FileOutputStream & outputStream)
	{
		if (size_ == 0)
		{
			return;
		}

		outputStream.Write(data_, size_);
		size_ = 0;
	}
}

