#include "QueryResult.hpp"

#include <string.h>

#include <xtl/StringUtils.hpp>

#include "QueryResultRow.hpp"

namespace XTL
{
namespace MYSQL
{
	QueryResult::QueryResult()
		: result_(0), columns_()
	{
		;;
	}

	QueryResult::QueryResult(MYSQL_RES * result)
		: result_(result), columns_()
	{
		Init();
	}

	QueryResult::~QueryResult() throw()
	{
		Free();
	}

	QueryResult & QueryResult::operator= (MYSQL_RES * result)
	{
		if (result_ != result)
		{
			Free();
			result_ = result;
			Init();
		}
		return *this;
	}

	void QueryResult::Init()
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

	void QueryResult::Free()
	{
		if (result_ != 0)
		{
			::mysql_free_result(result_);
			result_ = 0;
			columns_.clear();
		}
	}

	bool QueryResult::IsNull() const
	{
		return result_ == 0;
	}

	unsigned int QueryResult::RowsCount() const
	{
		return ::mysql_num_rows(result_);
	}

	unsigned int QueryResult::ColumnsCount() const
	{
		return columns_.size();
	}

	const std::string QueryResult::ColumnName(unsigned int index) const
	{
		return columns_[index];
	}

	bool QueryResult::Fetch(QueryResultRow & row)
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
}
}

