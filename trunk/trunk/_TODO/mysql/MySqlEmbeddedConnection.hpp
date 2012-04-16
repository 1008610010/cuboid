#ifndef _XTL__MYSQL_EMBEDDED_CONNECTION_HPP__
#define _XTL__MYSQL_EMBEDDED_CONNECTION_HPP__ 1

#include "MySqlBaseConnection.hpp"
#include "MySqlLibrary.hpp"

namespace XTL
{
	class MySqlEmbeddedConnection : protected MySqlBaseConnection
	{
		typedef MySqlBaseConnection Super;

		public:

			MySqlEmbeddedConnection(const std::string & appName, const std::string & configFile);

			const bool Open1();

		protected:

			const std::string appName_;
			const std::string configFile_;
	};
}

#endif
