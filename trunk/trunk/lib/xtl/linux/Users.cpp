#include "PasswordFile.hpp"

#include <grp.h>
#include <pwd.h>

#include <vector>

#include "UnixError.hpp"

namespace XTL
{
	namespace
	{
		class MaxSize
		{
			public:

				static const long DEFAULT_VALUE = 256;

				static long GetPwName()
				{
					return Instance().getpwnam_;
				}
				
				static long GetGrName()
				{
					return Instance().getgrnam_;
				}

			private:

				static const MaxSize & Instance()
				{
					static MaxSize instance;
					return instance;
				}

				MaxSize()
					: getpwnam_(DEFAULT_VALUE),
					  getgrnam_(DEFAULT_VALUE)
				{
					getpwnam_ = ::sysconf(_SC_GETPW_R_SIZE_MAX);
					if (getpwnam_ < 0)
					{
						getpwnam_ = DEFAULT_VALUE;
					}

					getgrnam_ = ::sysconf(_SC_GETGR_R_SIZE_MAX);
					if (getgrnam_ < 0)
					{
						getgrnam_ = DEFAULT_VALUE;
					}
				}

				long getpwnam_;
				long getgrnam_;
		};
	}

	uid_t PasswordFile::GetUserId(const std::string & userName)
	{
		struct passwd pwd;
		struct passwd * result;
		std::vector<char> buffer(MaxSize::GetPwName());

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

	gid_t PasswordFile::GetGroupId(const std::string & groupName)
	{
		struct group grp;
		struct group * result;
		std::vector<char> buffer(MaxSize::GetGrName());

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

