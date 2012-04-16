#ifndef _XTL__MYSQL_BASE_CONNECTION_HPP__
#define _XTL__MYSQL_BASE_CONNECTION_HPP__ 1

#include <mysql.h>
#include <mysql/errmsg.h>
#include <string.h>
#include <string>
#include "MySqlResult.hpp"

namespace XTL
{
	class MySqlBaseConnection
	{
		public:

			MySqlBaseConnection();

			~MySqlBaseConnection() throw();

			const long long Execute(const std::string & query);

			MYSQL_RES * Select(const std::string & query);

			const int GetRowsNumber(const std::string & query);
	};
}

#endif
