#ifndef XTL__DIRECTORY_READER_RECURSIVE_HPP__
#define XTL__DIRECTORY_READER_RECURSIVE_HPP__ 1

#include <string>

#include "../UnixError.hpp"

namespace XTL
{
	class DirectoryReaderRecursive
	{
		public:

			class Listener
			{
				public:

					virtual ~Listener() throw() { ;; }

					virtual bool IsDirectoryAllowed(const std::string & baseDir, const std::string & directoryName) const = 0;

					virtual void OnDirectory(const std::string & baseDir, const std::string & directoryName) const = 0;

					virtual void OnRegularFile(const std::string & baseDir, const std::string & fileName) const = 0;

					virtual void OnDirectoryError(const std::string & baseDir, const XTL::UnixError & e) const = 0;
			};

			explicit DirectoryReaderRecursive(Listener & listener);

			void Read(const std::string & baseDir);

		private:

			void ProcessDirectory(const std::string & baseDir);

			Listener & listener_;
	};

	void ReadDirectoryRecursive(const std::string & baseDir, DirectoryReaderRecursive::Listener & listener);
}

#endif
