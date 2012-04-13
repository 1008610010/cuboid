#include "PostgreSqlQuery.hpp"
#include "PostgreSqlConnection.hpp"

namespace XC3
{
	PostgreSqlQuery::PostgreSqlQuery()
		: connection_(NULL)
	{
		;;
	}

	void PostgreSqlQuery::Clear()
	{
		connection_ = NULL;
		params_.Destroy();
		name_.clear();
		query_.clear();
	}

	bool PostgreSqlQuery::Prepare(PostgreSqlConnection * connection,
	                              const CHAR_8 * name,
	                              const CHAR_8 * query,
	                              const CHAR_8 * paramsSignature)
	{
		Clear();
		if (!connection->Opened() || name == NULL || query == NULL || paramsSignature == NULL)
		{
			return false;
		}
		connection_ = connection;
		params_.Create(paramsSignature);
		PGresult * result = PQprepare(connection_->Handle(), name, query, params_.Size(), params_.Types());
		bool success = (PQresultStatus(result) == PGRES_COMMAND_OK);
		PQclear(result);
		if (success)
		{
			name_ = name;
			query_ = query;
		}
		else
		{
			Clear();
		}
		return success;
	}

	bool PostgreSqlQuery::Execute()
	{
		if (!Prepared())
		{
			return false;
		}
		params_.UpdateValues();
		PGresult * result = PQexecPrepared(
			connection_->Handle(),
			name_.c_str(),
			params_.Size(),
			params_.Values(),
			params_.Lengths(),
			params_.Formats(),
			0
		);
		bool success = (PQresultStatus(result) == PGRES_COMMAND_OK) ||
		               (PQresultStatus(result) == PGRES_TUPLES_OK);
		PQclear(result);
		return success;
	}

	PostgreSqlResult PostgreSqlQuery::Select()
	{
		PostgreSqlResult result;
		if (!Prepared())
		{
			return result;
		}
		params_.UpdateValues();
		result = PQexecPrepared(
			connection_->Handle(),
			name_.c_str(),
			params_.Size(),
			params_.Values(),
			params_.Lengths(),
			params_.Formats(),
			0
		);
		return result;
	}

	void PostgreSqlQuery::SetParam(UINT_32 index, INT_64 value)
	{
		params_.Set(index, value);
	}

	void PostgreSqlQuery::SetParam(UINT_32 index, const std::string value)
	{
		params_.Set(index, value);
	}
}
