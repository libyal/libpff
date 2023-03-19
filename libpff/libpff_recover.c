/*
 * Recover functions
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

#include "libpff_checksum.h"
#include "libpff_data_block.h"
#include "libpff_definitions.h"
#include "libpff_descriptors_index.h"
#include "libpff_index.h"
#include "libpff_index_node.h"
#include "libpff_index_value.h"
#include "libpff_index_values_list.h"
#include "libpff_item_descriptor.h"
#include "libpff_item_tree.h"
#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcdata.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_local_descriptors_node.h"
#include "libpff_offsets_index.h"
#include "libpff_recover.h"

#include "pff_block.h"
#include "pff_index_node.h"

/* Scans for recoverable items
 * By default only the unallocated space is checked for recoverable items
 * Returns 1 if successful or -1 on error
 */
int libpff_recover_items(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_descriptors_index_t *descriptors_index,
     libpff_offsets_index_t *offsets_index,
     libcdata_range_list_t *unallocated_data_block_list,
     libcdata_range_list_t *unallocated_page_block_list,
     libcdata_list_t *recovered_item_list,
     uint8_t recovery_flags,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *item_tree_node          = NULL;
	libpff_index_value_t *descriptors_index_value = NULL;
	libpff_index_values_list_t *index_values_list = NULL;
	libpff_item_descriptor_t *item_descriptor     = NULL;
	static char *function                         = "libpff_recover_items";
	int data_identifier_value_index               = 0;
	int index_value_iterator                      = 0;
	int index_values_list_iterator                = 0;
	int local_descriptors_identifier_value_index  = 0;
	int number_of_index_values                    = 0;
	int number_of_index_values_lists              = 0;
	int result                                    = 0;

#ifdef TODO
        uint32_t maximum_data_block_data_size         = 0;
#endif

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
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file type.",
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
	if( offsets_index->index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid offsets index - missing index.",
		 function );

		return( -1 );
	}
	if( recovered_item_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid recovered item list.",
		 function );

		return( -1 );
	}
#ifdef TODO
	if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
	{
		maximum_data_block_data_size = 8192 - 12;
	}
	else if( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
	{
		maximum_data_block_data_size = 8192 - 16;
	}
	else
	{
/* TODO: this value is currently assumed based on the 512 x 8 = 4k page */
		maximum_data_block_data_size = 65536 - 24;
	}
#endif /* TODO */
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     0,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file IO handle.",
                 function );

		goto on_error;
	}
#endif
#ifdef TODO
	/* Scan the offsets index nodes for deleted values
	 */
	if( libpff_recover_analyze_offsets_index_node(
	     offsets_index,
	     io_handle,
	     file_io_handle,
	     offsets_index->index->root_node_offset,
	     offsets_index->index->root_node_back_pointer,
	     maximum_data_block_data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to recover offsets index nodes.",
		 function );

		return( -1 );
	}
#endif /* TODO */

	if( libpff_recover_data_blocks(
	     io_handle,
	     file_io_handle,
	     descriptors_index,
	     offsets_index,
	     unallocated_data_block_list,
	     unallocated_page_block_list,
	     recovery_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to recover data blocks.",
		 function );

		goto on_error;
	}
	/* Scan the descriptors index nodes for deleted values
	 */
	if( libpff_recover_descriptors_index_values(
	     descriptors_index,
	     io_handle,
	     file_io_handle,
	     descriptors_index->index->root_node_offset,
	     descriptors_index->index->root_node_back_pointer,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to recover descriptors index values.",
		 function );

		return( -1 );
	}
/* TODO combine the offsets index value check part with libpff_recover_descriptors_index_values so that the size of descriptors_index->recovered_index_values_tree remains small */

	/* For each recovered descriptors index value check if the corresonding local descriptor and data offsets index values exist
	 */
	if( libcdata_btree_get_number_of_values(
	     descriptors_index->recovered_index_values_tree,
	     &number_of_index_values_lists,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of recovered descriptors index values.",
		 function );

		goto on_error;
	}
	for( index_values_list_iterator = 0;
	     index_values_list_iterator < number_of_index_values_lists;
	     index_values_list_iterator++ )
	{
		if( io_handle->abort != 0 )
		{
/* TODO break instead of error ? */
			goto on_error;
		}
		if( libcdata_btree_get_value_by_index(
		     descriptors_index->recovered_index_values_tree,
		     index_values_list_iterator,
		     (intptr_t **) &index_values_list,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: unable to retrieve recovered descriptors index values list: %d.",
			 function,
			 index_values_list_iterator );

			goto on_error;
		}
		if( libpff_index_values_list_number_of_values(
		     index_values_list,
		     &number_of_index_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of recovered descriptors index values list: %d elements.",
			 function,
			 index_values_list_iterator );

			goto on_error;
		}
		for( index_value_iterator = 0;
		     index_value_iterator < number_of_index_values;
		     index_value_iterator++ )
		{
			if( io_handle->abort != 0 )
			{
/* TODO break instead of error ? */
				goto on_error;
			}
			if( libpff_index_values_list_get_value_by_index(
			     index_values_list,
			     index_value_iterator,
			     &descriptors_index_value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve recovered descriptors index value: %" PRIu64 " list element: %d.",
				 function,
				 index_values_list->identifier,
				 index_value_iterator );

				goto on_error;
			}
			if( descriptors_index_value == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing recovered descriptors index value: %" PRIu64 " list element: %d.",
				 function,
				 index_values_list->identifier,
				 index_value_iterator );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: analyzing identifier: %" PRIu64 ", data: %" PRIu64 ", local descriptors: %" PRIu64 ", parent: %" PRIu32 "\n",
				 function,
				 descriptors_index_value->identifier,
				 descriptors_index_value->data_identifier,
				 descriptors_index_value->local_descriptors_identifier,
				 descriptors_index_value->parent_identifier );
			}
#endif
			result = libpff_recover_analyze_descriptor_data_identifier(
				  io_handle,
				  file_io_handle,
				  offsets_index,
				  descriptors_index_value,
				  &data_identifier_value_index,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to recover descriptors index value: %" PRIu64 " data identifier: %" PRIu64 ".",
				 function,
				 descriptors_index_value->identifier,
				 descriptors_index_value->data_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_MASK );

				goto on_error;
			}
			else if( result == 0 )
			{
				continue;
			}
			result = libpff_recover_analyze_descriptor_local_descriptors_identifier(
				  io_handle,
				  file_io_handle,
				  offsets_index,
				  descriptors_index_value,
				  &local_descriptors_identifier_value_index,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to recover descriptors index value: %" PRIu64 " local descriptors identifier: %" PRIu64 ".",
				 function,
				 descriptors_index_value->identifier,
				 descriptors_index_value->local_descriptors_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_MASK );

				goto on_error;
			}
			else if( result == 0 )
			{
				continue;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: item descriptor: %" PRIu64 " is recoverable.\n",
				 function,
				 descriptors_index_value->identifier );
			}
#endif
			/* Create a new item descriptor
			 */
			if( libpff_item_descriptor_initialize(
			     &item_descriptor,
			     (uint32_t) descriptors_index_value->identifier,
			     descriptors_index_value->data_identifier,
			     descriptors_index_value->local_descriptors_identifier,
			     1,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create item descriptor: %" PRIu64 ".",
				 function,
				 descriptors_index_value->identifier );

				goto on_error;
			}
			if( item_descriptor == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing item descriptor: %" PRIu64 ".",
				 function,
				 descriptors_index_value->identifier );

				goto on_error;
			}
/* TODO add to initialize function or create a separate function for setting these value in the item descriptor */
			item_descriptor->recovered_data_identifier_value_index              = data_identifier_value_index;
			item_descriptor->recovered_local_descriptors_identifier_value_index = local_descriptors_identifier_value_index;

			/* Create a new item tree node
			 */
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
				 "%s: unable to set item descriptor in item tree node.",
				 function );

				goto on_error;
			}
			item_descriptor = NULL;

			if( libcdata_list_append_value(
			     recovered_item_list,
			     (intptr_t *) item_tree_node,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append tree node to recovered item list.",
				 function );

				goto on_error;
			}
			item_tree_node = NULL;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     0,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file IO handle.",
                 function );

                goto on_error;
	}
#endif
	/* TODO
	 * link recovered descriptors to parent? and add to item hierarchy?
	 * handle scanning without index data
	 * what about 'encryption' ?
	 */
	return( 1 );

