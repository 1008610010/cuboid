#ifndef XTL__MYSQL__CONNECTION_HPP__
#define XTL__MYSQL__CONNECTION_HPP__ 1

#include <string>

#include "ConnectionConfig.hpp"

namespace XTL
{
namespace MYSQL
{
	class QueryResult;

	class Connection
	{
		public:

			explicit Connection(const ConnectionConfig & config);

			~Connection() throw();

			const std::string EscapeString(const std::string & s);

			unsigned int LastErrorCode() const;

			const char * LastError() const;

			void Open();

			void Close();

			bool IsOpened() const;

			/*
				Значение -1 показывает, что данный запрос возвратил ошибку или
				что для запроса SELECT функция mysql_affected_rows() была
				вызвана прежде вызова функции mysql_store_result().
			*/
			long long Execute(const std::string & query);

			QueryResult Select(const std::string & query);

		private:

			Connection(const Connection &);
			Connection & operator= (const Connection &);

			void * handle_;
			ConnectionConfig config_;
	};

/*
	bool IsDatabaseExists(Connection & dbc, const std::string & database)
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

	bool MySqlCreateDatabase(Connection & dbc, const std::string & database)
	{
		return dbc.Execute(std::string("CREATE DATABASE ") + database) >= 0;
	}

	int Connection::GetRowsCount(const std::string & query)
	{
		if (::mysql_real_query(HANDLE_, query.data(), query.length()) != 0)
		{
			throw Exception(::mysql_error(HANDLE_), ::mysql_errno(HANDLE_));
		}

		MYSQL_RES * result = ::mysql_use_result(HANDLE_);
		if (result == 0)
		{
			return -1;
		}

		int size = ::mysql_num_rows(result);
		::mysql_free_result(result);

		return size;
	}

*/
}
}

#endif
