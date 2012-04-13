#ifndef XTL__ACCESS_MODE_HPP__
#define XTL__ACCESS_MODE_HPP__ 1

#include <sys/types.h>

namespace XTL
{
	class AccessMode
	{
		public:

			static const AccessMode DEFAULT_DIRECTORY;
			static const AccessMode DEFAULT_FILE;
			static const AccessMode DEFAULT_PROGRAM;

			AccessMode(mode_t mode)
				: mode_(mode) { ;; }

			mode_t Get() const
			{
				return mode_;
			}

		private:

			const mode_t mode_;
	};
}

#endif