on_error:
	if( item_tree_node != NULL )
	{
		libcdata_tree_node_free(
		 &item_tree_node,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_descriptor_free,
		 NULL );
	}
	if( item_descriptor != NULL )
	{
		libpff_item_descriptor_free(
		 &item_descriptor,
		 NULL );
	}
	libcdata_list_empty(
	 recovered_item_list,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_tree_node_free_recovered,
	 NULL );

	return( -1 );
}

/* Analyze if a specific descriptor data identifier is recoverable
 * Returns 1 if recoverable, 0 if not or -1 on error
 */
int libpff_recover_analyze_descriptor_data_identifier(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     libpff_index_value_t *descriptors_index_value,
     int *data_identifier_value_index,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *upper_node              = NULL;
	libpff_index_value_t *index_value             = NULL;
	libpff_index_value_t *lookup_index_value      = NULL;
	libpff_index_values_list_t *index_values_list = NULL;
	static char *function                         = "libpff_recover_analyze_descriptor_data_identifier";
	uint64_t lookup_identifier                    = 0;
	int index_value_iterator                      = 0;
	int number_of_index_values                    = 0;
	int result                                    = 0;

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
	if( descriptors_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptors index value.",
		 function );

		return( -1 );
	}
	if( data_identifier_value_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data identifier value index.",
		 function );

		return( -1 );
	}
	lookup_identifier = descriptors_index_value->data_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_MASK;

	if( libpff_index_value_initialize(
	     &lookup_index_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create lookup index value.",
		 function );

		goto on_error;
	}
	lookup_index_value->identifier = lookup_identifier;

/* TODO have a compare function that directly uses the lookup_identifier not the lookup_index_value */
	result = libcdata_btree_get_value_by_value(
	          offsets_index->recovered_index_values_tree,
	          (intptr_t *) lookup_index_value,
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &libpff_index_values_list_compare,
	          &upper_node, 
	          (intptr_t **) &index_values_list,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve recovered offsets index value: %" PRIu64 " list.",
		 function,
		 lookup_identifier );

		goto on_error;
	}
	if( libpff_index_value_free(
	     &lookup_index_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free lookup index value.",
		 function,
		 lookup_identifier );

		goto on_error;
	}
	if( result == 0 )
	{
		return( 0 );
	}
	if( libpff_index_values_list_number_of_values(
	     index_values_list,
	     &number_of_index_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of recovered offsets index value: %" PRIu64 " list.",
		 function,
		 lookup_identifier );

		goto on_error;
	}
/* TODO handle if more than 1 offsets index value is recoverable */

	result = 0;

	for( index_value_iterator = 0;
	     index_value_iterator < number_of_index_values;
	     index_value_iterator++ )
	{
		if( libpff_index_values_list_get_value_by_index(
		     index_values_list,
		     index_value_iterator,
		     &index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve recovered offsets index value: %d for data identifier: %" PRIu64 ".",
			 function,
			 index_value_iterator,
			 lookup_identifier );

			goto on_error;
		}
		/* Check if a data block is recoverable
		 */
		result = libpff_recover_analyze_data_block(
			  io_handle,
			  file_io_handle,
			  (uint32_t) descriptors_index_value->identifier,
			  index_value,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to recover data block: %" PRIu64 ".",
			 function,
			 descriptors_index_value->data_identifier );

#if defined( HAVE_DEBUG_OUTPUT )
			if( ( libcnotify_verbose != 0 )
			 && ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
#endif
			libcerror_error_free(
			 error );

/* TODO remove unreadable offset identifier from offsets_index->recovered_index_values_tree ? */
		}
		else if( result != 0 )
		{
			break;
		}
	}
	if( result != 0 )
	{
		*data_identifier_value_index = index_value_iterator;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	else if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: recovered offsets index value for data identifier: %" PRIu64 " not available.\n",
		 function,
		 lookup_identifier );
	}
#endif
	return( result );

on_error:
	if( lookup_index_value != NULL )
	{
		libpff_index_value_free(
		 &lookup_index_value,
		 NULL );
	}
	return( -1 );
}

/* Analyze if a specific descriptor local descriptors identifier is recoverable
 * Returns 1 if recoverable, 0 if not or -1 on error
 */
int libpff_recover_analyze_descriptor_local_descriptors_identifier(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     libpff_index_value_t *descriptors_index_value,
     int *local_descriptors_identifier_value_index,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *upper_node              = NULL;
	libpff_index_value_t *index_value             = NULL;
	libpff_index_value_t *lookup_index_value      = NULL;
	libpff_index_values_list_t *index_values_list = NULL;
	static char *function                         = "libpff_recover_analyze_descriptor_local_descriptors_identifier";
	uint64_t lookup_identifier                    = 0;
	int index_value_iterator                      = 0;
	int number_of_index_values                    = 0;
	int result                                    = 0;

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
	if( descriptors_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptors index value.",
		 function );

		return( -1 );
	}
	if( local_descriptors_identifier_value_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local descriptors identifier value index.",
		 function );

		return( -1 );
	}
	/* The local descriptors identifier is 0 if not set
	 */
	if( descriptors_index_value->local_descriptors_identifier == 0 )
	{
		return( 1 );
	}
	lookup_identifier = descriptors_index_value->local_descriptors_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_MASK;

	if( libpff_index_value_initialize(
	     &lookup_index_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create lookup index value.",
		 function );

		goto on_error;
	}
	lookup_index_value->identifier = lookup_identifier;

/* TODO have a compare function that directly uses the lookup_identifier not the lookup_index_value */
	result = libcdata_btree_get_value_by_value(
	          offsets_index->recovered_index_values_tree,
	          (intptr_t *) lookup_index_value,
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &libpff_index_values_list_compare,
	          &upper_node, 
	          (intptr_t **) &index_values_list,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve recovered offsets index value: %" PRIu64 " list.",
		 function,
		 lookup_identifier );

		goto on_error;
	}
	if( libpff_index_value_free(
	     &lookup_index_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free lookup index value.",
		 function,
		 lookup_identifier );

		goto on_error;
	}
	if( result == 0 )
	{
		return( 0 );
	}
	if( libpff_index_values_list_number_of_values(
	     index_values_list,
	     &number_of_index_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of recovered offsets index value: %" PRIu64 " list.",
		 function,
		 lookup_identifier );

		goto on_error;
	}
/* TODO handle if more than 1 offsets index value is recoverable */

	result = 0;

	for( index_value_iterator = 0;
	     index_value_iterator < number_of_index_values;
	     index_value_iterator++ )
	{
		if( libpff_index_values_list_get_value_by_index(
		     index_values_list,
		     index_value_iterator,
		     &index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve recovered offsets index value: %d for data identifier: %" PRIu64 ".",
			 function,
			 index_value_iterator,
			 lookup_identifier );

			goto on_error;
		}
		/* Check if local descriptors are recoverable
		 */
		result = libpff_recover_analyze_local_descriptors(
		          io_handle,
		          file_io_handle,
		          offsets_index,
		          descriptors_index_value->local_descriptors_identifier,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to recover local descriptors: %" PRIu64 ".",
			 function,
			 descriptors_index_value->local_descriptors_identifier );

			goto on_error;
		}
		else if( result != 0 )
		{
			break;
		}
	}
	if( result != 0 )
	{
		*local_descriptors_identifier_value_index = index_value_iterator;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	else if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: recovered offsets index value for local descriptors identifier: %" PRIu64 " not available.\n",
		 function,
		 lookup_identifier );
	}
#endif
	return( result );

on_error:
	if( lookup_index_value != NULL )
	{
		libpff_index_value_free(
		 &lookup_index_value,
		 NULL );
	}
	return( -1 );
}

/* Analyze if a specific data block back pointer is recoverable
 * Returns 1 if recoverable, 0 if not or -1 on error
 */
