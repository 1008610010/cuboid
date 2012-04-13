#ifndef XTL__PGSQL__QUERY_RESULT_HPP__
#define XTL__PGSQL__QUERY_RESULT_HPP__ 1

#include <stdint.h>

#include <string>

namespace XTL
{
namespace PGSQL
{
	class QueryResultRow;

	class QueryResult
	{
		private:

			/** For assign or construct from the result of function, which returns it by value */
			struct Reference
			{
				Reference(void * handle) : handle_(handle) { ;; }

				void * handle_;
			};

		public:

			QueryResult();

			QueryResult(QueryResult & other);

			QueryResult & operator= (QueryResult & other);

			QueryResult(Reference ref);

			operator Reference();

			QueryResult & operator= (Reference ref);

			~QueryResult() throw();

			void Clear();

			bool Success() const;

			bool Failed() const
			{
				return !Success();
			}

			int RowsCount() const;

			int ColumnsCount() const;

//			long long int Affected() const;

			bool Empty() const
			{
				return RowsCount() == 0;
			}

			const std::string ColumnName(unsigned int column) const;

			const char * GetSqlState() const;

			const QueryResultRow GetRow(unsigned int row) const;

			const std::string GetString(unsigned int row, unsigned int column) const;

			char GetChar(unsigned int row, unsigned int column, char defaultValue = '\0') const;

			long long int GetInteger(unsigned int row, unsigned int column, long long int defaultValue = 0) const;

			float GetFloat(unsigned int row, unsigned int column, float defaultValue = 0.0) const;

			double GetDouble(unsigned int row, unsigned int column, double defaultValue = 0.0) const;

		private:

			friend class Connection;

			explicit QueryResult(void * handle);

			void ResetHandle(void * newHandle);

			const char * GetValue(unsigned int row, unsigned int column) const;

			void * handle_; // PGResult *
	};

	template <typename Collection_>
	bool GetColumnsNames(const QueryResult & result, Collection_ & collection)
	{
		if (result.Failed()) {
			return false;
		}

		collection.clear();
		unsigned int count = result.ColumnsCount();
		for (unsigned int i = 0; i < count; ++i) {
			collection.push_back(result.ColumnName(i));
		}

		return true;
	}
}
}

#endif

