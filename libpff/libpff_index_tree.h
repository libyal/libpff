/*
 * Index tree functions
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

#if !defined( _LIBPFF_INDEX_TREE_H )
#define _LIBPFF_PAGE_INDEX_H

#include <common.h>
#include <types.h>

#include "libpff_index_value.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libpff_index_tree_get_number_of_leaf_nodes_by_identifier(
     libfdata_tree_t *index_tree,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *cache,
     uint64_t identifier,
     int *number_of_leaf_nodes,
     libcerror_error_t **error );

int libpff_index_tree_node_get_number_of_leaf_nodes_by_identifier(
     libfdata_tree_node_t *index_tree_node,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *cache,
     uint64_t identifier,
     int *number_of_leaf_nodes,
     libcerror_error_t **error );

int libpff_index_tree_get_leaf_node_by_identifier(
     libfdata_tree_t *index_tree,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *cache,
     uint64_t identifier,
     int *leaf_node_index,
     libfdata_tree_node_t **leaf_index_tree_node,
     libcerror_error_t **error );

int libpff_index_tree_node_get_leaf_node_by_identifier(
     libfdata_tree_node_t *index_tree_node,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *cache,
     uint64_t identifier,
     int *leaf_node_index,
     libfdata_tree_node_t **leaf_index_tree_node,
     libcerror_error_t **error );

int libpff_index_tree_get_value_by_identifier(
     libfdata_tree_t *index_tree,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *cache,
     uint64_t identifier,
     int value_index,
     libpff_index_value_t **index_tree_value,
     libcerror_error_t **error );

int libpff_index_tree_node_get_upper_branch_node_by_identifier(
     libfdata_tree_node_t *index_tree_node,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *cache,
     uint64_t identifier,
     libfdata_tree_node_t **upper_branch_index_tree_node,
     libcerror_error_t **error );

int libpff_index_tree_insert_value(
     libfdata_tree_t *index_tree,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *cache,
     uint64_t identifier,
     off64_t node_data_offset,
     size64_t node_data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_INDEX_TREE_H ) */

