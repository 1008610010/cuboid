#include "InitConfig.hpp"
#include "StringUtils.hpp"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

namespace XTL
{
	InitConfig::InitConfig()
		: sections_()
	{
		;;
	}

	InitConfig::~InitConfig() throw()
	{
		Clear();
	}

	void InitConfig::Clear()
	{
		for (SectionsMap::const_iterator itr = sections_.begin(); itr != sections_.end(); ++itr)
		{
			delete itr->second;
		}
		sections_.clear();
	}

	InitConfig & InitConfig::Set(const std::string & section, const std::string & key, const long long & value)
	{
		CreateSection(section)->Set(key, value);
		return *this;
	}

	InitConfig & InitConfig::Set(const std::string & section, const std::string & key, const double & value)
	{
		CreateSection(section)->Set(key, value);
		return *this;
	}

	InitConfig & InitConfig::Set(const std::string & section, const std::string & key, const std::string & value)
	{
		CreateSection(section)->Set(key, value);
		return *this;
	}

	const InitConfigSection * InitConfig::GetSection(const std::string & name) const
	{
		SectionsMap::const_iterator itr = sections_.find(name);
		return itr == sections_.end() ? 0 : itr->second;
	}

	InitConfigSection * InitConfig::CreateSection(const std::string & name)
	{
		SectionsMap::iterator itr = sections_.find(name);
		if (itr != sections_.end())
		{
			return itr->second;
		}
		else
		{
			InitConfigSection * section = new InitConfigSection(name);
			sections_.insert(std::make_pair(name, section));
			return section;
		}
	}

	bool InitConfig::Load(const std::string & filePath)
	{
		Clear();

		FILE * file = 0;
		char * buffer = 0;
		try
		{
			file = fopen(filePath.c_str(), "r");
			if (file == 0)
			{
				if (errno == ENOENT)
				{
					return false;
				}

				throw Error(
					FormatString(
						"Unable to open file \"%s\" for reading: %s",
						filePath.c_str(),
						::strerror(errno)
					)
				);
			}

			::fseek(file, 0, SEEK_END);
			long fileSize = ::ftell(file);
			::fseek(file, 0, SEEK_SET);

			buffer = static_cast<char *>(::malloc(fileSize + 1));
			if (buffer == 0)
			{
				::fclose(file);
				throw Error(
					FormatString(
						"Unable to allocate memory to read file \"%s\"",
						filePath.c_str()
					)
				);
			}

			long fileRead = ::fread(buffer, 1, fileSize, file);
			if (fileRead != fileSize)
			{
				::free(buffer);
				::fclose(file);
				throw Error(
					FormatString(
						"Unable to read whole file \"%s\": %s",
						filePath.c_str(),
						::strerror(errno)
					)
				);
			}
			buffer[fileSize] = '\0';

			::fclose(file);

			InitConfigParser parser(buffer, *this);
			::free(buffer);

			return true;
		}
		catch (const InitConfigParser::Error & e)
		{
			::free(buffer);
			throw;
		}
	}

}
