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

typedef XTL::FileInputStreamBuffered XTL__FileInputStreamBuffered;

MODULE = SUP::XTL::FileInputStream		PACKAGE = SUP::XTL::FileInputStream

XTL::FileInputStreamBuffered *
new(CLASS, filePath)
	char * CLASS
	char * filePath
	CODE:
		RETVAL = new XTL::FileInputStreamBuffered(filePath, 65536);
	OUTPUT:
		RETVAL

SV *
size(self)
	XTL::FileInputStreamBuffered * self
	CODE:
		RETVAL = XTL::PERL::newSViv64(self->Size());
	OUTPUT:
		RETVAL

SV *
position(self)
	XTL::FileInputStreamBuffered * self
	CODE:
		RETVAL = XTL::PERL::newSViv64(self->Position());
	OUTPUT:
		RETVAL

SV *
size_left(self)
	XTL::FileInputStreamBuffered * self
	CODE:
		RETVAL = XTL::PERL::newSViv64(self->Size()- self->Position());
	OUTPUT:
		RETVAL

SV *
at_end(self)
	XTL::FileInputStreamBuffered * self
	CODE:
		RETVAL = (self->AtEnd() ? &PL_sv_yes : &PL_sv_no);
	OUTPUT:
		RETVAL

void
read(self, buffer, size)
	XTL::FileInputStreamBuffered * self
	SV * buffer
	STRLEN size
	CODE:
		XTL::PERL::NeedString(buffer);

		if (size <= 0)
		{
			SvCUR_set(buffer, 0);
		}
		else
		{
			/* printf("SvLEN=%lu, SvCUR=%lu, size=%d\n", SvLEN(buffer), SvCUR(buffer), size); */
			SvGROW(buffer, size);
			/* TODO: catch exceptions here */
			self->Read(SvPVX(buffer), size);
			SvCUR_set(buffer, size);
			/* printf("SvLEN=%lu, SvCUR=%lu, size=%d\n", SvLEN(buffer), SvCUR(buffer), size); */
		}
	OUTPUT:
		buffer

void
DESTROY(self)
	XTL::FileInputStreamBuffered * self
	CODE:
		delete self;



