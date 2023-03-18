/*
 * Local descriptors functions
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

#include "libpff_definitions.h"
#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_local_descriptors_node.h"
#include "libpff_local_descriptor_value.h"
#include "libpff_local_descriptors.h"
#include "libpff_offsets_index.h"
#include "libpff_unused.h"

#include "pff_local_descriptors_node.h"

/* Creates local descriptors
 * Make sure the value local_descriptors is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptors_initialize(
     libpff_local_descriptors_t **local_descriptors,
     libpff_io_handle_t *io_handle,
     libpff_offsets_index_t *offsets_index,
     uint32_t descriptor_identifier,
     uint64_t root_node_data_identifier,
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
	     &( ( *local_descriptors )->local_descriptors_nodes_cache ),
	     LIBPFF_MAXIMUM_CACHE_ENTRIES_LOCAL_DESCRIPTORS_NODES,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create local descriptors nodes cache.",
		 function );

		goto on_error;
	}
	( *local_descriptors )->io_handle                 = io_handle;
	( *local_descriptors )->offsets_index             = offsets_index;
	( *local_descriptors )->descriptor_identifier     = descriptor_identifier;
	( *local_descriptors )->root_node_data_identifier = root_node_data_identifier;
	( *local_descriptors )->recovered                 = recovered;

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
		     &( ( *local_descriptors )->local_descriptors_nodes_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free local descriptors nodes cache.",
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
	     source_local_descriptors->root_node_data_identifier,
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

/* Retrieves the leaf node from an local descriptors node for the specific identifier
 * Returns 1 if successful, 0 if no leaf node was found or -1 on error
 */
int libpff_local_descriptors_get_leaf_node_from_node_by_identifier(
     libpff_local_descriptors_t *local_descriptors,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint64_t identifier,
     uint64_t data_identifier,
     libpff_local_descriptors_node_t **leaf_node,
     uint16_t *leaf_node_entry_index,
     libcerror_error_t **error )
{
	libpff_index_value_t *offsets_index_value               = NULL;
	libpff_local_descriptors_node_t *local_descriptors_node = NULL;
	uint8_t *node_entry_data                                = NULL;
	static char *function                                   = "libpff_local_descriptors_get_leaf_node_from_node_by_identifier";
	uint64_t entry_identifier                               = 0;
	uint64_t sub_node_identifier                            = 0;
	uint16_t entry_index                                    = 0;
	int result                                              = 0;

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
		 data_identifier,
		 data_identifier );
	}
#endif
/* TODO handle multiple recovered offsets index values */
	result = libpff_offsets_index_get_index_value_by_identifier(
	          local_descriptors->offsets_index,
	          io_handle,
	          file_io_handle,
	          data_identifier,
	          local_descriptors->recovered,
	          0,
	          &offsets_index_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to find offsets index value: 0x%08" PRIx64 " (%" PRIu64 ").",
		 function,
		 data_identifier,
		 data_identifier );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	if( offsets_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing offsets index value: 0x%08" PRIx64 " (%" PRIu64 ").",
		 function,
		 data_identifier,
		 data_identifier );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: local descriptors node: identifier: %" PRIu64 " (%s) at offset: 0x%08" PRIx64 " of size: %" PRIu32 "\n",
		 function,
		 offsets_index_value->identifier,
		 ( ( offsets_index_value->identifier & LIBPFF_OFFSET_INDEX_IDENTIFIER_FLAG_INTERNAL ) ? "internal" : "external" ),
		 offsets_index_value->file_offset,
		 offsets_index_value->data_size );
	}
