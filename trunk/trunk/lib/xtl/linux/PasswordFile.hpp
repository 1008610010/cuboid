#ifndef XTL__PASSWORD_FILE_HPP__
#define XTL__PASSWORD_FILE_HPP__ 1

#include <sys/types.h>

#include <string>

namespace XTL
{
	class PasswordFile
	{
		public:

			class NotFound
			{
			};

			class NoSuchUser : public NotFound
			{
			};

			class NoSuchGroup : public NotFound
			{
			};

			/**
				@throws XTL::PasswordFile::NoSuchUser
				@throws XTL::UnixError
			 */
			static uid_t GetUserId(const std::string & userName);

			/**
				@throws XTL::PasswordFile::NoSuchGroup
				@throws XTL::UnixError
			 */
			static gid_t GetGroupId(const std::string & groupName);
	};
}


#endif

