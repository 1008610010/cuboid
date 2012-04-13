#ifndef XTL__UNIX_ERROR_HPP__
#define XTL__UNIX_ERROR_HPP__ 1

#include <errno.h>

#include <string>

#include "../Exception.hpp"

namespace XTL
{
	class UnixError : public Exception
	{
		public:

			UnixError();

			explicit UnixError(int e);

			virtual ~UnixError() throw();

			virtual const std::string What() const;

			int Code() const;

			class Interrupted;
			class AlreadyExists;

		protected:

			const std::string ToString() const;

			int errno_;
	};

	class UnixError::Interrupted : public UnixError
	{
		public:

			Interrupted();

			virtual ~Interrupted() throw();
	};

	class UnixError::AlreadyExists : public UnixError
	{
		public:

			AlreadyExists();

			virtual ~AlreadyExists() throw();
	};
}

#endif

