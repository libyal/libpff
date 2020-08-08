/*
 * Index tree functions
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
#include <types.h>

#include "libpff_definitions.h"
#include "libpff_index.h"
#include "libpff_index_tree.h"
#include "libpff_index_value.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"

/* Creates an index tree
 * Make sure the value index_tree is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_index_tree_initialize(
     libfdata_tree_t **index_tree,
     libpff_io_handle_t *io_handle,
     libfdata_vector_t *index_nodes_vector,
     libfcache_cache_t *index_nodes_cache,
     uint8_t index_type,
     off64_t root_node_offset,
     uint64_t root_node_back_pointer,
     uint8_t recovered,
     libcerror_error_t **error )
{
	libpff_index_t *index = NULL;
	static char *function = "libpff_index_tree_initialize";

	if( index_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index tree.",
		 function );

		return( -1 );
	}
	if( *index_tree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid index tree value already set.",
		 function );

		return( -1 );
	}
	if( libpff_index_initialize(
	     &index,
	     io_handle,
	     index_nodes_vector,
	     index_nodes_cache,
	     index_type,
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
	     index_tree,
	     (intptr_t *) index,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_index_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libpff_index_clone,
	     (int (*)(intptr_t *, intptr_t *, libfdata_tree_node_t *, libfdata_cache_t *, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libpff_index_read_node_data,
	     (int (*)(intptr_t *, intptr_t *, libfdata_tree_node_t *, libfdata_cache_t *, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libpff_index_read_sub_nodes,
	     LIBFDATA_DATA_HANDLE_FLAG_MANAGED,
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
	/* The index is now managed by the index tree
	 */
	index = NULL;

	return( 1 );

