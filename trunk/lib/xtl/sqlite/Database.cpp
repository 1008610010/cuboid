#include "Database.hpp"

#include <sqlite3.h>

#include <xtl/Exception.hpp>

#include "Exception.hpp"

#define DB_ static_cast<sqlite3 *>(db_)

namespace XTL
{
namespace SQLITE
{
	Database::Database(const std::string & filePath)
		: filePath_(filePath),
		  db_(0)
	{
		;;
	}

	Database::~Database() throw()
	{
		Close();
	}

	const std::string & Database::FilePath() const
	{
		return filePath_;
	}

	void Database::Open()
	{
		Close();

		int rc = ::sqlite3_open(filePath_.c_str(), reinterpret_cast<sqlite3 **>(&db_));

		if (rc != SQLITE_OK)
		{
			Exception e(::sqlite3_errcode(DB_), ::sqlite3_errmsg(DB_));
			::sqlite3_close(DB_);
			db_ = 0;
			throw e;
		}
	}

	bool Database::Close()
	{
		if (db_ == 0)
		{
			return true;
		}

		int rc = ::sqlite3_close(DB_);
		if (rc == SQLITE_OK)
		{
			db_ = 0;
			return true;
		}

		return false;
	}

	int Database::Execute(const std::string & query)
	{
		if (db_ == 0)
		{
			throw ILLEGAL_OPERATION_ERROR("Database has not been opened");
		}

		char * errorMessage = 0;
		int rc = ::sqlite3_exec(DB_, query.c_str(), NULL, 0, &errorMessage);
		if (rc != SQLITE_OK)
		{
			QueryError e(::sqlite3_errcode(DB_), ::sqlite3_errmsg(DB_), query);
			::sqlite3_free(errorMessage);
			throw e;
		}

		return ::sqlite3_changes(DB_);
	}

	QueryResult Database::Select(const std::string & query)
	{
		if (db_ == 0)
		{
			throw ILLEGAL_OPERATION_ERROR("Database has not been opened");
		}

		sqlite3_stmt * stmt = 0;

		int rc = ::sqlite3_prepare_v2(DB_, query.c_str(), query.length() + 1, &stmt, 0);

		if (rc != SQLITE_OK)
		{
			ThrowQueryError(query);
		}

		return QueryResult(this, stmt);
	}

	void Database::ThrowException()
	{
		throw Exception(::sqlite3_errcode(DB_), ::sqlite3_errmsg(DB_));
	}

	void Database::ThrowQueryError(const std::string & query)
	{
		throw QueryError(::sqlite3_errcode(DB_), ::sqlite3_errmsg(DB_), query);
	}
}
}

#undef DB_
