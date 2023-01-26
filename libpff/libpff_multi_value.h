/*
 * Multi value functions
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

#if !defined( _LIBPFF_MULTI_VALUE_H )
#define _LIBPFF_MULTI_VALUE_H

#include <common.h>
#include <types.h>

#include "libpff_extern.h"
#include "libpff_libcerror.h"
#include "libpff_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_internal_multi_value libpff_internal_multi_value_t;

struct libpff_internal_multi_value
{
	/* The multi value type
	 */
	uint32_t value_type;

	/* The value data
	 */
	uint8_t *value_data;

	/* The value data size
	 */
	size_t value_data_size;

	/* The number of values
	 */
	uint32_t number_of_values;

	/* The value byte offsets
	 */
	uint32_t *value_offset;

	/* The value byte sizes
	 */
	size_t *value_size;

	/* A copy of the ASCII codepage
	 */
	int ascii_codepage;
};

int libpff_multi_value_initialize(
     libpff_multi_value_t **multi_value,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_multi_value_free(
     libpff_multi_value_t **multi_value,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_multi_value_get_number_of_values(
     libpff_multi_value_t *multi_value,
     int *number_of_values,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_multi_value_get_value(
     libpff_multi_value_t *multi_value,
     int value_index,
     uint32_t *value_type,
     uint8_t **value_data,
     size_t *value_data_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_multi_value_get_value_32bit(
     libpff_multi_value_t *multi_value,
     int value_index,
     uint32_t *value,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_multi_value_get_value_64bit(
     libpff_multi_value_t *multi_value,
     int value_index,
     uint64_t *value,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_multi_value_get_value_filetime(
     libpff_multi_value_t *multi_value,
     int value_index,
     uint64_t *filetime,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_multi_value_get_value_utf8_string_size(
     libpff_multi_value_t *multi_value,
     int value_index,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_multi_value_get_value_utf8_string(
     libpff_multi_value_t *multi_value,
     int value_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_multi_value_get_value_utf16_string_size(
     libpff_multi_value_t *multi_value,
     int value_index,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_multi_value_get_value_utf16_string(
     libpff_multi_value_t *multi_value,
     int value_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_multi_value_get_value_binary_data_size(
     libpff_multi_value_t *multi_value,
     int value_index,
     size_t *size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_multi_value_get_value_binary_data(
     libpff_multi_value_t *multi_value,
     int value_index,
     uint8_t *binary_data,
     size_t size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_multi_value_get_value_guid(
     libpff_multi_value_t *multi_value,
     int value_index,
     uint8_t *guid,
     size_t size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_MULTI_VALUE_H ) */

