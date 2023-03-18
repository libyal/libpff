/*
 * File functions
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

#if !defined( _LIBPFF_FILE_H )
#define _LIBPFF_FILE_H

#include <common.h>
#include <types.h>

#include "libpff_descriptors_index.h"
#include "libpff_extern.h"
#include "libpff_file_header.h"
#include "libpff_io_handle.h"
#include "libpff_item_tree.h"
#include "libpff_libbfio.h"
#include "libpff_libcdata.h"
#include "libpff_libcerror.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_offsets_index.h"
#include "libpff_types.h"

#if defined( _MSC_VER ) || defined( __BORLANDC__ ) || defined( __MINGW32_VERSION ) || defined( __MINGW64_VERSION_MAJOR )

/* This inclusion is needed otherwise some linkers
 * mess up exporting the legacy functions
 */
#include "libpff_legacy.h"
#endif

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_internal_file libpff_internal_file_t;

struct libpff_internal_file
{
	/* The IO handle
	 */
	libpff_io_handle_t *io_handle;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* Value to indicate if the file IO handle was created inside the library
	 */
	uint8_t file_io_handle_created_in_library;

	/* Value to indicate if the file IO handle was opened inside the library
	 */
	uint8_t file_io_handle_opened_in_library;

	/* The file header
	 */
	libpff_file_header_t *file_header;

	/* The descriptors index
	 */
	libpff_descriptors_index_t *descriptors_index;

	/* The offsets index
	 */
	libpff_offsets_index_t *offsets_index;

	/* The item tree
	 */
	libpff_item_tree_t *item_tree;

	/* The root folder item tree node
	 */
	libcdata_tree_node_t *root_folder_item_tree_node;

	/* The orphan item list
	 */
	libcdata_list_t *orphan_item_list;

	/* The recovered item list
	 */
	libcdata_list_t *recovered_item_list;

	/* Value to indicate if the allocation tables
	 * have been read
	 */
	int read_allocation_tables;

	/* The unallocated data block (offset) list
	 */
	libcdata_range_list_t *unallocated_data_block_list;

	/* The unallocated page block (offset) list
	 */
	libcdata_range_list_t *unallocated_page_block_list;

	/* The name to id map list
	 */
	libcdata_list_t *name_to_id_map_list;

	/* The content type
	 */
	int content_type;
};

LIBPFF_EXTERN \
int libpff_file_initialize(
     libpff_file_t **file,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_file_free(
     libpff_file_t **file,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_file_signal_abort(
     libpff_file_t *file,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_file_open(
     libpff_file_t *file,
     const char *filename,
     int access_flags,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBPFF_EXTERN \
int libpff_file_open_wide(
     libpff_file_t *file,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

LIBPFF_EXTERN \
int libpff_file_open_file_io_handle(
     libpff_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_file_close(
     libpff_file_t *file,
     libcerror_error_t **error );

int libpff_internal_file_open_read(
     libpff_internal_file_t *internal_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libpff_internal_file_read_allocation_tables(
     libpff_internal_file_t *internal_file,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_file_is_corrupted(
     libpff_file_t *file,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_file_recover_items(
     libpff_file_t *file,
     uint8_t recovery_flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_file_get_size(
     libpff_file_t *file,
     size64_t *size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_file_get_content_type(
     libpff_file_t *file,
     uint8_t *content_type,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_file_get_type(
     libpff_file_t *file,
     uint8_t *type,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_file_get_encryption_type(
     libpff_file_t *file,
     uint8_t *encryption_type,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_file_get_ascii_codepage(
     libpff_file_t *file,
     int *ascii_codepage,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_file_set_ascii_codepage(
     libpff_file_t *file,
     int ascii_codepage,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_file_get_number_of_unallocated_blocks(
     libpff_file_t *file,
     int unallocated_block_type,
     int *number_of_unallocated_blocks,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_file_get_unallocated_block(
     libpff_file_t *file,
     int unallocated_block_type,
     int unallocated_block_index,
     off64_t *offset,
     size64_t *size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_file_get_root_item(
     libpff_file_t *file,
     libpff_item_t **root_item,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_file_get_message_store(
     libpff_file_t *file,
     libpff_item_t **message_store,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_file_get_name_to_id_map(
     libpff_file_t *file,
     libpff_item_t **name_to_id_map,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_file_get_root_folder(
     libpff_file_t *file,
     libpff_item_t **root_folder,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_file_get_item_by_identifier(
     libpff_file_t *file,
     uint32_t item_identifier,
     libpff_item_t **item,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_file_get_number_of_orphan_items(
     libpff_file_t *file,
     int *number_of_orphan_items,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_file_get_orphan_item_by_index(
     libpff_file_t *file,
     int orphan_item_index,
     libpff_item_t **orphan_item,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_file_get_number_of_recovered_items(
     libpff_file_t *file,
     int *number_of_recovered_items,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_file_get_recovered_item_by_index(
     libpff_file_t *file,
     int recovered_item_index,
     libpff_item_t **recovered_item,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_FILE_H ) */

