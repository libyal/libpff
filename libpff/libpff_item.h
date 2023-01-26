/*
 * Item functions
 *
 * Copyright (C) 2008-2023, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined( _LIBPFF_ITEM_H )
#define _LIBPFF_ITEM_H

#include <common.h>
#include <types.h>

#include "libpff_descriptors_index.h"
#include "libpff_extern.h"
#include "libpff_file.h"
#include "libpff_io_handle.h"
#include "libpff_item_descriptor.h"
#include "libpff_item_tree.h"
#include "libpff_item_values.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libcdata.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_offsets_index.h"
#include "libpff_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

#define LIBPFF_ITEM_NUMBER_OF_SUB_ITEMS			4

typedef struct libpff_internal_item libpff_internal_item_t;

struct libpff_internal_item
{
	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The IO handle
	 */
	libpff_io_handle_t *io_handle;

	/* The name to id map list
	 */
	libcdata_list_t *name_to_id_map_list;

	/* The descriptors index
	 */
	libpff_descriptors_index_t *descriptors_index;

	/* The offsets index
	 */
	libpff_offsets_index_t *offsets_index;

	/* The item tree
	 */
	libpff_item_tree_t *item_tree;

	/* The item tree node
	 */
	libcdata_tree_node_t *item_tree_node;

	/* The item type
	 */
	uint8_t type;

	/* The item flags
	 */
	uint8_t flags;

	/* The item descriptor
	 */
        libpff_item_descriptor_t *item_descriptor;

	/* The item values
	 */
        libpff_item_values_t *item_values;

	/* The ASCII codepage
	 */
	int ascii_codepage;

	/* The message codepage
	 */
	uint32_t message_codepage;

	/* The message body codepage
	 */
	uint32_t message_body_codepage;

	/* Sub item tree nodes
	 */
	libcdata_tree_node_t *sub_item_tree_node[ LIBPFF_ITEM_NUMBER_OF_SUB_ITEMS ];

	/* Sub item values
	 */
	libpff_item_values_t *sub_item_values[ LIBPFF_ITEM_NUMBER_OF_SUB_ITEMS ];

	/* Embedded object data stream
	 */
	libfdata_stream_t *embedded_object_data_stream;
};

int libpff_item_initialize(
     libpff_item_t **item,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcdata_list_t *name_to_id_map_list,
     libpff_descriptors_index_t *descriptors_index,
     libpff_offsets_index_t *offsets_index,
     libpff_item_tree_t *item_tree,
     libcdata_tree_node_t *item_tree_node,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_free(
     libpff_item_t **item,
     libcerror_error_t **error );

int libpff_internal_item_determine_type(
     libpff_internal_item_t *internal_item,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_identifier(
     libpff_item_t *item,
     uint32_t *identifier,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_number_of_record_sets(
     libpff_item_t *item,
     int *number_of_record_sets,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_record_set_by_index(
     libpff_item_t *item,
     int record_set_index,
     libpff_record_set_t **record_set,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_number_of_entries(
     libpff_item_t *item,
     uint32_t *number_of_entries,
     libcerror_error_t **error );

int libpff_internal_item_get_entry_value_32bit_integer(
     libpff_internal_item_t *internal_item,
     uint32_t entry_type,
     uint32_t *value_32bit,
     libcerror_error_t **error );

int libpff_internal_item_get_entry_value_filetime(
     libpff_internal_item_t *internal_item,
     uint32_t entry_type,
     uint64_t *filetime,
     libcerror_error_t **error );

int libpff_internal_item_get_entry_value_utf8_string_size(
     libpff_internal_item_t *internal_item,
     uint32_t entry_type,
     int ascii_codepage,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libpff_internal_item_get_entry_value_utf8_string(
     libpff_internal_item_t *internal_item,
     uint32_t entry_type,
     int ascii_codepage,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libpff_internal_item_get_entry_value_utf16_string_size(
     libpff_internal_item_t *internal_item,
     uint32_t entry_type,
     int ascii_codepage,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libpff_internal_item_get_entry_value_utf16_string(
     libpff_internal_item_t *internal_item,
     uint32_t entry_type,
     int ascii_codepage,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libpff_internal_item_get_embedded_object_data(
     libpff_internal_item_t *internal_item,
     libpff_record_entry_t *record_entry,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_type(
     libpff_item_t *item,
     uint8_t *item_type,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_number_of_sub_items(
     libpff_item_t *item,
     int *number_of_sub_items,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_sub_item(
     libpff_item_t *item,
     int sub_item_index,
     libpff_item_t **sub_item,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_sub_item_by_identifier(
     libpff_item_t *item,
     uint32_t sub_item_identifier,
     libpff_item_t **sub_item,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_ITEM_H ) */