int libpff_recover_analyze_data_block_back_pointer(
     libpff_offsets_index_t *offsets_index,
     uint64_t data_block_back_pointer,
     off64_t data_block_data_offset,
     size32_t data_block_data_size,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *upper_node              = NULL;
	libpff_index_value_t *index_value             = NULL;
	libpff_index_value_t *lookup_index_value      = NULL;
	libpff_index_values_list_t *index_values_list = NULL;
	static char *function                         = "libpff_recover_analyze_data_block_back_pointer";
	uint64_t lookup_identifier                    = 0;
	int index_value_iterator                      = 0;
	int number_of_index_values                    = 0;
	int result                                    = 0;

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
	lookup_identifier = data_block_back_pointer;

	if( libpff_index_value_initialize(
	     &lookup_index_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create lookup index value.",
		 function );

		goto on_error;
	}
	lookup_index_value->identifier = lookup_identifier;

/* TODO have a compare function that directly uses the lookup_identifier not the lookup_index_value */
	result = libcdata_btree_get_value_by_value(
	          offsets_index->recovered_index_values_tree,
	          (intptr_t *) lookup_index_value,
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &libpff_index_values_list_compare,
	          &upper_node, 
	          (intptr_t **) &index_values_list,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve recovered offsets index value: %" PRIu64 " list.",
		 function,
		 lookup_identifier );

		goto on_error;
	}
	if( libpff_index_value_free(
	     &lookup_index_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free lookup index value.",
		 function,
		 lookup_identifier );

		goto on_error;
	}
	if( result == 0 )
	{
		return( 0 );
	}
	if( libpff_index_values_list_number_of_values(
	     index_values_list,
	     &number_of_index_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of recovered offsets index value: %" PRIu64 " list.",
		 function,
		 lookup_identifier );

		goto on_error;
	}
/* TODO handle if more than 1 offsets index value is recoverable */

	result = 0;

	for( index_value_iterator = 0;
	     index_value_iterator < number_of_index_values;
	     index_value_iterator++ )
	{
		if( libpff_index_values_list_get_value_by_index(
		     index_values_list,
		     index_value_iterator,
		     &index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve recovered offsets index value: %d for data identifier: %" PRIu64 ".",
			 function,
			 index_value_iterator,
			 lookup_identifier );

			goto on_error;
		}
		if( ( data_block_data_offset == index_value->file_offset )
		 && ( data_block_data_size == index_value->data_size ) )
		{
			result = 1;
		}
		if( result != 0 )
		{
			break;
		}
	}
	if( result != 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: recovered data block with identifier: %" PRIu64 " matches existing recovered item value.\n",
			 function,
			 lookup_identifier );
		}
#endif
	}
	return( result );

on_error:
	if( lookup_index_value != NULL )
	{
		libpff_index_value_free(
		 &lookup_index_value,
		 NULL );
	}
	return( -1 );
}

/* Analyze if a specific descriptors index value is recoverable
 * Returns 1 if recoverable, 0 if not or -1 on error
 */
int libpff_recover_analyze_descriptors_index_value(
     libpff_descriptors_index_t *descriptors_index,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_index_value_t *descriptors_index_value,
     libcerror_error_t **error )
{
	libpff_index_value_t *existing_index_value = NULL;
	static char *function                      = "libpff_recover_analyze_descriptors_index_value";
	int result                                 = 0;

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
	if( descriptors_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptors index value.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: analyzing identifier: %" PRIu64 ", data: %" PRIu64 ", local descriptors: %" PRIu64 ", parent: %" PRIu32 "\n",
		 function,
		 descriptors_index_value->identifier,
		 descriptors_index_value->data_identifier,
		 descriptors_index_value->local_descriptors_identifier,
		 descriptors_index_value->parent_identifier );
	}
#endif
	/* Check if the descriptors index value matches an existing index value
	 */
	result = libpff_index_get_value_by_identifier(
		  descriptors_index->index,
		  io_handle,
		  file_io_handle,
		  descriptors_index_value->identifier,
		  &existing_index_value,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve descriptors index value: %" PRIu64 " from index.",
		 function,
		 descriptors_index_value->identifier );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( existing_index_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing descriptors index value for identifier: %" PRIu64 ".",
			 function,
			 descriptors_index_value->identifier );

			goto on_error;
		}
		result = 0;

/* TODO what about parent changes ? */
		if( ( descriptors_index_value->data_identifier == existing_index_value->data_identifier )
		 && ( descriptors_index_value->local_descriptors_identifier == existing_index_value->local_descriptors_identifier ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: deleted descriptors index value: %" PRIu64 " matches existing item value.\n",
				 function,
				 descriptors_index_value->identifier );
			}
#endif
			result = 1;
		}
		if( libpff_index_value_free(
		     &existing_index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free index value: %d.",
			 function,
			 descriptors_index_value->identifier );

			goto on_error;
		}
		if( result != 0 )
		{
			return( 0 );
		}
	}
	result = libpff_recover_check_descriptors_index_for_recovered_value(
		  descriptors_index,
		  descriptors_index_value,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if deleted descriptors index value: %" PRIu64 " was previously recovered.",
		 function,
		 descriptors_index_value->identifier );

		goto on_error;
	}
	else if( result != 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: deleted descriptors index value: %" PRIu64 " matches previous recovered index value.\n",
			 function,
			 descriptors_index_value->identifier );
		}
#endif
		return( 0 );
	}
	return( 1 );

on_error:
	if( existing_index_value != NULL )
	{
		libpff_index_value_free(
		 &existing_index_value,
		 NULL );
	}
	return( -1 );
}

/* Checks if the specific descriptors index contains a specific recovered index value
 * Returns 1 if exists, 0 if not or -1 on error
 */
int libpff_recover_check_descriptors_index_for_recovered_value(
     libpff_descriptors_index_t *descriptors_index,
     libpff_index_value_t *descriptors_index_value,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *upper_node              = NULL;
	libpff_index_value_t *index_value             = NULL;
	libpff_index_values_list_t *index_values_list = NULL;
	static char *function                         = "libpff_recover_check_descriptors_index_for_recovered_value";
	int index_value_iterator                      = 0;
	int number_of_index_values                    = 0;
	int result                                    = 0;

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
	if( descriptors_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptors index value.",
		 function );

		return( -1 );
	}
	result = libcdata_btree_get_value_by_value(
		  descriptors_index->recovered_index_values_tree,
		  (intptr_t *) descriptors_index_value,
		  (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &libpff_index_values_list_compare,
		  &upper_node,
		  (intptr_t **) &index_values_list,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve recovered descriptors index value: %" PRIu64 " list.",
		 function,
		 descriptors_index_value->identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libpff_index_values_list_number_of_values(
		     index_values_list,
		     &number_of_index_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of recovered descriptors index values for identifier: %" PRIu64 ".",
			 function,
			 descriptors_index_value->identifier );

			return( -1 );
		}
		result = 0;

		for( index_value_iterator = 0;
		     index_value_iterator < number_of_index_values;
		     index_value_iterator++ )
		{
			if( libpff_index_values_list_get_value_by_index(
			     index_values_list,
			     index_value_iterator,
			     &index_value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve recovered descriptors index value: %" PRIu64 " list element: %d.",
				 function,
				 descriptors_index_value->identifier,
				 index_value_iterator );

				return( -1 );
			}
			if( index_value == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing recovered descriptors index value: %" PRIu64 " list element: %d.",
				 function,
				 descriptors_index_value->identifier,
				 index_value_iterator );

				return( -1 );
			}
/* TODO what about parent changes ? */
			if( ( descriptors_index_value->data_identifier == index_value->data_identifier )
			 && ( descriptors_index_value->local_descriptors_identifier == index_value->local_descriptors_identifier ) )
			{
				result = 1;

				break;
			}
		}
	}
	return( result );
}

/* Recovers descriptors index values
 * Returns 1 if successful or -1 on error
 */
int libpff_recover_descriptors_index_values(
     libpff_descriptors_index_t *descriptors_index,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t node_offset,
     uint64_t node_back_pointer,
     int recursion_depth,
     libcerror_error_t **error )
{
	libpff_index_node_t *index_node   = NULL;
	libpff_index_value_t *index_value = NULL;
	uint8_t *node_entry_data          = NULL;
	static char *function             = "libpff_recover_descriptors_index_values";
	size64_t node_data_size           = 0;
	off64_t node_data_offset          = 0;
	uint64_t sub_node_back_pointer    = 0;
	uint64_t sub_node_offset          = 0;
	uint16_t entry_index              = 0;
	int result                        = 0;

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
		 "%s: unable to create descriptors index node.",
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
		 "%s: unable to read descriptors index node at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 node_offset,
		 node_offset );

		goto on_error;
	}
	if( index_node->level == LIBPFF_INDEX_NODE_LEVEL_LEAF )
	{
		for( entry_index = index_node->number_of_entries;
		     entry_index < index_node->maximum_number_of_entries;
		     entry_index++ )
		{
/* TODO add if( io_handle->abort != 0 ) */
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
				 "%s: unable to retrieve deleted node entry: %" PRIu16 " data.",
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
				 "%s: missing deleted node entry: %" PRIu16 " data.",
				 function,
				 entry_index );

				goto on_error;
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
			result = libpff_recover_analyze_descriptors_index_value(
			          descriptors_index,
			          io_handle,
			          file_io_handle,
			          index_value,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to analyze deleted descriptors index value: %" PRIu16 ".",
				 function,
				 entry_index );

				goto on_error;
			}
			else if( result != 0 )
			{
				/* Add the recovered descriptors index values to the index tree
				 */
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: decriptor index value: %" PRIu16 " identifier: %" PRIu64 " is recoverable.\n",
					 function,
					 entry_index,
					 index_value->identifier );
				}
