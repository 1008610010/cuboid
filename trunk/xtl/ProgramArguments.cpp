#include "ProgramArguments.hpp"
#include "StringUtils.hpp"

namespace XTL
{
	const std::string OptionDesc::AsString() const
	{
		if (label1 != 0)
		{
			if (label2 != 0)
			{
				return std::string("'-") + label1 + "|--" + label2 + "'";
			}
			else
			{
				return std::string("'-") + label1 + "'";
			}
		}
		else if (label2 != 0)
		{
			return std::string("'--") + label2 + "'";
		}
		else
		{
			return std::string();
		}
	}

	OptionsPool::OptionsPool()
		: nextId_(0),
		  set_(0),
		  options_()
	{
		;;
	}

	void OptionsPool::AddOption(const OptionDesc * desc, Option * option)
	{
		fprintf(stderr, "AddOption: %s\n", desc->label1);
		if (desc->label1 != 0 && FindByLabel1(desc->label1) != options_.end())
		{
			throw ProgramOptionsError(FormatString("Internal logic error: duplicate option '-%s'", desc->label1));
		}

		if (desc->label2 != 0 && FindByLabel2(desc->label2) != options_.end())
		{
			throw ProgramOptionsError(FormatString("Internal logic error: duplicate options '--%s'", desc->label2));
		}

		options_.push_back(std::make_pair(desc, option));
	}
	
	const unsigned long long OptionsPool::NextId()
	{
		if (nextId_ >= 9223372036854775808llu)
		{
			throw ProgramOptionsError("Internal logic error: too many options in the same pool");
		}

		return nextId_ = (nextId_ > 0 ? (nextId_ << 1) : 1);
	}

	void OptionsPool::SetPresent(const unsigned long long & id)
	{
		set_ |= id;
	}

	bool OptionsPool::Contains(const unsigned long long & id)
	{
		return (set_ & id) != 0;
	}

	const OptionsPool::ListOfOptions::iterator OptionsPool::FindByLabel1(const char * label1)
	{
		for (ListOfOptions::iterator itr = options_.begin();
		     itr != options_.end();
		     ++itr)
		{
			if (itr->first->label1 != 0 && ::strcmp(itr->first->label1, label1) == 0)
			{
				return itr;
			}
		}
		return options_.end();
	}

	const OptionsPool::ListOfOptions::iterator OptionsPool::FindByLabel2(const char * label2)
	{
		for (ListOfOptions::iterator itr = options_.begin();
		     itr != options_.end();
		     ++itr)
		{
			if (itr->first->label2 != 0 && ::strcmp(itr->first->label2, label2) == 0)
			{
				return itr;
			}
		}
		return options_.end();
	}

	void OptionsPool::CheckRequiredOptions()
	{
		for (ListOfOptions::const_iterator itr = options_.begin();
		     itr != options_.end();
		     ++itr)
		{
			if ((itr->first->flags & Option::REQUIRED) != 0 &&
			    !Contains(itr->second->Id()))
			{
				throw ProgramOptionsError(FormatString("Program option %s is required", itr->first->AsString().c_str()));
			}
		}
	}

	bool OptionsPool::Parse(int argc, const char * argv[])
	{
		if (argc == 0)
		{
			CheckRequiredOptions();
		}

		std::list<OptionsPool *> pools;
		pools.push_back(this);

		std::string programName;
		programName.assign(argv[0]);

		for (int i = 1; i < argc; ++i)
		{
			const char * s = argv[i];
			if (s == 0)
			{
				throw ProgramOptionsError(FormatString("Argument #%d is NULL", i));
			}

			if (s[0] != '-')
			{
				throw ProgramOptionsError(FormatString("Unexpected bareword '%s' found", s));
			}

			const OptionDesc * desc = 0;
			Option * option = 0;

			if (s[1] == '-')
			{
				const char * t = ::strchr(s + 2, '=');
				if (t == 0)
				{
					for (std::list<OptionsPool *>::iterator itr = pools.begin();
					     itr != pools.end();
					     ++itr)
					{
						ListOfOptions::iterator opt = (*itr)->FindByLabel2(s + 2);
						if (opt != options_.end())
						{
							desc = opt->first;
							option = opt->second;
							break;
						}
					}

					if (desc == 0)
					{
						throw ProgramOptionsError(FormatString("Invalid program option: '--%s'", s + 2));
					}

					if (option->NeedValue())
					{
						throw ProgramOptionsError(FormatString("Value expected for program option '--%s'", s + 2));
					}

					OptionsPool * subpool = option->Set(0);
					if (subpool != 0)
					{
						pools.push_front(subpool);
					}
				}
				else
				{
					std::string label2 = std::string(s + 2, t - (s + 2));

					for (std::list<OptionsPool *>::iterator itr = pools.begin();
					     itr != pools.end();
					     ++itr)
					{
						ListOfOptions::iterator opt = (*itr)->FindByLabel2(label2.c_str());
						if (opt != options_.end())
						{
							desc = opt->first;
							option = opt->second;
							break;
						}
					}

					if (desc == 0)
					{
						throw ProgramOptionsError(FormatString("Invalid program option '--%s'", label2.c_str()));
					}

					if (!option->NeedValue())
					{
						throw ProgramOptionsError(FormatString("Unexpected value for program option '--%s'", label2.c_str()));
					}

					OptionsPool * subpool = option->Set(t + 1);
					if (subpool != 0)
					{
						pools.push_front(subpool);
					}

					if ((desc->flags & Option::PASSWORD) != 0)
					{
						for (++t; *t != '\0'; ++t)
						{
							*const_cast<char *>(t) = '*';
						}
					}
				}
			}
			else
			{
				for (std::list<OptionsPool *>::iterator itr = pools.begin();
				     itr != pools.end();
				     ++itr)
				{
					ListOfOptions::iterator opt = (*itr)->FindByLabel1(s + 1);
					if (opt != options_.end())
					{
						desc = opt->first;
						option = opt->second;
						break;
					}
				}

				if (desc == 0)
				{
					throw ProgramOptionsError(FormatString("Invalid program option '-%s'", s + 1));
				}

				if (option->NeedValue())
				{
					if (i >= argc - 1)
					{
						throw ProgramOptionsError(FormatString("Value expected for program option '-%s'", s + 1));
					}
					++i;

					OptionsPool * subpool = option->Set(argv[i]);
					if (subpool != 0)
					{
						pools.push_front(subpool);
					}

					if ((desc->flags & Option::PASSWORD) != 0)
					{
						for (char * t = const_cast<char *>(argv[i]); *t != '\0'; ++t)
						{
							*t = '*';
						}
					}
				}
				else
				{
					OptionsPool * subpool = option->Set(0);
					if (subpool != 0)
					{
						pools.push_front(subpool);
					}
				}
			}
		}

		for (std::list<OptionsPool *>::const_iterator itr = pools.begin();
		     itr != pools.end();
		     ++itr)
		{
			(*itr)->CheckRequiredOptions();
		}

		return true;
	}

	bool OptionStringToInteger(const char * s, long long & i)
	{
		const char * p = s;

		bool neg = false;
		if (*p == '-')
		{
			neg = true;
			++p;
		}
		else if (*p == '+')
		{
			++p;
		}

		i = 0;
		for (; *p != '\0'; ++p)
		{
			if (*p < '0' || *p > '9')
			{
				if (neg)
				{
					i = -i;
				}
				return false;
			}
			i = (i << 3) + (i << 1) + (*p - '0');
		}

		if (neg)
		{
			i = -i;
		}

		return true;
	}
}
