#include "DirectoryReaderRecursive.hpp"

#include <algorithm>
#include <vector>

#include "DirectoryReader.hpp"
#include "FileStats.hpp"

namespace XTL
{
	DirectoryReaderRecursive::DirectoryReaderRecursive(Listener & listener)
		: listener_(listener)
	{
		;;
	}

	void DirectoryReaderRecursive::Read(const std::string & baseDir)
	{
		ProcessDirectory(baseDir);
	}

	void DirectoryReaderRecursive::ProcessDirectory(const std::string & baseDir)
	{
		std::vector<std::string> files;
		std::vector<std::string> directories;

		try
		{
			XTL::DirectoryReader directoryReader(baseDir);
			while (directoryReader.Read())
			{
				const std::string fileName = directoryReader.Current().Name();

				if (fileName == "." || fileName == "..")
				{
					continue;
				}

				const XTL::FileStats fileStats(baseDir + "/" + fileName, true);

				if (fileStats.IsDirectory())
				{
					if (listener_.IsDirectoryAllowed(baseDir, fileName))
					{
						directories.push_back(fileName);
					}
				}
				else if (fileStats.IsRegular())
				{
					files.push_back(fileName);
				}

				/*
				if (reader->Current().IsDirectory())
				{
					if (listener_.IsDirectoryAllowed(baseDir, fileName))
					{
						directories.push_back(fileName);
					}
				}
				else if (reader->Current().IsRegular1())
				{
					files.push_back(fileName);
				}
				*/
			}
		}
		catch (const XTL::UnixError & e)
		{
			listener_.OnDirectoryError(baseDir, e);
			return;
		}

		std::sort(directories.begin(), directories.end());

		for (unsigned int i = 0; i < directories.size(); ++i)
		{
			listener_.OnDirectory(baseDir, directories[i]);
			ProcessDirectory(baseDir + "/" + directories[i]);
		}

		std::sort(files.begin(), files.end());

		for (unsigned int i = 0; i < files.size(); ++i)
		{
			listener_.OnRegularFile(baseDir, files[i]);
		}
	}

	void ReadDirectoryRecursive(const std::string & baseDir, DirectoryReaderRecursive::Listener & listener)
	{
		DirectoryReaderRecursive(listener).Read(baseDir);
	}
}