#endif
				node_data_offset = node_offset + ( entry_index * index_node->entry_size );
				node_data_size   = index_node->entry_size;

				if( libpff_descriptors_index_insert_recovered_index_value(
				     descriptors_index,
				     index_value,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to insert recovered descriptors index value: %" PRIu64 " list.",
					 function,
					 index_value->identifier );

					goto on_error;
				}
				index_value = NULL;
			}
			else
			{
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
		}
	}
	else
	{
		if( node_back_pointer != index_node->back_pointer )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: back pointer mismatch (entry: %" PRIu64 ", node: %" PRIu64 ").",
			 function,
			 node_back_pointer,
			 index_node->back_pointer );

			goto on_error;
		}
		for( entry_index = 0;
		     entry_index < index_node->number_of_entries;
		     entry_index++ )
		{
/* TODO add if( io_handle->abort != 0 ) */
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

			if( libpff_recover_descriptors_index_values(
			     descriptors_index,
			     io_handle,
			     file_io_handle,
			     sub_node_offset,
			     sub_node_back_pointer,
			     recursion_depth + 1,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to analyze descriptors index node at offset: %" PRIi64 " (0x%08" PRIx64 ").",
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
	return( -1 );
}

/* Analyze if a specific offsets index value is recoverable
 * Returns 1 if recoverable, 0 if not or -1 on error
 */
int libpff_recover_analyze_offsets_index_value(
     libpff_offsets_index_t *offsets_index,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_index_value_t *offsets_index_value,
     uint32_t maximum_data_block_data_size,
     libcerror_error_t **error )
{
	libpff_index_value_t *existing_index_value = NULL;
	static char *function                      = "libpff_recover_analyze_offsets_index_value";
	int result                                 = 0;

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
	if( offsets_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offsets index value.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: analyzing identifier: %" PRIu64 " (%s) at offset: %" PRIi64 " of size: %" PRIu32 "\n",
		 function,
		 offsets_index_value->identifier,
		 ( ( offsets_index_value->identifier & LIBPFF_OFFSET_INDEX_IDENTIFIER_FLAG_INTERNAL ) ? "internal" : "external" ),
		 offsets_index_value->file_offset,
		 offsets_index_value->data_size );
	}
#endif
	/* Ignore index values without a valid file offset
	 */
	if( offsets_index_value->file_offset <= 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: deleted offsets index value: %" PRIu64 " has an invalid file offset: %" PRIi64 ".\n",
			 function,
			 offsets_index_value->identifier,
			 offsets_index_value->file_offset );
		}
#endif
		return( 0 );
	}
	/* Ignore index values without a valid data size
	 */
	if( ( offsets_index_value->data_size == 0 )
	 || ( (uint32_t) offsets_index_value->data_size > maximum_data_block_data_size ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: deleted offsets index value: %" PRIu64 " has an invalid data size: %" PRIu32 ".\n",
			 function,
			 offsets_index_value->identifier,
			 offsets_index_value->data_size );
		}
#endif
		return( 0 );
	}
	/* Check if the offsets index value matches an existing index value
	 */
	result = libpff_index_get_value_by_identifier(
		  offsets_index->index,
		  io_handle,
		  file_io_handle,
		  offsets_index_value->identifier,
		  &existing_index_value,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve offsets index value for identifier: %" PRIu64 ".",
		 function,
		 offsets_index_value->identifier );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( existing_index_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing offsets index value for identifier: %" PRIu64 ".",
			 function,
			 offsets_index_value->identifier );

			goto on_error;
		}
		result = 0;

		if( ( offsets_index_value->file_offset == existing_index_value->file_offset )
		 && ( offsets_index_value->data_size == existing_index_value->data_size ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: deleted offsets index value: %" PRIu64 " matches existing item value.\n",
				 function,
				 offsets_index_value->identifier );
			}
#endif
			result = 1;
		}
		if( libpff_index_value_free(
		     &existing_index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free index value: %d.",
			 function,
			 offsets_index_value->identifier );

			goto on_error;
		}
		if( result != 0 )
		{
			return( 0 );
		}
	}
	/* Check if the offsets index value matches a previously recovered index value
	 */
	result = libpff_recover_check_offsets_index_for_recovered_value(
		  offsets_index,
		  offsets_index_value,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if deleted offsets index value: %" PRIu64 " was previously recovered.",
		 function,
		 offsets_index_value->identifier );

		goto on_error;
	}
	else if( result != 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: deleted offsets index value: %" PRIu64 " matches previous recovered index value.\n",
			 function,
			 offsets_index_value->identifier );
		}
#endif
		return( 0 );
	}
	return( 1 );

on_error:
	if( existing_index_value != NULL )
	{
		libpff_index_value_free(
		 &existing_index_value,
		 NULL );
	}
	return( -1 );
}

/* Checks if the specific offsets index contains a specific recovered index value
 * Returns 1 if exists, 0 if not or -1 on error
 */
int libpff_recover_check_offsets_index_for_recovered_value(
     libpff_offsets_index_t *offsets_index,
     libpff_index_value_t *offsets_index_value,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *upper_node              = NULL;
	libpff_index_value_t *index_value             = NULL;
	libpff_index_values_list_t *index_values_list = NULL;
	static char *function                         = "libpff_recover_check_offsets_index_for_recovered_value";
	int index_value_iterator                      = 0;
	int number_of_index_values                    = 0;
	int result                                    = 0;

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
	if( offsets_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offsets index value.",
		 function );

		return( -1 );
	}
	result = libcdata_btree_get_value_by_value(
		  offsets_index->recovered_index_values_tree,
		  (intptr_t *) offsets_index_value,
		  (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &libpff_index_values_list_compare,
		  &upper_node,
		  (intptr_t **) &index_values_list,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve recovered offsets index value: %" PRIu64 " list.",
		 function,
		 offsets_index_value->identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libpff_index_values_list_number_of_values(
		     index_values_list,
		     &number_of_index_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of recovered offsets index values for identifier: %" PRIu64 ".",
			 function,
			 offsets_index_value->identifier );

			return( -1 );
		}
		result = 0;

		for( index_value_iterator = 0;
		     index_value_iterator < number_of_index_values;
		     index_value_iterator++ )
		{
			if( libpff_index_values_list_get_value_by_index(
			     index_values_list,
			     index_value_iterator,
			     &index_value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve recovered offsets index value: %" PRIu64 " list element: %d.",
				 function,
				 offsets_index_value->identifier,
				 index_value_iterator );

				return( -1 );
			}
			if( index_value == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing recovered offsets index value: %" PRIu64 " list element: %d.",
				 function,
				 offsets_index_value->identifier,
				 index_value_iterator );

				return( -1 );
			}
			if( ( offsets_index_value->file_offset == index_value->file_offset )
			 && ( offsets_index_value->data_size == index_value->data_size ) )
			{
				result = 1;

				break;
			}
		}
	}
	return( result );
}

/* Scans an offsets index node for recoverable index nodes
 * Returns 1 if successful or -1 on error
 */
