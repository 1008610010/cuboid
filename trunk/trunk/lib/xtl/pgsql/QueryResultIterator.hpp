#ifndef XTL_PGSQL__QUERY_RESULT_ITERATOR_HPP__
#define XTL_PGSQL__QUERY_RESULT_ITERATOR_HPP__ 1

#include "QueryResult.hpp"
#include "QueryResultRow.hpp"

namespace XTL
{
namespace PGSQL
{
	class QueryResultIterator : public QueryResult
	{
		public:

			QueryResultIterator(QueryResult & result)
				: result_(result),
				  size_(0),
				  index_(0)
			{
				if (result_.Success()) {
					size_ = result_.RowsCount();
				}
			}

			bool AtEnd() const
			{
				return index_ >= size_;
			}

			bool NotAtEnd() const
			{
				return index_ < size_;
			}

			void Next()
			{
				++index_;
			}

			const QueryResultRow operator* () const
			{
				return QueryResultRow(result_, index_);
			}

			QueryResult Release()
			{
				return result_;
			}

		private:

			QueryResult  result_;
			unsigned int size_;
			unsigned int index_;
	};
}
}

#endif

