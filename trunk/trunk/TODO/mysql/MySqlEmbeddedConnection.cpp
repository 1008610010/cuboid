#include "MySqlEmbeddedConnection.hpp"

namespace XTL
{
	MySqlEmbeddedConnection::MySqlEmbeddedConnection(const std::string & appName, const std::string & configFile)
		: MySqlBaseConnection(),
		  appName_(appName),
		  configFile_(configFile)
	{
		static MySqlLibrary library(appName, configFile);
	}

	const bool MySqlEmbeddedConnection::Open1()
	{
		if (!Super::Open1())
		{
			return false;
		}

		::mysql_options(mysql_, MYSQL_READ_DEFAULT_GROUP, appName_.c_str());
		::mysql_options(mysql_, MYSQL_OPT_USE_EMBEDDED_CONNECTION, NULL);

		return ::mysql_real_connect(mysql_, NULL, NULL, NULL, NULL/*dbName_.c_str()*/, 0, NULL, 0) != 0;
	}
}
