/*
 * Data array functions
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

#include "libpff_data_array.h"
#include "libpff_data_array_entry.h"
#include "libpff_data_block.h"
#include "libpff_definitions.h"
#include "libpff_encryption.h"
#include "libpff_index_value.h"
#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcdata.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_unused.h"

#include "pff_array.h"

/* Creates a data array
 * Make sure the value data_array is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_data_array_initialize(
     libpff_data_array_t **data_array,
     libpff_io_handle_t *io_handle,
     uint32_t descriptor_identifier,
     uint64_t data_identifier,
     libcerror_error_t **error )
{
	static char *function = "libpff_data_array_initialize";

	if( data_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data array.",
		 function );

		return( -1 );
	}
	if( *data_array != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data array value already set.",
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
	*data_array = memory_allocate_structure(
	               libpff_data_array_t );

	if( *data_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data array.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *data_array,
	     0,
	     sizeof( libpff_data_array_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data array.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *data_array )->entries ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data array entries array.",
		 function );

		goto on_error;
	}
	( *data_array )->descriptor_identifier = descriptor_identifier;
	( *data_array )->data_identifier       = data_identifier;
	( *data_array )->io_handle             = io_handle;

	return( 1 );

on_error:
	if( *data_array != NULL )
	{
		memory_free(
		 *data_array );

		*data_array = NULL;
	}
	return( -1 );
}

/* Frees a data array
 * Returns 1 if successful or -1 on error
 */
int libpff_data_array_free(
     libpff_data_array_t **data_array,
     libcerror_error_t **error )
{
	static char *function = "libpff_data_array_free";
	int result            = 1;

	if( data_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data array.",
		 function );

		return( -1 );
	}
	if( *data_array != NULL )
	{
		if( libcdata_array_free(
		     &( ( *data_array )->entries ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_data_array_entry_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free the data array entries array.",
			 function );

			result = -1;
		}
		memory_free(
		 *data_array );

		*data_array = NULL;
	}
	return( result );
}

/* Clones the data array
 * Returns 1 if successful or -1 on error
 */
int libpff_data_array_clone(
     libpff_data_array_t **destination_data_array,
     libpff_data_array_t *source_data_array,
     libcerror_error_t **error )
{
	static char *function = "libpff_data_array_clone";

	if( destination_data_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination data array.",
		 function );

		return( -1 );
	}
	if( *destination_data_array != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination data array already set.",
		 function );

		return( -1 );
	}
	if( source_data_array == NULL )
	{
		*destination_data_array = NULL;

		return( 1 );
	}
	*destination_data_array = memory_allocate_structure(
	                           libpff_data_array_t );

	if( *destination_data_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination data array.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *destination_data_array,
	     0,
	     sizeof( libpff_data_array_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear destination data array.",
		 function );

		goto on_error;
	}
	( *destination_data_array )->descriptor_identifier = source_data_array->descriptor_identifier;
	( *destination_data_array )->data_identifier       = source_data_array->data_identifier;
	( *destination_data_array )->io_handle             = source_data_array->io_handle;
	( *destination_data_array )->data_size             = source_data_array->data_size;
	( *destination_data_array )->flags                 = source_data_array->flags;

	if( libcdata_array_clone(
	     &( ( *destination_data_array )->entries ),
	     source_data_array->entries,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_data_array_entry_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libpff_data_array_entry_clone,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to clone local descriptors.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_data_array != NULL )
	{
		memory_free(
		 *destination_data_array );

		*destination_data_array = NULL;
	}
	return( -1 );
}

/* Reads the data array entries
 * Returns 1 if successful or -1 on error
 */
