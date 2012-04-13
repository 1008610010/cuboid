#include "ConnectionConfig.hpp"

#include "../FormatString.hpp"

namespace XTL
{
namespace PGSQL
{
	ConnectionConfig::ConnectionConfig()
		: host(),
		  port(0),
		  database(),
		  user(),
		  password()
	{
		;;
	}

	const std::string ConnectionConfig::ConnectionString() const
	{
		std::string result = XTL::FormatString("dbname=%s", database.c_str());

		if (!host.empty())
		{
			result.append(XTL::FormatString(" host=%s", host.c_str()));
			if (port != 0)
			{
				result.append(XTL::FormatString(" port=%d", port));
			}
		}

		if (!user.empty())
		{
			result.append(XTL::FormatString(" user=%s", user.c_str()));
			if (!password.empty())
			{
				result.append(XTL::FormatString(" password=%s", password.c_str()));
			}
		}

		return result;
	}
}
}

