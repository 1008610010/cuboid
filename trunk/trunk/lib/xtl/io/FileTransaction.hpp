#ifndef XTL__FILE_TRANSACTION_HPP__
#define XTL__FILE_TRANSACTION_HPP__ 1

#include <string>

#include "OutputStream.hpp"
#include "FileOutputStreamBuffered.hpp"

namespace XTL
{
	class FileTransaction : public OutputStream
	{
		public:

			static const unsigned int DEFAULT_BUFFER_CAPACITY = (1 << 20);

			FileTransaction(const std::string & filePath, unsigned int bufferCapacity = DEFAULT_BUFFER_CAPACITY);

			virtual ~FileTransaction() throw();

			virtual void Write(const void * buffer, unsigned int size);

			FileSize Position() const;

			void Commit();

		protected:

			virtual const std::string TempFilePath(const std::string & filePath) const;

		private:

			const std::string filePath_;
			FileOutputStreamBuffered outputStream_;
	};
}

#endif

