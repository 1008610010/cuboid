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
				catch (const FileLock::Error &)
				{
					throw;
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
				catch (const FileLock::Error &)
				{
					throw;
				}
				catch (const JsonFile::Error &)
				{
					lock_.Unlock();
					throw;
				}
				catch (const JsonParser::Error & e)
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
					file_.Clear();
					lock_.Unlock();
				}
			}

			JsonVariable Root()
			{
				return file_.Root();
			}

		protected:

			JsonFile file_;
			FileLock lock_;
	};
}

int main(int argc, const char * argv[])
{
	try
	{
		XTL::JsonFile config("hit_stats.cfg");

		if (config.Load())
		{
			XTL::JsonConstant counterCfg = config.ConstRoot().Get("counter");
			fprintf(stdout, "table_dir = %s\n", counterCfg.Get("table_dir").AsString().c_str());
			fprintf(stdout, "flush_period = %s\n", counterCfg.Get("flush_period").AsString().c_str());
		}
		else
		{
			fprintf(stderr, "File \"%s\" not found\n", config.FilePath());
		}
	}
	catch (const XTL::JsonFile::Error & e)
	{
		fprintf(stderr, "%s\n", e.What());
	}
	catch (const XTL::JsonParser::Error & e)
	{
		fprintf(stderr, "Parse error (row %d, column %d): %s\n", e.Row() + 1, e.Column() + 1, e.What());
	}
	catch (const XTL::JsonException & e)
	{
		fprintf(stderr, "Config error: %s\n", e.What());
	}

	return 0;

	XTL::JsonDatabase db(".", "testdb");

	try
	{
		if (db.Open(true))
		{
			fprintf(stderr, "Database opened\n");
		}
		else
		{
			db.Create(true);
			fprintf(stderr, "Database created\n");
		}

		XTL::JsonObject obj = db.Root().CreateObject();
		obj["first"].CreateObject();
		obj["second"] = "Abobo and Jimmy";

		fprintf(stderr, "Database closed\n");


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
	catch (const XTL::JsonParser::Error & e)
	{
		fprintf(stderr, "[ERROR] Line %d, col %d: %s\n", e.Row(), e.Column(), e.What());
	}

	return 0;
}

