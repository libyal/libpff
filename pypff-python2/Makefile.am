# Note that we cannot use: AUTOMAKE_OPTIONS = subdir-objects
# subdir-objects will compile the source files to a single version of Python.
# Since subdir-objects is being deprecated we copy the source files instead.
am__installdirs = "$(DESTDIR)$(pyexecdir2)"
pyexecdir = $(pyexecdir2)

if HAVE_PYTHON2
AM_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@

am_pypff_la_rpath = -rpath $(pyexecdir2)

BUILT_SOURCES = \
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

pyexec_LTLIBRARIES = pypff.la

nodist_pypff_la_SOURCES = $(BUILT_SOURCES)

pypff_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	../libpff/libpff.la \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@

pypff_la_CPPFLAGS = $(PYTHON2_CPPFLAGS)
pypff_la_LDFLAGS  = -module -avoid-version $(PYTHON2_LDFLAGS)

$(BUILT_SOURCES):
	/bin/cp -f $(top_srcdir)/pypff/$@ $@

endif

MAINTAINERCLEANFILES = \
	Makefile.in

clean-local:
	/bin/rm -f pypff*.[ch]

distclean: clean
	/bin/rm -f Makefile

