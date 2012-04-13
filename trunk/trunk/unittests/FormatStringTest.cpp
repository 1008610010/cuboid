#include "FormatStringTest.hpp"

#include <xtl/FormatString.hpp>

#include <string>

#include <time.h>

CPPUNIT_TEST_SUITE_REGISTRATION (FormatStringTest);

void FormatStringTest::Test1()
{
	{
		CPPUNIT_ASSERT_EQUAL( std::string("0"),        XTL::FormatString("%d", 0) );
		CPPUNIT_ASSERT_EQUAL( std::string("01"),       XTL::FormatString("%d%d", 0, 1) );
		CPPUNIT_ASSERT_EQUAL( std::string("012"),      XTL::FormatString("%d%d%d", 0, 1, 2) );
		CPPUNIT_ASSERT_EQUAL( std::string("0123"),     XTL::FormatString("%d%d%d%d", 0, 1, 2, 3) );
		CPPUNIT_ASSERT_EQUAL( std::string("01234"),    XTL::FormatString("%d%d%d%d%d", 0, 1, 2, 3, 4) );
		CPPUNIT_ASSERT_EQUAL( std::string("012345"),   XTL::FormatString("%d%d%d%d%d%d", 0, 1, 2, 3, 4, 5) );
		CPPUNIT_ASSERT_EQUAL( std::string("0123456"),  XTL::FormatString("%d%d%d%d%d%d%d", 0, 1, 2, 3, 4, 5, 6) );
		CPPUNIT_ASSERT_EQUAL( std::string("01234567"), XTL::FormatString("%d%d%d%d%d%d%d%d", 0, 1, 2, 3, 4, 5, 6, 7) );

		CPPUNIT_ASSERT_EQUAL( std::string("00001024"), XTL::FormatString("%08u", 1024u) );
		CPPUNIT_ASSERT_EQUAL( std::string("003.1416"), XTL::FormatString("%08.4f", 3.1415926535) );

		CPPUNIT_ASSERT_EQUAL( std::string(""), XTL::FormatString( "%s", std::string()) );
		CPPUNIT_ASSERT_EQUAL( std::string(""), XTL::FormatString( "%s", "") );
		CPPUNIT_ASSERT_EQUAL( std::string("XTL Format String"), XTL::FormatString("%s %s %s", std::string("XTL"), "Format", std::string("String")) );

		std::string s("%c%s");
		for (unsigned int i = 0; i < 25; ++i)
		{
			s = XTL::FormatString(s.c_str(), 'A' + i, "%c%s");
		}
		s = XTL::FormatString(s.c_str(), 'A' + 25, "");

		CPPUNIT_ASSERT_EQUAL( std::string("ABCDEFGHIJKLMNOPQRSTUVWXYZ"), s );
	}
}

