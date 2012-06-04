#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <signal.h>

#include <xtl/linux/UnixError.hpp>

#include "lj-stats-writer/StatsMessage.hpp"
#include "lj-stats-writer/StatsWriterClient.hpp"
#include "lj-stats-writer/StatsWriterServer.hpp"
#include "lj-stats-writer/UnixSocketClient.hpp"
#include "lj-stats-writer/UnixSocketServer.hpp"

const char * const UNIX_SOCKET_PATH = "/home/dnikolaev/prj/cuboid/trunk/build/LJ_STATS_WRITER";

std::auto_ptr<XC3::StatsWriterServer> server;

void TerminateStatsServer(int i)
{	
	fprintf(stderr, "Server is terminating now\n");
	if (server.get() != 0)
	{
		server->OnTerminated();
	}
}

static void Daemonize()
{
	if (::getppid() == 1)
	{
		// Already a daemon
		printf("1\n");
		return;
	}

	// Fork off the parent process
	pid_t pid = ::fork();
	if (pid < 0)
	{
		printf("2\n");
		exit(EXIT_FAILURE);
	}

	// If we got a good PID, then we can exit the parent process
	if (pid > 0)
	{
		printf("3\n");
		exit(EXIT_SUCCESS);
	}

	/* At this point we are executing as the child process */

	/* Change the file mode mask */
	umask(0);

	/* Create a new SID for the child process */
	pid_t sid = ::setsid();
	if (sid < 0)
	{
		printf("4\n");
		exit(EXIT_FAILURE);
	}

	/* Change the current working directory. This prevents the current
	   directory from being locked; hence not being able to remove it. */
	if (::chdir("/") < 0)
	{
		printf("5\n");
		exit(EXIT_FAILURE);
	}

	/* Redirect standard files to /dev/null */
	FILE * dummy;
	//dummy = ::freopen( "/dev/null", "r", stdin);
	//dummy = ::freopen( "/dev/null", "w", stdout);
	//dummy = ::freopen( "/dev/null", "w", stderr);
}

int main(int argc, const char * argv[])
{
	Daemonize();

	sigset_t newset;
	sigemptyset(&newset);
	sigaddset(&newset, SIGHUP);
	sigprocmask(SIG_BLOCK, &newset, 0);

	struct sigaction sa;
	sa.sa_handler = TerminateStatsServer;
	sigaction(SIGTERM, &sa, 0);
	sigaction(SIGINT,  &sa, 0);

	try
	{
		server.reset(new XC3::StatsWriterServer(UNIX_SOCKET_PATH, "/home/dnikolaev/prj/cuboid/trunk/build"));

		server->Run();

		fprintf(stderr, "Server is shutting down.\n");
	}
	catch (const XTL::UnixError & e)
	{
		fprintf(stderr, "%d %s\n", e.Code(), e.What().c_str());
		return 1;
	}

/*
		// Child process
		try
		{
			sleep(1);

			XC3::StatsWriterClient client(UNIX_SOCKET_PATH);

			client.Send("This is test message...");

			sleep(1);

			client.Send("This is test message 2...");
		}
		catch (const XTL::UnixError & e)
		{
			fprintf(stderr, "Client Error: %s\n", e.What().c_str());
			return 1;
		}
*/
}

