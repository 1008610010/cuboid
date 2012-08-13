#include "Users.hpp"

#include <pwd.h>

#include <vector>

#include "SystemConfig.hpp"
#include "UnixError.hpp"

namespace XTL
{
	uid_t UnixUser::GetUserId(const std::string & userName)
	{
		struct passwd pwd;
		struct passwd * result;
		std::vector<char> buffer(SystemConfig::GetPwNameMaxSize());

		while (true)
		{
			if (::getpwnam_r(userName.c_str(), &pwd, &(buffer[0]), buffer.size(), &result) == 0)
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
			throw NoSuchUser();
		}

		return result->pw_uid;
	}
}

