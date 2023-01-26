/*
 * Column definition functions
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

#include "libpff_column_definition.h"
#include "libpff_libcerror.h"
#include "libpff_table.h"
#include "libpff_types.h"

/* Creates a column definition
 * Make sure the value column_definition is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_column_definition_initialize(
     libpff_column_definition_t **column_definition,
     libcerror_error_t **error )
{
	static char *function = "libpff_column_definition_initialize";

	if( column_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid column_definition.",
		 function );

		return( -1 );
	}
	if( *column_definition != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid column definition value already set.",
		 function );

		return( -1 );
	}
	*column_definition = memory_allocate_structure(
	                      libpff_column_definition_t );

	if( *column_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create column definition.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *column_definition,
	     0,
	     sizeof( libpff_column_definition_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear column definition.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *column_definition != NULL )
	{
		memory_free(
		 *column_definition );

		*column_definition = NULL;
	}
	return( -1 );
}

/* Frees a column definition
 * Returns 1 if successful or -1 on error
 */
int libpff_column_definition_free(
     libpff_column_definition_t **column_definition,
     libcerror_error_t **error )
{
	static char *function = "libpff_column_definition_free";
	int result            = 1;

	if( column_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid column definition.",
		 function );

		return( -1 );
	}
	if( *column_definition != NULL )
	{
		/* The name_to_id_map_entry reference is freed elsewhere
		 */
		if( ( *column_definition )->record_entry_values_table != NULL )
		{
			if( libpff_table_free(
			     &( ( *column_definition )->record_entry_values_table ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free record entry values table.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *column_definition );

		*column_definition = NULL;
	}
	return( result );
}

