#include "Date.hpp"
#include "Algorithm.hpp"
#include "../Exception.hpp"
#include "../FormatString.hpp"

namespace XTL
{
	Date::Date(int year, int month, int day)
		: year_(year),
		  month_(month),
		  day_(day)
	{
		if (!IsValid())
		{
			throw ILLEGAL_ARGUMENT_ERROR(FormatString("XTL::Date::Date(%d, %d, %d) - Invalid date (YYYY, MM, DD)", year, month, day));
		}
	}

	Date::Date(int dayIndex)
		: year_(0),
		  month_(0),
		  day_(0)
	{
		FromDayIndex(dayIndex);
	}

	bool Date::operator== (const Date & other) const
	{
		return Year() == other.Year() && Month() == other.Month() && Day() == other.Day();
	}

	bool Date::operator< (const Date & other) const
	{
		if (Year() < other.Year())
		{
			return true;
		}
		else if (Year() == other.Year())
		{
			if (Month() < other.Month())
			{
				return true;
			}
			else if (Month() == other.Month())
			{
				return Day() < other.Day();
			}
		}

		return false;
	}

	Date & Date::AddDays(int count)
	{
		if (count < 0)
		{
			return SubDays(-count);
		}

		day_ += count;

		int dim = DaysInMonth(year_, month_);
		while (day_ > dim)
		{
			day_ -= dim;

			++month_;
			if (month_ > MONTHES_IN_YEAR)
			{
				month_ = 1;
				++year_;
			}

			dim = DaysInMonth(year_, month_);
		}

		return *this;
	}

	Date & Date::SubDays(int count)
	{
		if (count < 0)
		{
			return AddDays(-count);
		}

		day_ -= count;

		while (day_ < 1)
		{
			--month_;
			if (month_ < 1)
			{
				month_ = MONTHES_IN_YEAR;
				--year_;
			}

			day_ += DaysInMonth(year_, month_);
		}

		return *this;
	}

	Date & Date::IncDay()
	{
		return AddDays(1);
	}

	Date & Date::DecDay()
	{
		return SubDays(1);
	}

	Date & Date::AddMonthes(int count)
	{
		if (count < 0)
		{
			return SubMonthes(-count);
		}

		month_ += count;

		if (month_ > MONTHES_IN_YEAR)
		{
			year_ += (month_ - 1) / MONTHES_IN_YEAR;
			month_ = (month_ - 1) % MONTHES_IN_YEAR + 1;
		}

		day_ = Min(day_, DaysInMonth(year_, month_));

		return *this;
	}

	Date & Date::SubMonthes(int count)
	{
		if (count < 0)
		{
			return AddMonthes(-count);
		}

		month_ -= count;

		if (month_ < 1)
		{
			year_ -= (MONTHES_IN_YEAR - month_) / MONTHES_IN_YEAR;
			month_ = MONTHES_IN_YEAR - (- month_) % MONTHES_IN_YEAR;
		}

		day_ = Min(day_, DaysInMonth(year_, month_));

		return *this;
	}

	Date & Date::IncMonth()
	{
		return AddMonthes(1);
	}

	Date & Date::DecMonth()
	{
		return SubMonthes(1);
	}

	Date & Date::AddYears(int count)
	{
		if (count < 0)
		{
			return SubYears(-count);
		}

		year_ += count;
		day_ = Min(day_, DaysInMonth(year_, month_));
	
		return *this;
	}

	Date & Date::SubYears(int count)
	{
		if (count < 0)
		{
			return AddYears(-count);
		}

		year_ -= count;
		day_ = Min(day_, DaysInMonth(year_, month_));

		return *this;
	}

	Date & Date::IncYear()
	{
		return AddYears(1);
	}

	Date & Date::DecYear()
	{
		return SubYears(1);
	}

	int Date::IsLeapYear(int year)
	{
		return (year % 4 == 0) && (year % 100 != 0 || year % 400 == 0) ? 1 : 0;
	}

	bool Date::IsValid(int year, int month, int day)
	{
		return month >= 1 && month <= MONTHES_IN_YEAR &&
		       day   >= 1 && day   <= DaysInMonth(year, month);
	}

	int Date::DaysInMonth(int year, int month)
	{
		static const int DAYS_IN_MONTH[2][13] = {
			{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
			{ 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
		};

		return DAYS_IN_MONTH[IsLeapYear(year)][month];
	}

	int Date::DaysInYear(int year)
	{
		return DAYS_IN_YEAR + IsLeapYear(year);
	}

	int Date::DayOfYear(int year, int month, int day)
	{
		static const int DAYS_BEFORE[2][13] = {
			{ -1, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 },
			{ -1, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 }
		};

		return DAYS_BEFORE[IsLeapYear(year)][month] + day - 1;
	}

	void Date::FromDayOfYear(int year, int dayOfYear, int & month, int & day)
	{
		static const int DAYS_FROM_YEAR_BEGIN[2][13] = {
			{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
			{ 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }
		};

		const int * const DFYB = DAYS_FROM_YEAR_BEGIN[IsLeapYear(year)];
		if (dayOfYear < 0 || dayOfYear >= DFYB[MONTHES_IN_YEAR])
		{
			throw ILLEGAL_ARGUMENT_ERROR(FormatString("XTL::Date::FromDayOfYear(%d, %d, month, day) - Invalid value of parameter dayOfYear", year, dayOfYear));
		}

		// TODO: добавить проверку условия 0 <= dayOfYear < DaysInYear(year)

		for (month = 1; month <= MONTHES_IN_YEAR; ++month)
		{
			if (dayOfYear < DFYB[month])
			{
				day = (dayOfYear + 1) - DFYB[month - 1];
				return;
			}
		}
	}

	int Date::ToDayIndex() const
	{
		int index = ((year_ - 1) / 400) * DAYS_IN_400_YEARS;

		int y = (year_ - 1) % 400;
		index += y * DAYS_IN_YEAR + (y / 4) - (y / 100);
		index += DayOfYear(year_, month_, day_);

		return index;
	}

	void Date::FromDayIndex(int dayIndex)
	{
		// TODO: dayIndex < 0 ???
		int y400 = dayIndex / DAYS_IN_400_YEARS;
		int d = dayIndex % DAYS_IN_400_YEARS;

		int y100 = Min(d / DAYS_IN_100_YEARS, 3);
		d -= y100 * DAYS_IN_100_YEARS;

		int y4 = d / DAYS_IN_4_YEARS;
		d = d % DAYS_IN_4_YEARS;

		int y1 = Min(d / DAYS_IN_YEAR, 3);
		d -= y1 * DAYS_IN_YEAR;

		year_ = 400 * y400 + 100 * y100 + 4 * y4 + y1 + 1;

		FromDayOfYear(year_, d, month_, day_);
	}
}

