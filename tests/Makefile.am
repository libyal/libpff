AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFDATA_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBFWNT_CPPFLAGS@ \
	@LIBFMAPI_CPPFLAGS@ \
	@ZLIB_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBPFF_DLL_IMPORT@

if HAVE_PYTHON_TESTS
TESTS_PYPFF = \
	test_python_module.sh
endif

TESTS = \
	test_library.sh \
	test_read_items.sh \
	test_pffinfo.sh \
	test_pffexport.sh \
	$(TESTS_PYPFF)

check_SCRIPTS = \
	pypff_test_file.py \
	pypff_test_support.py \
	test_library.sh \
	test_pffexport.sh \
	test_pffinfo.sh \
	test_python_module.sh \
	test_read_items.sh \
	test_runner.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	pff_test_allocation_table \
	pff_test_attached_file_io_handle \
	pff_test_attachment \
	pff_test_column_definition \
	pff_test_data_array \
	pff_test_data_array_entry \
	pff_test_data_block \
	pff_test_deflate \
	pff_test_descriptors_index \
	pff_test_error \
	pff_test_file \
	pff_test_folder \
	pff_test_index \
	pff_test_index_node \
	pff_test_index_value \
	pff_test_io_handle \
	pff_test_item \
	pff_test_item_descriptor \
	pff_test_item_tree \
	pff_test_item_values \
	pff_test_local_descriptor_node \
	pff_test_local_descriptor_value \
	pff_test_local_descriptors \
	pff_test_mapi_value \
	pff_test_message \
	pff_test_multi_value \
	pff_test_name_to_id_map_entry \
	pff_test_notify \
	pff_test_offsets_index \
	pff_test_read_items \
	pff_test_record_entry \
	pff_test_record_set \
	pff_test_reference_descriptor \
	pff_test_support \
	pff_test_table \
	pff_test_table_block_index \
	pff_test_table_index_value \
	pff_test_value_type

pff_test_allocation_table_SOURCES = \
	pff_test_allocation_table.c \
	pff_test_functions.c pff_test_functions.h \
	pff_test_libbfio.h \
	pff_test_libcdata.h \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_memory.c pff_test_memory.h \
	pff_test_unused.h

pff_test_allocation_table_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_attached_file_io_handle_SOURCES = \
	pff_test_attached_file_io_handle.c \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_memory.c pff_test_memory.h \
	pff_test_unused.h

pff_test_attached_file_io_handle_LDADD = \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_attachment_SOURCES = \
	pff_test_attachment.c \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_unused.h

pff_test_attachment_LDADD = \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_column_definition_SOURCES = \
	pff_test_column_definition.c \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_memory.c pff_test_memory.h \
	pff_test_unused.h

pff_test_column_definition_LDADD = \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_data_array_SOURCES = \
	pff_test_data_array.c \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_memory.c pff_test_memory.h \
	pff_test_unused.h

pff_test_data_array_LDADD = \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_data_array_entry_SOURCES = \
	pff_test_data_array_entry.c \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_memory.c pff_test_memory.h \
	pff_test_unused.h

pff_test_data_array_entry_LDADD = \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_data_block_SOURCES = \
	pff_test_data_block.c \
	pff_test_functions.c pff_test_functions.h \
	pff_test_libbfio.h \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_memory.c pff_test_memory.h \
	pff_test_unused.h

pff_test_data_block_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_deflate_SOURCES = \
	pff_test_deflate.c \
	pff_test_libcerror.h \
	pff_test_libcnotify.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_unused.h

pff_test_deflate_LDADD = \
	@LIBCNOTIFY_LIBADD@ \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_descriptors_index_SOURCES = \
	pff_test_descriptors_index.c \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_memory.c pff_test_memory.h \
	pff_test_unused.h

pff_test_descriptors_index_LDADD = \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_error_SOURCES = \
	pff_test_error.c \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_unused.h

pff_test_error_LDADD = \
	../libpff/libpff.la

pff_test_file_SOURCES = \
	pff_test_file.c \
	pff_test_functions.c pff_test_functions.h \
	pff_test_getopt.c pff_test_getopt.h \
	pff_test_libcerror.h \
	pff_test_libclocale.h \
	pff_test_libcnotify.h \
	pff_test_libpff.h \
	pff_test_libuna.h \
	pff_test_macros.h \
	pff_test_memory.c pff_test_memory.h \
	pff_test_unused.h

pff_test_file_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libpff/libpff.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

pff_test_folder_SOURCES = \
	pff_test_folder.c \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_unused.h

pff_test_folder_LDADD = \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_index_SOURCES = \
	pff_test_index.c \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_memory.c pff_test_memory.h \
	pff_test_unused.h

pff_test_index_LDADD = \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_index_node_SOURCES = \
	pff_test_index_node.c \
	pff_test_functions.c pff_test_functions.h \
	pff_test_libbfio.h \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_memory.c pff_test_memory.h \
	pff_test_unused.h

