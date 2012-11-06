#ifndef XTL__STRING_SPLITTER_TEST_HPP__
#define XTL__STRING_SPLITTER_TEST_HPP__ 1

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class StringSplitterTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(StringSplitterTest);
	CPPUNIT_TEST(Test1);
	CPPUNIT_TEST_SUITE_END();

	public:

		void setUp()
		{
			;;
		}

		void tearDown()
		{
			;;
		}

	protected:

		void Test1();
};

#endif

