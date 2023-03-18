/*
 * Item tree functions
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
#include "libpff_descriptors_index.h"
#include "libpff_index_value.h"
#include "libpff_item_descriptor.h"
#include "libpff_libbfio.h"
#include "libpff_libcdata.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_item_descriptor.h"
#include "libpff_item_tree.h"

#include "pff_index_node.h"

/* Creates an item tree
 * Make sure the value item_tree is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_item_tree_initialize(
     libpff_item_tree_t **item_tree,
     libcerror_error_t **error )
{
	static char *function = "libpff_item_tree_initialize";

	if( item_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item tree.",
		 function );

		return( -1 );
	}
	if( *item_tree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid item tree value already set.",
		 function );

		return( -1 );
	}
	*item_tree = memory_allocate_structure(
	              libpff_item_tree_t );

	if( *item_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create item tree.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *item_tree,
	     0,
	     sizeof( libpff_item_tree_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear item tree.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *item_tree != NULL )
	{
		memory_free(
		 *item_tree );

		*item_tree = NULL;
	}
	return( -1 );
}

/* Frees an item tree
 * Returns 1 if successful or -1 on error
 */
int libpff_item_tree_free(
     libpff_item_tree_t **item_tree,
     libcerror_error_t **error )
{
	static char *function = "libpff_item_tree_free";
	int result            = 1;

	if( item_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item tree.",
		 function );

		return( -1 );
	}
	if( *item_tree != NULL )
	{
		if( ( *item_tree )->root_node != NULL )
		{
			if( libcdata_tree_node_free(
			     &( ( *item_tree )->root_node ),
			     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_descriptor_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free root node.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *item_tree );

		*item_tree = NULL;
	}
	return( result );
}

/* Frees a recovered item tree node
 * Returns 1 if successful or -1 on error
 */
int libpff_item_tree_node_free_recovered(
     libcdata_tree_node_t **item_tree_node,
     libcerror_error_t **error )
{
	static char *function = "libpff_item_tree_node_free_recovered";
	int result            = 1;

	if( item_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item tree node.",
		 function );

		return( -1 );
	}
	if( *item_tree_node != NULL )
	{
		if( libcdata_tree_node_free(
		     item_tree_node,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_descriptor_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free tree node.",
			 function );

			result = -1;
		}
	}
	return( result );
}

/* Retrieves the tree node of an item node
 * Returns 1 if successful, 0 if the item node was not found or -1 on error
 */
int libpff_item_tree_get_tree_node_by_identifier(
     libcdata_tree_node_t *item_tree_node,
     uint32_t item_identifier,
     libcdata_tree_node_t **result_item_tree_node,
     int recursion_depth,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *sub_tree_node       = NULL;
	libpff_item_descriptor_t *item_descriptor = NULL;
	static char *function                     = "libpff_item_tree_get_tree_node_by_identifier";
	int number_of_sub_nodes                   = 0;
	int result                                = 0;
	int sub_node_index                        = 0;

	if( item_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item tree node.",
		 function );

		return( -1 );
	}
	if( ( recursion_depth < 0 )
	 || ( recursion_depth > LIBPFF_MAXIMUM_ITEM_TREE_RECURSION_DEPTH ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid recursion depth value out of bounds.",
		 function );

		return( -1 );
	}
	if( libcdata_tree_node_get_value(
	     item_tree_node,
	     (intptr_t **) &item_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve item descriptor.",
		 function );

		return( -1 );
	}
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
	if( item_descriptor->descriptor_identifier == item_identifier )
	{
		if( result_item_tree_node == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid result item tree node.",
			 function );

			return( -1 );
		}
		if( *result_item_tree_node != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: result item tree node already set.",
			 function );

			return( -1 );
		}
		*result_item_tree_node = item_tree_node;

		return( 1 );
	}
	if( libcdata_tree_node_get_number_of_sub_nodes(
	     item_tree_node,
	     &number_of_sub_nodes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub nodes.",
		 function );

		return( -1 );
	}
	if( number_of_sub_nodes > 0 )
	{
		if( libcdata_tree_node_get_sub_node_by_index(
		     item_tree_node,
		     0,
		     &sub_tree_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve first sub node.",
			 function );

			return( -1 );
		}
		for( sub_node_index = 0;
		     sub_node_index < number_of_sub_nodes;
		     sub_node_index++ )
		{
			if( sub_tree_node == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: corruption detected for sub node: %d.",
				 function,
				 sub_node_index );

				return( -1 );
			}
			result = libpff_item_tree_get_tree_node_by_identifier(
			          sub_tree_node,
			          item_identifier,
			          result_item_tree_node,
			          recursion_depth + 1,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to traverse sub node: %d.",
				 function,
				 sub_node_index );

				return( -1 );
			}
			else if( result != 0 )
			{
				break;
			}
			if( libcdata_tree_node_get_next_node(
			     sub_tree_node,
			     &sub_tree_node,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve next node of sub node: %d.",
				 function,
				 sub_node_index );

				return( -1 );
			}
		}
	}
	return( result );
}

