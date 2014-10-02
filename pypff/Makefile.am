if HAVE_PYTHON
AM_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@

pyexec_LTLIBRARIES = pypff.la

pypff_la_SOURCES = \
	pypff.c pypff.h \
	pypff_codepage.c pypff_codepage.h \
	pypff_datetime.c pypff_datetime.h \
	pypff_error.c pypff_error.h \
	pypff_file.c pypff_file.h \
	pypff_file_object_io_handle.c pypff_file_object_io_handle.h \
	pypff_folder.c pypff_folder.h \
	pypff_integer.c pypff_integer.h \
	pypff_item.c pypff_item.h \
	pypff_items.c pypff_items.h \
	pypff_libbfio.h \
	pypff_libcerror.h \
	pypff_libclocale.h \
	pypff_libcstring.h \
	pypff_libpff.h \
	pypff_message.c pypff_message.h \
	pypff_python.h \
	pypff_record_entry.c pypff_record_entry.h \
	pypff_record_entries.c pypff_record_entries.h \
	pypff_record_set.c pypff_record_set.h \
	pypff_record_sets.c pypff_record_sets.h \
	pypff_unused.h

pypff_la_LIBADD = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	../libpff/libpff.la \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@

pypff_la_CPPFLAGS = $(PYTHON_CPPFLAGS)
pypff_la_LDFLAGS  = -module -avoid-version $(PYTHON_LDFLAGS)

endif

EXTRA_DIST = \
	setup.py \
	setup.py.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f setup.py
	/bin/rm -f Makefile

