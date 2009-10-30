#include <stdio.h>
#include <xtl/ProgramArguments.hpp>

int main(int argc, const char * argv[])
{
	XTL::ProgramArguments & args = XTL::ProgramArguments::Instance();

	try
	{
		bool b = args.Parse(argc, argv);

		if (b)
		{
			fprintf(stderr, "Ok!\n");
			fprintf(stderr, "Config: %s\n", args.Config().c_str());
		}
		else
		{
			fprintf(stderr, "Parse error\n");
		}
	}
	catch (const std::runtime_error & e)
	{
		fprintf(stderr, "Error: %s\n", e.what());
	}
}
