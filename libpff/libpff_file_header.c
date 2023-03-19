/*
 * File header functions
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
#include "libpff_debug.h"
#include "libpff_definitions.h"
#include "libpff_file_header.h"
#include "libpff_io_handle.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"

#include "pff_file_header.h"

/* Creates file header
 * Make sure the value file_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_file_header_initialize(
     libpff_file_header_t **file_header,
     libcerror_error_t **error )
{
	static char *function = "libpff_file_header_initialize";

	if( file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file header.",
		 function );

		return( -1 );
	}
	if( *file_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file header value already set.",
		 function );

		return( -1 );
	}
	*file_header = memory_allocate_structure(
	                libpff_file_header_t );

	if( *file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *file_header,
	     0,
	     sizeof( libpff_file_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *file_header != NULL )
	{
		memory_free(
		 *file_header );

		*file_header = NULL;
	}
	return( -1 );
}

/* Frees file header
 * Returns 1 if successful or -1 on error
 */
int libpff_file_header_free(
     libpff_file_header_t **file_header,
     libcerror_error_t **error )
{
	static char *function = "libpff_file_header_free";

	if( file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file header.",
		 function );

		return( -1 );
	}
	if( *file_header != NULL )
	{
		memory_free(
		 *file_header );

		*file_header = NULL;
	}
	return( 1 );
}

/* Reads the file header data
 * Returns 1 if successful or -1 on error
 */
int libpff_file_header_read_data(
     libpff_file_header_t *file_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	const uint8_t *file_header_data                  = NULL;
	static char *function                            = "libpff_file_header_read_data";
	uint64_t safe_descriptors_index_root_node_offset = 0;
	uint64_t safe_offsets_index_root_node_offset     = 0;
	uint32_t calculated_checksum                     = 0;
	uint32_t stored_checksum                         = 0;
	uint16_t content_type                            = 0;
	uint16_t data_version                            = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	const uint8_t *value_data                        = 0;
	uint64_t value_64bit                             = 0;
	uint32_t value_32bit                             = 0;
	uint16_t value_16bit                             = 0;
	uint8_t sentinel                                 = 0;
	int value_iterator                               = 0;
#endif

	if( file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file header.",
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
	if( ( data_size < 564 )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	file_header_data = &( data[ sizeof( pff_file_header_t ) ] );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: file header:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( pff_file_header_t ),
		 0 );
	}
#endif
	if( memory_compare(
	     ( (pff_file_header_t *) data )->signature,
	     pff_file_signature,
	     4 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid file signature.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (pff_file_header_t *) data )->checksum,
	 stored_checksum );

	byte_stream_copy_to_uint16_little_endian(
	 ( (pff_file_header_t *) data )->content_type,
	 content_type );

	if( content_type == PFF_FILE_HEADER_CONTENT_TYPE_PAB )
	{
		file_header->file_content_type = LIBPFF_FILE_CONTENT_TYPE_PAB;
	}
	else if( content_type == PFF_FILE_HEADER_CONTENT_TYPE_PST )
	{
		file_header->file_content_type = LIBPFF_FILE_CONTENT_TYPE_PST;
	}
	else if( content_type == PFF_FILE_HEADER_CONTENT_TYPE_OST )
	{
		file_header->file_content_type = LIBPFF_FILE_CONTENT_TYPE_OST;
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

		return( -1 );
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (pff_file_header_t *) data )->data_version,
	 data_version );

	if( data_version <= 0x000f )
	{
		file_header->file_type = LIBPFF_FILE_TYPE_32BIT;
	}
	else if( data_version >= 0x0024 )
	{
		file_header->file_type = LIBPFF_FILE_TYPE_64BIT_4K_PAGE;
	}
	else if( data_version >= 0x0015 )
	{
		file_header->file_type = LIBPFF_FILE_TYPE_64BIT;
	}
	else
	{
		if( ( ( (pff_file_header_data_32bit_t *) file_header_data )->sentinel == 0x80 )
		 && ( ( (pff_file_header_data_64bit_t *) file_header_data )->sentinel != 0x80 ) )
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
			file_header->file_type = LIBPFF_FILE_TYPE_32BIT;
		}
		else if( ( ( (pff_file_header_data_32bit_t *) file_header_data )->sentinel != 0x80 )
		      && ( ( (pff_file_header_data_64bit_t *) file_header_data )->sentinel == 0x80 ) )
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
			file_header->file_type = LIBPFF_FILE_TYPE_64BIT;
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
	if( ( file_header->file_type != LIBPFF_FILE_TYPE_32BIT )
	 && ( file_header->file_type != LIBPFF_FILE_TYPE_64BIT )
	 && ( file_header->file_type != LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unable to determine file type.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t\t: %c%c%c%c\n",
		 function,
		 ( (pff_file_header_t *) data )->signature[ 0 ],
		 ( (pff_file_header_t *) data )->signature[ 1 ],
		 ( (pff_file_header_t *) data )->signature[ 2 ],
		 ( (pff_file_header_t *) data )->signature[ 3 ] );

		libcnotify_printf(
		 "%s: checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libcnotify_printf(
		 "%s: content type\t\t\t\t: %c%c\n",
		 function,
		 ( (pff_file_header_t *) data )->content_type[ 0 ],
		 ( (pff_file_header_t *) data )->content_type[ 1 ] );

		libcnotify_printf(
		 "%s: data version\t\t\t\t: %" PRIu16 "\n",
		 function,
		 data_version );
		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_file_header_t *) data )->content_version,
		 value_16bit );

		libcnotify_printf(
		 "%s: content version\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: creation platform\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (pff_file_header_t *) data )->creation_platform );

		libcnotify_printf(
		 "%s: access platform\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (pff_file_header_t *) data )->access_platform );

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_file_header_t *) data )->unknown1,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
		 function,
		 value_32bit,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_file_header_t *) data )->unknown2,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
		 function,
		 value_32bit,
		 value_32bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( libpff_checksum_calculate_weak_crc32(
	     &calculated_checksum,
	     &( data[ 8 ] ),
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

		return( -1 );
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

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( file_header->file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			data_size = sizeof( pff_file_header_data_32bit_t );
		}
		else if( ( file_header->file_type == LIBPFF_FILE_TYPE_64BIT )
		      || ( file_header->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
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
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( file_header->file_type == LIBPFF_FILE_TYPE_32BIT )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_file_header_data_32bit_t *) file_header_data )->file_size,
		 file_header->file_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_file_header_data_32bit_t *) file_header_data )->descriptors_index_back_pointer,
		 file_header->descriptors_index_root_node_back_pointer );

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_file_header_data_32bit_t *) file_header_data )->descriptors_index_root_node_offset,
		 safe_descriptors_index_root_node_offset );

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_file_header_data_32bit_t *) file_header_data )->offsets_index_back_pointer,
		 file_header->offsets_index_root_node_back_pointer );

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_file_header_data_32bit_t *) file_header_data )->offsets_index_root_node_offset,
		 safe_offsets_index_root_node_offset );

