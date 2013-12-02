#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <vector>

#include <xtl/linux/utils/Execute.hpp>

class MyErrorListener : public XTL::ForkExecErrorListener
{
	public:

		virtual ~MyErrorListener() throw() { ;; }

		virtual void OnExecError(const std::string & filePath, const XTL::UnixError & e) const
		{
			fprintf(stderr, "OnExecError: %s\n", e.What().c_str());
		}

		virtual void OnDoubleForkError(const XTL::UnixError & e) const
		{
			;;
		}
};

class C
{
	public:

		C()
			: ptr_(malloc(1000))
		{
			fprintf(stderr, "Constructor from %d\n", getpid());
		}

		~C()
		{
			fprintf(stderr, "Destructor from %d\n", getpid());
			free(ptr_);
		}

		void Say()
		{
			fprintf(stderr, "This is %d (%p).\n", getpid(), ptr_);
		}

	private:

		void * ptr_;
};

int main(int argc, char * argv[])
{
	C c;
	std::vector<char> v(1024);
	printf("Hello! I am %d.\n", getpid());

	try
	{
		pid_t pid = ::fork();
		c.Say();
		if (pid == 0)
		{
			// Child process
			XTL::ForkExecWait("ls", "-l", MyErrorListener());
			throw XTL::ChildExit();
		}
		else if (pid < 0)
		{
			throw XTL::UnixError();
		}
		else
		{
			XTL::WaitPid(pid);
		}
		// ForkExecWait("/bin/ls", MyErrorListener());
	}
	catch (const XTL::ChildExit &)
	{
		fprintf(stderr, "Error\n");
		// exit(0);
		//std::terminate();
	}
}
