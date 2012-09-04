#ifndef XTL__BASE_64_TEST_HPP__
#define XTL__BASE_64_TEST_HPP__ 1

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class Base64Test : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(Base64Test);
	CPPUNIT_TEST(Test1);
	CPPUNIT_TEST_SUITE_END();

	public:

		Base64Test();

		void setUp();

		void tearDown() { ;; }

	protected:

		void Test1();

	private:

		void TestAllowedChars(const std::string & encoded) const;

		void TestAllowedCharsUrl(const std::string & encoded) const;

		void TestEncode(const std::string & source, const std::string & encoded) const;

		void TestDecode(const std::string & encoded, const std::string & source) const;

		void TestEncodeDecode(const std::string & source) const;

		void TestEncodeDecodeUrl(const std::string & source) const;

		std::set<char> allowedChars_;
		std::set<char> allowedCharsUrl_;
};

#endif

