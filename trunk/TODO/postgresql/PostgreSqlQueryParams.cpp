#include "PostgreSqlQueryParams.hpp"
#include "PostgreSqlUtils.hpp"

namespace XC3
{
	PostgreSqlQueryParams::PostgreSqlQueryParams()
		: formats_(NULL),
		  types_(NULL),
		  lengths_(NULL),
		  values_(NULL)
	{
		;;
	}

	PostgreSqlQueryParams::~PostgreSqlQueryParams() throw()
	{
		Destroy();
	}

	void PostgreSqlQueryParams::Create(const CHAR_8 * signature)
	{
		Destroy();

		if (signature == NULL)
		{
			return;
		}

		for (const CHAR_8 * sign = signature; *sign != '\0'; sign++)
		{
			PostgreSqlParam * param = NULL;
			switch (*sign)
			{
				case 'i':
				case 'I':
					param = new PostgreSqlParamInt64();
					break;
				case 's':
				case 'S':
					param = new PostgreSqlParamString();
					break;
				default:
#ifdef _DEBUG
					fprintf(stderr, "Undefined parameter signature: '%c'\n", *sign);
#endif
					;;
			}

			if (param != NULL)
			{
				params_.push_back(param);
			}
		}
		formats_ = new int[params_.size()];
		types_   = new Oid[params_.size()];
		lengths_ = new int[params_.size()];
		values_  = new char*[params_.size()];
		for (UINT_32 i = 0; i < params_.size(); i++)
		{
			formats_[i] = 1;
			types_[i] = params_[i]->Type();
		}
	}

	void PostgreSqlQueryParams::Destroy()
	{
		DestroyCollection(params_);
		if (formats_ != NULL) { delete [] formats_; formats_ = NULL; }
		if (types_ != NULL)   { delete [] types_;   types_ = NULL; }
		if (lengths_ != NULL) { delete [] lengths_; lengths_ = NULL; }
		if (values_ != NULL)  { delete [] values_;  values_ = NULL; }
	}

	void PostgreSqlQueryParams::Set(UINT_32 index, INT_64 value)
	{
		params_[index]->Set(value);
	}

	void PostgreSqlQueryParams::Set(UINT_32 index, const std::string & value)
	{
		params_[index]->Set(value);
	}

	void PostgreSqlQueryParams::UpdateValues()
	{
		for (UINT_32 i = 0; i < params_.size(); i++)
		{
			lengths_[i] = params_[i]->Length();
			values_[i] = (char *) params_[i]->Value();
		}
	}


}
