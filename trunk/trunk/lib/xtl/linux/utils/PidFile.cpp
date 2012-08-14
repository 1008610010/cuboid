#include "PidFile.hpp"

namespace XTL
{
	PidFile::~PidFile() throw()
	{
		try
		{
			File f(fileName_);
			f.Unlink();
		}
		catch (...)
		{
			// Do nothing.
		}
	}

	PidFile::PidFile(const std::string & fileName)
		: fileName_(fileName)
	{
		File f(fileName_);
		f.Create(File::RECREATE_APPEND, AccessMode::DEFAULT_FILE);
		f.Write(FormatString("%u\n", static_cast<unsigned int>(::getpid())));
		f.Close();
	}
}

