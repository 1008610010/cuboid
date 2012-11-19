#include "QueryResult.hpp"

#include <sqlite3.h>

#include "Exception.hpp"
#include "Database.hpp"

#define STMT_ static_cast<sqlite3_stmt *>(stmt_)

namespace XTL
{
namespace SQLITE
{
	QueryResult::QueryResult(Database * db, void * stmt)
		: db_(db),
		  stmt_(stmt)
	{
		;;
	}

	QueryResult::QueryResult(QueryResult & other)
		: db_(other.db_),
		  stmt_(other.stmt_)
	{
		other.stmt_ = 0;
	}

	QueryResult::QueryResult(Reference ref)
		: db_(ref.db_),
		  stmt_(ref.stmt_)
	{
		;;
	}

	QueryResult::~QueryResult() throw()
	{
		try
		{
			Finalize();
		}
		catch (const Exception & e)
		{
			;;
		}
	}

	QueryResult & QueryResult::operator= (QueryResult & other)
	{
		Finalize();

		db_ = other.db_;
		stmt_ = other.stmt_;
		other.stmt_ = 0;

		return *this;
	}

	QueryResult & QueryResult::operator= (Reference ref)
	{
		Finalize();

		db_ = ref.db_;
		stmt_ = ref.stmt_;

		return *this;
	}

	QueryResult::operator Reference()
	{
		Reference ref(db_, stmt_);
		stmt_ = 0;
		return ref;
	}

	int QueryResult::ColumnsCount() const
	{
		return ::sqlite3_column_count(STMT_);
	}

	const char * QueryResult::ColumnName(unsigned int columnIndex) const
	{
		return ::sqlite3_column_name(STMT_, columnIndex);
	}

	bool QueryResult::Fetch()
	{
		int rc = ::sqlite3_step(STMT_);

		switch (rc)
		{
			case SQLITE_ROW  : return true;
			case SQLITE_DONE : return false;
		}

		db_->ThrowException();
		return false;
	}

	const char * QueryResult::GetText(unsigned int columnIndex) const
	{
		return reinterpret_cast<const char *>(::sqlite3_column_text(STMT_, columnIndex));
	}

	void QueryResult::Finalize()
	{
		if (stmt_ == 0)
		{
			return;
		}

		int rc = ::sqlite3_finalize(STMT_);

		if (rc != SQLITE_OK)
		{
			db_->ThrowException();
		}

		stmt_ = 0;
	}
}
}

#undef STMT_
