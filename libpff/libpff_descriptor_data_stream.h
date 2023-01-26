/*
 * The descriptor data stream functions
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

#if !defined( _LIBPFF_DESCRIPTOR_DATA_STREAM_H )
#define _LIBPFF_DESCRIPTOR_DATA_STREAM_H

#include <common.h>
#include <types.h>

#include "libpff_libcerror.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_descriptor_data_stream_data_handle libpff_descriptor_data_stream_data_handle_t;

struct libpff_descriptor_data_stream_data_handle
{
	/* The current offset
	 */
	off64_t current_offset;

	/* The descriptor data list
	 */
	libfdata_list_t *descriptor_data_list;

	/* The descriptor data cache
	 */
	libfcache_cache_t *descriptor_data_cache;

	/* The flags
	 */
	uint8_t flags;
};

int libpff_descriptor_data_stream_data_handle_initialize(
     libpff_descriptor_data_stream_data_handle_t **data_handle,
     libcerror_error_t **error );

int libpff_descriptor_data_stream_data_handle_free(
     libpff_descriptor_data_stream_data_handle_t **data_handle,
     libcerror_error_t **error );

int libpff_descriptor_data_stream_data_handle_clone(
     libpff_descriptor_data_stream_data_handle_t **destination_data_handle,
     libpff_descriptor_data_stream_data_handle_t *source_data_handle,
     libcerror_error_t **error );

ssize_t libpff_descriptor_data_stream_data_handle_read_segment_data(
         libpff_descriptor_data_stream_data_handle_t *data_handle,
         intptr_t *file_io_handle,
         int segment_index,
         int segment_file_index,
         uint8_t *segment_data,
         size_t segment_data_size,
         uint32_t segment_flags,
         uint8_t read_flags,
         libcerror_error_t **error );

off64_t libpff_descriptor_data_stream_data_handle_seek_segment_offset(
         libpff_descriptor_data_stream_data_handle_t *data_handle,
         intptr_t *file_io_handle,
         int segment_index,
         int segment_file_index,
         off64_t segment_offset,
         libcerror_error_t **error );

int libpff_descriptor_data_stream_initialize(
     libfdata_stream_t **descriptor_data_stream,
     libfdata_list_t *descriptor_data_list,
     libfcache_cache_t *descriptor_data_cache,
     uint8_t flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_DESCRIPTOR_DATA_STREAM_H ) */

