#include "Connection.hpp"

#include <libpq-fe.h>

#include <vector>

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

		for (int i = 0; i < 2; ++i)
		{
			if (!IsOpened())
			{
				// TODO: Делать или нет автореконнект должен решать класс Connection.
				Open();
			}

			result.ResetHandle(PQexec(HANDLE_, query));
			if (result.Success())
			{
				return result;
			}

			if (IsOpened())
			{
				break;
			}
		}

		throw QueryError(LastError(), query, result);
	}

	QueryResult Connection::Execute(const char * query, const QueryParameters & params)
	{
		QueryResult result;
		if (query == 0)
		{
			return result;
		}

		for (int i = 0; i < 2; ++i)
		{
			if (!IsOpened())
			{
				// TODO: Делать или нет автореконнект должен решать класс Connection.
				Open();
			}

			result.ResetHandle(PQexecParams(HANDLE_, query, params.Size(), static_cast<const Oid *>(params.Types()), params.Values(), NULL, NULL, 0));
			if (result.Success())
			{
				return result;
			}

			if (IsOpened())
			{
				break;
			}
		}

		throw QueryError(LastError(), query, result);
	}

	/*
	QueryResult Connection::Prepare(const std::string & name, const std::string & query)
	{
		QueryResult result;
		for (int i = 0; i < 2; ++i)
		{
			if (!IsOpened())
			{
				// TODO: Делать или нет автореконнект должен решать класс Connection.
				Open();
			}

			result.ResetHandle(PQprepare(HANDLE_, name.c_str(), query.c_str(), 0, NULL));
			if (result.Success())
			{
				return result;
			}

			if (IsOpened())
			{
				break;
			}
		}

		throw QueryError(LastError(), query.c_str(), result);
	}

	QueryResult Connection::ExecutePrepared(const std::string & name, const QueryParameters & params)
	{
		QueryResult result;

		for (int i = 0; i < 2; ++i)
		{
			if (!IsOpened())
			{
				// TODO: Делать или нет автореконнект должен решать класс Connection.
				Open();
			}

			result.ResetHandle(PQexecPrepared(HANDLE_, name.c_str(), params.Size(), params.Values(), NULL, NULL, 0));
			if (result.Success())
			{
				return result;
			}

			if (IsOpened())
			{
				break;
			}
		}

		throw QueryError(LastError(), query.c_str(), result);
	}
	*/

	QueryResult Connection::Execute(const std::string & query)
	{
		return Execute(query.c_str());
	}

	const std::string Connection::SelectString(const std::string & query, const std::string & defaultValue)
	{
		QueryResult result = Execute(query);
		return result.Empty() ? defaultValue : result.GetString(0, 0);
	}

	const long long int Connection::SelectLongLong(const std::string & query, const long long int & defaultValue)
	{
		QueryResult result = Execute(query);
		return result.Empty() ? defaultValue : result.GetLongLong(0, 0, defaultValue);
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

	void Connection::BeginTransaction()
	{
		Execute("BEGIN TRANSACTION;");
	}

	void Connection::CommitTransaction()
	{
		Execute("COMMIT TRANSACTION;");
	}

	void Connection::RollbackTransaction()
	{
		Execute("ROLLBACK TRANSACTION;");
	}

	const std::string Connection::Escape(const std::string & value)
	{
		if (value.empty())
		{
			return std::string();
		}

		if (!IsOpened())
		{
			throw Exception("Connection was not opened");
		}

		std::vector<char> result(value.size() * 2 + 1);
		int err = 0;
		const int length = ::PQescapeStringConn(HANDLE_, &(result[0]), value.c_str(), value.size(), &err);
		if (length > 0 && err == 0)
		{
			return std::string(result.begin(), result.begin() + length);
		}
		else
		{
			throw Exception("Error in PQescapeStringConn");
		}
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


*/
/*
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
