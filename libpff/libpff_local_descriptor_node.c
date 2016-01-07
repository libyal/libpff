/*
 * Local descriptor node functions
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

#include "libpff_data_block.h"
#include "libpff_definitions.h"
#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfmapi.h"
#include "libpff_local_descriptor_node.h"
#include "libpff_unused.h"

#include "pff_local_descriptor_node.h"

/* Creates a local descriptor node
 * Make sure the value local_descriptor_node is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptor_node_initialize(
     libpff_local_descriptor_node_t **local_descriptor_node,
     libcerror_error_t **error )
{
	static char *function = "libpff_local_descriptor_node_initialize";

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
	if( *local_descriptor_node != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid local descriptor node value already set.",
		 function );

		return( -1 );
	}
	*local_descriptor_node = memory_allocate_structure(
	                          libpff_local_descriptor_node_t );

	if( *local_descriptor_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create local descriptor node.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *local_descriptor_node,
	     0,
	     sizeof( libpff_local_descriptor_node_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear local descriptor node.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *local_descriptor_node != NULL )
	{
		memory_free(
		 *local_descriptor_node );

		*local_descriptor_node = NULL;
	}
	return( -1 );
}

/* Frees a local descriptor node
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptor_node_free(
     libpff_local_descriptor_node_t **local_descriptor_node,
     libcerror_error_t **error )
{
	static char *function = "libpff_local_descriptor_node_free";

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
	if( *local_descriptor_node != NULL )
	{
		if( ( *local_descriptor_node )->entries_data != NULL )
		{
			memory_free(
			 ( *local_descriptor_node )->entries_data );
		}
		memory_free(
		 *local_descriptor_node );

		*local_descriptor_node = NULL;
	}
	return( 1 );
}

/* Retrieves the data of a specific entry
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptor_node_get_entry_data(
     libpff_local_descriptor_node_t *local_descriptor_node,
     uint16_t entry_index,
     uint8_t **entry_data,
     libcerror_error_t **error )
{
	static char *function = "libpff_local_descriptor_node_get_entry_data";
	size_t entry_offset   = 0;

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
	if( local_descriptor_node->entries_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid local descriptor node - missing entries data.",
		 function );

		return( -1 );
	}
	if( entry_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry data.",
		 function );

		return( -1 );
	}
	entry_offset = (size_t) ( local_descriptor_node->entry_size * entry_index );

	if( entry_offset > local_descriptor_node->entries_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: entry index value exceeds maximum.",
		 function );

		return( -1 );
	}
	*entry_data = &( local_descriptor_node->entries_data[ entry_offset ] );

	return( 1 );
}

/* Retrieves the identifier of a specific entry
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptor_node_get_entry_identifier(
     libpff_local_descriptor_node_t *local_descriptor_node,
     libpff_io_handle_t *io_handle,
     uint16_t entry_index,
     uint64_t *entry_identifier,
     libcerror_error_t **error )
{
	uint8_t *node_entry_data = NULL;
	static char *function    = "libpff_local_descriptor_node_get_entry_identifier";
	size_t entry_offset      = 0;

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
	if( local_descriptor_node->entries_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid local descriptor node - missing entries data.",
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
	if( ( io_handle->file_type != LIBPFF_FILE_TYPE_32BIT )
	 && ( io_handle->file_type != LIBPFF_FILE_TYPE_64BIT )
	 && ( io_handle->file_type != LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid IO handle - unsupported file type.",
		 function );

		return( -1 );
	}
	if( entry_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry identifier.",
		 function );

		return( -1 );
	}
	entry_offset = (size_t) ( local_descriptor_node->entry_size * entry_index );

	if( entry_offset > local_descriptor_node->entries_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: entry index value exceeds maximum.",
		 function );

		return( -1 );
	}
	node_entry_data = &( local_descriptor_node->entries_data[ entry_offset ] );

	if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
	{
		byte_stream_copy_to_uint32_little_endian(
		 node_entry_data,
		 *entry_identifier );
	}
	else if( ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
	      || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
	{
		byte_stream_copy_to_uint64_little_endian(
		 node_entry_data,
		 *entry_identifier );
	}
	return( 1 );
}

/* Retrieves the sub node identifier of a specific entry
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptor_node_get_entry_sub_node_identifier(
     libpff_local_descriptor_node_t *local_descriptor_node,
     libpff_io_handle_t *io_handle,
     uint16_t entry_index,
     uint64_t *entry_sub_node_identifier,
     libcerror_error_t **error )
{
	uint8_t *node_entry_data = NULL;
	static char *function    = "libpff_local_descriptor_node_get_entry_sub_node_identifier";
	size_t entry_offset      = 0;

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
	if( local_descriptor_node->entries_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid local descriptor node - missing entries data.",
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
	if( ( io_handle->file_type != LIBPFF_FILE_TYPE_32BIT )
	 && ( io_handle->file_type != LIBPFF_FILE_TYPE_64BIT )
	 && ( io_handle->file_type != LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid IO handle - unsupported file type.",
		 function );

		return( -1 );
	}
	if( entry_sub_node_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry sub node identifier.",
		 function );

		return( -1 );
	}
	entry_offset = (size_t) ( local_descriptor_node->entry_size * entry_index );

	if( entry_offset > local_descriptor_node->entries_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: entry index value exceeds maximum.",
		 function );

		return( -1 );
	}
	node_entry_data = &( local_descriptor_node->entries_data[ entry_offset ] );

	if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_local_descriptor_branch_node_entry_type_32bit_t *) node_entry_data )->sub_node_identifier,
		 *entry_sub_node_identifier );
	}
	else if( ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
	      || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (pff_local_descriptor_branch_node_entry_type_64bit_t *) node_entry_data )->sub_node_identifier,
		 *entry_sub_node_identifier );
	}
	return( 1 );
}

/* Reads an local descriptor node
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptor_node_read(
     libpff_local_descriptor_node_t *local_descriptor_node,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t descriptor_identifier,
     uint64_t data_identifier,
     off64_t node_offset,
     size32_t node_size,
     libcerror_error_t **error )
{
	libpff_data_block_t *data_block           = NULL;
	static char *function                     = "libpff_local_descriptor_node_read";
	size_t local_descriptor_node_header_size  = 0;
	uint8_t node_signature                    = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t *local_descriptor_node_entry_data = NULL;
	uint64_t value_64bit                      = 0;
	uint16_t entry_index                      = 0;
#endif

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
	if( local_descriptor_node->entries_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid local descriptor node - entries data value already set.",
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
	if( ( io_handle->file_type != LIBPFF_FILE_TYPE_32BIT )
	 && ( io_handle->file_type != LIBPFF_FILE_TYPE_64BIT )
	 && ( io_handle->file_type != LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid IO handle - unsupported file type.",
		 function );

		return( -1 );
	}
	if( node_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid node offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( node_size > (size32_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: node size value exceeds maximum.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading local descriptor node data at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 node_offset,
		 node_offset );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     node_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek local descriptor node at offset: 0x%08" PRIx64 ".",
		 function,
		 node_offset );

		goto on_error;
	}
	if( libpff_data_block_initialize(
	     &data_block,
	     io_handle,
	     descriptor_identifier,
	     data_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data block.",
		 function );

		goto on_error;
	}
	if( libpff_data_block_read(
	     data_block,
	     file_io_handle,
	     node_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read data block at offset: %" PRIi64 ".",
		 function,
		 node_offset );

		goto on_error;
	}
	if( data_block->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data block - missing data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: local descriptor node data:\n",
		 function );
		libcnotify_print_data(
		 data_block->data,
		 data_block->uncompressed_data_size,
		 0 );
	}
#endif
	/* Parse the local descriptors node data
	 */
	if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
	{
		node_signature               = ( (pff_local_descriptor_node_32bit_t *) data_block->data )->signature;
		local_descriptor_node->level = ( (pff_local_descriptor_node_32bit_t *) data_block->data )->level;

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_local_descriptor_node_32bit_t *) data_block->data )->number_of_entries,
		 local_descriptor_node->number_of_entries );
	}
	else if( ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
	      || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
	{
		node_signature               = ( (pff_local_descriptor_node_64bit_t *) data_block->data )->signature;
		local_descriptor_node->level = ( (pff_local_descriptor_node_64bit_t *) data_block->data )->level;

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_local_descriptor_node_64bit_t *) data_block->data )->number_of_entries,
		 local_descriptor_node->number_of_entries );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: local descriptors node signature\t\t: 0x%02" PRIx8 "\n",
		 function,
		 node_signature );
		libcnotify_printf(
		 "%s: local descriptors node level\t\t\t: %" PRIu8 "\n",
		 function,
		 local_descriptor_node->level );
		libcnotify_printf(
		 "%s: number of entries\t\t\t\t: %" PRIu16 "\n",
		 function,
		 local_descriptor_node->number_of_entries );

		if( ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
		 || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
		{
			libcnotify_printf(
			 "%s: padding1:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_local_descriptor_node_64bit_t *) data_block->data )->padding1,
			 4,
			 0 );
		}
	}
