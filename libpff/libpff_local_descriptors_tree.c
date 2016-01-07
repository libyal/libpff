/*
 * Local descriptors tree functions
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
#include <types.h>

#include "libpff_definitions.h"
#include "libpff_index_value.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_local_descriptor_value.h"
#include "libpff_local_descriptors.h"
#include "libpff_local_descriptors_tree.h"
#include "libpff_offsets_index.h"

/* Retrieves the leaf node for the specific identifier
 * Returns 1 if successful, 0 if no value was found or -1 on error
 */
int libpff_local_descriptors_tree_get_leaf_node_by_identifier(
     libfdata_tree_t *local_descriptors_tree,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *cache,
     uint64_t identifier,
     libfdata_tree_node_t **leaf_local_descriptors_tree_node,
     libcerror_error_t **error )
{
	libfdata_tree_node_t *local_descriptors_tree_root_node = NULL;
	static char *function                                  = "libpff_local_descriptors_tree_get_leaf_node_by_identifier";
	int result                                             = 0;

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
	if( leaf_local_descriptors_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid leaf local descriptors tree node.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: requested identifier\t: 0x%08" PRIx64 " (%" PRIu64 ").\n",
		 function,
		 identifier,
		 identifier );
	}
#endif
	if( libfdata_tree_get_root_node(
	     local_descriptors_tree,
	     &local_descriptors_tree_root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve root node from local descriptors tree.",
		 function );

		return( -1 );
	}
	result = libpff_local_descriptors_tree_node_get_leaf_node_by_identifier(
		  local_descriptors_tree_root_node,
		  file_io_handle,
		  cache,
		  identifier,
		  leaf_local_descriptors_tree_node,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve leaf node by identifier in root node.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the leaf node for the specific identifier
 * Returns 1 if successful, 0 if no leaf node was found or -1 on error
 */
int libpff_local_descriptors_tree_node_get_leaf_node_by_identifier(
     libfdata_tree_node_t *local_descriptors_tree_node,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *cache,
     uint64_t identifier,
     libfdata_tree_node_t **leaf_local_descriptors_tree_node,
     libcerror_error_t **error )
{
	libfdata_tree_node_t *local_descriptors_tree_sub_node                  = NULL;
	libpff_local_descriptor_value_t *local_descriptors_tree_sub_node_value = NULL;
	static char *function                                                  = "libpff_local_descriptors_tree_node_get_leaf_node_by_identifier";
	int16_t compare                                                        = 0;
	int number_of_sub_nodes                                                = 0;
	int result                                                             = 0;
	int sub_node_index                                                     = 0;

	if( local_descriptors_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local descriptors tree node.",
		 function );

		return( -1 );
	}
	if( libfdata_tree_node_get_number_of_sub_nodes(
	     local_descriptors_tree_node,
	     (intptr_t *) file_io_handle,
	     cache,
	     &number_of_sub_nodes,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub nodes from local descriptors tree node.",
		 function );

		return( -1 );
	}
	for( sub_node_index = 0;
	     sub_node_index < number_of_sub_nodes;
	     sub_node_index++ )
	{
		if( libfdata_tree_node_get_sub_node_by_index(
		     local_descriptors_tree_node,
		     (intptr_t *) file_io_handle,
		     cache,
		     sub_node_index,
		     &local_descriptors_tree_sub_node,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub node: %d from local descriptors tree node.",
			 function,
			 sub_node_index );

			return( -1 );
		}
		if( libfdata_tree_node_get_node_value(
		     local_descriptors_tree_sub_node,
		     (intptr_t *) file_io_handle,
		     cache,
		     (intptr_t **) &local_descriptors_tree_sub_node_value,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve local descriptors tree sub node value: %d.",
			 function,
			 sub_node_index );

			return( -1 );
		}
		if( local_descriptors_tree_sub_node_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing local descriptors tree sub node value: %d.",
			 function,
			 sub_node_index );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: local descriptors tree sub node value: %d identifier\t: 0x%08" PRIx64 " (%" PRIu64 ").\n",
			 function,
			 sub_node_index,
			 local_descriptors_tree_sub_node_value->identifier,
			 local_descriptors_tree_sub_node_value->identifier );
		}
