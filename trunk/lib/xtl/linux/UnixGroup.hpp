#ifndef XTL__UNIX_GROUP_HPP__
#define XTL__UNIX_GROUP_HPP__ 1

#include <sys/types.h>

#include <string>

namespace XTL
{
	class UnixGroup
	{
		public:

			class NoSuchGroup
			{
			};

			/**
				@throws XTL::UnixGroup::NoSuchGroup
				@throws XTL::UnixError
			 */
			static gid_t GetGroupId(const std::string & groupName);

			/**
				@throws XTL::UnixError
			 */
			static void SetGroupId(gid_t id);
	};
}

#endif

