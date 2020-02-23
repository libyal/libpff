/*
 * Index value functions
 *
 * Copyright (C) 2008-2020, Joachim Metz <joachim.metz@gmail.com>
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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libpff_index_value.h"
#include "libpff_libcerror.h"
#include "libpff_libfdata.h"

/* Creates an index value
 * Make sure the value index_value is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_index_value_initialize(
     libpff_index_value_t **index_value,
     libcerror_error_t **error )
{
	static char *function = "libpff_index_value_initialize";

	if( index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index value.",
		 function );

		return( -1 );
	}
	if( *index_value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid index value value already set.",
		 function );

		return( -1 );
	}
	*index_value = memory_allocate_structure(
	                libpff_index_value_t );

	if( *index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create index value.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *index_value,
	     0,
	     sizeof( libpff_index_value_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear index value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *index_value != NULL )
	{
		memory_free(
		 *index_value );

		*index_value = NULL;
	}
	return( -1 );
}

/* Frees an index value
 * Returns 1 if successful or -1 on error
 */
int libpff_index_value_free(
     libpff_index_value_t **index_value,
     libcerror_error_t **error )
{
	static char *function = "libpff_index_value_free";

	if( index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index value.",
		 function );

		return( -1 );
	}
	if( *index_value != NULL )
	{
		memory_free(
		 *index_value );

		*index_value = NULL;
	}
	return( 1 );
}

/* Compares two index values
 * Returns LIBFDATA_COMPARE_LESS, LIBFDATA_COMPARE_EQUAL, LIBFDATA_COMPARE_GREATER if successful or -1 on error
 */
int libpff_index_value_compare(
     libpff_index_value_t *first_index_value,
     libpff_index_value_t *second_index_value,
     libcerror_error_t **error )
{
	static char *function = "libpff_index_value_compare";

	if( first_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid first index value.",
		 function );

		return( -1 );
	}
	if( second_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid second index value.",
		 function );

		return( -1 );
	}
	if( first_index_value->identifier < second_index_value->identifier )
	{
		return( LIBFDATA_COMPARE_LESS );
	}
	else if( first_index_value->identifier > second_index_value->identifier )
	{
		return( LIBFDATA_COMPARE_GREATER );
	}
	return( LIBFDATA_COMPARE_EQUAL );
}

