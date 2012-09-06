#include "FileInputStreamBuffered.hpp"

#include <string.h>

#include "../utils/Algorithm.hpp"

namespace XTL
{
	FileInputStreamBuffered::FileInputStreamBuffered(const std::string & filePath, unsigned int bufferCapacity)
		: inputStream_(filePath),
		  buffer_(bufferCapacity)
	{
		;;
	}

	FileInputStreamBuffered::~FileInputStreamBuffered() throw()
	{
		;;
	}

	bool FileInputStreamBuffered::AtEnd() const
	{
		return buffer_.AtEnd() && inputStream_.AtEnd();
	}

	void FileInputStreamBuffered::Read(void * buffer, unsigned int size)
	{
		unsigned int wasRead = buffer_.Read(buffer, size);

		size -= wasRead;
		if (size == 0)
		{
			return;
		}

		buffer = static_cast<char *>(buffer) + wasRead;

		if (size >= buffer_.Capacity())
		{
			inputStream_.Read(buffer, size);
		}
		else
		{
			buffer_.Update(inputStream_, size);
			buffer_.Read(buffer, size);
		}
	}

	FileSize FileInputStreamBuffered::Size() const
	{
		return inputStream_.Size();
	}

	FileSize FileInputStreamBuffered::Position() const
	{
		return inputStream_.Position() - buffer_.AvailableSize();
	}

	FileSize FileInputStreamBuffered::AvailableSize() const
	{
		return inputStream_.AvailableSize() + buffer_.AvailableSize();
	}

	const std::string & FileInputStreamBuffered::FilePath() const throw()
	{
		return inputStream_.FilePath();
	}



	FileInputStreamBuffered::Buffer::Buffer(unsigned int capacity)
		: data_(capacity > 0 ? new char[capacity] : 0),
		  capacity_(capacity),
		  size_(0),
		  position_(0)
	{
		;;
	}

	FileInputStreamBuffered::Buffer::~Buffer() throw()
	{
		delete [] data_;
	}

	bool FileInputStreamBuffered::Buffer::AtEnd() const
	{
		return position_ >= size_;
	}

	unsigned int FileInputStreamBuffered::Buffer::Capacity() const
	{
		return capacity_;
	}

	unsigned int FileInputStreamBuffered::Buffer::AvailableSize() const
	{
		return size_ - position_;
	}

	unsigned int FileInputStreamBuffered::Buffer::Read(void * buffer, unsigned int size)
	{
		size = Min(size, AvailableSize());

		if (size == 0)
		{
			return 0;
		}

		::memcpy(buffer, data_ + position_, size);
		position_ += size;

		return size;
	}

	void FileInputStreamBuffered::Buffer::Update(FileInputStream & inputStream, unsigned int minimumSize)
	{
		// TODO: Assert(minimumSize <= Capacity())

		size_ = capacity_;

		if (inputStream.AvailableSize() < capacity_)
		{
			size_ = inputStream.AvailableSize();

			if (size_ < minimumSize)
			{
				size_ = minimumSize;
			}
		}

		position_ = 0;

		inputStream.Read(data_, size_);
	}
}

