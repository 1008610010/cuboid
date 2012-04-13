#include "CurlDownloader.hpp"

namespace XTL
{
	CurlDownloader::LibrarySentinel::LibrarySentinel()
	{
		::curl_global_init(CURL_GLOBAL_NOTHING);
	}

	CurlDownloader::LibrarySentinel::~LibrarySentinel() throw()
	{
		::curl_global_cleanup();
	}

	CurlDownloader::CurlDownloader()
		: curl_(0),
		  header_(),
		  content_()
	{
		static LibrarySentinel sentinel;
	}

	CurlDownloader::~CurlDownloader() throw()
	{
		Clear();
	}

	void CurlDownloader::Clear()
	{
		if (curl_ != 0)
		{
			::curl_easy_cleanup(curl_);
			curl_ = 0;
			header_.clear();
			content_.clear();
		}
	}

	void CurlDownloader::InitCurl()
	{
		Clear();
		curl_ = ::curl_easy_init();
		if (curl_ == 0)
		{
			throw std::runtime_error("Unable to init libcurl handle");
		}

		// TODO: check return values
		::curl_easy_setopt(curl_, CURLOPT_HEADERFUNCTION, &OnHeader);
		::curl_easy_setopt(curl_, CURLOPT_HEADERDATA, this);
		::curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, &OnWrite);
		::curl_easy_setopt(curl_, CURLOPT_WRITEDATA, this);
		::curl_easy_setopt(curl_, CURLOPT_TIMEOUT, OPT_TIMEOUT);
		::curl_easy_setopt(curl_, CURLOPT_CONNECTTIMEOUT, OPT_CONNECT_TIMEOUT);
		::curl_easy_setopt(curl_, CURLOPT_ERRORBUFFER, errorBuffer_);
	}

	void CurlDownloader::PerformCurl()
	{
		int result = 0;
		for (int i = 0; i < OPT_PERFORM_TRIES; ++i)
		{
			result = ::curl_easy_perform(curl_);
			if (result == 0)
			{
				break;
			}
		}

		if (result != 0)
		{
			throw std::runtime_error(std::string("Download error: ") + errorBuffer_);
		}
	}

	size_t CurlDownloader::OnHeader(void   * ptr,
	                                size_t   size,
	                                size_t   nmemb,
	                                void   * stream)
	{
		size_t totalBytes = size * nmemb;
		reinterpret_cast<CurlDownloader *>(stream)->header_.append(static_cast<const char *>(ptr), totalBytes);
		return totalBytes;
	}

	size_t CurlDownloader::OnWrite(void   * ptr,
	                               size_t   size,
	                               size_t   nmemb,
	                               void   * stream)
	{
		size_t totalBytes = size * nmemb;
		reinterpret_cast<CurlDownloader *>(stream)->content_.append(static_cast<const char *>(ptr), totalBytes);
		return totalBytes;
	}

	void CurlDownloader::Get(const std::string & url)
	{
		InitCurl();

		::curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());

		PerformCurl();
	}

	void CurlDownloader::Post(const std::string & url, const std::string & postData, const std::string & cookie)
	{
		InitCurl();

		::curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());
		::curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, postData.c_str());
		::curl_easy_setopt(curl_, CURLOPT_COOKIE, cookie.c_str());

		PerformCurl();
	}

	const long int CurlDownloader::ResponseCode() const
	{
		if (curl_ == 0)
		{
			return 0;
		}

		long int code;
		::curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &code);
		return code;
	}

}
