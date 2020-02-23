/*
 * Local descriptor value functions
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

#include "libpff_libcerror.h"
#include "libpff_local_descriptor_value.h"

/* Creates a local descriptor value
 * Make sure the value local_descriptor_value is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptor_value_initialize(
     libpff_local_descriptor_value_t **local_descriptor_value,
     libcerror_error_t **error )
{
	static char *function = "libpff_local_descriptor_value_initialize";

	if( local_descriptor_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local descriptor value.",
		 function );

		return( -1 );
	}
	if( *local_descriptor_value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid local descriptor value value already set.",
		 function );

		return( -1 );
	}
	*local_descriptor_value = memory_allocate_structure(
	                           libpff_local_descriptor_value_t );

	if( *local_descriptor_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create local descriptor value.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *local_descriptor_value,
	     0,
	     sizeof( libpff_local_descriptor_value_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear local descriptor value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *local_descriptor_value != NULL )
	{
		memory_free(
		 *local_descriptor_value );

		*local_descriptor_value = NULL;
	}
	return( -1 );
}

/* Frees a local descriptor value
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptor_value_free(
     libpff_local_descriptor_value_t **local_descriptor_value,
     libcerror_error_t **error )
{
	static char *function = "libpff_local_descriptor_value_free";

	if( local_descriptor_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local descriptor value.",
		 function );

		return( -1 );
	}
	if( *local_descriptor_value != NULL )
	{
		memory_free(
		 *local_descriptor_value );

		*local_descriptor_value = NULL;
	}
	return( 1 );
}

/* Clones the local descriptor value
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptor_value_clone(
     libpff_local_descriptor_value_t **destination_local_descriptor_value,
     libpff_local_descriptor_value_t *source_local_descriptor_value,
     libcerror_error_t **error )
{
	static char *function = "libpff_local_descriptor_value_clone";

	if( destination_local_descriptor_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination local descriptor value.",
		 function );

		return( -1 );
	}
	if( *destination_local_descriptor_value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination local descriptor value already set.",
		 function );

		return( -1 );
	}
	if( source_local_descriptor_value == NULL )
	{
		*destination_local_descriptor_value = NULL;

		return( 1 );
	}
	*destination_local_descriptor_value = memory_allocate_structure(
	                                       libpff_local_descriptor_value_t );

	if( *destination_local_descriptor_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination local descriptor value.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_local_descriptor_value,
	     source_local_descriptor_value,
	     sizeof( libpff_local_descriptor_value_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy local descriptor value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_local_descriptor_value != NULL )
	{
		memory_free(
		 *destination_local_descriptor_value );

		*destination_local_descriptor_value = NULL;
	}
	return( -1 );
}

