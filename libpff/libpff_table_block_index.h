/*
 * Table block index functions
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

#if !defined( _LIBPFF_TABLE_BLOCK_INDEX_H )
#define _LIBPFF_TABLE_BLOCK_INDEX_H

#include <common.h>
#include <types.h>

#include "libpff_libcdata.h"
#include "libpff_libcerror.h"
#include "libpff_table_index_value.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_table_block_index libpff_table_block_index_t;

struct libpff_table_block_index
{
	/* The (index) values array
	 */
	libcdata_array_t *values_array;
};

int libpff_table_block_index_initialize(
     libpff_table_block_index_t **table_block_index,
     libcerror_error_t **error );

int libpff_table_block_index_free(
     libpff_table_block_index_t **table_block_index,
     libcerror_error_t **error );

int libpff_table_block_index_get_number_of_values(
     libpff_table_block_index_t *table_block_index,
     uint16_t *number_of_values,
     libcerror_error_t **error );

int libpff_table_block_index_get_value_by_index(
     libpff_table_block_index_t *table_block_index,
     uint16_t value_index,
     libpff_table_index_value_t **table_index_value,
     libcerror_error_t **error );

int libpff_table_block_index_append_value(
     libpff_table_block_index_t *table_block_index,
     uint16_t *value_index,
     libpff_table_index_value_t *table_index_value,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_TABLE_BLOCK_INDEX_H ) */