on_error:
	if( index != NULL )
	{
		libpff_index_free(
		 &index,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the number of leaf nodes for the specific identifier
 * Returns 1 if successful or -1 on error
 */
int libpff_index_tree_get_number_of_leaf_nodes_by_identifier(
     libfdata_tree_t *index_tree,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *cache,
     uint64_t identifier,
     int *number_of_leaf_nodes,
     libcerror_error_t **error )
{
	libfdata_tree_node_t *index_tree_root_node = NULL;
	static char *function                      = "libpff_index_tree_get_number_of_leaf_nodes_by_identifier";

	if( index_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index tree.",
		 function );

		return( -1 );
	}
	if( number_of_leaf_nodes == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of leaf nodes.",
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
	     index_tree,
	     &index_tree_root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve root node from index tree.",
		 function );

		return( -1 );
	}
	*number_of_leaf_nodes = 0;

	if( libpff_index_tree_node_get_number_of_leaf_nodes_by_identifier(
	     index_tree_root_node,
	     file_io_handle,
	     cache,
	     identifier,
	     number_of_leaf_nodes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of leaf nodes by identifier in root node.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of leaf nodes for the specific identifier
 * Returns 1 if successful or -1 on error
 */
int libpff_index_tree_node_get_number_of_leaf_nodes_by_identifier(
     libfdata_tree_node_t *index_tree_node,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *cache,
     uint64_t identifier,
     int *number_of_leaf_nodes,
     libcerror_error_t **error )
{
	libfdata_tree_node_t *index_tree_sub_node       = NULL;
	libpff_index_value_t *index_tree_sub_node_value = NULL;
	static char *function                           = "libpff_index_tree_node_get_number_of_leaf_nodes_by_identifier";
	int16_t compare                                 = 0;
	int number_of_sub_nodes                         = 0;
	int result                                      = 0;
	int sub_node_index                              = 0;

	if( index_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index tree node.",
		 function );

		return( -1 );
	}
	if( libfdata_tree_node_get_number_of_sub_nodes(
	     index_tree_node,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) cache,
	     &number_of_sub_nodes,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub nodes from index tree node.",
		 function );

		return( -1 );
	}
	for( sub_node_index = 0;
	     sub_node_index < number_of_sub_nodes;
	     sub_node_index++ )
	{
		if( libfdata_tree_node_get_sub_node_by_index(
		     index_tree_node,
		     (intptr_t *) file_io_handle,
		     (libfdata_cache_t *) cache,
		     sub_node_index,
		     &index_tree_sub_node,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub node: %d from index tree node.",
			 function,
			 sub_node_index );

			return( -1 );
		}
		if( libfdata_tree_node_get_node_value(
		     index_tree_sub_node,
		     (intptr_t *) file_io_handle,
		     (libfdata_cache_t *) cache,
		     (intptr_t **) &index_tree_sub_node_value,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve index tree sub node value: %d.",
			 function,
			 sub_node_index );

			return( -1 );
		}
		if( index_tree_sub_node_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing index tree sub node value: %d.",
			 function,
			 sub_node_index );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: index tree sub node value: %d identifier\t: 0x%08" PRIx64 " (%" PRIu64 ").\n",
			 function,
			 sub_node_index,
			 index_tree_sub_node_value->identifier,
			 index_tree_sub_node_value->identifier );
		}
#endif
		if( identifier > index_tree_sub_node_value->identifier )
		{
			compare = 1;
		}
		else if( identifier < index_tree_sub_node_value->identifier )
		{
			compare = -1;
		}
		else
		{
			compare = 0;
		}
		result = libfdata_tree_node_is_leaf(
		          index_tree_sub_node,
		          (intptr_t *) file_io_handle,
		          (libfdata_cache_t *) cache,
		          0,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if index tree sub node: %d is a leaf node.",
			 function,
			 sub_node_index );

			return( -1 );
		}
		else if( result != 0 )
		{
			result = libfdata_tree_node_is_deleted(
				  index_tree_sub_node,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine if index tree sub node: %d is a deleted node.",
				 function,
				 sub_node_index );

				return( -1 );
			}
			else if( result == 0 )
			{
				if( compare == 0 )
				{
					if( number_of_leaf_nodes == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
						 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
						 "%s: invalid number of leaf nodes.",
						 function );

						return( -1 );
					}
					*number_of_leaf_nodes += 1;
				}
			}
		}
		else
		{
			/* A branch node contains the identifier of its first sub node
			 */
			if( ( compare == 0 )
			 || ( ( compare > 0 )
			   && ( sub_node_index == ( number_of_sub_nodes - 1 ) ) ) )
			{
				result = libpff_index_tree_node_get_number_of_leaf_nodes_by_identifier(
					  index_tree_sub_node,
					  file_io_handle,
					  cache,
					  identifier,
					  number_of_leaf_nodes,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve leaf index tree node by identifier in sub node: %d.",
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
				     index_tree_node,
				     (intptr_t *) file_io_handle,
				     (libfdata_cache_t *) cache,
				     sub_node_index - 1,
				     &index_tree_sub_node,
				     0,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve sub node: %d from index tree node.",
					 function,
					 sub_node_index - 1 );

					return( -1 );
				}
				result = libpff_index_tree_node_get_number_of_leaf_nodes_by_identifier(
					  index_tree_sub_node,
					  file_io_handle,
					  cache,
					  identifier,
					  number_of_leaf_nodes,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve leaf index tree node by identifier in sub node: %d.",
					 function,
					 sub_node_index - 1 );

					return( -1 );
				}
				break;
			}
		}
	}
	return( 1 );
}

/* Retrieves the leaf node for the specific identifier
 * Returns 1 if successful, 0 if no value was found or -1 on error
 */
