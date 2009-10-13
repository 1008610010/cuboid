#ifndef _XTL__JSON_DATABASE_HPP__
#define _XTL__JSON_DATABASE_HPP__ 1

#include "FileLock.hpp"
#include "Json.hpp"
#include "JsonFile.hpp"

namespace XTL
{
	class JsonDatabase
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

			JsonDatabase(const std::string & filePath);

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
