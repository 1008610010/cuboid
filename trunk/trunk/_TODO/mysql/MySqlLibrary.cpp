#include "MySqlLibrary.hpp"
#include <mysql.h>
#include <string.h>
#include <stdexcept>

namespace XTL
{
	MySqlLibrary::MySqlLibrary(const std::string & appName, const std::string & configFile)
	{
		MySqlOptionList serverOptions;
		serverOptions << appName
		              << (std::string("--defaults-file=") + configFile)
		              << "--default-storage-engine=MYISAM"
		              << "--skip-grant-tables"
		              << "--bootstrap"
		              << "--external-locking"
		              << "--skip-innodb"
		;

		MySqlOptionList serverGroups;
		serverGroups << appName;

		int r = ::mysql_library_init(
			serverOptions.Size(),
			CStringArray(serverOptions.ToArray()),
			CStringArray(serverGroups.ToArray())
		);

		if (r != 0)
		{
			throw std::runtime_error(std::string("mysql_library_init: ") + ::mysql_error(0));
		}
	}

	MySqlLibrary::~MySqlLibrary() throw()
	{
		::mysql_library_end();
	}

	unsigned int MySqlOptionList::Size() const
	{
		return values_.size();
	}

	MySqlOptionList & MySqlOptionList::operator<< (const char * v)
	{
		values_.push_back(v);
		return *this;
	}

	MySqlOptionList & MySqlOptionList::operator<< (const std::string & v)
	{
		values_.push_back(v);
		return *this;
	}

	char ** MySqlOptionList::ToArray() const
	{
		char ** array = new char *[values_.size() + 1];

		unsigned int i = 0;
		for (std::list<std::string>::const_iterator itr = values_.begin(); itr != values_.end(); ++i, ++itr)
		{
			array[i] = new char[itr->size() + 1];
			::strcpy(array[i], itr->c_str());
		}
		array[i] = 0;

		return array;
	}

	CStringArray::CStringArray(char ** values)
		: values_(values)
	{
		;;
	}

	CStringArray::~CStringArray() throw()
	{
		if (values_ != 0)
		{
			for (int i = 0; values_[i] != 0; ++i)
			{
				delete [] values_[i];
			}
			delete [] values_;
		}
	}
}
