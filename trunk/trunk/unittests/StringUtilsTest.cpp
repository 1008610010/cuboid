#include "StringUtilsTest.hpp"

#include <string>

#include <xtl/StringUtils.hpp>
#include <xtl/Types.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION (StringUtilsTest);

void StringUtilsTest::StringCastTest()
{
	CPPUNIT_ASSERT_EQUAL(XTL::StringToInteger<int>(0), 0);
	CPPUNIT_ASSERT_EQUAL(XTL::StringToInteger<int>(""), 0);
	CPPUNIT_ASSERT_EQUAL(XTL::StringToInteger<int>(std::string()), 0);

	CPPUNIT_ASSERT_EQUAL(XTL::StringToInteger<XTL::UINT_8 >("255"),                  XTL::TypeTraits<XTL::UINT_8 >::MaxValue);
	CPPUNIT_ASSERT_EQUAL(XTL::StringToInteger<XTL::UINT_16>("65535"),                XTL::TypeTraits<XTL::UINT_16>::MaxValue);
	CPPUNIT_ASSERT_EQUAL(XTL::StringToInteger<XTL::UINT_32>("4294967295"),           XTL::TypeTraits<XTL::UINT_32>::MaxValue);
	CPPUNIT_ASSERT_EQUAL(XTL::StringToInteger<XTL::UINT_64>("18446744073709551615"), XTL::TypeTraits<XTL::UINT_64>::MaxValue);

	CPPUNIT_ASSERT_EQUAL(XTL::StringToInteger<XTL::INT_8 >("-128"),                  XTL::TypeTraits<XTL::INT_8 >::MinValue);
	CPPUNIT_ASSERT_EQUAL(XTL::StringToInteger<XTL::INT_16>("-32768"),                XTL::TypeTraits<XTL::INT_16>::MinValue);
	CPPUNIT_ASSERT_EQUAL(XTL::StringToInteger<XTL::INT_32>("-2147483648"),           XTL::TypeTraits<XTL::INT_32>::MinValue);
	CPPUNIT_ASSERT_EQUAL(XTL::StringToInteger<XTL::INT_64>("-9223372036854775808"),  XTL::TypeTraits<XTL::INT_64>::MinValue);

	CPPUNIT_ASSERT_EQUAL(XTL::StringToInteger<XTL::INT_8 >("127"),                   XTL::TypeTraits<XTL::INT_8 >::MaxValue);
	CPPUNIT_ASSERT_EQUAL(XTL::StringToInteger<XTL::INT_16>("32767"),                 XTL::TypeTraits<XTL::INT_16>::MaxValue);
	CPPUNIT_ASSERT_EQUAL(XTL::StringToInteger<XTL::INT_32>("2147483647"),            XTL::TypeTraits<XTL::INT_32>::MaxValue);
	CPPUNIT_ASSERT_EQUAL(XTL::StringToInteger<XTL::INT_64>("9223372036854775807"),   XTL::TypeTraits<XTL::INT_64>::MaxValue);

/*
	CPPUNIT_ASSERT_EQUAL(XTL::StringToInteger("000000000001024"), 1024ll);
	CPPUNIT_ASSERT_EQUAL(XTL::StringToInteger("2147483647"), 2147483647ll);
	CPPUNIT_ASSERT_EQUAL(XTL::StringToInteger("-2147483648"), -2147483648ll);
	CPPUNIT_ASSERT_EQUAL(XTL::StringToInteger("-123 456"), -123ll);
	CPPUNIT_ASSERT_EQUAL(XTL::StringToInteger("--10"), 0ll);
	CPPUNIT_ASSERT_EQUAL(XTL::StringToInteger("+10"), 0ll);
	*/
}

