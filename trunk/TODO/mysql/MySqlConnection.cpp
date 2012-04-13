#include "MySqlConnection.hpp"
#include <strings.h>

namespace XTL
{
	MySqlConnection::MySqlConnection(const std::string & host,
	                                 int port,
	                                 const std::string & user,
	                                 const std::string & password,
	                                 const std::string & database)
		: Super(),
		  host_(host),
		  port_(port),
		  user_(user),
		  password_(password),
		  database_(database)
	{
		;;
	}

	const bool MySqlConnection::Open1()
	{
		return Super::Open1() &&
		       ::mysql_real_connect(mysql_, host_.c_str(), user_.c_str(), password_.c_str(), database_.c_str(), port_, NULL, 0) != 0;
	}
}
