/*
 * Recover functions
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

#if !defined( _LIBPFF_RECOVER_H )
#define _LIBPFF_RECOVER_H

#include <common.h>
#include <types.h>

#include "libpff_descriptors_index.h"
#include "libpff_libbfio.h"
#include "libpff_io_handle.h"
#include "libpff_offsets_index.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libpff_recover_items(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_descriptors_index_t *descriptors_index,
     libpff_offsets_index_t *offsets_index,
     libcdata_range_list_t *unallocated_data_block_list,
     libcdata_range_list_t *unallocated_page_block_list,
     libcdata_list_t *recovered_item_list,
     uint8_t recovery_flags,
     libcerror_error_t **error );

int libpff_recover_analyze_descriptor_data_identifier(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     libpff_index_value_t *descriptors_index_value,
     int *data_identifier_value_index,
     libcerror_error_t **error );

int libpff_recover_analyze_descriptor_local_descriptors_identifier(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     libpff_index_value_t *descriptors_index_value,
     int *local_descriptors_identifier_value_index,
     libcerror_error_t **error );

int libpff_recover_analyze_data_block_back_pointer(
     libpff_offsets_index_t *offsets_index,
     uint64_t data_block_back_pointer,
     off64_t data_block_data_offset,
     size32_t data_block_data_size,
     libcerror_error_t **error );

int libpff_recover_analyze_descriptors_index_value(
     libpff_descriptors_index_t *descriptors_index,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_index_value_t *descriptors_index_value,
     libcerror_error_t **error );

int libpff_recover_check_descriptors_index_for_recovered_value(
     libpff_descriptors_index_t *descriptors_index,
     libpff_index_value_t *descriptors_index_value,
     libcerror_error_t **error );

int libpff_recover_descriptors_index_values(
     libpff_descriptors_index_t *descriptors_index,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t node_offset,
     uint64_t node_back_pointer,
     int recursion_depth,
     libcerror_error_t **error );

int libpff_recover_analyze_offsets_index_value(
     libpff_offsets_index_t *offsets_index,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_index_value_t *offsets_index_value,
     uint32_t maximum_data_block_data_size,
     libcerror_error_t **error );

int libpff_recover_check_offsets_index_for_recovered_value(
     libpff_offsets_index_t *offsets_index,
     libpff_index_value_t *offsets_index_value,
     libcerror_error_t **error );

int libpff_recover_analyze_offsets_index_node(
     libpff_offsets_index_t *offsets_index,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t node_offset,
     uint64_t node_back_pointer,
     uint32_t maximum_data_block_data_size,
     int recursion_depth,
     libcerror_error_t **error );

int libpff_recover_data_blocks(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_descriptors_index_t *descriptors_index,
     libpff_offsets_index_t *offsets_index,
     libcdata_range_list_t *unallocated_data_block_list,
     libcdata_range_list_t *unallocated_page_block_list,
     uint8_t recovery_flags,
     libcerror_error_t **error );

int libpff_recover_index_values(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_descriptors_index_t *descriptors_index,
     libpff_offsets_index_t *offsets_index,
     libcdata_range_list_t *unallocated_data_block_list,
     size64_t node_offset,
     uint8_t recovery_flags,
     libcerror_error_t **error );

int libpff_recover_analyze_data_block(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t descriptor_identifier,
     libpff_index_value_t *offsets_index_value,
     libcerror_error_t **error );

int libpff_recover_analyze_local_descriptors(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     uint64_t local_descriptors_identifier,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_RECOVER_H ) */