int libpff_recover_analyze_offsets_index_node(
     libpff_offsets_index_t *offsets_index,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t node_offset,
     uint64_t node_back_pointer,
     uint32_t maximum_data_block_data_size,
     int recursion_depth,
     libcerror_error_t **error )
{
	libpff_index_node_t *index_node   = NULL;
	libpff_index_value_t *index_value = NULL;
	uint8_t *node_entry_data          = NULL;
	static char *function             = "libpff_recover_analyze_offsets_index_node";
	size64_t node_data_size           = 0;
	off64_t node_data_offset          = 0;
	uint64_t sub_node_back_pointer    = 0;
	uint64_t sub_node_offset          = 0;
	uint16_t entry_index              = 0;
	int result                        = 0;

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
	if( offsets_index->index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid offsets index - missing index.",
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
		 "%s: unable to create offsets index node.",
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
		 "%s: unable to read offsets index node at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 node_offset,
		 node_offset );

		goto on_error;
	}
	if( index_node->level == LIBPFF_INDEX_NODE_LEVEL_LEAF )
	{
		for( entry_index = index_node->number_of_entries;
		     entry_index < index_node->maximum_number_of_entries;
		     entry_index++ )
		{
/* TODO add if( io_handle->abort != 0 ) */
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
				 "%s: unable to retrieve deleted node entry: %" PRIu16 " data.",
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
				 "%s: missing deleted node entry: %" PRIu16 " data.",
				 function,
				 entry_index );

				goto on_error;
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
			if( libpff_index_value_read_data(
			     index_value,
			     io_handle,
			     LIBPFF_INDEX_TYPE_OFFSET,
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
			result = libpff_recover_analyze_offsets_index_value(
			          offsets_index,
			          io_handle,
			          file_io_handle,
			          index_value,
			          maximum_data_block_data_size,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to analyze deleted offsets index value: %" PRIu16 ".",
				 function,
				 entry_index );

				goto on_error;
			}
			else if( result != 0 )
			{
				/* Add the recovered offsets index values to the index tree
				 */
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: offsets index value: %" PRIu16 " identifier: %" PRIu64 " is recoverable.\n",
					 function,
					 entry_index,
					 index_value->identifier );
				}
#endif
				node_data_offset = node_offset + ( entry_index * index_node->entry_size );
				node_data_size   = index_node->entry_size;

				if( libpff_offsets_index_insert_recovered_index_value(
				     offsets_index,
				     index_value,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to insert recovered offsets index value: %" PRIu64 " list.",
					 function,
					 index_value->identifier );

					goto on_error;
				}
				index_value = NULL;
			}
			else
			{
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
		}
	}
	else
	{
		if( node_back_pointer != index_node->back_pointer )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: back pointer mismatch (entry: %" PRIu64 ", node: %" PRIu64 ").",
			 function,
			 node_back_pointer,
			 index_node->back_pointer );

			goto on_error;
		}
		for( entry_index = 0;
		     entry_index < index_node->number_of_entries;
		     entry_index++ )
		{
/* TODO add if( io_handle->abort != 0 ) */
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

			if( libpff_recover_analyze_offsets_index_node(
			     offsets_index,
			     io_handle,
			     file_io_handle,
			     sub_node_offset,
			     sub_node_back_pointer,
			     maximum_data_block_data_size,
			     recursion_depth + 1,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to analyze offsets index node at offset: %" PRIi64 " (0x%08" PRIx64 ").",
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
	return( -1 );
}

/* Scans for recoverable data blocks
 * Returns 1 if successful or -1 on error
 */
int libpff_recover_data_blocks(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_descriptors_index_t *descriptors_index,
     libpff_offsets_index_t *offsets_index,
     libcdata_range_list_t *unallocated_data_block_list,
     libcdata_range_list_t *unallocated_page_block_list,
     uint8_t recovery_flags,
     libcerror_error_t **error )
{
	uint8_t *block_buffer                   = NULL;
	uint8_t *data_block_footer              = NULL;
	intptr_t *value                         = NULL;
	static char *function                   = "libpff_recover_data_blocks";
	off64_t block_buffer_data_offset        = 0;
	off64_t block_offset                    = 0;
	off64_t data_block_offset               = 0;
	off64_t page_block_offset               = 0;
	size64_t block_size                     = 0;
	size64_t data_block_size                = 0;
	size64_t page_block_size                = 0;
	size_t block_buffer_offset              = 0;
	size_t block_buffer_size_available      = 0;
	size_t data_block_data_offset           = 0;
	size_t read_size                        = 0;
	ssize_t read_count                      = 0;
	uint64_t data_block_back_pointer        = 0;
	uint32_t data_block_calculated_checksum = 0;
	uint32_t data_block_stored_checksum     = 0;
	uint32_t maximum_data_block_size        = 0;
	uint16_t data_block_data_size           = 0;
	uint16_t format_data_block_size         = 0;
	uint16_t format_page_block_size         = 0;
	uint16_t scan_block_size                = 0;
	uint8_t supported_recovery_flags        = 0;
	int number_of_unallocated_data_blocks   = 0;
	int number_of_unallocated_page_blocks   = 0;
	int result                              = 0;
	int unallocated_data_block_index        = 0;
	int unallocated_page_block_index        = 0;

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
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file type.",
		 function );

		return( -1 );
	}
	if( ( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
	 || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT ) )
	{
		format_data_block_size  = 64;
		format_page_block_size  = 512;
	}
	else
	{
		format_data_block_size  = 512;
		format_page_block_size  = 4096;
	}
	if( ( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
	 || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT ) )
	{
		maximum_data_block_size = 8192;
	}
	else
	{
/* TODO: this value is currently assumed based on the 512 x 8 = 4k page */
		maximum_data_block_size = 65536;
	}
	supported_recovery_flags = LIBPFF_RECOVERY_FLAG_IGNORE_ALLOCATION_DATA
	                         | LIBPFF_RECOVERY_FLAG_SCAN_FOR_FRAGMENTS;

	if( ( recovery_flags & ~( supported_recovery_flags ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported recovery flags.",
		 function );

		return( -1 );
	}
	/* Scan the unallocated page block list or all blocks for index nodes
	 */
	if( ( recovery_flags & LIBPFF_RECOVERY_FLAG_IGNORE_ALLOCATION_DATA ) == 0 )
	{
		if( libcdata_range_list_get_number_of_elements(
		     unallocated_data_block_list,
		     &number_of_unallocated_data_blocks,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of unallocated data blocks.",
			 function );

			goto on_error;
		}
		if( ( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
		 || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT ) )
		{
			if( libcdata_range_list_get_number_of_elements(
			     unallocated_page_block_list,
			     &number_of_unallocated_page_blocks,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve number of unallocated page blocks.",
				 function );

				goto on_error;
			}
		}
		else
		{
			number_of_unallocated_page_blocks = 0;
		}
	}
	if( ( recovery_flags & LIBPFF_RECOVERY_FLAG_SCAN_FOR_FRAGMENTS ) == 0 )
	{
		scan_block_size = format_page_block_size;
	}
	else
	{
		scan_block_size = format_data_block_size;
	}
	block_buffer = (uint8_t *) memory_allocate(
	                            sizeof( uint8_t ) * ( maximum_data_block_size * 2 ) );

	if( block_buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create block buffer.",
		 function );

		goto on_error;
	}
	if( ( number_of_unallocated_data_blocks > 0 )
	 || ( number_of_unallocated_page_blocks > 0 )
	 || ( ( recovery_flags & LIBPFF_RECOVERY_FLAG_IGNORE_ALLOCATION_DATA ) != 0 ) )
	{
		block_offset      = 0;
		data_block_offset = -1;
		page_block_offset = -1;

		while( block_offset < (off64_t) io_handle->file_size )
		{
			if( io_handle->abort != 0 )
			{
				goto on_error;
			}
			if( ( recovery_flags & LIBPFF_RECOVERY_FLAG_IGNORE_ALLOCATION_DATA ) == 0 )
			{
				if( data_block_offset < block_offset )
				{
					if( unallocated_data_block_index < number_of_unallocated_data_blocks )
					{
						if( libcdata_range_list_get_range_by_index(
						     unallocated_data_block_list,
						     unallocated_data_block_index,
						     (uint64_t *) &data_block_offset,
						     (uint64_t *) &data_block_size,
						     &value,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
							 "%s: unable to retrieve unallocated data block: %d.",
							 function,
							 unallocated_data_block_index );

							goto on_error;
						}
						unallocated_data_block_index++;
					}
					else
					{
						data_block_offset = (off64_t) io_handle->file_size;
						data_block_size   = 0;
					}
				}
				if( page_block_offset < block_offset )
				{
					if( unallocated_page_block_index < number_of_unallocated_page_blocks )
					{
						if( libcdata_range_list_get_range_by_index(
						     unallocated_page_block_list,
						     unallocated_page_block_index,
						     (uint64_t *) &page_block_offset,
						     (uint64_t *) &page_block_size,
						     &value,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
							 "%s: unable to retrieve unallocated page block: %d.",
							 function,
							 unallocated_page_block_index );

							goto on_error;
						}
						unallocated_page_block_index++;
					}
					else
					{
						page_block_offset = (off64_t) io_handle->file_size;
						page_block_size   = 0;
					}
				}
			}
			if( ( recovery_flags & LIBPFF_RECOVERY_FLAG_IGNORE_ALLOCATION_DATA ) == 0 )
			{
				if( ( data_block_offset >= (off64_t) io_handle->file_size )
				 && ( page_block_offset >= (off64_t) io_handle->file_size ) )
				{
					break;
				}
				/* Process the smallest offset
				 */
				else if( ( data_block_offset < page_block_offset )
				      && ( data_block_size > scan_block_size ) )
				{
					block_offset = data_block_offset;
					block_size   = data_block_size;
				}
				else if( ( page_block_offset < data_block_offset )
				      && ( page_block_size > scan_block_size ) )
				{
					block_offset = page_block_offset;
					block_size   = page_block_size;
				}
				/* Process the largest range
				 */
				else if( data_block_size > page_block_size )
				{
					block_offset = data_block_offset;
					block_size   = data_block_size;
				}
				else
				{
					block_offset = page_block_offset;
					block_size   = page_block_size;
				}
			}
			else
			{
				block_size = scan_block_size;
			}
			if( ( block_offset % scan_block_size ) != 0 )
			{
				block_offset  = ( ( block_offset / scan_block_size ) + 1 ) * scan_block_size;
				block_size   -= block_size % scan_block_size;
			}
			if( block_size < scan_block_size )
			{
				block_offset += block_size;

				continue;
			}
			while( block_size >= scan_block_size )
			{
				/* The index nodes have a fixed block size and stored block size aligned
				 */
				if( ( block_size >= format_page_block_size )
				 && ( ( block_offset % format_page_block_size ) == 0 ) )
				{
					/* Scan for index values in the index node
					 */
					result = libpff_recover_index_values(
						  io_handle,
						  file_io_handle,
						  descriptors_index,
						  offsets_index,
						  unallocated_data_block_list,
						  block_offset,
						  recovery_flags,
						  error );

					if( result == -1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
						 "%s: unable to recover index node at offset: %" PRIi64 ".\n",
						 function,
						 block_offset );

						goto on_error;
					}
					else if( result == 1 )
					{
						block_offset += format_page_block_size;
						block_size   -= format_page_block_size;

						continue;
					}
				}
				if( ( recovery_flags & LIBPFF_RECOVERY_FLAG_SCAN_FOR_FRAGMENTS ) == 0 )
				{
					block_offset += scan_block_size;
					block_size   -= scan_block_size;

					continue;
				}
/* TODO optimize by minimizing amount of reads */
				if( block_buffer_size_available == 0 )
				{
					block_buffer_data_offset = block_offset;

					if( block_buffer_offset > 0 )
					{
/* TODO optimize by copying the needed data to the front of the buffer */
						block_buffer_data_offset -= format_data_block_size;
						block_buffer_offset       = maximum_data_block_size - format_data_block_size;
					}
					read_size = (size_t) block_size;

					if( read_size > (size_t) maximum_data_block_size )
					{
						read_size = (size_t) maximum_data_block_size;
					}
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: reading data block at offset: %" PRIi64 " (0x%08" PRIx64 ") of size: %" PRIzd "\n",
						 function,
						 block_buffer_data_offset,
						 block_buffer_data_offset,
						 read_size );
					}
#endif
					read_count = libbfio_handle_read_buffer_at_offset(
						      file_io_handle,
						      &( block_buffer[ block_buffer_offset ] ),
						      read_size,
						      block_buffer_data_offset,
						      error );

					if( read_count != (ssize_t) read_size )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_READ_FAILED,
						 "%s: unable to read data block at offset: %" PRIi64 " (0x%08" PRIx64 ").",
						 function,
						 block_buffer_data_offset,
						 block_buffer_data_offset );

						goto on_error;
					}
					block_buffer_size_available = read_size;
				}
				if( block_buffer_size_available >= format_data_block_size )
				{
					/* Scan the block for a data block footer
					 */
					data_block_footer = &( block_buffer[ block_buffer_offset ] );

					if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
					{
						data_block_footer += format_data_block_size - sizeof( pff_block_footer_32bit_t );

						byte_stream_copy_to_uint16_little_endian(
						 ( (pff_block_footer_32bit_t *) data_block_footer )->data_size,
						 data_block_data_size );
						byte_stream_copy_to_uint32_little_endian(
						 ( (pff_block_footer_32bit_t *) data_block_footer )->back_pointer,
						 data_block_back_pointer );
						byte_stream_copy_to_uint32_little_endian(
						 ( (pff_block_footer_32bit_t *) data_block_footer )->checksum,
						 data_block_stored_checksum );
					}
					else if( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
					{
						data_block_footer += format_data_block_size - sizeof( pff_block_footer_64bit_t );

						byte_stream_copy_to_uint16_little_endian(
						 ( (pff_block_footer_64bit_t *) data_block_footer )->data_size,
						 data_block_data_size );
						byte_stream_copy_to_uint32_little_endian(
						 ( (pff_block_footer_64bit_t *) data_block_footer )->checksum,
						 data_block_stored_checksum );
						byte_stream_copy_to_uint64_little_endian(
						 ( (pff_block_footer_64bit_t *) data_block_footer )->back_pointer,
						 data_block_back_pointer );
					}
					else if( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE )
					{
						data_block_footer += format_data_block_size - sizeof( pff_block_footer_64bit_4k_page_t );

						byte_stream_copy_to_uint16_little_endian(
						 ( (pff_block_footer_64bit_4k_page_t *) data_block_footer )->data_size,
						 data_block_data_size );
						byte_stream_copy_to_uint32_little_endian(
						 ( (pff_block_footer_64bit_4k_page_t *) data_block_footer )->checksum,
						 data_block_stored_checksum );
						byte_stream_copy_to_uint64_little_endian(
						 ( (pff_block_footer_64bit_4k_page_t *) data_block_footer )->back_pointer,
						 data_block_back_pointer );
					}
					/* Check if back pointer itself is not empty but the upper 32-bit are
					 */
					if( ( data_block_back_pointer != 0 )
					 && ( ( data_block_back_pointer >> 32 ) == 0 ) )
					{
						data_block_data_offset = block_buffer_offset - ( ( data_block_data_size / format_data_block_size ) * format_data_block_size );

						if( (size_t) data_block_data_size < read_size )
						{
							result = libpff_recover_analyze_data_block_back_pointer(
							          offsets_index,
							          data_block_back_pointer,
							          (off64_t) block_buffer_data_offset + data_block_data_offset,
							          (size32_t) data_block_data_size,
							          error );

							if( result == -1 )
							{
								libcerror_error_set(
								 error,
								 LIBCERROR_ERROR_DOMAIN_RUNTIME,
								 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
								 "%s: unable to recovere data block: %" PRIu64 ".",
								 function,
								 data_block_back_pointer );

								goto on_error;
							}
							else if( result != 0 )
							{
								block_offset += format_data_block_size;
								block_size   -= format_data_block_size;

/* TODO reset block_buffer_offset and block_buffer_size_available ? */

								continue;
							}
							if( data_block_stored_checksum != 0 )
							{
								if( libpff_checksum_calculate_weak_crc32(
								     &data_block_calculated_checksum,
								     &( block_buffer[ data_block_data_offset ] ),
								     data_block_data_size,
								     0,
								     error ) != 1 )
								{
									libcerror_error_set(
									 error,
									 LIBCERROR_ERROR_DOMAIN_RUNTIME,
									 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
									 "%s: unable to calculate weak CRC-32.",
									 function );

									goto on_error;
								}
								if( data_block_stored_checksum != data_block_calculated_checksum )
								{
#if defined( HAVE_DEBUG_OUTPUT )
									if( libcnotify_verbose != 0 )
									{
										libcnotify_printf(
										 "%s: mismatch in data block: %" PRIu64 " checksum ( %" PRIu32 " != %" PRIu32 " ).\n",
										 function,
										 data_block_back_pointer,
										 data_block_stored_checksum,
										 data_block_calculated_checksum );
									}
#endif
									block_offset += format_data_block_size;
									block_size   -= format_data_block_size;

									continue;
								}
							}
/* TODO consider data block as fragment */

							data_block_data_size = ( ( data_block_data_size / format_data_block_size ) + 1 ) * format_data_block_size;

#if defined( HAVE_DEBUG_OUTPUT )
							if( libcnotify_verbose != 0 )
							{
								libcnotify_printf(
								 "%s: data block back pointer: 0x%08" PRIx64 "\n",
								 function,
								 data_block_back_pointer );

								libcnotify_printf(
								 "%s: data block data at offset: %" PRIi64 " (0x%08" PRIx64 ") of size: %" PRIzd "\n",
								 function,
								 block_buffer_data_offset + data_block_data_offset,
								 block_buffer_data_offset + data_block_data_offset,
								 data_block_data_size );
								libcnotify_print_data(
								 &( block_buffer[ data_block_data_offset ] ),
								 data_block_data_size,
								 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
							}
#endif
						}
					}
					block_buffer_offset         += format_data_block_size;
					block_buffer_size_available -= format_data_block_size;
				}
				else
				{
					block_buffer_offset         = 0;
					block_buffer_size_available = 0;
				}
				block_offset += scan_block_size;
				block_size   -= scan_block_size;
			}
		}
	}
	memory_free(
	 block_buffer );

	return( 1 );

