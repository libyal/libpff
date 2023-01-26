/*
 * Values type functions
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

#if !defined( _LIBPFF_VALUE_TYPE_H )
#define _LIBPFF_VALUE_TYPE_H

#include <common.h>
#include <types.h>

#include "libpff_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libpff_value_type_string_contains_zero_bytes(
     const uint8_t *buffer,
     size_t buffer_size,
     libcerror_error_t **error );

int libpff_value_type_copy_to_32bit(
     const uint8_t *value_data,
     size_t value_data_size,
     uint32_t *value_32bit,
     libcerror_error_t **error );

int libpff_value_type_copy_to_64bit(
     const uint8_t *value_data,
     size_t value_data_size,
     uint64_t *value_64bit,
     libcerror_error_t **error );

int libpff_value_type_get_utf8_string_size(
     const uint8_t *value_data,
     size_t value_data_size,
     uint8_t is_ascii_string,
     uint32_t ascii_codepage,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libpff_value_type_copy_to_utf8_string(
     const uint8_t *value_data,
     size_t value_data_size,
     uint8_t is_ascii_string,
     uint32_t ascii_codepage,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libpff_value_type_get_utf16_string_size(
     const uint8_t *value_data,
     size_t value_data_size,
     uint8_t is_ascii_string,
     uint32_t ascii_codepage,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libpff_value_type_copy_to_utf16_string(
     const uint8_t *value_data,
     size_t value_data_size,
     uint8_t is_ascii_string,
     uint32_t ascii_codepage,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libpff_value_type_get_binary_data_size(
     const uint8_t *value_data,
     size_t value_data_size,
     size_t *binary_data_size,
     libcerror_error_t **error );

int libpff_value_type_copy_to_binary_data(
     const uint8_t *value_data,
     size_t value_data_size,
     uint8_t *binary_data,
     size_t binary_data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_VALUE_TYPE_H ) */

