#include "ProgramOptions.hpp"
#include "StringUtils.hpp"
#include <string.h> // strchr, strcmp

namespace XTL
{
	const std::string OptionDesc::AsString() const
	{
		if (label1 != 0)
		{
			if (label2 != 0)
			{
				if (value2 != 0)
				{
					return std::string("-") + label1 + " " + value2 + ", --" + label2 + "=" + value2;
				}
				else
				{
					return std::string("-") + label1 + ", --" + label2;
				}
			}
			else
			{
				if (value2 != 0)
				{
					return std::string("-") + label1 + " " + value2;
				}
				else
				{
					return std::string("-") + label1;
				}
			}
		}
		else if (label2 != 0)
		{
			if (value2 != 0)
			{
				return std::string("--") + label2 + "=" + value2;
			}
			else
			{
				return std::string("--") + label2;
			}
		}
		else
		{
			return text;
		}
	}

	bool OptionDesc::Required() const
	{
		return (flags & Option::REQUIRED) != 0;
	}

	void OptionValueString::Set(const OptionDesc * desc, const char * s)
	{
		this->value_.assign(s);
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

	void OptionValueInteger::Set(const OptionDesc * desc, const char * s)
	{
		if (!OptionStringToInteger(s, this->value_))
		{
			throw ProgramOptionsError(std::string("Invalid option's integer value '") + s + "'");
		}
	}

	OptionsPool::OptionsPool()
		: options_()
	{
		;;
	}

	void OptionsPool::RegisterOption(const OptionDesc * desc, Option * option, OptionValue * value, unsigned int flags)
	{
//		fprintf(stderr, "AddOption: %s to %p\n", desc->AsString().c_str(), (void *) this);
		if (desc->label1 != 0 && FindByLabel1(desc->label1) != 0)
		{
			throw ProgramOptionsError(FormatString("Internal logic error: duplicate option '-%s'", desc->label1));
		}

		if (desc->label2 != 0 && FindByLabel2(desc->label2) != 0)
		{
			throw ProgramOptionsError(FormatString("Internal logic error: duplicate options '--%s'", desc->label2));
		}

		options_.push_back(OptionEntry(desc, option, value, flags));
	}

	OptionsPool::OptionEntry * OptionsPool::FindByLabel1(const char * label1)
	{
		for (ListOfOptions::iterator itr = options_.begin();
		     itr != options_.end();
		     ++itr)
		{
			if (itr->desc->label1 != 0 && ::strcmp(itr->desc->label1, label1) == 0)
			{
				return &(*itr);
			}
		}
		return 0;
	}

	OptionsPool::OptionEntry * OptionsPool::FindByLabel2(const char * label2)
	{
		for (ListOfOptions::iterator itr = options_.begin();
		     itr != options_.end();
		     ++itr)
		{
			if (itr->desc->label2 != 0 && ::strcmp(itr->desc->label2, label2) == 0)
			{
				return &(*itr);
			}
		}
		return 0;
	}

	void OptionsPool::CheckRequiredOptions()
	{
		for (ListOfOptions::const_iterator itr = options_.begin();
		     itr != options_.end();
		     ++itr)
		{
//			fprintf(stderr, "%s\n", itr->desc->AsString().c_str());

			if (!itr->SkipCheck() && itr->desc->Required() && !itr->option->Exists())
			{
				throw ProgramOptionsError(FormatString("Program option '%s' is required", itr->desc->AsString().c_str()));
			}
		}
	}

	void OptionsPool::Parse(int argc, const char * argv[], std::string & programName)
	{
		if (argc == 0)
		{
			CheckRequiredOptions();
		}

		std::list<OptionsPool *> pools;
		pools.push_back(this);

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

			OptionEntry * entry = 0;

			if (s[1] == '-')
			{
				const char * t = ::strchr(s + 2, '=');
				if (t == 0)
				{
					for (std::list<OptionsPool *>::iterator itr = pools.begin();
					     entry == 0 && itr != pools.end();
					     ++itr)
					{
						entry = (*itr)->FindByLabel2(s + 2);
					}

					if (entry == 0)
					{
						throw ProgramOptionsError(FormatString("Invalid program option: '--%s'", s + 2));
					}

					if (entry->value != 0)
					{
						if (entry->value->NeedValue())
						{
							throw ProgramOptionsError(FormatString("Value expected for program option '--%s'", s + 2));
						}
						else
						{
							entry->value->Set(entry->desc, 0);
						}
					}
				}
				else
				{
					std::string label2 = std::string(s + 2, t - (s + 2));

					for (std::list<OptionsPool *>::iterator itr = pools.begin();
					     entry == 0 && itr != pools.end();
					     ++itr)
					{
						entry = (*itr)->FindByLabel2(label2.c_str());
					}

					if (entry == 0)
					{
						throw ProgramOptionsError(FormatString("Invalid program option '--%s'", label2.c_str()));
					}

					if (entry->value == 0 || !entry->value->NeedValue())
					{
						throw ProgramOptionsError(FormatString("Unexpected value for program option '--%s'", label2.c_str()));
					}

					entry->value->Set(entry->desc, t + 1);

					if ((entry->desc->flags & Option::PASSWORD) != 0)
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
				     entry == 0 && itr != pools.end();
				     ++itr)
				{
					entry = (*itr)->FindByLabel1(s + 1);
				}

				if (entry == 0)
				{
					throw ProgramOptionsError(FormatString("Invalid program option '-%s'", s + 1));
				}

				if (entry->value != 0)
				{
					if (entry->value->NeedValue())
					{
						if (i >= argc - 1)
						{
							throw ProgramOptionsError(FormatString("Value expected for program option '-%s'", s + 1));
						}

						++i;

						entry->value->Set(entry->desc, argv[i]);

						if ((entry->desc->flags & Option::PASSWORD) != 0)
						{
							for (char * t = const_cast<char *>(argv[i]); *t != '\0'; ++t)
							{
								*t = '*';
							}
						}
					}
					else
					{
						entry->value->Set(entry->desc, 0);
					}
				}
			}

			OptionsPool * subpool = entry->option->Set();
			if (subpool != 0)
			{
				pools.push_front(subpool);
			}
		}

		for (std::list<OptionsPool *>::const_iterator itr = pools.begin();
		     itr != pools.end();
		     ++itr)
		{
			(*itr)->CheckRequiredOptions();
		}
	}
}
