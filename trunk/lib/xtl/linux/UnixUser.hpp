#ifndef XTL__UNIX_USER_HPP__
#define XTL__UNIX_USER_HPP__ 1

#include <sys/types.h>

#include <string>

namespace XTL
{
	class UnixUser
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
				@throws XTL::UnixUser::NoSuchUser
				@throws XTL::UnixError
			 */
			static uid_t GetUserId(const std::string & userName);

			/**
				@throws XTL::UnixUser::NoSuchGroup
				@throws XTL::UnixError
			 */
			static gid_t GetGroupId(const std::string & groupName);

			/**
				@throws XTL::UnixError
			 */
			static void SetUserId(uid_t id);

			/**
				@throws XTL::UnixError
			 */
			static void SetGroupId(gid_t id);
	};
}


#endif

