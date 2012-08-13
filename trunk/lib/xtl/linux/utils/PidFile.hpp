#ifndef XTL__PID_FILE_HPP__
#define XTL__PID_FILE_HPP__ 1

#include <memory>
#include <string>

namespace XTL
{
	class PidFile
	{
		public:

			// @throws XTL::UnixError
			static std::auto_ptr<PidFile> Create(const std::string & fileName);

			~PidFile() throw();

		private:

			explicit PidFile(const std::string & fileName);

			const std::string fileName_;
	};

}

#endif

