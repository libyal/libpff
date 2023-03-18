/*
 * Index functions
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

#if !defined( _LIBPFF_INDEX_H )
#define _LIBPFF_INDEX_H

#include <common.h>
#include <types.h>

#include "libpff_block_tree.h"
#include "libpff_index_node.h"
#include "libpff_index_value.h"
#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_index libpff_index_t;

struct libpff_index
{
	/* The index type
	 */
	uint8_t type;

	/* The root node offset
	 */
	off64_t root_node_offset;

	/* The root node back pointer
	 */
	uint64_t root_node_back_pointer;
};

int libpff_index_initialize(
     libpff_index_t **index,
     uint8_t index_type,
     off64_t root_node_offset,
     uint64_t root_node_back_pointer,
     libcerror_error_t **error );

int libpff_index_free(
     libpff_index_t **index,
     libcerror_error_t **error );

int libpff_index_check_if_node_block_first_read(
     libpff_index_t *index,
     libpff_block_tree_t *node_block_tree,
     off64_t node_offset,
     uint64_t identifier,
     libcerror_error_t **error );

int libpff_index_get_leaf_node_from_node_by_identifier(
     libpff_index_t *index,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_block_tree_t *node_block_tree,
     off64_t node_offset,
     uint64_t node_back_pointer,
     uint64_t identifier,
     libpff_index_node_t **leaf_node,
     uint16_t *leaf_node_entry_index,
     libcerror_error_t **error );

int libpff_index_get_value_by_identifier(
     libpff_index_t *index,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint64_t identifier,
     libpff_index_value_t **index_value,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_INDEX_H ) */

