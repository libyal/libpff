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
	@LIBPFF_DLL_EXPORT@

lib_LTLIBRARIES = libpff.la

libpff_la_SOURCES = \
	libpff.c \
	libpff_allocation_table.c libpff_allocation_table.h \
	libpff_attached_file_io_handle.c libpff_attached_file_io_handle.h \
	libpff_attachment.c libpff_attachment.h \
	libpff_bit_stream.c libpff_bit_stream.h \
	libpff_block_descriptor.c libpff_block_descriptor.h \
	libpff_block_tree.c libpff_block_tree.h \
	libpff_block_tree_node.c libpff_block_tree_node.h \
	libpff_checksum.c libpff_checksum.h \
	libpff_codepage.h \
	libpff_column_definition.c libpff_column_definition.h \
	libpff_compression.c libpff_compression.h \
	libpff_data_array.c libpff_data_array.h \
	libpff_data_array_entry.c libpff_data_array_entry.h \
	libpff_data_block.c libpff_data_block.h \
	libpff_debug.c libpff_debug.h \
	libpff_definitions.h \
	libpff_deflate.c libpff_deflate.h \
	libpff_descriptor_data_stream.c libpff_descriptor_data_stream.h \
	libpff_descriptors_index.c libpff_descriptors_index.h \
	libpff_encryption.c libpff_encryption.h \
	libpff_error.c libpff_error.h \
	libpff_extern.h \
	libpff_file.c libpff_file.h \
	libpff_file_header.c libpff_file_header.h \
	libpff_folder.c libpff_folder.h \
	libpff_free_map.c libpff_free_map.h \
	libpff_huffman_tree.c libpff_huffman_tree.h \
	libpff_index.c libpff_index.h \
	libpff_index_node.c libpff_index_node.h \
	libpff_index_value.c libpff_index_value.h \
	libpff_index_values_list.c libpff_index_values_list.h \
	libpff_io_handle.c libpff_io_handle.h \
	libpff_item.c libpff_item.h \
	libpff_item_descriptor.c libpff_item_descriptor.h \
	libpff_item_tree.c libpff_item_tree.h \
	libpff_item_values.c libpff_item_values.h \
	libpff_legacy.c libpff_legacy.h \
	libpff_libbfio.h \
	libpff_libcdata.h \
	libpff_libcerror.h \
	libpff_libclocale.h \
	libpff_libcnotify.h \
	libpff_libfcache.h \
	libpff_libfdata.h \
	libpff_libfguid.h \
	libpff_libfmapi.h \
	libpff_libfvalue.h \
	libpff_libuna.h \
	libpff_local_descriptor_value.c libpff_local_descriptor_value.h \
	libpff_local_descriptors.c libpff_local_descriptors.h \
	libpff_local_descriptors_node.c libpff_local_descriptors_node.h \
	libpff_local_descriptors_tree.c libpff_local_descriptors_tree.h \
	libpff_mapi.h \
	libpff_mapi_value.c libpff_mapi_value.h \
	libpff_message.c libpff_message.h \
	libpff_multi_value.c libpff_multi_value.h \
	libpff_name_to_id_map.c libpff_name_to_id_map.h \
	libpff_notify.c libpff_notify.h \
	libpff_offsets_index.c libpff_offsets_index.h \
	libpff_record_entry.c libpff_record_entry.h \
	libpff_record_entry_identifier.h \
	libpff_record_set.c libpff_record_set.h \
	libpff_recover.c libpff_recover.h \
	libpff_reference_descriptor.c libpff_reference_descriptor.h \
	libpff_support.c libpff_support.h \
	libpff_table.c libpff_table.h \
	libpff_table_block_index.c libpff_table_block_index.h \
	libpff_table_header.c libpff_table_header.h \
	libpff_table_index_value.c libpff_table_index_value.h \
	libpff_types.h \
	libpff_unused.h \
	libpff_value_type.c libpff_value_type.h \
	pff_allocation_table.h \
	pff_array.h \
	pff_block.h \
	pff_file_header.h \
	pff_free_map.h \
	pff_index_node.h \
	pff_local_descriptors_node.h \
	pff_table.h \
	pff_table_block.h \
	pff_value_data.h

libpff_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	@LIBCTHREADS_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
	@LIBFDATA_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFWNT_LIBADD@ \
	@LIBFMAPI_LIBADD@ \
	@ZLIB_LIBADD@ \
	@PTHREAD_LIBADD@

libpff_la_LDFLAGS = -no-undefined -version-info 1:0:0

EXTRA_DIST = \
	libpff_definitions.h.in \
	libpff.rc \
	libpff.rc.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f libpff_definitions.h
	-rm -f libpff.rc
	-rm -f Makefile

sources-local: $(BUILT_SOURCES)

splint-local:
	@echo "Running splint on libpff ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libpff_la_SOURCES)

