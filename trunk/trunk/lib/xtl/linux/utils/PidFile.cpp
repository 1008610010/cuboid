#include "PidFile.hpp"

#include <unistd.h>

#include "../../FormatString.hpp"
#include "../AccessMode.hpp"
#include "../fs/File.hpp"

namespace XTL
{
	std::auto_ptr<PidFile> PidFile::Create(const std::string & fileName)
	{
		File f(fileName);
		f.Create(File::RECREATE_APPEND, AccessMode::DEFAULT_FILE);
		f.Write(FormatString("%u\n", static_cast<unsigned int>(::getpid())));
		f.Close();

		return std::auto_ptr<PidFile>(new PidFile(fileName));
	}

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

