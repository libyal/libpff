/*
 * Local descriptors functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libpff_definitions.h"
#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_local_descriptor_node.h"
#include "libpff_local_descriptor_value.h"
#include "libpff_local_descriptors.h"
#include "libpff_offsets_index.h"
#include "libpff_unused.h"

/* Creates local descriptors
 * Make sure the value local_descriptors is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptors_initialize(
     libpff_local_descriptors_t **local_descriptors,
     libpff_io_handle_t *io_handle,
     libpff_offsets_index_t *offsets_index,
     uint32_t descriptor_identifier,
     off64_t root_node_offset,
     uint8_t recovered,
     libcerror_error_t **error )
{
	static char *function = "libpff_local_descriptors_initialize";

	if( local_descriptors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local descriptors.",
		 function );

		return( -1 );
	}
	if( *local_descriptors != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid local descriptors value already set.",
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
	*local_descriptors = memory_allocate_structure(
	                      libpff_local_descriptors_t );

	if( *local_descriptors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create local descriptors.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *local_descriptors,
	     0,
	     sizeof( libpff_local_descriptors_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear local descriptors.",
		 function );

		memory_free(
		 *local_descriptors );

		*local_descriptors = NULL;

		return( -1 );
	}
	if( libfcache_cache_initialize(
	     &( ( *local_descriptors )->local_descriptor_nodes_cache ),
	     LIBPFF_MAXIMUM_CACHE_ENTRIES_LOCAL_DESCRIPTORS_NODES,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create local descriptor nodes cache.",
		 function );

		goto on_error;
	}
	( *local_descriptors )->io_handle             = io_handle;
	( *local_descriptors )->offsets_index         = offsets_index;
	( *local_descriptors )->descriptor_identifier = descriptor_identifier;
	( *local_descriptors )->root_node_offset      = root_node_offset;
	( *local_descriptors )->recovered             = recovered;

	return( 1 );

on_error:
	if( *local_descriptors != NULL )
	{
		memory_free(
		 *local_descriptors );

		*local_descriptors = NULL;
	}
	return( -1 );
}

/* Frees local descriptors
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptors_free(
     libpff_local_descriptors_t **local_descriptors,
     libcerror_error_t **error )
{
	static char *function = "libpff_local_descriptors_free";
	int result            = 1;

	if( local_descriptors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local descriptors.",
		 function );

		return( -1 );
	}
	if( *local_descriptors != NULL )
	{
		if( libfcache_cache_free(
		     &( ( *local_descriptors )->local_descriptor_nodes_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free local descriptor nodes cache.",
			 function );

			result = -1;
		}
		memory_free(
		 *local_descriptors );

		*local_descriptors = NULL;
	}
	return( result );
}

/* Clones the local descriptors
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptors_clone(
     libpff_local_descriptors_t **destination_local_descriptors,
     libpff_local_descriptors_t *source_local_descriptors,
     libcerror_error_t **error )
{
	static char *function = "libpff_local_descriptors_clone";

	if( destination_local_descriptors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination local descriptors.",
		 function );

		return( -1 );
	}
	if( *destination_local_descriptors != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination local descriptors already set.",
		 function );

		return( -1 );
	}
	if( source_local_descriptors == NULL )
	{
		*destination_local_descriptors = NULL;

		return( 1 );
	}
	if( libpff_local_descriptors_initialize(
	     destination_local_descriptors,
	     source_local_descriptors->io_handle,
	     source_local_descriptors->offsets_index,
	     source_local_descriptors->descriptor_identifier,
	     source_local_descriptors->root_node_offset,
	     source_local_descriptors->recovered,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination local descriptors.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the local descriptor node
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptors_read_local_descriptor_node(
     libpff_local_descriptors_t *local_descriptors,
     libbfio_handle_t *file_io_handle,
     uint64_t data_identifier,
     libpff_local_descriptor_node_t **local_descriptor_node,
     libcerror_error_t **error )
{
	libfcache_cache_value_t *cache_value     = NULL;
	libpff_index_value_t *offset_index_value = NULL;
	static char *function                    = "libpff_local_descriptors_read_local_descriptor_node";
	off64_t cache_value_offset               = 0;
	time_t cache_value_timestamp             = 0;
	int cache_value_index                    = 0;
	int cache_value_file_index               = 0;
	int is_cached                            = 0;

	if( local_descriptors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local descriptors.",
		 function );

		return( -1 );
	}
	if( local_descriptors->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid local descriptors - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( local_descriptors->io_handle->file_type != LIBPFF_FILE_TYPE_32BIT )
	 && ( local_descriptors->io_handle->file_type != LIBPFF_FILE_TYPE_64BIT )
	 && ( local_descriptors->io_handle->file_type != LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file type.",
		 function );

		return( -1 );
	}
	if( local_descriptor_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local descriptor node.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: requested data identifier: %" PRIu64 "\n",
		 function,
		 data_identifier );
	}
#endif
	cache_value_index = data_identifier % LIBPFF_MAXIMUM_CACHE_ENTRIES_LOCAL_DESCRIPTORS_NODES;

	if( libfcache_cache_get_value_by_index(
	     local_descriptors->local_descriptor_nodes_cache,
	     cache_value_index,
	     &cache_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to find data identifier: %" PRIu64 ".",
		 function,
		 data_identifier );

		return( -1 );
	}
	if( cache_value != NULL )
	{
		if( libfcache_cache_value_get_identifier(
		     cache_value,
		     &cache_value_file_index,
		     &cache_value_offset,
		     &cache_value_timestamp,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve cache value identifier.",
			 function );

			return( -1 );
		}
		if( (uint64_t) cache_value_offset == data_identifier )
		{
			is_cached = 1;
		}
	}
#if !defined( HAVE_DEBUG_OUTPUT )
	if( is_cached == 0 )
#endif
	{
/* TODO handle multiple recovered offset index values */
		if( libpff_offsets_index_get_index_value_by_identifier(
		     local_descriptors->offsets_index,
		     file_io_handle,
		     data_identifier,
		     local_descriptors->recovered,
		     0,
		     &offset_index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to find data identifier: %" PRIu64 ".",
			 function,
			 data_identifier );

			return( -1 );
		}
		if( offset_index_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing offset index value: %" PRIu64 ".",
			 function,
			 data_identifier );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: local descriptor node: identifier: %" PRIu64 " (%s) at offset: 0x%08" PRIx64 " of size: %" PRIu32 "\n",
			 function,
			 offset_index_value->identifier,
			 ( ( offset_index_value->identifier & LIBPFF_OFFSET_INDEX_IDENTIFIER_FLAG_INTERNAL ) ? "internal" : "external" ),
			 offset_index_value->file_offset,
			 offset_index_value->data_size );
		}
