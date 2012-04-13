#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include "MessageQueue.hpp"

struct Message
{
	long  mtype;
	long  pid;
	float x;
	float y;
};

int main(int argc, const char * argv[])
{
	XC2::MessageQueue queue;
	if (!queue.Open(".", 'q'))
	{
		fprintf(stderr, "Unable to open queue\n");
		return 1;
	}

	timeval timeout;

	timeval last;
	timeval now;

	gettimeofday(&last, NULL);

	int pid = getpid();
	int i = 0;
	while (true)
	{
		//printf("*\n");
		Message msg;
		msg.mtype = 1;
		msg.pid = pid;
		msg.x = i++;

		if (queue.SendMessage(msg) == XC2::MessageQueue::OK)
		{
			while (queue.RecvMessage(pid, msg) != XC2::MessageQueue::OK)
			{
				timeout.tv_sec = 0;
				timeout.tv_usec = 50;
				select(32, NULL, NULL, NULL, &timeout);
			}
//			printf("Received: %0.3f\n", msg.y);
			if (i % 10000 == 0)
			{
				gettimeofday(&now, NULL);
				printf("%d - %0.3f\n", i, (double) i / ((double) now.tv_sec - (double) last.tv_sec + ((double) now.tv_usec - (double) last.tv_usec) / 1000000.0));
			}
		}
		else
		{
			printf("Error: send message\n");
		}
		/*
		timeout.tv_sec = 0;
		timeout.tv_usec = 10;
		select(1, NULL, NULL, NULL, &timeout);
		*/
		//usleep(100);
		//select(0, NULL, NULL, NULL, &timeout);
	}
}
