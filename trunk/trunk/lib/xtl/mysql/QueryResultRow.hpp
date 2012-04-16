#ifndef XTL_MYSQL__QUERY_RESULT_ROW_HPP__
#define XTL_MYSQL__QUERY_RESULT_ROW_HPP__ 1

#include <string>
#include <vector>

namespace XTL
{
namespace MYSQL
{
	class QueryResultRow
	{
		public:

			QueryResultRow();

			~QueryResultRow() throw();

			void Clear();

			bool IsEmpty() const;

			unsigned int Size() const;

			bool IsNull(unsigned int index) const;

			const std::string GetString(unsigned int index) const;

			long long GetInteger(unsigned int index) const;

		private:

			QueryResultRow(const QueryResultRow &);
			QueryResultRow & operator= (const QueryResultRow &);

			void PushBack(const char * s);

			friend class QueryResult;

			std::vector<char *> values_;
	};
}
}

#endif

