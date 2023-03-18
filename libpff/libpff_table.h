/*
 * Table functions
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

#if !defined( _LIBPFF_TABLE_H )
#define _LIBPFF_TABLE_H

#include <common.h>
#include <types.h>

#include "libpff_data_block.h"
#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcdata.h"
#include "libpff_libcerror.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_local_descriptor_value.h"
#include "libpff_local_descriptors_tree.h"
#include "libpff_name_to_id_map.h"
#include "libpff_offsets_index.h"
#include "libpff_table_block_index.h"
#include "libpff_table_header.h"
#include "libpff_table_index_value.h"
#include "libpff_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_table libpff_table_t;

struct libpff_table
{
	/* The header
	 */
	libpff_table_header_t *header;

	/* The descriptor identifier value
	 */
	uint32_t descriptor_identifier;

	/* The data identifier value
	 */
	uint64_t data_identifier;

	/* The local descriptors identifier value
	 */
	uint64_t local_descriptors_identifier;

	/* Value to indicate if the table was recovered
	 */
	uint8_t recovered;

	/* The value index of the recovered data identifier
	 */
	int recovered_data_identifier_value_index;

	/* The value index of the recovered local descriptors identifier
	 */
	int recovered_local_descriptors_identifier_value_index;

	/* The descriptor data list
	 */
	libfdata_list_t *descriptor_data_list;

	/* The descriptor data cache
	 */
	libfcache_cache_t *descriptor_data_cache;

	/* The local descriptors tree
	 */
	libpff_local_descriptors_tree_t *local_descriptors_tree;

	/* The local descriptor values cache
	 */
	libfcache_cache_t *local_descriptor_values_cache;

	/* The values array data list
	 */
	libfdata_list_t *values_array_data_list;

	/* The values array data cache
	 */
	libfcache_cache_t *values_array_data_cache;

	/* The number of value array entries per data block
	 */
	int value_array_entries_per_block;

	/* The index array
	 */
	libcdata_array_t *index_array;

	/* The record sets array
	 */
	libcdata_array_t *record_sets_array;

	/* The flags
	 */
	uint8_t flags;
};

typedef struct libpff_table_values_array_entry libpff_table_values_array_entry_t;

struct libpff_table_values_array_entry
{
	/* The identifier
	 */
	uint32_t identifier;

	/* The entry number
	 */
	uint32_t entry_number;
};

int libpff_table_initialize(
     libpff_table_t **table,
     uint32_t descriptor_identifier,
     uint64_t data_identifier,
     uint64_t local_descriptors_identifier,
     uint8_t recovered,
     libcerror_error_t **error );

int libpff_table_free(
     libpff_table_t **table,
     libcerror_error_t **error );

int libpff_table_clone(
     libpff_table_t **destination_table,
     libpff_table_t *source_table,
     libcerror_error_t **error );

int libpff_table_resize_record_entries(
     libpff_table_t *table,
     int number_of_sets,
     int number_of_entries,
     int ascii_codepage,
     libcerror_error_t **error );

int libpff_table_expand_record_entries(
     libpff_table_t *table,
     int number_of_sets,
     int number_of_entries,
     int ascii_codepage,
     libcerror_error_t **error );

int libpff_table_entries_get_record_entry_by_index(
     libpff_table_t *table,
     int set_index,
     int entry_index,
     libpff_record_entry_t **record_entry,
     libcerror_error_t **error );

int libpff_table_get_local_descriptors_value_by_identifier(
     libpff_table_t *table,
     libbfio_handle_t *file_io_handle,
     uint32_t descriptor_identifier,
     libpff_local_descriptor_value_t **local_descriptor_value,
     libcerror_error_t **error );

int libpff_table_get_index_value_by_reference(
     libpff_table_t *table,
     uint32_t table_index_reference,
     libpff_io_handle_t *io_handle,
     libpff_table_index_value_t **table_index_value,
     libcerror_error_t **error );

int libpff_table_get_value_data_by_index_value(
     libpff_table_t *table,
     libpff_table_index_value_t *table_index_value,
     libbfio_handle_t *file_io_handle,
     uint8_t **value_data,
     size_t *value_data_size,
     libcerror_error_t **error );

int libpff_table_get_value_data_by_reference(
     libpff_table_t *table,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t table_index_reference,
     uint8_t **value_data,
     size_t *value_data_size,
     libcerror_error_t **error );

int libpff_table_clone_value_data_by_reference(
     libpff_table_t *table,
     uint32_t table_index_reference,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint8_t **value_data,
     size_t *value_data_size,
     libcerror_error_t **error );

int libpff_table_get_number_of_record_sets(
     libpff_table_t *table,
     int *number_of_record_sets,
     libcerror_error_t **error );

int libpff_table_get_record_set_by_index(
     libpff_table_t *table,
     int record_set_index,
     libpff_record_set_t **record_set,
     libcerror_error_t **error );

int libpff_table_get_number_of_entries(
     libpff_table_t *table,
     int *number_of_entries,
     libcerror_error_t **error );

int libpff_table_get_entry_type_by_index(
     libpff_table_t *table,
     int set_index,
     int entry_index,
     uint32_t *entry_type,
     uint32_t *value_type,
     libpff_name_to_id_map_entry_t **name_to_id_map_entry,
     libcerror_error_t **error );

int libpff_table_get_record_entry_by_index(
     libpff_table_t *table,
     int set_index,
     int entry_index,
     libpff_record_entry_t **record_entry,
     libcerror_error_t **error );

