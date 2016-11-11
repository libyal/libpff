/*
 * Local descriptors functions
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

#if !defined( _LIBPFF_LOCAL_DESCRIPTORS_H )
#define _LIBPFF_LOCAL_DESCRIPTORS_H

#include <common.h>
#include <types.h>

#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_local_descriptor_node.h"
#include "libpff_local_descriptor_value.h"
#include "libpff_offsets_index.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_local_descriptors libpff_local_descriptors_t;

struct libpff_local_descriptors
{
	/* The IO handle
	 */
	libpff_io_handle_t *io_handle;

	/* The offsets index
	 */
	libpff_offsets_index_t *offsets_index;

	/* The descriptor identifier
	 */
	uint32_t descriptor_identifier;

	/* The root node offset
	 */
	off64_t root_node_offset;

	/* Value to indicate if the local descriptors were recovered
	 */
	uint8_t recovered;

	/* The local descriptor nodes cache
	 */
	libfcache_cache_t *local_descriptor_nodes_cache;
};

int libpff_local_descriptors_initialize(
     libpff_local_descriptors_t **local_descriptors,
     libpff_io_handle_t *io_handle,
     libpff_offsets_index_t *offsets_index,
     uint32_t descriptor_identifier,
     off64_t root_node_offset,
     uint8_t recovered,
     libcerror_error_t **error );

int libpff_local_descriptors_free(
     libpff_local_descriptors_t **local_descriptors,
     libcerror_error_t **error );

int libpff_local_descriptors_clone(
     libpff_local_descriptors_t **destination_local_descriptors,
     libpff_local_descriptors_t *source_local_descriptors,
     libcerror_error_t **error );

int libpff_local_descriptors_read_local_descriptor_node(
     libpff_local_descriptors_t *local_descriptors,
     libbfio_handle_t *file_io_handle,
     uint64_t data_identifier,
     libpff_local_descriptor_node_t **local_descriptor_node,
     libcerror_error_t **error );

int libpff_local_descriptors_read_tree_node(
     libpff_local_descriptors_t *local_descriptors,
     libbfio_handle_t *file_io_handle,
     off64_t node_offset,
     uint64_t data_identifier,
     libfdata_tree_node_t *local_descriptors_tree_node,
     libcerror_error_t **error );

int libpff_local_descriptors_read_local_descriptor_value(
     libpff_local_descriptors_t *local_descriptors,
     libbfio_handle_t *file_io_handle,
     uint64_t data_identifier,
     uint16_t entry_index,
     libfdata_tree_node_t *local_descriptors_tree_node,
     libpff_local_descriptor_value_t *local_descriptor_value,
     libcerror_error_t **error );

int libpff_local_descriptors_read_node(
     libpff_local_descriptors_t *local_descriptors,
     libbfio_handle_t *file_io_handle,
     libfdata_tree_node_t *node,
     libfcache_cache_t *cache,
     int node_file_index,
     off64_t node_offset,
     size64_t node_size,
     uint32_t node_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

int libpff_local_descriptors_read_sub_nodes(
     libpff_local_descriptors_t *local_descriptors,
     libbfio_handle_t *file_io_handle,
     libfdata_tree_node_t *node,
     libfcache_cache_t *cache,
     int sub_nodes_file_index,
     off64_t sub_nodes_offset,
     size64_t sub_nodes_size,
     uint32_t sub_nodes_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_LOCAL_DESCRIPTORS_H ) */

