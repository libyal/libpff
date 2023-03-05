/*
 * Index values list functions
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

#if !defined( _LIBPFF_INDEX_VALUES_LIST_H )
#define _LIBPFF_INDEX_VALUES_LIST_H

#include <common.h>
#include <types.h>

#include "libpff_libcdata.h"
#include "libpff_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_index_values_list libpff_index_values_list_t;

struct libpff_index_values_list
{
	/* The identifier value
	 */
	uint64_t identifier;

	/* The index values array
	 */
	libcdata_array_t *index_values_array;
};

int libpff_index_values_list_initialize(
     libpff_index_values_list_t **index_values_list,
     libcerror_error_t **error );

int libpff_index_values_list_free(
     libpff_index_values_list_t **index_values_list,
     libcerror_error_t **error );

int libpff_index_values_list_compare(
     libpff_index_value_t *lookup_index_value,
     libpff_index_values_list_t *index_values_list,
     libcerror_error_t **error );

int libpff_index_values_list_number_of_values(
     libpff_index_values_list_t *index_values_list,
     int *number_of_values,
     libcerror_error_t **error );

int libpff_index_values_list_get_value_by_index(
     libpff_index_values_list_t *index_values_list,
     int value_index,
     libpff_index_value_t **index_value,
     libcerror_error_t **error );

int libpff_index_values_list_append_value(
     libpff_index_values_list_t *index_values_list,
     libpff_index_value_t *index_value,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_INDEX_VALUES_LIST_H ) */

