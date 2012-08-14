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
			explicit PidFile(const std::string & fileName);

			~PidFile() throw();

		private:

			PidFile(const PidFile &);
			PidFile & operator= (const PidFile &);

			const std::string fileName_;
	};

}

#endif

