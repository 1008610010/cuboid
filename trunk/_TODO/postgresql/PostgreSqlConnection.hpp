#ifndef _XC3__POSTGRESQL_CONNECTION
#define _XC3__POSTGRESQL_CONNECTION 1

#include <libpq-fe.h>
#include "PostgreSqlQuery.hpp"
#include "PostgreSqlResult.hpp"
#include <stdexcept>

namespace XC3
{
	class PostgreSqlConnection
	{
		public:

			PostgreSqlConnection(const std::string & connectionInfo);

			~PostgreSqlConnection() throw();

			const std::string ConnectionInfo() const { return connectionInfo_; }

			bool Open();

			void Close();

			bool Opened() const;

			const char * LastError() const;

			const std::string Database() const;

			const std::string User() const;
			std::string Escape(const std::string & data);

			PostgreSqlResult Execute(const char * query, unsigned int attempts = 1, unsigned int pauseMicroSecs = 1000000);

			INT_64 SelectInteger(const CHAR_8 * query, INT_64 errorValue = 0, UINT_32 attempts = 1, UINT_32 pauseMicroSecs = 1000000);

			bool Prepare(const CHAR_8 * name,
			             const CHAR_8 * sql,
			             const CHAR_8 * paramsSignature,
			             PostgreSqlQuery & query);

			bool BeginTransaction();

			bool CommitTransaction();

			bool RollbackTransaction();

			bool TableExists(const char * tableName, const char * tableSchema = 0);

		private:

			friend class PostgreSqlQuery;

			PostgreSqlConnection(const PostgreSqlConnection &);
			PostgreSqlConnection & operator= (const PostgreSqlConnection &);

			PGconn * Handle()  { return handle_; }

			PGconn      * handle_;
			std::string   connectionInfo_;
	};

	class PostgreSqlTransaction
	{
	public:
		PostgreSqlTransaction & operator=(PostgreSqlTransaction & tr);

		PostgreSqlTransaction(PostgreSqlTransaction & tr);

		PostgreSqlTransaction(PostgreSqlConnection * conn);

		~PostgreSqlTransaction() throw();

		PostgreSqlConnection * operator->() { return conn_; }
		const PostgreSqlConnection * operator->() const { return conn_; }

		bool Commit() throw();
	private:
		PostgreSqlConnection * conn_;
	};

}

#endif
