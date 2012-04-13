#include "ByteBuffer.hpp"
#include "Exception.hpp"
#include <stdlib.h>
#include <string.h>

namespace XTL
{
	template <typename T> void SwapValues(T & x, T & y)
	{
		T temp(x);
		x = y;
		y = temp;
	}

	ByteBuffer::ByteBuffer()
		: capacity_(0),
		  size_(0),
		  data_(0)
	{
		;;
	}

	ByteBuffer::ByteBuffer(int capacity)
		: capacity_(0),
		  size_(0),
		  data_(0)
	{
		Reserve(capacity);
	}

	ByteBuffer::ByteBuffer(const ByteBuffer & buffer)
		: capacity_(0),
		  size_(0),
		  data_(0)
	{
		Append(buffer);
	}

	ByteBuffer::~ByteBuffer() throw()
	{
		::free(data_);
	}

	int ByteBuffer::Capacity() const
	{
		return capacity_;
	}

	int ByteBuffer::Size() const
	{
		return size_;
	}

	const char * ByteBuffer::Data() const
	{
		return data_;
	}

	bool ByteBuffer::Empty() const
	{
		return size_ == 0;
	}

	void ByteBuffer::Reserve(int size)
	{
		if (size <= capacity_ - size_)
		{
			return;
		}

		int newCapacity = capacity_ > 0 ? capacity_ : MIN_CAPACITY;
		while (size > newCapacity - size_)
		{
			newCapacity <<= 1;
		}

		char * newData = static_cast<char *>(::realloc(data_, newCapacity));
		if (newData == 0)
		{
			throw OutOfMemoryException("ByteBuffer::Reserve");
		}
		data_ = newData;
		capacity_ = newCapacity;
	}

	void ByteBuffer::Free()
	{
		::free(data_);
		capacity_ = 0;
		size_ = 0;
		data_ = 0;
	}

	void ByteBuffer::Clear()
	{
		size_ = 0;
	}

	ByteBuffer & ByteBuffer::operator= (const ByteBuffer & buffer)
	{
		if (this != &buffer)
		{
			Clear(); // or Free()?
			Append(buffer);
		}
		return *this;
	}

	void ByteBuffer::Swap(ByteBuffer & buffer)
	{
		SwapValues(capacity_, buffer.capacity_);
		SwapValues(size_, buffer.size_);
		SwapValues(data_, buffer.data_);
	}

	ByteBuffer & ByteBuffer::Append(const void * buffer, int size)
	{
		Reserve(size);
		::memcpy(data_ + size_, buffer, size);
		size_ += size;
		return *this;
	}

	ByteBuffer & ByteBuffer::Append(const ByteBuffer & buffer)
	{
		return Append(buffer.Data(), buffer.Size());
	}

	ByteBuffer & ByteBuffer::Append(const char * buffer)
	{
		return Append(buffer, ::strlen(buffer));
	}

	ByteBuffer & ByteBuffer::Append(const std::string & value)
	{
		return Append(value.data(), value.size());
	}

	ByteBuffer & ByteBuffer::operator<< (const ByteBuffer & buffer)
	{
		return Append(buffer);
	}

	ByteBuffer & ByteBuffer::operator<< (const char * value)
	{
		return Append(value);
	}

	ByteBuffer & ByteBuffer::operator<< (const std::string & value)
	{
		return Append(value);
	}

	void ByteBuffer::Shift(int size)
	{
		if (size >= size_)
		{
			size_ = 0;
		}
		else
		{
			::memmove(data_, data_ + size, size_ - size);
			size_ -= size;
		}
	}
}

