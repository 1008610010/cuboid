#ifndef XTL__PGSQL__QUERY_PARAMETERS_HPP__
#define XTL__PGSQL__QUERY_PARAMETERS_HPP__ 1

#include <memory>
#include <string>

#include "Types.hpp"

namespace XTL
{
namespace PGSQL
{
	class QueryParametersImpl;

	class QueryParameters
	{
		public:

			QueryParameters();

			void Add(NullType value);

			void Add(const std::string & value);

			void Add(long long int value);

			int Size() const;

			const void * const Types() const;

			const char * const * const Values() const;

		private:

			std::auto_ptr<QueryParametersImpl> impl_;
	};
}
}

#endif

