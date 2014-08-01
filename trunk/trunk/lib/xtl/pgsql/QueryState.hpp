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
				: state_(state)
			{
				;;
			}

			bool IsUniqueViolation() const
			{
				return state_ == "23505";
			}

		private:

			const std::string state_;
	};
}
}

#endif