#endif
	if( libpff_local_descriptors_node_initialize(
	     &local_descriptors_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create local descriptors node.",
		 function );

		goto on_error;
	}
	if( libpff_local_descriptors_node_read_file_io_handle(
	     local_descriptors_node,
	     io_handle,
	     file_io_handle,
	     local_descriptors->descriptor_identifier,
	     data_identifier,
	     offsets_index_value->file_offset,
	     offsets_index_value->data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read local descriptors node at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 offsets_index_value->file_offset,
		 offsets_index_value->file_offset );

		goto on_error;
	}
	for( entry_index = 0;
	     entry_index < local_descriptors_node->number_of_entries;
	     entry_index++ )
	{
		if( libpff_local_descriptors_node_get_entry_data(
		     local_descriptors_node,
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
		if( local_descriptors->io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (pff_local_descriptor_branch_node_entry_type_32bit_t *) node_entry_data )->identifier,
			 entry_identifier );
		}
		else if( ( local_descriptors->io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
		      || ( local_descriptors->io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
		{
			byte_stream_copy_to_uint64_little_endian(
			 ( (pff_local_descriptor_branch_node_entry_type_64bit_t *) node_entry_data )->identifier,
			 entry_identifier );
		}
		/* Ignore the upper 32-bit of local descriptor identifiers
		 */
		entry_identifier &= 0xffffffffUL;

		if( local_descriptors_node->level != LIBPFF_LOCAL_DESCRIPTOR_NODE_LEVEL_LEAF )
		{
			if( ( entry_index == 0 )
			 || ( identifier >= entry_identifier ) )
			{
				if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
				{
					byte_stream_copy_to_uint32_little_endian(
					 ( (pff_local_descriptor_branch_node_entry_type_32bit_t *) node_entry_data )->sub_node_identifier,
					 sub_node_identifier );
				}
				else if( ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
				      || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
				{
					byte_stream_copy_to_uint64_little_endian(
					 ( (pff_local_descriptor_branch_node_entry_type_64bit_t *) node_entry_data )->sub_node_identifier,
					 sub_node_identifier );
				}
			}
		}
		else if( identifier == entry_identifier )
		{
			*leaf_node             = local_descriptors_node;
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
	if( local_descriptors_node->level != LIBPFF_INDEX_NODE_LEVEL_LEAF )
	{
		result = libpff_local_descriptors_get_leaf_node_from_node_by_identifier(
			  local_descriptors,
		          io_handle,
			  file_io_handle,
			  identifier,
			  sub_node_identifier,
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
			 offsets_index_value->file_offset,
			 offsets_index_value->file_offset );

			goto on_error;
		}
	}
	if( ( local_descriptors_node != NULL )
	 && ( local_descriptors_node != *leaf_node ) )
	{
		if( libpff_local_descriptors_node_free(
		     &local_descriptors_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free local descriptors node.",
			 function );

			goto on_error;
		}
	}
	if( libpff_index_value_free(
	     &offsets_index_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free offsets index value.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( local_descriptors_node != NULL )
	{
		libpff_local_descriptors_node_free(
		 &local_descriptors_node,
		 NULL );
	}
	if( offsets_index_value != NULL )
	{
		libpff_index_value_free(
		 &offsets_index_value,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the value for the specific identifier
 * Returns 1 if successful, 0 if no value was found or -1 on error
 */
int libpff_local_descriptors_get_value_by_identifier(
     libpff_local_descriptors_t *local_descriptors,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint64_t identifier,
     libpff_local_descriptor_value_t **local_descriptor_value,
     libcerror_error_t **error )
{
	libpff_local_descriptor_value_t *safe_local_descriptor_value = NULL;
	libpff_local_descriptors_node_t *leaf_node                   = NULL;
	uint8_t *node_entry_data                                     = NULL;
	static char *function                                        = "libpff_local_descriptors_get_value_by_identifier";
	uint16_t leaf_node_entry_index                               = 0;
	int result                                                   = 0;

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
	result = libpff_local_descriptors_get_leaf_node_from_node_by_identifier(
		  local_descriptors,
		  io_handle,
		  file_io_handle,
		  identifier,
		  local_descriptors->root_node_data_identifier,
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
		if( libpff_local_descriptors_node_get_entry_data(
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
		if( libpff_local_descriptor_value_initialize(
		     &safe_local_descriptor_value,
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
		if( libpff_local_descriptor_value_read_data(
		     safe_local_descriptor_value,
		     io_handle,
		     node_entry_data,
		     (size_t) leaf_node->entry_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read local descriptor value.",
			 function );

			goto on_error;
		}
		if( libpff_local_descriptors_node_free(
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
	*local_descriptor_value = safe_local_descriptor_value;

	return( result );

on_error:
	if( safe_local_descriptor_value != NULL )
	{
		libpff_local_descriptor_value_free(
		 &safe_local_descriptor_value,
		 NULL );
	}
	if( leaf_node != NULL )
	{
		libpff_local_descriptors_node_free(
		 &leaf_node,
		 NULL );
	}
	return( -1 );
}

