/*
 * Data array entry functions
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

#include "libpff_data_array_entry.h"
#include "libpff_libcerror.h"

/* Creates a data array entry
 * Make sure the value data_array_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_data_array_entry_initialize(
     libpff_data_array_entry_t **data_array_entry,
     libcerror_error_t **error )
{
	static char *function = "libpff_data_array_entry_initialize";

	if( data_array_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data array entry.",
		 function );

		return( -1 );
	}
	if( *data_array_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data array entry value already set.",
		 function );

		return( -1 );
	}
	*data_array_entry = memory_allocate_structure(
	                     libpff_data_array_entry_t );

	if( *data_array_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data array entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *data_array_entry,
	     0,
	     sizeof( libpff_data_array_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data array entry.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *data_array_entry != NULL )
	{
		memory_free(
		 *data_array_entry );

		*data_array_entry = NULL;
	}
	return( -1 );
}

/* Frees a data array entry
 * Returns 1 if successful or -1 on error
 */
int libpff_data_array_entry_free(
     libpff_data_array_entry_t **data_array_entry,
     libcerror_error_t **error )
{
	static char *function = "libpff_data_array_entry_free";

	if( data_array_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data array entry.",
		 function );

		return( -1 );
	}
	if( *data_array_entry != NULL )
	{
		memory_free(
		 *data_array_entry );

		*data_array_entry = NULL;
	}
	return( 1 );
}

/* Clones the data array entry
 * Returns 1 if successful or -1 on error
 */
int libpff_data_array_entry_clone(
     libpff_data_array_entry_t **destination_data_array_entry,
     libpff_data_array_entry_t *source_data_array_entry,
     libcerror_error_t **error )
{
	static char *function = "libpff_data_array_entry_clone";

	if( destination_data_array_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination data array entry.",
		 function );

		return( -1 );
	}
	if( *destination_data_array_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination data array entry already set.",
		 function );

		return( -1 );
	}
	if( source_data_array_entry == NULL )
	{
		*destination_data_array_entry = NULL;

		return( 1 );
	}
	*destination_data_array_entry = memory_allocate_structure(
	                                 libpff_data_array_entry_t );

	if( *destination_data_array_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination data array entry.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_data_array_entry,
	     source_data_array_entry,
	     sizeof( libpff_data_array_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy data array entry.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_data_array_entry != NULL )
	{
		memory_free(
		 *destination_data_array_entry );

		*destination_data_array_entry = NULL;
	}
	return( -1 );
}

