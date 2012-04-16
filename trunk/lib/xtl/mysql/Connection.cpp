#include "Connection.hpp"

#include <mysql/mysql.h>
#include <mysql/errmsg.h>

#include <vector>

#include "Exception.hpp"
#include "QueryResult.hpp"

typedef MYSQL MySqlHandle;

#define HANDLE_ static_cast<MySqlHandle *>(handle_)

namespace XTL
{
namespace MYSQL
{
	namespace
	{
		void ThrowConnectError(Connection & dbc)
		{
			throw ConnectError(dbc.LastError(), dbc.LastErrorCode());
		}

		void ThrowQueryError(Connection & dbc, const std::string & query)
		{
			throw QueryError(dbc.LastError(), dbc.LastErrorCode(), query);
		}
	}

	Connection::Connection(const ConnectionConfig & config)
		: handle_(0),
		  config_(config)
	{
		;;
	}

	Connection::~Connection() throw()
	{
		Close();
	}

	const std::string Connection::EscapeString(const std::string & s)
	{
		std::vector<char> result(2 * s.length() + 1);
		unsigned int length = ::mysql_real_escape_string(static_cast<MySqlHandle *>(handle_), &result[0], s.c_str(), s.length());
		return std::string(&result[0], length);
	}

	unsigned int Connection::LastErrorCode() const
	{
		return ::mysql_errno(HANDLE_);
	}

	const char * Connection::LastError() const
	{
		return ::mysql_error(HANDLE_);
	}

	void Connection::Open()
	{
		Close();

		handle_ = ::mysql_init(0);

		if (handle_ == 0)
		{
			ThrowConnectError(*this);
		}

		if (::mysql_real_connect(HANDLE_, config_.host.c_str(), config_.user.c_str(), config_.password.c_str(), config_.database.c_str(), config_.port, NULL, 0) == 0)
		{
			ThrowConnectError(*this);
		}
	}

	void Connection::Close()
	{
		if (handle_ != 0)
		{
			::mysql_close(HANDLE_);
			handle_ = 0;
		}
	}

	bool Connection::IsOpened() const
	{
		return handle_ != 0;
	}

	/*
		Значение -1 показывает, что данный запрос возвратил ошибку или
		что для запроса SELECT функция mysql_affected_rows() была
		вызвана прежде вызова функции mysql_store_result().
	*/
	long long Connection::Execute(const std::string & query)
	{
		if (::mysql_real_query(HANDLE_, query.c_str(), query.length()) != 0)
		{
			ThrowQueryError(*this, query);
		}

		my_ulonglong i = ::mysql_affected_rows(HANDLE_);
		if (i == (my_ulonglong) -1)
		{
			ThrowQueryError(*this, query);
		}

		return i;
	}

	QueryResult Connection::Select(const std::string & query)
	{
		if (::mysql_real_query(HANDLE_, query.data(), query.length()) != 0)
		{
			ThrowQueryError(*this, query);
		}

		MYSQL_RES * res = ::mysql_use_result(HANDLE_);
		if (res == 0)
		{
			ThrowQueryError(*this, query);
		}

		return QueryResult(res);
	}
}
}

#undef HANDLE_

