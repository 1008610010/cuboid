#include "QueryParameters.hpp"

#include <postgres.h>
#include <catalog/pg_type.h>

#include <vector>

#include "../FormatString.hpp"

namespace XTL
{
namespace PGSQL
{
	class QueryParametersImpl
	{
		public:

			QueryParametersImpl()
				: paramTypes_(),
				  paramValues_(),
				  paramStrings_()
			{
				;;
			}

			void Add(const std::string & value)
			{
				paramTypes_.push_back(TEXTOID);
				paramStrings_.push_back(value);
				paramValues_.push_back(paramStrings_.back().c_str());
			}

			void Add(long long int value)
			{
				paramTypes_.push_back(INT8OID);
				paramStrings_.push_back(XTL::FormatString("%lld", value));
				paramValues_.push_back(paramStrings_.back().c_str());
			}

			int Size() const
			{
				return paramTypes_.size();
			}

			const void * const Types() const
			{
				return &(paramTypes_[0]);
			}

			const char * const * const Values() const
			{
				return &(paramValues_[0]);
			}

		private:

			QueryParametersImpl(const QueryParametersImpl &);
			QueryParametersImpl & operator= (const QueryParametersImpl &);

			std::vector<Oid>          paramTypes_;
			std::vector<const char *> paramValues_;
			std::vector<std::string>  paramStrings_;
	};


	QueryParameters::QueryParameters()
		: impl_(new QueryParametersImpl())
	{
		;;
	}

	void QueryParameters::Add(const std::string & value)
	{
		impl_->Add(value);
	}

	void QueryParameters::Add(long long int value)
	{
		impl_->Add(value);
	}

	int QueryParameters::Size() const
	{
		return impl_->Size();
	}

	const void * const QueryParameters::Types() const
	{
		return impl_->Types();
	}

	const char * const * const QueryParameters::Values() const
	{
		return impl_->Values();
	}
}
}
