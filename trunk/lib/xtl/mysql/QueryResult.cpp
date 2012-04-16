#include "QueryResult.hpp"

#include <string.h>
#include <mysql/mysql.h>

#include <xtl/StringUtils.hpp>

#include "QueryResultRow.hpp"

#include <stdio.h>

#define HANDLE_ static_cast<MYSQL_RES *>(handle_)

namespace XTL
{
namespace MYSQL
{
	QueryResult::QueryResult()
		: handle_(0)
	{
		;;
	}

	QueryResult::QueryResult(Reference ref)
		: handle_(ref.handle_)
	{
		;;
	}

	QueryResult::QueryResult(QueryResult & other)
		: handle_(other.handle_)
	{
		other.handle_ = 0;
	}

	QueryResult::~QueryResult() throw()
	{
		Free();
	}

	QueryResult::operator Reference()
	{
		void * temp = handle_;
		handle_ = 0;
		return temp;
	}

	QueryResult & QueryResult::operator= (Reference ref)
	{
		if (handle_ != ref.handle_)
		{
			Free();
			handle_ = ref.handle_;
		}

		return *this;
	}

	QueryResult & QueryResult::operator= (QueryResult & other)
	{
		if (handle_ != other.handle_)
		{
			Free();
			handle_ = other.handle_;
			other.handle_ = 0;
		}

		return *this;
	}

	void QueryResult::Free()
	{
		if (handle_ != 0)
		{
			::mysql_free_result(HANDLE_);
			handle_ = 0;
		}
	}

	bool QueryResult::IsNull() const
	{
		return handle_ == 0;
	}

	unsigned int QueryResult::RowsCount() const
	{
		return ::mysql_num_rows(HANDLE_);
	}

	unsigned int QueryResult::ColumnsCount() const
	{
		return ::mysql_num_fields(HANDLE_);
	}

	void QueryResult::GetColumnsNames(std::vector<std::string> & columns) const
	{
		if (handle_ != 0)
		{
			columns.resize(ColumnsCount());

			MYSQL_FIELD * field;
			unsigned int i = 0;
			while ((field = ::mysql_fetch_field(HANDLE_)) != 0 && i < columns.size())
			{
				columns[i] = field->name;
				++i;
			}
		}
	}

	bool QueryResult::Fetch(QueryResultRow & row)
	{
		row.Clear();

		MYSQL_ROW r = ::mysql_fetch_row(HANDLE_);

		if (r == NULL)
		{
			return false;
		}

		unsigned int columnsCount = ColumnsCount();

		// TODO: check field type
		for (unsigned int i = 0; i < columnsCount; ++i)
		{
			row.PushBack(r[i]);
		}

		return true;
	}
}
}

#undef HANDLE_

