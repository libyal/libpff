/*
 * Item descriptor functions
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

#include "libpff_item_descriptor.h"
#include "libpff_libcdata.h"
#include "libpff_libcerror.h"

/* Creates an item descriptor
 * Make sure the value item_descriptot is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_item_descriptor_initialize(
     libpff_item_descriptor_t **item_descriptor,
     uint32_t descriptor_identifier,
     uint64_t data_identifier,
     uint64_t local_descriptors_identifier,
     uint8_t recovered,
     libcerror_error_t **error )
{
	static char *function = "libpff_item_descriptor_initialize";

	if( item_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item descriptor.",
		 function );

		return( -1 );
	}
	if( *item_descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid item descriptor value already set.",
		 function );

		return( -1 );
	}
	*item_descriptor = memory_allocate_structure(
	                    libpff_item_descriptor_t );

	if( *item_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create item descriptor.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *item_descriptor,
	     0,
	     sizeof( libpff_item_descriptor_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear item descriptor.",
		 function );

		goto on_error;
	}
	( *item_descriptor )->descriptor_identifier        = descriptor_identifier;
	( *item_descriptor )->data_identifier              = data_identifier;
	( *item_descriptor )->local_descriptors_identifier = local_descriptors_identifier;
	( *item_descriptor )->recovered                    = recovered;

	return( 1 );

on_error:
	if( *item_descriptor != NULL )
	{
		memory_free(
		 *item_descriptor );

		*item_descriptor = NULL;
	}
	return( -1 );
}

/* Frees an item descriptor
 * Returns 1 if successful or -1 on error
 */
int libpff_item_descriptor_free(
     libpff_item_descriptor_t **item_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libpff_item_descriptor_free";

	if( item_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item descriptor.",
		 function );

		return( -1 );
	}
	if( *item_descriptor != NULL )
	{
		memory_free(
		 *item_descriptor );

		*item_descriptor = NULL;
	}
	return( 1 );
}

/* Clones the item descriptor
 * Returns 1 if successful or -1 on error
 */
int libpff_item_descriptor_clone(
     libpff_item_descriptor_t **destination_item_descriptor,
     libpff_item_descriptor_t *source_item_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libpff_item_descriptor_clone";

	if( destination_item_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination item descriptor.",
		 function );

		return( -1 );
	}
	if( *destination_item_descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination item descriptor already set.",
		 function );

		return( -1 );
	}
	if( source_item_descriptor == NULL )
	{
		*destination_item_descriptor = NULL;

		return( 1 );
	}
	*destination_item_descriptor = memory_allocate_structure(
	                                libpff_item_descriptor_t );

	if( *destination_item_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination item descriptor.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_item_descriptor,
	     source_item_descriptor,
	     sizeof( libpff_item_descriptor_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy item descriptor.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_item_descriptor != NULL )
	{
		memory_free(
		 *destination_item_descriptor );

		*destination_item_descriptor = NULL;
	}
	return( -1 );
}

/* Compares two item descriptors
 * Returns LIBCDATA_COMPARE_LESS, LIBCDATA_COMPARE_EQUAL, LIBCDATA_COMPARE_GREATER if successful or -1 on error
 */
int libpff_item_descriptor_compare(
     libpff_item_descriptor_t *first_item_descriptor,
     libpff_item_descriptor_t *second_item_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libpff_item_descriptor_compare";

	if( first_item_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid first item descriptor.",
		 function );

		return( -1 );
	}
	if( second_item_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid second item descriptor.",
		 function );

		return( -1 );
	}
	if( first_item_descriptor->descriptor_identifier < second_item_descriptor->descriptor_identifier )
	{
		return( LIBCDATA_COMPARE_LESS );
	}
	else if( first_item_descriptor->descriptor_identifier > second_item_descriptor->descriptor_identifier )
	{
		return( LIBCDATA_COMPARE_GREATER );
	}
	return( LIBCDATA_COMPARE_EQUAL );
}

/* Retrieves the descriptor identifier of the item
 * Returns 1 if successful or -1 on error
 */
int libpff_item_descriptor_get_descriptor_identifier(
     libpff_item_descriptor_t *item_descriptor,
     uint32_t *identifier,
     libcerror_error_t **error )
{
	static char *function = "libpff_item_descriptor_get_descriptor_identifier";

	if( item_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing item descriptor.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	*identifier = item_descriptor->descriptor_identifier;

	return( 1 );
}

