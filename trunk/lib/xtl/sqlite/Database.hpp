#ifndef XTL_SQLITE__DATABASE_HPP__
#define XTL_SQLITE__DATABASE_HPP__ 1

#include <string>

#include "Exception.hpp"
#include "QueryResult.hpp"

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

			/**
			 * @throw XTL::SQLITE::QueryError
			 * @throw XTL::IllegalOperationError - база данных не была открыта
			 */
			QueryResult Select(const std::string & query);

		private:

			friend class QueryResult;

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
