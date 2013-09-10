#include <stdio.h>
#include <xtl/sqlite/Database.hpp>

int main(int argc, const char * argv[])
{
	try
	{
		XTL::SQLITE::Database dbc("test.sqlite");
		dbc.Open();
		dbc.Execute("CREATE TABLE IF NOT EXISTS properties ( key TEXT NOT NULL PRIMARY KEY, value TEXT NOT NULL )");
		XTL::SQLITE::Statement statement(dbc.Prepare("INSERT INTO properties (key, value) VALUES (?, ?)"));
		statement.Bind(1, "x");
		statement.Bind(2, "1");
		statement.Execute();
		statement.Reset();
		dbc.Close();
	}
	catch (const XTL::SQLITE::Exception & e)
	{
		fprintf(stderr, "SQLite Error: %s\n", e.Message().c_str());
	}
}

