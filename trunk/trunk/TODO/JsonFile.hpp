#ifndef _XTL__JSON_FILE_HPP__
#define _XTL__JSON_FILE_HPP__ 1

#include "Json.hpp"
#include "JsonParser.hpp"

namespace XTL
{
	class Json;

	class JsonFile
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

			JsonFile(const std::string & filePath);

			~JsonFile() throw();

			void Clear();

			bool Load();

			void Save();

			const char * FilePath() const   { return filePath_.c_str(); }

//			JsonConstant ConstRoot() const  { return root_; }

			JsonVariableRef Root()           { return root_; }

		protected:

			const std::string TempFileName() const;

			const std::string   filePath_;
			JsonValue         * root_;
	};
}

#endif

