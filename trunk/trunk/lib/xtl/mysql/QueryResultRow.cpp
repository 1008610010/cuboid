#include "QueryResultRow.hpp"

#include <string.h>

namespace XTL
{
namespace MYSQL
{
	QueryResultRow::QueryResultRow()
		: values_()
	{
		;;
	}

	QueryResultRow::~QueryResultRow() throw()
	{
		Clear();
	}

	void QueryResultRow::Clear()
	{
		for (std::vector<char *>::iterator itr = values_.begin(); itr != values_.end(); ++itr)
		{
			delete [] *itr;
		}

		values_.clear();
	}

	const bool QueryResultRow::IsEmpty() const
	{
		return values_.empty();
	}

	const unsigned int QueryResultRow::Size() const
	{
		return values_.size();
	}

	const bool QueryResultRow::IsNull(unsigned int index) const
	{
		return values_[index] == 0;
	}

	const std::string QueryResultRow::GetString(unsigned int index) const
	{
		return IsNull(index) ? "" : values_[index];
	}

	const long long QueryResultRow::GetInteger(unsigned int index) const
	{
		return IsNull(index) ? 0 : StringToInteger(values_[index]);
	}

	void QueryResultRow::PushBack(const char * s)
	{
		if (s == 0)
		{
			values_.push_back(0);
		}
		else
		{
			char * tmp = new char[::strlen(s) + 1];
			::strcpy(tmp, s);
			values_.push_back(tmp);
		}
	}
}
}

