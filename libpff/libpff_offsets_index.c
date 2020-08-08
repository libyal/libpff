/*
 * Offsets index functions
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

#include "libpff_definitions.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_index.h"
#include "libpff_index_tree.h"
#include "libpff_index_value.h"
#include "libpff_offsets_index.h"

/* Creates an offsets index
 * Make sure the value offsets_index is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_offsets_index_initialize(
     libpff_offsets_index_t **offsets_index,
     libpff_io_handle_t *io_handle,
     libfdata_vector_t *index_nodes_vector,
     libfcache_cache_t *index_nodes_cache,
     libcerror_error_t **error )
{
	static char *function = "libpff_offsets_index_initialize";

	if( offsets_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offsets index.",
		 function );

		return( -1 );
	}
	if( *offsets_index != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid offsets index value already set.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	*offsets_index = memory_allocate_structure(
	                  libpff_offsets_index_t );

	if( *offsets_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create offsets index.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *offsets_index,
	     0,
	     sizeof( libpff_offsets_index_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear offsets index.",
		 function );

		goto on_error;
	}
	( *offsets_index )->io_handle          = io_handle;
	( *offsets_index )->index_nodes_vector = index_nodes_vector;
	( *offsets_index )->index_nodes_cache  = index_nodes_cache;

	return( 1 );

on_error:
	if( *offsets_index != NULL )
	{
		memory_free(
		 *offsets_index );

		*offsets_index = NULL;
	}
	return( -1 );
}

/* Frees an offsets index
 * Returns 1 if successful or -1 on error
 */
int libpff_offsets_index_free(
     libpff_offsets_index_t **offsets_index,
     libcerror_error_t **error )
{
	static char *function = "libpff_offsets_index_free";
	int result            = 1;

	if( offsets_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offsets index.",
		 function );

		return( -1 );
	}
	if( *offsets_index != NULL )
	{
		if( ( *offsets_index )->index_tree != NULL )
		{
			if( libfdata_tree_free(
			     &( ( *offsets_index )->index_tree ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free index tree.",
				 function );

				result = -1;
			}
		}
		if( ( *offsets_index )->recovered_index_tree != NULL )
		{
			if( libfdata_tree_free(
			     &( ( *offsets_index )->recovered_index_tree ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free recovered index tree.",
				 function );

				result = -1;
			}
		}
		if( ( *offsets_index )->index_cache != NULL )
		{
			if( libfcache_cache_free(
			     &( ( *offsets_index )->index_cache ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free index cache.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *offsets_index );

		*offsets_index = NULL;
	}
	return( result );
}

/* Sets the root node
 * Returns 1 if successful or -1 on error
 */
int libpff_offsets_index_set_root_node(
     libpff_offsets_index_t *offsets_index,
     off64_t root_node_offset,
     uint64_t root_node_back_pointer,
     uint8_t recovered,
     libcerror_error_t **error )
{
	static char *function = "libpff_offsets_index_set_root_node";

	if( offsets_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offsets index.",
		 function );

		return( -1 );
	}
	if( recovered == 0 )
	{
		if( libpff_index_tree_initialize(
		     &( offsets_index->index_tree ),
		     offsets_index->io_handle,
		     offsets_index->index_nodes_vector,
		     offsets_index->index_nodes_cache,
		     LIBPFF_INDEX_TYPE_OFFSET,
		     root_node_offset,
		     root_node_back_pointer,
		     recovered,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create index tree.",
			 function );

			goto on_error;
		}
		/* Point the root node data offset to LIBPFF_OFFSETS_INDEX_TREE_ROOT_OFFSET
		 * otherwise it will mess up the caching of the first index value
		 */
		if( libfdata_tree_set_root_node(
		     offsets_index->index_tree,
		     0,
		     LIBPFF_OFFSETS_INDEX_TREE_ROOT_OFFSET,
		     0,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set index tree root node.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( libpff_index_tree_initialize(
		     &( offsets_index->recovered_index_tree ),
		     offsets_index->io_handle,
		     offsets_index->index_nodes_vector,
		     offsets_index->index_nodes_cache,
		     LIBPFF_INDEX_TYPE_OFFSET,
		     0,
		     0,
		     recovered,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create index tree.",
			 function );

			goto on_error;
		}
		/* Point the root node data offset to LIBPFF_RECOVERED_OFFSETS_INDEX_TREE_ROOT_OFFSET
		 * otherwise it will mess up the caching of the first index value
		 */
		if( libfdata_tree_set_root_node(
		     offsets_index->recovered_index_tree,
		     0,
		     LIBPFF_RECOVERED_OFFSETS_INDEX_TREE_ROOT_OFFSET,
		     0,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set recovered index tree root node.",
			 function );

			goto on_error;
		}
	}
	if( offsets_index->index_cache == NULL )
	{
		if( libfcache_cache_initialize(
		     &( offsets_index->index_cache ),
		     LIBPFF_MAXIMUM_CACHE_ENTRIES_OFFSET_INDEX_VALUES,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create index tree cache.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( recovered == 0 )
	{
		if( offsets_index->index_tree != NULL )
		{
			libfdata_tree_free(
			 &( offsets_index->index_tree ),
			 NULL );
		}
	}
	else
	{
		if( offsets_index->recovered_index_tree != NULL )
		{
			libfdata_tree_free(
			 &( offsets_index->recovered_index_tree ),
			 NULL );
		}
	}
	return( -1 );
}

/* Retrieves an index value for a specific identifier
 * Returns 1 if successful, 0 if no index value was found or -1 on error
 */
int libpff_offsets_index_get_index_value_by_identifier(
     libpff_offsets_index_t *offsets_index,
     libbfio_handle_t *file_io_handle,
     uint64_t data_identifier,
     uint8_t recovered,
     int recovered_value_index,
     libpff_index_value_t **index_value,
     libcerror_error_t **error )
{
	static char *function      = "libpff_offsets_index_get_index_value_by_identifier";
	int number_of_index_values = 0;
	int result                 = 0;

	if( offsets_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offsets index.",
		 function );

		return( -1 );
	}
	if( recovered == 0 )
	{
		result = libpff_index_tree_get_value_by_identifier(
			  offsets_index->index_tree,
			  file_io_handle,
			  offsets_index->index_cache,
			  data_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_MASK,
			  0,
			  index_value,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to find offsets index value identifier: %" PRIu64 ".",
			 function,
			 data_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_MASK );

			return( -1 );
		}
	}
	else
	{
		if( libpff_index_tree_get_number_of_leaf_nodes_by_identifier(
		     offsets_index->recovered_index_tree,
		     file_io_handle,
		     offsets_index->index_cache,
		     data_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_MASK,
		     &number_of_index_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of recovered offsets index values for identifier: %" PRIu64 ".",
			 function,
			 data_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_MASK );

			return( -1 );
		}
		if( number_of_index_values <= 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: no recovered offsets index values for identifier: %" PRIu64 ".",
			 function,
			 data_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_MASK );

			return( -1 );
		}
		if( ( recovered_value_index < 0 )
		 || ( recovered_value_index >= number_of_index_values ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid recovered value index value out of bounds.",
			 function );

			return( -1 );
		}
		result = libpff_index_tree_get_value_by_identifier(
			  offsets_index->recovered_index_tree,
			  file_io_handle,
			  offsets_index->index_cache,
			  data_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_MASK,
			  recovered_value_index,
			  index_value,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to find recovered offsets index value identifier: %" PRIu64 ".",
			 function,
			 data_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_MASK );

			return( -1 );
		}
	}
	return( result );
}

