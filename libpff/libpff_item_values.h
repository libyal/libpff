/*
 * Item values functions
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

#if !defined( _LIBPFF_ITEM_VALUES_H )
#define _LIBPFF_ITEM_VALUES_H

#include <common.h>
#include <types.h>

#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcdata.h"
#include "libpff_libcerror.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_local_descriptor_value.h"
#include "libpff_offsets_index.h"
#include "libpff_table.h"
#include "libpff_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_item_values libpff_item_values_t;

struct libpff_item_values
{
	/* The descriptor identifier value
	 */
	uint32_t descriptor_identifier;

	/* The data identifier value
	 */
	uint64_t data_identifier;

	/* The local descriptors identifier value
	 */
	uint64_t local_descriptors_identifier;

	/* Value to indicate if the item was recovered
	 */
	uint8_t recovered;

	/* The value index of the recovered data identifier
	 */
	int recovered_data_identifier_value_index;

	/* The value index of the recovered local descriptors identifier
	 */
	int recovered_local_descriptors_identifier_value_index;

	/* The item table
	 */
	libpff_table_t *table;
};

int libpff_item_values_initialize(
     libpff_item_values_t **item_values,
     uint32_t descriptor_identifier,
     uint64_t data_identifier,
     uint64_t local_descriptors_identifier,
     uint8_t recovered,
     libcerror_error_t **error );

int libpff_item_values_free(
     libpff_item_values_t **item_values,
     libcerror_error_t **error );

int libpff_item_values_clone_copy(
     libpff_item_values_t *destination_item_values,
     libpff_item_values_t *source_item_values,
     libcerror_error_t **error );

int libpff_item_values_read(
     libpff_item_values_t *item_values,
     libcdata_list_t *name_to_id_map_list,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     int debug_item_type,
     libcerror_error_t **error );

int libpff_item_values_get_local_descriptors_value_by_identifier(
     libpff_item_values_t *item_values,
     libbfio_handle_t *file_io_handle,
     uint32_t descriptor_identifier,
     libpff_local_descriptor_value_t **local_descriptor_value,
     libcerror_error_t **error );

int libpff_item_values_get_number_of_record_sets(
     libpff_item_values_t *item_values,
     libcdata_list_t *name_to_id_map_list,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     int *number_of_record_sets,
     libcerror_error_t **error );

int libpff_item_values_get_record_entry_by_type(
     libpff_item_values_t *item_values,
     libcdata_list_t *name_to_id_map_list,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     int record_set_index,
     uint32_t entry_type,
     uint32_t value_type,
     libpff_record_entry_t **record_entry,
     uint8_t flags,
     libcerror_error_t **error );

int libpff_item_values_get_record_entry_by_utf8_name(
     libpff_item_values_t *item_values,
     libcdata_list_t *name_to_id_map_list,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     int record_set_index,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint32_t value_type,
     libpff_record_entry_t **record_entry,
     uint8_t flags,
     libcerror_error_t **error );

int libpff_item_values_get_record_entry_by_utf16_name(
     libpff_item_values_t *item_values,
     libcdata_list_t *name_to_id_map_list,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     int record_set_index,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint32_t value_type,
     libpff_record_entry_t **record_entry,
     uint8_t flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_ITEM_VALUES_H ) */

