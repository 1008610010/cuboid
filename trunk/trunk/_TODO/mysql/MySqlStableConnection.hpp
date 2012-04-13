#ifndef _XTL_MYSQL_STABLE_CONNECTION_HPP__
#define _XTL_MYSQL_STABLE_CONNECTION_HPP__ 1

#include "MySqlConnection.hpp"

namespace XTL
{
	/*
	 TODO: transform to template StableConnection<typename MySqlConnector>
	*/
	class MySqlStableConnection : protected MySqlConnection
	{
		typedef MySqlConnection Super;

		public:

			enum
			{
				AUTO_CONNECT_TRIES = 1
			};

			MySqlStableConnection(const std::string & host,
			                      int port,
			                      const std::string & user,
			                      const std::string & password,
			                      const std::string & database);

			using Super::Escape;
			using Super::Opened;

			const unsigned int LastErrorCode() const
			{
				return lastErrorCode_;
			}

			const char * LastError() const
			{
				return lastError_.c_str();
			}

			const bool Open1();

			void Close();

			const long long Execute(const std::string & query);

			MYSQL_RES * Select(const std::string & query);

			const int GetRowsNumber(const std::string & query);

			const bool BeginTransaction();

			const bool CommitTransaction();

			const bool RollbackTransaction();

		protected:

			const bool ConnectionLost() const;

			void ClearLastError();

			void SetLastError();

			unsigned int      lastErrorCode_;
			std::string       lastError_;
			bool              transaction_;
	};
}

#endif
