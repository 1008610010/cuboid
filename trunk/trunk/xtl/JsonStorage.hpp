#ifndef _XTL__JSON_STORAGE_HPP__
#define _XTL__JSON_STORAGE_HPP__ 1

#include "FileLock.hpp"
#include "Json.hpp"
#include "JsonFile.hpp"

namespace XTL
{
	class JsonStorage
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

			class Locked { };

			JsonStorage(const std::string & filePath);

			const char * FilePath() const { return file_.FilePath(); }

			const char * LockFilePath() const { return (std::string(file_.FilePath()) + ".lock").c_str(); }

			bool Create(bool waitLock = false);

			bool Open(bool waitLock = false);

			bool Flush();

			void Close();

			JsonVariableRef Root()  { return file_.Root(); }

		protected:

			JsonFile file_;
			FileLock lock_;
	};
}

#endif

