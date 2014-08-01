#ifndef XTL__PGSQL__QUERY_RESULT_ROW_HPP__
#define XTL__PGSQL__QUERY_RESULT_ROW_HPP__ 1

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

			bool IsNull(unsigned int column) const
			{
				return result_.IsNull(row_, column);
			}

			const std::string GetString(unsigned int column) const
			{
				return result_.GetString(row_, column);
			}

			char GetChar(unsigned int column, char defaultValue = '\0') const
			{
				return result_.GetChar(row_, column, defaultValue);
			}

			long long int GetLongLong(unsigned int column, long long int defaultValue = 0) const
			{
				return result_.GetLongLong(row_, column, defaultValue);
			}

			float GetFloat(unsigned int column, float defaultValue = 0.0) const
			{
				return result_.GetFloat(row_, column, defaultValue);
			}

			double GetDouble(unsigned int column, double defaultValue = 0.0) const
			{
				return result_.GetDouble(row_, column, defaultValue);
			}

		protected:

			const QueryResult  & result_;
			const unsigned int   row_;
	};
}
}

#endif

