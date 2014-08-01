#ifndef XTL__PGSQL__CONNECTION_HPP__
#define XTL__PGSQL__CONNECTION_HPP__ 1

#include <string>

#include "ConnectionConfig.hpp"
#include "QueryParameters.hpp"
#include "QueryResult.hpp"
#include "Types.hpp"

namespace XTL
{
namespace PGSQL
{
	class ConnectionConfig;

	class Connection
	{
		public:

			/*
			 * "host=172.16.11.250 port=5432 dbname=usersdb user=stats"
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

			QueryResult Execute(const char * query, const QueryParameters & params);

			QueryResult Execute(const std::string & query);

			template <typename T1>
			QueryResult Execute(const char * query, const T1 & p1);

			template <typename T1, typename T2>
			QueryResult Execute(const char * query, const T1 & p1, const T2 & p2);

			template <typename T1, typename T2, typename T3>
			QueryResult Execute(const char * query, const T1 & p1, const T2 & p2, const T3 & p3);

			template <typename T1, typename T2, typename T3, typename T4>
			QueryResult Execute(const char * query, const T1 & p1, const T2 & p2, const T3 & p3, const T4 & p4);

			template <typename T1, typename T2, typename T3, typename T4, typename T5>
			QueryResult Execute(const char * query, const T1 & p1, const T2 & p2, const T3 & p3, const T4 & p4, const T5 & p5);

			template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
			QueryResult Execute(const char * query, const T1 & p1, const T2 & p2, const T3 & p3, const T4 & p4, const T5 & p5, const T6 & p6);

			template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
			QueryResult Execute(const char * query, const T1 & p1, const T2 & p2, const T3 & p3, const T4 & p4, const T5 & p5, const T6 & p6, const T7 & p7);

			template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
			QueryResult Execute(const char * query, const T1 & p1, const T2 & p2, const T3 & p3, const T4 & p4, const T5 & p5, const T6 & p6, const T7 & p7, const T8 & p8);

			/**
			 * From postgresql doc:
			 *    The function creates a prepared statement named stmtName
			 *    from the query string, which must contain a single SQL
			 *    command. stmtName can be "" to create an unnamed statement,
			 *    in which case any pre-existing unnamed statement is
			 *    automatically replaced; otherwise it is an error if the
			 *    statement name is already defined in the current session.
			 */
			/*
			QueryResult Prepare(const std::string & name, const std::string & query);

			QueryResult ExecutePrepared(const std::string & name, const QueryParameters & params);
			*/

			const std::string SelectString(const std::string & query, const std::string & defaultValue = "");

			const long long int SelectLongLong(const std::string & query, const long long int & defaultValue = 0);

			class CopyDataConsumer
			{
				public:

					virtual ~CopyDataConsumer() throw() { ;; }

					/*
						The returned string is always null-terminated, though this is probably only useful for textual COPY.
					*/
					virtual void OnRow(const char * buffer, unsigned int size) = 0;
			};

			void CopyTable(const char * tableName, CopyDataConsumer & dataConsumer);

			void BeginTransaction();

			void CommitTransaction();

			void RollbackTransaction();

			const std::string Escape(const std::string & value);

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

	template <typename T1>
	QueryResult Connection::Execute(const char * query, const T1 & p1)
	{
		QueryParameters params;
		params.Add(p1);
		return Execute(query, params);
	}

	template <typename T1, typename T2>
	QueryResult Connection::Execute(const char * query, const T1 & p1, const T2 & p2)
	{
		QueryParameters params;
		params.Add(p1);
		params.Add(p2);
		return Execute(query, params);
	}

	template <typename T1, typename T2, typename T3>
	QueryResult Connection::Execute(const char * query, const T1 & p1, const T2 & p2, const T3 & p3)
	{
		QueryParameters params;
		params.Add(p1);
		params.Add(p2);
		params.Add(p3);
		return Execute(query, params);
	}

	template <typename T1, typename T2, typename T3, typename T4>
	QueryResult Connection::Execute(const char * query, const T1 & p1, const T2 & p2, const T3 & p3, const T4 & p4)
	{
		QueryParameters params;
		params.Add(p1);
		params.Add(p2);
		params.Add(p3);
		params.Add(p4);
		return Execute(query, params);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5>
	QueryResult Connection::Execute(const char * query, const T1 & p1, const T2 & p2, const T3 & p3, const T4 & p4, const T5 & p5)
	{
		QueryParameters params;
		params.Add(p1);
		params.Add(p2);
		params.Add(p3);
		params.Add(p4);
		params.Add(p5);
		return Execute(query, params);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	QueryResult Connection::Execute(const char * query, const T1 & p1, const T2 & p2, const T3 & p3, const T4 & p4, const T5 & p5, const T6 & p6)
	{
		QueryParameters params;
		params.Add(p1);
		params.Add(p2);
		params.Add(p3);
		params.Add(p4);
		params.Add(p5);
		params.Add(p6);
		return Execute(query, params);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	QueryResult Connection::Execute(const char * query, const T1 & p1, const T2 & p2, const T3 & p3, const T4 & p4, const T5 & p5, const T6 & p6, const T7 & p7)
	{
		QueryParameters params;
		params.Add(p1);
		params.Add(p2);
		params.Add(p3);
		params.Add(p4);
		params.Add(p5);
		params.Add(p6);
		params.Add(p7);
		return Execute(query, params);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	QueryResult Connection::Execute(const char * query, const T1 & p1, const T2 & p2, const T3 & p3, const T4 & p4, const T5 & p5, const T6 & p6, const T7 & p7, const T8 & p8)
	{
		QueryParameters params;
		params.Add(p1);
		params.Add(p2);
		params.Add(p3);
		params.Add(p4);
		params.Add(p5);
		params.Add(p6);
		params.Add(p7);
		params.Add(p8);
		return Execute(query, params);
	}
}
}

#endif
