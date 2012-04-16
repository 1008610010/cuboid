#ifndef XTL_MYSQL__MYSQL_RESULT_HPP__
#define XTL_MYSQL__MYSQL_RESULT_HPP__ 1

#include <mysql.h>

#include <string>
#include <vector>

namespace XTL
{
namespace MYSQL
{
	class MySqlRow;

	class MySqlResult
	{
		public:

			MySqlResult();

			MySqlResult(MYSQL_RES * result);

			~MySqlResult();

			MySqlResult & operator= (MYSQL_RES * result);

			void Free();

			const bool IsNull() const;

			const unsigned int RowsCount() const;

			const unsigned int ColumnsCount() const;

			const std::string ColumnName(unsigned int index) const;

			const bool Fetch(MySqlRow & row);

		protected:

			MySqlResult(const MySqlResult &);
			MySqlResult & operator= (const MySqlResult &);

			void Init();

			MYSQL_RES * result_;
			std::vector<std::string> columns_;
	};

	class MySqlRow
	{
		public:

			MySqlRow();

			~MySqlRow() throw();

			void Clear();

			const bool Empty() const;

			const unsigned int Size() const;

			const bool IsNull(unsigned int index) const;

			const long long Integer(unsigned int index) const;

			const std::string String(unsigned int index) const;

			const std::string operator[] (unsigned int index) const;

		protected:

			MySqlRow(const MySqlRow &);
			MySqlRow & operator= (const MySqlRow &);

			void PushBack(const char * s);

			friend class MySqlResult;

			std::vector<char *> values_;
	};
}
}

#endif
