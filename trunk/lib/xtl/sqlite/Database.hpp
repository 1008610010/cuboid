#ifndef XTL__SQLITE__DATABASE_HPP__
#define XTL__SQLITE__DATABASE_HPP__ 1

#include <string>

#include "Exception.hpp"
#include "Statement.hpp"

namespace XTL
{
namespace SQLITE
{
	class Database
	{
		public:

			/*
			 * TODO: add callback to method 'execute'
			 * If an sqlite3_exec() callback returns non-zero, the sqlite3_exec()
			 * routine returns SQLITE_ABORT without invoking the callback again and
			 * without running any subsequent SQL statements.
			 */
			// typedef int (*Callback) (void * param, int argc, char ** argv, char ** columnsNames);

			Database(const std::string & filePath);

			/**
			 * База данных автоматически закрывается в деструкторе.
			 */
			~Database() throw();

			/**
			 * Путь к файлу базы данных, указанный в конструкторе объекта.
			 */
			const std::string & FilePath() const;

			/**
			 * @throw XTL::SQLITE::Exception
			 */
			void Open();

			bool Close();

			/**
			 * @throw XTL::SQLITE::QueryError
			 * @throw XTL::IllegalOperationError - база данных не была открыта
			 */
			int Execute(const std::string & query);

			template <typename T1>
			int Execute(const std::string & query, const T1 & t1)
			{
				Statement statement(Prepare(query));
				statement.Bind(1, t1);
				return statement.Execute();
			}

			template <typename T1, typename T2>
			int Execute(const std::string & query, const T1 & t1, const T2 & t2)
			{
				Statement statement(Prepare(query));
				statement.Bind(1, t1);
				statement.Bind(2, t2);
				return statement.Execute();
			}

			int AffectedRows();

			/**
			 * @throw XTL::SQLITE::QueryError
			 * @throw XTL::IllegalOperationError - база данных не была открыта
			 */
			Statement Prepare(const std::string & query);

			template <typename T1>
			const std::string SelectString(const std::string & query, const T1 & t1)
			{
				XTL::SQLITE::Statement statement = Prepare(query);
				statement.Bind(1, t1);
				return statement.FetchString("");
			}

		private:

			friend class QueryResult;
			friend class Statement;

			Database(const Database &);

			Database & operator= (const Database &);

			void ThrowException();

			void ThrowQueryError(const std::string & query);

			const std::string filePath_;
			void * db_;
	};
}
}

#endif
