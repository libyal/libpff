/*
 * Input/Output (IO) handle functions
 *
 * Copyright (C) 2008-2016, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _LIBPFF_IO_HANDLE_H )
#define _LIBPFF_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include "libpff_index_value.h"
#include "libpff_libbfio.h"
#include "libpff_libcdata.h"
#include "libpff_libcerror.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const uint8_t pff_file_signature[ 4 ];

typedef struct libpff_io_handle libpff_io_handle_t;

struct libpff_io_handle
{
	/* The index nodes vector
	 */
	libfdata_vector_t *index_nodes_vector;

	/* The index nodes cache
	 */
	libfcache_cache_t *index_nodes_cache;

	/* The file size
	 */
	size64_t file_size;

	/* The encryption type
	 */
	uint8_t encryption_type;

	/* Value to indicate decryption should be forced
	 */
	uint8_t force_decryption;

	/* The file type
	 */
	uint8_t file_type;

	/* Various flags
	 */
	uint8_t flags;

	/* The codepage of the extended ASCII strings
	 */
	int ascii_codepage;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int libpff_io_handle_initialize(
     libpff_io_handle_t **io_handle,
     libcerror_error_t **error );

int libpff_io_handle_free(
     libpff_io_handle_t **io_handle,
     libcerror_error_t **error );

int libpff_io_handle_clear(
     libpff_io_handle_t *io_handle,
     libcerror_error_t **error );

int libpff_io_handle_read_file_header(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     int *file_content_type,
     off64_t *descriptors_index_root_node_offset,
     uint64_t *descriptors_index_root_node_back_pointer,
     off64_t *offsets_index_root_node_offset,
     uint64_t *offsets_index_root_node_back_pointer,
     libcerror_error_t **error );

int libpff_io_handle_read_unallocated_data_blocks(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcdata_range_list_t *unallocated_data_block_list,
     libcerror_error_t **error );

int libpff_io_handle_read_unallocated_page_blocks(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcdata_range_list_t *unallocated_page_block_list,
     libcerror_error_t **error );

int libpff_io_handle_read_index_node(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfcache_cache_t *cache,
     int element_index,
     int element_data_file_index,
     off64_t element_data_offset,
     size64_t element_data_size,
      uint32_t element_data_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_IO_HANDLE_H ) */

