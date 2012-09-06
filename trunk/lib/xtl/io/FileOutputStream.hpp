#ifndef XTL__FILE_OUTPUT_STREAM_HPP__
#define XTL__FILE_OUTPUT_STREAM_HPP__ 1

#include <string>

#include "OutputStream.hpp"
#include "../Types.hpp"
#include "../linux/fs/File.hpp"

namespace XTL
{
	class FileOutputStream : public OutputStream
	{
		public:

			FileOutputStream(const std::string & filePath, AccessMode accessMode = AccessMode::DEFAULT_FILE);

			virtual ~FileOutputStream() throw();

			virtual void Write(const void * buffer, unsigned int size);

			void Close()
			{
				file_.Flush();
				file_.Close();
			}

			FileSize Position() const
			{
				return position_;
			}

			const std::string & FilePath() const
			{
				return file_.Path();
			}

		private:

			FileOutputStream(const FileOutputStream &);
			FileOutputStream & operator= (const FileOutputStream &);

			File     file_;
			FileSize position_;
	};
}

#endif