int libpff_data_array_read_entries(
     libpff_data_array_t *data_array,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     libfdata_list_t *descriptor_data_list,
     uint8_t recovered,
     uint8_t *array_data,
     size_t array_data_size,
     uint32_t *total_data_size,
     int recursion_depth,
     libcerror_error_t **error )
{
	libpff_data_array_entry_t *data_array_entry = NULL;
	libpff_data_block_t *data_block             = NULL;
	libpff_index_value_t *offset_index_value    = NULL;
	static char *function                       = "libpff_data_array_read_entries";
	size_t array_entry_data_size                = 0;
	uint64_t array_entry_identifier             = 0;
	uint32_t calculated_total_data_size         = 0;
	uint32_t sub_total_data_size                = 0;
	uint16_t array_entry_index                  = 0;
	uint16_t number_of_array_entries            = 0;
	uint8_t array_entries_level                 = 0;
	int element_index                           = 0;
	int previous_number_of_data_array_entries   = 0;

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
	if( data_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data array.",
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
		 "%s: unsupported file type.",
		 function );

		return( -1 );
	}
	if( array_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid array data.",
		 function );

		return( -1 );
	}
	if( ( array_data_size < sizeof( pff_array_t ) )
	 || ( array_data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid array data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( total_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid total data size.",
		 function );

		return( -1 );
	}
	if( ( recursion_depth < 0 )
	 || ( recursion_depth > LIBPFF_MAXIMUM_DATA_ARRAY_RECURSION_DEPTH ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid recursion depth value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: array:\n",
		 function );
		libcnotify_print_data(
		 array_data,
		 array_data_size,
		 0 );
	}
#endif
	if( ( (pff_array_t *) array_data )->signature != 0x01 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported array signature.",
		 function );

		goto on_error;
	}
	array_entries_level = ( (pff_array_t *) array_data )->array_entries_level;

	byte_stream_copy_to_uint16_little_endian(
	 ( (pff_array_t *) array_data )->number_of_entries,
	 number_of_array_entries );
	byte_stream_copy_to_uint32_little_endian(
	 ( (pff_array_t *) array_data )->total_data_size,
	 *total_data_size );

	array_data      += sizeof( pff_array_t );
	array_data_size -= sizeof( pff_array_t );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: array entries level\t\t\t: %" PRIu8 "\n",
		 function,
		 array_entries_level );

		libcnotify_printf(
		 "%s: array number of entries\t\t\t: %" PRIu16 "\n",
		 function,
		 number_of_array_entries );

		libcnotify_printf(
		 "%s: array total data size\t\t\t: %" PRIu32 "\n",
		 function,
		 *total_data_size );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( array_entries_level == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported array entries level: 0.",
		 function );

		goto on_error;
	}
	if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
	{
		array_entry_data_size = 4;
	}
	else if( ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
	      || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
	{
		array_entry_data_size = 8;
	}
	if( ( (size_t) number_of_array_entries * array_entry_data_size ) > array_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of array entries value out of bounds.",
		 function );

		goto on_error;
	}
	if( data_array->data_size == 0 )
	{
		data_array->data_size = *total_data_size;
	}
	if( libcdata_array_get_number_of_entries(
	     data_array->entries,
	     &previous_number_of_data_array_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of data array entries.",
		 function );

		goto on_error;
	}
	if( array_entries_level == 1 )
	{
		if( libfdata_list_resize(
		     descriptor_data_list,
		     previous_number_of_data_array_entries + (int) number_of_array_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize descriptor data list.",
			 function );

			goto on_error;
		}
		if( libcdata_array_resize(
		     data_array->entries,
		     previous_number_of_data_array_entries + (int) number_of_array_entries,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_data_array_entry_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize data array entries array.",
			 function );

			goto on_error;
		}
	}
	element_index = previous_number_of_data_array_entries;

	for( array_entry_index = 0;
	     array_entry_index < number_of_array_entries;
	     array_entry_index++ )
	{
		if( array_entry_data_size == 4 )
		{
			byte_stream_copy_to_uint32_little_endian(
			 array_data,
			 array_entry_identifier );
		}
		else if( array_entry_data_size == 8 )
		{
			byte_stream_copy_to_uint64_little_endian(
			 array_data,
			 array_entry_identifier );
		}
		array_data += array_entry_data_size;

/* TODO handle multiple recovered offset index values */
		if( libpff_offsets_index_get_index_value_by_identifier(
		     offsets_index,
		     io_handle,
		     file_io_handle,
		     array_entry_identifier,
		     recovered,
		     0,
		     &offset_index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to find data identifier: %" PRIu64 ".",
			 function,
			 array_entry_identifier );

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
			 array_entry_identifier );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: array entry: %03" PRIu16 " at level: %" PRIu8 " identifier: %" PRIu64 " (%s) at offset: 0x%08" PRIx64 " of size: %" PRIu32 "\n",
			 function,
			 array_entry_index,
			 array_entries_level,
			 offset_index_value->identifier,
			 ( ( offset_index_value->identifier & LIBPFF_OFFSET_INDEX_IDENTIFIER_FLAG_INTERNAL ) ? "internal" : "external" ),
			 offset_index_value->file_offset,
			 offset_index_value->data_size );
		}
