/*
 * Input/Output (IO) handle functions
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

#include "libpff_allocation_table.h"
#include "libpff_codepage.h"
#include "libpff_data_array.h"
#include "libpff_data_block.h"
#include "libpff_debug.h"
#include "libpff_definitions.h"
#include "libpff_descriptors_index.h"
#include "libpff_index.h"
#include "libpff_index_node.h"
#include "libpff_index_tree.h"
#include "libpff_index_value.h"
#include "libpff_item_descriptor.h"
#include "libpff_item_tree.h"
#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcdata.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_libfmapi.h"
#include "libpff_local_descriptor_node.h"
#include "libpff_local_descriptor_value.h"
#include "libpff_local_descriptors.h"
#include "libpff_local_descriptors_tree.h"
#include "libpff_offsets_index.h"
#include "libpff_unused.h"

#include "pff_block.h"
#include "pff_file_header.h"
#include "pff_index_node.h"
#include "pff_local_descriptor_node.h"

/* Reads the data list of a descriptor
 * Returns 1 if successful or -1 on error
 */
int libpff_io_handle_read_descriptor_data_list(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     uint32_t descriptor_identifier,
     uint64_t data_identifier,
     uint8_t recovered,
     int recovered_value_index,
     libfdata_list_t **descriptor_data_list,
     libfcache_cache_t **descriptor_data_cache,
     libcerror_error_t **error )
{
	libpff_data_array_t *data_array          = NULL;
	libpff_data_block_t *data_block          = NULL;
	libpff_index_value_t *offset_index_value = NULL;
	static char *function                    = "libpff_io_handle_read_descriptor_data_list";
	int element_index                        = 0;

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
	if( descriptor_data_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid descriptor data list.",
		 function );

		return( -1 );
	}
	if( *descriptor_data_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: descriptor data list already set.",
		 function );

		return( -1 );
	}
	if( descriptor_data_cache == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid descriptor data cache.",
		 function );

		return( -1 );
	}
	if( *descriptor_data_cache != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: descriptor data cache already set.",
		 function );

		return( -1 );
	}
	if( libpff_offsets_index_get_index_value_by_identifier(
	     offsets_index,
	     file_io_handle,
	     data_identifier,
	     recovered,
	     recovered_value_index,
	     &offset_index_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to find offset index value identifier: %" PRIu64 ".",
		 function,
		 data_identifier );

		goto on_error;
	}
	if( offset_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid offset index value.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: identifier: %" PRIu64 " (%s) at offset: %" PRIi64 " of size: %" PRIu32 "\n",
		 function,
		 offset_index_value->identifier,
		 ( ( offset_index_value->identifier & LIBPFF_OFFSET_INDEX_IDENTIFIER_FLAG_INTERNAL ) ? "internal" : "external" ),
		 offset_index_value->file_offset,
		 offset_index_value->data_size );
	}
#endif
	if( offset_index_value->file_offset <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid offset index value - file offset value out of bounds.",
		 function );

		goto on_error;
	}
	if( offset_index_value->data_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid offset index value - data size value value out of bounds.",
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
		 "%s: invalid offset index value - data size value exceeds maximum.",
		 function );

		goto on_error;
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading data block at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 offset_index_value->file_offset,
		 offset_index_value->file_offset );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     offset_index_value->file_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek data block offset: %" PRIi64 ".",
		 function,
		 offset_index_value->file_offset );

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
	     offset_index_value->data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read data block at offset: %" PRIi64 ".",
		 function,
		 offset_index_value->file_offset );

		goto on_error;
	}
	/* Check if the data block contains a data array
	 * The data array should have the internal flag set in the (data) offset index identifier
	 * The data array starts with 0x01 followed by either 0x01 or 0x02
	 */
	if( ( ( data_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_FLAG_INTERNAL ) != 0 )
	 && ( ( data_block->data[ 0 ] == 0x01 )
	  &&  ( ( data_block->data[ 1 ] == 0x01 )
	   ||   ( data_block->data[ 1 ] == 0x02 ) ) ) )
	{
		if( libpff_data_array_initialize(
		     &data_array,
		     descriptor_identifier,
		     data_identifier,
		     io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create data array.",
			 function );

			goto on_error;
		}
		if( libfdata_list_initialize(
		     descriptor_data_list,
		     (intptr_t *) data_array,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_data_array_free,
		     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libpff_data_array_clone,
		     (int (*)(intptr_t *, intptr_t *, libfdata_list_element_t *, libfcache_cache_t *, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libpff_data_array_read_element_data,
		     NULL,
		     LIBFDATA_DATA_HANDLE_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create descriptor data list.",
			 function );

			goto on_error;
		}
		/* The data_array is now managed by the list */

		if( libpff_data_array_read(
		     data_array,
		     io_handle,
		     file_io_handle,
		     offsets_index,
		     *descriptor_data_list,
		     recovered,
		     data_block->data,
		     (size_t) offset_index_value->data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read data array.",
			 function );

			data_array = NULL;

			goto on_error;
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

			data_array = NULL;

			goto on_error;
		}
		if( libfcache_cache_initialize(
		     descriptor_data_cache,
		     LIBPFF_MAXIMUM_CACHE_ENTRIES_DATA_ARRAY,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create descriptor data cache.",
			 function );

			data_array = NULL;

			goto on_error;
		}
	}
	else
	{
		if( libpff_data_block_decrypt_data(
		     data_block,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBCERROR_ENCRYPTION_ERROR_DECRYPT_FAILED,
			 "%s: unable to decrypt data block data.",
			 function );

			goto on_error;
		}
/* TODO change data block not be a data handle ? pass a descriptor instead ? */
		if( libfdata_list_initialize(
		     descriptor_data_list,
		     (intptr_t *) data_block,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_data_block_free,
		     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libpff_data_block_clone,
		     (int (*)(intptr_t *, intptr_t *, libfdata_list_element_t *, libfcache_cache_t *, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libpff_data_block_read_element_data,
		     NULL,
		     LIBFDATA_DATA_HANDLE_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create descriptor data list.",
			 function );

			goto on_error;
		}
		/* The data_block is now managed by the list */

		if( libfdata_list_append_element_with_mapped_size(
		     *descriptor_data_list,
		     &element_index,
		     0,
		     offset_index_value->file_offset,
		     (size64_t) offset_index_value->data_size,
		     0,
		     (size_t) data_block->uncompressed_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to append data list element.",
			 function );

			data_block = NULL;

			goto on_error;
		}
		if( libfcache_cache_initialize(
		     descriptor_data_cache,
		     LIBPFF_MAXIMUM_CACHE_ENTRIES_DATA_BLOCK,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create descriptor data cache.",
			 function );

			data_block = NULL;

			goto on_error;
		}
		/* The data block is managed by the list and should not be managed by the cache as well
		 */
		if( libfdata_list_set_element_value_by_index(
		     *descriptor_data_list,
		     (intptr_t *) file_io_handle,
		     *descriptor_data_cache,
		     0,
		     (intptr_t *) data_block,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_data_block_free,
		     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_NON_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set data list element: 0.",
			 function );

			data_block = NULL;

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( *descriptor_data_cache != NULL )
	{
		libfcache_cache_free(
		 descriptor_data_cache,
		 NULL );
	}
	if( *descriptor_data_list != NULL )
	{
		libfdata_list_free(
		 descriptor_data_list,
		 NULL );
	}
	if( data_array != NULL )
	{
		libpff_data_array_free(
		 &data_array,
		 NULL );
	}
	if( data_block != NULL )
	{
		libpff_data_block_free(
		 &data_block,
		 NULL );
	}
	return( -1 );
}

