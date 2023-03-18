/*
 * Data block functions
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

#if !defined( _LIBPFF_DATA_BLOCK_H )
#define _LIBPFF_DATA_BLOCK_H

#include <common.h>
#include <types.h>

#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_data_block libpff_data_block_t;

struct libpff_data_block
{
	/* Value to indicate if the descriptor identifier
	 */
	uint32_t descriptor_identifier;

	/* Value to indicate if the data identifier
	 */
	uint64_t data_identifier;

	/* A reference to the IO handle
	 */
	libpff_io_handle_t *io_handle;

	/* The (block) data
	 */
	uint8_t *data;

	/* The (block data) size
	 */
	size_t size;

	/* The data size
	 */
	uint32_t data_size;

	/* The uncompressed data size
	 */
	uint32_t uncompressed_data_size;

	/* The stored checksum
	 */
	uint32_t stored_checksum;

	/* The flags
	 */
	uint8_t flags;
};

int libpff_data_block_initialize(
     libpff_data_block_t **data_block,
     libpff_io_handle_t *io_handle,
     uint32_t descriptor_identifier,
     uint64_t data_identifier,
     libcerror_error_t **error );

int libpff_data_block_free(
     libpff_data_block_t **data_block,
     libcerror_error_t **error );

int libpff_data_block_clone(
     libpff_data_block_t **destination_data_block,
     libpff_data_block_t *source_data_block,
     libcerror_error_t **error );

int libpff_data_block_read_footer_data(
     libpff_data_block_t *data_block,
     const uint8_t *data,
     size_t data_size,
     uint8_t file_type,
     libcerror_error_t **error );

int libpff_data_block_read_file_io_handle(
     libpff_data_block_t *data_block,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     size32_t data_size,
     uint8_t file_type,
     libcerror_error_t **error );

int libpff_data_block_read_element_data(
     libpff_data_block_t *data_block,
     libbfio_handle_t *file_io_handle,
     libfdata_list_element_t *list_element,
     libfcache_cache_t *cache,
     int element_file_index,
     off64_t element_offset,
     size64_t element_size,
     uint32_t element_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

int libpff_data_block_decrypt_data(
     libpff_data_block_t *data_block,
     uint8_t read_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_DATA_BLOCK_H ) */

