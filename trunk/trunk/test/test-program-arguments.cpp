#include <stdio.h>
#include <xtl/ProgramOptions.hpp>

namespace XTL
{
	class SimpleOptionsPrinter
	{
		public:

			SimpleOptionsPrinter(FILE * file, int columnSize)
				: f_(file), column_(columnSize, ' ')
			{
				;;
			}

			void OnMode(const OptionDesc * desc, const Option * option, int depth)
			{
				fprintf(f_, "  %s:\n", (Space(depth * 2) + desc->desc).c_str());
			}

			void OnModeValue(const OptionDesc * desc, const Option * option, int depth)
			{
				Print(std::string("  ") + Space(depth * 2) + desc->AsString(), desc->desc);
			}

			void OnOption(const OptionDesc * desc, const Option * option, int depth)
			{
				Print(std::string("  ") + Space(depth * 2) + desc->AsString(), desc->desc);
			}

		protected:

			void Indent(int depth)
			{
				for (; depth > 0; --depth)
				{
					fprintf(f_, "  ");
				}
			}

			const std::string Space(int size) const
			{
				return std::string(size, ' ');
			}

			void Print(const std::string & first, const std::string second)
			{
				if (first.length() >= column_.length())
				{
					fprintf(
						f_,
						"%s\n%s%s\n",
						first.c_str(),
						column_.c_str(),
						second.c_str()
					);
				}
				else
				{
					fprintf(
						f_,
						"%s%s\n",
						(first + Space(column_.length() - first.length())).c_str(),
						second.c_str()
					);
				}
			}

			FILE * f_;
			std::string column_;
	};
}

int main(int argc, const char * argv[])
{
	try
	{
		XTL::ProgramArguments & args = XTL::ProgramArguments::Instance();

//		XTL::ProgramArguments::PrintHelp(stderr);
		XTL::ProgramArguments::Instance().Iterate(XTL::SimpleOptionsPrinter(stderr, 28));
		return 0;

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
