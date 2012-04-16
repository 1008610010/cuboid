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

	template <typename MySqlConnector>
	bool MySqlDatabaseExists(MySqlConnector & dbc, const std::string & database)
	{
		MySqlResult result(dbc.Select("SHOW DATABASES;"));
		if (result.IsNull())
		{
			return false;
		}

		for (MySqlRow row; result.Fetch(row); )
		{
			if (::strcasecmp(row[0].c_str(), database.c_str()) == 0)
			{
				return true;
			}
		}

		return false;
	}

	template <typename MySqlConnector>
	bool MySqlCreateDatabase(MySqlConnector & dbc, const std::string & database)
	{
		return dbc.Execute(std::string("CREATE DATABASE ") + database) >= 0;
	}
}

#endif
