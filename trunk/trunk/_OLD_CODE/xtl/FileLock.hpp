#ifndef _XTL__FILE_LOCK_HPP__
#define _XTL__FILE_LOCK_HPP__ 1

#include <string>

namespace XTL
{
	class FileLock
	{
		public:

			class Error
			{
				public:
					explicit Error(const char * what) : what_(what) { ;; }
					explicit Error(const std::string & what) : what_(what) { ;; }
					const char * What() const { return what_.c_str(); }
				protected:
					const std::string what_;
			};

			explicit FileLock(const char * filePath)
				: filePath_(filePath), locked_(false) { ;; }

			explicit FileLock(const std::string & filePath)
				: filePath_(filePath), locked_(false) { ;; }

			bool Locked() const  { return locked_; }

			bool Lock(bool wait = false);

			bool Unlock();

		protected:

			bool TryLock();

			std::string filePath_;
			bool        locked_;
	};
}

#endif
