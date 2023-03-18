/*
 * Local descriptors node functions
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

#if !defined( _LIBPFF_LOCAL_DESCRIPTORS_NODE_H )
#define _LIBPFF_LOCAL_DESCRIPTORS_NODE_H

#include <common.h>
#include <types.h>

#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_local_descriptors_node libpff_local_descriptors_node_t;

struct libpff_local_descriptors_node
{
	/* The level
	 */
	uint8_t level;

	/* The number of entries
	 */
	uint16_t number_of_entries;

	/* The entry size
	 */
	uint8_t entry_size;

	/* The entries data
	 */
	uint8_t *entries_data;

	/* The entries data size
	 */
	size_t entries_data_size;
};

int libpff_local_descriptors_node_initialize(
     libpff_local_descriptors_node_t **local_descriptors_node,
     libcerror_error_t **error );

int libpff_local_descriptors_node_free(
     libpff_local_descriptors_node_t **local_descriptors_node,
     libcerror_error_t **error );

int libpff_local_descriptors_node_get_entry_data(
     libpff_local_descriptors_node_t *local_descriptors_node,
     uint16_t entry_index,
     uint8_t **entry_data,
     libcerror_error_t **error );

int libpff_local_descriptors_node_get_entry_identifier(
     libpff_local_descriptors_node_t *local_descriptors_node,
     libpff_io_handle_t *io_handle,
     uint16_t entry_index,
     uint64_t *entry_identifier,
     libcerror_error_t **error );

int libpff_local_descriptors_node_get_entry_sub_node_identifier(
     libpff_local_descriptors_node_t *local_descriptors_node,
     libpff_io_handle_t *io_handle,
     uint16_t entry_index,
     uint64_t *entry_sub_node_identifier,
     libcerror_error_t **error );

int libpff_local_descriptors_node_read_data(
     libpff_local_descriptors_node_t *local_descriptors_node,
     libpff_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libpff_local_descriptors_node_read_file_io_handle(
     libpff_local_descriptors_node_t *local_descriptors_node,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t descriptor_identifier,
     uint64_t data_identifier,
     off64_t node_offset,
     size32_t node_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_LOCAL_DESCRIPTORS_NODE_H ) */

