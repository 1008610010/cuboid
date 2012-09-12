#include "Utils.hpp"

#include <math.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

namespace XTL
{
	void Sleep(double t)
	{
		struct timeval tv = {
			static_cast<int>(t),
			static_cast<int>(1000000.0 * (t - ::floor(t)))
		};

		::select(0, NULL, NULL, NULL, &tv);
	}
}
