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
#include "libpff_definitions.h"
#include "libpff_index.h"
#include "libpff_index_node.h"
#include "libpff_index_tree.h"
#include "libpff_index_value.h"
#include "libpff_item_descriptor.h"
#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcdata.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_libfmapi.h"
#include "libpff_local_descriptor_node.h"
#include "libpff_unused.h"

#include "pff_file_header.h"

const uint8_t pff_file_signature[ 4 ] = { 0x21, 0x42, 0x44, 0x4e };

/* Creates an IO handle
 * Make sure the value io_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_io_handle_initialize(
     libpff_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libpff_io_handle_initialize";

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
	if( *io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid IO handle value already set.",
		 function );

		return( -1 );
	}
	*io_handle = memory_allocate_structure(
	              libpff_io_handle_t );

	if( *io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *io_handle,
	     0,
	     sizeof( libpff_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file.",
		 function );

		goto on_error;
	}
	( *io_handle )->ascii_codepage = LIBPFF_CODEPAGE_WINDOWS_1252;

	return( 1 );

on_error:
	if( *io_handle != NULL )
	{
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( -1 );
}

/* Frees an IO handle
 * Returns 1 if successful or -1 on error
 */
int libpff_io_handle_free(
     libpff_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libpff_io_handle_free";
	int result            = 1;

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
	if( *io_handle != NULL )
	{
		if( libpff_io_handle_clear(
		     *io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to clear IO handle.",
			 function );

			result = -1;
		}
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( result );
}

/* Clears the IO handle
 * Returns 1 if successful or -1 on error
 */
int libpff_io_handle_clear(
     libpff_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function = "libpff_io_handle_clear";
	int result            = 1;

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
	if( io_handle->index_nodes_vector != NULL )
	{
		if( libfdata_vector_free(
		     &( io_handle->index_nodes_vector ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free index nodes vector.",
			 function );

			result = -1;
		}
	}
	if( io_handle->index_nodes_cache != NULL )
	{
		if( libfcache_cache_free(
		     &( io_handle->index_nodes_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free index nodes cache.",
			 function );

			result = -1;
		}
	}
	if( memory_set(
	     io_handle,
	     0,
	     sizeof( libpff_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		result = -1;
	}
	return( result );
}

/* Reads the file header
 * Returns 1 if successful or -1 on error
 */
int libpff_io_handle_read_file_header(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     int *file_content_type,
     off64_t *descriptors_index_root_node_offset,
     uint64_t *descriptors_index_root_node_back_pointer,
     off64_t *offsets_index_root_node_offset,
     uint64_t *offsets_index_root_node_back_pointer,
     libcerror_error_t **error )
{
	uint8_t *file_header         = NULL;
	uint8_t *file_header_data    = NULL;
	static char *function        = "libpff_io_handle_read_file_header";
	size_t page_size             = 0;
	size_t read_size             = 564;
	ssize_t read_count           = 0;
	uint32_t calculated_checksum = 0;
	uint32_t stored_checksum     = 0;
	uint16_t content_type        = 0;
	uint16_t data_version        = 0;
	int segment_index            = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t *value_data          = 0;
	size_t data_size             = 0;
	uint64_t value_64bit         = 0;
	uint32_t value_32bit         = 0;
	uint16_t value_16bit         = 0;
	uint8_t sentinal             = 0;
	int value_iterator           = 0;
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
	if( io_handle->index_nodes_vector != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid IO handle - index nodes vector value already set.",
		 function );

		return( -1 );
	}
	if( io_handle->index_nodes_cache != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid IO handle - index nodes cache value already set.",
		 function );

		return( -1 );
	}
	if( file_content_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file content type.",
		 function );

		return( -1 );
	}
	if( descriptors_index_root_node_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptors index root node offset.",
		 function );

		return( -1 );
	}
	if( descriptors_index_root_node_back_pointer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid descriptors index root node back pointer.",
		 function );

		return( -1 );
	}
	if( offsets_index_root_node_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offsets index root node offset.",
		 function );

		return( -1 );
	}
	if( offsets_index_root_node_back_pointer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offsets index root node back pointer.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading file header at offset: 0 (0x00000000)\n",
		 function );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     0,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek file header offset: 0.",
		 function );

		goto on_error;
	}
	file_header = (uint8_t *) memory_allocate(
	                           sizeof( uint8_t ) * read_size );

	if( file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file header.",
		 function );

		goto on_error;
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              file_header,
	              read_size,
	              error );

	if( read_count != (ssize_t) read_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header data.",
		 function );

		goto on_error;
	}
	file_header_data = &( file_header[ sizeof( pff_file_header_t ) ] );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: file header:\n",
		 function );
		libcnotify_print_data(
		 file_header,
		 sizeof( pff_file_header_t ),
		 0 );
	}
