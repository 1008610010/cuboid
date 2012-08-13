#ifndef XTL__SYSTEM_CONFIG_HPP__
#define XTL__SYSTEM_CONFIG_HPP__ 1

#include <memory>

namespace XTL
{
	struct SystemConfigImpl;

	class SystemConfig
	{
		public:

			static long GetPwNameMaxSize();

			static long GetGrNameMaxSize();

		private:

			static const SystemConfigImpl & Impl();

			SystemConfig();

			std::auto_ptr<SystemConfigImpl> impl_;
	};
}

#endif

