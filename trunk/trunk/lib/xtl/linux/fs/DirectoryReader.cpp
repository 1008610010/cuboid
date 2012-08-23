#include "DirectoryReader.hpp"

#include <fnmatch.h>

#include "../UnixError.hpp"

namespace XTL
{
	DirectoryReader::DirectoryReader(const std::string & directory)
		: directory_(directory),
		  pattern_(),
		  dir_(0),
		  entry_()
	{
		Init();
	}

	DirectoryReader::DirectoryReader(const std::string & directory, const std::string & pattern)
		: directory_(directory),
		  pattern_(pattern),
		  dir_(0),
		  entry_()
	{
		Init();
	}

	void DirectoryReader::Init()
	{
		dir_ = ::opendir(directory_.c_str());

		if (dir_ == 0)
		{
			throw XTL::UnixError();
		}

		GetNext();
	}

	DirectoryReader::~DirectoryReader() throw()
	{
		if (dir_ == 0)
		{
			return;
		}

		if (::closedir(dir_) < 0)
		{
			dir_ = 0;
			// throw XTL::UnixError();
		}
	}

	void DirectoryReader::GetNext()
	{
		if (entry_.IsNull())
		{
			return;
		}

		while (true)
		{
			int result = ::readdir_r(dir_, entry_.EntryPtr(), entry_.ResultPtr());
			if (result != 0)
			{
				throw UnixError(result);
			}

			if (entry_.IsNull() || pattern_.empty())
			{
				break;
			}

			// printf(">>> %s\n", entry_.Name());

			if (::fnmatch(pattern_.c_str(), entry_.Name(), FNM_PATHNAME) == 0)
			{
				break;
			}
		}
	}
}

