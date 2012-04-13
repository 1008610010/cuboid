#ifndef XTL__SIMPLE_TEMPLATE_TEST_HPP__
#define XTL__SIMPLE_TEMPLATE_TEST_HPP__ 1

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class SimpleTemplateTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(SimpleTemplateTest);
	CPPUNIT_TEST(TemplateParsingTest);
	CPPUNIT_TEST_SUITE_END();

	public:

		virtual void setUp()
		{
			;;
		}

		virtual void tearDown()
		{
			;;
		}

	protected:

		void TemplateParsingTest();
};

#endif

