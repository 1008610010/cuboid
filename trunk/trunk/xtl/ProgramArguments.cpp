#include "ProgramArguments.hpp"
#include "StringUtils.hpp"

namespace XTL
{
	const std::string OptionDesc::AsString() const
	{
		if (label1 != 0)
		{
			return std::string("-") + label1;
		}
		else if (label2 != 0)
		{
			return std::string("--") + label2;
		}
		else
		{
			return std::string();
		}
	}

	ArgumentsPool::ArgumentsPool()
		: nextId_(0),
		  set_(0),
		  options_()
	{
		;;
	}

	void ArgumentsPool::AddOption(const OptionDesc * desc, ArgumentBase * arg)
	{
		if (desc->label1 != 0 && FindByLabel1(desc->label1) != options_.end())
		{
			throw ProgramOptionsError(FormatString("Internal logic error: doubled option '-%s'", desc->label1));
		}

		if (desc->label2 != 0 && FindByLabel2(desc->label2) != options_.end())
		{
			throw ProgramOptionsError(FormatString("Internal logic error: doubled options '--%s'", desc->label2));
		}

		options_.push_back(std::make_pair(desc, arg));
	}
	
	const unsigned long long ArgumentsPool::NextId()
	{
		if (nextId_ >= 9223372036854775808llu)
		{
			throw ProgramOptionsError("Internal logic error: too many options in the same pool");
		}

		return nextId_ = (nextId_ > 0 ? (nextId_ << 1) : 1);
	}

	void ArgumentsPool::SetPresent(const unsigned long long & id)
	{
		set_ |= id;
	}

	bool ArgumentsPool::Contains(const unsigned long long & id)
	{
		return (set_ & id) != 0;
	}

	const ArgumentsPool::ListOfOptions::iterator ArgumentsPool::FindByLabel1(const char * label1)
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

	const ArgumentsPool::ListOfOptions::iterator ArgumentsPool::FindByLabel2(const char * label2)
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

	void ArgumentsPool::CheckRequiredOptions()
	{
		for (ListOfOptions::const_iterator itr = options_.begin();
		     itr != options_.end();
		     ++itr)
		{
			if ((itr->first->flags & ArgumentBase::REQUIRED) != 0 &&
			    !Contains(itr->second->Id()))
			{
				throw ProgramOptionsError(FormatString("Required program option '%s' is not specified", itr->first->AsString().c_str()));
			}
		}
	}

	bool ArgumentsPool::Parse(int argc, const char * argv[])
	{
		if (argc == 0)
		{
			CheckRequiredOptions();
		}

		std::list<ArgumentsPool *> pools;
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
			ArgumentBase * arg = 0;

			if (s[1] == '-')
			{
				const char * t = ::strchr(s + 2, '=');
				if (t == 0)
				{
					for (std::list<ArgumentsPool *>::iterator itr = pools.begin();
					     itr != pools.end();
					     ++itr)
					{
						ListOfOptions::iterator opt = (*itr)->FindByLabel2(s + 2);
						if (opt != options_.end())
						{
							desc = opt->first;
							arg = opt->second;
							break;
						}
					}

					if (desc == 0)
					{
						throw ProgramOptionsError(FormatString("Invalid program option: '--%s'", s + 2));
					}

					if (arg->NeedValue())
					{
						throw ProgramOptionsError(FormatString("Value expected for program option '--%s'", s + 2));
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

					for (std::list<ArgumentsPool *>::iterator itr = pools.begin();
					     itr != pools.end();
					     ++itr)
					{
						ListOfOptions::iterator opt = (*itr)->FindByLabel2(label2.c_str());
						if (opt != options_.end())
						{
							desc = opt->first;
							arg = opt->second;
							break;
						}
					}

					if (desc == 0)
					{
						throw ProgramOptionsError(FormatString("Invalid program option '--%s'", label2.c_str()));
					}

					if ((desc->flags & ArgumentBase::PASSWORD) != 0)
					{
						for (++t; *t != '\0'; ++t)
						{
							*const_cast<char *>(t) = '*';
						}
					}

					if (!arg->NeedValue())
					{
						throw ProgramOptionsError(FormatString("Unexpected value for program option '--%s'", label2.c_str()));
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
				for (std::list<ArgumentsPool *>::iterator itr = pools.begin();
				     itr != pools.end();
				     ++itr)
				{
					ListOfOptions::iterator opt = (*itr)->FindByLabel1(s + 1);
					if (opt != options_.end())
					{
						desc = opt->first;
						arg = opt->second;
						break;
					}
				}

				if (desc == 0)
				{
					throw ProgramOptionsError(FormatString("Invalid program option '-%s'", s + 1));
				}

				if (arg->NeedValue())
				{
					if (i >= argc - 1)
					{
						throw ProgramOptionsError(FormatString("Value expected for program option '-%s'", s + 1));
					}
					++i;

					ArgumentsPool * subpool = arg->Set(argv[i]);
					if (subpool != 0)
					{
						pools.push_front(subpool);
					}

					if ((desc->flags & ArgumentBase::PASSWORD) != 0)
					{
						for (char * t = const_cast<char *>(argv[i]); *t != '\0'; ++t)
						{
							*t = '*';
						}
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

		for (std::list<ArgumentsPool *>::const_iterator itr = pools.begin();
		     itr != pools.end();
		     ++itr)
		{
			(*itr)->CheckRequiredOptions();
		}

		return true;
	}
}
