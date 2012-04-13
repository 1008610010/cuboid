#include "File.hpp"
#include <sup/Types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/file.h>
#include <limits.h>

namespace STXX
{
	//const short int File::eintrIgnore_ = 5;
	File::File()
		: desc_(-1),
		  status_(0),
		  size_(0),
		  position_(0)
	{
		;;
	}

	File::~File() throw()
	{
		Close();
	}

	bool File::Opened() const
	{
		return desc_ != -1;
	}

	void File::Close()
	{
		if (Opened())
		{
			Flush();
			if (HasStatus(STATUS_LOCKED))
			{
				Unlock();
			}
			close(desc_);
			desc_ = -1;
			status_ = 0;
			size_ = 0;
			position_ = 0;
		}
	}

	bool File::Open(const CHAR_8 * fileName, UINT_32 mode)
	{
		Close();

		if (fileName == NULL)
		{
			return false;
		}

		if (mode == OPEN_OR_CREATE)
		{
			status_ = STATUS_CAN_READ | STATUS_CAN_WRITE;
			desc_ = open(fileName, O_CREAT | O_RDWR | O_LARGEFILE, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		}
		else if (mode == OPEN_READ_WRITE)
		{
			status_ = STATUS_CAN_READ | STATUS_CAN_WRITE;
			desc_ = open(fileName, O_RDWR | O_LARGEFILE);
		}
		else if (mode == OPEN_READ)
		{
			status_ = STATUS_CAN_READ;
			desc_ = open(fileName, O_RDONLY | O_LARGEFILE);
		}
		else
		{
			return false;
		}

		struct stat64 s;
		if (desc_ < 0 || fstat64(desc_, &s) != 0)
		{
			status_ = 0;
			desc_ = -1;
			return false;
		}

		size_ = s.st_size;
		position_ = 0;
		return true;
	}

	bool File::Create(const CHAR_8 * fileName)
	{
		Close();

		if (fileName == NULL)
		{
			return false;
		}

		desc_ = open(fileName, O_CREAT | O_TRUNC | O_RDWR | O_LARGEFILE, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		if (desc_ < 0)
		{
			return false;
		}

		status_ = STATUS_CAN_READ | STATUS_CAN_WRITE;
		size_ = 0;
		position_ = 0;
		return true;
	}

	bool File::Lock(bool exclusive, bool wait)
	{
		if (desc_ < 0)
		{
			return false;
		}

		struct flock lock;
		lock.l_type = (exclusive ? F_WRLCK : F_RDLCK);
		lock.l_whence = SEEK_SET;
		lock.l_start = 0;
		lock.l_len = 0;
		if (fcntl(desc_, (wait ? F_SETLKW : F_SETLK), &lock) < 0)
		{
			return false;
		}
		status_ |= STATUS_LOCKED;
		return true;
	}

	bool File::Unlock()
	{
		if (desc_ < 0)
		{
			return false;
		}

		struct flock lock;
		lock.l_type = F_UNLCK;
		lock.l_whence = SEEK_SET;
		lock.l_start = 0;
		lock.l_len = 0;
		if (fcntl(desc_, F_SETLK, &lock) < 0)
		{
			return false;
		}
		status_ &= ~STATUS_LOCKED;
		return true;
	}

	bool File::Seek(UINT_64 offset, int whence)
	{
		if (desc_ < 0)
		{
			return false;
		}
		const INT_64 seekRes = ::lseek64(desc_, offset, whence);
		switch (whence)
		{
			case SEEK_SET:
				if (seekRes != (INT_64) offset)
				{
					return false;
				}
				position_ = offset;
				break;

			case SEEK_CUR:
				if (seekRes != (INT_64) position_ + (INT_64) offset)
				{
					return false;
				}
				position_ += offset;
				break;

			default:
				if (-1 == seekRes) { return false; }
		}

		return true;
	}

	bool File::Truncate(UINT_64 size)
	{
		if (ftruncate64(desc_, size) != 0)
		{
			return false;
		}
		size_ = size;
		if (position_ > size_)
		{
			return Seek(size_);
		}
		return true;
	}

	bool File::SetSize(UINT_64 size)
	{
		if (size < size_)
		{
			return Truncate(size);
		}
		else
		{
			Seek(size_);
			static const UINT_32 BITS = 16;
			static const UINT_32 SIZE = 1 << BITS;
			static CHAR_8 BUFFER[SIZE];
			UINT_64 delta = size - size_;
			for (UINT_32 i = 0; i < (delta >> BITS); ++i)
			{
				if (!WriteBuffer(BUFFER, SIZE))
				{
					return false;
				}
			}
			if ((delta & (SIZE - 1)) != 0)
			{
				return WriteBuffer(BUFFER, delta & (SIZE - 1));
			}
			return true;
		}
	}

	bool File::ReadBuffer(void * buffer, UINT_32 size)
	{
		if (!HasStatus(STATUS_CAN_READ))
		{
			return false;
		}
		if ((UINT_64) SignalSafeRead(buffer, size) != size)
		{
			return false;
		}
		else
		{
			position_ += size;
			if (size_ < position_) { size_ = position_; }
			return true;
		}
	}

	bool File::WriteBuffer(const void * buffer, UINT_32 size)
	{
		if (!HasStatus(STATUS_CAN_WRITE))
		{
			return false;
		}
		if ((UINT_32) SignalSafeWrite(buffer, size) != size)
		{
			return false;
		}
		else
		{
			position_ += size;
			if (size_ < position_)
			{
				size_ = position_;
			}
			return true;
		}
	}

	ssize_t File::SignalSafeRead(void *ptr, size_t size)
	{
		if (-1 == desc_) { return -1; }
		char * data = static_cast<char*>(ptr);
		size_t rdn(0);

		short int eintrIgnore(eintrIgnore_);
		for ( ; 0 != size ; )
		{
			ssize_t res (0);
			if (size > SSIZE_MAX)
			{
				res = ::read(desc_, data, SSIZE_MAX);
			}
			else
			{
				res = ::read(desc_, data, size);
			}
			
			const bool eof (0 == res);
			if (eof) { return rdn; }

			if (res < 0)
			{
				if (EINTR != errno || 0 == --eintrIgnore)
				{
					return -1;
				}
			}
			else
			{
				rdn  += res;
				data += res;
				size -= res;
			}
		}
		return rdn;
	}

	ssize_t File::SignalSafeWrite(const void *ptr, size_t size)
	{
		if (-1 == desc_) { return -1; }
		const char * data = static_cast<const char*>(ptr);
		size_t wrt(0);

		short int eintrIgnore(eintrIgnore_);
		for ( ; 0 != size ; )
		{
			ssize_t res = ::write(desc_, data, size);
			if (res < 0)
			{
				if (EINTR != errno || 0 == --eintrIgnore)
				{
					return -1;
				}
			}
			else
			{
				wrt  += res;
				data += res;
				size -= res;
			}
		}
		return wrt;
	}

	bool File::Flush()
	{
		if (-1 == desc_) { return false; }

		for (short int i = 0; i <= eintrIgnore_ || eintrIgnore_ < 0 ; i++)
		{
			if (0 == ::fsync(desc_)) { return true; }
			else if (errno != EINTR) { return false; }
		}
		return false;

	}

	bool File::Exists(const CHAR_8 * fileName)
	{
		struct stat64 s;
		if (stat64(fileName, &s) != 0)
		{
			if (errno == ENOENT)
			{
				return false;
			}
		}
		return true;
	}

	bool File::Remove(const CHAR_8 * fileName)
	{
		return ::unlink(fileName) == 0;
	}

	UINT_64 File::GetSize(const CHAR_8 * fileName)
	{
		struct stat64 s;
		return ::stat64(fileName, &s) == 0 ? s.st_size : 0;
	}

	time_t File::GetModifiedTime(const CHAR_8 * fileName)
	{
		struct stat64 s;
		return ::stat64(fileName, &s) == 0 ? s.st_mtime : 0;
	}

	bool File::Rename(const CHAR_8 * oldName, const CHAR_8 * newName)
	{
		return ::rename(oldName, newName) == 0;
	}
}
