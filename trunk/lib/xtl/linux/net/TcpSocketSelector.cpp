#include "TcpSocketSelector.hpp"

#include "../UnixError.hpp"

#include <math.h>
#include <unistd.h>

namespace XTL
{
	TcpSocketSelector::TcpSocketSelector()
		: maxfd_(-1),
		  readSet_(),
		  writeSet_()
	{
		;;
	}

	void TcpSocketSelector::Clear()
	{
		maxfd_ = -1;
		readSet_.Clear();
		writeSet_.Clear();
	}

	bool TcpSocketSelector::Insert(TcpSocket socket, bool checkRead, bool checkWrite)
	{
		bool insertedRead  = checkRead  && readSet_.Insert(socket);
		bool insertedWrite = checkWrite && writeSet_.Insert(socket);

		if (!insertedRead && !insertedWrite)
		{
			return false;
		}

		if (socket.Desc() > maxfd_)
		{
			maxfd_ = socket.Desc();
		}

		return true;
	}

	bool TcpSocketSelector::Delete(TcpSocket socket)
	{
		bool deletedRead  = readSet_.Delete(socket);
		bool deletedWrite = writeSet_.Delete(socket);

		if (!deletedRead && !deletedWrite)
		{
			return false;
		}

		if (socket.Desc() == maxfd_)
		{
			for (--maxfd_; maxfd_ >= 0; --maxfd_)
			{
				if (readSet_.Contains(maxfd_) || writeSet_.Contains(maxfd_))
				{
					break;
				}
			}
		}

		return true;
	}

	void TcpSocketSelector::Select(SelectResult & result, const Timeout & timeout)
	{
		result.Select(maxfd_, readSet_, writeSet_, timeout);
	}



	TcpSocketSelector::Timeout::Timeout()
		: t_(0)
	{
		;;
	}

	TcpSocketSelector::Timeout::Timeout(int secs, int microsecs)
		: t_(0)
	{
		Init(secs, microsecs);
	}

	TcpSocketSelector::Timeout::Timeout(int secs)
		: t_(0)
	{
		Init(secs, 0);
	}

	TcpSocketSelector::Timeout::Timeout(double secs)
		: t_(0)
	{
		Init(floor(secs), static_cast<int>(1000000.0 * (secs - floor(secs))));
	}

	TcpSocketSelector::Timeout::Timeout(const Timeout & other)
		: t_(0)
	{
		if (!other.IsNull())
		{
			Init(other.Secs(), other.MicroSecs());
		}
	}

	TcpSocketSelector::Timeout::~Timeout() throw()
	{
		delete t_;
	}

	TcpSocketSelector::Timeout & TcpSocketSelector::Timeout::operator= (const Timeout & other)
	{
		if (this == &other)
		{
			return *this;
		}

		if (other.IsNull())
		{
			if (t_ != 0)
			{
				delete t_;
				t_ = 0;
			}
		}
		else
		{
			Init(other.Secs(), other.MicroSecs());
		}

		return *this;
	}

	void TcpSocketSelector::Timeout::Init(int secs, int microsecs)
	{
		if (t_ == 0)
		{
			t_ = new struct timeval;
		}

		t_->tv_sec = secs;
		t_->tv_usec = microsecs;
	}



	void TcpSocketSelector::SelectResult::Select(int maxfd, const TcpSocketSet & readSet, const TcpSocketSet & writeSet, const Timeout & timeout)
	{
		readSet_  = readSet;
		writeSet_ = writeSet;
		timeout_  = timeout;

		selectedCount_ = ::select(maxfd + 1, readSet_.Raw(), writeSet_.Raw(), 0, timeout_.Raw());

		if (selectedCount_ == -1)
		{
			if (errno == EINTR)
			{
				throw UnixError::Interrupted();
			}
			else
			{
				throw UnixError();
			}
		}
	}
}

