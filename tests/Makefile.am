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
	@LIBPFF_DLL_IMPORT@

if HAVE_PYTHON
TESTS_PYPFF  = \
	test_pypff_set_ascii_codepage.sh \
	test_pypff_open_close.sh
endif

TESTS = \
	test_set_ascii_codepage.sh \
	test_open_close.sh \
	test_read_items.sh \
	test_pffinfo.sh \
	test_pffexport.sh \
	test_pffexport_recovered.sh \
	$(TESTS_PYPFF)

check_SCRIPTS = \
	pypff_test_open_close.py \
	pypff_test_set_ascii_codepage.py \
	test_open_close.sh \
	test_pypff_open_close.sh \
	test_pypff_set_ascii_codepage.sh \
	test_pffexport.sh \
	test_pffexport_recovered.sh \
	test_pffinfo.sh \
	test_read_items.sh \
	test_runner.sh \
	test_set_ascii_codepage.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS =  \
	pff_test_open_close \
	pff_test_read_items \
	pff_test_set_ascii_codepage

pff_test_open_close_SOURCES = \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_libcstring.h \
	pff_test_open_close.c

pff_test_open_close_LDADD = \
	../libpff/libpff.la \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

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

