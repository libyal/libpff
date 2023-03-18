/*
 * Offsets index functions
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

#if !defined( _LIBPFF_OFFSETS_INDEX_H )
#define _LIBPFF_OFFSETS_INDEX_H

#include <common.h>
#include <types.h>

#include "libpff_index.h"
#include "libpff_index_value.h"
#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcdata.h"
#include "libpff_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_offsets_index libpff_offsets_index_t;

struct libpff_offsets_index
{
	/* The index
	 */
	libpff_index_t *index;

	/* The recovered index tree
	 */
	libcdata_btree_t *recovered_index_values_tree;
};

int libpff_offsets_index_initialize(
     libpff_offsets_index_t **offsets_index,
     off64_t root_node_offset,
     uint64_t root_node_back_pointer,
     libcerror_error_t **error );

int libpff_offsets_index_free(
     libpff_offsets_index_t **offsets_index,
     libcerror_error_t **error );

int libpff_offsets_index_get_index_value_by_identifier(
     libpff_offsets_index_t *offsets_index,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint64_t data_identifier,
     uint8_t recovered,
     int recovered_value_index,
     libpff_index_value_t **index_value,
     libcerror_error_t **error );

int libpff_offsets_index_insert_recovered_index_value(
     libpff_offsets_index_t *offsets_index,
     libpff_index_value_t *index_value,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_OFFSETS_INDEX_H ) */

