#include "FileUtils.hpp"

#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <vector>

#include "File.hpp"
#include "FilePath.hpp"
#include "FileStats.hpp"
#include "../../FormatString.hpp"
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

	bool FileUtils::Unlink(const std::string & filePath)
	{
		if (::unlink(filePath.c_str()) == 0)
		{
			return true;
		}

		if (errno == ENOENT)
		{
			return false;
		}
		else
		{
			throw UnixError();
		}
	}

	void FileUtils::CreateSymlink(const std::string & linkPath, const std::string & destination)
	{
		if (::symlink(destination.c_str(), linkPath.c_str()) != 0)
		{
			if (errno == EEXIST)
			{
				throw UnixError::AlreadyExists();
			}
			else
			{
				throw UnixError();
			}
		}
	}

	void FileUtils::RecreateSymlink(const std::string & linkPath, const std::string & destination)
	{
		try
		{
			CreateSymlink(linkPath, destination);
			return;
		}
		catch (const XTL::UnixError::AlreadyExists & e)
		{
			;;
		}

		unsigned int i = 1;
		while (true)
		{
			const std::string tempLinkPath = XTL::FormatString("%s.tmp.%u", linkPath, i);

			try
			{
				CreateSymlink(tempLinkPath, destination);
			}
			catch (const XTL::UnixError::AlreadyExists & e)
			{
				++i;
				continue;
			}

			try
			{
				FileUtils::Rename(tempLinkPath, linkPath);
			}
			catch (const XTL::UnixError & e)
			{
				FileUtils::Unlink(tempLinkPath);
				throw e;
			}

			break;
		}
	}

	const std::string FileUtils::ReadSymlink(const std::string & linkPath)
	{
		static const unsigned int MAX_BUFFER_SIZE = 65536;
		std::vector<char> buffer(1);

		while (true)
		{
			int length = ::readlink(linkPath.c_str(), &buffer[0], buffer.size());
			if (length < 0)
			{
				throw XTL::UnixError();
			}
			else if (length < static_cast<int>(buffer.size()) || buffer.size() >= MAX_BUFFER_SIZE)
			{
				return std::string(&buffer[0], length);
			}
			else
			{
				buffer.resize(buffer.size() << 1);
			}
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

	bool FileUtils::CreateDirectory(const std::string & directoryPath, AccessMode accessMode)
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

	bool FileUtils::CreatePath(const FilePath & filePath, AccessMode accessMode)
	{
		bool created = false;

		for (FilePath::Iterator itr(filePath); !itr.AtEnd(); itr.Advance())
		{
			if (::mkdir(itr.CurrentPath().c_str(), accessMode.Get()) != 0)
			{
				if (errno == EEXIST)
				{
					if (FileStats(itr.CurrentPath(), true).IsDirectory())
					{
						created = false;
						continue;
					}
				}

				throw UnixError();
			}

			created = true;
		}

		return created;
	}

	bool FileUtils::CreatePath(const std::string & dirPath, AccessMode accessMode)
	{
		XTL::FilePath fp(dirPath);
		fp.ConvertToAbsolute();

		return CreatePath(fp, accessMode);
	}

	bool FileUtils::CreatePathForFile(const std::string & filePath, AccessMode accessMode)
	{
		XTL::FilePath fp(filePath);
		fp.ConvertToAbsolute();
		fp.Remove();

		return CreatePath(fp, accessMode);
	}

	void FileUtils::SlurpFile(const std::string & filePath, std::vector<char> & content)
	{
		File file(filePath);
		file.Open(File::OPEN_READ_ONLY);

		FileCloseSentinel sentinel(file);

		const FileSize size = file.Size();
		content.reserve(size + 1); // Reserve one byte for NULL character
		content.resize(size);

		file.Read(&content[0], size);
	}
}
