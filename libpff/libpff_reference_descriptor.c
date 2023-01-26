/*
 * Reference descriptor functions
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
#include "libpff_reference_descriptor.h"

/* Creates a reference descriptor
 * Make sure the value reference_descriptor is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_reference_descriptor_initialize(
     libpff_reference_descriptor_t **reference_descriptor,
     uint32_t value,
     libcerror_error_t **error )
{
	static char *function = "libpff_reference_descriptor_initialize";

	if( reference_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid reference descriptor.",
		 function );

		return( -1 );
	}
	if( *reference_descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid reference descriptor value already set.",
		 function );

		return( -1 );
	}
	*reference_descriptor = memory_allocate_structure(
	                         libpff_reference_descriptor_t );

	if( *reference_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create reference descriptor.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *reference_descriptor,
	     0,
	     sizeof( libpff_reference_descriptor_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear reference descriptor.",
		 function );

		goto on_error;
	}
	( *reference_descriptor )->value = value;

	return( 1 );

on_error:
	if( *reference_descriptor != NULL )
	{
		memory_free(
		 *reference_descriptor );

		*reference_descriptor = NULL;
	}
	return( -1 );
}

/* Frees a reference descriptor
 * Returns 1 if successful or -1 on error
 */
int libpff_reference_descriptor_free(
     libpff_reference_descriptor_t **reference_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libpff_reference_descriptor_free";

	if( reference_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid reference descriptor.",
		 function );

		return( -1 );
	}
	if( *reference_descriptor != NULL )
	{
		memory_free(
		 *reference_descriptor );

		*reference_descriptor = NULL;
	}
	return( 1 );
}

