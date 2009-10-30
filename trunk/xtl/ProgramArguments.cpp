#include "ProgramArguments.hpp"

namespace XTL
{
	bool ArgumentsPool::Parse(int argc, const char * argv[])
	{
		if (argc == 0)
		{
			return false;
		}

		std::string programName;
		programName.assign(argv[0]);

		for (int i = 1; i < argc; ++i)
		{
			const char * s = argv[i];
			if (s == 0)
			{
				continue;
			}

			if (s[0] != '-')
			{
				return false;
			}

			ArgumentBase * arg = 0;
			if (s[1] == '-')
			{
				const char * t = ::strchr(s + 2, '=');
				if (t == 0)
				{
					ArgumentDesc * desc = FindByFlag2(s + 2);
					if (desc == 0)
					{
						return false;
					}

					arg = desc->arg;

					if (arg->NeedValue())
					{
						return false;
					}

					arg->Set(0);
				}
				else
				{
					ArgumentDesc * desc = FindByFlag2(std::string(s + 2, t - (s + 2)).c_str());
					if (desc == 0)
					{
						return false;
					}

					arg = desc->arg;

					if (!arg->NeedValue())
					{
						return false;
					}

					arg->Set(t + 1);
				}
			}
			else
			{
				ArgumentDesc * desc = FindByFlag1(s + 1);
				if (desc == 0)
				{
					return false;
				}

				arg = desc->arg;

				if (arg->NeedValue())
				{
					if (i >= argc - 1)
					{
						return false;
					}
					++i;
					arg->Set(argv[i]);
				}
				else
				{
					arg->Set(0);
				}
			}
		}

		return true;
	}
}
