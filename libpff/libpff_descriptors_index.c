/*
 * Descriptors index functions
 *
 * Copyright (C) 2008-2016, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libpff_definitions.h"
#include "libpff_descriptors_index.h"
#include "libpff_index.h"
#include "libpff_index_tree.h"
#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"

/* Creates a descriptors index
 * Make sure the value descriptors_index is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_descriptors_index_initialize(
     libpff_descriptors_index_t **descriptors_index,
     libpff_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function = "libpff_descriptors_index_initialize";

	if( descriptors_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptors index.",
		 function );

		return( -1 );
	}
	if( *descriptors_index != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid descriptors index value already set.",
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
	*descriptors_index = memory_allocate_structure(
	                      libpff_descriptors_index_t );

	if( *descriptors_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create descriptors index.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *descriptors_index,
	     0,
	     sizeof( libpff_descriptors_index_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear descriptors index.",
		 function );

		goto on_error;
	}
	( *descriptors_index )->io_handle = io_handle;

	return( 1 );

on_error:
	if( *descriptors_index != NULL )
	{
		memory_free(
		 *descriptors_index );

		*descriptors_index = NULL;
	}
	return( -1 );
}

/* Frees a descriptors index
 * Returns 1 if successful or -1 on error
 */
int libpff_descriptors_index_free(
     libpff_descriptors_index_t **descriptors_index,
     libcerror_error_t **error )
{
	static char *function = "libpff_descriptors_index_free";
	int result            = 1;

	if( descriptors_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptors index.",
		 function );

		return( -1 );
	}
	if( *descriptors_index != NULL )
	{
		if( ( *descriptors_index )->index_tree != NULL )
		{
			if( libfdata_tree_free(
			     &( ( *descriptors_index )->index_tree ),
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
		if( ( *descriptors_index )->recovered_index_tree != NULL )
		{
			if( libfdata_tree_free(
			     &( ( *descriptors_index )->recovered_index_tree ),
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
		if( ( *descriptors_index )->index_cache != NULL )
		{
			if( libfcache_cache_free(
			     &( ( *descriptors_index )->index_cache ),
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
		 *descriptors_index );

		*descriptors_index = NULL;
	}
	return( result );
}

/* Sets the root node
 * Returns 1 if successful or -1 on error
 */
int libpff_descriptors_index_set_root_node(
     libpff_descriptors_index_t *descriptors_index,
     off64_t root_node_offset,
     uint64_t root_node_back_pointer,
     uint8_t recovered,
     libcerror_error_t **error )
{
	libpff_index_t *index = NULL;
	static char *function = "libpff_descriptors_index_set_root_node";

	if( descriptors_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptors index.",
		 function );

		return( -1 );
	}
	if( recovered == 0 )
	{
		if( descriptors_index->index_tree != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: invalid descriptors index - index tree value already set.",
			 function );

			return( -1 );
		}
		if( libpff_index_initialize(
		     &index,
		     descriptors_index->io_handle,
		     LIBPFF_INDEX_TYPE_DESCRIPTOR,
		     root_node_offset,
		     root_node_back_pointer,
		     recovered,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create index.",
			 function );

			goto on_error;
		}
		if( libfdata_tree_initialize(
		     &( descriptors_index->index_tree ),
		     (intptr_t *) index,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_index_free,
		     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libpff_index_clone,
		     (int (*)(intptr_t *, intptr_t *, libfdata_tree_node_t *, libfcache_cache_t *, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libpff_index_read_node_data,
		     (int (*)(intptr_t *, intptr_t *, libfdata_tree_node_t *, libfcache_cache_t *, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libpff_index_read_sub_nodes,
		     LIBFDATA_DATA_HANDLE_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create index tree",
			 function );

			goto on_error;
		}
		/* The index is now managed by the index tree
		 */
		index = NULL;

		/* Point the root node data offset to LIBPFF_DESCRIPTOR_INDEX_TREE_ROOT_OFFSET
		 * otherwise it will mess up the caching of the first index value
		 */
		if( libfdata_tree_set_root_node(
		     descriptors_index->index_tree,
		     0,
		     LIBPFF_DESCRIPTOR_INDEX_TREE_ROOT_OFFSET,
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
		if( descriptors_index->recovered_index_tree != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: invalid descriptors index - recovered index tree value already set.",
			 function );

			return( -1 );
		}
		if( libpff_index_initialize(
		     &index,
		     descriptors_index->io_handle,
		     LIBPFF_INDEX_TYPE_DESCRIPTOR,
		     0,
		     0,
		     recovered,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create recovered index.",
			 function );

			goto on_error;
		}
		if( libfdata_tree_initialize(
		     &( descriptors_index->recovered_index_tree ),
		     (intptr_t *) index,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_index_free,
		     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libpff_index_clone,
		     (int (*)(intptr_t *, intptr_t *, libfdata_tree_node_t *, libfcache_cache_t *, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libpff_index_read_node_data,
		     (int (*)(intptr_t *, intptr_t *, libfdata_tree_node_t *, libfcache_cache_t *, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libpff_index_read_sub_nodes,
		     LIBFDATA_DATA_HANDLE_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create recovered index tree",
			 function );

			goto on_error;
		}
		/* The index is now managed by the index tree
		 */
		index = NULL;

		/* Point the root node data offset to LIBPFF_RECOVERED_DESCRIPTOR_INDEX_TREE_ROOT_OFFSET
		 * otherwise it will mess up the caching of the first index value
		 */
		if( libfdata_tree_set_root_node(
		     descriptors_index->recovered_index_tree,
		     0,
		     LIBPFF_RECOVERED_DESCRIPTOR_INDEX_TREE_ROOT_OFFSET,
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
	if( descriptors_index->index_cache == NULL )
	{
		if( libfcache_cache_initialize(
		     &( descriptors_index->index_cache ),
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
		if( descriptors_index->index_tree != NULL )
		{
			libfdata_tree_free(
			 &( descriptors_index->index_tree ),
			 NULL );
		}
	}
	else
	{
		if( descriptors_index->recovered_index_tree != NULL )
		{
			libfdata_tree_free(
			 &( descriptors_index->recovered_index_tree ),
			 NULL );
		}
	}
	if( index != NULL )
	{
		libpff_index_free(
		 &index,
		 NULL );
	}
	return( -1 );
}

/* Retrieves an index value for a specific identifier
 * Returns 1 if successful, 0 if no index value was found or -1 on error
 */
int libpff_descriptors_index_get_index_value_by_identifier(
     libpff_descriptors_index_t *descriptors_index,
     libbfio_handle_t *file_io_handle,
     uint32_t descriptor_identifier,
     uint8_t recovered,
     libpff_index_value_t **index_value,
     libcerror_error_t **error )
{
	static char *function      = "libpff_descriptors_index_get_index_value_by_identifier";
	int number_of_index_values = 0;
	int result                 = 0;

	if( descriptors_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptors index.",
		 function );

		return( -1 );
	}
	if( recovered == 0 )
	{
		result = libpff_index_tree_get_value_by_identifier(
			  descriptors_index->index_tree,
			  file_io_handle,
			  descriptors_index->index_cache,
			  descriptor_identifier,
			  0,
			  index_value,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve index value: %" PRIu32 ".",
			 function,
			 descriptor_identifier );

			return( -1 );
		}
	}
	else
	{
		if( libpff_index_tree_get_number_of_leaf_nodes_by_identifier(
		     descriptors_index->recovered_index_tree,
		     file_io_handle,
		     descriptors_index->index_cache,
		     descriptor_identifier,
		     &number_of_index_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of recovered index values for identifier: %" PRIu32 ".",
			 function,
			 descriptor_identifier );

			return( -1 );
		}
/* TODO is it necessary to lookup removed descriptors with a value index > 0 ? */
		result = libpff_index_tree_get_value_by_identifier(
			  descriptors_index->recovered_index_tree,
			  file_io_handle,
			  descriptors_index->index_cache,
			  descriptor_identifier,
			  0,
			  index_value,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve recovered index value: %" PRIu32 ".",
			 function,
			 descriptor_identifier );

			return( -1 );
		}
	}
	return( result );
}

