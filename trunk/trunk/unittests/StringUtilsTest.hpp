#ifndef XTL__STRING_UTILS_TEST_HPP__
#define XTL__STRING_UTILS_TEST_HPP__ 1

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class StringUtilsTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(StringUtilsTest);
	CPPUNIT_TEST(StringCastTest);
	CPPUNIT_TEST(CheckIsNumberTest);
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

		void StringCastTest();

		void CheckIsNumberTest();
};

#endif

