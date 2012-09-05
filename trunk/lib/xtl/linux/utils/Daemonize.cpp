#include "Daemonize.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace XTL
{
	void Daemonize()
	{
		if (::getppid() == 1)
		{
			// Already a daemon
			return;
		}

		// Fork off the parent process
		pid_t pid = ::fork();
		if (pid < 0)
		{
			::exit(EXIT_FAILURE);
		}

		// If we got a good PID, then we can exit the parent process
		if (pid > 0)
		{
			::exit(EXIT_SUCCESS);
		}

		/* At this point we are executing as the child process */

		/* Change the file mode mask */
		::umask(0);

		/* Create a new SID for the child process */
		pid_t sid = ::setsid();
		if (sid < 0)
		{
			exit(EXIT_FAILURE);
		}

		/* Change the current working directory. This prevents the current
		   directory from being locked; hence not being able to remove it. */
		if (::chdir("/") < 0)
		{
			exit(EXIT_FAILURE);
		}

		/* Redirect standard files to /dev/null */
		// FILE * dummy;
		// dummy = ::freopen( "/dev/null", "r", stdin);
		// dummy = ::freopen( "/dev/null", "w", stdout);
		// dummy = ::freopen( "/dev/null", "w", stderr);
	}
}

