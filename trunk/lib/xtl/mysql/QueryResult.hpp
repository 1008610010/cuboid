#ifndef XTL_MYSQL__QUERY_RESULT_HPP__
#define XTL_MYSQL__QUERY_RESULT_HPP__ 1

#include <mysql.h>

#include <string>
#include <vector>

namespace XTL
{
namespace MYSQL
{
	class QueryResultRow;

	class QueryResult
	{
		public:

			QueryResult();

			QueryResult(MYSQL_RES * result);

			~QueryResult() throw();

			QueryResult & operator= (MYSQL_RES * result);

			void Free();

			bool IsNull() const;

			unsigned int RowsCount() const;

			unsigned int ColumnsCount() const;

			const std::string ColumnName(unsigned int index) const;

			bool Fetch(QueryResultRow & row);

		protected:

			QueryResult(const QueryResult &);
			QueryResult & operator= (const QueryResult &);

			void Init();

			MYSQL_RES * result_;
			std::vector<std::string> columns_;
	};
}
}

#endif
