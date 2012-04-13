#include "FileUtils.hpp"

#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <vector>

#include "FilePath.hpp"
#include "../UnixError.hpp"

namespace XTL
{
	void FileUtils::Rename(const std::string & oldPath, const std::string & newPath)
	{
		/*
			man 2 rename
			Если newPath уже существует, то он будет атомарно перезаписан (если не возникнет ошибок).
			Нет ни одной точки, когда другой процесс, пытающийся обратиться к newPath, не обнаружит его.
		*/

		if (::rename(oldPath.c_str(), newPath.c_str()) != 0)
		{
			throw UnixError();
		}
	}

	const std::string FileUtils::GetCurrentDirectory()
	{
		static const unsigned int MIN_BUFFER_SIZE = 256;
		std::vector<char> buffer(MIN_BUFFER_SIZE);

		while (::getcwd(&buffer[0], buffer.size()) == 0)
		{
			if (errno == ERANGE)
			{
				buffer.resize(buffer.size() << 1);
			}
			else
			{
				throw UnixError();
			}
		}

		return std::string(&buffer[0], buffer.size());
	}

	const std::string FileUtils::NormalizeFilePath(const std::string & filePath)
	{
		return FilePath(filePath).ToString();
	}

	const std::string FileUtils::AbsoluteFilePath(const std::string & filePath, const std::string & baseDir)
	{
		if (filePath.empty())
		{
			return baseDir;
		}

		if (filePath[0] == '/')
		{
			return NormalizeFilePath(filePath);
		}

		return FilePath(baseDir).Append(filePath).ToString();
	}

	const std::string FileUtils::AbsoluteFilePath(const std::string & filePath)
	{
		return AbsoluteFilePath(filePath, XTL::FileUtils::GetCurrentDirectory());
	}

	const std::string FileUtils::JoinFilePath(const std::string & left, const std::string & right)
	{
		return FilePath(left).Append(right).ToString();
	}

	bool FileUtils::CreateDirectory(const std::string & directoryPath)
	{
		if (::mkdir(directoryPath.c_str(), 0755) == 0)
		{
			return true;
		}

		if (errno == EEXIST)
		{
			return false;
		}

		throw UnixError();
	}
}

