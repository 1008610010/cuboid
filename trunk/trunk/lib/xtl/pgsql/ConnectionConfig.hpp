#ifndef XTL_PGSQL__CONNECTION_CONFIG_HPP__
#define XTL_PGSQL__CONNECTION_CONFIG_HPP__ 1

#include <string>

namespace XTL
{
namespace PGSQL
{
	struct ConnectionConfig
	{
		std::string host;
		int         port;
		std::string database;
		std::string user;
		std::string password;

		ConnectionConfig();

		const std::string ConnectionString() const;
	};
}
}

#endif

