#include "MySqlResult.hpp"
#include <xtl/StringUtils.hpp>
#include <string.h>

namespace XTL
{
	MySqlResult::MySqlResult()
		: result_(0), columns_()
	{
		;;
	}

	MySqlResult::MySqlResult(MYSQL_RES * result)
		: result_(result), columns_()
	{
		Init();
	}

	MySqlResult::~MySqlResult()
	{
		Free();
	}

	MySqlResult & MySqlResult::operator= (MYSQL_RES * result)
	{
		if (result_ != result)
		{
			Free();
			result_ = result;
			Init();
		}
		return *this;
	}

	void MySqlResult::Init()
	{
		if (result_ != 0)
		{
			MYSQL_FIELD * field;
			while ((field = ::mysql_fetch_field(result_)) != 0)
			{
				columns_.push_back(field->name);
			}
		}
	}

	void MySqlResult::Free()
	{
		if (result_ != 0)
		{
			::mysql_free_result(result_);
			result_ = 0;
			columns_.clear();
		}
	}

	const bool MySqlResult::IsNull() const
	{
		return result_ == 0;
	}

	const unsigned int MySqlResult::RowsCount() const
	{
		return ::mysql_num_rows(result_);
	}

	const unsigned int MySqlResult::ColumnsCount() const
	{
		return columns_.size();
	}

	const std::string MySqlResult::ColumnName(unsigned int index) const
	{
		return columns_[index];
	}

	const bool MySqlResult::Fetch(MySqlRow & row)
	{
		row.Clear();

		MYSQL_ROW r = ::mysql_fetch_row(result_);
		if (r == NULL)
		{
			return false;
		}

		// TODO: check field type
		for (unsigned int i = 0; i < columns_.size(); ++i)
		{
			row.PushBack(r[i]);
		}

		return true;
	}


	MySqlRow::MySqlRow()
		: values_()
	{
		;;
	}

	MySqlRow::~MySqlRow() throw()
	{
		Clear();
	}

	void MySqlRow::Clear()
	{
		for (std::vector<char *>::iterator itr = values_.begin(); itr != values_.end(); ++itr)
		{
			delete [] *itr;
		}

		values_.clear();
	}

	const bool MySqlRow::Empty() const
	{
		return values_.empty();
	}

	const unsigned int MySqlRow::Size() const
	{
		return values_.size();
	}

	const bool MySqlRow::IsNull(unsigned int index) const
	{
		return values_[index] == 0;
	}

	const long long MySqlRow::Integer(unsigned int index) const
	{
		return IsNull(index) ? 0 : StringToInteger(values_[index]);
	}

	const std::string MySqlRow::String(unsigned int index) const
	{
		return IsNull(index) ? "" : values_[index];
	}

	const std::string MySqlRow::operator[] (unsigned int index) const
	{
		return String(index);
	}

	void MySqlRow::PushBack(const char * s)
	{
		if (s == 0)
		{
			values_.push_back(0);
		}
		else
		{
			char * tmp = new char[::strlen(s) + 1];
			strcpy(tmp, s);
			values_.push_back(tmp);
		}
	}
}
