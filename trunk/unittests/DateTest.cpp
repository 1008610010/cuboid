#include "DateTest.hpp"

#include <xtl/utils/Date.hpp>
#include <xtl/FormatString.hpp>

#include <string>

#include <time.h>

CPPUNIT_TEST_SUITE_REGISTRATION (DateTest);

const std::string DateToString(const XTL::Date & date)
{
	return XTL::FormatString("%04d-%02d-%02d", date.Year(), date.Month(), date.Day());
}

std::ostream & operator<< (std::ostream & os, const XTL::Date & date)
{
	os << DateToString(date);
	return os;
}

void DateTest::Test1()
{
	{
		CPPUNIT_ASSERT(XTL::Date::IsValid(2012, 1, 31));
		CPPUNIT_ASSERT(XTL::Date::IsValid(2012, 2, 29));
		CPPUNIT_ASSERT(XTL::Date::IsValid(2000, 2, 29));
		CPPUNIT_ASSERT(!XTL::Date::IsValid(2100, 2, 29));
	}

	{
		static const int N = 120000;

		XTL::Date d1(1, 1, 1);
		XTL::Date d2(1, 1, 1);
		for (int i = 0; i < N; ++i)
		{
			d1.IncMonth();
			d2.AddDays(d2.DaysInMonth());
			CPPUNIT_ASSERT_EQUAL(d1, d2);
		}

		for (int i = 0; i < N; ++i)
		{
			d1.DecMonth();
			d2.DecDay();
			d2.SubDays(d2.DaysInMonth() - 1);
			CPPUNIT_ASSERT_EQUAL(d1, d2);
		}
	}

	{
		XTL::Date d1(2000, 2, 29);
		d1.IncYear();
		CPPUNIT_ASSERT_EQUAL(d1, XTL::Date(2001, 2, 28));

		XTL::Date d2(d1);
		d2.IncDay();
		CPPUNIT_ASSERT_EQUAL(d2 - d1, 1);
		CPPUNIT_ASSERT_EQUAL(d1 - d2, -1);
	}

	{
		static const int N = 1000000;

		XTL::Date d1(1, 1, 1);
		for (int i = 0; i < N; ++i)
		{
			XTL::Date d2(i);
			CPPUNIT_ASSERT_EQUAL(d2.ToDayIndex(), i);
			CPPUNIT_ASSERT_EQUAL(d1, d2);
			d1.IncDay();
		}

		for (int i = N; i > 0; --i)
		{
			XTL::Date d2(i);
			CPPUNIT_ASSERT_EQUAL(d2.ToDayIndex(), i);
			CPPUNIT_ASSERT_EQUAL(d1, d2);
			d1.DecDay();
		}
	}

	{
		static const int N = 10000;

		int di = 0;
		XTL::Date d1(1, 1, 1);
		for (int i = 1; i <= N; ++i)
		{
			XTL::Date d2(di);
			CPPUNIT_ASSERT_EQUAL(d1, d2);
			CPPUNIT_ASSERT_EQUAL(d2.ToDayIndex(), di);
			di += i;
			d1.AddDays(i);
		}

		for (int i = 1; i <= N; ++i)
		{
			XTL::Date d2(di);
			CPPUNIT_ASSERT_EQUAL(d1, d2);
			CPPUNIT_ASSERT_EQUAL(d2.ToDayIndex(), di);
			di -= i;
			d1.SubDays(i);
		}
	}

	{
		static const int N = 0x7fffffff / (24 * 60 * 60);
		time_t ts = 0;
		for (int i = 0; i < N; ++i)
		{
			XTL::Date d1(1970, 1, 1);
			d1.AddDays(i);

			struct tm * t = ::gmtime(&ts);

			CPPUNIT_ASSERT_EQUAL(d1, XTL::Date(t->tm_year + 1900, t->tm_mon + 1, t->tm_mday));
			CPPUNIT_ASSERT_EQUAL((d1 - XTL::Date(1970, 1, 1)) * 86400, static_cast<int>(ts));

			ts += 24 * 60 * 60;
		}
	}
}

