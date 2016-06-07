AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBCSYSTEM_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBPFF_DLL_IMPORT@

if HAVE_PYTHON
TESTS_PYPFF = \
	test_python_functions.sh \
	test_pypff_set_ascii_codepage.sh
endif

TESTS = \
	test_api_functions.sh \
	test_set_ascii_codepage.sh \
	test_read_items.sh \
	test_pffinfo.sh \
	test_pffexport.sh \
	$(TESTS_PYPFF)

check_SCRIPTS = \
	pypff_test_get_version.py \
	pypff_test_open_close.py \
	pypff_test_set_ascii_codepage.py \
	test_api_functions.sh \
	test_pffexport.sh \
	test_pffinfo.sh \
	test_pypff_set_ascii_codepage.sh \
	test_read_items.sh \
	test_runner.sh \
	test_set_ascii_codepage.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	pff_test_get_version \
	pff_test_open_close \
	pff_test_read_items \
	pff_test_set_ascii_codepage

pff_test_get_version_SOURCES = \
	pff_test_get_version.c \
	pff_test_libcstring.h \
	pff_test_libpff.h \
	pff_test_unused.h

pff_test_get_version_LDADD = \
	../libpff/libpff.la \
	@LIBCSTRING_LIBADD@

pff_test_open_close_SOURCES = \
	pff_test_libcerror.h \
	pff_test_libcstring.h \
	pff_test_libcsystem.h \
	pff_test_libpff.h \
	pff_test_open_close.c

pff_test_open_close_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libpff/libpff.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@PTHREAD_LIBADD@

pff_test_read_items_SOURCES = \
	pff_test_libcerror.h \
	pff_test_libcstring.h \
	pff_test_libpff.h \
	pff_test_read_items.c

pff_test_read_items_LDADD = \
	../libpff/libpff.la \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

pff_test_set_ascii_codepage_SOURCES = \
	pff_test_libcerror.h \
	pff_test_libclocale.h \
	pff_test_libpff.h \
	pff_test_libcstring.h \
	pff_test_set_ascii_codepage.c

pff_test_set_ascii_codepage_LDADD = \
	../libpff/libpff.la \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