#endif
	}
	if( is_cached != 0 )
	{
		if( libfcache_cache_value_get_value(
		     cache_value,
		     (intptr_t **) local_descriptor_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve local descriptor node from cache value.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( libpff_local_descriptor_node_initialize(
		     local_descriptor_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create local descriptor node.",
			 function );

			return( -1 );
		}
		if( local_descriptor_node == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing local descriptor node.",
			 function );

			return( -1 );
		}
		if( libpff_local_descriptor_node_read(
		     *local_descriptor_node,
		     local_descriptors->io_handle,
		     file_io_handle,
		     local_descriptors->descriptor_identifier,
		     data_identifier,
		     offset_index_value->file_offset,
		     offset_index_value->data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read local descriptor node at offset: 0x%08" PRIx64 ".",
			 function,
			 offset_index_value->file_offset );

			libpff_local_descriptor_node_free(
			 local_descriptor_node,
			 NULL );

			return( -1 );
		}
		if( libfcache_cache_set_value_by_index(
		     local_descriptors->local_descriptor_nodes_cache,
		     cache_value_index,
		     0,
		     (off64_t) data_identifier,
		     0,
		     (intptr_t *) *local_descriptor_node,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_local_descriptor_node_free,
		     LIBFCACHE_CACHE_VALUE_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set local descriptor node in cache.",
			 function );

			libpff_local_descriptor_node_free(
			 local_descriptor_node,
			 NULL );

			return( -1 );
		}
	}
	return( 1 );
}