#endif
		if( offset_index_value->file_offset < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid file offset value out of bounds.",
			 function );

			goto on_error;
		}
#if UINT32_MAX > SSIZE_MAX
		if( offset_index_value->data_size > (size32_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: data size value exceeds maximum.",
			 function );

			goto on_error;
		}
#endif
		/* The data block uses the identifier as the back pointer
		 */
		if( libpff_data_block_initialize(
		     &data_block,
		     io_handle,
		     data_array->descriptor_identifier,
		     offset_index_value->identifier,
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
		if( libpff_data_block_read_file_io_handle(
		     data_block,
		     file_io_handle,
		     offset_index_value->file_offset,
		     offset_index_value->data_size,
		     io_handle->file_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read data block.",
			 function );

			goto on_error;
		}
		if( array_entries_level == 1 )
		{
			calculated_total_data_size += data_block->uncompressed_data_size;

			if( calculated_total_data_size > data_array->data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: data size: %" PRIu32 " exceeds total data size: %" PRIu32 ".",
				 function,
				 calculated_total_data_size,
				 data_array->data_size );

				goto on_error;
			}
/* TODO replace separate function calls by libfdata_list_set_element_by_index_with_mapped_size */
			if( libfdata_list_set_element_by_index(
			     descriptor_data_list,
			     element_index,
			     0,
			     offset_index_value->file_offset,
			     (size64_t) offset_index_value->data_size,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set descriptor data list element: %d.",
				 function,
				 element_index );

				goto on_error;
			}
			if( libfdata_list_set_mapped_size_by_index(
			     descriptor_data_list,
			     element_index,
			     (size64_t) data_block->uncompressed_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set descriptor data list element: %d mapped size.",
				 function,
				 element_index );

				goto on_error;
			}
			if( libpff_data_array_entry_initialize(
			     &data_array_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create data array entry.",
				 function );

				goto on_error;
			}
			data_array_entry->data_identifier = offset_index_value->identifier;

			if( libcdata_array_set_entry_by_index(
			     data_array->entries,
			     element_index,
			     (intptr_t *) data_array_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set data array entry: %d.",
				 function,
				 element_index );

				goto on_error;
			}
			data_array_entry = NULL;
		}
		else
		{
			if( libpff_data_array_read_entries(
			     data_array,
			     io_handle,
			     file_io_handle,
			     offsets_index,
			     descriptor_data_list,
			     recovered,
			     data_block->data,
			     data_block->data_size,
			     &sub_total_data_size,
			     recursion_depth + 1,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read data block.",
				 function );

				goto on_error;
			}
			calculated_total_data_size += sub_total_data_size;
		}
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
		if( libpff_index_value_free(
		     &offset_index_value,
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
		element_index++;
	}
	if( *total_data_size != calculated_total_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: mismatch in total data size (%" PRIu32 " != %" PRIu32 ").",
		 function,
		 *total_data_size,
		 calculated_total_data_size );

		goto on_error;
	}
	return( 1 );

on_error:
	if( data_block != NULL )
	{
		libpff_data_block_free(
		 &data_block,
		 NULL );
	}
	if( data_array_entry != NULL )
	{
		libpff_data_array_entry_free(
		 &data_array_entry,
		 NULL );
	}
	if( offset_index_value != NULL )
	{
		libpff_index_value_free(
		 &offset_index_value,
		 NULL );
	}
	return( -1 );
}

