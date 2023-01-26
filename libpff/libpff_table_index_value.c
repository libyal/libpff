/*
 * Table index value functions
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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libpff_libcerror.h"
#include "libpff_table_index_value.h"

/* Creates a table index value
 * Make sure the value table_index_value is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_table_index_value_initialize(
     libpff_table_index_value_t **table_index_value,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_index_value_initialize";

	if( table_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table index value.",
		 function );

		return( -1 );
	}
	if( *table_index_value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid table index value value already set.",
		 function );

		return( -1 );
	}
	*table_index_value = memory_allocate_structure(
	                      libpff_table_index_value_t );

	if( *table_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create table index value.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *table_index_value,
	     0,
	     sizeof( libpff_table_index_value_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear table index value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *table_index_value != NULL )
	{
		memory_free(
		 *table_index_value );

		*table_index_value = NULL;
	}
	return( -1 );
}

/* Frees a table index value
 * Returns 1 if successful or -1 on error
 */
int libpff_table_index_value_free(
     libpff_table_index_value_t **table_index_value,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_index_value_free";

	if( table_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table index value.",
		 function );

		return( -1 );
	}
	if( *table_index_value != NULL )
	{
		memory_free(
		 *table_index_value );

		*table_index_value = NULL;
	}
	return( 1 );
}