/* Reads the local descriptor node tree node
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptors_read_tree_node(
     libpff_local_descriptors_t *local_descriptors,
     libbfio_handle_t *file_io_handle,
     off64_t node_offset,
     uint64_t data_identifier,
     libfdata_tree_node_t *local_descriptors_tree_node,
     libcerror_error_t **error )
{
	libpff_index_value_t *offset_index_value              = NULL;
	libpff_local_descriptor_node_t *local_descriptor_node = NULL;
	static char *function                                 = "libpff_local_descriptors_read_tree_node";
	uint64_t local_descriptor_sub_node_identifier         = 0;
	uint16_t entry_index                                  = 0;

	if( local_descriptors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local descriptors.",
		 function );

		return( -1 );
	}
	if( local_descriptors->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid local descriptors - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( local_descriptors->io_handle->file_type != LIBPFF_FILE_TYPE_32BIT )
	 && ( local_descriptors->io_handle->file_type != LIBPFF_FILE_TYPE_64BIT )
	 && ( local_descriptors->io_handle->file_type != LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file type.",
		 function );

		return( -1 );
	}
	if( libpff_local_descriptors_read_local_descriptor_node(
	     local_descriptors,
	     file_io_handle,
	     data_identifier,
	     &local_descriptor_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve local descriptor node.",
		 function );

		return( -1 );
	}
	if( local_descriptor_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing local descriptor node.",
		 function );

		return( -1 );
	}
	if( local_descriptors->io_handle->file_type != LIBPFF_FILE_TYPE_32BIT )
	{
		node_offset += 4;
	}
	else if( ( local_descriptors->io_handle->file_type != LIBPFF_FILE_TYPE_64BIT )
	      && ( local_descriptors->io_handle->file_type != LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
	{
		node_offset += 8;
	}
	if( local_descriptor_node->number_of_entries > 0 )
	{
		if( libfdata_tree_node_resize_sub_nodes(
		     local_descriptors_tree_node,
		     (int) local_descriptor_node->number_of_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize number of sub nodes.",
			 function );

			return( -1 );
		}
		for( entry_index = 0;
		     entry_index < local_descriptor_node->number_of_entries;
		     entry_index++ )
		{
			if( local_descriptor_node->level != LIBPFF_LOCAL_DESCRIPTOR_NODE_LEVEL_LEAF )
			{
				if( libpff_local_descriptor_node_get_entry_sub_node_identifier(
				     local_descriptor_node,
				     local_descriptors->io_handle,
				     entry_index,
				     &local_descriptor_sub_node_identifier,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve node entry: %" PRIu16 " sub node identifier.",
					 function,
					 entry_index );

					return( -1 );
				}
				/* Check if the local descriptor sub node identifier exists
				 */
/* TODO handle multiple recovered offset index values */
				if( libpff_offsets_index_get_index_value_by_identifier(
				     local_descriptors->offsets_index,
				     file_io_handle,
				     local_descriptor_sub_node_identifier,
				     local_descriptors->recovered,
				     0,
				     &offset_index_value,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to find sub node identifier: %" PRIu64 ".",
					 function,
					 local_descriptor_sub_node_identifier );

					return( -1 );
				}
				if( offset_index_value == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing offset index value.",
					 function );

					return( -1 );
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: local descriptor entry: %03" PRIu16 " at level: %" PRIu8 " identifier: %" PRIu64 " (%s) at offset: 0x%08" PRIx64 " of size: %" PRIu32 "\n",
					 function,
					 entry_index,
					 local_descriptor_node->level,
					 offset_index_value->identifier,
					 ( ( offset_index_value->identifier & LIBPFF_OFFSET_INDEX_IDENTIFIER_FLAG_INTERNAL ) ? "internal" : "external" ),
					 offset_index_value->file_offset,
					 offset_index_value->data_size );
				}
#endif
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: local descriptor entry: %03" PRIu16 " at level: %" PRIu8 " data identifier: %" PRIu64 " node offset: 0x%08" PRIx64 "\n",
				 function,
				 entry_index,
				 local_descriptor_node->level,
				 data_identifier,
				 node_offset );
			}
#endif
			/* The sub node point to the specific entry in the sub nodes data of the current node.
			 * The node offset is used as a unique identifier for the individual nodes.
			 * The actual data is found using the data identifier and the entry index.
			 */
			if( libfdata_tree_node_set_sub_node_by_index(
			     local_descriptors_tree_node,
			     (int) entry_index,
			     (int) entry_index,
			     node_offset,
			     (size64_t) data_identifier,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set local descriptor node entry: %" PRIu16 " as sub node.",
				 function,
				 entry_index );

				return( -1 );
			}
			if( local_descriptor_node->level == LIBPFF_LOCAL_DESCRIPTOR_NODE_LEVEL_LEAF )
			{
				if( libfdata_tree_node_set_leaf_sub_node(
				     local_descriptors_tree_node,
				     (int) entry_index,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set local descriptor node entry: %" PRIu16 " as leaf node.",
					 function,
					 entry_index );

					return( -1 );
				}
			}
			node_offset += local_descriptor_node->entry_size;
		}
	}
	return( 1 );
}

