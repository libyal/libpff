/*
 * Index functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libpff_block_descriptor.h"
#include "libpff_block_tree.h"
#include "libpff_block_tree_node.h"
#include "libpff_definitions.h"
#include "libpff_index.h"
#include "libpff_index_node.h"
#include "libpff_index_value.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfdata.h"
#include "libpff_libfmapi.h"
#include "libpff_unused.h"

#include "pff_index_node.h"

/* Creates an index
 * Make sure the value index is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_index_initialize(
     libpff_index_t **index,
     uint8_t index_type,
     off64_t root_node_offset,
     uint64_t root_node_back_pointer,
     libcerror_error_t **error )
{
	static char *function = "libpff_index_initialize";

	if( index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index.",
		 function );

		return( -1 );
	}
	if( *index != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid index value already set.",
		 function );

		return( -1 );
	}
	*index = memory_allocate_structure(
	          libpff_index_t );

	if( *index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create index.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *index,
	     0,
	     sizeof( libpff_index_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear index.",
		 function );

		goto on_error;
	}
	( *index )->type                   = index_type;
	( *index )->root_node_offset       = root_node_offset;
	( *index )->root_node_back_pointer = root_node_back_pointer;

	return( 1 );

on_error:
	if( *index != NULL )
	{
		memory_free(
		 *index );

		*index = NULL;
	}
	return( -1 );
}

/* Frees an index
 * Returns 1 if successful or -1 on error
 */
int libpff_index_free(
     libpff_index_t **index,
     libcerror_error_t **error )
{
	static char *function = "libpff_index_free";

	if( index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index.",
		 function );

		return( -1 );
	}
	if( *index != NULL )
	{
		memory_free(
		 *index );

		*index = NULL;
	}
	return( 1 );
}

/* Checks if this is the first time the index node block is being read
 * Returns 1 if successful or -1 on error
 */
int libpff_index_check_if_node_block_first_read(
     libpff_index_t *index,
     libpff_block_tree_t *node_block_tree,
     off64_t node_offset,
     uint64_t identifier,
     libcerror_error_t **error )
{
	libpff_block_descriptor_t *existing_block_descriptor = NULL;
	libpff_block_descriptor_t *new_block_descriptor      = NULL;
	libpff_block_tree_node_t *leaf_block_tree_node       = NULL;
	static char *function                                = "libpff_index_check_if_node_block_first_read";
	int leaf_value_index                                 = 0;
	int result                                           = 0;

	if( index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index.",
		 function );

		return( -1 );
	}
	if( libpff_block_descriptor_initialize(
	     &new_block_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create block descriptor.",
		 function );

		goto on_error;
	}
	new_block_descriptor->identifier = identifier;

	result = libpff_block_tree_insert_block_descriptor_by_offset(
	          node_block_tree,
	          node_offset,
	          new_block_descriptor,
	          &leaf_value_index,
	          &leaf_block_tree_node,
	          &existing_block_descriptor,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to insert block descriptor for index node at offset: %" PRIi64 " (0x%08" PRIx64 ") in node block tree.",
		 function,
		 node_offset,
		 node_offset );

		goto on_error;
	}
	else if( result == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid index node at offset: %" PRIi64 " (0x%08" PRIx64 ") value already exists.",
		 function,
		 node_offset,
		 node_offset );

		goto on_error;
	}
	new_block_descriptor = NULL;

	return( 1 );

on_error:
	if( new_block_descriptor != NULL )
	{
		libpff_block_descriptor_free(
		 &new_block_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the leaf node from an index node for the specific identifier
 * Returns 1 if successful, 0 if no leaf node was found or -1 on error
 */
int libpff_index_get_leaf_node_from_node_by_identifier(
     libpff_index_t *index,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_block_tree_t *node_block_tree,
     off64_t node_offset,
     uint64_t node_back_pointer,
     uint64_t identifier,
     libpff_index_node_t **leaf_node,
     uint16_t *leaf_node_entry_index,
     libcerror_error_t **error )
{
	libpff_index_node_t *index_node = NULL;
	uint8_t *node_entry_data        = NULL;
	static char *function           = "libpff_index_get_leaf_node_from_node_by_identifier";
	uint64_t entry_identifier       = 0;
	uint64_t sub_node_back_pointer  = 0;
	uint64_t sub_node_offset        = 0;
	uint16_t entry_index            = 0;
	int result                      = 0;

	if( index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index.",
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
	if( leaf_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid leaf node.",
		 function );

		return( -1 );
	}
	if( leaf_node_entry_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid leaf node entry index.",
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
	if( libpff_index_check_if_node_block_first_read(
	     index,
	     node_block_tree,
	     node_offset,
	     identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to check if first read of index node at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 node_offset,
		 node_offset );

		goto on_error;
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
	if( index->type != index_node->type )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: index type mismatch (index: 0x%02" PRIx8 ", node: 0x%02" PRIx8 ").",
		 function,
		 index->type,
		 index_node->type );

		/* TODO error tollerance */

		goto on_error;
	}
	if( index_node->level != LIBPFF_INDEX_NODE_LEVEL_LEAF )
	{
		if( index_node->back_pointer != node_back_pointer )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: back pointer mismatch (index entry: %" PRIu64 ", node: %" PRIu64 ").",
			 function,
			 node_back_pointer,
			 index_node->back_pointer );

			/* TODO error tollerance */

			goto on_error;
		}
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
		if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (pff_index_node_branch_entry_32bit_t *) node_entry_data )->identifier,
			 entry_identifier );
		}
		else if( ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
		      || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
		{
			byte_stream_copy_to_uint64_little_endian(
			 ( (pff_index_node_branch_entry_64bit_t *) node_entry_data )->identifier,
			 entry_identifier );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: node entry: %" PRIu16 " identifier\t: 0x%08" PRIx64 " (%" PRIu64 ").\n",
			 function,
			 entry_index,
			 entry_identifier,
			 entry_identifier );
		}
