#include <stdio.h>

#include <xtl/linux/UnixError.hpp>

#include "lj-stats-writer/StatsMessage.hpp"
#include "lj-stats-writer/StatsWriterClient.hpp"

const char * const UNIX_SOCKET_PATH = "/home/dnikolaev/prj/cuboid/trunk/build/LJ_STATS_WRITER";

int main(int argc, const char * argv[])
{
	try
	{
		XC3::StatsWriterClient client(UNIX_SOCKET_PATH);

		client.Send("This is test message...");
		client.Send("This is test message 2...");
	}
	catch (const XTL::UnixError & e)
	{
		fprintf(stderr, "Client Error: %s\n", e.What().c_str());
		return 1;
	}
}

