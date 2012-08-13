#include "UnixGroup.hpp"

#include <grp.h>

#include <vector>

#include "SystemConfig.hpp"
#include "UnixError.hpp"

namespace XTL
{
	gid_t UnixGroup::GetGroupId(const std::string & groupName)
	{
		struct group grp;
		struct group * result;
		std::vector<char> buffer(SystemConfig::GetPwNameMaxSize());

		while (true)
		{
			if (::getgrnam_r(groupName.c_str(), &grp, &(buffer[0]), buffer.size(), &result) == 0)
			{
				break;
			}

			if (errno == ERANGE)
			{
				buffer.resize(buffer.size() * 2);
				continue;
			}

			throw UnixError();
		}

		if (result == 0)
		{
			throw NoSuchGroup();
		}

		return result->gr_gid;
	}
}