on_error:
	if( block_buffer != NULL )
	{
		memory_free(
		 block_buffer );
	}
	return( -1 );
}

/* Scans for recoverable index values in an index node
 * Returns 1 if successful, returns 0 if no valid index node could be found or -1 on error
 */
int libpff_recover_index_values(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_descriptors_index_t *descriptors_index,
     libpff_offsets_index_t *offsets_index,
     libcdata_range_list_t *unallocated_data_block_list,
     size64_t node_offset,
     uint8_t recovery_flags,
     libcerror_error_t **error )
{
	libpff_index_node_t *index_node       = NULL;
	libpff_index_value_t *index_value     = NULL;
	uint8_t *node_entry_data              = NULL;
	static char *function                 = "libpff_recover_index_values";
	const char *index_string              = NULL;
	off64_t index_value_file_offset       = 0;
	uint64_t index_value_identifier       = 0;
        uint32_t maximum_data_block_data_size = 0;
	uint16_t index_value_data_size        = 0;
	uint8_t entry_index                   = 0;
	int recoverable                       = 0;
	int result                            = 0;

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
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file type.",
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
	if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
	{
		maximum_data_block_data_size = 8192 - 12;
	}
	else if( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
	{
		maximum_data_block_data_size = 8192 - 16;
	}
	else
	{
/* TODO: this value is currently assumed based on the 512 x 8 = 4k page */
		maximum_data_block_data_size = 65536 - 24;
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
#if defined( HAVE_DEBUG_OUTPUT )
		if( ( libcnotify_verbose != 0 )
		 && ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
#endif
		libcerror_error_free(
		 error );

		libpff_index_node_free(
		 &index_node,
		 NULL );

		return( 0 );
	}
	if( index_node->type == LIBPFF_INDEX_TYPE_DESCRIPTOR )
	{
		index_string = "descriptors";
	}
	else if( index_node->type == LIBPFF_INDEX_TYPE_OFFSET )
	{
		index_string = "offsets";
	}
	if( ( index_node->type != LIBPFF_INDEX_TYPE_DESCRIPTOR )
	 && ( index_node->type != LIBPFF_INDEX_TYPE_OFFSET ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unsupported index type: 0x%02" PRIx8 ".\n",
			 function,
			 index_node->type );
		}
#endif
	}
	else if( index_node->level != LIBPFF_INDEX_NODE_LEVEL_LEAF )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: skipping %s index branch node at level: %" PRIu8 ".\n",
			 function,
			 index_string,
			 index_node->level );
		}
