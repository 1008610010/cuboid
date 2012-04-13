#ifndef _XC3__POSTGRESQL_RESULT_HPP__
#define _XC3__POSTGRESQL_RESULT_HPP__ 1

#include <libpq-fe.h>
#include <string>
#include <inttypes.h>

namespace XC3
{
	class PostgreSqlResult
	{
		private:

			/** For assign or construct from the result of function, which returns it by value */
			struct Reference
			{
				Reference(PGresult * handle) : handle_(handle) { ;; }

				PGresult * handle_;
			};

		public:

			PostgreSqlResult();

			explicit PostgreSqlResult(PostgreSqlResult & result);

			PostgreSqlResult(Reference ref);

			~PostgreSqlResult() throw();

			PostgreSqlResult & operator= (PostgreSqlResult & result);

			PostgreSqlResult & operator= (Reference ref);

			operator Reference() throw();

			void Clear();

			bool Success() const;

			bool Failed() const;

			bool Empty() const;

			unsigned int RowsCount() const;

			unsigned int ColumnsCount() const;

			const std::string ColumnName(unsigned int column) const;

			long long int Affected() const;

			const char * Status() const;

			template <typename _StorageType>
			bool GetColumnsNames(_StorageType & storage)
			{
				if (Failed())
				{
					return false;
				}

				storage.clear();
				unsigned int count = ColumnsCount();
				for (unsigned int i = 0; i < count; ++i)
				{
					storage.push_back(ColumnName(i));
				}

				return true;
			}

			const std::string GetString(unsigned int row, unsigned int column);

			int64_t GetBigInt(unsigned int row, unsigned int column, int64_t defaultValue = 0);

		private:

			friend class PostgreSqlConnection;
			friend class PostgreSqlQuery;

			explicit PostgreSqlResult(PGresult * handle);

			PostgreSqlResult & operator= (PGresult * handle);

			PGresult * handle_;
	};
}

#endif
