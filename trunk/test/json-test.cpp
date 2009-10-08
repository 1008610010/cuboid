#include <xtl/Json.hpp>
#include <memory>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <xtl/FileLock.hpp>

namespace XTL
{
	bool FormatString(std::string & s, const char * format, ...)
	{
		va_list ap;

		int size = 128;
		char * buffer = static_cast<char *>(::malloc(size));
		if (buffer == 0)
		{
			return false;
		}

		while (true)
		{
			va_start(ap, format);
			int result = ::vsnprintf(buffer, size, format, ap);
			va_end(ap);

			if (result >= 0)
			{
				if (result < size)
				{
					break;
				}
				else
				{
					size = result + 1;
				}
			}
			else
			{
				size <<= 1;
			}

			char * newBuffer = static_cast<char *>(::realloc(buffer, size));
			if (newBuffer == 0)
			{
				::free(buffer);
				return false;
			}

			buffer = newBuffer;
		}

		s.assign(buffer);
		::free(buffer);
		return true;
	}

	const std::string FormatString(const char * format, ...)
	{
		va_list ap;

		int size = 128;
		char * buffer = static_cast<char *>(::malloc(size));
		if (buffer == 0)
		{
			return std::string();
		}

		while (true)
		{
			va_start(ap, format);
			int result = ::vsnprintf(buffer, size, format, ap);
			va_end(ap);

			if (result >= 0)
			{
				if (result < size)
				{
					break;
				}
				else
				{
					size = result + 1;
				}
			}
			else
			{
				size <<= 1;
			}

			char * newBuffer = static_cast<char *>(::realloc(buffer, size));
			if (newBuffer == 0)
			{
				::free(buffer);
				return std::string();
			}

			buffer = newBuffer;
		}

		std::string s(buffer);
		::free(buffer);
		return s;
	}

	class JsonDatabase
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

			class Locked { };

			JsonDatabase(const std::string & path, const std::string & name)
				: filePath_(path + "/" + name + ".json"),
				  lock_(path + "/" + name + ".lock"),
				  root_(0)
			{
				;;
			}

			~JsonDatabase() throw()
			{
				delete root_;
			}

			void Create(bool waitLock = false)
			{
				try
				{
					if (!lock_.Lock(waitLock))
					{
						throw Locked();
					}

					root_ = 0;
				}
				catch (const FileLock::Error & e)
				{
					throw Error(e.What());
				}
			}

			bool Open(bool waitLock = false)
			{
				FILE * file = 0;
				char * buffer = 0;
				try
				{
					if (!lock_.Lock(waitLock))
					{
						throw Locked();
					}

					file = fopen(filePath_.c_str(), "r");
					if (file == 0)
					{
						lock_.Unlock();

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
						lock_.Unlock();
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
						lock_.Unlock();
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
				catch (const FileLock::Error & e)
				{
					throw Error(e.What());
				}
				catch (const JsonParseError & e)
				{
					::free(buffer);
					lock_.Unlock();
					throw Error(
						FormatString(
							"Database file corrupted (row %d, column %d): %s",
							e.Row() + 1,
							e.Column() + 1,
							e.What()
						)
					);
				}
			}

			void Close()
			{
				if (!lock_.Locked())
				{
					return;
				}

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

				lock_.Unlock();
			}

			class JsonVar
			{
				public:

					JsonVar(JsonValue ** value)
						: value_(value) { ;; }

				protected:

					JsonValue ** value_;
			};

			template <typename ValueType>
			class JsonVarBase
			{
				public:

					JsonVarBase(JsonValue ** value)
						: value_(reinterpret_cast<ValueType **>(value)) { ;; }

				protected:

					ValueType ** value_;
			};

			class JsonObject : public JsonVarBase<JsonObjectValue>
			{
				public:

					JsonObject(JsonValue ** value)
						: JsonVarBase<JsonObjectValue>(value) { ;; }

					JsonVar operator[] (const char * key)
					{
						return JsonVar((*value_)->Set(key, 0));
					}

				protected:
			};

			class Node
			{
				public:

					Node(JsonValue ** value) : value_(value) { ;; }

					JsonVarBase<JsonObjectValue> CreateObject()
					{
						delete *value_;
						JsonVarBase<JsonObjectValue> var(value_);
						*value_ = new JsonObjectValue();
						return var;
					}

				protected:

					JsonValue ** value_;
			};

			JsonVar Root()
			{
				return JsonVar(&root_);
			}

		protected:

			const std::string   filePath_;
			FileLock            lock_;
			JsonValue         * root_;
	};
}

int main(int argc, const char * argv[])
{
	XTL::JsonDatabase db(".", "testdb");

	try
	{
		if (!db.Open())
		{
			db.Create();
		}

		XTL::JsonDatabase::JsonObject obj = db.Root().CreateObject();

		db.Close();
	}
	catch (const XTL::JsonDatabase::Locked)
	{
		fprintf(stderr, "Database is locked\n");
	}
	catch (const XTL::JsonDatabase::Error & e)
	{
		fprintf(stderr, "%s\n", e.What());
	}

	return 0;

	printf("JSON test.\n");

	const char * source =
	" { \n"
	"\"a\" : [{}] , \"x\\\"y\\\"z\" : -1024e+3, \"obj\":{\"x\":1,\"y\":false, \"z\":null}, \"\\\\fucky\" : 123456.789000000000000000000000000000000, \"flags\" : [[true], [true, false], [   [   null   ,  [  null  ] , null ] ,  null  ] , null ]   \n"
	" }";

	fprintf(stderr, "%s\n", source);

	try
	{
		XTL::JsonParser parser(source);
		std::auto_ptr<XTL::JsonValue> root(parser.Release());
		root->PrintPlain(stdout);
		fprintf(stdout, "\n");
		root->Print(stdout, 0, false);
	}
	catch (const XTL::JsonParseError & error)
	{
		fprintf(stderr, "[ERROR] Line %d, col %d: %s\n", error.Row(), error.Column(), error.What());
	}

	return 0;
}