int libpff_index_tree_get_leaf_node_by_identifier(
     libfdata_tree_t *index_tree,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *cache,
     uint64_t identifier,
     int *leaf_node_index,
     libfdata_tree_node_t **leaf_index_tree_node,
     libcerror_error_t **error )
{
	libfdata_tree_node_t *index_tree_root_node = NULL;
	static char *function                      = "libpff_index_tree_get_leaf_node_by_identifier";
	int result                                 = 0;

	if( index_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index tree.",
		 function );

		return( -1 );
	}
	if( leaf_index_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid leaf index tree node.",
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
	     index_tree,
	     &index_tree_root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve root node from index tree.",
		 function );

		return( -1 );
	}
	result = libpff_index_tree_node_get_leaf_node_by_identifier(
		  index_tree_root_node,
		  file_io_handle,
		  cache,
		  identifier,
		  leaf_node_index,
		  leaf_index_tree_node,
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
int libpff_index_tree_node_get_leaf_node_by_identifier(
     libfdata_tree_node_t *index_tree_node,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *cache,
     uint64_t identifier,
     int *leaf_node_index,
     libfdata_tree_node_t **leaf_index_tree_node,
     libcerror_error_t **error )
{
	libfdata_tree_node_t *index_tree_sub_node       = NULL;
	libpff_index_value_t *index_tree_sub_node_value = NULL;
	static char *function                           = "libpff_index_tree_node_get_leaf_node_by_identifier";
	int16_t compare                                 = 0;
	int number_of_sub_nodes                         = 0;
	int result                                      = 0;
	int sub_node_index                              = 0;

	if( index_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index tree node.",
		 function );

		return( -1 );
	}
	if( libfdata_tree_node_get_number_of_sub_nodes(
	     index_tree_node,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) cache,
	     &number_of_sub_nodes,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub nodes from index tree node.",
		 function );

		return( -1 );
	}
	for( sub_node_index = 0;
	     sub_node_index < number_of_sub_nodes;
	     sub_node_index++ )
	{
		if( libfdata_tree_node_get_sub_node_by_index(
		     index_tree_node,
		     (intptr_t *) file_io_handle,
		     (libfdata_cache_t *) cache,
		     sub_node_index,
		     &index_tree_sub_node,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub node: %d from index tree node.",
			 function,
			 sub_node_index );

			return( -1 );
		}
		if( libfdata_tree_node_get_node_value(
		     index_tree_sub_node,
		     (intptr_t *) file_io_handle,
		     (libfdata_cache_t *) cache,
		     (intptr_t **) &index_tree_sub_node_value,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve index tree sub node value: %d.",
			 function,
			 sub_node_index );

			return( -1 );
		}
		if( index_tree_sub_node_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing index tree sub node value: %d.",
			 function,
			 sub_node_index );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: index tree sub node value: %d identifier\t: 0x%08" PRIx64 " (%" PRIu64 ").\n",
			 function,
			 sub_node_index,
			 index_tree_sub_node_value->identifier,
			 index_tree_sub_node_value->identifier );
		}
