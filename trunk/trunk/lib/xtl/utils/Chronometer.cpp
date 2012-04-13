#include "Chronometer.hpp"

#include <time.h>
#include <sys/time.h>

namespace XTL
{
	Chronometer::Chronometer()
		: start_(0.0),
		  elapsed_(0.0)
	{
		;;
	}

	namespace
	{
		double GetTimeStamp()
		{
			timeval t;
			::gettimeofday(&t, 0);
			return static_cast<double>(t.tv_sec) + static_cast<double>(t.tv_usec) / 1000000.0;
		}
	}

	Chronometer & Chronometer::Start()
	{
		start_ = GetTimeStamp();
		return *this;
	}

	Chronometer & Chronometer::Stop()
	{
		elapsed_ = GetTimeStamp() - start_;
		return *this;
	}

	double Chronometer::Elapsed() const
	{
		return elapsed_;
	}
}