/* Reads the local descriptor value
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptors_read_local_descriptor_value(
     libpff_local_descriptors_t *local_descriptors,
     libbfio_handle_t *file_io_handle,
     uint64_t data_identifier,
     uint16_t entry_index,
     libfdata_tree_node_t *local_descriptors_tree_node,
     libpff_local_descriptor_value_t *local_descriptor_value,
     libcerror_error_t **error )
{
	libpff_index_value_t *offset_index_value              = NULL;
	libpff_local_descriptor_node_t *local_descriptor_node = NULL;
	uint8_t *node_entry_data                              = NULL;
	static char *function                                 = "libpff_local_descriptors_read_local_descriptor_value";
	int result                                            = 0;

	if( local_descriptors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local descriptors.",
		 function );

		return( -1 );
	}
	if( local_descriptors->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid local descriptors - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( local_descriptors->io_handle->file_type != LIBPFF_FILE_TYPE_32BIT )
	 && ( local_descriptors->io_handle->file_type != LIBPFF_FILE_TYPE_64BIT )
	 && ( local_descriptors->io_handle->file_type != LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file type.",
		 function );

		return( -1 );
	}
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
	if( libpff_local_descriptors_read_local_descriptor_node(
	     local_descriptors,
	     file_io_handle,
	     data_identifier,
	     &local_descriptor_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve local descriptor node.",
		 function );

		return( -1 );
	}
	if( local_descriptor_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing local descriptor node.",
		 function );

		return( -1 );
	}
	if( libpff_local_descriptor_node_get_entry_identifier(
	     local_descriptor_node,
	     local_descriptors->io_handle,
	     entry_index,
	     &( local_descriptor_value->identifier ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve node entry: %" PRIu16 " identifier.",
		 function,
		 entry_index );

		return( -1 );
	}
	/* Ignore the upper 32-bit of local descriptor identifiers
	 */
	local_descriptor_value->identifier &= 0xffffffffUL;

	if( local_descriptor_node->level == LIBPFF_LOCAL_DESCRIPTOR_NODE_LEVEL_LEAF )
	{
/* TODO get_entry_identifiers function ? */
		if( libpff_local_descriptor_node_get_entry_data(
		     local_descriptor_node,
		     entry_index,
		     &node_entry_data,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve node entry: %" PRIu16 " data.",
			 function,
			 entry_index );

			return( -1 );
		}
		if( node_entry_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing node entry: %" PRIu16 " data.",
			 function,
			 entry_index );

			return( -1 );
		}
		if( local_descriptors->io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			node_entry_data += 4;

			byte_stream_copy_to_uint32_little_endian(
			 node_entry_data,
			 local_descriptor_value->data_identifier );

			node_entry_data += 4;

			byte_stream_copy_to_uint32_little_endian(
			 node_entry_data,
			 local_descriptor_value->local_descriptors_identifier );

			node_entry_data += 4;
		}
		else if( ( local_descriptors->io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
		      || ( local_descriptors->io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
		{
			node_entry_data += 8;

			byte_stream_copy_to_uint64_little_endian(
			 node_entry_data,
			 local_descriptor_value->data_identifier );

			node_entry_data += 8;

			byte_stream_copy_to_uint64_little_endian(
			 node_entry_data,
			 local_descriptor_value->local_descriptors_identifier );

			node_entry_data += 8;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: local descriptor entry: %03" PRIu16 " at level: %" PRIu8 " identifier: %" PRIu64 ", data identifier: %" PRIu64 ", local descriptors identifier: %" PRIu64 "\n",
			 function,
			 entry_index,
			 local_descriptor_node->level,
			 local_descriptor_value->identifier,
			 local_descriptor_value->data_identifier,
			 local_descriptor_value->local_descriptors_identifier );
		}
#endif
	}
	else
	{
		if( libpff_local_descriptor_node_get_entry_sub_node_identifier(
		     local_descriptor_node,
		     local_descriptors->io_handle,
		     entry_index,
		     &( local_descriptor_value->sub_node_identifier ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve node entry: %" PRIu16 " sub node identifier.",
			 function,
			 entry_index );

			return( -1 );
		}
/* TODO handle multiple recovered offset index values */
		if( libpff_offsets_index_get_index_value_by_identifier(
		     local_descriptors->offsets_index,
		     file_io_handle,
		     local_descriptor_value->sub_node_identifier,
		     local_descriptors->recovered,
		     0,
		     &offset_index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to find sub node identifier: %" PRIu64 ".",
			 function,
			 local_descriptor_value->sub_node_identifier );

			return( -1 );
		}
		if( offset_index_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing offset index value.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: local descriptor entry: %03" PRIu16 " at level: %" PRIu8 " identifier: %" PRIu64 " (%s) at offset: 0x%08" PRIx64 " of size: %" PRIu32 "\n",
			 function,
			 entry_index,
			 local_descriptor_node->level,
			 offset_index_value->identifier,
			 ( ( offset_index_value->identifier & LIBPFF_OFFSET_INDEX_IDENTIFIER_FLAG_INTERNAL ) ? "internal" : "external" ),
			 offset_index_value->file_offset,
			 offset_index_value->data_size );
		}
#endif
		result = libfdata_tree_node_sub_nodes_data_range_is_set(
		          local_descriptors_tree_node,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if sub nodes data range is set.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			if( libfdata_tree_node_set_sub_nodes_data_range(
			     local_descriptors_tree_node,
			     0,
			     offset_index_value->file_offset,
			     (size64_t) local_descriptor_value->sub_node_identifier,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set sub nodes data range.",
				 function );

				return( -1 );
			}
		}
	}
	return( 1 );
}

