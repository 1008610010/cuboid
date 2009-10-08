#include <xtl/FileLock.hpp>

int main(int argc, const char * argv[])
{
	XTL::FileLock lock("test.lock");

	try
	{
		lock.Lock(true);
		fprintf(stderr, "Started\n");
		for (int i = 3; i > 0; --i)
		{
			fprintf(stderr, "%d\n", i);
			sleep(1);
		}
		fprintf(stderr, "Stopped\n");
		lock.Unlock();
	}
	catch (const XTL::FileLock::Error & e)
	{
		fprintf(stderr, "%s\n", e.What());
		return 1;
	}

	return 0;
}