#endif
	if( memory_compare(
	     ( (pff_file_header_t *) file_header )->signature,
	     pff_file_signature,
	     4 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid file signature.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (pff_file_header_t *) file_header )->checksum,
	 stored_checksum );

	byte_stream_copy_to_uint16_little_endian(
	 ( (pff_file_header_t *) file_header )->content_type,
	 content_type );

	if( content_type == PFF_FILE_HEADER_CONTENT_TYPE_PAB )
	{
		*file_content_type = LIBPFF_FILE_CONTENT_TYPE_PAB;
	}
	else if( content_type == PFF_FILE_HEADER_CONTENT_TYPE_PST )
	{
		*file_content_type = LIBPFF_FILE_CONTENT_TYPE_PST;
	}
	else if( content_type == PFF_FILE_HEADER_CONTENT_TYPE_OST )
	{
		*file_content_type = LIBPFF_FILE_CONTENT_TYPE_OST;
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported content type: 0x%04" PRIx16 ".",
		 function,
		 content_type );

		goto on_error;
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (pff_file_header_t *) file_header )->data_version,
	 data_version );

	if( data_version <= 0x000f )
	{
		io_handle->file_type = LIBPFF_FILE_TYPE_32BIT;
	}
	else if( data_version >= 0x0024 )
	{
		io_handle->file_type = LIBPFF_FILE_TYPE_64BIT_4K_PAGE;
	}
	else if( data_version >= 0x0015 )
	{
		io_handle->file_type = LIBPFF_FILE_TYPE_64BIT;
	}
	else
	{
		if( ( ( (pff_file_header_data_32bit_t *) file_header_data )->sentinal == 0x80 )
		 && ( ( (pff_file_header_data_64bit_t *) file_header_data )->sentinal != 0x80 ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: detected 32-bit file type for unsupported data version: 0x%04" PRIx16 ".\n",
				 function,
				 data_version );
			}
#endif
			io_handle->file_type = LIBPFF_FILE_TYPE_32BIT;
		}
		else if( ( ( (pff_file_header_data_32bit_t *) file_header_data )->sentinal != 0x80 )
		      && ( ( (pff_file_header_data_64bit_t *) file_header_data )->sentinal == 0x80 ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: detected 64-bit file type for unsupported data version: 0x%04" PRIx16 ".\n",
				 function,
				 data_version );
			}
#endif
			io_handle->file_type = LIBPFF_FILE_TYPE_64BIT;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		else if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unsupported data version: 0x%04" PRIx16 ".\n",
			 function,
			 data_version );
		}
#endif
	}
	if( ( io_handle->file_type != LIBPFF_FILE_TYPE_32BIT )
	 && ( io_handle->file_type != LIBPFF_FILE_TYPE_64BIT )
	 && ( io_handle->file_type != LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unable to determine file type.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t: %c%c%c%c\n",
		 function,
		 ( (pff_file_header_t *) file_header )->signature[ 0 ],
		 ( (pff_file_header_t *) file_header )->signature[ 1 ],
		 ( (pff_file_header_t *) file_header )->signature[ 2 ],
		 ( (pff_file_header_t *) file_header )->signature[ 3 ] );

		libcnotify_printf(
		 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libcnotify_printf(
		 "%s: content type\t\t\t\t: %c%c\n",
		 function,
		 ( (pff_file_header_t *) file_header )->content_type[ 0 ],
		 ( (pff_file_header_t *) file_header )->content_type[ 1 ] );

		libcnotify_printf(
		 "%s: data version\t\t\t\t: %" PRIu16 "\n",
		 function,
		 data_version );
		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_file_header_t *) file_header )->content_version,
		 value_16bit );

		libcnotify_printf(
		 "%s: content version\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: creation platform\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (pff_file_header_t *) file_header )->creation_platform );

		libcnotify_printf(
		 "%s: access platform\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (pff_file_header_t *) file_header )->access_platform );

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_file_header_t *) file_header )->unknown1,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
		 function,
		 value_32bit,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_file_header_t *) file_header )->unknown2,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
		 function,
		 value_32bit,
		 value_32bit );

		libcnotify_printf(
		 "\n" );
	}
