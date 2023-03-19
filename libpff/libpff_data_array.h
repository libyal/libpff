/*
 * Data array functions
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

#if !defined( _LIBPFF_DATA_ARRAY_H )
#define _LIBPFF_DATA_ARRAY_H

#include <common.h>
#include <types.h>

#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcdata.h"
#include "libpff_libcerror.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_offsets_index.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_data_array libpff_data_array_t;

struct libpff_data_array
{
	/* Value to indicate if the data array descriptor identifier
	 */
	uint32_t descriptor_identifier;

	/* Value to indicate if the data array data identifier
	 */
	uint64_t data_identifier;

	/* A reference to the IO handle
	 */
	libpff_io_handle_t *io_handle;

	/* The total size of the data in the array
	 */
	size64_t data_size;

	/* The data array entries array
	 */
	libcdata_array_t *entries;

	/* The (data array) flags
	 */
	uint8_t flags;
};

int libpff_data_array_initialize(
     libpff_data_array_t **data_array,
     libpff_io_handle_t *io_handle,
     uint32_t descriptor_identifier,
     uint64_t data_identifier,
     libcerror_error_t **error );

int libpff_data_array_free(
     libpff_data_array_t **data_array,
     libcerror_error_t **error );

int libpff_data_array_clone(
     libpff_data_array_t **destination_data_array,
     libpff_data_array_t *source_data_array,
     libcerror_error_t **error );

int libpff_data_array_read_entries(
     libpff_data_array_t *data_array,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     libfdata_list_t *descriptor_data_list,
     uint8_t recovered,
     uint8_t *array_data,
     size_t array_data_size,
     uint32_t *total_data_size,
     int recursion_depth,
     libcerror_error_t **error );

int libpff_data_array_read_element_data(
     libpff_data_array_t *data_array,
     libbfio_handle_t *file_io_handle,
     libfdata_list_element_t *list_element,
     libfcache_cache_t *cache,
     int element_file_index,
     off64_t element_offset,
     size64_t element_size,
     uint32_t element_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

int libpff_data_array_decrypt_entry_data(
     libpff_data_array_t *data_array,
     int array_entry_index,
     uint8_t encryption_type,
     uint8_t *data,
     size_t data_size,
     uint8_t read_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_DATA_ARRAY_H ) */