/* Reads the local descriptors node
 * Callback for the local descriptors tree
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptors_read_node(
     libpff_local_descriptors_t *local_descriptors,
     libbfio_handle_t *file_io_handle,
     libfdata_tree_node_t *node,
     libfcache_cache_t *cache,
     int node_file_index,
     off64_t node_offset,
     size64_t node_size,
     uint32_t node_flags LIBPFF_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBPFF_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libpff_local_descriptor_value_t *local_descriptor_value = NULL;
	static char *function                                   = "libpff_local_descriptors_read_node";
	int result                                              = 0;

	LIBPFF_UNREFERENCED_PARAMETER( node_flags )
	LIBPFF_UNREFERENCED_PARAMETER( read_flags )

	if( local_descriptors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local descriptors.",
		 function );

		return( -1 );
	}
	if( node_file_index > (int) UINT16_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid node file index value out of bounds.",
		 function );

		return( -1 );
	}
	/* The node size value contains the data identifier
	 * of the corresponding local descriptor node
	 */
	if( libpff_local_descriptor_value_initialize(
	     &local_descriptor_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create local descriptor value.",
		 function );

		goto on_error;
	}
	if( node_offset == local_descriptors->root_node_offset )
	{
		/* The local descriptors tree root node is virtual
		 */
		result = libfdata_tree_node_sub_nodes_data_range_is_set(
		          node,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if sub nodes data range is set.",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
			if( libfdata_tree_node_set_sub_nodes_data_range(
			     node,
			     0,
			     node_offset,
			     (uint64_t) node_size,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set sub nodes data range.",
				 function );

				goto on_error;
			}
		}
	}
	else
	{
		/* The node_size value contains the local descriptor node entry
		 */
		if( libpff_local_descriptors_read_local_descriptor_value(
		     local_descriptors,
		     file_io_handle,
		     (uint64_t) node_size,
		     (uint16_t) node_file_index,
		     node,
		     local_descriptor_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read local descriptor node entry at offset: 0x%08" PRIx64 ".",
			 function,
			 node_offset );

			goto on_error;
		}
	}
	if( libfdata_tree_node_set_node_value(
	     node,
	     cache,
	     (intptr_t *) local_descriptor_value,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_local_descriptor_value_free,
	     LIBFDATA_TREE_NODE_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set local descriptor value as node value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( local_descriptor_value != NULL )
	{
		libpff_local_descriptor_value_free(
		 &local_descriptor_value,
		 NULL );
	}
	return( -1 );
}

/* Reads the local descriptors sub nodes
 * Callback for the local descriptors tree
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptors_read_sub_nodes(
     libpff_local_descriptors_t *local_descriptors,
     libbfio_handle_t *file_io_handle,
     libfdata_tree_node_t *node,
     libfcache_cache_t *cache LIBPFF_ATTRIBUTE_UNUSED,
     int sub_nodes_file_index LIBPFF_ATTRIBUTE_UNUSED,
     off64_t sub_nodes_offset,
     size64_t sub_nodes_size,
     uint32_t sub_nodes_flags LIBPFF_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBPFF_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	static char *function = "libpff_local_descriptors_read_sub_nodes";

	LIBPFF_UNREFERENCED_PARAMETER( cache )
	LIBPFF_UNREFERENCED_PARAMETER( sub_nodes_file_index )
	LIBPFF_UNREFERENCED_PARAMETER( sub_nodes_flags )
	LIBPFF_UNREFERENCED_PARAMETER( read_flags )

	if( local_descriptors == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local descriptors.",
		 function );

		return( -1 );
	}
	/* The sub nodes size value contains the data identifier
	 * of the corresponding local descriptor node
	 */
	if( libpff_local_descriptors_read_tree_node(
	     local_descriptors,
	     file_io_handle,
	     sub_nodes_offset,
	     (uint64_t) sub_nodes_size,
	     node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read local descriptor tree node: %" PRIu64 ".",
		 function,
		 (uint64_t) sub_nodes_size );

		return( -1 );
	}
	return( 1 );
}

