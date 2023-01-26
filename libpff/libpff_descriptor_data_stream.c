/*
 * The descriptor data stream functions
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

#include "libpff_data_block.h"
#include "libpff_definitions.h"
#include "libpff_descriptor_data_stream.h"
#include "libpff_libcerror.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_unused.h"

/* Creates a data handle
 * Make sure the value data_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_descriptor_data_stream_data_handle_initialize(
     libpff_descriptor_data_stream_data_handle_t **data_handle,
     libcerror_error_t **error )
{
	static char *function = "libpff_descriptor_data_stream_data_handle_initialize";

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( *data_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data handle value already set.",
		 function );

		return( -1 );
	}
	*data_handle = memory_allocate_structure(
	                libpff_descriptor_data_stream_data_handle_t );

	if( *data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *data_handle,
	     0,
	     sizeof( libpff_descriptor_data_stream_data_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *data_handle != NULL )
	{
		memory_free(
		 *data_handle );

		*data_handle = NULL;
	}
	return( -1 );
}

/* Frees a data handle
 * Returns 1 if successful or -1 on error
 */
int libpff_descriptor_data_stream_data_handle_free(
     libpff_descriptor_data_stream_data_handle_t **data_handle,
     libcerror_error_t **error )
{
	static char *function = "libpff_descriptor_data_stream_data_handle_free";
	int result            = 1;

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( *data_handle != NULL )
	{
		if( ( *data_handle )->flags == LIBPFF_DESCRIPTOR_DATA_STREAM_DATA_HANDLE_FLAG_MANAGED )
		{
			if( ( *data_handle )->descriptor_data_cache != NULL )
			{
				if( libfcache_cache_free(
				     &( ( *data_handle )->descriptor_data_cache ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to free descriptor data cache.",
					 function );

					result = -1;
				}
			}
			if( ( *data_handle )->descriptor_data_list != NULL )
			{
				if( libfdata_list_free(
				     &( ( *data_handle )->descriptor_data_list ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to free descriptor data list.",
					 function );

					result = -1;
				}
			}
		}
		memory_free(
		 *data_handle );

		*data_handle = NULL;
	}
	return( result );
}

/* Clones (duplicates) the data handle
 * Returns 1 if successful or -1 on error
 */
int libpff_descriptor_data_stream_data_handle_clone(
     libpff_descriptor_data_stream_data_handle_t **destination_data_handle,
     libpff_descriptor_data_stream_data_handle_t *source_data_handle,
     libcerror_error_t **error )
{
	static char *function = "libpff_descriptor_data_stream_data_handle_clone";

	if( destination_data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination data handle.",
		 function );

		return( -1 );
	}
	if( *destination_data_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination data handle value already set.",
		 function );

		return( -1 );
	}
	if( source_data_handle == NULL )
	{
		*destination_data_handle = NULL;

		return( 1 );
	}
	*destination_data_handle = memory_allocate_structure(
	                            libpff_descriptor_data_stream_data_handle_t );

	if( *destination_data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination data handle.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_data_handle,
	     source_data_handle,
	     sizeof( libpff_descriptor_data_stream_data_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination data handle.",
		 function );

		goto on_error;
	}
/* TODO clone list and cache ? */
	return( 1 );

on_error:
	if( *destination_data_handle != NULL )
	{
		memory_free(
		 *destination_data_handle );

		*destination_data_handle = NULL;
	}
	return( -1 );
}

/* Reads data from the current offset into a buffer
 * Callback for the descriptor data stream
 * Returns the number of bytes read or -1 on error
 */
ssize_t libpff_descriptor_data_stream_data_handle_read_segment_data(
         libpff_descriptor_data_stream_data_handle_t *data_handle,
         intptr_t *file_io_handle,
         int segment_index,
         int segment_file_index LIBPFF_ATTRIBUTE_UNUSED,
         uint8_t *segment_data,
         size_t segment_data_size,
         uint32_t segment_flags LIBPFF_ATTRIBUTE_UNUSED,
         uint8_t read_flags,
         libcerror_error_t **error )
{
	libpff_data_block_t *data_block = NULL;
	static char *function           = "libpff_descriptor_data_stream_data_handle_read_segment_data";
	off64_t element_data_offset     = 0;
	ssize_t read_count              = 0;
	int element_index               = 0;

	LIBPFF_UNREFERENCED_PARAMETER( segment_file_index )
	LIBPFF_UNREFERENCED_PARAMETER( segment_flags )

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( segment_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment data.",
		 function );

		return( -1 );
	}
	if( segment_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid segment data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( libfdata_list_get_element_value_at_offset(
	     data_handle->descriptor_data_list,
	     file_io_handle,
	     (libfdata_cache_t *) data_handle->descriptor_data_cache,
	     data_handle->current_offset,
	     &element_index,
	     &element_data_offset,
	     (intptr_t **) &data_block,
	     read_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data block: %d.",
		 function,
		 segment_index );

		return( -1 );
	}
	if( element_index != segment_index )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: element index value out of bounds.",
		 function );

		return( -1 );
	}
	if( data_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing data block: %d.",
		 function,
		 segment_index );

		return( -1 );
	}
	if( ( element_data_offset < 0 )
	 || ( element_data_offset >= (off64_t) data_block->uncompressed_data_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid element data offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( element_data_offset + segment_data_size ) > (off64_t) data_block->uncompressed_data_size )
	{
		read_count = (ssize_t) ( data_block->uncompressed_data_size - element_data_offset );
	}
	else
	{
		read_count = (ssize_t) segment_data_size;
	}
	if( memory_copy(
	     segment_data,
	     &( data_block->data[ element_data_offset ] ),
	     (size_t) read_count ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy data block data.",
		 function );

		return( -1 );
	}
	return( read_count );
}