#endif
	}
	else
	{
		/* Check if the index leaf entries are recoverable
		 */
		for( entry_index = 0;
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
				 "%s: unable to retrieve node entry: %" PRIu8 " data.",
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
				 "%s: missing node entry: %" PRIu8 " data.",
				 function,
				 entry_index );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: analyzing %s index entry: %" PRIu8 ".\n",
				 function,
				 index_string,
				 entry_index );
			}
#endif
			if( libpff_index_value_initialize(
			     &index_value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create %s index value.",
				 function,
				 index_string );

				goto on_error;
			}
			if( libpff_index_value_read_data(
			     index_value,
			     io_handle,
			     index_node->type,
			     node_entry_data,
			     (size_t) index_node->entry_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read %s index value.",
				 function,
				 index_string );

				goto on_error;
			}
			recoverable = 1;

			/* Ignore index values without an identifier
			 */
			if( index_value->identifier == 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: %s index entry: %" PRIu8 " has an empty identifier.\n",
					 function,
					 index_string,
					 entry_index );
				}
#endif
				recoverable = 0;
			}
			else if( index_node->type == LIBPFF_INDEX_TYPE_DESCRIPTOR )
			{
				/* Ignore descriptors index values without a data identifier
				 */
				if( index_value->data_identifier == 0 )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: %s index entry: %" PRIu8 " identifier: %" PRIu64 " has an empty data identifier.\n",
						 function,
						 index_string,
						 entry_index,
						 index_value->identifier );
					}
#endif
					recoverable = 0;
				}
			}
			else if( index_node->type == LIBPFF_INDEX_TYPE_OFFSET )
			{
				/* Ignore index values without a valid file offset
				 */
				if( index_value->file_offset <= 0 )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: %s index entry: %" PRIu8 " identifier: %" PRIu64 " has an invalid file offset: %" PRIi64 " (0x%" PRIx64 ").\n",
						 function,
						 index_string,
						 entry_index,
						 index_value->identifier,
						 index_value->file_offset,
						 index_value->file_offset );
					}
#endif
					recoverable = 0;
				}
				/* Ignore index values without a valid data size
				 */
				else if( ( index_value->data_size == 0 )
				      || ( (uint32_t) index_value->data_size > maximum_data_block_data_size ) )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: %s index entry: %" PRIu8 " identifier: %" PRIu64 " has an invalid data size: %" PRIu16 ".\n",
						 function,
						 index_string,
						 entry_index,
						 index_value->identifier,
						 index_value->data_size );
					}
#endif
					recoverable = 0;
				}
			}
			if( recoverable != 0 )
			{
				if( index_node->type == LIBPFF_INDEX_TYPE_DESCRIPTOR )
				{
					result = libpff_recover_analyze_descriptors_index_value(
						  descriptors_index,
						  io_handle,
						  file_io_handle,
						  index_value,
						  error );
				}
				else if( index_node->type == LIBPFF_INDEX_TYPE_OFFSET )
				{
					result = libpff_recover_analyze_offsets_index_value(
						  offsets_index,
						  io_handle,
						  file_io_handle,
						  index_value,
						  maximum_data_block_data_size,
						  error );
				}
				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GENERIC,
					 "%s: unable to analyze deleted %d index value: %" PRIu64 ".",
					 function,
					 index_string,
					 index_value->identifier );

					goto on_error;
				}
				recoverable = result;
			}
			if( recoverable != 0 )
			{
				/* Check if the offsets index value is unallocated according to the
				 * unallocated data block list
				 */
				if( ( index_node->type == LIBPFF_INDEX_TYPE_OFFSET )
				 && ( ( recovery_flags & LIBPFF_RECOVERY_FLAG_IGNORE_ALLOCATION_DATA ) == 0 ) )
				{
					result = libcdata_range_list_range_is_present(
						  unallocated_data_block_list,
						  (uint64_t) index_value_file_offset,
						  (uint64_t) index_value_data_size,
						  error );

					if( result == -1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: error determining if offset range is unallocated.",
						 function );

						goto on_error;
					}
					else if( result == 0 )
					{
#if defined( HAVE_DEBUG_OUTPUT )
						if( libcnotify_verbose != 0 )
						{
							libcnotify_printf(
							 "%s: %s index entry: %" PRIu8 " identifier: %" PRIu64 " refers to allocated range: 0x%08" PRIx64 " - 0x%08" PRIx64 " (%" PRIu64 ").\n",
							 function,
							 index_string,
							 entry_index,
							 index_value_identifier,
							 index_value_file_offset,
							 index_value_file_offset + index_value_data_size,
							 index_value_data_size );
						}
#endif
						recoverable = 0;
					}
#if defined( HAVE_DEBUG_OUTPUT )
					else if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: %s index entry: %" PRIu8 " identifier: %" PRIu64 " refers to unallocated range: 0x%08" PRIx64 " - 0x%08" PRIx64 " (%" PRIu64 ").\n",
						 function,
						 index_string,
						 entry_index,
						 index_value_identifier,
						 index_value_file_offset,
						 index_value_file_offset + index_value_data_size,
						 index_value_data_size );
					}
