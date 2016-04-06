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
	/bin/rm -f libpff.h
	/bin/rm -f libpff/definitions.h
	/bin/rm -f libpff/features.h
	/bin/rm -f libpff/types.h
	/bin/rm -f Makefile

