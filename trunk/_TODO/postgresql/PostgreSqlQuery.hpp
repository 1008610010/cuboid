#ifndef _XC3__POSTGRESQL_QUERY_HPP__
#define _XC3__POSTGRESQL_QUERY_HPP__ 1

#include "PostgreSqlQueryParams.hpp"
#include "PostgreSqlResult.hpp"

namespace XC3
{
	class PostgreSqlConnection;

	class PostgreSqlQuery
	{
		public:

			PostgreSqlQuery();

			void Clear();

			bool Prepared() const { return connection_ != NULL; }

			const std::string Name() const { return name_; }

			const std::string Query() const { return query_; }

			/**
			From postgresql doc:
			    The function creates a prepared statement named stmtName
			    from the query string, which must contain a single SQL
			    command. stmtName can be "" to create an unnamed statement,
			    in which case any pre-existing unnamed statement is
			    automatically replaced; otherwise it is an error if the
			    statement name is already defined in the current session.
			*/
			bool Prepare(PostgreSqlConnection * connection,
			             const CHAR_8 * name,
			             const CHAR_8 * query,
			             const CHAR_8 * paramsSignature);

			bool Execute();

			PostgreSqlResult Select();

			void SetParam(UINT_32 index, INT_64 value);

			void SetParam(UINT_32 index, const std::string value);

		private:

			friend class PostgreSqlConnection;

			PostgreSqlConnection * connection_;
			PostgreSqlQueryParams  params_;
			std::string            name_;
			std::string            query_;
	};
}

#endif
