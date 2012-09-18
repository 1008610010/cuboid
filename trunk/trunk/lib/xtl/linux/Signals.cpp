#include "Signals.hpp"

#include <string.h>

namespace XTL
{
	void SetSignalHandler(int signum, SignalHandlerProc proc)
	{
		struct sigaction sa;
		::memset(&sa, 0, sizeof(sa));
		sa.sa_handler = proc;

		if (::sigaction(signum, &sa, 0) != 0)
		{
			throw UnixError();
		}
	}
}
