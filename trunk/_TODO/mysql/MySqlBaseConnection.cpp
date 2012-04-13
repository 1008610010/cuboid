#include "MySqlBaseConnection.hpp"

namespace XTL
{
	MySqlBaseConnection::MySqlBaseConnection()
		: mysql_(0)
	{
		;;
	}

	MySqlBaseConnection::~MySqlBaseConnection() throw()
	{
		Close();
	}

	const unsigned int MySqlBaseConnection::LastErrorCode() const
	{
		return ::mysql_errno(mysql_);
	}

	const char * MySqlBaseConnection::LastError() const
	{
		return ::mysql_error(mysql_);
	}

	const std::string MySqlBaseConnection::Escape(const std::string & s)
	{
		std::vector<char> result(2 * s.length() + 1);
		unsigned int length = ::mysql_real_escape_string(mysql_, &result[0], s.c_str(), s.length());
		return std::string(&result[0], length);
	}

	const bool MySqlBaseConnection::Opened() const
	{
		return mysql_ != 0;
	}

	const bool MySqlBaseConnection::Open1()
	{
		Close();
		mysql_ = ::mysql_init(0);
		return mysql_ != 0;
	}

	void MySqlBaseConnection::Close()
	{
		if (mysql_ != 0)
		{
			::mysql_close(mysql_);
			mysql_ = 0;
		}
	}

	const long long MySqlBaseConnection::Execute(const std::string & query)
	{
		if (::mysql_real_query(mysql_, query.c_str(), query.length()) != 0)
		{
			return -1;
		}

		//return ::mysql_affected_rows(mysql_);
		int i = ::mysql_affected_rows(mysql_);
		return i;
	}

	MYSQL_RES * MySqlBaseConnection::Select(const std::string & query)
	{
		if (::mysql_real_query(mysql_, query.data(), query.length()) != 0)
		{
			return 0;
		}

		return ::mysql_use_result(mysql_);
	}

	const int MySqlBaseConnection::GetRowsNumber(const std::string & query)
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
}
