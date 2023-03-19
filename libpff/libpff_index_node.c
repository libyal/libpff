/*
 * Index node functions
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
#include "libpff_definitions.h"
#include "libpff_index_node.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_types.h"

#include "pff_index_node.h"

/* Creates an index node
 * Make sure the value index_node is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_index_node_initialize(
     libpff_index_node_t **index_node,
     libcerror_error_t **error )
{
	static char *function = "libpff_index_node_initialize";

	if( index_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index node.",
		 function );

		return( -1 );
	}
	if( *index_node != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid index node value already set.",
		 function );

		return( -1 );
	}
	*index_node = memory_allocate_structure(
	               libpff_index_node_t );

	if( *index_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create index node.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *index_node,
	     0,
	     sizeof( libpff_index_node_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear index node.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *index_node != NULL )
	{
		memory_free(
		 *index_node );

		*index_node = NULL;
	}
	return( -1 );
}

/* Frees an index node
 * Returns 1 if successful or -1 on error
 */
int libpff_index_node_free(
     libpff_index_node_t **index_node,
     libcerror_error_t **error )
{
	static char *function = "libpff_index_node_free";

	if( index_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index node.",
		 function );

		return( -1 );
	}
	if( *index_node != NULL )
	{
		if( ( *index_node )->data != NULL )
		{
			memory_free(
			 ( *index_node )->data );
		}
		memory_free(
		 *index_node );

		*index_node = NULL;
	}
	return( 1 );
}

/* Retrieves the data of a specific entry
 * Returns 1 if successful or -1 on error
 */
int libpff_index_node_get_entry_data(
     libpff_index_node_t *index_node,
     uint16_t entry_index,
     uint8_t **entry_data,
     libcerror_error_t **error )
{
	static char *function = "libpff_index_node_get_entry_data";
	size_t entry_offset   = 0;

	if( index_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index node.",
		 function );

		return( -1 );
	}
	if( index_node->entries_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid index node - missing entries data.",
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
	if( entry_index > index_node->maximum_number_of_entries )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid entry index value out of bounds.",
		 function );

		return( -1 );
	}
	entry_offset = (size_t) index_node->entry_size * entry_index;

	if( entry_offset > (size_t) index_node->maximum_entries_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: entry index value exceeds maximum.",
		 function );

		return( -1 );
	}
	*entry_data = &( index_node->entries_data[ entry_offset ] );

	return( 1 );
}

/* Reads an index node
 * Returns 1 if successful or -1 on error
 */
int libpff_index_node_read_data(
     libpff_index_node_t *index_node,
     const uint8_t *data,
     size_t data_size,
     uint8_t file_type,
     libcerror_error_t **error )
{
	static char *function                        = "libpff_index_node_read_data";
	size_t checksum_data_size                    = 0;
	size_t index_node_data_size                  = 0;
	size_t index_node_footer_data_size           = 0;
	size_t maximum_entries_data_size             = 0;
	uint32_t calculated_checksum                 = 0;
	uint8_t calculated_entry_size                = 0;
	uint8_t calculated_maximum_number_of_entries = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t *index_node_entry_data               = NULL;
	uint64_t value_64bit                         = 0;
	uint32_t value_32bit                         = 0;
	uint16_t entry_index                         = 0;
	uint16_t index_node_entry_data_size          = 0;
	uint16_t value_16bit                         = 0;
	int result                                   = 0;
#endif

	if( index_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index node.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( file_type != LIBPFF_FILE_TYPE_32BIT )
	 && ( file_type != LIBPFF_FILE_TYPE_64BIT )
	 && ( file_type != LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file type.",
		 function );

		return( -1 );
	}
	if( file_type == LIBPFF_FILE_TYPE_32BIT )
	{
		checksum_data_size          = 500;
		index_node_data_size        = 512;
		index_node_footer_data_size = sizeof( pff_index_node_32bit_footer_t );
	}
	else if( file_type == LIBPFF_FILE_TYPE_64BIT )
	{
		checksum_data_size          = 496;
		index_node_data_size        = 512;
		index_node_footer_data_size = sizeof( pff_index_node_64bit_footer_t );
	}
	else if( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE )
	{
		checksum_data_size          = 4072;
		index_node_data_size        = 4096;
		index_node_footer_data_size = sizeof( pff_index_node_64bit_4k_page_footer_t );
	}
	maximum_entries_data_size = index_node_data_size - index_node_footer_data_size;

	if( data_size < index_node_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid data size value too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: index node data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( libpff_index_node_read_footer_data(
	     index_node,
	     &( data[ maximum_entries_data_size ] ),
	     index_node_footer_data_size,
	     file_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read index node footer.",
		 function );

		return( -1 );
	}
	if( libpff_checksum_calculate_weak_crc32(
	     &calculated_checksum,
	     data,
	     checksum_data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to calculate weak CRC-32.",
		 function );

		return( -1 );
	}
	if( index_node->stored_checksum != calculated_checksum )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: mismatch in checksum ( 0x%08" PRIx32 " != 0x%08" PRIx32 " ).\n",
			 function,
			 index_node->stored_checksum,
			 calculated_checksum );
		}
