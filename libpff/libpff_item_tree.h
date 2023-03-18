/*
 * Item tree functions
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

#if !defined( _LIBPFF_ITEM_TREE_H )
#define _LIBPFF_ITEM_TREE_H

#include <common.h>
#include <types.h>

#include "libpff_descriptors_index.h"
#include "libpff_index_node.h"
#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcdata.h"
#include "libpff_libcerror.h"
#include "libpff_libfcache.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_item_tree libpff_item_tree_t;

struct libpff_item_tree
{
	/* The root node
	 */
	libcdata_tree_node_t *root_node;
};

int libpff_item_tree_initialize(
     libpff_item_tree_t **item_tree,
     libcerror_error_t **error );

int libpff_item_tree_free(
     libpff_item_tree_t **item_tree,
     libcerror_error_t **error );

int libpff_item_tree_node_free_recovered(
     libcdata_tree_node_t **item_tree_node,
     libcerror_error_t **error );

int libpff_item_tree_get_tree_node_by_identifier(
     libcdata_tree_node_t *item_tree_node,
     uint32_t item_identifier,
     libcdata_tree_node_t **result_item_tree_node,
     int recursion_depth,
     libcerror_error_t **error );

int libpff_item_tree_get_sub_node_by_identifier(
     libcdata_tree_node_t *item_tree_node,
     uint32_t sub_node_identifier,
     libcdata_tree_node_t **sub_node,
     libcerror_error_t **error );

int libpff_item_tree_append_identifier(
     libcdata_tree_node_t *item_tree_node,
     uint32_t descriptor_identifier,
     uint64_t data_identifier,
     uint64_t local_descriptors_identifier,
     uint8_t recovered,
     libcerror_error_t **error );

int libpff_item_tree_create(
     libpff_item_tree_t *item_tree,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_descriptors_index_t *descriptors_index,
     libcdata_list_t *orphan_node_list,
     libcdata_tree_node_t **root_folder_item_tree_node,
     libcerror_error_t **error );

int libpff_item_tree_create_node_from_descriptor_index_node(
     libpff_item_tree_t *item_tree,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_descriptors_index_t *descriptors_index,
     off64_t node_offset,
     libcdata_list_t *orphan_node_list,
     libcdata_tree_node_t **root_folder_item_tree_node,
     int recursion_depth,
     libcerror_error_t **error );

int libpff_item_tree_create_leaf_node_from_descriptor_index_value(
     libpff_item_tree_t *item_tree,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_descriptors_index_t *descriptors_index,
     libpff_index_value_t *descriptor_index_value,
     libcdata_list_t *orphan_node_list,
     libcdata_tree_node_t **root_folder_item_tree_node,
     int recursion_depth,
     libcerror_error_t **error );

int libpff_item_tree_get_node_by_identifier(
     libpff_item_tree_t *item_tree,
     uint32_t item_identifier,
     libcdata_tree_node_t **item_tree_node,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_ITEM_TREE_H ) */

