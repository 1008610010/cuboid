#include "AccessMode.hpp"

#include <fcntl.h>

namespace XTL
{
	const AccessMode AccessMode::DEFAULT_DIRECTORY (0755);
	const AccessMode AccessMode::DEFAULT_FILE      (0644);
	const AccessMode AccessMode::DEFAULT_PROGRAM   (0755);

	bool AccessMode::IsExecutable() const
	{
		return (mode_ & (S_IXUSR | S_IXGRP | S_IXOTH)) != 0;
	}
}