#endif
		if( identifier > index_tree_sub_node_value->identifier )
		{
			compare = 1;
		}
		else if( identifier < index_tree_sub_node_value->identifier )
		{
			compare = -1;
		}
		else
		{
			compare = 0;
		}
		result = libfdata_tree_node_is_leaf(
		          index_tree_sub_node,
		          (intptr_t *) file_io_handle,
		          (libfdata_cache_t *) cache,
		          0,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if index tree sub node: %d is a leaf node.",
			 function,
			 sub_node_index );

			return( -1 );
		}
		else if( result != 0 )
		{
			result = libfdata_tree_node_is_deleted(
				  index_tree_sub_node,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine if index tree sub node: %d is a deleted node.",
				 function,
				 sub_node_index );

				return( -1 );
			}
			else if( result == 0 )
			{
				if( compare == 0 )
				{
					if( leaf_node_index == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
						 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
						 "%s: invalid leaf node index.",
						 function );

						return( -1 );
					}
					if( *leaf_node_index == 0 )
					{
						if( leaf_index_tree_node == NULL )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
							 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
							 "%s: invalid leaf index tree node.",
							 function );

							return( -1 );
						}
						if( *leaf_index_tree_node != NULL )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
							 "%s: leaf index tree node value already set.",
							 function );

							return( -1 );
						}
						*leaf_index_tree_node = index_tree_sub_node;

						result = 1;

						break;
					}
					else
					{
						*leaf_node_index -= 1;
					}
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
				result = libpff_index_tree_node_get_leaf_node_by_identifier(
					  index_tree_sub_node,
					  file_io_handle,
					  cache,
					  identifier,
					  leaf_node_index,
					  leaf_index_tree_node,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve leaf index tree node by identifier in sub node: %d.",
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
				     index_tree_node,
				     (intptr_t *) file_io_handle,
				     (libfdata_cache_t *) cache,
				     sub_node_index - 1,
				     &index_tree_sub_node,
				     0,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve sub node: %d from index tree node.",
					 function,
					 sub_node_index - 1 );

					return( -1 );
				}
				result = libpff_index_tree_node_get_leaf_node_by_identifier(
					  index_tree_sub_node,
					  file_io_handle,
					  cache,
					  identifier,
					  leaf_node_index,
					  leaf_index_tree_node,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve leaf index tree node by identifier in sub node: %d.",
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

/* Retrieves the index value for the specific identifier
 * Returns 1 if successful, 0 if no value was found or -1 on error
 */
int libpff_index_tree_get_value_by_identifier(
     libfdata_tree_t *index_tree,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *cache,
     uint64_t identifier,
     int value_index,
     libpff_index_value_t **index_value,
     libcerror_error_t **error )
{
	libfdata_tree_node_t *leaf_index_tree_node = NULL;
	static char *function                      = "libpff_index_tree_get_value_by_identifier";
	int result                                 = 0;

	result = libpff_index_tree_get_leaf_node_by_identifier(
		  index_tree,
		  file_io_handle,
		  cache,
		  identifier,
		  &value_index,
		  &leaf_index_tree_node,
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
		if( leaf_index_tree_node == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing leaf index tree node.",
			 function );

			return( -1 );
		}
		if( libfdata_tree_node_get_node_value(
		     leaf_index_tree_node,
		     (intptr_t *) file_io_handle,
		     (libfdata_cache_t *) cache,
		     (intptr_t **) index_value,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve leaf index tree node value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the upper branch node for the specific identifier
 * Returns 1 if successful, 0 if no value was found or -1 on error
 */
int libpff_index_tree_node_get_upper_branch_node_by_identifier(
     libfdata_tree_node_t *index_tree_node,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *cache,
     uint64_t identifier,
     libfdata_tree_node_t **upper_branch_index_tree_node,
     libcerror_error_t **error )
{
	libfdata_tree_node_t *index_tree_sub_node       = NULL;
	libpff_index_value_t *index_tree_sub_node_value = NULL;
	static char *function                           = "libpff_index_tree_node_get_upper_branch_node_by_identifier";
	int16_t compare                                 = 0;
	int number_of_sub_nodes                         = 0;
	int result                                      = 0;
	int sub_node_index                              = 0;

	if( index_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index tree node.",
		 function );

		return( -1 );
	}
	if( upper_branch_index_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid upper branch index tree node.",
		 function );

		return( -1 );
	}
	if( libfdata_tree_node_get_number_of_sub_nodes(
	     index_tree_node,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) cache,
	     &number_of_sub_nodes,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub nodes from index tree node.",
		 function );

		return( -1 );
	}
	if( number_of_sub_nodes == 0 )
	{
		result = libfdata_tree_node_is_leaf(
		          index_tree_node,
		          (intptr_t *) file_io_handle,
		          (libfdata_cache_t *) cache,
		          0,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if index tree node is a leaf node.",
			 function,
			 sub_node_index );

			return( -1 );
		}
		else if( result != 0 )
		{
			return( 0 );
		}
		result = libfdata_tree_node_is_deleted(
		          index_tree_node,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if index tree node is deleted.",
			 function,
			 sub_node_index );

			return( -1 );
		}
		else if( result != 0 )
		{
			return( 0 );
		}
		if( *upper_branch_index_tree_node != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: upper branch index tree node value already set.",
			 function );

			return( -1 );
		}
		*upper_branch_index_tree_node = index_tree_node;

		return( 1 );
	}
	for( sub_node_index = 0;
	     sub_node_index < number_of_sub_nodes;
	     sub_node_index++ )
	{
		if( libfdata_tree_node_get_sub_node_by_index(
		     index_tree_node,
		     (intptr_t *) file_io_handle,
		     (libfdata_cache_t *) cache,
		     sub_node_index,
		     &index_tree_sub_node,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub node: %d from index tree node.",
			 function,
			 sub_node_index );

			return( -1 );
		}
		if( libfdata_tree_node_get_node_value(
		     index_tree_sub_node,
		     (intptr_t *) file_io_handle,
		     (libfdata_cache_t *) cache,
		     (intptr_t **) &index_tree_sub_node_value,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve index tree sub node value: %d.",
			 function,
			 sub_node_index );

			return( -1 );
		}
		if( index_tree_sub_node_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing index tree sub node value: %d.",
			 function,
			 sub_node_index );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: index tree sub node value: %d identifier\t: 0x%08" PRIx64 " (%" PRIu64 ").\n",
			 function,
			 sub_node_index,
			 index_tree_sub_node_value->identifier,
			 index_tree_sub_node_value->identifier );
		}