#endif
				}
			}
			if( recoverable != 0 )
			{
				/* Move the recovered node to the index
				 */
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: %s index entry: %" PRIu8 " identifier: %" PRIu64 " is recoverable.\n",
					 function,
					 index_string,
					 entry_index,
					 index_value_identifier );
				}
#endif
				if( index_node->type == LIBPFF_INDEX_TYPE_DESCRIPTOR )
				{
					result = libpff_descriptors_index_insert_recovered_index_value(
					          descriptors_index,
					          index_value,
					          error );
				}
				else if( index_node->type == LIBPFF_INDEX_TYPE_OFFSET )
				{
					result = libpff_offsets_index_insert_recovered_index_value(
					          offsets_index,
					          index_value,
					          error );
				}
				if( result != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to insert recovered %s index value: %" PRIu64 " list.",
					 function,
					 index_string,
					 index_value_identifier );

					goto on_error;
				}
				index_value = NULL;
			}
			else
			{
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
			node_offset += index_node->entry_size;
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
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
	return( -1 );
}

/* Analyze if specific data block is recoverable
 * Returns 1 if recoverable, 0 if not or -1 on error
 */
int libpff_recover_analyze_data_block(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t descriptor_identifier,
     libpff_index_value_t *offsets_index_value,
     libcerror_error_t **error )
{
	libpff_data_block_t *data_block = NULL;
	static char *function           = "libpff_recover_analyze_data_block";
	int result                      = 0;

	if( offsets_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offsets index value.",
		 function );

		return( -1 );
	}
	/* Check if the data block is readable
	 */
	if( libpff_data_block_initialize(
	     &data_block,
	     io_handle,
	     descriptor_identifier,
	     offsets_index_value->identifier,
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: attempting to read data block at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 offsets_index_value->file_offset,
		 offsets_index_value->file_offset );
	}
#endif
	result = libpff_data_block_read_file_io_handle(
		  data_block,
		  file_io_handle,
		  offsets_index_value->file_offset,
		  offsets_index_value->data_size,
		  io_handle->file_type,
		  error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read data block.",
		 function );
#if defined( HAVE_DEBUG_OUTPUT )
		if( ( libcnotify_verbose != 0 )
		 && ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
#endif
		libcerror_error_free(
		 error );

/* TODO delete unreadable offset identifier in offsets_index->recovered_index_tree */
	}
	if( libpff_data_block_free(
	     &data_block,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free recovered data block.",
		 function );

		goto on_error;
	}
/* TODO validate the block data ? */

	return( 1 );

on_error:
	if( data_block != NULL )
	{
		libpff_data_block_free(
		 &data_block,
		 NULL );
	}
	return( -1 );
}

/* Analyze if specific local descriptors are recoverable
 * Returns 1 if recoverable, 0 if not or -1 on error
 */
int libpff_recover_analyze_local_descriptors(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     uint64_t local_descriptors_identifier,
     libcerror_error_t **error )
{
	libpff_index_value_t *offsets_index_value                    = NULL;
	libpff_local_descriptors_node_t *local_descriptors_node      = NULL;
	uint8_t *node_entry_data                                     = NULL;
	static char *function                                        = "libpff_recover_analyze_local_descriptors";
	uint64_t local_descriptor_value_data_identifier              = 0;
	uint64_t local_descriptor_value_identifier                   = 0;
	uint64_t local_descriptor_value_local_descriptors_identifier = 0;
	uint64_t local_descriptor_value_sub_node_identifier          = 0;
	uint16_t entry_index                                         = 0;
	int result                                                   = 1;

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
	     io_handle,
	     file_io_handle,
	     local_descriptors_identifier,
	     1,
	     0,
	     &offsets_index_value,
	     error ) != 1 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( ( libcnotify_verbose != 0 )
		 && ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
#endif
		libcerror_error_free(
		 error );

		return( 0 );
	}
	if( offsets_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing offsets index value.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: local descriptors node identifier: %" PRIu64 " (%s) at offset: %" PRIi64 " of size: %" PRIu32 "\n",
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

		return( -1 );
	}
	if( libpff_local_descriptors_node_read_file_io_handle(
	     local_descriptors_node,
	     io_handle,
	     file_io_handle,
	     0 /* TODO descriptor identifier */,
	     offsets_index_value->identifier,
	     offsets_index_value->file_offset,
	     offsets_index_value->data_size,
	     error ) != 1 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( ( libcnotify_verbose != 0 )
		 && ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
#endif
		libcerror_error_free(
		 error );

		libpff_local_descriptors_node_free(
		 &local_descriptors_node,
		 NULL );

		return( 0 );
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
		if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			byte_stream_copy_to_uint32_little_endian(
			 node_entry_data,
			 local_descriptor_value_identifier );

			node_entry_data += 4;
		}
		else if( ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
		      || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
		{
			byte_stream_copy_to_uint64_little_endian(
			 node_entry_data,
			 local_descriptor_value_identifier );

			node_entry_data += 8;
		}
		/* Ignore the upper 32-bit of local descriptor identifiers
		 */
		local_descriptor_value_identifier &= 0xffffffffUL;

		/* Ignore local descriptor values without a data identifier
		 */
		if( local_descriptor_value_identifier == 0 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: local descriptor entry: %" PRIu8 " identifier: %" PRIu64 " has an empty identifier.\n",
				 function,
				 entry_index,
				 local_descriptor_value_identifier );
			}
#endif
			result = 0;

			break;
		}
		if( local_descriptors_node->level == LIBPFF_LOCAL_DESCRIPTOR_NODE_LEVEL_LEAF )
		{
			if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
			{
				byte_stream_copy_to_uint32_little_endian(
				 node_entry_data,
				 local_descriptor_value_data_identifier );

				node_entry_data += 4;

				byte_stream_copy_to_uint32_little_endian(
				 node_entry_data,
				 local_descriptor_value_local_descriptors_identifier );

				node_entry_data += 4;
			}
			else if( ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
			      || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
			{
				byte_stream_copy_to_uint64_little_endian(
				 node_entry_data,
				 local_descriptor_value_data_identifier );

				node_entry_data += 8;

				byte_stream_copy_to_uint64_little_endian(
				 node_entry_data,
				 local_descriptor_value_local_descriptors_identifier );

				node_entry_data += 8;
			}
			/* Ignore local descriptor values without a data identifier
			 */
			if( local_descriptor_value_data_identifier == 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: local descriptor entry: %" PRIu8 " identifier: %" PRIu64 " has an empty data identifier.\n",
					 function,
					 entry_index,
					 local_descriptor_value_identifier );
				}
#endif
				result = 0;

				break;
			}
		}
		else
		{
			if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
			{
				byte_stream_copy_to_uint32_little_endian(
				 node_entry_data,
				 local_descriptor_value_sub_node_identifier );

				node_entry_data += 4;
			}
			else if( ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
			      || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
			{
				byte_stream_copy_to_uint64_little_endian(
				 node_entry_data,
				 local_descriptor_value_sub_node_identifier );

				node_entry_data += 8;
			}
			/* Ignore local descriptor values without a sub node identifier
			 */
			if( local_descriptor_value_sub_node_identifier == 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: local descriptor entry: %" PRIu8 " identifier: %" PRIu64 " has an empty sub node identifier.\n",
					 function,
					 entry_index,
					 local_descriptor_value_identifier );
				}
#endif
				result = 0;

				break;
			}
			result = libpff_recover_analyze_local_descriptors(
				  io_handle,
				  file_io_handle,
				  offsets_index,
				  local_descriptor_value_sub_node_identifier,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to recover local descriptors: %" PRIu64 ".",
				 function,
				 local_descriptor_value_sub_node_identifier );

				libpff_local_descriptors_node_free(
				 &local_descriptors_node,
				 NULL );

				return( -1 );
			}
			else if( result == 0 )
			{
				break;
			}
		}
	}
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

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( result );
}

