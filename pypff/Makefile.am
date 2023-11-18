if HAVE_PYTHON
AM_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBPFF_DLL_IMPORT@

pyexec_LTLIBRARIES = pypff.la

pypff_la_SOURCES = \
	pypff.c pypff.h \
	pypff_attachment.c pypff_attachment.h \
	pypff_codepage.c pypff_codepage.h \
	pypff_datetime.c pypff_datetime.h \
	pypff_encryption_types.c pypff_encryption_types.h \
	pypff_error.c pypff_error.h \
	pypff_file.c pypff_file.h \
	pypff_file_content_types.c pypff_file_content_types.h \
	pypff_file_object_io_handle.c pypff_file_object_io_handle.h \
	pypff_file_types.c pypff_file_types.h \
	pypff_folder.c pypff_folder.h \
	pypff_integer.c pypff_integer.h \
	pypff_item.c pypff_item.h \
	pypff_item_types.c pypff_item_types.h \
	pypff_items.c pypff_items.h \
	pypff_libbfio.h \
	pypff_libcerror.h \
	pypff_libclocale.h \
	pypff_libpff.h \
	pypff_message.c pypff_message.h \
	pypff_python.h \
	pypff_record_entry.c pypff_record_entry.h \
	pypff_record_entries.c pypff_record_entries.h \
	pypff_record_set.c pypff_record_set.h \
	pypff_record_sets.c pypff_record_sets.h \
	pypff_unused.h

pypff_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	../libpff/libpff.la \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@

pypff_la_CPPFLAGS = $(PYTHON_CPPFLAGS)
pypff_la_LDFLAGS  = -module -avoid-version $(PYTHON_LDFLAGS)

endif

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f Makefile