#endif
		/* Ignore the upper 32-bit of descriptor identifiers
		 */
		if( index_node->type == LIBPFF_INDEX_TYPE_DESCRIPTOR )
		{
			entry_identifier &= 0xffffffffUL;
		}
		if( index_node->level != LIBPFF_INDEX_NODE_LEVEL_LEAF )
		{
			if( ( entry_index == 0 )
			 || ( identifier >= entry_identifier ) )
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
			}
		}
		else if( identifier == entry_identifier )
		{
			*leaf_node             = index_node;
			*leaf_node_entry_index = entry_index;

			result = 1;
		}
		/* A branch node contains the identifier of its first sub node
		 */
		if( identifier <= entry_identifier )
		{
			break;
		}
	}
	if( index_node->level != LIBPFF_INDEX_NODE_LEVEL_LEAF )
	{
		if( sub_node_offset > (uint64_t) INT64_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid sub node offset value out of bounds.",
			 function );

			goto on_error;
		}
		result = libpff_index_get_leaf_node_from_node_by_identifier(
			  index,
			  io_handle,
			  file_io_handle,
			  node_block_tree,
			  sub_node_offset,
			  sub_node_back_pointer,
			  identifier,
			  leaf_node,
			  leaf_node_entry_index,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve leaf node by identifier: 0x%08" PRIx64 " (%" PRIu64 ") from node at offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 identifier,
			 identifier,
			 node_offset,
			 node_offset );

			goto on_error;
		}
	}
	if( ( index_node != NULL )
	 && ( index_node != *leaf_node ) )
	{
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
	}
	return( result );

on_error:
	if( index_node != NULL )
	{
		libpff_index_node_free(
		 &index_node,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the value for the specific identifier
 * Returns 1 if successful, 0 if no value was found or -1 on error
 */
int libpff_index_get_value_by_identifier(
     libpff_index_t *index,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint64_t identifier,
     libpff_index_value_t **index_value,
     libcerror_error_t **error )
{
	libpff_block_tree_t *node_block_tree   = NULL;
	libpff_index_node_t *leaf_node         = NULL;
	libpff_index_value_t *safe_index_value = NULL;
	uint8_t *node_entry_data               = NULL;
	static char *function                  = "libpff_index_get_value_by_identifier";
	size_t index_node_size                 = 0;
	uint16_t leaf_node_entry_index         = 0;
	int result                             = 0;

	if( index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index.",
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
	if( index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index value.",
		 function );

		return( -1 );
	}
	if( ( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
	 || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT ) )
	{
		index_node_size = 512;
	}
	else if( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE )
	{
		index_node_size = 4096;
	}
	if( libpff_block_tree_initialize(
	     &node_block_tree,
	     io_handle->file_size,
	     index_node_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create index node block tree.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: requested identifier\t\t: 0x%08" PRIx64 " (%" PRIu64 ").\n",
		 function,
		 identifier,
		 identifier );
	}
#endif
	result = libpff_index_get_leaf_node_from_node_by_identifier(
		  index,
		  io_handle,
		  file_io_handle,
		  node_block_tree,
		  index->root_node_offset,
		  index->root_node_back_pointer,
		  identifier,
		  &leaf_node,
		  &leaf_node_entry_index,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve leaf node by identifier: 0x%08" PRIx64 " (%" PRIu64 ") from root node.",
		 function,
		 identifier,
		 identifier );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libpff_index_node_get_entry_data(
		     leaf_node,
		     leaf_node_entry_index,
		     &node_entry_data,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve node entry: %" PRIu16 " data.",
			 function,
			 leaf_node_entry_index );

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
			 leaf_node_entry_index );

			goto on_error;
		}
		if( libpff_index_value_initialize(
		     &safe_index_value,
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
		     safe_index_value,
		     io_handle,
		     index->type,
		     node_entry_data,
		     (size_t) leaf_node->entry_size,
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
		if( libpff_index_node_free(
		     &leaf_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free leaf node.",
			 function );

			goto on_error;
		}
	}
	if( libpff_block_tree_free(
	     &node_block_tree,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_block_descriptor_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free index node block tree.",
		 function );

		goto on_error;
	}
	*index_value = safe_index_value;

	return( result );

on_error:
	if( safe_index_value != NULL )
	{
		libpff_index_value_free(
		 &safe_index_value,
		 NULL );
	}
	if( leaf_node != NULL )
	{
		libpff_index_node_free(
		 &leaf_node,
		 NULL );
	}
	if( node_block_tree != NULL )
	{
		libpff_block_tree_free(
		 &node_block_tree,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_block_descriptor_free,
		 NULL );
	}
	return( -1 );
}