/* Seeks a certain offset of the data
 * Callback for the descriptor data stream
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libpff_descriptor_data_stream_data_handle_seek_segment_offset(
         libpff_descriptor_data_stream_data_handle_t *data_handle,
         intptr_t *file_io_handle LIBPFF_ATTRIBUTE_UNUSED,
         int segment_index LIBPFF_ATTRIBUTE_UNUSED,
         int segment_file_index LIBPFF_ATTRIBUTE_UNUSED,
         off64_t segment_offset,
         libcerror_error_t **error )
{
	static char *function = "libpff_descriptor_data_stream_data_handle_seek_segment_offset";

	LIBPFF_UNREFERENCED_PARAMETER( file_io_handle )
	LIBPFF_UNREFERENCED_PARAMETER( segment_index )
	LIBPFF_UNREFERENCED_PARAMETER( segment_file_index )

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
		 function );

		return( -1 );
	}
	if( segment_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid segment offset value out of bounds.",
		 function );

		return( -1 );
	}
	data_handle->current_offset = segment_offset;

	return( segment_offset );
}

/* Creates a descriptor data stream
 * Make sure the value descriptor_data_stream is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_descriptor_data_stream_initialize(
     libfdata_stream_t **descriptor_data_stream,
     libfdata_list_t *descriptor_data_list,
     libfcache_cache_t *descriptor_data_cache,
     uint8_t flags,
     libcerror_error_t **error )
{
	libpff_descriptor_data_stream_data_handle_t *data_handle = NULL;
	static char *function                                    = "libpff_descriptor_data_stream_initialize";
	off64_t segment_offset                                   = 0;
	size64_t segment_size                                    = 0;
	int element_index                                        = 0;
	int number_of_elements                                   = 0;
	int segment_index                                        = 0;

	if( descriptor_data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptor data stream.",
		 function );

		return( -1 );
	}
	if( descriptor_data_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptor data list.",
		 function );

		return( -1 );
	}
	if( descriptor_data_cache == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptor data cache.",
		 function );

		return( -1 );
	}
	if( libpff_descriptor_data_stream_data_handle_initialize(
	     &data_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data handle.",
		 function );

		goto on_error;
	}
	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing data handle.",
		 function );

		goto on_error;
	}
	if( libfdata_stream_initialize(
	     descriptor_data_stream,
	     (intptr_t *) data_handle,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_descriptor_data_stream_data_handle_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libpff_descriptor_data_stream_data_handle_clone,
	     NULL,
	     (ssize_t (*)(intptr_t *, intptr_t *, int, int, uint8_t *, size_t, uint32_t, uint8_t, libcerror_error_t **)) &libpff_descriptor_data_stream_data_handle_read_segment_data,
	     NULL,
	     (off64_t (*)(intptr_t *, intptr_t *, int, int, off64_t, libcerror_error_t **)) &libpff_descriptor_data_stream_data_handle_seek_segment_offset,
	     LIBFDATA_DATA_HANDLE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create stream.",
		 function );

		goto on_error;
	}
	if( libfdata_list_get_number_of_elements(
	     descriptor_data_list,
	     &number_of_elements,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of descriptor data list elements.",
		 function );

		data_handle = NULL;

		goto on_error;
	}
	for( element_index = 0;
	     element_index < number_of_elements;
	     element_index++ )
	{
		if( libfdata_list_get_mapped_size_by_index(
		     descriptor_data_list,
		     element_index,
		     &segment_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve descriptor data list element: %d mapped size.",
			 function,
			 element_index );

			data_handle = NULL;

			goto on_error;
		}
		if( libfdata_stream_append_segment(
		     *descriptor_data_stream,
		     &segment_index,
		     0,
		     segment_offset,
		     segment_size,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append descriptor data stream segment: %d.",
			 function,
			 element_index );

			data_handle = NULL;

			goto on_error;
		}
		segment_offset += segment_size;
	}
	data_handle->descriptor_data_list  = descriptor_data_list;
	data_handle->descriptor_data_cache = descriptor_data_cache;
	data_handle->flags                 = flags;

	return( 1 );

on_error:
	if( *descriptor_data_stream != NULL )
	{
		libfdata_stream_free(
		 descriptor_data_stream,
		 NULL );
	}
	if( data_handle != NULL )
	{
		libpff_descriptor_data_stream_data_handle_free(
		 &data_handle,
		 NULL );
	}
	return( -1 );
}

