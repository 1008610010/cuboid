#ifndef XTL_MYSQL__QUERY_RESULT_HPP__
#define XTL_MYSQL__QUERY_RESULT_HPP__ 1

#include <string>
#include <vector>

namespace XTL
{
namespace MYSQL
{
	class QueryResultRow;

	class QueryResult
	{
		private:

			struct Reference
			{
				Reference(void * handle) : handle_(handle) { ;; }

				void * handle_;
			};

		public:

			QueryResult();

			QueryResult(Reference ref);

			QueryResult(QueryResult & other);

			~QueryResult() throw();

			operator Reference();

			QueryResult & operator= (Reference ref);

			QueryResult & operator= (QueryResult & other);

			void Free();

			bool IsNull() const;

			unsigned int RowsCount() const;

			unsigned int ColumnsCount() const;

			void GetColumnsNames(std::vector<std::string> & columns) const;

			bool Fetch(QueryResultRow & row);

		private:

			friend class Connection;

			// QueryResult(const QueryResult &);
			QueryResult & operator= (const QueryResult &);

			void * handle_;
	};
}
}

#endif