int libpff_table_get_record_entry_by_type(
     libpff_table_t *table,
     int set_index,
     uint32_t entry_type,
     uint32_t value_type,
     libpff_record_entry_t **record_entry,
     uint8_t flags,
     libcerror_error_t **error );

int libpff_table_get_record_entry_by_utf8_name(
     libpff_table_t *table,
     int set_index,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint32_t value_type,
     libpff_record_entry_t **record_entry,
     uint8_t flags,
     libcerror_error_t **error );

int libpff_table_get_record_entry_by_utf16_name(
     libpff_table_t *table,
     int set_index,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint32_t value_type,
     libpff_record_entry_t **record_entry,
     uint8_t flags,
     libcerror_error_t **error );

int libpff_table_read(
     libpff_table_t *table,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     libcdata_list_t *name_to_id_map_list,
     int debug_item_type,
     libcerror_error_t **error );

int libpff_table_read_descriptor_data_list(
     libpff_table_t *table,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     uint32_t descriptor_identifier,
     uint64_t data_identifier,
     uint8_t recovered,
     int recovered_value_index,
     libfdata_list_t **descriptor_data_list,
     libfcache_cache_t **descriptor_data_cache,
     libcerror_error_t **error );

int libpff_table_read_index_entries(
     libpff_table_t *table,
     libpff_data_block_t *data_block,
     libpff_table_block_index_t *table_block_index,
     uint32_t table_array_entry_iterator,
     libcerror_error_t **error );

int libpff_table_read_index(
     libpff_table_t *table,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libpff_table_read_record_entries(
     libpff_table_t *table,
     libcdata_array_t *record_entries_references_array,
     uint8_t record_entries_level,
     uint8_t record_entry_identifier_size,
     uint32_t record_entries_reference,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     int recursion_depth,
     libcerror_error_t **error );

int libpff_table_read_values(
     libpff_table_t *table,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     libcdata_list_t *name_to_id_map_list,
     int debug_item_type,
     libcerror_error_t **error );

int libpff_table_read_6c_values(
     libpff_table_t *table,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libpff_table_read_7c_values(
     libpff_table_t *table,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     libcdata_list_t *name_to_id_map_list,
     libcerror_error_t **error );

int libpff_table_read_8c_values(
     libpff_table_t *table,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libpff_table_read_9c_values(
     libpff_table_t *table,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libpff_table_read_a5_values(
     libpff_table_t *table,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libpff_table_read_ac_values(
     libpff_table_t *table,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     libcdata_list_t *name_to_id_map_list,
     libcerror_error_t **error );

int libpff_table_read_bc_values(
     libpff_table_t *table,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     libcdata_list_t *name_to_id_map_list,
     int debug_item_type,
     libcerror_error_t **error );

int libpff_table_read_6c_record_entries(
     libpff_table_t *table,
     libcdata_array_t *record_entries_references_array,
     uint32_t values_array_reference,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libpff_table_read_7c_column_definitions(
     libpff_table_t *table,
     libcdata_array_t *column_definitions_array,
     uint8_t *column_definitions_data,
     size_t column_definitions_data_size,
     int number_of_column_definitions,
     libbfio_handle_t *file_io_handle,
     libcdata_list_t *name_to_id_map_list,
     libcerror_error_t **error );

int libpff_table_read_8c_record_entries(
     libpff_table_t *table,
     libcdata_array_t *record_entries_references_array,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libpff_table_read_9c_record_entries(
     libpff_table_t *table,
     libcdata_array_t *record_entries_references_array,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libpff_table_read_a5_record_entries(
     libpff_table_t *table,
     uint32_t record_entries_reference,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libpff_table_read_ac_column_definitions(
     libpff_table_t *table,
     libcdata_array_t *column_definitions_array,
     uint32_t column_definitions_reference,
     int number_of_column_definitions,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     libcdata_list_t *name_to_id_map_list,
     libcerror_error_t **error );

int libpff_table_read_bc_record_entries(
     libpff_table_t *table,
     libcdata_array_t *record_entries_references_array,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     libcdata_list_t *name_to_id_map_list,
     int debug_item_type,
     libcerror_error_t **error );

int libpff_table_values_array_get_value_data_by_entry_number(
     libpff_table_t *table,
     uint32_t values_array_reference,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     uint32_t values_array_entry_number,
     uint16_t values_array_entry_size,
     uint8_t **values_array_data,
     size_t *values_array_data_size,
     uint8_t read_flags,
     libcerror_error_t **error );

int libpff_table_read_values_array(
     libpff_table_t *table,
     libcdata_array_t *record_entries_references_array,
     uint32_t values_array_reference,
     uint8_t record_entry_identifier_size,
     uint8_t record_entry_value_size,
     uint16_t values_array_entry_size,
     libcdata_array_t *column_definitions_array,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     libcerror_error_t **error );

int libpff_table_read_entry_value(
     libpff_table_t *table,
     int set_index,
     int entry_index,
     uint32_t record_entry_type,
     uint32_t record_entry_value_type,
     uint8_t *record_entry_value,
     size_t record_entry_value_size,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     libcdata_list_t *name_to_id_map_list,
     libpff_internal_name_to_id_map_entry_t *name_to_id_map_entry,
     libpff_table_t *record_entry_values_table,
     int debug_item_type,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_TABLE_H ) */

