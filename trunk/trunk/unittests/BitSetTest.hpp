#ifndef XTL__BIT_SET_TEST_HPP__
#define XTL__BIT_SET_TEST_HPP__ 1

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class BitSetTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(BitSetTest);
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