/* Reads the data array element data
 * Callback for the descriptor data list
 * Returns the number of bytes read if successful or -1 on error
 */
int libpff_data_array_read_element_data(
     libpff_data_array_t *data_array,
     libbfio_handle_t *file_io_handle,
     libfdata_list_element_t *list_element,
     libfcache_cache_t *cache,
     int element_file_index LIBPFF_ATTRIBUTE_UNUSED,
     off64_t element_offset,
     size64_t element_size,
     uint32_t element_flags LIBPFF_ATTRIBUTE_UNUSED,
     uint8_t read_flags,
     libcerror_error_t **error )
{
	libpff_data_array_entry_t *data_array_entry = NULL;
	libpff_data_block_t *data_block             = NULL;
	static char *function                       = "libpff_data_array_read_element_data";
	int element_index                           = 0;

	LIBPFF_UNREFERENCED_PARAMETER( element_file_index )
	LIBPFF_UNREFERENCED_PARAMETER( element_flags )

	if( data_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data array.",
		 function );

		return( -1 );
	}
	if( data_array->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data array - missing IO handle.",
		 function );

		return( -1 );
	}
	if( element_size > (size64_t) UINT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid element size value exceeds maximum.\n",
		 function );

		return( -1 );
	}
	if( libfdata_list_element_get_element_index(
	     list_element,
	     &element_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element index.",
		 function );

		goto on_error;
	}
	if( libcdata_array_get_entry_by_index(
	     data_array->entries,
	     element_index,
	     (intptr_t **) &data_array_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data array entry: %d.",
		 function,
		 element_index );

		goto on_error;
	}
	if( data_array_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing data array entry.",
		 function );

		goto on_error;
	}
	if( libpff_data_block_initialize(
	     &data_block,
	     data_array->io_handle,
	     data_array->descriptor_identifier,
	     data_array_entry->data_identifier,
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
	if( libpff_data_block_read_file_io_handle(
	     data_block,
	     file_io_handle,
	     element_offset,
	     (size32_t) element_size,
	     data_array->io_handle->file_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read data block.",
		 function );

		goto on_error;
	}
/* TODO move function into data block ? */
	if( libpff_data_array_decrypt_entry_data(
	     data_array,
	     element_index,
	     data_array->io_handle->encryption_type,
	     data_block->data,
	     (size_t) element_size,
	     read_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
		 LIBCERROR_ENCRYPTION_ERROR_DECRYPT_FAILED,
		 "%s: unable to decrypt data array entry: %d data.",
		 function,
		 element_index );

		goto on_error;
	}
	if( libfdata_list_element_set_element_value(
	     list_element,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) cache,
	     (intptr_t *) data_block,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_data_block_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set data block as element value.",
		 function );

		goto on_error;
	}
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

/* Decrypts the data array entry data
 * Returns 1 if successful or -1 on error
 */
int libpff_data_array_decrypt_entry_data(
     libpff_data_array_t *data_array,
     int array_entry_index,
     uint8_t encryption_type,
     uint8_t *data,
     size_t data_size,
     uint8_t read_flags,
     libcerror_error_t **error )
{
	libpff_data_array_entry_t *data_array_entry = NULL;
	static char *function                       = "libpff_data_array_decrypt_entry_data";
	ssize_t process_count                       = 0;
	uint16_t table_index_offset                 = 0;
	uint8_t decrypt_data                        = 0;
	uint8_t force_decryption                    = 0;
	uint8_t node_identifier_type                = 0;
	uint8_t node_contains_table                 = 0;
	int number_of_data_array_entries            = 0;

	if( data_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data array.",
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
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     data_array->entries,
	     &number_of_data_array_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of data array entries.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     data_array->entries,
	     array_entry_index,
	     (intptr_t **) &data_array_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data array entry: %d.",
		 function,
		 array_entry_index );

		return( -1 );
	}
	if( data_array_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing data array entry.",
		 function );

		return( -1 );
	}
	/* Check if the internal (unencrypted) flag in (data) offset index identifier is not set
	 */
	if( ( data_array_entry->data_identifier & LIBPFF_OFFSET_INDEX_IDENTIFIER_FLAG_INTERNAL ) == 0 )
	{
		decrypt_data = 1;
	}
	/* Check if data is encrypted
	 * Some 'invalid' files have an encryption type of none but contain encrypted data
	 * Although they are considered invalid by Outlook it is still possilble to read them
	 */
	if( ( encryption_type == LIBPFF_ENCRYPTION_TYPE_NONE )
	 && ( ( read_flags & LIBPFF_READ_FLAG_IGNORE_FORCE_DECRYPTION ) == 0 )
	 && ( data_size > 4 ) )
	{
		node_identifier_type = (uint8_t) ( data_array->descriptor_identifier & 0x0000001fUL );

		if( ( ( node_identifier_type == LIBPFF_NODE_IDENTIFIER_TYPE_INTERNAL )
		  && ( ( data_array->descriptor_identifier == LIBPFF_DESCRIPTOR_IDENTIFIER_MESSAGE_STORE )
		   || ( data_array->descriptor_identifier == LIBPFF_DESCRIPTOR_IDENTIFIER_NAME_TO_ID_MAP )
		   || ( data_array->descriptor_identifier == LIBPFF_DESCRIPTOR_IDENTIFIER_UNKNOWN_2049 )
		   || ( data_array->descriptor_identifier == LIBPFF_DESCRIPTOR_IDENTIFIER_UNKNOWN_2081 )
		   || ( data_array->descriptor_identifier == LIBPFF_DESCRIPTOR_IDENTIFIER_UNKNOWN_2113 )
		   || ( data_array->descriptor_identifier == LIBPFF_DESCRIPTOR_IDENTIFIER_UNKNOWN_3073 ) ) )
		 || ( node_identifier_type == LIBPFF_NODE_IDENTIFIER_TYPE_FOLDER )
		 || ( node_identifier_type == LIBPFF_NODE_IDENTIFIER_TYPE_SEARCH_FOLDER )
		 || ( node_identifier_type == LIBPFF_NODE_IDENTIFIER_TYPE_MESSAGE )
		 || ( node_identifier_type == LIBPFF_NODE_IDENTIFIER_TYPE_ASSOCIATED_CONTENT )
		 || ( node_identifier_type == LIBPFF_NODE_IDENTIFIER_TYPE_SUB_FOLDERS )
		 || ( node_identifier_type == LIBPFF_NODE_IDENTIFIER_TYPE_SUB_MESSAGES )
		 || ( node_identifier_type == LIBPFF_NODE_IDENTIFIER_TYPE_SUB_ASSOCIATED_CONTENTS )
		 || ( node_identifier_type == LIBPFF_NODE_IDENTIFIER_TYPE_SEARCH_CONTENTS_TABLE )
		 || ( node_identifier_type == LIBPFF_NODE_IDENTIFIER_TYPE_ATTACHMENTS )
		 || ( node_identifier_type == LIBPFF_NODE_IDENTIFIER_TYPE_RECIPIENTS )
		 || ( node_identifier_type == LIBPFF_NODE_IDENTIFIER_TYPE_UNKNOWN_1718 )
		 || ( node_identifier_type == LIBPFF_NODE_IDENTIFIER_TYPE_UNKNOWN_1751 )
		 || ( node_identifier_type == LIBPFF_NODE_IDENTIFIER_TYPE_UNKNOWN_1784 ) )
		{
			node_contains_table = 1;
		}
		/* Only check the first entry
		 * some table array contain the table type in every array entry but not all
		 */
		if( ( array_entry_index == 0 )
		 && ( node_contains_table != 0 ) )
		{
			/* Test if the data contains an unencrypted table
			 * a table consists of 0xec in the third byte
			 * and 0x6c, 0x7c, 0x8c, 0x9c, 0xa5, 0xac, 0xbc, 0xcc in the fourth
			 */
			if( ( data[ 2 ] != 0xec )
			 || ( ( data[ 3 ] != 0x6c )
			  &&  ( data[ 3 ] != 0x7c )
			  &&  ( data[ 3 ] != 0x8c )
			  &&  ( data[ 3 ] != 0x9c )
			  &&  ( data[ 3 ] != 0xa5 )
			  &&  ( data[ 3 ] != 0xac )
			  &&  ( data[ 3 ] != 0xbc )
			  &&  ( data[ 3 ] != 0xcc ) ) )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: table signature missing trying to force decryption.\n",
					 function );
				}
