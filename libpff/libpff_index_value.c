/*
 * Index value functions
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
#include "libpff_index_value.h"
#include "libpff_io_handle.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfdata.h"

#include "pff_index_node.h"

/* Creates an index value
 * Make sure the value index_value is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_index_value_initialize(
     libpff_index_value_t **index_value,
     libcerror_error_t **error )
{
	static char *function = "libpff_index_value_initialize";

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
	if( *index_value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid index value value already set.",
		 function );

		return( -1 );
	}
	*index_value = memory_allocate_structure(
	                libpff_index_value_t );

	if( *index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create index value.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *index_value,
	     0,
	     sizeof( libpff_index_value_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear index value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *index_value != NULL )
	{
		memory_free(
		 *index_value );

		*index_value = NULL;
	}
	return( -1 );
}

/* Frees an index value
 * Returns 1 if successful or -1 on error
 */
int libpff_index_value_free(
     libpff_index_value_t **index_value,
     libcerror_error_t **error )
{
	static char *function = "libpff_index_value_free";

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
	if( *index_value != NULL )
	{
		memory_free(
		 *index_value );

		*index_value = NULL;
	}
	return( 1 );
}

/* Compares two index values
 * Returns LIBFDATA_COMPARE_LESS, LIBFDATA_COMPARE_EQUAL, LIBFDATA_COMPARE_GREATER if successful or -1 on error
 */
int libpff_index_value_compare(
     libpff_index_value_t *first_index_value,
     libpff_index_value_t *second_index_value,
     libcerror_error_t **error )
{
	static char *function = "libpff_index_value_compare";

	if( first_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid first index value.",
		 function );

		return( -1 );
	}
	if( second_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid second index value.",
		 function );

		return( -1 );
	}
	if( first_index_value->identifier < second_index_value->identifier )
	{
		return( LIBFDATA_COMPARE_LESS );
	}
	else if( first_index_value->identifier > second_index_value->identifier )
	{
		return( LIBFDATA_COMPARE_GREATER );
	}
	return( LIBFDATA_COMPARE_EQUAL );
}

/* Reads the index value data
 * Returns 1 if successful or -1 on error
 */
