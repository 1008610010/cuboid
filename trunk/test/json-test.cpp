#include <xtl/Json.hpp>
#include <xtl/JsonFile.hpp>
#include <xtl/StringUtils.hpp>
#include <xtl/FileLock.hpp>
#include <memory>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

namespace XTL
{
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
				: file_(path + "/" + name + ".json"),
				  lock_(path + "/" + name + ".lock")
			{
				;;
			}

			~JsonDatabase() throw()
			{
				;;
			}

			void Create(bool waitLock = false)
			{
				try
				{
					if (!lock_.Lock(waitLock))
					{
						throw Locked();
					}

					file_.Clear();
				}
				catch (const FileLock::Error & e)
				{
					throw Error(e.What());
				}
			}

			bool Open(bool waitLock = false)
			{
				try
				{
					if (!lock_.Lock(waitLock))
					{
						throw Locked();
					}

					if (!file_.Load())
					{
						lock_.Unlock();
						return false;
					}

					return true;
				}
				catch (const FileLock::Error & e)
				{
					throw Error(e.What());
				}
				catch (const JsonParseError & e)
				{
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
				if (lock_.Locked())
				{
					file_.Save();
					lock_.Unlock();
				}
			}
			
			class JsonVar
			{
				public:

					JsonVar(JsonValue ** value)
						: value_(value) { ;; }

					JsonValue ** CreateObject()
					{
						delete *value_;
						*value_ = new JsonObjectValue();
						return value_;
					}

				protected:

					JsonValue ** value_;
			};

			class JsonObject
			{
				public:

					JsonObject(JsonValue ** value)
						: value_(reinterpret_cast<JsonObjectValue **>(value))
					{
						// TODO: check (*value_)->Type() == OBJECT
					}

					JsonVar operator[] (const char * key)
					{
						return (*value_)->Set(key, 0);
					}

				protected:
				
					JsonObjectValue ** value_;
			};

			JsonVar Root()
			{
				return JsonVar(file_.Root());
			}

		protected:

			JsonFile file_;
			FileLock lock_;
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
		obj["first"].CreateObject();
		obj["second"];

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

