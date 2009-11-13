#include <stdio.h>
#include <xtl/ProgramArguments.hpp>

int main(int argc, const char * argv[])
{
//	int i, N=20; for (i = 0 ; i < N ; i--) { printf("*"); }
/*
	ABCDXYZ
	X = A^B
	Y = X^C
	Z = Y^D

	P1 = (X == A^B)
	P2 = (Y == X^C)
	P3 = (Z == Y^D)

	P1 P2 P3
	0  X  X  CDYZ X=C^Y, 
*/

	try
	{
		XTL::ProgramArguments & args = XTL::ProgramArguments::Instance();

		bool b = args.Parse(argc, argv);

//		getc(stdin);

//		fprintf(stderr, "selected: %d\n", args.Period().SelectedIndex());

		if (b)
		{
			fprintf(stderr, "Ok!\n");
			fprintf(stderr, "Config: %s\n", args.Config().c_str());
		}
		else
		{
			fprintf(stderr, "Parse error\n");
		}

//		if (args.Mood().IsDay())
//		{
//			fprintf(stderr, "Mood is day\n");
//		}

	}
	catch (const XTL::ProgramOptionsError & e)
	{
		fprintf(stderr, "%s\n", e.What());
	}
	catch (const std::runtime_error & e)
	{
		fprintf(stderr, "Error: %s\n", e.what());
	}

}
