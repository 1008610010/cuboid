#include "Database.hpp"

#include <sqlite3.h>

#include <xtl/Exception.hpp>
#include <xtl/linux/Utils.hpp>

#include "Exception.hpp"

#define DB_ static_cast<sqlite3 *>(db_)

namespace XTL
{
namespace SQLITE
{
	namespace
	{
		static const double SQLITE_BUSY_DELAY = 0.001; // 1 ms

		static const int MODE_DEFAULT      = 0x0000;
		static const int MODE_WAIT_IF_BUSY = 0x0001;
	}

	const Database::Options Database::Options::DEFAULT      (MODE_DEFAULT);
	const Database::Options Database::Options::WAIT_IF_BUSY (MODE_WAIT_IF_BUSY);

	Database::Options::Options(unsigned int value)
		: value_(value)
	{
		;;
	}

	bool Database::Options::DoWaitIfBusy() const
	{
		return (value_ & MODE_WAIT_IF_BUSY) != 0;
	}


	Database::Database(const std::string & filePath, Options options)
		: filePath_(filePath),
		  db_(0),
		  options_(options)
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

	bool Database::WaitIfBusy()
	{
		return options_.DoWaitIfBusy();
	}

	void Database::BusySleep()
	{
		Sleep(SQLITE_BUSY_DELAY);
	}

	int Database::Execute(const std::string & query)
	{
		if (db_ == 0)
		{
			throw ILLEGAL_OPERATION_ERROR("Database has not been opened");
		}

		char * errorMessage = 0;

		while (true)
		{
			int rc = ::sqlite3_exec(DB_, query.c_str(), NULL, 0, &errorMessage);

			if (rc == SQLITE_BUSY && WaitIfBusy())
			{
				BusySleep();
				continue;
			}

			if (rc != SQLITE_OK)
			{
				QueryError e(::sqlite3_errcode(DB_), ::sqlite3_errmsg(DB_), query);
				::sqlite3_free(errorMessage);
				throw e;
			}

			return ::sqlite3_changes(DB_);
		}
	}

	int Database::AffectedRows()
	{
		if (db_ == 0)
		{
			throw ILLEGAL_OPERATION_ERROR("Database has not been opened");
		}

		return ::sqlite3_changes(DB_);
	}

	Statement Database::Prepare(const std::string & query)
	{
		if (db_ == 0)
		{
			throw ILLEGAL_OPERATION_ERROR("Database has not been opened");
		}

		sqlite3_stmt * stmt = 0;

		/*
			Здесь должен быть вызов:
			int rc = ::sqlite3_prepare_v2(DB_, query.c_str(), query.length() + 1, &stmt, 0);
			Но на CentOS 5.3 в библиотеке sqlite еще нет таких функций.
		*/
		int rc = ::sqlite3_prepare(DB_, query.c_str(), query.length() + 1, &stmt, 0);

		if (rc != SQLITE_OK)
		{
			ThrowQueryError(query);
		}

		return Statement(this, stmt);
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
