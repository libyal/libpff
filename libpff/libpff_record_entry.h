/*
 * Record entry functions
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

#if !defined( _LIBPFF_RECORD_ENTRY_H )
#define _LIBPFF_RECORD_ENTRY_H

#include <common.h>
#include <types.h>

#include "libpff_extern.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_name_to_id_map.h"
#include "libpff_record_entry_identifier.h"
#include "libpff_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_internal_record_entry libpff_internal_record_entry_t;

struct libpff_internal_record_entry
{
	/* The record entry identifier
	 */
	libpff_record_entry_identifier_t identifier;

	/* The value data
	 */
	uint8_t *value_data;

	/* The value data size
	 */
	size_t value_data_size;

	/* The value data offset
	 */
	off64_t value_data_offset;

	/* The name to id map entry
	 */
	libpff_internal_name_to_id_map_entry_t *name_to_id_map_entry;

	/* The codepage of the extended ASCII strings
	 */
	int ascii_codepage;

	/* The flags
	 */
	uint8_t flags;
};

int libpff_record_entry_initialize(
     libpff_record_entry_t **record_entry,
     int ascii_codepage,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_free(
     libpff_record_entry_t **record_entry,
     libcerror_error_t **error );

int libpff_internal_record_entry_free(
     libpff_internal_record_entry_t **internal_record_entry,
     libcerror_error_t **error );

int libpff_record_entry_clone(
     libpff_record_entry_t **destination_record_entry,
     libpff_record_entry_t *source_record_entry,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_entry_type(
     libpff_record_entry_t *record_entry,
     uint32_t *entry_type,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_value_type(
     libpff_record_entry_t *record_entry,
     uint32_t *value_type,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_name_to_id_map_entry(
     libpff_record_entry_t *record_entry,
     libpff_name_to_id_map_entry_t **name_to_id_map_entry,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_data_size(
     libpff_record_entry_t *record_entry,
     size_t *data_size,
     libcerror_error_t **error );

int libpff_record_entry_get_value_data(
     libpff_record_entry_t *record_entry,
     uint8_t **value_data,
     size_t *value_data_size,
     libcerror_error_t **error );

int libpff_record_entry_set_value_data(
     libpff_record_entry_t *record_entry,
     const uint8_t *value_data,
     size_t value_data_size,
     libcerror_error_t **error );

int libpff_record_entry_set_value_data_from_list(
     libpff_record_entry_t *record_entry,
     libbfio_handle_t *file_io_handle,
     libfdata_list_t *value_data_list,
     libfcache_cache_t *value_data_cache,
     libcerror_error_t **error );

int libpff_record_entry_set_value_data_from_stream(
     libpff_record_entry_t *record_entry,
     libbfio_handle_t *file_io_handle,
     libfdata_stream_t *value_data_stream,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_data(
     libpff_record_entry_t *record_entry,
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_data_as_boolean(
     libpff_record_entry_t *record_entry,
     uint8_t *value_boolean,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_data_as_16bit_integer(
     libpff_record_entry_t *record_entry,
     uint16_t *value_16bit,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_data_as_32bit_integer(
     libpff_record_entry_t *record_entry,
     uint32_t *value_32bit,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_data_as_64bit_integer(
     libpff_record_entry_t *record_entry,
     uint64_t *value_64bit,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_data_as_filetime(
     libpff_record_entry_t *record_entry,
     uint64_t *filetime,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_data_as_floatingtime(
     libpff_record_entry_t *record_entry,
     uint64_t *floatingtime,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_data_as_size(
     libpff_record_entry_t *record_entry,
     size64_t *value_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_data_as_floating_point(
     libpff_record_entry_t *record_entry,
     double *value_floating_point,
     libcerror_error_t **error );

int libpff_record_entry_get_data_as_utf8_string_size_with_codepage(
     libpff_record_entry_t *record_entry,
     int ascii_codepage,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libpff_record_entry_get_data_as_utf8_string_with_codepage(
     libpff_record_entry_t *record_entry,
     int ascii_codepage,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libpff_record_entry_compare_value_with_utf8_string_with_codepage(
     libpff_record_entry_t *record_entry,
     int ascii_codepage,
     const uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_data_as_utf8_string_size(
     libpff_record_entry_t *record_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_data_as_utf8_string(
     libpff_record_entry_t *record_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libpff_record_entry_get_data_as_utf16_string_size_with_codepage(
     libpff_record_entry_t *record_entry,
     int ascii_codepage,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libpff_record_entry_get_data_as_utf16_string_with_codepage(
     libpff_record_entry_t *record_entry,
     int ascii_codepage,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libpff_record_entry_compare_value_with_utf16_string_with_codepage(
     libpff_record_entry_t *record_entry,
     int ascii_codepage,
     const uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_data_as_utf16_string_size(
     libpff_record_entry_t *record_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_data_as_utf16_string(
     libpff_record_entry_t *record_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_data_as_guid(
     libpff_record_entry_t *record_entry,
     uint8_t *gui_data,
     size_t guid_data_size,
     libcerror_error_t **error );

int libpff_record_entry_copy_object_identifier(
     libpff_record_entry_t *record_entry,
     uint32_t *object_identifier,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_multi_value(
     libpff_record_entry_t *record_entry,
     libpff_multi_value_t **multi_value,
     libcerror_error_t **error );

LIBPFF_EXTERN \
ssize_t libpff_record_entry_read_buffer(
         libpff_record_entry_t *record_entry,
         uint8_t *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBPFF_EXTERN \
off64_t libpff_record_entry_seek_offset(
         libpff_record_entry_t *record_entry,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_RECORD_ENTRY_H ) */

