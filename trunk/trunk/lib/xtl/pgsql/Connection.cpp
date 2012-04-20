#include "Connection.hpp"

#include <libpq-fe.h>

//#include "QueryParameters.hpp"
#include "QueryResult.hpp"
#include "Exception.hpp"
#include "../FormatString.hpp"

#define HANDLE_ static_cast<PGconn *>(handle_)

namespace XTL
{
namespace PGSQL
{
	Connection::Connection(const std::string & connectionString)
		: handle_(0),
		  connectionString_(connectionString)
	{
		;;
	}

	Connection::Connection(const ConnectionConfig & config)
		: handle_(0),
		  connectionString_(config.ConnectionString())
	{
		;;
	}

	Connection::~Connection() throw()
	{
		Close();
	}

	const std::string Connection::ConnectionString() const
	{
		return connectionString_;
	}

	void Connection::Open()
	{
		Close();
		handle_ = ::PQconnectdb(connectionString_.c_str());
		if (PQstatus(HANDLE_) != CONNECTION_OK)
		{
			throw ConnectError(LastError());
		}
	}

	void Connection::Close()
	{
		if (handle_ != 0)
		{
			PQfinish(HANDLE_);
			handle_ = 0;
		}
	}

	bool Connection::IsOpened() const
	{
		return handle_ != 0 && PQstatus(HANDLE_) == CONNECTION_OK;
	}

	const char * Connection::LastError() const
	{
		return PQerrorMessage(HANDLE_);
	}

	const std::string Connection::Database() const
	{
		return IsOpened() ? PQdb(HANDLE_) : "";
	}

	const std::string Connection::User() const
	{
		return IsOpened() ? PQuser(HANDLE_) : "";
	}

	QueryResult Connection::Execute(const char * query)
	{
		QueryResult result;
		if (query == 0)
		{
			return result;
		}

		if (IsOpened())
		{
			result.ResetHandle(PQexec(HANDLE_, query));
			if (result.Success())
			{
				return result;
			}

			if (IsOpened())
			{
				throw QueryError(LastError(), query, result);
			}
		}

		// TODO: Делать или нет автореконнект должен решать класс Connection.
		Open();
		result.ResetHandle(PQexec(HANDLE_, query));
		if (result.Failed())
		{
			throw QueryError(LastError(), query, result);
		}

		return result;
	}

	QueryResult Connection::Execute(const std::string & query)
	{
		return Execute(query.c_str());
	}

	void Connection::CopyTable(const char * tableName, CopyDataConsumer & dataConsumer)
	{
		const std::string query = XTL::FormatString("COPY %s TO STDOUT;", tableName);

		PGresult * result = PQexec(HANDLE_, query.c_str());

		if (PQresultStatus(result) != PGRES_COPY_OUT)
		{
			throw Exception(LastError());
		}

		char * buffer = 0;
		int copyResult = PQgetCopyData(HANDLE_, &buffer, 0);
		while (copyResult >= 0)
		{
			dataConsumer.OnRow(buffer, copyResult);

			PQfreemem(buffer);

			copyResult = PQgetCopyData(HANDLE_, &buffer, 0);
		}

		if (copyResult == -2)
		{
			throw Exception(LastError());
		}
	}

	bool Connection::BeginTransaction()
	{
		return Execute("BEGIN TRANSACTION;").Success();
	}

	bool Connection::CommitTransaction()
	{
		return Execute("COMMIT TRANSACTION;").Success();
	}

	bool Connection::RollbackTransaction()
	{
		return Execute("ROLLBACK TRANSACTION;").Success();
	}

/*
	QueryResult Connection::ExecutePrepared(const std::string     & name,
	                                        const QueryParameters & params)
	{
		QueryResult result(PQexecPrepared(
			handle_,
			name.c_str(),
			params.Size(),
			params.ArrayOfValues(),
			params.ArrayOfLengths(),
			0,
			0
		));

		return result;
	}

	bool Connection::Prepare(const std::string     & name,
	                         const std::string     & query,
	                         const QueryParameters & params)
	{
		if (!IsOpened())
		{
			return false;
		}

		PGresult * result = PQprepare (
			handle_,
			name.c_str(),
			query.c_str(),
			params.Size(),
			static_cast<const Oid *>(params.ArrayOfTypes())
		);

		bool success = (PQresultStatus(result) == PGRES_COMMAND_OK);

		PQclear(result);

		return success;
	}

	const std::string Connection::Escape(const std::string & value)
	{
		if (!value.empty())
		{
			if (IsOpened() || Open())
			{
				std::vector<char> out(value.size() * 2 + 1);
				int err = 0;
				const int result = ::PQescapeStringConn(handle_, &*out.begin(), value.c_str(), value.size(), &err);
				if (0 < result && 0 == err)
				{
					std::vector<char>::iterator it = out.begin() + result;
					return std::string(out.begin(), it);
				}
			}
		}
		return std::string();
	}
*/
/*
	const long long int PostgreSqlConnection::SelectInteger(const std::string & query,
	                                                        long long int       errorValue,
	                                                        unsigned int        attempts,
	                                                        unsigned int        pauseMicroSecs)
	{
		PostgreSqlResult result = Execute(query, attempts, pauseMicroSecs);
		return result.Failed() ? errorValue : result.GetField(0, 0).ToInteger(errorValue);
	}

	bool PostgreSqlConnection::Prepare(const char * name,
	                                   const char * sql,
	                                   const char * paramsSignature,
	                                   PostgreSqlQuery & query)
	{
		return query.Prepare(this, name, sql, paramsSignature);
	}

	bool PostgreSqlConnection::TableExists(const char * tableName, const char * tableSchema)
	{
		static XC3::StringTemplate queryCheckTable1("\
			SELECT count(*)\
			    FROM information_schema.tables\
			    WHERE table_schema='$table_schema' and table_name='$table_name';"
		);

		static XC3::StringTemplate queryCheckTable2("\
			SELECT count(*)\
			    FROM information_schema.tables\
			    WHERE table_name='$table_name';"
		);

		XC3::StringTemplate & query = (tableSchema != 0 ? queryCheckTable1 : queryCheckTable2);
		if (tableSchema != 0)
		{
			query["table_schema"] = tableSchema;
		}
		query["table_name"] = tableName;

		return SelectInteger(query.Evaluate(), -1) > 0;
	}

	bool PostgreSqlTransaction::Commit() throw()
	{
		PostgreSqlConnection * tmp(dbc_);
		dbc_ = 0;
		try
		{
			return tmp->CommitTransaction();
		}
		catch (...)
		{
			return false;
		}
	}
*/
}
}

#undef HANDLE_

