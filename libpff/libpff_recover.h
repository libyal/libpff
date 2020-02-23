/*
 * Recover functions
 *
 * Copyright (C) 2008-2020, Joachim Metz <joachim.metz@gmail.com>
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

int libpff_recover_index_nodes(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_descriptors_index_t *descriptors_index,
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

int libpff_recover_local_descriptors(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     uint64_t local_descriptors_identifier,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_RECOVER_H ) */