#endif
		if( identifier > local_descriptors_tree_sub_node_value->identifier )
		{
			compare = 1;
		}
		else if( identifier < local_descriptors_tree_sub_node_value->identifier )
		{
			compare = -1;
		}
		else
		{
			compare = 0;
		}
		result = libfdata_tree_node_is_leaf(
		          local_descriptors_tree_sub_node,
		          (intptr_t *) file_io_handle,
		          cache,
		          0,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if local descriptors tree sub node: %d is a leaf node.",
			 function,
			 sub_node_index );

			return( -1 );
		}
		else if( result != 0 )
		{
			result = libfdata_tree_node_is_deleted(
				  local_descriptors_tree_sub_node,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine if local descriptors tree sub node: %d is a deleted node.",
				 function,
				 sub_node_index );

				return( -1 );
			}
			else if( result == 0 )
			{
				if( compare == 0 )
				{
					if( leaf_local_descriptors_tree_node == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
						 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
						 "%s: invalid leaf local descriptors tree node.",
						 function );

						return( -1 );
					}
					if( *leaf_local_descriptors_tree_node != NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
						 "%s: leaf local descriptors tree node value already set.",
						 function );

						return( -1 );
					}
					*leaf_local_descriptors_tree_node = local_descriptors_tree_sub_node;

					result = 1;

					break;
				}
			}
			result = 0;
		}
		else
		{
			/* A branch node contains the identifier of its first sub node
			 */
			if( ( compare == 0 )
			 || ( ( compare > 0 )
			   && ( sub_node_index == ( number_of_sub_nodes - 1 ) ) ) )
			{
				result = libpff_local_descriptors_tree_node_get_leaf_node_by_identifier(
					  local_descriptors_tree_sub_node,
					  file_io_handle,
					  cache,
					  identifier,
					  leaf_local_descriptors_tree_node,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve leaf local descriptors tree node by identifier in sub node: %d.",
					 function,
					 sub_node_index );

					return( -1 );
				}
				break;
			}
			else if( ( compare < 0 )
			      && ( sub_node_index >= 1 ) )
			{
				if( libfdata_tree_node_get_sub_node_by_index(
				     local_descriptors_tree_node,
				     (intptr_t *) file_io_handle,
				     cache,
				     sub_node_index - 1,
				     &local_descriptors_tree_sub_node,
				     0,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve sub node: %d from local descriptors tree node.",
					 function,
					 sub_node_index - 1 );

					return( -1 );
				}
				result = libpff_local_descriptors_tree_node_get_leaf_node_by_identifier(
					  local_descriptors_tree_sub_node,
					  file_io_handle,
					  cache,
					  identifier,
					  leaf_local_descriptors_tree_node,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve leaf local descriptors tree node by identifier in sub node: %d.",
					 function,
					 sub_node_index - 1 );

					return( -1 );
				}
				break;
			}
		}
	}
	return( result );
}

/* Retrieves the local descriptor value for the specific identifier
 * Returns 1 if successful, 0 if no value was found or -1 on error
 */
int libpff_local_descriptors_tree_get_value_by_identifier(
     libfdata_tree_t *local_descriptors_tree,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *cache,
     uint64_t identifier,
     libpff_local_descriptor_value_t **local_descriptor_value,
     libcerror_error_t **error )
{
	libfdata_tree_node_t *leaf_local_descriptors_tree_node = NULL;
	static char *function                                  = "libpff_local_descriptors_tree_get_value_by_identifier";
	int result                                             = 0;

	result = libpff_local_descriptors_tree_get_leaf_node_by_identifier(
		  local_descriptors_tree,
		  file_io_handle,
		  cache,
		  identifier,
		  &leaf_local_descriptors_tree_node,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve leaf node by identifier in root node.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( leaf_local_descriptors_tree_node == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing leaf local descriptors tree node.",
			 function );

			return( -1 );
		}
		if( libfdata_tree_node_get_node_value(
		     leaf_local_descriptors_tree_node,
		     (intptr_t *) file_io_handle,
		     cache,
		     (intptr_t **) local_descriptor_value,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve leaf local descriptors tree node value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Reads a specifi local descriptors tree
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptors_tree_read(
     libfdata_tree_t **local_descriptors_tree,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     uint32_t descriptor_identifier,
     uint64_t data_identifier,
     uint8_t recovered,
     int recovered_value_index,
     libcerror_error_t **error )
{
	libpff_index_value_t *offset_index_value      = NULL;
	libpff_local_descriptors_t *local_descriptors = NULL;
	static char *function                         = "libpff_local_descriptors_tree_read";

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
	if( *local_descriptors_tree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: local descriptors tree already set.",
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
	if( libpff_offsets_index_get_index_value_by_identifier(
	     offsets_index,
	     file_io_handle,
	     data_identifier,
	     recovered,
	     recovered_value_index,
	     &offset_index_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve offset index value identifier: %" PRIu64 ".",
		 function,
		 data_identifier );

		goto on_error;
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

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: identifier: %" PRIu64 " (%s) at offset: %" PRIi64 " of size: %" PRIu32 "\n",
		 function,
		 offset_index_value->identifier,
		 ( ( offset_index_value->identifier & LIBPFF_OFFSET_INDEX_IDENTIFIER_FLAG_INTERNAL ) ? "internal" : "external" ),
		 offset_index_value->file_offset,
		 offset_index_value->data_size );
	}
#endif
	/* Create the local descriptors tree
	 */
	if( libpff_local_descriptors_initialize(
	     &local_descriptors,
	     io_handle,
	     offsets_index,
	     descriptor_identifier,
	     offset_index_value->file_offset,
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
	if( libfdata_tree_initialize(
	     local_descriptors_tree,
	     (intptr_t *) local_descriptors,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_local_descriptors_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libpff_local_descriptors_clone,
	     (int (*)(intptr_t *, intptr_t *, libfdata_tree_node_t *, libfcache_cache_t *, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libpff_local_descriptors_read_node,
	     (int (*)(intptr_t *, intptr_t *, libfdata_tree_node_t *, libfcache_cache_t *, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libpff_local_descriptors_read_sub_nodes,
	     LIBFDATA_DATA_HANDLE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create local descriptors tree",
		 function );

		goto on_error;
	}
	if( libfdata_tree_set_root_node(
	     *local_descriptors_tree,
	     0,
	     offset_index_value->file_offset,
	     (size64_t) data_identifier,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set local descriptors tree root node.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( local_descriptors != NULL )
	{
		libpff_local_descriptors_free(
		 &local_descriptors,
		 NULL );
	}
	return( -1 );
}

