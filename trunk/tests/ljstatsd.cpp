#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <signal.h>

#include <xtl/linux/UnixError.hpp>
#include <xtl/linux/utils/Daemonize.hpp>

#include "lj-stats-writer/StatsMessage.hpp"
#include "lj-stats-writer/StatsWriterServer.hpp"

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

int main(int argc, const char * argv[])
{
	XTL::Daemonize();

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

