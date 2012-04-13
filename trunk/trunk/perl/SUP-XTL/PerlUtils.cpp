#include "PerlUtils.hpp"

#include <xtl/StringUtils.hpp>

namespace XTL
{
namespace PERL
{
	void NeedString(SV * sv)
	{
		if (!SvOK(sv))
		{
			sv_setpvn(sv,"",0);
		}
		else if (!SvPOK(sv))
		{
			SvUPGRADE(sv, SVt_PV); 
			SvPOK_only(sv); 
		}
	}

	SV * Int64_IV::ToSV(XTL::INT_64 value)
	{
		return newSViv(value);
	}

	XTL::INT_64 Int64_IV::ToInt64(SV * sv)
	{
		if (!SvOK(sv))
		{
			return 0;
		}
		else if (SvIOK_UV(sv))
		{
			return SvUV(sv);
		}
		else if (SvIOK(sv))
		{
			return SvIV(sv);
		}
		else if (SvNOK(sv))
		{
			return SvNV(sv);
		}

		return XTL::StringToInteger<XTL::INT_64>(static_cast<const char *>(SvPV_nolen(sv)));
	}

	SV * Int64_NV::ToSV(XTL::INT_64 value)
	{
		SV * sv = newRV_noinc(newSVnv(*reinterpret_cast<const double *>(&value)));
		sv_bless(sv, SUP_XTL_Int64_Stash());
		return sv;
	}

	XTL::INT_64 Int64_NV::ToInt64(SV * sv)
	{
		if (!SvOK(sv))
		{
			return 0;
		}
		else if (SvROK(sv))
		{
			SV * sv64 = SvRV(sv);
			if (sv64 && (sv_isa(sv, "SUP::XTL::Int64") || sv_isa(sv, "SUP::XTL::UInt64")))
			{
				return *reinterpret_cast<const XTL::INT_64 *>( &(SvNVX(sv64)) );
			}
		}
		else if (SvIOK_UV(sv))
		{
			return SvUV(sv);
		}
		else if (SvIOK(sv))
		{
			return SvIV(sv);
		}
		else if (SvNOK(sv))
		{
			return SvNV(sv);
		}

		return XTL::StringToInteger<XTL::INT_64>(static_cast<const char *>(SvPV_nolen(sv)));
	}

	SV * newSViv64(XTL::INT_64 value)
	{
		return Int64_IV::ToSV(value);
	}

	XTL::INT_64 sv_to_int64(SV * sv)
	{
		return Int64_IV::ToInt64(sv);
	}

	HV * SUP_XTL_Int64_Stash()
	{
		static HV * stash = gv_stashsv(newSVpv("SUP::XTL::Int64", 0), TRUE);
		return stash;
	}
}
}

