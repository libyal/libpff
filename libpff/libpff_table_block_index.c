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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libpff_libcdata.h"
#include "libpff_libcerror.h"
#include "libpff_table_block_index.h"
#include "libpff_table_index_value.h"

/* Creates table block index
 * Make sure the value table_block_index is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_table_block_index_initialize(
     libpff_table_block_index_t **table_block_index,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_block_index_initialize";

	if( table_block_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table block index.",
		 function );

		return( -1 );
	}
	if( *table_block_index != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid table block index value already set.",
		 function );

		return( -1 );
	}
	*table_block_index = memory_allocate_structure(
	                      libpff_table_block_index_t );

	if( *table_block_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create table block index.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *table_block_index,
	     0,
	     sizeof( libpff_table_block_index_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear table block index.",
		 function );

		memory_free(
		 *table_block_index );

		*table_block_index = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *table_block_index )->values_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create values array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *table_block_index != NULL )
	{
		memory_free(
		 *table_block_index );

		*table_block_index = NULL;
	}
	return( -1 );
}

/* Frees table block index
 * Returns 1 if successful or -1 on error
 */
int libpff_table_block_index_free(
     libpff_table_block_index_t **table_block_index,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_block_index_free";
	int result            = 1;

	if( table_block_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table block index.",
		 function );

		return( -1 );
	}
	if( *table_block_index != NULL )
	{
		if( libcdata_array_free(
		     &( ( *table_block_index )->values_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_table_index_value_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free values array.",
			 function );

			result = -1;
		}
		memory_free(
		 *table_block_index );

		*table_block_index = NULL;
	}
	return( result );
}

/* Retrieves the number of values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_block_index_get_number_of_values(
     libpff_table_block_index_t *table_block_index,
     uint16_t *number_of_values,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_block_index_get_number_of_values";
	int number_of_entries = 0;

	if( table_block_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table block index.",
		 function );

		return( -1 );
	}
	if( number_of_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of values.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     table_block_index->values_array,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries.",
		 function );

		return( -1 );
	}
	if( ( number_of_entries < 0 )
	 || ( number_of_entries > (int) UINT16_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of entries value out of bounds.",
		 function );

		return( -1 );
	}
	*number_of_values = (uint16_t) number_of_entries;

	return( 1 );
}

/* Retrieves a specific table index value
 * Returns 1 if successful or -1 on error
 */
int libpff_table_block_index_get_value_by_index(
     libpff_table_block_index_t *table_block_index,
     uint16_t value_index,
     libpff_table_index_value_t **table_index_value,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_block_index_get_value_by_index";

	if( table_block_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table block index.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     table_block_index->values_array,
	     (int) value_index,
	     (intptr_t **) table_index_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %" PRIu16 ".",
		 function,
		 value_index );

		return( -1 );
	}
	return( 1 );
}

/* Appends a table index value
 * Returns 1 if successful or -1 on error
 */
int libpff_table_block_index_append_value(
     libpff_table_block_index_t *table_block_index,
     uint16_t *value_index,
     libpff_table_index_value_t *table_index_value,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_block_index_append_value";
	int entry_index       = 0;

	if( table_block_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table block index.",
		 function );

		return( -1 );
	}
	if( value_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value index.",
		 function );

		return( -1 );
	}
	if( libcdata_array_append_entry(
	     table_block_index->values_array,
	     &entry_index,
	     (intptr_t *) table_index_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append table index value to array.",
		 function );

		return( -1 );
	}
	if( ( entry_index < 0 )
	 || ( entry_index > (int) UINT16_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid entry index value out of bounds.",
		 function );

		return( -1 );
	}
	*value_index = (uint16_t) entry_index;

	return( 1 );
}

