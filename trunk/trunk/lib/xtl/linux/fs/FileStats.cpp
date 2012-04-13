#include "FileStats.hpp"

#include "../UnixError.hpp"

namespace XTL
{
	const FileType FileType::NOT_EXISTS (0);
	const FileType FileType::REGULAR    (1);
	const FileType FileType::DIRECTORY  (2);
	const FileType FileType::LINK       (3);
	const FileType FileType::UNKNOWN    (4);

	FileType FileType::FromStats(const struct ::stat64 & stats)
	{
		mode_t m = stats.st_mode;
		if (S_ISREG(m))
		{
			return REGULAR;
		}
		else if (S_ISDIR(m))
		{
			return DIRECTORY;
		}
		else if (S_ISLNK(m))
		{
			return LINK;
		}
		else
		{
			return UNKNOWN;
		}
	}

	FileStats::FileStats()
		: stats_()
	{
		;;
	}

	FileStats::FileStats(const std::string & fileName)
	{
		Init(fileName);
	}

	FileStats::FileStats(int fd)
	{
		Init(fd);
	}

	void FileStats::Init(const std::string & fileName)
	{
		if (fileName.empty())
		{
			throw ILLEGAL_ARGUMENT_ERROR("fileName");
		}

		if (::lstat64(fileName.c_str(), &stats_) != 0)
		{
			throw UnixError();
		}
	}

	void FileStats::Init(int fd)
	{
		if (fd < 0)
		{
			throw ILLEGAL_ARGUMENT_ERROR("fd");
		}

		if (::fstat64(fd, &stats_) != 0)
		{
			throw UnixError();
		}
	}

	FileSize FileStats::Size() const
	{
		return stats_.st_size;
	}

	const FileType FileStats::Type() const
	{
		return FileType::FromStats(stats_);
	}

	bool FileStats::IsRegular() const
	{
		return S_ISREG(stats_.st_mode);
	}

	bool FileStats::IsDirectory() const
	{
		return S_ISDIR(stats_.st_mode);
	}

	const FileType FileStats::Type(const std::string & filePath)
	{
		struct ::stat64 stats;
		if (::lstat64(filePath.c_str(), &stats) != 0)
		{
			if (errno == ENOENT || errno == ENOTDIR)
			{
				return FileType::NOT_EXISTS;
			}
			else
			{
				throw UnixError();
			}
		}

		return FileType::FromStats(stats);
	}
}

