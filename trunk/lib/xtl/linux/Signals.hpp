#ifndef XTL__SIGNALS_HPP__
#define XTL__SIGNALS_HPP__ 1

#include <signal.h>

#include "UnixError.hpp"

namespace XTL
{
	typedef void (*SignalHandlerProc) (int);

	/**
	 * @throw UnixError
	 */
	void SetSignalHandler(int signum, SignalHandlerProc proc);
}

#endif