/* Retrieves the sub node of an item node
 * Returns 1 if successful, 0 if the item node was not found or -1 on error
 */
int libpff_item_tree_get_sub_node_by_identifier(
     libcdata_tree_node_t *item_tree_node,
     uint32_t sub_node_identifier,
     libcdata_tree_node_t **sub_node,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *sub_tree_node       = NULL;
	libpff_item_descriptor_t *item_descriptor = NULL;
	static char *function                     = "libpff_item_tree_get_sub_node_by_identifier";
	int number_of_sub_nodes                   = 0;
	int result                                = 0;
	int sub_node_index                        = 0;

	if( item_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item tree node.",
		 function );

		return( -1 );
	}
	if( sub_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub node.",
		 function );

		return( -1 );
	}
	if( *sub_node != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub node already set.",
		 function );

		return( -1 );
	}
	if( libcdata_tree_node_get_value(
	     item_tree_node,
	     (intptr_t **) &item_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve item descriptor.",
		 function );

		return( -1 );
	}
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
	if( libcdata_tree_node_get_number_of_sub_nodes(
	     item_tree_node,
	     &number_of_sub_nodes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub nodes.",
		 function );

		return( -1 );
	}
	if( number_of_sub_nodes > 0 )
	{
		if( libcdata_tree_node_get_sub_node_by_index(
		     item_tree_node,
		     0,
		     &sub_tree_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve first sub node.",
			 function );

			return( -1 );
		}
		for( sub_node_index = 0;
		     sub_node_index < number_of_sub_nodes;
		     sub_node_index++ )
		{
			if( libcdata_tree_node_get_value(
			     sub_tree_node,
			     (intptr_t **) &item_descriptor,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve item descriptor from sub node: %d.",
				 function,
				 sub_node_index );

				return( -1 );
			}
			if( item_descriptor == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing sub item descriptor: %d.",
				 function,
				 sub_node_index );

				return( -1 );
			}
			if( item_descriptor->descriptor_identifier == sub_node_identifier )
			{
				*sub_node = sub_tree_node;

				return( 1 );
			}
			if( libcdata_tree_node_get_next_node(
			     sub_tree_node,
			     &sub_tree_node,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve next node of sub node: %d.",
				 function,
				 sub_node_index );

				return( -1 );
			}
		}
	}
	return( result );
}

/* Appends the identifier of the item
 * Returns 1 if successful or -1 on error
 */
