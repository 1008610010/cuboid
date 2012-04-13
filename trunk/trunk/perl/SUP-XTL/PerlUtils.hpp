#ifndef XTL__PERL_UTILS_HPP__
#define XTL__PERL_UTILS_HPP__ 1

#ifdef __cplusplus
extern "C" {
#endif
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#ifdef __cplusplus
}
#endif

#include <xtl/Types.hpp>

namespace XTL
{
namespace PERL
{
	void NeedString(SV * sv);

	struct Int64_IV
	{
		static SV * ToSV(XTL::INT_64 value);

		static XTL::INT_64 ToInt64(SV * sv);
	};

	struct Int64_NV
	{
		static SV * ToSV(XTL::INT_64 value);

		static XTL::INT_64 ToInt64(SV * sv);
	};

	SV * newSViv64(XTL::INT_64 value);

	XTL::INT_64 sv_to_int64(SV * sv);

	HV * SUP_XTL_Int64_Stash();
}
}

#endif

