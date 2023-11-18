include_HEADERS = \
	libpff.h

pkginclude_HEADERS = \
	libpff/codepage.h \
	libpff/definitions.h \
	libpff/error.h \
	libpff/extern.h \
	libpff/features.h \
	libpff/mapi.h \
	libpff/types.h

EXTRA_DIST = \
	libpff.h.in \
	libpff/definitions.h.in \
	libpff/features.h.in \
	libpff/types.h.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f libpff.h
	-rm -f libpff/definitions.h
	-rm -f libpff/features.h
	-rm -f libpff/types.h
	-rm -f Makefile