#endif
		if( identifier > index_tree_sub_node_value->identifier )
		{
			compare = 1;
		}
		else if( identifier < index_tree_sub_node_value->identifier )
		{
			compare = -1;
		}
		else
		{
			compare = 0;
		}
		result = libfdata_tree_node_is_leaf(
		          index_tree_sub_node,
		          (intptr_t *) file_io_handle,
		          (libfdata_cache_t *) cache,
		          0,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if index tree sub node: %d is a leaf node.",
			 function,
			 sub_node_index );

			return( -1 );
		}
		else if( result != 0 )
		{
			result = libfdata_tree_node_is_deleted(
				  index_tree_sub_node,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine if index tree sub node: %d is a deleted node.",
				 function,
				 sub_node_index );

				return( -1 );
			}
			else if( result == 0 )
			{
				if( compare == 0 )
				{
					if( *upper_branch_index_tree_node != NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
						 "%s: upper branch index tree node value already set.",
						 function );

						return( -1 );
					}
					*upper_branch_index_tree_node = index_tree_node;

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
				result = libpff_index_tree_node_get_upper_branch_node_by_identifier(
					  index_tree_sub_node,
					  file_io_handle,
					  cache,
					  identifier,
					  upper_branch_index_tree_node,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve upper branch index tree node by identifier in sub node: %d.",
					 function,
					 sub_node_index );

					return( -1 );
				}
				else if( result == 0 )
				{
					if( *upper_branch_index_tree_node == NULL )
					{
						*upper_branch_index_tree_node = index_tree_sub_node;

						result = 1;
					}
				}
				break;
			}
			else if( ( compare < 0 )
			      && ( sub_node_index >= 1 ) )
			{
				if( libfdata_tree_node_get_sub_node_by_index(
				     index_tree_node,
				     (intptr_t *) file_io_handle,
				     (libfdata_cache_t *) cache,
				     sub_node_index - 1,
				     &index_tree_sub_node,
				     0,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve sub node: %d from index tree node.",
					 function,
					 sub_node_index - 1 );

					return( -1 );
				}
				result = libpff_index_tree_node_get_upper_branch_node_by_identifier(
					  index_tree_sub_node,
					  file_io_handle,
					  cache,
					  identifier,
					  upper_branch_index_tree_node,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve upper branch index tree node by identifier in sub node: %d.",
					 function,
					 sub_node_index - 1 );

					return( -1 );
				}
				else if( result == 0 )
				{
					if( *upper_branch_index_tree_node == NULL )
					{
						*upper_branch_index_tree_node = index_tree_sub_node;

						result = 1;
					}
				}
				break;
			}
		}
	}
	if( result == 0 )
	{
		if( *upper_branch_index_tree_node == NULL )
		{
			*upper_branch_index_tree_node = index_tree_node;

			result = 1;
		}
	}
	return( result );
}

