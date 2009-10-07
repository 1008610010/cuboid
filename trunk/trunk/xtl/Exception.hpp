#ifndef _XTL__EXCEPTION_HPP__
#define _XTL__EXCEPTION_HPP__ 1

#include <string>

namespace XTL
{
	class Exception
	{
		public:

			Exception()
				: code_(0), what_() { ;; }

			explicit Exception(int code)
				: code_(code), what_() { ;; }

			explicit Exception(const char * what)
				: code_(0), what_(what) { ;; }

			explicit Exception(const std::string & what)
				: code_(0), what_(what) { ;; }

			Exception(int code, const char * what)
				: code_(code), what_(what) { ;; }

			Exception(int code, const std::string & what)
				: code_(code), what_(what) { ;; }

			int Code() const
			{
				return code_;
			}

			const char * What() const
			{
				return what_.c_str();
			}

		protected:

			int         code_;
			std::string what_;
	};

#define XTL_EXCEPTION_CONSTRUCTORS(CLASS) \
			CLASS() : Exception() { ;; } \
			explicit CLASS(int code) : Exception(code) { ;; } \
			explicit CLASS(const char * what) : Exception(what) { ;; } \
			explicit CLASS(const std::string & what) : Exception(what) { ;; } \
			CLASS(int code, const char * what) : Exception(code, what) { ;; } \
			CLASS(int code, const std::string & what) : Exception(code, what) { ;; }

	class SocketAddressError : public Exception
	{
		public:

			XTL_EXCEPTION_CONSTRUCTORS(SocketAddressError)
	};

	class SocketError : public Exception
	{
		public:

			XTL_EXCEPTION_CONSTRUCTORS(SocketError)
	};

	class InterruptError : public Exception
	{
		public:

			XTL_EXCEPTION_CONSTRUCTORS(InterruptError)
	};

	class ExceptionNotEnoughMemory : public Exception
	{
		public:

			ExceptionNotEnoughMemory(const char * where)
				: Exception(where) { ;; }
	};
}

#endif