int libpff_item_tree_append_identifier(
     libcdata_tree_node_t *item_tree_node,
     uint32_t descriptor_identifier,
     uint64_t data_identifier,
     uint64_t local_descriptors_identifier,
     uint8_t recovered,
     libcerror_error_t **error )
{
	libpff_item_descriptor_t *item_descriptor = NULL;
	static char *function                     = "libpff_item_tree_append_identifier";

	if( item_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item tree node.",
		 function );

		return( -1 );
	}
	if( libpff_item_descriptor_initialize(
	     &item_descriptor,
	     descriptor_identifier,
	     data_identifier,
	     local_descriptors_identifier,
	     recovered,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create item descriptor.",
		 function );

		goto on_error;
	}
	if( item_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing item descriptor.",
		 function );

		goto on_error;
	}
	if( libcdata_tree_node_append_value(
	     item_tree_node,
	     (intptr_t *) item_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append item descriptor to item tree node.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( item_descriptor != NULL )
	{
		libpff_item_descriptor_free(
		 &item_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Creates an item tree from the descriptors index
 * Returns 1 if successful or -1 on error
 */
int libpff_item_tree_create(
     libpff_item_tree_t *item_tree,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_descriptors_index_t *descriptors_index,
     libcdata_list_t *orphan_node_list,
     libcdata_tree_node_t **root_folder_item_tree_node,
     libcerror_error_t **error )
{
	libpff_item_descriptor_t *item_descriptor = NULL;
	static char *function                     = "libpff_item_tree_create";

	if( item_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item tree.",
		 function );

		return( -1 );
	}
	if( item_tree->root_node != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid item tree - root node already set.",
		 function );

		return( -1 );
	}
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
	if( descriptors_index->index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid descriptors index - missing index.",
		 function );

		return( -1 );
	}
	if( libpff_item_descriptor_initialize(
	     &item_descriptor,
	     0,
	     0,
	     0,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create item descriptor.",
		 function );

		goto on_error;
	}
	if( libcdata_tree_node_initialize(
	     &( item_tree->root_node ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create item tree root node.",
		 function );

		goto on_error;
	}
	if( libcdata_tree_node_set_value(
	     item_tree->root_node,
	     (intptr_t *) item_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set item descriptor in item tree root node.",
		 function );

		goto on_error;
	}
	/* The item descriptor is now managed by the item tree root node
	 */
	item_descriptor = NULL;

	if( libpff_item_tree_create_node_from_descriptor_index_node(
	     item_tree,
	     io_handle,
	     file_io_handle,
	     descriptors_index,
	     descriptors_index->index->root_node_offset,
	     orphan_node_list,
	     root_folder_item_tree_node,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create item tree.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( item_tree->root_node != NULL )
	{
		libcdata_tree_node_free(
		 &( item_tree->root_node ),
		 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_descriptor_free,
		 NULL );
	}
	if( item_descriptor != NULL )
	{
		libpff_item_descriptor_free(
		 &item_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Creates an item tree node from a descriptor index node
 *
 * If a descriptor index value has no existing parent it is added to the orphan node list
 *
 * Returns 1 if successful or -1 on error
 */
int libpff_item_tree_create_node_from_descriptor_index_node(
     libpff_item_tree_t *item_tree,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_descriptors_index_t *descriptors_index,
     off64_t node_offset,
     libcdata_list_t *orphan_node_list,
     libcdata_tree_node_t **root_folder_item_tree_node,
     int recursion_depth,
     libcerror_error_t **error )
{
	libpff_index_node_t *index_node   = NULL;
	libpff_index_value_t *index_value = NULL;
	uint8_t *node_entry_data          = NULL;
	static char *function             = "libpff_item_tree_create_node_from_descriptor_index_node";
	uint64_t sub_node_back_pointer    = 0;
	uint64_t sub_node_offset          = 0;
	uint16_t entry_index              = 0;

	if( item_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item tree.",
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
	if( root_folder_item_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid root folder item tree node.",
		 function );

		return( -1 );
	}
	if( ( recursion_depth < 0 )
	 || ( recursion_depth > LIBPFF_MAXIMUM_ITEM_TREE_RECURSION_DEPTH ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid recursion depth value out of bounds.",
		 function );

		return( -1 );
	}
	if( libpff_index_node_initialize(
	     &index_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create index node.",
		 function );

		goto on_error;
	}
	if( libpff_index_node_read_file_io_handle(
	     index_node,
	     file_io_handle,
	     node_offset,
	     io_handle->file_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read index node at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 node_offset,
		 node_offset );

		goto on_error;
	}
	for( entry_index = 0;
	     entry_index < index_node->number_of_entries;
	     entry_index++ )
	{
		if( libpff_index_node_get_entry_data(
		     index_node,
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

			goto on_error;
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

			goto on_error;
		}
		if( index_node->level == LIBPFF_INDEX_NODE_LEVEL_LEAF )
		{
			if( libpff_index_value_initialize(
			     &index_value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create index value.",
				 function );

				goto on_error;
			}
			if( libpff_index_value_read_data(
			     index_value,
			     io_handle,
			     LIBPFF_INDEX_TYPE_DESCRIPTOR,
			     node_entry_data,
			     (size_t) index_node->entry_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read index value.",
				 function );

				goto on_error;
			}
			if( libpff_item_tree_create_leaf_node_from_descriptor_index_value(
			     item_tree,
			     io_handle,
			     file_io_handle,
			     descriptors_index,
			     index_value,
			     orphan_node_list,
			     root_folder_item_tree_node,
			     recursion_depth,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create item tree from descriptor index tree leaf node.",
				 function );

				goto on_error;
			}
			if( libpff_index_value_free(
			     &index_value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free index value.",
				 function );

				goto on_error;
			}
		}
		else
		{
			if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
			{
				byte_stream_copy_to_uint32_little_endian(
				 ( (pff_index_node_branch_entry_32bit_t *) node_entry_data )->file_offset,
				 sub_node_offset );

				byte_stream_copy_to_uint32_little_endian(
				 ( (pff_index_node_branch_entry_32bit_t *) node_entry_data )->back_pointer,
				 sub_node_back_pointer );
			}
			else if( ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
			      || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
			{
				byte_stream_copy_to_uint64_little_endian(
				 ( (pff_index_node_branch_entry_64bit_t *) node_entry_data )->file_offset,
				 sub_node_offset );

				byte_stream_copy_to_uint64_little_endian(
				 ( (pff_index_node_branch_entry_64bit_t *) node_entry_data )->back_pointer,
				 sub_node_back_pointer );
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: node entry: %" PRIu16 " sub node offset\t: %" PRIi64 " (0x%08" PRIx64 ")\n",
				 function,
				 entry_index,
				 sub_node_offset,
				 sub_node_offset );
			}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

			if( libpff_item_tree_create_node_from_descriptor_index_node(
			     item_tree,
			     io_handle,
			     file_io_handle,
			     descriptors_index,
			     sub_node_offset,
			     orphan_node_list,
			     root_folder_item_tree_node,
			     recursion_depth + 1,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create item tree node from descriptor index node at offset: %" PRIi64 " (0x%08" PRIx64 ").",
				 function,
				 sub_node_offset,
				 sub_node_offset );

				goto on_error;
			}
		}
	}
	if( libpff_index_node_free(
	     &index_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free index node.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( index_value != NULL )
	{
		libpff_index_value_free(
		 &index_value,
		 NULL );
	}
	if( index_node != NULL )
	{
		libpff_index_node_free(
		 &index_node,
		 NULL );
	}
	if( *root_folder_item_tree_node != NULL )
	{
		libcdata_tree_node_free(
		 root_folder_item_tree_node,
		 NULL,
		 NULL );
	}
	return( -1 );
}

/* Creates an item tree leaf node from the descriptor index value
 *
 * If a descriptor index value has no existing parent it is added to the orphan node list
 *
 * Returns 1 if successful or -1 on error
 */
int libpff_item_tree_create_leaf_node_from_descriptor_index_value(
     libpff_item_tree_t *item_tree,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_descriptors_index_t *descriptors_index,
     libpff_index_value_t *descriptor_index_value,
     libcdata_list_t *orphan_node_list,
     libcdata_tree_node_t **root_folder_item_tree_node,
     int recursion_depth,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *item_tree_node                = NULL;
	libcdata_tree_node_t *parent_node                   = NULL;
	libpff_index_value_t *parent_descriptor_index_value = NULL;
	libpff_item_descriptor_t *item_descriptor           = NULL;
	static char *function                               = "libpff_item_tree_create_leaf_node_from_descriptor_index_value";
	int result                                          = 0;

	if( item_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item tree.",
		 function );

		return( -1 );
	}
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
	if( descriptor_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptor index value.",
		 function );

		return( -1 );
	}
	if( descriptor_index_value->identifier > (uint64_t) UINT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid descriptor index - identifier value exceeds maximum.",
		 function );

		goto on_error;
	}
	if( root_folder_item_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid root folder item tree node.",
		 function );

		return( -1 );
	}
	if( ( recursion_depth < 0 )
	 || ( recursion_depth > LIBPFF_MAXIMUM_ITEM_TREE_RECURSION_DEPTH ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid recursion depth value out of bounds.",
		 function );

		return( -1 );
	}
	/* Create a new item descriptor
	 */
	if( libpff_item_descriptor_initialize(
	     &item_descriptor,
	     (uint32_t) descriptor_index_value->identifier,
	     descriptor_index_value->data_identifier,
	     descriptor_index_value->local_descriptors_identifier,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create item descriptor.",
		 function );

		goto on_error;
	}
	/* The root folder index descriptor points to itself as its parent
	 */
	if( (uint32_t) descriptor_index_value->identifier == descriptor_index_value->parent_identifier )
	{
		if( *root_folder_item_tree_node != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: root folder item tree node already set.",
			 function );

			goto on_error;
		}
		if( libcdata_tree_node_initialize(
		     root_folder_item_tree_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create root folder item tree node.",
			 function );

			goto on_error;
		}
		if( libcdata_tree_node_set_value(
		     *root_folder_item_tree_node,
		     (intptr_t *) item_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set item descriptor in root folder item tree node.",
			 function );

			goto on_error;
		}
		result = libcdata_tree_node_insert_node(
			  item_tree->root_node,
			  *root_folder_item_tree_node,
			  (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &libpff_item_descriptor_compare,
			  LIBCDATA_INSERT_FLAG_UNIQUE_ENTRIES,
			  NULL );

		/* Free the node if it could not be inserted
		 */
		if( result != 1 )
		{
			libcdata_tree_node_free(
			 root_folder_item_tree_node,
			 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_descriptor_free,
			 NULL );
		}
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to insert item descriptor in item tree node.",
			 function );

			goto on_error;
		}
	}
	else
	{
		parent_node = NULL;

		result = libpff_item_tree_get_tree_node_by_identifier(
			  item_tree->root_node,
			  descriptor_index_value->parent_identifier,
			  &parent_node,
			  0,
			  error );

		if( result == 0 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: reading ahead for descriptor: %" PRIu64 " with parent %" PRIu32 ".\n",
				 function,
				 descriptor_index_value->identifier,
				 descriptor_index_value->parent_identifier );
			}
#endif
			result = libpff_index_get_value_by_identifier(
				  descriptors_index->index,
				  io_handle,
				  file_io_handle,
				  descriptor_index_value->parent_identifier,
				  &parent_descriptor_index_value,
				  error );

			if( result == 1 )
			{
				if( parent_descriptor_index_value == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: invalid parent descriptor index value.",
					 function );

					goto on_error;
				}
				if( libpff_item_tree_create_leaf_node_from_descriptor_index_value(
				     item_tree,
				     io_handle,
				     file_io_handle,
				     descriptors_index,
				     parent_descriptor_index_value,
				     orphan_node_list,
				     root_folder_item_tree_node,
				     recursion_depth + 1,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create item tree from parent descriptor: %" PRIu32 ".",
					 function,
					 descriptor_index_value->parent_identifier );

					goto on_error;
				}
				if( libpff_index_value_free(
				     &parent_descriptor_index_value,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free parent descriptor index value.",
					 function );

					goto on_error;
				}
				parent_node = NULL;

				result = libpff_item_tree_get_tree_node_by_identifier(
					  item_tree->root_node,
					  descriptor_index_value->parent_identifier,
					  &parent_node,
					  0,
					  error );
			}
		}
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to find parent node: %" PRIu32 ".",
			 function,
			 descriptor_index_value->parent_identifier );

			goto on_error;
		}
		else if( result == 0 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: parent node: %" PRIu32 " missing - found orphan node: %" PRIu64 ".\n",
				 function,
				 descriptor_index_value->parent_identifier,
				 descriptor_index_value->identifier );
			}
