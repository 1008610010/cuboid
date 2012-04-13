#ifndef _XTL__CURL_DOWNLOADER_HPP__
#define _XTL__CURL_DOWNLOADER_HPP__ 1

#include <string>
#include <stdexcept>
#include <curl/curl.h>

namespace XTL
{
	class CurlDownloader
	{
		public:

			enum Options
			{
				OPT_TIMEOUT         = 60,
				OPT_CONNECT_TIMEOUT = 60,
				OPT_PERFORM_TRIES   =  3
			};

			CurlDownloader();

			~CurlDownloader() throw();

			void Clear();

			void Get(const std::string & url);

			void Post(const std::string & url, const std::string & postData, const std::string & cookie);

			const long int ResponseCode() const;

			const std::string Header() const { return header_; }

			const std::string Content() const { return content_; }

		protected:

			class LibrarySentinel
			{
				public:

					LibrarySentinel();

					~LibrarySentinel() throw();
			};

			void InitCurl();

			void PerformCurl();

			static size_t OnHeader(void   * ptr,
			                       size_t   size,
			                       size_t   nmemb,
			                       void   * stream);

			static size_t OnWrite(void   * ptr,
			                      size_t   size,
			                      size_t   nmemb,
			                      void   * stream);

			CURL * curl_;
			std::string header_;
			std::string content_;
			char errorBuffer_[CURL_ERROR_SIZE];
	};
}

#endif
