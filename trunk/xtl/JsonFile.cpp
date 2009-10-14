#include "JsonFile.hpp"
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "StringUtils.hpp"

namespace XTL
{
	JsonFile::JsonFile(const std::string & filePath)
		: filePath_(filePath),
		  root_(0)
	{
		;;
	}

	JsonFile::~JsonFile() throw()
	{
		delete root_;
	}

	void JsonFile::Clear()
	{
		delete root_;
		root_ = 0;
	}

	bool JsonFile::Load()
	{
		Clear();

		FILE * file = 0;
		char * buffer = 0;
		try
		{
			file = fopen(filePath_.c_str(), "r");
			if (file == 0)
			{
				if (errno == ENOENT)
				{
					return false;
				}

				throw Error(
					FormatString(
						"Unable to open file \"%s\" for reading: %s",
						filePath_.c_str(),
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
						filePath_.c_str()
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
						"Unable to read file \"%s\": %s",
						filePath_.c_str(),
						::strerror(errno)
					)
				);
			}
			buffer[fileSize] = '\0';

			::fclose(file);
			
			JsonParser parser(buffer);

			::free(buffer);
			
			root_ = parser.Release();

			return true;
		}
		catch (const JsonParser::Error & e)
		{
			::free(buffer);
			throw;
		}
	}

	void JsonFile::Save()
	{
		if (root_ == 0)
		{
			if (::unlink(filePath_.c_str()) == -1 && errno != ENOENT)
			{
				throw Error(
					FormatString(
						"Unable to remove file \"%s\": %s",
						filePath_.c_str(),
						::strerror(errno)
					)
				);
			}
		}
		else
		{
			const std::string tempFilePath = TempFileName();

			FILE * file = ::fopen(tempFilePath.c_str(), "w");
			if (file == 0)
			{
				throw Error(
					FormatString(
						"Unable to open temporary file \"%s\" for writing: %s",
						tempFilePath.c_str(),
						::strerror(errno)
					)
				);
			}

			root_->Print(file, 0, 0);

			if (::fclose(file) != 0)
			{
				throw Error(
					FormatString(
						"Unable to close temporary file \"%s\": %s",
						tempFilePath.c_str(),
						::strerror(errno)
					)
				);
			}

			if (::rename(tempFilePath.c_str(), filePath_.c_str()) != 0)
			{
				throw Error(
					FormatString(
						"Unable to rename temporary file \"%s\" to \"%s\": %s",
						tempFilePath.c_str(),
						filePath_.c_str(),
						::strerror(errno)
					)
				);
			}
		}
	}

	const std::string JsonFile::TempFileName() const
	{
		return filePath_ + ".tmp";
	}
}