#endif
	if( node_signature != 0x02 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: unsupported local descriptors node signature: 0x%02" PRIx8 ".",
		 function,
		 node_signature );

		goto on_error;
	}
	if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
	{
		local_descriptor_node_header_size = sizeof( pff_local_descriptor_node_32bit_t );

		if( local_descriptor_node->level == LIBPFF_LOCAL_DESCRIPTOR_NODE_LEVEL_LEAF )
		{
			local_descriptor_node->entry_size = sizeof( pff_local_descriptor_leaf_node_entry_type_32bit_t );
		}
		else
		{
			local_descriptor_node->entry_size = sizeof( pff_local_descriptor_branch_node_entry_type_32bit_t );
		}
	}
	else if( ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
	      || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
	{
		local_descriptor_node_header_size = sizeof( pff_local_descriptor_node_64bit_t );

		if( local_descriptor_node->level == LIBPFF_LOCAL_DESCRIPTOR_NODE_LEVEL_LEAF )
		{
			local_descriptor_node->entry_size = sizeof( pff_local_descriptor_leaf_node_entry_type_64bit_t );
		}
		else
		{
			local_descriptor_node->entry_size = sizeof( pff_local_descriptor_branch_node_entry_type_64bit_t );
		}
	}
/* TODO additional sanity checks */

	if( local_descriptor_node->number_of_entries == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of entries.",
		 function );

/* TODO implement error tollerance */
		goto on_error;
	}
	local_descriptor_node->entries_data_size = local_descriptor_node->number_of_entries * local_descriptor_node->entry_size;

	if( local_descriptor_node->entries_data_size != ( data_block->uncompressed_data_size - local_descriptor_node_header_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: mismatch in total size of the entries data and the size of the entries.",
		 function );

/* TODO implement error tollerance */
		goto on_error;
	}
	if( local_descriptor_node->entries_data_size > (size32_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: entries data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* Copy the entries data to the local descriptor node
	 * to prevent loosing it when the data block is cached out.
	 */
	local_descriptor_node->entries_data = (uint8_t *) memory_allocate(
	                                                   sizeof( uint8_t ) * local_descriptor_node->entries_data_size );

	if( local_descriptor_node->entries_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create entries data.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     local_descriptor_node->entries_data,
	     &( data_block->data[ local_descriptor_node_header_size ] ),
	     local_descriptor_node->entries_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy data array entry.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		local_descriptor_node_entry_data = local_descriptor_node->entries_data;

		for( entry_index = 0;
		     entry_index < local_descriptor_node->number_of_entries;
		     entry_index++ )
		{
			if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
			{
				byte_stream_copy_to_uint32_little_endian(
				 local_descriptor_node_entry_data,
				 value_64bit );

				local_descriptor_node_entry_data += 4;
			}
			else if( ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
			      || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
			{
				byte_stream_copy_to_uint64_little_endian(
				 local_descriptor_node_entry_data,
				 value_64bit );

				local_descriptor_node_entry_data += 8;
			}
			libcnotify_printf(
			 "%s: entry: %03d identifier\t\t\t: 0x%08" PRIx64 " (%" PRIu64 ")\n",
			 function,
			 entry_index,
			 value_64bit,
			 value_64bit );

			if( local_descriptor_node->level == LIBPFF_LOCAL_DESCRIPTOR_NODE_LEVEL_LEAF )
			{
				if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
				{
					byte_stream_copy_to_uint32_little_endian(
					 local_descriptor_node_entry_data,
					 value_64bit );

					local_descriptor_node_entry_data += 4;
				}
				else if( ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
				      || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
				{
					byte_stream_copy_to_uint64_little_endian(
					 local_descriptor_node_entry_data,
					 value_64bit );

					local_descriptor_node_entry_data += 8;
				}
				libcnotify_printf(
				 "%s: entry: %03d data identifier\t\t\t: 0x%08" PRIx64 " (%" PRIu64 ")\n",
				 function,
				 entry_index,
				 value_64bit,
				 value_64bit );
			}
			if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
			{
				byte_stream_copy_to_uint32_little_endian(
				 local_descriptor_node_entry_data,
				 value_64bit );

				local_descriptor_node_entry_data += 4;
			}
			else if( ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
			      || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
			{
				byte_stream_copy_to_uint64_little_endian(
				 local_descriptor_node_entry_data,
				 value_64bit );

				local_descriptor_node_entry_data += 8;
			}
			if( local_descriptor_node->level == LIBPFF_LOCAL_DESCRIPTOR_NODE_LEVEL_LEAF )
			{
				libcnotify_printf(
				 "%s: entry: %03d local descriptors identifier\t: 0x%08" PRIx64 " (%" PRIu64 ")\n",
				 function,
				 entry_index,
				 value_64bit,
				 value_64bit );
			}
			else
			{
				libcnotify_printf(
				 "%s: entry: %03d sub node identifier\t\t: 0x%08" PRIx64 " (%" PRIu64 ")\n",
				 function,
				 entry_index,
				 value_64bit,
				 value_64bit );
			}
		}
		libcnotify_printf(
		 "\n" );
	}
#endif
	if( libpff_data_block_free(
	     &data_block,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free data block.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( local_descriptor_node->entries_data != NULL )
	{
		memory_free(
		 local_descriptor_node->entries_data );

		local_descriptor_node->entries_data = NULL;
	}
	local_descriptor_node->entries_data_size = 0;

	if( data_block != NULL )
	{
		libpff_data_block_free(
		 &data_block,
		 NULL );
	}
	return( -1 );
}

/* Reads a local descriptor node
 * Callback for the local descriptor nodes list
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptor_node_read_element_data(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_list_element_t *list_element,
     libfcache_cache_t *cache,
     int data_range_file_index LIBPFF_ATTRIBUTE_UNUSED,
     off64_t data_range_offset,
     size64_t data_range_size,
     uint32_t data_range_flags LIBPFF_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBPFF_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libpff_local_descriptor_node_t *local_descriptor_node = NULL;
	static char *function                                 = "libpff_local_descriptor_node_read_element_data";

	LIBPFF_UNREFERENCED_PARAMETER( data_range_file_index )
	LIBPFF_UNREFERENCED_PARAMETER( data_range_flags )
	LIBPFF_UNREFERENCED_PARAMETER( read_flags )

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
	if( data_range_size > (size64_t) UINT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: data range size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( libpff_local_descriptor_node_initialize(
	     &local_descriptor_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create local descriptor node.",
		 function );

		goto on_error;
	}
	if( libpff_local_descriptor_node_read(
	     local_descriptor_node,
	     io_handle,
	     file_io_handle,
	     0,
	     0,
	     data_range_offset,
	     (size32_t) data_range_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read local descriptor node at offset: 0x%08" PRIx64 ".",
		 function,
		 data_range_offset );

		goto on_error;
	}
	if( libfdata_list_element_set_element_value(
	     list_element,
	     (intptr_t *) file_io_handle,
	     cache,
	     (intptr_t *) local_descriptor_node,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_local_descriptor_node_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set local descriptor node as element value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( local_descriptor_node != NULL )
	{
		libpff_local_descriptor_node_free(
		 &local_descriptor_node,
		 NULL );
	}
	return( -1 );
}

