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
		;;
	}
}

