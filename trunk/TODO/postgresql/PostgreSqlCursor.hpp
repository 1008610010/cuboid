#ifndef _XC3__POSTGRESQL_CURSOR
#define _XC3__POSTGRESQL_CURSOR 1

#include "PostgreSqlConnection.hpp"
#include "PostgreSqlQuery.hpp"
#include "PostgreSqlResult.hpp"
#include <xtl/StringUtils.hpp>
#include <stdexcept>
#include <string>

namespace XC3
{
	class PostgreSqlCursor
	{
		public:

			PostgreSqlCursor()
				: dbc_(0),
				  name_()
			{
				;;
			}

			bool Open(PostgreSqlConnection * dbc,
			          const char           * name,
			          const char           * query)
			{
				if (!Close())
				{
					return false;
				}

				dbc_ = dbc;
				name_ = name;

				std::string cursorQuery = XTL::FormatString(
					"DECLARE %s CURSOR FOR %s;",
					name,
					query
				);

				if (dbc_->Execute(cursorQuery.c_str()).Failed())
				{
					return false;
				}

				return true;
			}

			bool Close()
			{
				if (name_.empty())
				{
					return true;
				}

				bool result = !dbc_->Execute(XTL::FormatString("CLOSE %s;", name_.c_str()).c_str()).Failed();
				name_.clear();
				dbc_ = 0;
				return result;
			}

			PostgreSqlResult Fetch(unsigned int count)
			{
				std::string query = XTL::FormatString("FETCH %u IN %s;", count, name_.c_str());
				return dbc_->Execute(query.c_str());
			}

		private:

			PostgreSqlConnection * dbc_;
			std::string            name_;
	};
}

#endif
