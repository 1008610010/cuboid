#include "PostgreSqlConnection.hpp"
#include <StringTemplate.hpp>
#include <libpq-fe.h>

namespace XC3
{
	PostgreSqlConnection::PostgreSqlConnection(const std::string & connectionInfo)
		: handle_(0),
		  connectionInfo_(connectionInfo)
	{
		;;
	}

	PostgreSqlConnection::~PostgreSqlConnection() throw()
	{
		Close();
	}

	bool PostgreSqlConnection::Open()
	{
		Close();
		handle_ = ::PQconnectdb(connectionInfo_.c_str());
		return PQstatus(handle_) == CONNECTION_OK;
	}

	void PostgreSqlConnection::Close()
	{
		if (handle_ != 0)
		{
			PQfinish(handle_);
			handle_ = 0;
		}
	}

	std::string PostgreSqlConnection::Escape(const std::string & data)
	{
		if (!data.empty())
		{
			if (Opened() || Open())
			{
				std::vector<char> out(data.size() * 2 + 1);
				int err(0);
				const int result (::PQescapeStringConn(handle_, &*out.begin(), data.c_str(), data.size(), &err));
				if (0 < result && 0 == err)
				{
					std::vector<char>::iterator it = out.begin() + result;
					return std::string(out.begin(), it);
				}
			}
		}
		return std::string();
	}

	bool PostgreSqlConnection::Opened() const
	{
		return handle_ != 0 && PQstatus(handle_) == CONNECTION_OK;
	}

	const char * PostgreSqlConnection::LastError() const
	{
		return PQerrorMessage(handle_);
	}

	const std::string PostgreSqlConnection::Database() const
	{
		return Opened() ? std::string(PQdb(handle_)) : "";
	}

	const std::string PostgreSqlConnection::User() const
	{
		return Opened() ? std::string(PQuser(handle_)) : "";
	}

	PostgreSqlResult PostgreSqlConnection::Execute(const char * query, unsigned int attempts, unsigned int pauseMicroSecs)
	{
		PostgreSqlResult result;
		if (query == 0)
		{
			return result;
		}

		if (Opened())
		{
			result = PQexec(handle_, query);
		}

		if (!result.Failed() || Opened())
		{
			return result;
		}

		for (; attempts > 0; --attempts)
		{
			if (Open())
			{
				result = PQexec(handle_, query);
				if (!result.Failed() || Opened())
				{
					return result;
				}
			}

			if (attempts > 1)
			{
				usleep(pauseMicroSecs);
			}
		}
		return result;
	}

	INT_64 PostgreSqlConnection::SelectInteger(const CHAR_8 * query,
	                                           INT_64  errorValue,
	                                           UINT_32 attempts,
	                                           UINT_32 pauseMicroSecs)
	{
		PostgreSqlResult result = Execute(query, attempts, pauseMicroSecs);
		return result.Failed() ? errorValue : result.GetBigInt(0, 0, errorValue);
	}

	bool PostgreSqlConnection::Prepare(const CHAR_8 * name,
	                                   const CHAR_8 * sql,
	                                   const CHAR_8 * paramsSignature,
	                                   PostgreSqlQuery & query)
	{
		return query.Prepare(this, name, sql, paramsSignature);
	}

	bool PostgreSqlConnection::BeginTransaction()
	{
		return Execute("BEGIN TRANSACTION;").Success();
	}

	bool PostgreSqlConnection::CommitTransaction()
	{
		return Execute("COMMIT TRANSACTION;").Success();
	}

	bool PostgreSqlConnection::RollbackTransaction()
	{
		return Execute("ROLLBACK TRANSACTION;").Success();
	}

	bool PostgreSqlConnection::TableExists(const char * tableName, const char * tableSchema)
	{
		static StringTemplate queryCheckTable1("\
			SELECT count(*)\
			    FROM information_schema.tables\
			    WHERE table_schema='$table_schema' and table_name='$table_name';"
		);
		static StringTemplate queryCheckTable2("\
			SELECT count(*)\
			    FROM information_schema.tables\
			    WHERE table_name='$table_name';"
		);

		StringTemplate & query = (tableSchema != 0 ? queryCheckTable1 : queryCheckTable2);
		if (tableSchema != 0)
		{
			query["table_schema"] = tableSchema;
		}
		query["table_name"] = tableName;

		return SelectInteger(query.Evaluate(), -1) > 0;
	}


	PostgreSqlTransaction & PostgreSqlTransaction::operator=(PostgreSqlTransaction & tr)
	{
		if( 0 == tr.conn_ )
		{
			throw std::runtime_error("PostgreSQL connection is NULL in PostgreSqlTransaction constructor");
		}
		else if (conn_ != tr.conn_)
		{
			conn_ = tr.conn_;
			tr.conn_ = 0;
		}
		return *this;
	}

	PostgreSqlTransaction::PostgreSqlTransaction(PostgreSqlTransaction & tr):
		conn_( tr.conn_ )
	{
		if( 0 == tr.conn_ )
		{
			throw std::runtime_error("PostgreSQL connection is NULL in PostgreSqlTransaction constructor");
		}
		tr.conn_ = 0;
	}

	PostgreSqlTransaction::PostgreSqlTransaction(PostgreSqlConnection * conn): conn_(conn)
	{
		if( 0 == conn_ )
		{
			throw std::runtime_error("PostgreSQL connection is NULL in PostgreSqlTransaction constructor");
		}
		else if (! conn_->BeginTransaction() )
		{
			throw std::runtime_error(std::string("Can't start PostgreSQL trnasaction: ") + conn_->LastError() );
		}
	}

	PostgreSqlTransaction::~PostgreSqlTransaction() throw()
	{
		try
		{
			if (0 != conn_)
			{
				 conn_->RollbackTransaction();
			}
		}
		catch (...) { ;; }
	}

	bool PostgreSqlTransaction::Commit() throw()
	{
		PostgreSqlConnection * tmp(conn_);
		conn_ = 0;
		try { return tmp->CommitTransaction(); } catch (...) { return false; }
	}
}
