/*
 * Index value list functions
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

#include "libpff_index_value.h"
#include "libpff_index_values_list.h"
#include "libpff_libcdata.h"
#include "libpff_libcerror.h"

/* Creates an index value
 * Make sure the value index_values_list is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_index_values_list_initialize(
     libpff_index_values_list_t **index_values_list,
     libcerror_error_t **error )
{
	static char *function = "libpff_index_values_list_initialize";

	if( index_values_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index value.",
		 function );

		return( -1 );
	}
	if( *index_values_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid index value value already set.",
		 function );

		return( -1 );
	}
	*index_values_list = memory_allocate_structure(
	                      libpff_index_values_list_t );

	if( *index_values_list == NULL )
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
	     *index_values_list,
	     0,
	     sizeof( libpff_index_values_list_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear index value.",
		 function );

		memory_free(
		 *index_values_list );

		*index_values_list = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *index_values_list )->index_values_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create index values array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *index_values_list != NULL )
	{
		memory_free(
		 *index_values_list );

		*index_values_list = NULL;
	}
	return( -1 );
}

/* Frees an index value
 * Returns 1 if successful or -1 on error
 */
int libpff_index_values_list_free(
     libpff_index_values_list_t **index_values_list,
     libcerror_error_t **error )
{
	static char *function = "libpff_index_values_list_free";
	int result            = 1;

	if( index_values_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index value.",
		 function );

		return( -1 );
	}
	if( *index_values_list != NULL )
	{
		if( libcdata_array_free(
		     &( ( *index_values_list )->index_values_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_index_value_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free index values array.",
			 function );

			result = -1;
		}
		memory_free(
		 *index_values_list );

		*index_values_list = NULL;
	}
	return( result );
}

/* Compares an index value with an index values list
 * Returns LIBCDATA_COMPARE_LESS, LIBCDATA_COMPARE_EQUAL, LIBCDATA_COMPARE_GREATER if successful or -1 on error
 */
int libpff_index_values_list_compare(
     libpff_index_value_t *lookup_index_value,
     libpff_index_values_list_t *index_values_list,
     libcerror_error_t **error )
{
	static char *function = "libpff_index_values_list_compare";

	if( lookup_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid lookup index value.",
		 function );

		return( -1 );
	}
	if( index_values_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index values list.",
		 function );

		return( -1 );
	}
	if( lookup_index_value->identifier < index_values_list->identifier )
	{
		return( LIBCDATA_COMPARE_LESS );
	}
	else if( lookup_index_value->identifier > index_values_list->identifier )
	{
		return( LIBCDATA_COMPARE_GREATER );
	}
	return( LIBCDATA_COMPARE_EQUAL );
}

/* Retrieves the number of values
 * Returns 1 if successful or -1 on error
 */
int libpff_index_values_list_number_of_values(
     libpff_index_values_list_t *index_values_list,
     int *number_of_values,
     libcerror_error_t **error )
{
	static char *function = "libpff_index_values_list_get_number_of_values";

	if( index_values_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index values list.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     index_values_list->index_values_array,
	     number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values from array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific index value
 * Returns 1 if successful or -1 on error
 */
int libpff_index_values_list_get_value_by_index(
     libpff_index_values_list_t *index_values_list,
     int value_index,
     libpff_index_value_t **index_value,
     libcerror_error_t **error )
{
	static char *function = "libpff_index_values_list_get_value_by_index";

	if( index_values_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index values list.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     index_values_list->index_values_array,
	     value_index,
	     (intptr_t **) index_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve index value: %d from array.",
		 function,
		 value_index );

		return( -1 );
	}
	return( 1 );
}

/* Appends an index value
 * Returns 1 if successful or -1 on error
 */
int libpff_index_values_list_append_value(
     libpff_index_values_list_t *index_values_list,
     libpff_index_value_t *index_value,
     libcerror_error_t **error )
{
	static char *function = "libpff_index_values_list_append_value";
	int entry_index       = 0;

	if( index_values_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index values list.",
		 function );

		return( -1 );
	}
	if( libcdata_array_append_entry(
	     index_values_list->index_values_array,
	     &entry_index,
	     (intptr_t *) index_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append index value to array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

