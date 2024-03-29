#ifndef XTL__MYSQL__CONNECTION_CONFIG_HPP__
#define XTL__MYSQL__CONNECTION_CONFIG_HPP__ 1

#include <string>

namespace XTL
{
namespace MYSQL
{
	struct ConnectionConfig
	{
		std::string host;
		int         port;
		std::string database;
		std::string user;
		std::string password;

		ConnectionConfig()
			: host(),
			  port(0),
			  database(),
			  user(),
			  password()
		{
			;;
		}
	};
}
}

#endif