int libpff_index_value_read_data(
     libpff_index_value_t *index_value,
     libpff_io_handle_t *io_handle,
     uint8_t index_node_type,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libpff_index_value_read_data";
	size_t value_size     = 0;
	uint64_t file_offset  = 0;

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
	if( index_node_type == LIBPFF_INDEX_TYPE_DESCRIPTOR )
	{
		if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			value_size = sizeof( pff_index_node_descriptor_entry_32bit_t );
		}
		else if( ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
		      || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
		{
			value_size = sizeof( pff_index_node_descriptor_entry_64bit_t );
		}
	}
	else if( index_node_type == LIBPFF_INDEX_TYPE_OFFSET )
	{
		if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			value_size = sizeof( pff_index_node_offset_entry_32bit_t );
		}
		else if( ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
		      || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
		{
			value_size = sizeof( pff_index_node_offset_entry_64bit_t );
		}
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
	if( ( data_size < value_size )
	 || ( data_size > SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: unsupported data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: index value data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 value_size,
		 0 );
	}
#endif
	if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
	{
		byte_stream_copy_to_uint32_little_endian(
		 data,
		 index_value->identifier );
	}
	else if( ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
	      || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
	{
		byte_stream_copy_to_uint64_little_endian(
		 data,
		 index_value->identifier );
	}
	/* Ignore the upper 32-bit of descriptor identifiers
	 */
	if( index_node_type == LIBPFF_INDEX_TYPE_DESCRIPTOR )
	{
		index_value->identifier &= 0xffffffffUL;
	}
	if( index_node_type == LIBPFF_INDEX_TYPE_DESCRIPTOR )
	{
		if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (pff_index_node_descriptor_entry_32bit_t *) data )->data_identifier,
			 index_value->data_identifier );

			byte_stream_copy_to_uint32_little_endian(
			 ( (pff_index_node_descriptor_entry_32bit_t *) data )->local_descriptors_identifier,
			 index_value->local_descriptors_identifier );

			byte_stream_copy_to_uint32_little_endian(
			 ( (pff_index_node_descriptor_entry_32bit_t *) data )->parent_identifier,
			 index_value->parent_identifier );
		}
		else if( ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
		      || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
		{
			byte_stream_copy_to_uint64_little_endian(
			 ( (pff_index_node_descriptor_entry_64bit_t *) data )->data_identifier,
			 index_value->data_identifier );

			byte_stream_copy_to_uint64_little_endian(
			 ( (pff_index_node_descriptor_entry_64bit_t *) data )->local_descriptors_identifier,
			 index_value->local_descriptors_identifier );

			byte_stream_copy_to_uint32_little_endian(
			 ( (pff_index_node_descriptor_entry_64bit_t *) data )->parent_identifier,
			 index_value->parent_identifier );
		}
	}
	else if( index_node_type == LIBPFF_INDEX_TYPE_OFFSET )
	{
		if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (pff_index_node_offset_entry_32bit_t *) data )->file_offset,
			 file_offset );

			byte_stream_copy_to_uint16_little_endian(
			 ( (pff_index_node_offset_entry_32bit_t *) data )->data_size,
			 index_value->data_size );

			byte_stream_copy_to_uint16_little_endian(
			 ( (pff_index_node_offset_entry_32bit_t *) data )->reference_count,
			 index_value->reference_count );
		}
		else if( ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
		      || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
		{
			byte_stream_copy_to_uint64_little_endian(
			 ( (pff_index_node_offset_entry_64bit_t *) data )->file_offset,
			 file_offset );

			byte_stream_copy_to_uint16_little_endian(
			 ( (pff_index_node_offset_entry_64bit_t *) data )->data_size,
			 index_value->data_size );

			byte_stream_copy_to_uint16_little_endian(
			 ( (pff_index_node_offset_entry_64bit_t *) data )->reference_count,
			 index_value->reference_count );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: identifier\t\t\t\t: %" PRIu64 " (0x%08" PRIx64 ")\n",
		 function,
		 index_value->identifier,
		 index_value->identifier );

		if( index_node_type == LIBPFF_INDEX_TYPE_DESCRIPTOR )
		{
			libcnotify_printf(
			 "%s: data identifier\t\t\t\t: %" PRIu64 " (0x%08" PRIx64 ")\n",
			 function,
			 index_value->data_identifier,
			 index_value->data_identifier );

			libcnotify_printf(
			 "%s: local descriptors identifier\t\t: %" PRIu64 " (0x%08" PRIx64 ")\n",
			 function,
			 index_value->local_descriptors_identifier,
			 index_value->local_descriptors_identifier );

			libcnotify_printf(
			 "%s: parent identifier\t\t\t\t: %" PRIu32 " (0x%08" PRIx32 ")\n",
			 function,
			 index_value->parent_identifier,
			 index_value->parent_identifier );
		}
		else if( index_node_type == LIBPFF_INDEX_TYPE_OFFSET )
		{
			libcnotify_printf(
			 "%s: file offset\t\t\t\t: %" PRIu64 " (0x%08" PRIx64 ")\n",
			 function,
			 file_offset,
			 file_offset );

			libcnotify_printf(
			 "%s: data size\t\t\t\t\t: %" PRIu64 "\n",
			 function,
			 index_value->data_size );

			libcnotify_printf(
			 "%s: reference count\t\t\t\t: %" PRIu64 "\n",
			 function,
			 index_value->reference_count );
		}
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( index_node_type == LIBPFF_INDEX_TYPE_OFFSET )
	{
		if( file_offset > (uint64_t) INT64_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid file offset value out of bounds.",
			 function );

			return( -1 );
		}
		index_value->file_offset = file_offset;
	}
	return( 1 );
}

