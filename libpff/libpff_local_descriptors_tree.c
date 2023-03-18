/*
 * Local descriptors tree functions
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

#include "libpff_definitions.h"
#include "libpff_index_value.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_local_descriptor_value.h"
#include "libpff_local_descriptors.h"
#include "libpff_local_descriptors_tree.h"
#include "libpff_offsets_index.h"

/* Creates a local descriptors tree
 * Make sure the value local_descriptors_tree is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptors_tree_initialize(
     libpff_local_descriptors_tree_t **local_descriptors_tree,
     libpff_io_handle_t *io_handle,
     libpff_offsets_index_t *offsets_index,
     uint32_t descriptor_identifier,
     uint64_t root_data_identifier,
     uint8_t recovered,
     int recovered_value_index,
     libcerror_error_t **error )
{
	static char *function = "libpff_local_descriptors_tree_initialize";

	if( local_descriptors_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local descriptors tree.",
		 function );

		return( -1 );
	}
	if( *local_descriptors_tree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid local descriptors tree value already set.",
		 function );

		return( -1 );
	}
	*local_descriptors_tree = memory_allocate_structure(
	                           libpff_local_descriptors_tree_t );

	if( *local_descriptors_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create local descriptors tree.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *local_descriptors_tree,
	     0,
	     sizeof( libpff_local_descriptors_tree_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file.",
		 function );

		memory_free(
		 *local_descriptors_tree );

		*local_descriptors_tree = NULL;

		return( -1 );
	}
	if( libpff_local_descriptors_initialize(
	     &( ( *local_descriptors_tree )->local_descriptors ),
	     io_handle,
	     offsets_index,
	     descriptor_identifier,
	     root_data_identifier,
	     recovered,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create local descriptors.",
		 function );

		goto on_error;
	}
	( *local_descriptors_tree )->io_handle             = io_handle;
	( *local_descriptors_tree )->offsets_index         = offsets_index;
	( *local_descriptors_tree )->descriptor_identifier = descriptor_identifier;
	( *local_descriptors_tree )->root_data_identifier  = root_data_identifier;
	( *local_descriptors_tree )->recovered             = recovered;
	( *local_descriptors_tree )->recovered_value_index = recovered_value_index;

	return( 1 );

on_error:
	if( *local_descriptors_tree != NULL )
	{
		memory_free(
		 *local_descriptors_tree );

		*local_descriptors_tree = NULL;
	}
	return( -1 );
}

/* Frees a local descriptors tree
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptors_tree_free(
     libpff_local_descriptors_tree_t **local_descriptors_tree,
     libcerror_error_t **error )
{
	static char *function = "libpff_local_descriptors_tree_free";
	int result            = 1;

	if( local_descriptors_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local descriptors tree.",
		 function );

		return( -1 );
	}
	if( *local_descriptors_tree != NULL )
	{
		if( libpff_local_descriptors_free(
		     &( ( *local_descriptors_tree )->local_descriptors ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free local descriptors.",
			 function );

			result = -1;
		}
		memory_free(
		 *local_descriptors_tree );

		*local_descriptors_tree = NULL;
	}
	return( result );
}

/* Clones a record set
 * Make sure the value destination_local_descriptors_tree is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptors_tree_clone(
     libpff_local_descriptors_tree_t **destination_local_descriptors_tree,
     libpff_local_descriptors_tree_t *source_local_descriptors_tree,
     libcerror_error_t **error )
{
	static char *function = "libpff_local_descriptors_tree_clone";

	if( destination_local_descriptors_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination local descriptors tree.",
		 function );

		return( -1 );
	}
	if( *destination_local_descriptors_tree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination local descriptors tree value already set.",
		 function );

		return( -1 );
	}
	if( source_local_descriptors_tree == NULL )
	{
		*destination_local_descriptors_tree = NULL;

		return( 1 );
	}
	if( libpff_local_descriptors_tree_initialize(
	     destination_local_descriptors_tree,
	     source_local_descriptors_tree->io_handle,
	     source_local_descriptors_tree->offsets_index,
	     source_local_descriptors_tree->descriptor_identifier,
	     source_local_descriptors_tree->root_data_identifier,
	     source_local_descriptors_tree->recovered,
	     source_local_descriptors_tree->recovered_value_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination local descriptors tree",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_local_descriptors_tree != NULL )
	{
		libpff_local_descriptors_tree_free(
		 destination_local_descriptors_tree,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the local descriptor value for the specific identifier
 * Returns 1 if successful, 0 if no value was found or -1 on error
 */
int libpff_local_descriptors_tree_get_value_by_identifier(
     libpff_local_descriptors_tree_t *local_descriptors_tree,
     libbfio_handle_t *file_io_handle,
     uint64_t identifier,
     libpff_local_descriptor_value_t **local_descriptor_value,
     libcerror_error_t **error )
{
	static char *function = "libpff_local_descriptors_tree_get_value_by_identifier";
	int result            = 0;

	if( local_descriptors_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid local descriptors tree.",
		 function );

		return( -1 );
	}
	result = libpff_local_descriptors_get_value_by_identifier(
		  local_descriptors_tree->local_descriptors,
		  local_descriptors_tree->io_handle,
		  file_io_handle,
		  identifier,
		  local_descriptor_value,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve index value: %" PRIu32 " from index.",
		 function,
		 identifier );

		return( -1 );
	}
	return( result );
}

