#ifdef __cplusplus
extern "C" {
#endif
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#ifdef __cplusplus
}
#endif

#include "ppport.h"

#include "PerlUtils.hpp"

#include <stdio.h>

#include <xtl/Types.hpp>
#include <xtl/StringUtils.hpp>
#include <xtl/io/FileInputStreamBuffered.hpp>

XS(boot_SUP__XTL__FileInputStream);

MODULE = SUP::XTL		PACKAGE = SUP::XTL

BOOT:
	printf("sizeof(IV)=%lu\n", sizeof(IV));
	printf("sizeof(UV)=%lu\n", sizeof(UV));
	printf("sizeof(NV)=%lu\n", sizeof(NV));
	boot_SUP__XTL__FileInputStream(aTHX_ cv);

SV *
int64()
	CODE:
		RETVAL = XTL::PERL::newSViv64(0);
	OUTPUT:
		RETVAL



MODULE = SUP::XTL		PACKAGE = SUP::XTL::RecordPrototype