#endif
		/* TODO smart error handling */
	}
	if( file_type == LIBPFF_FILE_TYPE_32BIT )
	{
		if( ( index_node->type == LIBPFF_INDEX_TYPE_DESCRIPTOR )
		 && ( index_node->level == LIBPFF_INDEX_NODE_LEVEL_LEAF ) )
		{
			calculated_entry_size                = 16;
			calculated_maximum_number_of_entries = 496 / 16;
		}
		else
		{
			calculated_entry_size                = 12;
			calculated_maximum_number_of_entries = 496 / 12;
		}
	}
	else if( file_type == LIBPFF_FILE_TYPE_64BIT )
	{
		if( ( index_node->type == LIBPFF_INDEX_TYPE_DESCRIPTOR )
		 && ( index_node->level == LIBPFF_INDEX_NODE_LEVEL_LEAF ) )
		{
			calculated_entry_size                = 32;
			calculated_maximum_number_of_entries = 488 / 32;
		}
		else
		{
			calculated_entry_size                = 24;
			calculated_maximum_number_of_entries = 488 / 24;
		}
	}
	else if( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE )
	{
		if( ( index_node->type == LIBPFF_INDEX_TYPE_DESCRIPTOR )
		 && ( index_node->level == LIBPFF_INDEX_NODE_LEVEL_LEAF ) )
		{
			calculated_entry_size                = 32;
			calculated_maximum_number_of_entries = 4056 / 32;
		}
		else
		{
			calculated_entry_size                = 24;
			calculated_maximum_number_of_entries = 4056 / 24;
		}
	}
	if( ( index_node->entry_size != 0 )
	 && ( index_node->entry_size != calculated_entry_size ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: entry size mismatch (calculated: %" PRIu8 ", stored: %" PRIu8 ").\n",
			 function,
			 calculated_entry_size,
			 index_node->entry_size );
		}
#endif
		index_node->entry_size = calculated_entry_size;
	}
	if( ( index_node->maximum_number_of_entries != 0 )
	 && ( index_node->maximum_number_of_entries != calculated_maximum_number_of_entries ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: maximum number of entries mismatch (calculated: %" PRIu8 ", stored: %" PRIu8 ").\n",
			 function,
			 calculated_maximum_number_of_entries,
			 index_node->maximum_number_of_entries );
		}
#endif
		index_node->maximum_number_of_entries = calculated_maximum_number_of_entries;
	}
	if( index_node->number_of_entries > index_node->maximum_number_of_entries )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: number of entries: %" PRIu8 ", exceeds maximum: %" PRIu8 ".",
			 function,
			 index_node->number_of_entries,
			 index_node->maximum_number_of_entries );
		}
