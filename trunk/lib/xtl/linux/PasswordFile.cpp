#include "PasswordFile.hpp"

#include <pwd.h>

#include <vector>

#include "UnixError.hpp"

namespace XTL
{
	namespace
	{
		static const unsigned int DEFAULT_BUFFER_SIZE = 128;
	}

	uid_t PasswordFile::GetUserId(const std::string & userName)
	{
		struct passwd pwd;
		struct passwd * result;
		std::vector<char> buffer(DEFAULT_BUFFER_SIZE);

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

			throw XTL::UnixError();
		}

		if (result == 0)
		{
			throw UserNotFound();
		}

		return result->pw_uid;
	}
}

