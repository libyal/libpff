/*
 * Descriptors index functions
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

#if !defined( _LIBPFF_DESCRIPTORS_INDEX_H )
#define _LIBPFF_DESCRIPTORS_INDEX_H

#include <common.h>
#include <types.h>

#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_descriptors_index libpff_descriptors_index_t;

struct libpff_descriptors_index
{
	/* The IO handle
	 */
	libpff_io_handle_t *io_handle;

	/* The index tree
	 */
	libfdata_tree_t *index_tree;

	/* The recovered index tree
	 */
	libfdata_tree_t *recovered_index_tree;

	/* The index cache
	 */
	libfcache_cache_t *index_cache;
};

int libpff_descriptors_index_initialize(
     libpff_descriptors_index_t **descriptors_index,
     libpff_io_handle_t *io_handle,
     libcerror_error_t **error );

int libpff_descriptors_index_free(
     libpff_descriptors_index_t **descriptors_index,
     libcerror_error_t **error );

int libpff_descriptors_index_set_root_node(
     libpff_descriptors_index_t *descriptors_index,
     off64_t root_node_offset,
     uint64_t root_node_back_pointer,
     uint8_t recovered,
     libcerror_error_t **error );

int libpff_descriptors_index_get_index_value_by_identifier(
     libpff_descriptors_index_t *descriptors_index,
     libbfio_handle_t *file_io_handle,
     uint32_t descriptor_identifier,
     uint8_t recovered,
     libpff_index_value_t **index_value,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_DESCRIPTORS_INDEX_H ) */