#if defined( HAVE_DEBUG_OUTPUT )
		sentinel = ( (pff_file_header_data_32bit_t *) file_header_data )->sentinel;
#endif
		file_header->encryption_type = ( (pff_file_header_data_32bit_t *) file_header_data )->encryption_type;
	}
	else if( ( file_header->file_type == LIBPFF_FILE_TYPE_64BIT )
	      || ( file_header->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (pff_file_header_data_64bit_t *) file_header_data )->file_size,
		 file_header->file_size );

		byte_stream_copy_to_uint64_little_endian(
		 ( (pff_file_header_data_64bit_t *) file_header_data )->descriptors_index_back_pointer,
		 file_header->descriptors_index_root_node_back_pointer );

		byte_stream_copy_to_uint64_little_endian(
		 ( (pff_file_header_data_64bit_t *) file_header_data )->descriptors_index_root_node_offset,
		 safe_descriptors_index_root_node_offset );

		byte_stream_copy_to_uint64_little_endian(
		 ( (pff_file_header_data_64bit_t *) file_header_data )->offsets_index_back_pointer,
		 file_header->offsets_index_root_node_back_pointer );

		byte_stream_copy_to_uint64_little_endian(
		 ( (pff_file_header_data_64bit_t *) file_header_data )->offsets_index_root_node_offset,
		 safe_offsets_index_root_node_offset );

#if defined( HAVE_DEBUG_OUTPUT )
		sentinel = ( (pff_file_header_data_64bit_t *) file_header_data )->sentinel;
