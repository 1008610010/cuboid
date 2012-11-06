#include "StringSplitterTest.hpp"

#include <xtl/StringUtils.hpp>

#include <string>

#include <stdarg.h>
#include <time.h>

CPPUNIT_TEST_SUITE_REGISTRATION (StringSplitterTest);

bool TestStringSplitter(const char * source, char delimiter, const char * firstArg, ...)
{
	va_list args;
	va_start(args, firstArg);

	XTL::StringSplitter splitter(source, delimiter);
	std::string item;
	bool result = true;
	for (const char * arg = firstArg; arg != NULL; arg = va_arg(args, const char *))
	{
		if (!splitter.GetNext(item))
		{
			result = false;
			break;
		}

		if (item != arg)
		{
			result = false;
			break;
		}
	}
	va_end(args);

	if (splitter.GetNext(item))
	{
		result = false;
	}

	if (!result)
	{
		fprintf(stderr, "StringSplitter error: \"%s\"\n", source);
	}

	return result;
}

void StringSplitterTest::Test1()
{
	CPPUNIT_ASSERT( TestStringSplitter(NULL, ',', NULL) );
	CPPUNIT_ASSERT( TestStringSplitter("", ',', "", NULL) );
	CPPUNIT_ASSERT( TestStringSplitter("||||", '|', "", "", "", "", "", NULL) );
	CPPUNIT_ASSERT( TestStringSplitter("abc,,xyzd,", ',', "abc", "", "xyzd", "", NULL) );
	CPPUNIT_ASSERT( TestStringSplitter(",abc,xyzd,", ',', "", "abc", "xyzd", "", NULL) );
}

