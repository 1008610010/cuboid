#ifndef XTL__PGSQL__QUERY_STATE_HPP__
#define XTL__PGSQL__QUERY_STATE_HPP__ 1

#include <string>

namespace XTL
{
namespace PGSQL
{
	class QueryState
	{
		public:

			QueryState(const char * state)
				: state_(state) { ;; }

		private:

			const std::string state_;
	};
}
}

#endif

