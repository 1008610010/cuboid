#ifndef XTL_PGSQL__CONNECTION_HPP__
#define XTL_PGSQL__CONNECTION_HPP__ 1

#include <string>

#include "ConnectionConfig.hpp"
#include "QueryResult.hpp"

namespace XTL
{
namespace PGSQL
{
	class ConnectionConfig;
	class QueryParameters;
	class QueryResult;

	class Connection
	{
		public:

			/*
			 * "host=172.16.11.250 port=5432 dbname=stats user=stats"
			 */
			explicit Connection(const std::string & connectionString);

			explicit Connection(const ConnectionConfig & config);

			~Connection() throw();

			const std::string ConnectionString() const;

			void Open();

			/**
			 * Соединение автоматически закрывается в деструкторе.
			 */
			void Close();

			bool IsOpened() const;

			const char * LastError() const;

			const std::string Database() const;

			const std::string User() const;

			QueryResult Execute(const char * query);

			QueryResult Execute(const std::string & query);

			class CopyDataConsumer
			{
				public:

					virtual ~CopyDataConsumer() throw() { ;; }

					virtual void OnRow(const char * buffer, unsigned int size) = 0;
			};

			void CopyTable(const char * tableName, CopyDataConsumer & dataConsumer);

			bool BeginTransaction();

			bool CommitTransaction();

			bool RollbackTransaction();

//			QueryResult ExecutePrepared(const std::string      & name,
//			                            const QueryParameters & params);

			/**
			 * From postgresql doc:
			 *    The function creates a prepared statement named stmtName
			 *    from the query string, which must contain a single SQL
			 *    command. stmtName can be "" to create an unnamed statement,
			 *    in which case any pre-existing unnamed statement is
			 *    automatically replaced; otherwise it is an error if the
			 *    statement name is already defined in the current session.
			 */
//			bool Prepare(const std::string     & name,
//			             const std::string     & query,
//			             const QueryParameters & params);

//			const std::string Escape(const std::string & value);

		private:

			Connection(const Connection &);
			Connection & operator= (const Connection &);

			void * handle_; // На самом деле указатель PGconn *, заменен на void *, чтобы не включать libpq-fe.h в этот заголовочный файл
			const std::string connectionString_;

/*
			const long long int SelectInteger(const std::string & query,
			                                  long long int       errorValue = 0,
			                                  unsigned int        attempts = 1,
			                                  unsigned int        pauseMicroSecs = 1000000);

			bool TableExists(const char * tableName, const char * tableSchema = 0);
*/
	};
}
}

#endif