#endif
	if( libfmapi_checksum_calculate_weak_crc32(
	     &calculated_checksum,
	     &( file_header[ 8 ] ),
	     471,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to calculate weak CRC-32.",
		 function );

		goto on_error;
	}
	if( stored_checksum != calculated_checksum )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
		 "%s: mismatch in file header checksum ( %" PRIu32 " != %" PRIu32 " ).",
		 function,
		 stored_checksum,
		 calculated_checksum );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			data_size = sizeof( pff_file_header_data_32bit_t );
		}
		else if( ( io_handle->file_type = LIBPFF_FILE_TYPE_64BIT )
		      || ( io_handle->file_type = LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
		{
			data_size = sizeof( pff_file_header_data_64bit_t );
		}
		else
		{
			data_size = 564;
		}
		libcnotify_printf(
		 "%s: file header data:\n",
		 function );
		libcnotify_print_data(
		 file_header_data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_file_header_data_32bit_t *) file_header_data )->file_size,
		 io_handle->file_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_file_header_data_32bit_t *) file_header_data )->descriptors_index_back_pointer,
		 *descriptors_index_root_node_back_pointer );

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_file_header_data_32bit_t *) file_header_data )->descriptors_index_root_node_offset,
		 *descriptors_index_root_node_offset );

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_file_header_data_32bit_t *) file_header_data )->offsets_index_back_pointer,
		 *offsets_index_root_node_back_pointer );

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_file_header_data_32bit_t *) file_header_data )->offsets_index_root_node_offset,
		 *offsets_index_root_node_offset );

#if defined( HAVE_DEBUG_OUTPUT )
		sentinal                   = ( (pff_file_header_data_32bit_t *) file_header_data )->sentinal;
#endif
		io_handle->encryption_type = ( (pff_file_header_data_32bit_t *) file_header_data )->encryption_type;
	}
	else if( ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
	      || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (pff_file_header_data_64bit_t *) file_header_data )->file_size,
		 io_handle->file_size );

		byte_stream_copy_to_uint64_little_endian(
		 ( (pff_file_header_data_64bit_t *) file_header_data )->descriptors_index_back_pointer,
		 *descriptors_index_root_node_back_pointer );

		byte_stream_copy_to_uint64_little_endian(
		 ( (pff_file_header_data_64bit_t *) file_header_data )->descriptors_index_root_node_offset,
		 *descriptors_index_root_node_offset );

		byte_stream_copy_to_uint64_little_endian(
		 ( (pff_file_header_data_64bit_t *) file_header_data )->offsets_index_back_pointer,
		 *offsets_index_root_node_back_pointer );

		byte_stream_copy_to_uint64_little_endian(
		 ( (pff_file_header_data_64bit_t *) file_header_data )->offsets_index_root_node_offset,
		 *offsets_index_root_node_offset );

#if defined( HAVE_DEBUG_OUTPUT )
		sentinal                   = ( (pff_file_header_data_64bit_t *) file_header_data )->sentinal;
