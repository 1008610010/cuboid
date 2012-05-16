#include "QueryResult.hpp"

#include <stdlib.h>
#include <libpq-fe.h>

#include "QueryResultRow.hpp"
#include "../StringUtils.hpp"

#define HANDLE_       static_cast<PGresult *>(handle_)
#define CONST_HANDLE_ static_cast<const PGresult *>(handle_)

namespace XTL
{
namespace PGSQL
{
	QueryResult::QueryResult()
		: handle_(0)
	{
		;;
	}

	QueryResult::QueryResult(QueryResult & other)
		: handle_(other.handle_)
	{
		other.handle_ = 0;
	}

	QueryResult & QueryResult::operator= (QueryResult & other)
	{
		if (handle_ != other.handle_)
		{
			Clear();
			handle_ = other.handle_;
			other.handle_ = 0;
		}

		return *this;
	}

	QueryResult::QueryResult(void * handle)
		: handle_(handle)
	{
		;;
	}

	QueryResult::QueryResult(Reference ref)
		: handle_(ref.handle_)
	{
		;;
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
			Clear();
			handle_ = ref.handle_;
		}

		return *this;
	}

	QueryResult::~QueryResult() throw()
	{
		Clear();
	}

	void QueryResult::Clear()
	{
		if (handle_ != 0)
		{
			PQclear(HANDLE_);
			handle_ = 0;
		}
	}

	void QueryResult::ResetHandle(void * newHandle)
	{
		if (handle_ != newHandle)
		{
			Clear();
			handle_ = newHandle;
		}
	}

	bool QueryResult::Success() const
	{
		if (handle_ == 0)
		{
			return false;
		}

		ExecStatusType status = PQresultStatus(CONST_HANDLE_);
		/*
		if (status == PGRES_COPY_IN)
		{
			printf("PGRES_COPY_IN\n");
		}
		*/
		return status == PGRES_COMMAND_OK || status == PGRES_TUPLES_OK;
	}

	int QueryResult::RowsCount() const
	{
		return Success() ? PQntuples(CONST_HANDLE_) : 0;
	}

	int QueryResult::ColumnsCount() const
	{
		return Success() ? PQnfields(CONST_HANDLE_) : 0;
	}

	long long int QueryResult::Affected() const
	{
		const char * s = PQcmdTuples(HANDLE_);

		return s != 0 ? XTL::StringToInteger<long long int>(s) : 0;
	}

	const std::string QueryResult::ColumnName(unsigned int column) const
	{
		if (Success())
		{
			const char * name = PQfname(CONST_HANDLE_, column);
			if (name != 0)
			{
				return std::string(name);
			}
		}

		return std::string();
	}

	const char * QueryResult::GetSqlState() const
	{
		return handle_ != 0 ? PQresultErrorField(CONST_HANDLE_, PG_DIAG_SQLSTATE) : 0;
	}

	const char * QueryResult::GetValue(unsigned int row, unsigned int column) const
	{
		return PQgetvalue(CONST_HANDLE_, row, column);
	}

	const QueryResultRow QueryResult::GetRow(unsigned int row) const
	{
		return QueryResultRow(*this, row);
	}

	const std::string QueryResult::GetString(unsigned int row, unsigned int column) const
	{
		if (Failed())
		{
			return std::string();
		}

		const char * value = GetValue(row, column);
		return value == 0 ? std::string() : std::string(value);
	}

	char QueryResult::GetChar(unsigned int row, unsigned int column, char defaultValue) const
	{
		if (Failed())
		{
			return defaultValue;
		}

		const char * value = GetValue(row, column);
		return value == 0 ? defaultValue : value[0];
	}

	long long int QueryResult::GetInteger(unsigned int row, unsigned int column, long long int defaultValue) const
	{
		if (Failed())
		{
			return defaultValue;
		}

		const char * value = GetValue(row, column);
		return value == 0 ? defaultValue : XTL::StringToInteger<long long int>(value);
	}

	float QueryResult::GetFloat(unsigned int row, unsigned int column, float defaultValue) const
	{
		if (Failed())
		{
			return defaultValue;
		}

		const char * value = GetValue(row, column);
		return value == 0 ? defaultValue : XTL::StringToFloat<float>(value);
	}

	double QueryResult::GetDouble(unsigned int row, unsigned int column, double defaultValue) const
	{
		if (Failed())
		{
			return defaultValue;
		}

		const char * value = GetValue(row, column);
		return value == 0 ? defaultValue : XTL::StringToFloat<double>(value);
	}
}
}

#undef HANDLE_
#undef CONST_HANDLE_

