#ifndef _XTL__MYSQL_CONNECTION_HPP__
#define _XTL__MYSQL_CONNECTION_HPP__ 1

#include "MySqlBaseConnection.hpp"

namespace XTL
{
	class MySqlConnection : protected MySqlBaseConnection
	{
		typedef MySqlBaseConnection Super;

		public:

			MySqlConnection(const std::string & host,
			                int port,
			                const std::string & user,
			                const std::string & password,
			                const std::string & database);

			using Super::LastErrorCode;
			using Super::LastError;
			using Super::Escape;
			using Super::Opened;
			using Super::Close;
			using Super::Execute;
			using Super::Select;
			using Super::GetRowsNumber;

			const bool Open1();

		protected:

			const std::string host_;
			int               port_;
			const std::string user_;
			const std::string password_;
			const std::string database_;
	};
}

#endif
