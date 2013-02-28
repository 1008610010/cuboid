#include "PidFile.hpp"

#include <unistd.h>

namespace XTL
{
	const XTL::PidFile::Mode XTL::PidFile::REMOVE(1);
	const XTL::PidFile::Mode XTL::PidFile::DO_NOT_REMOVE(2);

	PidFile::PidFile(const std::string & fileName, Mode mode)
		: fileName_(fileName),
		  mode_(mode)
	{
		File f(fileName_);
		f.Create(File::RECREATE_APPEND, AccessMode::DEFAULT_FILE);
		f.Write(FormatString("%u\n", static_cast<unsigned int>(::getpid())));
		f.Close();
	}

	PidFile::~PidFile() throw()
	{
		if (mode_ == REMOVE)
		{
			Remove();
		}
	}

	void PidFile::Remove()
	{
		if (fileName_.empty())
		{
			return;
		}

		try
		{
			File f(fileName_);
			f.Unlink();
			fileName_.clear();
		}
		catch (...)
		{
			// Do nothing.
		}
	}
}

