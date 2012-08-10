#ifndef XTL__PASSWORD_FILE_HPP__
#define XTL__PASSWORD_FILE_HPP__ 1

#include <sys/types.h>

#include <string>

namespace XTL
{
	class PasswordFile
	{
		public:

			class UserNotFound
			{
			};

			/**
				@throws XTL::PasswordFile::UserNotFound
				@throws XTL::UnixError
			 */
			static uid_t GetUserId(const std::string & userName);
	};
}


#endif

