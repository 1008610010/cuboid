#ifndef XTL_PGSQL__QUERY_RESULT_ROW_HPP__
#define XTL_PGSQL__QUERY_RESULT_ROW_HPP__ 1

#include "QueryResult.hpp"

namespace XTL
{
namespace PGSQL
{
	class QueryResultRow
	{
		public:

			QueryResultRow(const QueryResult & result, unsigned int row)
				: result_(result),
				  row_(row)
			{
				;;
			}

			unsigned int GetIndex() const
			{
				return row_;
			}

			const std::string GetString(unsigned int column) const
			{
				return result_.GetString(row_, column);
			}

			char GetChar(unsigned int column, char defaultValue = '\0') const
			{
				return result_.GetChar(row_, column, defaultValue);
			}
/*
			long long int GetInteger(unsigned int column, int64_t defaultValue = 0) const
			{
				return result_.GetInteger(row_, column, defaultValue);
			}
*/
		protected:

			const QueryResult  & result_;
			const unsigned int   row_;
	};
}
}

#endif

