#include "PostgreSqlResult.hpp"
#include <StringUtils.hpp>
#include <stdlib.h>

namespace XC3
{
	PostgreSqlResult::PostgreSqlResult()
		: handle_(0)
	{
		;;
	}

	PostgreSqlResult::PostgreSqlResult(PostgreSqlResult & result)
	{
		handle_ = result.handle_;
		result.handle_ = 0;
	}

	PostgreSqlResult::PostgreSqlResult(Reference ref)
		: handle_(ref.handle_)
	{
		;;
	}

	PostgreSqlResult::~PostgreSqlResult() throw()
	{
		Clear();
	}

	PostgreSqlResult & PostgreSqlResult::operator= (PostgreSqlResult & result)
	{
		if (handle_ != result.handle_)
		{
			Clear();
			handle_ = result.handle_;
			result.handle_ = 0;
		}
		return *this;
	}

	PostgreSqlResult & PostgreSqlResult::operator= (Reference ref)
	{
		if (handle_ != ref.handle_)
		{
			Clear();
			handle_ = ref.handle_;
		}
		return *this;
	}

	PostgreSqlResult::operator Reference() throw()
	{
		PGresult * temp = handle_;
		handle_ = 0;
		return temp;
	}

	void PostgreSqlResult::Clear()
	{
		if (handle_ != 0)
		{
			PQclear(handle_);
			handle_ = 0;
		}
	}

	bool PostgreSqlResult::Success() const
	{
		return handle_ != 0 &&
		       (PQresultStatus(handle_) == PGRES_COMMAND_OK ||
		        PQresultStatus(handle_) == PGRES_TUPLES_OK);
	}

	bool PostgreSqlResult::Failed() const
	{
		return handle_ == 0 ||
		       (PQresultStatus(handle_) != PGRES_COMMAND_OK &&
		        PQresultStatus(handle_) != PGRES_TUPLES_OK);
	}

	bool PostgreSqlResult::Empty() const
	{
		return RowsCount() == 0;
	}

	unsigned int PostgreSqlResult::RowsCount() const
	{
		return Failed() ? 0 : PQntuples(handle_);
	}

	unsigned int PostgreSqlResult::ColumnsCount() const
	{
		return Failed() ? 0 : PQnfields(handle_);
	}

	const std::string PostgreSqlResult::ColumnName(unsigned int column) const
	{
		if (Failed())
		{
			return std::string();
		}

		char * name = PQfname(handle_, column);
		return name == 0 ? std::string() : std::string(name);
	}

	long long int PostgreSqlResult::Affected() const
	{
		return strtoll(PQcmdTuples(handle_), NULL, 10);
	}

	const char * PostgreSqlResult::Status() const
	{
		return handle_ == 0 ? 0 : PQresultErrorField(handle_, PG_DIAG_SQLSTATE);
	}

	const std::string PostgreSqlResult::GetString(unsigned int row, unsigned int column)
	{
		return Failed() ? std::string() : std::string(PQgetvalue(handle_, row, column));
	}

	int64_t PostgreSqlResult::GetBigInt(unsigned int row, unsigned int column, int64_t defaultValue)
	{
		return Failed() ? defaultValue : StrToInt(PQgetvalue(handle_, row, column));
	}

	PostgreSqlResult::PostgreSqlResult(PGresult * handle)
		: handle_(handle)
	{
		;;
	}

	PostgreSqlResult & PostgreSqlResult::operator= (PGresult * handle)
	{
		if (handle_ != handle)
		{
			Clear();
			handle_ = handle;
		}
		return *this;
	}
}