#endif
				force_decryption         = 1;
				encryption_type          = LIBPFF_ENCRYPTION_TYPE_COMPRESSIBLE;
				decrypt_data             = 1;
				data_array_entry->flags |= LIBPFF_DATA_BLOCK_FLAG_DECRYPTION_FORCED;
				data_array->flags       |= LIBPFF_DATA_ARRAY_FLAG_DECRYPTION_FORCED;
			}
		}
		else if( data_array->io_handle->force_decryption == 1 )
		{
			/* Some of the last table array entries do not seem to be encrypted
			 */
			if( ( node_contains_table != 0 )
			 && ( array_entry_index == ( number_of_data_array_entries - 1 ) ) )
			{
				byte_stream_copy_to_uint16_little_endian(
				 data,
				 table_index_offset );

				if( (size_t) table_index_offset > data_size )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: detected encrypted last table array entry - decryption forced.\n",
						 function );
					}
#endif
					encryption_type          = LIBPFF_ENCRYPTION_TYPE_COMPRESSIBLE;
					decrypt_data             = 1;
					data_array_entry->flags |= LIBPFF_DATA_BLOCK_FLAG_DECRYPTION_FORCED;
					data_array->flags       |= LIBPFF_DATA_ARRAY_FLAG_DECRYPTION_FORCED;
				}
			}
			else
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: decryption forced.\n",
					 function );
				}