#endif
			if( libcdata_tree_node_initialize(
			     &item_tree_node,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create item tree node.",
				 function );

				goto on_error;
			}
			if( libcdata_tree_node_set_value(
			     item_tree_node,
			     (intptr_t *) item_descriptor,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set item descriptor in item tree root node.",
				 function );

				goto on_error;
			}
			item_descriptor = NULL;

			if( libcdata_list_append_value(
			     orphan_node_list,
			     (intptr_t *) item_tree_node,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append orphan node in orphan node list.",
				 function );

				goto on_error;
			}
		}
		else
		{
			if( parent_node == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid parent node.",
				 function );

				goto on_error;
			}
			result = libcdata_tree_node_insert_value(
			          parent_node,
			          (intptr_t *) item_descriptor,
			          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &libpff_item_descriptor_compare,
			          LIBCDATA_INSERT_FLAG_UNIQUE_ENTRIES,
			          error );

			/* Free the node if it could not be inserted
			 */
			if( result != 1 )
			{
				libpff_item_descriptor_free(
				 &item_descriptor,
				 NULL );
			}
			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to insert item descriptor in item tree node.",
				 function );

				goto on_error;
			}
		}
	}
	return( 1 );

on_error:
	if( parent_descriptor_index_value != NULL )
	{
		libpff_index_value_free(
		 &parent_descriptor_index_value,
		 NULL );
	}
	if( item_tree_node != NULL )
	{
		libcdata_tree_node_free(
		 &item_tree_node,
		 NULL,
		 NULL );
	}
	if( *root_folder_item_tree_node != NULL )
	{
		libcdata_tree_node_free(
		 root_folder_item_tree_node,
		 NULL,
		 NULL );
	}
	if( item_descriptor != NULL )
	{
		libpff_item_descriptor_free(
		 &item_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the tree node of an item node
 * Returns 1 if successful, 0 if the item node was not found or -1 on error
 */
int libpff_item_tree_get_node_by_identifier(
     libpff_item_tree_t *item_tree,
     uint32_t item_identifier,
     libcdata_tree_node_t **item_tree_node,
     libcerror_error_t **error )
{
	static char *function = "libpff_item_tree_get_node_by_identifier";
	int result            = 0;

	if( item_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item tree.",
		 function );

		return( -1 );
	}
	result = libpff_item_tree_get_tree_node_by_identifier(
	          item_tree->root_node,
	          item_identifier,
                  item_tree_node,
                  0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve item tree node: %" PRIu32 ".",
		 function,
		 item_identifier );

		return( -1 );
	}
	return( result );
}

