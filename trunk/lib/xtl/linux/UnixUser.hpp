#ifndef XTL__UNIX_USER_HPP__
#define XTL__UNIX_USER_HPP__ 1

#include <sys/types.h>

#include <string>

namespace XTL
{
	class UnixUser
	{
		public:

			class NoSuchUser
			{
			};

			/**
				@throws XTL::UnixUser::NoSuchUser
				@throws XTL::UnixError
			 */
			static uid_t GetUserId(const std::string & userName);
	};
}


#endif