#endif

				encryption_type          = LIBPFF_ENCRYPTION_TYPE_COMPRESSIBLE;
				decrypt_data             = 1;
				data_array_entry->flags |= LIBPFF_DATA_BLOCK_FLAG_DECRYPTION_FORCED;
				data_array->flags       |= LIBPFF_DATA_ARRAY_FLAG_DECRYPTION_FORCED;
			}
		}
	}
	/* Check if unencrypted flag in offset index identifier is not set
	 */
	if( decrypt_data != 0 )
	{
		process_count = libpff_encryption_decrypt(
		                 encryption_type,
		                 (uint32_t) data_array_entry->data_identifier,
		                 data,
		                 data_size,
		                 error );

		if( process_count != (ssize_t) data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBCERROR_ENCRYPTION_ERROR_DECRYPT_FAILED,
			 "%s: unable to decrypt array entry data.",
			 function );

			return( -1 );
		}
		if( force_decryption != 0 )
		{
			/* Test if the data contains an unencrypted table
			 * a table consists of 0xec in the third byte
			 * and 0x6c, 0x7c, 0x8c, 0x9c, 0xa5, 0xac, 0xbc, 0xcc in the fourth
			 */
			if( ( data[ 2 ] == 0xec )
			 && ( ( data[ 3 ] == 0x6c )
			  ||  ( data[ 3 ] == 0x7c )
			  ||  ( data[ 3 ] == 0x8c )
			  ||  ( data[ 3 ] == 0x9c )
			  ||  ( data[ 3 ] == 0xa5 )
			  ||  ( data[ 3 ] == 0xac )
			  ||  ( data[ 3 ] == 0xbc )
			  ||  ( data[ 3 ] == 0xcc ) ) )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: compressible encrypted data detected while encryption type is none - decryption forced.\n",
					 function );
				}
#endif
				data_array->io_handle->force_decryption = 1;
			}
		}
	}
	return( 1 );
}

