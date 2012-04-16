#ifndef XTL__MYSQL_CONNECTION_HPP__
#define XTL__MYSQL_CONNECTION_HPP__ 1

namespace XTL
{
namespace MYSQL
{
	class MySqlConnection
	{
		public:

			MySqlConnection(const std::string & host,
			                int port,
			                const std::string & user,
			                const std::string & password,
			                const std::string & database)
				: mysql_(0),
				  host_(host),
				  port_(port),
				  user_(user),
				  password_(password),
				  database_(database)
			{
				;;
			}

			~MySqlConnection() throw()
			{
				Close();
			}

			unsigned int LastErrorCode() const
			{
				return ::mysql_errno(mysql_);
			}

			const char * LastError() const
			{
				return ::mysql_error(mysql_);
			}

			const std::string EscapeString(const std::string & s)
			{
				std::vector<char> result(2 * s.length() + 1);
				unsigned int length = ::mysql_real_escape_string(mysql_, &result[0], s.c_str(), s.length());
				return std::string(&result[0], length);
			}

			bool IsOpened() const
			{
				return mysql_ != 0;
			}

			bool Open()
			{
				Close();

				mysql_ = ::mysql_init(0);

				if (mysql_ == 0)
				{
					return false;
				}

				return ::mysql_real_connect(mysql_, host_.c_str(), user_.c_str(), password_.c_str(), database_.c_str(), port_, NULL, 0) != 0;
			}

			void Close()
			{
				if (mysql_ != 0)
				{
					::mysql_close(mysql_);
					mysql_ = 0;
				}
			}

			/*
				Значение -1 показывает, что данный запрос возвратил ошибку или
				что для запроса SELECT функция mysql_affected_rows() была
				вызвана прежде вызова функции mysql_store_result().
			*/
			long long Execute(const std::string & query)
			{
				if (::mysql_real_query(mysql_, query.c_str(), query.length()) != 0)
				{
					return -1;
				}

				//return ::mysql_affected_rows(mysql_);
				int i = ::mysql_affected_rows(mysql_);
				return i;
			}

			MYSQL_RES * Select(const std::string & query)
			{
				if (::mysql_real_query(mysql_, query.data(), query.length()) != 0)
				{
					return 0;
				}

				return ::mysql_use_result(mysql_);
			}

			// TODO: move it outside the class
			int GetRowsNumber(const std::string & query)
			{
				if (::mysql_real_query(mysql_, query.data(), query.length()) != 0)
				{
					return -1;
				}

				MYSQL_RES * result = ::mysql_use_result(mysql_);
				if (result == 0)
				{
					return -1;
				}

				int size = ::mysql_num_rows(result);
				::mysql_free_result(result);

				return size;
			}

		private:

			MySqlConnection(const MySqlConnection &);
			MySqlConnection & operator= (const MySqlConnection &);

			MYSQL * mysql_;

			const std::string host_;
			int               port_;
			const std::string user_;
			const std::string password_;
			const std::string database_;
	};

/*
	bool IsDatabaseExists(MySqlConnection & dbc, const std::string & database)
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
*/
}
}

#endif