/* Inserts an index value to the index tree
 * Returns 1 if successful or -1 on error
 */
int libpff_index_tree_insert_value(
     libfdata_tree_t *index_tree,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *cache,
     uint64_t identifier,
     off64_t node_data_offset,
     size64_t node_data_size,
     libcerror_error_t **error )
{
	libfdata_tree_node_t *index_tree_branch_node       = NULL;
	libfdata_tree_node_t *index_tree_root_node         = NULL;
	libpff_index_value_t *index_tree_branch_node_value = NULL;
	static char *function                              = "libpff_index_tree_insert_value";
	int number_of_sub_nodes                            = 0;
	int sub_node_index                                 = 0;

	if( index_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index tree.",
		 function );

		return( -1 );
	}
	if( libfdata_tree_get_root_node(
	     index_tree,
	     &index_tree_root_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve root node from index tree.",
		 function );

		return( -1 );
	}
	if( libpff_index_tree_node_get_upper_branch_node_by_identifier(
	     index_tree_root_node,
	     file_io_handle,
	     cache,
	     identifier,
	     &index_tree_branch_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve upper branch index tree node by identifier: %" PRIu64 ".",
		 function,
		 identifier );

		return( -1 );
	}
	if( libfdata_tree_node_get_number_of_sub_nodes(
	     index_tree_branch_node,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) cache,
	     &number_of_sub_nodes,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub nodes of branch node.",
		 function );

		return( -1 );
	}
	if( number_of_sub_nodes >= 512 )
	{
		if( libfdata_tree_node_get_node_value(
		     index_tree_branch_node,
		     (intptr_t *) file_io_handle,
		     (libfdata_cache_t *) cache,
		     (intptr_t **) &index_tree_branch_node_value,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve index tree branch node value.",
			 function );

			return( -1 );
		}
		if( index_tree_branch_node_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing index tree branch node value.",
			 function,
			 sub_node_index );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: index tree branch node value identifier\t: 0x%08" PRIx64 " (%" PRIu64 ").\n",
			 function,
			 index_tree_branch_node_value->identifier,
			 index_tree_branch_node_value->identifier );
		}
#endif
		if( libfdata_tree_node_split_sub_nodes(
		     index_tree_branch_node,
		     32,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to split index tree branch node value.",
			 function );

			return( -1 );
		}
		index_tree_root_node   = index_tree_branch_node;
		index_tree_branch_node = NULL;

		if( libpff_index_tree_node_get_upper_branch_node_by_identifier(
		     index_tree_root_node,
		     file_io_handle,
		     cache,
		     identifier,
		     &index_tree_branch_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve upper branch index tree node by identifier: %" PRIu64 ".",
			 function,
			 identifier );

			return( -1 );
		}
	}
	if( libfdata_tree_node_insert_sub_node(
	     index_tree_branch_node,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) cache,
	     &sub_node_index,
	     0,
	     node_data_offset,
	     node_data_size,
	     0,
	     (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &libpff_index_value_compare,
             LIBFDATA_TREE_NODE_INSERT_FLAG_NON_UNIQUE_SUB_NODE_VALUES,
             0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to insert index value: %" PRIu64 " in branch index tree node.",
		 function,
		 identifier );

		return( -1 );
	}
	if( libfdata_tree_node_set_leaf_sub_node(
	     index_tree_branch_node,
	     sub_node_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set leaf in index tree sub node: %d.",
		 function,
		 sub_node_index );

		return( -1 );
	}
	return( 1 );
}

