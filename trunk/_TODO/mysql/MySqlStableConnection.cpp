#include "MySqlStableConnection.hpp"
#include <strings.h>

namespace XTL
{
	MySqlStableConnection::MySqlStableConnection(const std::string & host,
	                                             int port,
	                                             const std::string & user,
	                                             const std::string & password,
	                                             const std::string & database)
		: Super(host, port, user, password, database),
		  lastErrorCode_(0),
		  lastError_(),
		  transaction_(false)
	{
		;;
	}

	const bool MySqlStableConnection::Open1()
	{
		if (Super::Open1())
		{
			ClearLastError();
			return true;
		}
		else
		{
			SetLastError();
			return false;
		}
	}

	void MySqlStableConnection::Close()
	{
		Super::Close();
		ClearLastError();
	}

	const long long MySqlStableConnection::Execute(const std::string & query)
	{
		int i = AUTO_CONNECT_TRIES;

		while (true)
		{
			long long int result = Super::Execute(query);
			SetLastError();

			if (result >= 0)
			{
				return result;
			}

			if (ConnectionLost() && i-- > 0)
			{
				Close();

				if (transaction_)
				{
					transaction_ = false;
					return result;
				}

				if (Open1())
				{
					continue;
				}
			}

			return result;
		}
	}

	MYSQL_RES * MySqlStableConnection::Select(const std::string & query)
	{
		int i = AUTO_CONNECT_TRIES;

		while (true)
		{
			MYSQL_RES * result = Super::Select(query);
			SetLastError();

			if (result != 0)
			{
				return result;
			}

			if (ConnectionLost() && i-- > 0)
			{
				Close();

				if (transaction_)
				{
					transaction_ = false;
					return result;
				}

				if (Open1())
				{
					continue;
				}
			}

			return result;
		}
	}

	const int MySqlStableConnection::GetRowsNumber(const std::string & query)
	{
		int i = AUTO_CONNECT_TRIES;

		while (true)
		{
			int result = Super::GetRowsNumber(query);
			SetLastError();

			if (result >= 0)
			{
				return result;
			}

			if (ConnectionLost() && i-- > 0)
			{
				Close();

				if (transaction_)
				{
					transaction_ = false;
					return result;
				}

				if (Open1())
				{
					continue;
				}
			}

			return result;
		}
	}

	const bool MySqlStableConnection::BeginTransaction()
	{
		if (transaction_)
		{
			ClearLastError();
			return true;
		}

		int i = AUTO_CONNECT_TRIES;

		while (true)
		{
			long long int result = Super::Execute("START TRANSACTION;");
			SetLastError();

			if (result >= 0)
			{
				transaction_ = true;
				return true;
			}

			if (ConnectionLost() && i-- > 0)
			{
				Close();

				if (Open1())
				{
					continue;
				}
			}

			return false;
		}
	}

	const bool MySqlStableConnection::CommitTransaction()
	{
		if (!transaction_)
		{
			ClearLastError();
			return false;
		}

		transaction_ = false;

		long long int result = Super::Execute("COMMIT;");
		SetLastError();

		if (result >= 0)
		{
			return true;
		}
		else
		{
			if (ConnectionLost())
			{
				Close();
			}
			return false;
		}
	}

	const bool MySqlStableConnection::RollbackTransaction()
	{
		if (!transaction_)
		{
			ClearLastError();
			return true;
		}

		transaction_ = false;

		long long int result = Super::Execute("ROLLBACK;");
		SetLastError();

		if (result >= 0)
		{
			return true;
		}
		else
		{
			if (ConnectionLost())
			{
				Close();
			}
			return false;
		}
	}

	const bool MySqlStableConnection::ConnectionLost() const
	{
		unsigned int e = Super::LastErrorCode();
		return e == CR_SERVER_GONE_ERROR || e == CR_SERVER_LOST;
	}

	void MySqlStableConnection::ClearLastError()
	{
		lastErrorCode_ = 0;
		lastError_.clear();
	}

	void MySqlStableConnection::SetLastError()
	{
		lastErrorCode_ = Super::LastErrorCode();
		lastError_ = Super::LastError();
	}
}
