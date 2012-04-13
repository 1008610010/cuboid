#include "MessageQueue.hpp"

#include <errno.h>
//#include <sys/ipc.h>
#include <sys/msg.h>

namespace XC2
{
	MessageQueue::MessageQueue()
		: key_(-1), id_(-1)
	{
		;;
	}

	bool MessageQueue::Open(const CHAR_8 * path, CHAR_8 sym)
	{
		key_ = ftok(path, sym);
		if (key_ < 0)
		{
			return false;
		}
		id_ = msgget(key_, 0666);
		return id_ >= 0;
	}

	bool MessageQueue::Create(const CHAR_8 * path, CHAR_8 sym)
	{
		key_ = ftok(path, sym);
		if (key_ < 0)
		{
			return false;
		}
		id_ = msgget(key_, IPC_CREAT | IPC_EXCL | 0666);
		if (id_ >= 0)
		{
			return true;
		}
		else if (errno == EEXIST)
		{
			id_ = msgget(key_, 0666);
			if (id_ >= 0)
			{
				return true;
			}
		}
		return false;
	}

	void MessageQueue::Close()
	{
		key_ = -1;
		id_ = -1;
	}

	bool MessageQueue::Destroy(const CHAR_8 * path, CHAR_8 sym)
	{
		key_t key = ftok(path, sym);
		if (key < 0)
		{
			return false;
		}
		int id = msgget(key, 0666);
		if (id < 0)
		{
			return false;
		}
		return msgctl(id, IPC_RMID, NULL) == 0;
	}

	MessageQueue::Result MessageQueue::SendMessage(const void * msg, INT_32 size, bool wait)
	{
		if (id_ < 0)
		{
			return CLOSED;
		}
		if (msgsnd(id_, msg, size - sizeof(long), wait ? 0 : IPC_NOWAIT) == 0)
		{
			return OK;
		}
		switch (errno)
		{
			case EAGAIN: return WAIT;
			case EIDRM:  return REMOVED;
			default:     return ERROR;
		}
	}

	MessageQueue::Result MessageQueue::RecvMessage(long type, void * msg, INT_32 size, bool wait)
	{
		if (id_ < 0)
		{
			return CLOSED;
		}
		if (msgrcv(id_, msg, size - sizeof(long), type, wait ? 0 : IPC_NOWAIT) == size - (INT_32) sizeof(long))
		{
			return OK;
		}
		switch (errno)
		{
			case EAGAIN: return WAIT;
			case EIDRM:  return REMOVED;
			default:     return ERROR;
		}
	}

}
