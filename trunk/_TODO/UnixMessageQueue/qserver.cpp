#include <stdio.h>
#include "MessageQueue.hpp"
#include <time.h>

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
	if (!queue.Create(".", 'q'))
	{
		fprintf(stderr, "Unable to create queue\n");
		return 1;
	}

	printf("%d\n", time(NULL));
	return 0;

	timeval timeout;

	while (true)
	{
		Message msg;
		if (queue.RecvMessage(1, msg) == XC2::MessageQueue::OK)
		{
			msg.mtype = msg.pid;
			msg.y = msg.x * msg.x;
			if (queue.SendMessage(msg) != XC2::MessageQueue::OK)
			{
				printf("Error: send message\n");
			}
		}
//		timeout.tv_sec = 0;
//		timeout.tv_usec = 50;
//		select(32, NULL, NULL, NULL, &timeout);
	}
}