#endif
		io_handle->encryption_type = ( (pff_file_header_data_64bit_t *) file_header_data )->encryption_type;

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_file_header_data_64bit_t *) file_header_data )->checksum,
		 stored_checksum );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->next_index_pointer,
			 value_32bit );
			libcnotify_printf(
			 "%s: next index pointer\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->next_index_back_pointer,
			 value_32bit );
			libcnotify_printf(
			 "%s: next index back pointer\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->seed_value,
			 value_32bit );
			libcnotify_printf(
			 "%s: seed value\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			value_data = ( (pff_file_header_data_32bit_t *) file_header_data )->descriptors_index_high_water_marks;

			for( value_iterator = 0;
			     value_iterator < 32;
			     value_iterator++ )
			{
				byte_stream_copy_to_uint32_little_endian(
				 value_data,
				 value_32bit );
				libcnotify_printf(
				 "%s: high water mark: %02d value\t\t: %" PRIu32 "\n",
				 function,
				 value_iterator,
				 value_32bit );

				value_data += 4;
			}
			libcnotify_printf(
			 "\n" );
		}
		else if( ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
		      || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
		{
			libcnotify_printf(
			 "%s: unknown3:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->unknown3,
			 8,
			 0 );

			byte_stream_copy_to_uint64_little_endian(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->next_index_back_pointer,
			 value_64bit );
			libcnotify_printf(
			 "%s: next index back pointer\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->seed_value,
			 value_32bit );
			libcnotify_printf(
			 "%s: seed value\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			value_data = ( (pff_file_header_data_64bit_t *) file_header_data )->descriptors_index_high_water_marks;

			for( value_iterator = 0;
			     value_iterator < 32;
			     value_iterator++ )
			{
				byte_stream_copy_to_uint32_little_endian(
				 value_data,
				 value_32bit );
				libcnotify_printf(
				 "%s: high water mark: %02d value\t\t: %" PRIu32 "\n",
				 function,
				 value_iterator,
				 value_32bit );

				value_data += 4;
			}
			libcnotify_printf(
			 "\n" );
			libcnotify_printf(
			 "%s: unknown4:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->unknown4,
			 8,
			 0 );
		}
		libcnotify_printf(
		 "%s: file header data root:\n",
		 function );

		if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			libcnotify_printf(
			 "%s: unknown5:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->unknown5,
			 4,
			 0 );
		}
		else if( ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
		      || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
		{
			libcnotify_printf(
			 "%s: unknown5:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->unknown5,
			 4,
			 0 );
		}
		libcnotify_printf(
		 "%s: file size\t\t\t\t: %" PRIu64 "\n",
		 function,
		 io_handle->file_size );

		if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->last_data_allocation_table_offset,
			 value_32bit );
			libcnotify_printf(
			 "%s: last data allocation table offset\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->total_available_data_size,
			 value_32bit );
			libcnotify_printf(
			 "%s: total available data size\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->total_available_page_size,
			 value_32bit );
			libcnotify_printf(
			 "%s: total available page size\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );
		}
		else if( ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
		      || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
		{
			byte_stream_copy_to_uint64_little_endian(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->last_data_allocation_table_offset,
			 value_64bit );
			libcnotify_printf(
			 "%s: last data allocation table offset\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->total_available_data_size,
			 value_64bit );
			libcnotify_printf(
			 "%s: total available data size\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->total_available_page_size,
			 value_64bit );
			libcnotify_printf(
			 "%s: total available page size\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );
		}
		libcnotify_printf(
		 "%s: descriptors index back pointer\t: %" PRIu64 "\n",
		 function,
		 *descriptors_index_root_node_back_pointer );

		libcnotify_printf(
		 "%s: descriptors index root node offset\t: %" PRIi64 "\n",
		 function,
		 *descriptors_index_root_node_offset );

		libcnotify_printf(
		 "%s: offsets index back pointer\t\t: %" PRIu64 "\n",
		 function,
		 *offsets_index_root_node_back_pointer );

		libcnotify_printf(
		 "%s: offsets index root node offset\t: %" PRIi64 "\n",
		 function,
		 *offsets_index_root_node_offset );

		if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			libcnotify_printf(
			 "%s: allocation table validation type\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (pff_file_header_data_32bit_t *) file_header_data )->allocation_table_validation_type );

			libcnotify_printf(
			 "%s: unknown6:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->unknown6,
			 3,
			 0 );

			libcnotify_printf(
			 "%s: initial data free map:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->initial_data_free_map,
			 128,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

			libcnotify_printf(
			 "%s: initial page free map:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->initial_page_free_map,
			 128,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
		else if( ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
		      || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
		{
			libcnotify_printf(
			 "%s: allocation table validation type\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (pff_file_header_data_64bit_t *) file_header_data )->allocation_table_validation_type );

			libcnotify_printf(
			 "%s: unknown6:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->unknown6,
			 3,
			 0 );

			libcnotify_printf(
			 "%s: unknown7:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->unknown7,
			 3,
			 0 );

			libcnotify_printf(
			 "%s: initial data free map:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->initial_data_free_map,
			 128,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

			libcnotify_printf(
			 "%s: initial page free map:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->initial_page_free_map,
			 128,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
		libcnotify_printf(
		 "%s: sentinal\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 sentinal );

		libcnotify_printf(
		 "%s: encryption type\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 io_handle->encryption_type );

		if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			libcnotify_printf(
			 "%s: unknown8:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->unknown8,
			 2,
			 0 );

			libcnotify_printf(
			 "%s: unknown9:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->unknown9,
			 8,
			 0 );

			libcnotify_printf(
			 "%s: unknown10:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->unknown10,
			 4,
			 0 );

			libcnotify_printf(
			 "%s: unknown11:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->unknown11,
			 3,
			 0 );

			libcnotify_printf(
			 "%s: unknown12:\n",
			 function );
			libcnotify_print_data(
			 &( ( (pff_file_header_data_32bit_t *) file_header_data )->unknown12 ),
			 1,
			 0 );

			libcnotify_printf(
			 "%s: unknown13:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->unknown13,
			 32,
			 0 );
		}
		else if( ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
		      || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
		{
			libcnotify_printf(
			 "%s: unknown8:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->unknown8,
			 2,
			 0 );

			byte_stream_copy_to_uint64_little_endian(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->next_index_pointer,
			 value_32bit );
			libcnotify_printf(
			 "%s: next index pointer\t\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			libcnotify_printf(
			 "%s: checksum\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 stored_checksum );

			libcnotify_printf(
			 "%s: unknown11:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->unknown11,
			 3,
			 0 );

			libcnotify_printf(
			 "%s: unknown12:\n",
			 function );
			libcnotify_print_data(
			 &( ( (pff_file_header_data_32bit_t *) file_header_data )->unknown12 ),
			 1,
			 0 );

			libcnotify_printf(
			 "%s: unknown13:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->unknown13,
			 32,
			 0 );
		}
	}
#endif
	if( ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
	 || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
	{
		if( libfmapi_checksum_calculate_weak_crc32(
		     &calculated_checksum,
		     &( file_header[ 8 ] ),
		     516,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to calculate weak CRC-32.",
			 function );

			goto on_error;
		}
		if( stored_checksum != calculated_checksum )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
			 "%s: mismatch in file header checksum ( %" PRIu32 " != %" PRIu32 " ).",
			 function,
			 stored_checksum,
			 calculated_checksum );

			goto on_error;
		}
	}
	memory_free(
	 file_header );

	file_header = NULL;

	if( ( io_handle->encryption_type != LIBPFF_ENCRYPTION_TYPE_NONE )
	 && ( io_handle->encryption_type != LIBPFF_ENCRYPTION_TYPE_COMPRESSIBLE )
	 && ( io_handle->encryption_type != LIBPFF_ENCRYPTION_TYPE_HIGH ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported encryption type: 0x%02x",
		 function,
		 io_handle->encryption_type );

		goto on_error;
	}
	if( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE )
	{
		page_size = 4096;
	}
	else
	{
		page_size = 512;
	}
/* TODO free and clone function ? */
	if( libfdata_vector_initialize(
	     &( io_handle->index_nodes_vector ),
	     page_size,
	     (intptr_t *) io_handle,
	     NULL,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_vector_t *, libfcache_cache_t *, int, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libpff_io_handle_read_index_node,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create index nodes vector.",
		 function );

		goto on_error;
	}
	if( libfdata_vector_append_segment(
	     io_handle->index_nodes_vector,
	     &segment_index,
	     0,
	     0,
	     io_handle->file_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to create append segment to nodes vector.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( io_handle->index_nodes_cache ),
	     LIBPFF_MAXIMUM_CACHE_ENTRIES_INDEX_NODES,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create index nodes cache.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( file_header != NULL )
	{
		memory_free(
		 file_header );
	}
	return( -1 );
}

/* Reads the unallocated data blocks
 * Returns 1 if successful or -1 on error
 */
int libpff_io_handle_read_unallocated_data_blocks(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcdata_range_list_t *unallocated_data_block_list,
     libcerror_error_t **error )
{
	static char *function           = "libpff_io_handle_read_unallocated_data_blocks";
	off64_t allocation_table_offset = 0;
	size64_t allocation_block_size  = 0;

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
		allocation_table_offset = 0x4400;
		allocation_block_size   = 496 * 8 * 64;
	}
	else
	{
		allocation_table_offset = 0x22000;
		allocation_block_size   = 4072 * 8 * 512;
	}
	while( allocation_table_offset < (off64_t) io_handle->file_size )
	{
		if( libpff_allocation_table_read(
		     unallocated_data_block_list,
		     file_io_handle,
		     allocation_table_offset,
		     (int) io_handle->file_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read allocation table at offset: %" PRIi64 ".",
			 function,
			 allocation_table_offset );

			return( -1 );
		}
		allocation_table_offset += allocation_block_size;
	}
	return( 1 );
}

/* Reads the unallocated page blocks
 * Returns 1 if successful or -1 on error
 */
int libpff_io_handle_read_unallocated_page_blocks(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcdata_range_list_t *unallocated_page_block_list,
     libcerror_error_t **error )
{
	static char *function           = "libpff_io_handle_read_unallocated_page_blocks";
	off64_t allocation_table_offset = 0;
	size64_t allocation_block_size  = 0;

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
	 && ( io_handle->file_type != LIBPFF_FILE_TYPE_64BIT ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file type.",
		 function );

		return( -1 );
	}
	allocation_table_offset = 0x4600;
	allocation_block_size   = 496 * 8 * 512;

	while( allocation_table_offset < (off64_t) io_handle->file_size )
	{
		if( libpff_allocation_table_read(
		     unallocated_page_block_list,
		     file_io_handle,
		     allocation_table_offset,
		     (int) io_handle->file_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read allocation table at offset: %" PRIi64 ".",
			 function,
			 allocation_table_offset );

			return( -1 );
		}
		allocation_table_offset += allocation_block_size;
	}
	return( 1 );
}

/* Reads an index node
 * Returns 1 if successful or -1 on error
 */
int libpff_io_handle_read_index_node(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfcache_cache_t *cache,
     int element_index,
     int element_data_file_index LIBPFF_ATTRIBUTE_UNUSED,
     off64_t element_data_offset,
     size64_t element_data_size LIBPFF_ATTRIBUTE_UNUSED,
     uint32_t element_data_flags LIBPFF_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBPFF_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libpff_index_node_t *index_node = NULL;
	static char *function           = "libpff_io_handle_read_index_node";

	LIBPFF_UNREFERENCED_PARAMETER( element_data_file_index )
	LIBPFF_UNREFERENCED_PARAMETER( element_data_size )
	LIBPFF_UNREFERENCED_PARAMETER( element_data_flags )
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
	/* TODO check if element data size matches index node size
	 * remove corresponding LIBPFF_ATTRIBUTE_UNUSED, LIBPFF_UNREFERENCED_PARAMETER
	 */
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
	if( libpff_index_node_read(
	     index_node,
	     io_handle,
	     file_io_handle,
	     element_data_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read index node at offset: %" PRIi64 ".",
		 function,
		 element_data_offset );

		goto on_error;
	}
	if( libfdata_vector_set_element_value_by_index(
	     vector,
	     (intptr_t *) file_io_handle,
	     cache,
	     element_index,
	     (intptr_t *) index_node,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_index_node_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set index node as element value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( index_node != NULL )
	{
		libpff_index_node_free(
		 &index_node,
		 NULL );
	}
	return( -1 );
}

