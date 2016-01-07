/*
 * Index functions
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
#include "libpff_index.h"
#include "libpff_index_node.h"
#include "libpff_index_value.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfcache.h"
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
     libpff_io_handle_t *io_handle,
     uint8_t type,
     off64_t root_node_offset,
     uint64_t root_node_back_pointer,
     uint8_t recovered,
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
	( *index )->io_handle              = io_handle;
	( *index )->type                   = type;
	( *index )->root_node_offset       = root_node_offset;
	( *index )->root_node_back_pointer = root_node_back_pointer;
	( *index )->recovered              = recovered;

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

/* Clones the index
 * Returns 1 if successful or -1 on error
 */
int libpff_index_clone(
     libpff_index_t **destination_index,
     libpff_index_t *source_index,
     libcerror_error_t **error )
{
	static char *function = "libpff_index_clone";

	if( destination_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination index.",
		 function );

		return( -1 );
	}
	if( *destination_index != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination index already set.",
		 function );

		return( -1 );
	}
	if( source_index == NULL )
	{
		*destination_index = NULL;

		return( 1 );
	}
	if( libpff_index_initialize(
	     destination_index,
	     source_index->io_handle,
	     source_index->type,
	     source_index->root_node_offset,
	     source_index->root_node_back_pointer,
	     source_index->recovered,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination index.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the index node
 * Returns 1 if successful or -1 on error
 */
int libpff_index_read_node(
     libpff_index_t *index,
     libbfio_handle_t *file_io_handle,
     off64_t node_offset,
     libfdata_tree_node_t *index_tree_node,
     libpff_index_value_t *index_value,
     libcerror_error_t **error )
{
	libpff_index_node_t *index_node      = NULL;
	static char *function                = "libpff_index_read_node";
	uint8_t *node_entry_data             = NULL;
	off64_t element_data_offset          = 0;
	off64_t index_value_file_offset      = 0;
	off64_t node_data_offset             = 0;
	uint64_t index_value_identifier      = 0;
	uint64_t index_value_data_identifier = 0;
	uint16_t index_value_data_size       = 0;
	uint16_t entry_index                 = 0;
	int sub_node_index                   = 0;

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
	if( index->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid index - missing IO handle.",
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
	if( libfdata_vector_get_element_value_at_offset(
	     index->io_handle->index_nodes_vector,
	     (intptr_t *) file_io_handle,
	     index->io_handle->index_nodes_cache,
	     node_offset,
	     &element_data_offset,
	     (intptr_t **) &index_node,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve index node at offset: %" PRIi64 ".",
		 function,
		 node_offset );

		return( -1 );
	}
	if( index_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing index node.",
		 function );

		return( -1 );
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

		return( -1 );
	}
	if( index_node->number_of_entries > 0 )
	{
		if( index_node->level != LIBPFF_INDEX_NODE_LEVEL_LEAF )
		{
			if( index_value->back_pointer != index_node->back_pointer )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: back pointer mismatch (entry: %" PRIu64 ", node: %" PRIu64 ").",
				 function,
				 index_value->back_pointer,
				 index_node->back_pointer );

				/* TODO error tollerance */

				return( -1 );
			}
		}
		if( libfdata_tree_node_resize_sub_nodes(
		     index_tree_node,
		     (int) index_node->number_of_entries,
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
		node_data_offset = node_offset;

		for( entry_index = 0;
		     entry_index < index_node->number_of_entries;
		     entry_index++ )
		{
			if( libfdata_tree_node_set_sub_node_by_index(
			     index_tree_node,
			     (int) entry_index,
			     0,
			     node_data_offset,
			     (size64_t) entry_index,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set index node entry: %" PRIu16 " as sub node.",
				 function,
				 entry_index );

				return( -1 );
			}
			node_data_offset += (off64_t) index_node->entry_size;
		}
		if( index_node->level == LIBPFF_INDEX_NODE_LEVEL_LEAF )
		{
			for( entry_index = index_node->number_of_entries;
			     entry_index < index_node->maximum_number_of_entries;
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

					return( -1 );
				}
				if( node_entry_data == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing node entry: %" PRIu8 " data.",
					 function,
					 entry_index );

					return( -1 );
				}
				if( index->io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
				{
					byte_stream_copy_to_uint32_little_endian(
					 node_entry_data,
					 index_value_identifier );
				}
				else if( ( index->io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
				      || ( index->io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
				{
					byte_stream_copy_to_uint64_little_endian(
					 node_entry_data,
					 index_value_identifier );
				}
				/* Ignore the upper 32-bit of descriptor identifiers
				 */
				if( index_node->type == LIBPFF_INDEX_TYPE_DESCRIPTOR )
				{
					index_value_identifier &= 0xffffffffUL;
				}
				/* Ignore index values without an identifier
				 */
				if( index_value_identifier == 0 )
				{
					continue;
				}
				if( index_node->type == LIBPFF_INDEX_TYPE_DESCRIPTOR )
				{
					if( index->io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
					{
						byte_stream_copy_to_uint32_little_endian(
						 ( (pff_index_node_descriptor_entry_32bit_t *) node_entry_data )->data_identifier,
						 index_value_data_identifier );
					}
					else if( ( index->io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
					      || ( index->io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
					{
						byte_stream_copy_to_uint64_little_endian(
						 ( (pff_index_node_descriptor_entry_64bit_t *) node_entry_data )->data_identifier,
						 index_value_data_identifier );
					}
					/* Ignore descriptor index values without a data identifier
					 */
					if( index_value_data_identifier == 0 )
					{
						continue;
					}
				}
				else if( index_node->type == LIBPFF_INDEX_TYPE_OFFSET )
				{
					if( index->io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
					{
						byte_stream_copy_to_uint32_little_endian(
						 ( (pff_index_node_offset_entry_32bit_t *) node_entry_data )->file_offset,
						 index_value_file_offset );
						byte_stream_copy_to_uint16_little_endian(
						 ( (pff_index_node_offset_entry_32bit_t *) node_entry_data )->data_size,
						 index_value_data_size );
					}
					else if( ( index->io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
					      || ( index->io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
					{
						byte_stream_copy_to_uint64_little_endian(
						 ( (pff_index_node_offset_entry_64bit_t *) node_entry_data )->file_offset,
						 index_value_file_offset );
						byte_stream_copy_to_uint16_little_endian(
						 ( (pff_index_node_offset_entry_64bit_t *) node_entry_data )->data_size,
						 index_value_data_size );
					}
					/* Ignore offset index values without a file offset and data size
					 */
					if( ( index_value_file_offset <= 0 )
					 || ( index_value_data_size == 0 ) )
					{
						continue;
					}
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: adding deleted index value: %" PRIu64 ".\n",
					 function,
					 index_value_identifier );
				}
#endif
				if( libfdata_tree_node_append_sub_node(
				     index_tree_node,
				     &sub_node_index,
				     0,
				     node_data_offset,
				     (size64_t) entry_index,
				     0,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append index node entry: %" PRIu16 " as sub node.",
					 function,
					 entry_index );

					return( -1 );
				}
				if( libfdata_tree_node_set_deleted_sub_node(
				     index_tree_node,
				     sub_node_index,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set deleted in sub node: %d.",
					 sub_node_index );

					return( -1 );
				}
				node_data_offset += (off64_t) index_node->entry_size;
			}
		}
	}
	return( 1 );
}

/* Reads the index node entry
 * Returns 1 if successful or -1 on error
 */
int libpff_index_read_node_entry(
     libpff_index_t *index,
     libbfio_handle_t *file_io_handle,
     off64_t node_offset,
     uint16_t entry_index,
     libfdata_tree_node_t *index_tree_node,
     libpff_index_value_t *index_value,
     libcerror_error_t **error )
{
	libpff_index_node_t *index_node = NULL;
	uint8_t *node_entry_data        = NULL;
	static char *function           = "libpff_index_read_node_entry";
	off64_t element_data_offset     = 0;
	off64_t sub_nodes_offset        = 0;
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
	if( index->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid index - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( index->io_handle->file_type != LIBPFF_FILE_TYPE_32BIT )
	 && ( index->io_handle->file_type != LIBPFF_FILE_TYPE_64BIT )
	 && ( index->io_handle->file_type != LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file type.",
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
	if( libfdata_vector_get_element_value_at_offset(
	     index->io_handle->index_nodes_vector,
	     (intptr_t *) file_io_handle,
	     index->io_handle->index_nodes_cache,
	     node_offset,
	     &element_data_offset,
	     (intptr_t **) &index_node,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve index node at offset: %" PRIi64 ".",
		 function,
		 node_offset );

		return( -1 );
	}
	if( index_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing index node.",
		 function );

		return( -1 );
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

		return( -1 );
	}
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
	if( index->io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
	{
		byte_stream_copy_to_uint32_little_endian(
		 node_entry_data,
		 index_value->identifier );
	}
	else if( ( index->io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
	      || ( index->io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
	{
		byte_stream_copy_to_uint64_little_endian(
		 node_entry_data,
		 index_value->identifier );
	}
	/* Ignore the upper 32-bit of descriptor identifiers
	 */
	if( index_node->type == LIBPFF_INDEX_TYPE_DESCRIPTOR )
	{
		index_value->identifier &= 0xffffffffUL;
	}
	if( index_node->level == LIBPFF_INDEX_NODE_LEVEL_LEAF )
	{
		if( index_node->type == LIBPFF_INDEX_TYPE_DESCRIPTOR )
		{
			if( index->io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
			{
				byte_stream_copy_to_uint32_little_endian(
				 ( (pff_index_node_descriptor_entry_32bit_t *) node_entry_data )->data_identifier,
				 index_value->data_identifier );
				byte_stream_copy_to_uint32_little_endian(
				 ( (pff_index_node_descriptor_entry_32bit_t *) node_entry_data )->local_descriptors_identifier,
				 index_value->local_descriptors_identifier );
				byte_stream_copy_to_uint32_little_endian(
				 ( (pff_index_node_descriptor_entry_32bit_t *) node_entry_data )->parent_identifier,
				 index_value->parent_identifier );
			}
			else if( ( index->io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
			      || ( index->io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
			{
				byte_stream_copy_to_uint64_little_endian(
				 ( (pff_index_node_descriptor_entry_64bit_t *) node_entry_data )->data_identifier,
				 index_value->data_identifier );
				byte_stream_copy_to_uint64_little_endian(
				 ( (pff_index_node_descriptor_entry_64bit_t *) node_entry_data )->local_descriptors_identifier,
				 index_value->local_descriptors_identifier );
				byte_stream_copy_to_uint32_little_endian(
				 ( (pff_index_node_descriptor_entry_64bit_t *) node_entry_data )->parent_identifier,
				 index_value->parent_identifier );
			}
		}
		else if( index_node->type == LIBPFF_INDEX_TYPE_OFFSET )
		{
			if( index->io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
			{
				byte_stream_copy_to_uint32_little_endian(
				 ( (pff_index_node_offset_entry_32bit_t *) node_entry_data )->file_offset,
				 index_value->file_offset );
				byte_stream_copy_to_uint16_little_endian(
				 ( (pff_index_node_offset_entry_32bit_t *) node_entry_data )->data_size,
				 index_value->data_size );
				byte_stream_copy_to_uint16_little_endian(
				 ( (pff_index_node_offset_entry_32bit_t *) node_entry_data )->reference_count,
				 index_value->reference_count );
			}
			else if( ( index->io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
			      || ( index->io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
			{
				byte_stream_copy_to_uint64_little_endian(
				 ( (pff_index_node_offset_entry_64bit_t *) node_entry_data )->file_offset,
				 index_value->file_offset );
				byte_stream_copy_to_uint16_little_endian(
				 ( (pff_index_node_offset_entry_64bit_t *) node_entry_data )->data_size,
				 index_value->data_size );
				byte_stream_copy_to_uint16_little_endian(
				 ( (pff_index_node_offset_entry_64bit_t *) node_entry_data )->reference_count,
				 index_value->reference_count );
			}
		}
		if( libfdata_tree_node_set_leaf(
		     index_tree_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set leaf in index tree node.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( index->io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (pff_index_node_branch_entry_32bit_t *) node_entry_data )->file_offset,
			 sub_nodes_offset );
			byte_stream_copy_to_uint32_little_endian(
			 ( (pff_index_node_branch_entry_32bit_t *) node_entry_data )->back_pointer,
			 index_value->back_pointer );
		}
		else if( ( index->io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
		      || ( index->io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
		{
			byte_stream_copy_to_uint64_little_endian(
			 ( (pff_index_node_branch_entry_64bit_t *) node_entry_data )->file_offset,
			 sub_nodes_offset );
			byte_stream_copy_to_uint64_little_endian(
			 ( (pff_index_node_branch_entry_64bit_t *) node_entry_data )->back_pointer,
			 index_value->back_pointer );
		}
		result = libfdata_tree_node_sub_nodes_data_range_is_set(
		          index_tree_node,
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
			     index_tree_node,
			     0,
			     sub_nodes_offset,
			     0,
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

/* Reads the index value
 * Returns 1 if successful or -1 on error
 */
int libpff_index_read_node_data(
     libpff_index_t *index,
     libbfio_handle_t *file_io_handle,
     libfdata_tree_node_t *node,
     libfcache_cache_t *cache,
     int node_data_file_index LIBPFF_ATTRIBUTE_UNUSED,
     off64_t node_data_offset,
     size64_t node_data_size,
     uint32_t node_data_flags LIBPFF_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBPFF_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libpff_index_value_t *index_value = NULL;
	static char *function             = "libpff_index_read_node_data";
	int result                        = 0;

	LIBPFF_UNREFERENCED_PARAMETER( node_data_file_index )
	LIBPFF_UNREFERENCED_PARAMETER( node_data_flags )
	LIBPFF_UNREFERENCED_PARAMETER( read_flags )

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
	if( node_data_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid node data offset value less than zero.",
		 function );

		return( -1 );
	}
	if( node_data_size > (off64_t) UINT8_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid node data size value exceeds maximum.",
		 function );

		return( -1 );
	}
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
	result = libfdata_tree_node_is_root(
	          node,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if node is the root.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		/* The index tree root node is virtual
		 */
		if( index->recovered == 0 )
		{
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
				     index->root_node_offset,
				     0,
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
			index_value->back_pointer = index->root_node_back_pointer;
		}
	}
	else
	{
		/* node_data_size contains the index node entry
		 */
		if( libpff_index_read_node_entry(
		     index,
		     file_io_handle,
		     node_data_offset,
		     (uint8_t) node_data_size,
		     node,
		     index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read index node entry at offset: %" PRIi64 ".",
			 function,
			 node_data_offset );

			goto on_error;
		}
	}
	if( libfdata_tree_node_set_node_value(
	     node,
	     cache,
	     (intptr_t *) index_value,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_index_value_free,
	     LIBFDATA_TREE_NODE_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set index value as node value.",
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
	return( -1 );
}

/* Reads the index sub nodes
 * Returns 1 if successful or -1 on error
 */
int libpff_index_read_sub_nodes(
     libpff_index_t *index,
     libbfio_handle_t *file_io_handle,
     libfdata_tree_node_t *node,
     libfcache_cache_t *cache,
     int sub_nodes_data_file_index LIBPFF_ATTRIBUTE_UNUSED,
     off64_t sub_nodes_data_offset,
     size64_t sub_nodes_data_size LIBPFF_ATTRIBUTE_UNUSED,
     uint32_t sub_nodes_data_flags LIBPFF_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBPFF_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libpff_index_value_t *index_value = NULL;
	static char *function             = "libpff_index_read_sub_nodes";

	LIBPFF_UNREFERENCED_PARAMETER( sub_nodes_data_file_index )
	LIBPFF_UNREFERENCED_PARAMETER( sub_nodes_data_size )
	LIBPFF_UNREFERENCED_PARAMETER( sub_nodes_data_flags )
	LIBPFF_UNREFERENCED_PARAMETER( read_flags )

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
	if( sub_nodes_data_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid sub nodes data offset value less than zero.",
		 function );

		return( -1 );
	}
	if( libfdata_tree_node_get_node_value(
	     node,
	     (intptr_t *) file_io_handle,
	     cache,
	     (intptr_t **) &index_value,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve node value.",
		 function );

		return( -1 );
	}
	if( index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing index value.",
		 function );

		return( -1 );
	}
	if( libpff_index_read_node(
	     index,
	     file_io_handle,
	     sub_nodes_data_offset,
	     node,
	     index_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read index node at offset: %" PRIi64 ".",
		 function,
		 sub_nodes_data_offset );

		return( -1 );
	}
	return( 1 );
}

