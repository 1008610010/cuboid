#ifndef XTL__PGSQL__CURSOR_HPP__
#define XTL__PGSQL__CURSOR_HPP__ 1

#include <string>

namespace XTL
{
namespace PGSQL
{
	class Connection;
	class QueryResult;

	class Cursor
	{
		public:

			Cursor(Connection & dbc);

			void Open(const std::string & name, const std::string & query);

			void Close();

			QueryResult Fetch(int count);

		private:

			Connection  & dbc_;
			std::string   name_;
	};
}
}

#endif
