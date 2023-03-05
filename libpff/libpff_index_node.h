/*
 * Index node functions
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

#if !defined( _LIBPFF_NODE_INDEX_H )
#define _LIBPFF_NODE_INDEX_H

#include <common.h>
#include <types.h>

#include "libpff_index_value.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_index_node libpff_index_node_t;

struct libpff_index_node
{
	/* The node data
	 */
	uint8_t *data;

	/* The node data size
	 */
	size_t data_size;

	/* The type
	 */
	uint8_t type;

	/* The level
	 */
	uint8_t level;

	/* The number of entries
	 */
	uint16_t number_of_entries;

	/* The maximum number of entries
	 */
	uint16_t maximum_number_of_entries;

	/* The entry size
	 */
	uint8_t entry_size;

	/* The entries data
	 */
	uint8_t *entries_data;

	/* The maximum entries data size
	 */
	uint16_t maximum_entries_data_size;

	/* The back pointer
	 */
	uint64_t back_pointer;

	/* The stored checksum
	 */
	uint32_t stored_checksum;
};

int libpff_index_node_initialize(
     libpff_index_node_t **index_node,
     libcerror_error_t **error );

int libpff_index_node_free(
     libpff_index_node_t **index_node,
     libcerror_error_t **error );

int libpff_index_node_get_entry_data(
     libpff_index_node_t *index_node,
     uint16_t entry_index,
     uint8_t **entry_data,
     libcerror_error_t **error );

int libpff_index_node_read_data(
     libpff_index_node_t *index_node,
     const uint8_t *data,
     size_t data_size,
     uint8_t file_type,
     libcerror_error_t **error );

int libpff_index_node_read_footer_data(
     libpff_index_node_t *index_node,
     const uint8_t *data,
     size_t data_size,
     uint8_t file_type,
     libcerror_error_t **error );

int libpff_index_node_read_file_io_handle(
     libpff_index_node_t *index_node,
     libbfio_handle_t *file_io_handle,
     off64_t node_offset,
     uint8_t file_type,
     libcerror_error_t **error );

int libpff_index_node_check_for_empty_block(
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_NODE_INDEX_H ) */

