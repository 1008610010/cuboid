#include <xtl/JsonDatabase.hpp>

int main(int argc, const char * argv[])
{
	/*
	XTL::JsonVariable root;
	XTL::JsonObject obj = root.CreateObject();
	XTL::JsonArray arr2 = obj["test"].CreateArray();
	arr2 = arr2.PushBack().CreateArray();

	arr2.PushBack("a");
	arr2.PushBack("b");
	arr2.PushBack("c");
	arr2.PushBack("d");
	arr2.PushBack("e");

	obj["world"] = "of warcraft";


	int i = 0;
	for (XTL::JsonArray::Iterator itr(arr2); !itr.AtEnd(); )
	{
		if (i++ % 2 == 0) ++itr; else itr.Delete();
	}

	root.Print(stderr);
	fprintf(stderr, "\n");

	root.Destroy();
	*/

	try
	{
		XTL::JsonDatabase db("database.json");

		db.Open() || db.Create();

		XTL::JsonVariableRef root = db.Root();

		if (root.IsNull())
		{
			root.CreateObject();
		}
		else if (!root.IsObject())
		{
			fprintf(stderr, "Root is not object\n");
			return 1;
		}

		XTL::JsonObject obj = root.AsObject();

//		obj[""]

		db.Close();

		/*
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
		*/
	}
	catch (...)
	{
		throw;
	}

/*
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
*/

	return 0;

	/*
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
	*/
	return 0;
}

