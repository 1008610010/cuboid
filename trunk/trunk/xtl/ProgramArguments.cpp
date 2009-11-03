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

		std::list<ArgumentsPool *> pools;
		pools.push_back(this);

		for (int i = 1; i < argc; ++i)
		{
			const char * s = argv[i];
			if (s == 0)
			{
				continue;
			}

			fprintf(stdout, "%s\n", s);

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
					ArgumentDesc * desc = 0;
					for (std::list<ArgumentsPool *>::iterator itr = pools.begin();
					     desc == 0 && itr != pools.end();
					     ++itr)
					{
						desc = (*itr)->FindByLabel2(s + 2);
					}

					if (desc == 0)
					{
						fprintf(stderr, "label2=%s not found\n", s + 2);
						return false;
					}

					arg = desc->arg;

					if (arg->NeedValue())
					{
						fprintf(stderr, "label2=%s not need value\n", s + 2);
						return false;
					}

					ArgumentsPool * subpool = arg->Set(0);
					if (subpool != 0)
					{
						pools.push_front(subpool);
					}
				}
				else
				{
					std::string label2 = std::string(s + 2, t - (s + 2));
					ArgumentDesc * desc = 0;
					for (std::list<ArgumentsPool *>::iterator itr = pools.begin();
					     desc == 0 && itr != pools.end();
					     ++itr)
					{
						desc = (*itr)->FindByLabel2(label2.c_str());
					}

					if (desc == 0)
					{
						fprintf(stderr, "label2=%s not found\n", std::string(s + 2, t - (s + 2)).c_str());
						return false;
					}

					arg = desc->arg;

					if (!arg->NeedValue())
					{
						return false;
					}

					ArgumentsPool * subpool = arg->Set(t + 1);
					if (subpool != 0)
					{
						pools.push_front(subpool);
					}
				}
			}
			else
			{
				ArgumentDesc * desc = 0;
				for (std::list<ArgumentsPool *>::iterator itr = pools.begin();
				     desc == 0 && itr != pools.end();
				     ++itr)
				{
					desc = (*itr)->FindByLabel1(s + 1);
				}

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

					ArgumentsPool * subpool = arg->Set(arg->Set(argv[i]));
					if (subpool != 0)
					{
						pools.push_front(subpool);
					}
				}
				else
				{
					ArgumentsPool * subpool = arg->Set(0);

					if (subpool != 0)
					{
						pools.push_front(subpool);
					}
				}
			}
		}

		// TODO: check pools list for "required" arguments

		return true;
	}
}
