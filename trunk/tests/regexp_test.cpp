#include <stdio.h>
#include <stdexcept>
#include <xtl/RegExp.hpp>

int main(int argc, const char * argv[])
{
	try
	{
		XTL::RegExp re("(\\d\\d\\d\\d)-(\\d\\d)-(\\d\\d)");
		int year;
		int month;
		int day;
		if (re.Search("Today is 2014-02-20.", year, month, day))
		{
			printf("%02d/%02d/%04d\n", day, month, year);
		}
		else
		{
			printf("Not found.\n");
		}
	}
	catch (const std::runtime_error & e)
	{
		fprintf(stderr, "Error: %s\n", e.what());
	}
}

