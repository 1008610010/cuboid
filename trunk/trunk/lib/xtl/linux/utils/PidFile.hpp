#ifndef XTL__PID_FILE_HPP__
#define XTL__PID_FILE_HPP__ 1

#include <memory>
#include <string>

#include "../../FormatString.hpp"
#include "../AccessMode.hpp"
#include "../fs/File.hpp"

namespace XTL
{
	class PidFile
	{
		public:

			// @throws XTL::UnixError
			// TODO: Some bullshit happens, when this piece of code moved to PidFile.cpp (SIGSEGV)
			static std::auto_ptr<PidFile> Create(const std::string & fileName)
			{
				File f(fileName);
				f.Create(File::RECREATE_APPEND, AccessMode::DEFAULT_FILE);
				f.Write(FormatString("%u\n", static_cast<unsigned int>(::getpid())));
				f.Close();

				return std::auto_ptr<PidFile>(new PidFile(fileName));
			}

			~PidFile() throw();

		private:

			explicit PidFile(const std::string & fileName);

			PidFile(const PidFile &);
			PidFile & operator= (const PidFile &);

			const std::string fileName_;
	};

}

#endif