#endif
		index_node->number_of_entries = index_node->maximum_number_of_entries;
	}
	if( ( (uint16_t) index_node->number_of_entries * (uint16_t) index_node->entry_size ) > maximum_entries_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: size of entries: %" PRIu16 ", exceeds maximum: %" PRIzd ".",
		 function,
		 index_node->number_of_entries * index_node->entry_size,
		 maximum_entries_data_size );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		index_node_entry_data      = index_node->data;
		index_node_entry_data_size = maximum_entries_data_size;

		/* Print all the entries
		 */
		for( entry_index = 0;
		     entry_index < index_node->maximum_number_of_entries;
		     entry_index++ )
		{
			if( entry_index == index_node->number_of_entries )
			{
				result = libpff_index_node_check_for_empty_block(
					  index_node_entry_data,
					  index_node_entry_data_size,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to determine if remaining index nodes are empty.",
					 function );

					return( -1 );
				}
				else if( result != 0 )
				{
					break;
				}
				libcnotify_printf(
				 "\n" );
				libcnotify_printf(
				 "%s: remaining node entries\n",
				 function );
			}
			if( file_type == LIBPFF_FILE_TYPE_32BIT )
			{
				byte_stream_copy_to_uint32_little_endian(
				 index_node_entry_data,
				 value_64bit );
			}
			else if( ( file_type == LIBPFF_FILE_TYPE_64BIT )
			      || ( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
			{
				byte_stream_copy_to_uint64_little_endian(
				 index_node_entry_data,
				 value_64bit );
			}
			libcnotify_printf(
			 "%s: entry: %03" PRIu16 " index node identifier\t\t: 0x%08" PRIx64 " (%" PRIu64 ")\n",
			 function,
			 entry_index,
			 value_64bit,
			 value_64bit );

			/* Process descriptor index node leaf nodes
			 */
			if( ( index_node->type == LIBPFF_INDEX_TYPE_DESCRIPTOR )
			 && ( index_node->level == LIBPFF_INDEX_NODE_LEVEL_LEAF ) )
			{
				if( file_type == LIBPFF_FILE_TYPE_32BIT )
				{
					byte_stream_copy_to_uint32_little_endian(
					 ( (pff_index_node_descriptor_entry_32bit_t *) index_node_entry_data )->data_identifier,
					 value_64bit );
				}
				else if( ( file_type == LIBPFF_FILE_TYPE_64BIT )
				      || ( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
				{
					byte_stream_copy_to_uint64_little_endian(
					 ( (pff_index_node_descriptor_entry_64bit_t *) index_node_entry_data )->data_identifier,
					 value_64bit );
				}
				libcnotify_printf(
				 "%s: entry: %03" PRIu16 " data identifier\t\t\t: 0x%08" PRIx64 " (%" PRIu64 ")\n",
				 function,
				 entry_index,
				 value_64bit,
				 value_64bit );

				if( file_type == LIBPFF_FILE_TYPE_32BIT )
				{
					byte_stream_copy_to_uint32_little_endian(
					 ( (pff_index_node_descriptor_entry_32bit_t *) index_node_entry_data )->local_descriptors_identifier,
					 value_64bit );
				}
				else if( ( file_type == LIBPFF_FILE_TYPE_64BIT )
				      || ( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
				{
					byte_stream_copy_to_uint64_little_endian(
					 ( (pff_index_node_descriptor_entry_64bit_t *) index_node_entry_data )->local_descriptors_identifier,
					 value_64bit );
				}
				libcnotify_printf(
				 "%s: entry: %03" PRIu16 " local descriptors identifier\t: 0x%08" PRIx64 " (%" PRIu64 ")\n",
				 function,
				 entry_index,
				 value_64bit,
				 value_64bit );

				if( file_type == LIBPFF_FILE_TYPE_32BIT )
				{
					byte_stream_copy_to_uint32_little_endian(
					 ( (pff_index_node_descriptor_entry_32bit_t *) index_node_entry_data )->parent_identifier,
					 value_32bit );
				}
				else if( ( file_type == LIBPFF_FILE_TYPE_64BIT )
				      || ( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
				{
					byte_stream_copy_to_uint32_little_endian(
					 ( (pff_index_node_descriptor_entry_64bit_t *) index_node_entry_data )->parent_identifier,
					 value_32bit );
				}
				libcnotify_printf(
				 "%s: entry: %03" PRIu16 " parent identifier\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
				 function,
				 entry_index,
				 value_32bit,
				 value_32bit );

				if( ( file_type == LIBPFF_FILE_TYPE_64BIT )
				 || ( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
				{
					byte_stream_copy_to_uint32_little_endian(
					 ( (pff_index_node_descriptor_entry_64bit_t *) index_node_entry_data )->unknown1,
					 value_32bit );

					libcnotify_printf(
					 "%s: entry: %03" PRIu16 " unknown1\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
					 function,
					 entry_index,
					 value_32bit,
					 value_32bit );
				}
			}
			/* Process offset and descriptor index node branch nodes and offset index node leaf nodes
			 */
			else
			{
				if( index_node->level != LIBPFF_INDEX_NODE_LEVEL_LEAF )
				{
					if( file_type == LIBPFF_FILE_TYPE_32BIT )
					{
						byte_stream_copy_to_uint32_little_endian(
						 ( (pff_index_node_branch_entry_32bit_t *) index_node_entry_data )->back_pointer,
						 value_64bit );
					}
					else if( ( file_type == LIBPFF_FILE_TYPE_64BIT )
					      || ( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
					{
						byte_stream_copy_to_uint64_little_endian(
						 ( (pff_index_node_branch_entry_64bit_t *) index_node_entry_data )->back_pointer,
						 value_64bit );
					}
					libcnotify_printf(
					 "%s: entry: %03" PRIu16 " back pointer\t\t\t: 0x%08" PRIx64 "\n",
					 function,
					 entry_index,
					 value_64bit );
				}
				if( index_node->level == LIBPFF_INDEX_NODE_LEVEL_LEAF )
				{
					if( file_type == LIBPFF_FILE_TYPE_32BIT )
					{
						byte_stream_copy_to_uint32_little_endian(
						 ( (pff_index_node_offset_entry_32bit_t *) index_node_entry_data )->file_offset,
						 value_64bit );
					}
					else if( ( file_type == LIBPFF_FILE_TYPE_64BIT )
					      || ( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
					{
						byte_stream_copy_to_uint64_little_endian(
						 ( (pff_index_node_offset_entry_64bit_t *) index_node_entry_data )->file_offset,
						 value_64bit );
					}
				}
				else
				{
					if( file_type == LIBPFF_FILE_TYPE_32BIT )
					{
						byte_stream_copy_to_uint32_little_endian(
						 ( (pff_index_node_branch_entry_32bit_t *) index_node_entry_data )->file_offset,
						 value_64bit );
					}
					else if( ( file_type == LIBPFF_FILE_TYPE_64BIT )
					      || ( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
					{
						byte_stream_copy_to_uint64_little_endian(
						 ( (pff_index_node_branch_entry_64bit_t *) index_node_entry_data )->file_offset,
						 value_64bit );
					}
				}
				libcnotify_printf(
				 "%s: entry: %03" PRIu16 " file offset\t\t\t: 0x%08" PRIx64 " (%" PRIu64 ")\n",
				 function,
				 entry_index,
				 value_64bit,
				 value_64bit );

				if( index_node->level == LIBPFF_INDEX_NODE_LEVEL_LEAF )
				{
					if( file_type == LIBPFF_FILE_TYPE_32BIT )
					{
						byte_stream_copy_to_uint16_little_endian(
						 ( (pff_index_node_offset_entry_32bit_t *) index_node_entry_data )->data_size,
						 value_16bit );
					}
					else if( ( file_type == LIBPFF_FILE_TYPE_64BIT )
					      || ( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
					{
						byte_stream_copy_to_uint16_little_endian(
						 ( (pff_index_node_offset_entry_64bit_t *) index_node_entry_data )->data_size,
						 value_16bit );
					}
					libcnotify_printf(
					 "%s: entry: %03" PRIu16 " data size\t\t\t: %" PRIu16 "\n",
					 function,
					 entry_index,
					 value_16bit );

					if( file_type == LIBPFF_FILE_TYPE_32BIT )
					{
						byte_stream_copy_to_uint16_little_endian(
						 ( (pff_index_node_offset_entry_32bit_t *) index_node_entry_data )->reference_count,
						 value_16bit );
					}
					else if( ( file_type == LIBPFF_FILE_TYPE_64BIT )
					      || ( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
					{
						byte_stream_copy_to_uint16_little_endian(
						 ( (pff_index_node_offset_entry_64bit_t *) index_node_entry_data )->reference_count,
						 value_16bit );
					}
					libcnotify_printf(
					 "%s: entry: %03" PRIu16 " reference count\t\t\t: %" PRIu16 "\n",
					 function,
					 entry_index,
					 value_16bit );

					if( ( file_type == LIBPFF_FILE_TYPE_64BIT )
					 || ( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
					{
						byte_stream_copy_to_uint32_little_endian(
						 ( (pff_index_node_offset_entry_64bit_t *) index_node_entry_data )->data_allocation_table_file_offset,
						 value_32bit );

						libcnotify_printf(
						 "%s: entry: %03" PRIu16 " data allocation table offset\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
						 function,
						 entry_index,
						 value_32bit,
						 value_32bit );
					}
				}
			}
			index_node_entry_data      += index_node->entry_size;
			index_node_entry_data_size -= index_node->entry_size;
		}
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	index_node->maximum_entries_data_size = (uint16_t) maximum_entries_data_size;

	return( 1 );
}

/* Reads an index node footer
 * Returns 1 if successful or -1 on error
 */
int libpff_index_node_read_footer_data(
     libpff_index_node_t *index_node,
     const uint8_t *data,
     size_t data_size,
     uint8_t file_type,
     libcerror_error_t **error )
{
	static char *function              = "libpff_index_node_read_footer_data";
	size_t index_node_footer_data_size = 0;
	uint8_t index_node_type_copy       = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit               = 0;
	uint16_t value_16bit               = 0;
#endif

	if( index_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index node.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( file_type != LIBPFF_FILE_TYPE_32BIT )
	 && ( file_type != LIBPFF_FILE_TYPE_64BIT )
	 && ( file_type != LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file type.",
		 function );

		return( -1 );
	}
	if( file_type == LIBPFF_FILE_TYPE_32BIT )
	{
		index_node_footer_data_size = sizeof( pff_index_node_32bit_footer_t );;
	}
	else if( file_type == LIBPFF_FILE_TYPE_64BIT )
	{
		index_node_footer_data_size = sizeof( pff_index_node_64bit_footer_t );;
	}
	else if( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE )
	{
		index_node_footer_data_size = sizeof( pff_index_node_64bit_4k_page_footer_t );
	}
	if( data_size < index_node_footer_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid data size value too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: index node footer data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
	if( file_type == LIBPFF_FILE_TYPE_32BIT )
	{
		index_node->type     = ( (pff_index_node_32bit_footer_t *) data )->type;
		index_node_type_copy = ( (pff_index_node_32bit_footer_t *) data )->type_copy;

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_index_node_32bit_footer_t *) data )->back_pointer,
		 index_node->back_pointer );

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_index_node_32bit_footer_t *) data )->checksum,
		 index_node->stored_checksum );

		index_node->number_of_entries         = ( (pff_index_node_32bit_footer_t *) data )->number_of_entries;
		index_node->maximum_number_of_entries = ( (pff_index_node_32bit_footer_t *) data )->maximum_number_of_entries;
		index_node->entry_size                = ( (pff_index_node_32bit_footer_t *) data )->entry_size;
		index_node->level                     = ( (pff_index_node_32bit_footer_t *) data )->level;
	}
	else if( file_type == LIBPFF_FILE_TYPE_64BIT )
	{
		index_node->type     = ( (pff_index_node_64bit_footer_t *) data )->type;
		index_node_type_copy = ( (pff_index_node_64bit_footer_t *) data )->type_copy;

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_index_node_64bit_footer_t *) data )->checksum,
		 index_node->stored_checksum );

		byte_stream_copy_to_uint64_little_endian(
		 ( (pff_index_node_64bit_footer_t *) data )->back_pointer,
		 index_node->back_pointer );

		index_node->number_of_entries         = ( (pff_index_node_64bit_footer_t *) data )->number_of_entries;
		index_node->maximum_number_of_entries = ( (pff_index_node_64bit_footer_t *) data )->maximum_number_of_entries;
		index_node->entry_size                = ( (pff_index_node_64bit_footer_t *) data )->entry_size;
		index_node->level                     = ( (pff_index_node_64bit_footer_t *) data )->level;
	}
	else if( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE )
	{
		index_node->type     = ( (pff_index_node_64bit_4k_page_footer_t *) data )->type;
		index_node_type_copy = ( (pff_index_node_64bit_4k_page_footer_t *) data )->type_copy;

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_index_node_64bit_4k_page_footer_t *) data )->checksum,
		 index_node->stored_checksum );

		byte_stream_copy_to_uint64_little_endian(
		 ( (pff_index_node_64bit_4k_page_footer_t *) data )->back_pointer,
		 index_node->back_pointer );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_index_node_64bit_4k_page_footer_t *) data )->number_of_entries,
		 index_node->number_of_entries );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_index_node_64bit_4k_page_footer_t *) data )->maximum_number_of_entries,
		 index_node->maximum_number_of_entries );

		index_node->entry_size = ( (pff_index_node_64bit_4k_page_footer_t *) data )->entry_size;
		index_node->level      = ( (pff_index_node_64bit_4k_page_footer_t *) data )->level;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of entries\t\t\t: %" PRIu16 "\n",
		 function,
		 index_node->number_of_entries );

		libcnotify_printf(
		 "%s: maximum number of entries\t\t: %" PRIu16 "\n",
		 function,
		 index_node->maximum_number_of_entries );

		libcnotify_printf(
		 "%s: entry size\t\t\t\t: %" PRIu8 "\n",
		 function,
		 index_node->entry_size );

		libcnotify_printf(
		 "%s: node level\t\t\t\t: %" PRIu8 "\n",
		 function,
		 index_node->level );

		if( file_type == LIBPFF_FILE_TYPE_64BIT )
		{
			libcnotify_printf(
			 "%s: padding:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_index_node_64bit_footer_t *) data )->padding1,
			 4,
			 0 );
		}
		else if( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE )
		{
			libcnotify_printf(
			 "%s: padding:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_index_node_64bit_4k_page_footer_t *) data )->padding1,
			 10,
			 0 );
		}
		libcnotify_printf(
		 "%s: index node type\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 index_node->type );
		libcnotify_printf(
		 "%s: index node type copy\t\t: 0x%02" PRIx8 "\n",
		 function,
		 index_node_type_copy );

		if( file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			byte_stream_copy_to_uint16_little_endian(
			 ( (pff_index_node_32bit_footer_t *) data )->signature,
			 value_16bit );
			libcnotify_printf(
			 "%s: signature\t\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );

			libcnotify_printf(
			 "%s: back pointer\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 index_node->back_pointer );
			libcnotify_printf(
			 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 index_node->stored_checksum );
		}
		else if( file_type == LIBPFF_FILE_TYPE_64BIT )
		{
			byte_stream_copy_to_uint16_little_endian(
			 ( (pff_index_node_64bit_footer_t *) data )->signature,
			 value_16bit );
			libcnotify_printf(
			 "%s: signature\t\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );

			libcnotify_printf(
			 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 index_node->stored_checksum );
			libcnotify_printf(
			 "%s: back pointer\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 index_node->back_pointer );
		}
		else if( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE )
		{
			byte_stream_copy_to_uint16_little_endian(
			 ( (pff_index_node_64bit_4k_page_footer_t *) data )->signature,
			 value_16bit );
			libcnotify_printf(
			 "%s: signature\t\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );

			libcnotify_printf(
			 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 index_node->stored_checksum );
			libcnotify_printf(
			 "%s: back pointer\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 index_node->back_pointer );

			byte_stream_copy_to_uint64_little_endian(
			 ( (pff_index_node_64bit_4k_page_footer_t *) data )->unknown1,
			 value_64bit );
			libcnotify_printf(
			 "%s: unknown1\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );
		}
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( index_node->type != index_node_type_copy )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: mismatch in index node type (0x%02" PRIx8 " != 0x%02" PRIx8 ").\n",
			 function,
			 index_node->type,
			 index_node_type_copy );
		}
#endif
		if( ( index_node->type != LIBPFF_INDEX_TYPE_DESCRIPTOR )
		 && ( index_node->type != LIBPFF_INDEX_TYPE_OFFSET )
		 && ( ( index_node_type_copy == LIBPFF_INDEX_TYPE_DESCRIPTOR )
		   || ( index_node_type_copy == LIBPFF_INDEX_TYPE_OFFSET ) ) )
		{
			index_node->type = index_node_type_copy;
		}
	}
	if( ( index_node->type != LIBPFF_INDEX_TYPE_DESCRIPTOR )
	 && ( index_node->type != LIBPFF_INDEX_TYPE_OFFSET ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported index node type: 0x%02" PRIx8 ".",
		 function,
		 index_node->type );

		return( -1 );
	}
	return( 1 );
}

/* Reads an index node
 * Returns 1 if successful or -1 on error
 */
int libpff_index_node_read_file_io_handle(
     libpff_index_node_t *index_node,
     libbfio_handle_t *file_io_handle,
     off64_t node_offset,
     uint8_t file_type,
     libcerror_error_t **error )
{
	static char *function = "libpff_index_node_read_file_io_handle";
	ssize_t read_count    = 0;

	if( index_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index node.",
		 function );

		return( -1 );
	}
	if( index_node->data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid index node - data already set.",
		 function );

		return( -1 );
	}
	if( ( file_type != LIBPFF_FILE_TYPE_32BIT )
	 && ( file_type != LIBPFF_FILE_TYPE_64BIT )
	 && ( file_type != LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file type.",
		 function );

		return( -1 );
	}
	if( ( file_type == LIBPFF_FILE_TYPE_32BIT )
	 || ( file_type == LIBPFF_FILE_TYPE_64BIT ) )
	{
		index_node->data_size = 512;
	}
	else if( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE )
	{
		index_node->data_size = 4096;
	}
	index_node->data = (uint8_t *) memory_allocate(
	                                sizeof( uint8_t ) * index_node->data_size );

	if( index_node->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create index node data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading index node data at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 node_offset,
		 node_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              index_node->data,
	              index_node->data_size,
	              node_offset,
	              error );

	if( read_count != (ssize_t) index_node->data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read index node data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 node_offset,
		 node_offset );

		goto on_error;
	}
	if( libpff_index_node_read_data(
	     index_node,
	     index_node->data,
	     index_node->data_size,
	     file_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read index node.",
		 function );

		goto on_error;
	}
	index_node->entries_data = index_node->data;

	return( 1 );

on_error:
	if( index_node->data != NULL )
	{
		memory_free(
		 index_node->data );

		index_node->data = NULL;
	}
	return( -1 );
}

/* Checks if a buffer containing the chunk data is filled with same value bytes (empty-block)
 * Returns 1 if a pattern was found, 0 if not or -1 on error
 */
int libpff_index_node_check_for_empty_block(
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libpff_aligned_t *aligned_data_index = NULL;
	libpff_aligned_t *aligned_data_start = NULL;
	uint8_t *data_index                  = NULL;
	uint8_t *data_start                  = NULL;
	static char *function                = "libpff_index_node_check_for_empty_block";

	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	data_start = (uint8_t *) data;
	data_index = (uint8_t *) data + 1;
	data_size -= 1;

	/* Only optimize for data larger than the alignment
	 */
	if( data_size > ( 2 * sizeof( libpff_aligned_t ) ) )
	{
		/* Align the data start
		 */
		while( ( (intptr_t) data_start % sizeof( libpff_aligned_t ) ) != 0 )
		{
			if( *data_start != *data_index )
			{
				return( 0 );
			}
			data_start += 1;
			data_index += 1;
			data_size  -= 1;
		}
		/* Align the data index
		 */
		while( ( (intptr_t) data_index % sizeof( libpff_aligned_t ) ) != 0 )
		{
			if( *data_start != *data_index )
			{
				return( 0 );
			}
			data_index += 1;
			data_size  -= 1;
		}
		aligned_data_start = (libpff_aligned_t *) data_start;
		aligned_data_index = (libpff_aligned_t *) data_index;

		while( data_size > sizeof( libpff_aligned_t ) )
		{
			if( *aligned_data_start != *aligned_data_index )
			{
				return( 0 );
			}
			aligned_data_index += 1;
			data_size          -= sizeof( libpff_aligned_t );
		}
		data_index = (uint8_t *) aligned_data_index;
	}
	while( data_size != 0 )
	{
		if( *data_start != *data_index )
		{
			return( 0 );
		}
		data_index += 1;
		data_size  -= 1;
	}
	return( 1 );
}

