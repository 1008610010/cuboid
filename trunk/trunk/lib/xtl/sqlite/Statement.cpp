#include "Statement.hpp"

#include <sqlite3.h>

#include "Exception.hpp"
#include "Database.hpp"

#define STMT_ static_cast<sqlite3_stmt *>(stmt_)

namespace XTL
{
namespace SQLITE
{
	Statement::Statement(Database * db, void * stmt)
		: db_(db),
		  stmt_(stmt)
	{
		;;
	}

	Statement::Statement(Statement & other)
		: db_(other.db_),
		  stmt_(other.stmt_)
	{
		other.stmt_ = 0;
	}

	Statement::Statement(Reference ref)
		: db_(ref.db_),
		  stmt_(ref.stmt_)
	{
		;;
	}

	Statement::~Statement() throw()
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

	Statement & Statement::operator= (Statement & other)
	{
		Finalize();

		db_ = other.db_;
		stmt_ = other.stmt_;
		other.stmt_ = 0;

		return *this;
	}

	Statement & Statement::operator= (Reference ref)
	{
		Finalize();

		db_ = ref.db_;
		stmt_ = ref.stmt_;

		return *this;
	}

	Statement::operator Reference()
	{
		Reference ref(db_, stmt_);
		stmt_ = 0;
		return ref;
	}

	int Statement::ColumnsCount() const
	{
		return ::sqlite3_column_count(STMT_);
	}

	const char * Statement::ColumnName(unsigned int columnIndex) const
	{
		return ::sqlite3_column_name(STMT_, columnIndex);
	}

	void Statement::BindNull(int paramIndex)
	{
		int rc = ::sqlite3_bind_null(STMT_, paramIndex);
		if (rc != SQLITE_OK)
		{
			db_->ThrowException();
		}
	}

	void Statement::Bind(int paramIndex, int value)
	{
		int rc = ::sqlite3_bind_int(STMT_, paramIndex, value);
		if (rc != SQLITE_OK)
		{
			db_->ThrowException();
		}
	}

	void Statement::Bind(int paramIndex, unsigned int value)
	{
		Bind(paramIndex, static_cast<int>(value));
	}

	void Statement::Bind(int paramIndex, XTL::INT_64 value)
	{
		int rc = ::sqlite3_bind_int64(STMT_, paramIndex, value);
		if (rc != SQLITE_OK)
		{
			db_->ThrowException();
		}
	}

	void Statement::Bind(int paramIndex, double value)
	{
		int rc = ::sqlite3_bind_double(STMT_, paramIndex, value);
		if (rc != SQLITE_OK)
		{
			db_->ThrowException();
		}
	}

	void Statement::Bind(int paramIndex, const std::string & value)
	{
		int rc = ::sqlite3_bind_text(STMT_, paramIndex, value.data(), value.size(), SQLITE_TRANSIENT);
		if (rc != SQLITE_OK)
		{
			db_->ThrowException();
		}
	}

	void Statement::Reset()
	{
		::sqlite3_clear_bindings(STMT_);
		::sqlite3_reset(STMT_);
	}

	int Statement::Execute()
	{
		int rc = ::sqlite3_step(STMT_);

		if (rc != SQLITE_DONE && rc != SQLITE_ROW)
		{
			db_->ThrowException();
		}

		if (rc == SQLITE_DONE)
		{
			const int affected = db_->AffectedRows();
			Reset();
			return affected;
		}
		else
		{
			return 0;
		}
	}

	bool Statement::Fetch()
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

	const char * Statement::GetText(unsigned int columnIndex) const
	{
		return reinterpret_cast<const char *>(::sqlite3_column_text(STMT_, columnIndex));
	}

	void Statement::Finalize()
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
