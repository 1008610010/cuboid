#include "SystemConfig.hpp"

#include <unistd.h>

namespace XTL
{
	struct SystemConfigImpl
	{
		static const long DEFAULT_MAX_SIZE_VALUE = 256;

		long getpwnam_max_size;
		long getgrnam_max_size;

		SystemConfigImpl()
			: getpwnam_max_size(DEFAULT_MAX_SIZE_VALUE),
			  getgrnam_max_size(DEFAULT_MAX_SIZE_VALUE)
		{
			getpwnam_max_size = ::sysconf(_SC_GETPW_R_SIZE_MAX);
			if (getpwnam_max_size < 0)
			{
				getpwnam_max_size = DEFAULT_MAX_SIZE_VALUE;
			}

			getgrnam_max_size = ::sysconf(_SC_GETGR_R_SIZE_MAX);
			if (getgrnam_max_size < 0)
			{
				getgrnam_max_size = DEFAULT_MAX_SIZE_VALUE;
			}
		}
	};

	long SystemConfig::GetPwNameMaxSize()
	{
		return Impl().getpwnam_max_size;
	}

	long SystemConfig::GetGrNameMaxSize()
	{
		return Impl().getgrnam_max_size;
	}

	const SystemConfigImpl & SystemConfig::Impl()
	{
		static SystemConfig instance;
		return *(instance.impl_);
	}

	SystemConfig::SystemConfig()
		: impl_(new SystemConfigImpl())
	{
		;;
	}
}