pff_test_index_node_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_index_value_SOURCES = \
	pff_test_index_value.c \
	pff_test_libcerror.h \
	pff_test_libfdata.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_memory.c pff_test_memory.h \
	pff_test_unused.h

pff_test_index_value_LDADD = \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_io_handle_SOURCES = \
	pff_test_io_handle.c \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_memory.c pff_test_memory.h \
	pff_test_unused.h

pff_test_io_handle_LDADD = \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_item_SOURCES = \
	pff_test_item.c \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_memory.c pff_test_memory.h \
	pff_test_unused.h

pff_test_item_LDADD = \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_item_descriptor_SOURCES = \
	pff_test_item_descriptor.c \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_memory.c pff_test_memory.h \
	pff_test_unused.h

pff_test_item_descriptor_LDADD = \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_item_tree_SOURCES = \
	pff_test_item_tree.c \
	pff_test_libbfio.h \
	pff_test_libcdata.h \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_memory.c pff_test_memory.h \
	pff_test_unused.h

pff_test_item_tree_LDADD = \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_item_values_SOURCES = \
	pff_test_item_values.c \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_memory.c pff_test_memory.h \
	pff_test_unused.h

pff_test_item_values_LDADD = \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_local_descriptor_node_SOURCES = \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_local_descriptor_node.c \
	pff_test_macros.h \
	pff_test_memory.c pff_test_memory.h \
	pff_test_unused.h

pff_test_local_descriptor_node_LDADD = \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_local_descriptor_value_SOURCES = \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_local_descriptor_value.c \
	pff_test_macros.h \
	pff_test_memory.c pff_test_memory.h \
	pff_test_unused.h

pff_test_local_descriptor_value_LDADD = \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_local_descriptors_SOURCES = \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_local_descriptors.c \
	pff_test_macros.h \
	pff_test_memory.c pff_test_memory.h \
	pff_test_unused.h

pff_test_local_descriptors_LDADD = \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_mapi_value_SOURCES = \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_mapi_value.c \
	pff_test_unused.h

pff_test_mapi_value_LDADD = \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_message_SOURCES = \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_message.c \
	pff_test_unused.h

pff_test_message_LDADD = \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_multi_value_SOURCES = \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_memory.c pff_test_memory.h \
	pff_test_multi_value.c \
	pff_test_unused.h

pff_test_multi_value_LDADD = \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_name_to_id_map_entry_SOURCES = \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_name_to_id_map_entry.c \
	pff_test_unused.h

pff_test_name_to_id_map_entry_LDADD = \
	../libpff/libpff.la

pff_test_notify_SOURCES = \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_notify.c \
	pff_test_unused.h

pff_test_notify_LDADD = \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_offsets_index_SOURCES = \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_memory.c pff_test_memory.h \
	pff_test_offsets_index.c \
	pff_test_unused.h

pff_test_offsets_index_LDADD = \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_read_items_SOURCES = \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_read_items.c

pff_test_read_items_LDADD = \
	../libpff/libpff.la \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@

pff_test_record_entry_SOURCES = \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_memory.c pff_test_memory.h \
	pff_test_record_entry.c \
	pff_test_unused.h

pff_test_record_entry_LDADD = \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_record_set_SOURCES = \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_memory.c pff_test_memory.h \
	pff_test_record_set.c \
	pff_test_unused.h

pff_test_record_set_LDADD = \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_reference_descriptor_SOURCES = \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_memory.c pff_test_memory.h \
	pff_test_reference_descriptor.c \
	pff_test_unused.h

pff_test_reference_descriptor_LDADD = \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_support_SOURCES = \
	pff_test_getopt.c pff_test_getopt.h \
	pff_test_functions.c pff_test_functions.h \
	pff_test_libbfio.h \
	pff_test_libcerror.h \
	pff_test_libclocale.h \
	pff_test_libcnotify.h \
	pff_test_libpff.h \
	pff_test_libuna.h \
	pff_test_macros.h \
	pff_test_memory.c pff_test_memory.h \
	pff_test_support.c \
	pff_test_unused.h

pff_test_support_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_table_SOURCES = \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_memory.c pff_test_memory.h \
	pff_test_table.c \
	pff_test_unused.h

pff_test_table_LDADD = \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_table_block_index_SOURCES = \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_memory.c pff_test_memory.h \
	pff_test_table_block_index.c \
	pff_test_unused.h

pff_test_table_block_index_LDADD = \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_table_index_value_SOURCES = \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_memory.c pff_test_memory.h \
	pff_test_table_index_value.c \
	pff_test_unused.h

pff_test_table_index_value_LDADD = \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

pff_test_value_type_SOURCES = \
	pff_test_libcerror.h \
	pff_test_libpff.h \
	pff_test_macros.h \
	pff_test_unused.h \
	pff_test_value_type.c

pff_test_value_type_LDADD = \
	../libpff/libpff.la \
	@LIBCERROR_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

