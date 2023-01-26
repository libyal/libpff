/*
 * Legacy functions
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

#if !defined( _LIBPFF_LEGACY_H )
#define _LIBPFF_LEGACY_H

#include <common.h>
#include <types.h>

#include "libpff_extern.h"
#include "libpff_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

LIBPFF_EXTERN \
int libpff_file_get_orphan_item(
     libpff_file_t *file,
     int orphan_item_index,
     libpff_item_t **orphan_item,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_file_get_recovered_item(
     libpff_file_t *file,
     int recovered_item_index,
     libpff_item_t **recovered_item,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_clone(
     libpff_item_t **destination_item,
     libpff_item_t *source_item,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_number_of_sets(
     libpff_item_t *item,
     uint32_t *number_of_sets,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_type(
     libpff_item_t *item,
     int set_index,
     int entry_index,
     uint32_t *entry_type,
     uint32_t *value_type,
     libpff_name_to_id_map_entry_t **name_to_id_map_entry,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_value_type(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint32_t *value_type,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint32_t *value_type,
     uint8_t **value_data,
     size_t *value_data_size,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     uint32_t *value_type,
     uint8_t **value_data,
     size_t *value_data_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     uint32_t *value_type,
     uint8_t **value_data,
     size_t *value_data_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_boolean(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint8_t *entry_value,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_boolean_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     uint8_t *entry_value,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_boolean_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     uint8_t *entry_value,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_16bit(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint16_t *entry_value,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_16bit_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     uint16_t *entry_value,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_16bit_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     uint16_t *entry_value,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_32bit(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint32_t *entry_value,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_32bit_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     uint32_t *entry_value,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_32bit_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     uint32_t *entry_value,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_64bit(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint64_t *entry_value,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_64bit_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     uint64_t *entry_value,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_64bit_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     uint64_t *entry_value,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_filetime(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint64_t *entry_value,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_filetime_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     uint64_t *entry_value,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_filetime_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     uint64_t *entry_value,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_size(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     size_t *entry_value,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_size_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     size_t *entry_value,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_size_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     size_t *entry_value,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_floating_point(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     double *entry_value,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_floating_point_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     double *entry_value,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_floating_point_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     double *entry_value,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_utf8_string_size(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     size_t *utf8_string_size,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_utf8_string_size_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_utf8_string_size_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_utf8_string(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_utf8_string_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_utf8_string_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_utf16_string_size(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     size_t *utf16_string_size,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_utf16_string_size_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_utf16_string_size_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_utf16_string(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_utf16_string_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_utf16_string_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_binary_data_size(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     size_t *binary_data_size,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_binary_data_size_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     size_t *binary_data_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_binary_data_size_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     size_t *binary_data_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_binary_data(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint8_t *binary_data,
     size_t binary_data_size,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_binary_data_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     uint8_t *binary_data,
     size_t binary_data_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_binary_data_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     uint8_t *binary_data,
     size_t binary_data_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_value_guid(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint8_t *guid,
     size_t guid_size,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_item_get_entry_multi_value(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     libpff_multi_value_t **multi_value,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_value_data_size(
     libpff_record_entry_t *record_entry,
     size_t *value_data_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_copy_value_data(
     libpff_record_entry_t *record_entry,
     uint8_t *value_data,
     size_t value_data_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_value_boolean(
     libpff_record_entry_t *record_entry,
     uint8_t *value_boolean,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_value_16bit(
     libpff_record_entry_t *record_entry,
     uint16_t *value_16bit,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_value_32bit(
     libpff_record_entry_t *record_entry,
     uint32_t *value_32bit,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_value_64bit(
     libpff_record_entry_t *record_entry,
     uint64_t *value_64bit,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_value_filetime(
     libpff_record_entry_t *record_entry,
     uint64_t *value_64bit,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_value_size(
     libpff_record_entry_t *record_entry,
     size_t *value_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_value_floating_point(
     libpff_record_entry_t *record_entry,
     double *value_floating_point,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_value_utf8_string_size(
     libpff_record_entry_t *record_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_value_utf8_string(
     libpff_record_entry_t *record_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_value_utf16_string_size(
     libpff_record_entry_t *record_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_entry_get_value_utf16_string(
     libpff_record_entry_t *record_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_LEGACY_H ) */

