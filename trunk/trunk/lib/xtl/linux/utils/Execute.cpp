#include "Execute.hpp"

#include <unistd.h>
#include <sys/wait.h>

#include <memory>

#include "../../utils/AutoPtrVector.hpp"

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

	void Exec(const std::string & filePath, char * const argv[], const ForkExecErrorListener & errorListener)
	{
		if (::execvp(filePath.c_str(), argv) == -1)
		{
			errorListener.OnExecError(filePath, UnixError());
		}
	}

	void Exec(const std::string & filePath, const ForkExecErrorListener & errorListener)
	{
		std::string argv0(filePath);
		char * const argv[] = { &argv0[0], NULL };

		Exec(filePath, argv, errorListener);
	}

	namespace
	{
		class MutableArguments
		{
			public:

				explicit MutableArguments(const std::string & filePath, const std::vector<std::string> & arguments)
					: args_(arguments.size() + 1)
				{
					args_.Set(0, CopyArgument(filePath));
					for (unsigned int i = 0; i < arguments.size(); ++i)
					{
						args_.Set(i + 1, CopyArgument(arguments[i]));
					}
				}

				void ReleasePointers(std::vector<char *> & outputArgs)
				{
					outputArgs.resize(args_.Size() + 1);
					unsigned int i = 0;
					while (i < args_.Size())
					{
						outputArgs[i] = &(*args_[i])[0];
						++i;
					}

					outputArgs[i] = NULL;
				}

			private:

				static std::auto_ptr<std::vector<char> > CopyArgument(const std::string & arg)
				{
					return std::auto_ptr<std::vector<char> >(new std::vector<char>(arg.c_str(), arg.c_str() + (arg.size() + 1)));
				}

				XTL::AutoPtrVector<std::vector<char> > args_;
		};
	}

	void Exec(const std::string & filePath, const std::vector<std::string> & arguments, const ForkExecErrorListener & errorListener)
	{
		MutableArguments mutableArguments(filePath, arguments);

		std::vector<char *> argv;
		mutableArguments.ReleasePointers(argv);

		Exec(filePath, &argv[0], errorListener);
	}

	namespace
	{
		void ForkExecWait(const std::string & filePath, char * const argv[], const ForkExecErrorListener & errorListener)
		{
			pid_t pid = ::fork();
			if (pid == 0)
			{
				// Child process

				Exec(filePath, argv, errorListener);

				// It will be executed on Exec error.
				throw ChildExit();
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
	}

	void ForkExecWait(const std::string & filePath, const ForkExecErrorListener & errorListener)
	{
		std::string argv0(filePath);
		char * const argv[] = { &argv0[0], NULL };

		ForkExecWait(filePath, argv, errorListener);
	}

	void ForkExecWait(const std::string & filePath, const std::vector<std::string> & arguments, const ForkExecErrorListener & errorListener)
	{
		MutableArguments mutableArguments(filePath, arguments);

		std::vector<char *> argv;
		mutableArguments.ReleasePointers(argv);

		ForkExecWait(filePath, &argv[0], errorListener);
	}

	void ForkExecWait(const std::string & filePath, const std::string & arg1, const ForkExecErrorListener & errorListener)
	{
		std::vector<std::string> arguments(1);
		arguments[0] = arg1;
		ForkExecWait(filePath, arguments, errorListener);
	}

	void ForkExecWait(const std::string & filePath, const std::string & arg1, const std::string & arg2, const ForkExecErrorListener & errorListener)
	{
		std::vector<std::string> arguments(2);
		arguments[0] = arg1;
		arguments[1] = arg2;
		ForkExecWait(filePath, arguments, errorListener);
	}

	void DoubleForkExec(const std::string & filePath, const std::vector<std::string> & arguments, const ForkExecErrorListener & errorListener)
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
				errorListener.OnDoubleForkError(UnixError());
				throw ChildExit();
			}
			else if (pid2 != 0)
			{
				throw ChildExit();
			}
			else
			{
				Exec(filePath, arguments, errorListener);

				// It will be executed on Exec error.
				throw ChildExit();
			}
		}
	}

	void DoubleForkExec(const std::string           & filePath,
	                    const ForkExecErrorListener & errorListener)
	{
		std::vector<std::string> arguments;
		DoubleForkExec(filePath, arguments, errorListener);
	}

	void DoubleForkExec(const std::string           & filePath,
	                    const std::string           & arg1,
	                    const ForkExecErrorListener & errorListener)
	{
		std::vector<std::string> arguments(1);
		arguments[0] = arg1;
		DoubleForkExec(filePath, arguments, errorListener);
	}
}
