#include "UnixUser.hpp"

#include <grp.h>
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

		uid_t uid = result->pw_uid;

		::endpwent();

		return uid;
	}

	gid_t UnixUser::GetGroupId(const std::string & groupName)
	{
		struct group grp;
		struct group * result;
		std::vector<char> buffer(SystemConfig::GetGrNameMaxSize());

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

		gid_t gid = result->gr_gid;

		::endgrent();

		return gid;
	}

	void UnixUser::SetUserId(uid_t id)
	{
		if (::setuid(id) != 0)
		{
			throw UnixError();
		}
	}

	void UnixUser::SetGroupId(gid_t id)
	{
		if (::setgid(id) != 0)
		{
			throw UnixError();
		}
	}
}

