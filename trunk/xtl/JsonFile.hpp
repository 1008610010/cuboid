#ifndef _XTL__JSON_FILE_HPP__
#define _XTL__JSON_FILE_HPP__ 1

#include "Json.hpp"
#include "StringUtils.hpp"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

namespace XTL
{
	class JsonFile
	{
		public:

			class Error
			{
				public:
					explicit Error(const char * what) : what_(what) { ;; }
					explicit Error(const std::string & what) : what_(what) { ;; }
					const char * What() const { return what_.c_str(); }
				protected:
					const std::string what_;
			};

			JsonFile(const std::string & filePath)
				: filePath_(filePath),
				  root_(0)
			{
				;;
			}

			~JsonFile() throw()
			{
				delete root_;
			}
			
			void Clear()
			{
				delete root_;
				root_ = 0;
			}

			bool Load()
			{
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

					char * buffer = static_cast<char *>(::malloc(fileSize + 1));
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
				catch (const JsonParseError & e)
				{
					::free(buffer);
					throw Error(
						FormatString(
							"JSON file corrupted (row %d, column %d): %s",
							e.Row() + 1,
							e.Column() + 1,
							e.What()
						)
					);
				}
			}

			void Save()
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
					FILE * file = ::fopen(filePath_.c_str(), "w");
					if (file == 0)
					{
						throw Error(
							FormatString(
								"Unable to open file \"%s\" for writing: %s",
								filePath_.c_str(),
								::strerror(errno)
							)
						);
					}

					root_->Print(file, 0, 0);

					::fclose(file);
					delete root_;
					root_ = 0;
				}
			}
			
			JsonValue ** Root()
			{
				return &root_;
			}

		protected:

			const std::string   filePath_;
			JsonValue         * root_;
	};
}

#endif

