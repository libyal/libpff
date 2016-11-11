/*
 * Index functions
 *
 * Copyright (C) 2008-2016, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _LIBPFF_INDEX_H )
#define _LIBPFF_INDEX_H

#include <common.h>
#include <types.h>

#include "libpff_index_value.h"
#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_index libpff_index_t;

struct libpff_index
{
	/* A reference to the IO handle
	 */
	libpff_io_handle_t *io_handle;

	/* The index type
	 */
	uint8_t type;

	/* The root node offset
	 */
	off64_t root_node_offset;

	/* The root node back pointer
	 */
	uint64_t root_node_back_pointer;

	/* Value to indicate if the index was recovered
	 */
	uint8_t recovered;
};

int libpff_index_initialize(
     libpff_index_t **index,
     libpff_io_handle_t *io_handle,
     uint8_t type,
     off64_t root_node_offset,
     uint64_t root_node_back_pointer,
     uint8_t recovered,
     libcerror_error_t **error );

int libpff_index_free(
     libpff_index_t **index,
     libcerror_error_t **error );

int libpff_index_clone(
     libpff_index_t **destination_index,
     libpff_index_t *source_index,
     libcerror_error_t **error );

int libpff_index_read_node(
     libpff_index_t *index,
     libbfio_handle_t *file_io_handle,
     off64_t node_offset,
     libfdata_tree_node_t *index_tree_node,
     libpff_index_value_t *index_value,
     libcerror_error_t **error );

int libpff_index_read_node_entry(
     libpff_index_t *index,
     libbfio_handle_t *file_io_handle,
     off64_t node_offset,
     uint16_t entry_index,
     libfdata_tree_node_t *index_tree_node,
     libpff_index_value_t *index_value,
     libcerror_error_t **error );

int libpff_index_read_node_data(
     libpff_index_t *index,
     libbfio_handle_t *file_io_handle,
     libfdata_tree_node_t *node,
     libfcache_cache_t *cache,
     int node_data_file_index,
     off64_t node_data_offset,
     size64_t node_data_size,
     uint32_t node_data_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

int libpff_index_read_sub_nodes(
     libpff_index_t *index,
     libbfio_handle_t *file_io_handle,
     libfdata_tree_node_t *node,
     libfcache_cache_t *cache,
     int sub_nodes_data_file_index,
     off64_t sub_nodes_data_offset,
     size64_t sub_nodes_data_size,
     uint32_t sub_nodes_data_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_INDEX_H ) */

