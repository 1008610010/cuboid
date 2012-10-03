#include "Execute.hpp"

#include <unistd.h>
#include <sys/wait.h>

namespace XTL
{
	void WaitPid(pid_t pid)
	{
		int status = 0;

		while (true)
		{
			pid_t result = ::waitpid(pid, &status, 0);

			if (result == -1)
			{
				if (errno != EINTR)
				{
					throw UnixError();
				}
			}
			else
			{
				break;
			}
		}
	}

	void Exec(const std::string & filePath, const ForkExecListener & listener)
	{
		char * const argv[] = { NULL };
		char * const envp[] = { NULL };

		if (::execve(filePath.c_str(), argv, envp) == -1)
		{
			listener.OnExecError(filePath, UnixError());
		}
	}

	void ForkExecWait(const std::string & filePath, const ForkExecListener & listener)
	{
		pid_t pid = ::fork();
		if (pid == 0)
		{
			// Child process

			Exec(filePath, listener);

			// It will never be executed.
			throw TerminateProgram();
		}
		else if (pid < 0)
		{
			throw UnixError();
		}
		else
		{
			// Parent process

			WaitPid(pid);
		}
	}

	void DoubleForkExec(const std::string & filePath, const ForkExecListener & listener)
	{
		pid_t pid = ::fork();
		if (pid < 0)
		{
			throw UnixError();
		}
		else if (pid != 0)
		{
			// Parent process

			WaitPid(pid);
		}
		else
		{
			// Child process

			pid_t pid2 = ::fork();
			if (pid2 < 0)
			{
				listener.OnDoubleForkError(UnixError());
				throw TerminateProgram();
			}
			else if (pid2 != 0)
			{
				throw TerminateProgram();
			}
			else
			{
				Exec(filePath, listener);

				// It will never be executed.
				throw TerminateProgram();
			}
		}
	}
}