#endif
		file_header->encryption_type = ( (pff_file_header_data_64bit_t *) file_header_data )->encryption_type;

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_file_header_data_64bit_t *) file_header_data )->checksum,
		 stored_checksum );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( file_header->file_type == LIBPFF_FILE_TYPE_32BIT )
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
			 "%s: next index back pointer\t\t\t: %" PRIu32 "\n",
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
				 "%s: high water mark: %02d value\t\t\t: %" PRIu32 "\n",
				 function,
				 value_iterator,
				 value_32bit );

				value_data += 4;
			}
			libcnotify_printf(
			 "\n" );
		}
		else if( ( file_header->file_type == LIBPFF_FILE_TYPE_64BIT )
		      || ( file_header->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
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
			 "%s: next index back pointer\t\t\t: %" PRIu64 "\n",
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
				 "%s: high water mark: %02d value\t\t\t: %" PRIu32 "\n",
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

		if( file_header->file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			libcnotify_printf(
			 "%s: unknown5:\n",
			 function );
			libcnotify_print_data(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->unknown5,
			 4,
			 0 );
		}
		else if( ( file_header->file_type == LIBPFF_FILE_TYPE_64BIT )
		      || ( file_header->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
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
		 "%s: file size\t\t\t\t\t: %" PRIu64 "\n",
		 function,
		 file_header->file_size );

		if( file_header->file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->last_data_allocation_table_offset,
			 value_32bit );
			libcnotify_printf(
			 "%s: last data allocation table offset\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->total_available_data_size,
			 value_32bit );
			libcnotify_printf(
			 "%s: total available data size\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (pff_file_header_data_32bit_t *) file_header_data )->total_available_page_size,
			 value_32bit );
			libcnotify_printf(
			 "%s: total available page size\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );
		}
		else if( ( file_header->file_type == LIBPFF_FILE_TYPE_64BIT )
		      || ( file_header->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
		{
			byte_stream_copy_to_uint64_little_endian(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->last_data_allocation_table_offset,
			 value_64bit );
			libcnotify_printf(
			 "%s: last data allocation table offset\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->total_available_data_size,
			 value_64bit );
			libcnotify_printf(
			 "%s: total available data size\t\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (pff_file_header_data_64bit_t *) file_header_data )->total_available_page_size,
			 value_64bit );
			libcnotify_printf(
			 "%s: total available page size\t\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );
		}
		libcnotify_printf(
		 "%s: descriptors index back pointer\t\t: %" PRIu64 "\n",
		 function,
		 file_header->descriptors_index_root_node_back_pointer );

		libcnotify_printf(
		 "%s: descriptors index root node offset\t: %" PRIu64 " (0x%08" PRIx64 ")\n",
		 function,
		 safe_descriptors_index_root_node_offset );

		libcnotify_printf(
		 "%s: offsets index back pointer\t\t: %" PRIu64 "\n",
		 function,
		 file_header->offsets_index_root_node_back_pointer );

		libcnotify_printf(
		 "%s: offsets index root node offset\t\t: %" PRIu64 " (0x%08" PRIx64 ")\n",
		 function,
		 safe_offsets_index_root_node_offset,
		 safe_offsets_index_root_node_offset );

		if( file_header->file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			libcnotify_printf(
			 "%s: allocation table validation type\t\t: 0x%02" PRIx8 "\n",
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
		else if( ( file_header->file_type == LIBPFF_FILE_TYPE_64BIT )
		      || ( file_header->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
		{
			libcnotify_printf(
			 "%s: allocation table validation type\t\t: 0x%02" PRIx8 "\n",
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
		 "%s: sentinel\t\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 sentinel );

		libcnotify_printf(
		 "%s: encryption type\t\t\t\t: 0x%02" PRIx8 " (%s)\n",
		 function,
		 file_header->encryption_type,
		 libpff_debug_get_encryption_type(
		  file_header->encryption_type ) );

		if( file_header->file_type == LIBPFF_FILE_TYPE_32BIT )
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
		else if( ( file_header->file_type == LIBPFF_FILE_TYPE_64BIT )
		      || ( file_header->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
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
			 "%s: checksum\t\t\t\t\t: 0x%08" PRIx64 "\n",
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
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( safe_descriptors_index_root_node_offset > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid descriptors index root node offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( safe_offsets_index_root_node_offset > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid offsets index root node offset value out of bounds.",
		 function );

		return( -1 );
	}

	file_header->descriptors_index_root_node_offset = (off64_t) safe_descriptors_index_root_node_offset;
	file_header->offsets_index_root_node_offset     = (off64_t) safe_offsets_index_root_node_offset;

	if( ( file_header->file_type == LIBPFF_FILE_TYPE_64BIT )
	 || ( file_header->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
	{
		if( libpff_checksum_calculate_weak_crc32(
		     &calculated_checksum,
		     &( data[ 8 ] ),
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

			return( -1 );
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

			return( -1 );
		}
	}
	return( 1 );
}

/* Reads the file header
 * Returns 1 if successful or -1 on error
 */
int libpff_file_header_read_file_io_handle(
     libpff_file_header_t *file_header,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	uint8_t file_header_data[ 564 ];

	static char *function = "libpff_file_header_read_file_io_handle";
	size_t read_size      = 564;
	ssize_t read_count    = 0;

	if( file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file header.",
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
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              file_header_data,
	              read_size,
	              0,
	              error );

	if( read_count != (ssize_t) read_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header data at offset: 0 (0x00000000).",
		 function );

		return( -1 );
	}
	if( libpff_file_header_read_data(
	     file_header,
	     file_header_data,
	     read_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header.",
		 function );

		return( -1 );
	}
	return( 1 );
}

