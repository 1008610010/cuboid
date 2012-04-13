#include "FileLock.hpp"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

namespace XTL
{
	bool FileLock::Lock(bool wait)
	{
		if (wait)
		{
			while (!TryLock())
			{
				::sleep(1);
			}
			return true;
		}
		else
		{
			return TryLock();
		}
	}

	bool FileLock::Unlock()
	{
		if (::unlink(filePath_.c_str()) == -1)
		{
			if (errno == ENOENT)
			{
				locked_ = false;
				return false;
			}
			throw Error(std::string("Unable to remove file \"") + filePath_ + "\": " + ::strerror(errno));
		}
		locked_ = false;
		return true;
	}

	bool FileLock::TryLock()
	{
		int fd = ::open(filePath_.c_str(), O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
		if (fd == -1)
		{
			if (errno == EEXIST)
			{
				return false;
			}
			else
			{
				throw Error(std::string("Unable to create file \"") + filePath_ + "\": " + ::strerror(errno));
			}
		}
		else
		{
			::close(fd);
			locked_ = true;
			return true;
		}
	}
}
