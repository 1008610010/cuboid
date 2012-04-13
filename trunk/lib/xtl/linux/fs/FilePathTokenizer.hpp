#ifndef XTL__FILE_PATH_TOKENIZER_HPP__
#define XTL__FILE_PATH_TOKENIZER_HPP__ 1

#include <string>

namespace XTL
{
	class FilePathTokenListener
	{
		public:

			virtual ~FilePathTokenListener() throw() { ;; }

			virtual void OnRootDir() = 0;

			virtual void OnCurrentDir() = 0;

			virtual void OnParentDir() = 0;

			virtual void OnPathPart(const std::string & part) = 0;
	};

	class FilePathTokenizer
	{
		public:

			static const char DEFAULT_DELIMITER = '/';

			FilePathTokenizer(FilePathTokenListener & listener, const char * filePath, char delimiter = DEFAULT_DELIMITER);

		private:

			bool IsDelimiter();

			void SkipDelimiters();

			void ProcessToken(const char * begin, const char * end);

			FilePathTokenListener & listener_;
			const char * ptr_;
			const char * begin_;
			const char   delimiter_;
	};

	void TokenizeFilePath(FilePathTokenListener & listener, const char * filePath, char delimiter = FilePathTokenizer::DEFAULT_DELIMITER);
}

#endif

