#ifndef XTL__DATE_HPP__
#define XTL__DATE_HPP__ 1

namespace XTL
{
	class Date
	{
		public:

			static const int MONTHES_IN_YEAR = 12;
			static const int DAYS_IN_YEAR = 365;
			static const int DAYS_IN_4_YEARS = 4 * DAYS_IN_YEAR + 1;        // +1 - четвертый год - високосный
			static const int DAYS_IN_100_YEARS = 25 * DAYS_IN_4_YEARS - 1;  // -1 - сотый год - невисокосный
			static const int DAYS_IN_400_YEARS = 4 * DAYS_IN_100_YEARS + 1; // +1 - четырехсотый год - високосный

			Date(int year, int month, int day);

			explicit Date(int dayIndex);

			int Year() const
			{
				return year_;
			}

			int Month() const
			{
				return month_;
			}

			int Day() const
			{
				return day_;
			}

			bool IsValid() const
			{
				return IsValid(year_, month_, day_);
			}

			int DaysInMonth() const
			{
				return DaysInMonth(year_, month_);
			}

			bool operator== (const Date & other) const;

			bool operator< (const Date & other) const;

			Date & AddDays(int count);

			Date & SubDays(int count);

			Date & IncDay();

			Date & DecDay();

			Date & AddMonthes(int count);

			Date & SubMonthes(int count);

			Date & IncMonth();

			Date & DecMonth();

			Date & AddYears(int count);

			Date & SubYears(int count);

			Date & IncYear();

			Date & DecYear();

			static bool IsValid(int year, int month, int day);

			static int IsLeapYear(int year);

			static int DaysInMonth(int year, int month);

			static int DaysInYear(int year);

			static int DayOfYear(int year, int month, int day);

			/*
				dayOfYear = [ 0 .. 364 + IsLeapYear(year) ]
					0 - 01/01/year
					1 - 02/01/year
					...
			*/
			static void FromDayOfYear(int year, int dayOfYear, int & month, int & day);

			int ToDayIndex() const;

			void FromDayIndex(int dayIndex);

		private:

			int year_;
			int month_;
			int day_;
	};

	inline bool operator!= (const Date & left, const Date & right)
	{
		return !(left == right);
	}

	inline bool operator> (const Date & left, const Date & right)
	{
		return right < left;
	}

	inline bool operator<= (const Date & left, const Date & right)
	{
		return !(right < left);
	}

	inline bool operator>= (const Date & left, const Date & right)
	{
		return !(left < right);
	}

	inline int operator- (const Date & dateTo, const Date & dateFrom)
	{
		return dateTo.ToDayIndex() - dateFrom.ToDayIndex();
	}
}

#endif

