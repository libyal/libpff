/*
 * Allocation table functions
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
#include "libpff_definitions.h"
#include "libpff_libbfio.h"
#include "libpff_libcdata.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfmapi.h"

#include "pff_allocation_table.h"

/* Reads an allocation table
 * Returns 1 if successful or -1 on error
 */
int libpff_allocation_table_read(
     libcdata_range_list_t *unallocated_block_list,
     libbfio_handle_t *file_io_handle,
     off64_t allocation_table_offset,
     uint8_t file_type,
     libcerror_error_t **error )
{
	uint8_t *allocation_table_data     = NULL;
	uint8_t *table_data                = NULL;
	static char *function              = "libpff_allocation_table_read";
	off64_t back_pointer_offset        = 0;
	off64_t unallocated_offset         = 0;
	size_t read_size                   = 0;
	size_t unallocated_size            = 0;
	size_t allocation_block_size       = 0;
	ssize_t read_count                 = 0;
	uint32_t stored_checksum           = 0;
	uint32_t calculated_checksum       = 0;
	uint16_t table_data_index          = 0;
	uint16_t table_data_size           = 0;
	uint8_t allocation_table_entry     = 0;
	uint8_t allocation_table_type      = 0;
	uint8_t allocation_table_type_copy = 0;
	uint8_t bit_index                  = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit               = 0;
	uint16_t value_16bit               = 0;
#endif

	if( unallocated_block_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid unallocated block list.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading allocation table at offset: 0x%08" PRIx64 "\n",
		 function,
		 allocation_table_offset );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     allocation_table_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek allocation table offset: 0x%08" PRIx64 ".",
		 function,
		 allocation_table_offset );

		goto on_error;
	}
	if( file_type == LIBPFF_FILE_TYPE_32BIT )
	{
		read_size       = sizeof( pff_allocation_table_32bit_t );
		table_data_size = 496;
	}
	else if( file_type == LIBPFF_FILE_TYPE_64BIT )
	{
		read_size       = sizeof( pff_allocation_table_64bit_t );
		table_data_size = 496;
	}
	else if( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE )
	{
		read_size       = sizeof( pff_allocation_table_64bit_4k_page_t );
		table_data_size = 4072;
	}
	allocation_table_data = (uint8_t *) memory_allocate(
	                                     sizeof( uint8_t ) * read_size );

	if( allocation_table_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create alloction table data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              allocation_table_data,
	              read_size,
	              error );

	if( read_count != (ssize_t) read_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read allocation table.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: allocation table:\n",
		 function );
		libcnotify_print_data(
		 allocation_table_data,
		 read_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( file_type == LIBPFF_FILE_TYPE_32BIT )
	{
		table_data                 = ( (pff_allocation_table_32bit_t *) allocation_table_data )->data;
		allocation_table_type      = ( (pff_allocation_table_32bit_t *) allocation_table_data )->type;
		allocation_table_type_copy = ( (pff_allocation_table_32bit_t *) allocation_table_data )->type_copy;

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_allocation_table_32bit_t *) allocation_table_data )->back_pointer,
		 back_pointer_offset );
		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_allocation_table_32bit_t *) allocation_table_data )->checksum,
		 stored_checksum );
	}
	else if( file_type == LIBPFF_FILE_TYPE_64BIT )
	{
		table_data                 = ( (pff_allocation_table_64bit_t *) allocation_table_data )->data;
		allocation_table_type      = ( (pff_allocation_table_64bit_t *) allocation_table_data )->type;
		allocation_table_type_copy = ( (pff_allocation_table_64bit_t *) allocation_table_data )->type_copy;

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_allocation_table_64bit_t *) allocation_table_data )->checksum,
		 stored_checksum );
		byte_stream_copy_to_uint64_little_endian(
		 ( (pff_allocation_table_64bit_t *) allocation_table_data )->back_pointer,
		 back_pointer_offset );
	}
	else if( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE )
	{
		table_data                 = ( (pff_allocation_table_64bit_4k_page_t *) allocation_table_data )->data;
		allocation_table_type      = ( (pff_allocation_table_64bit_4k_page_t *) allocation_table_data )->type;
		allocation_table_type_copy = ( (pff_allocation_table_64bit_4k_page_t *) allocation_table_data )->type_copy;

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_allocation_table_64bit_4k_page_t *) allocation_table_data )->checksum,
		 stored_checksum );
		byte_stream_copy_to_uint64_little_endian(
		 ( (pff_allocation_table_64bit_4k_page_t *) allocation_table_data )->back_pointer,
		 back_pointer_offset );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: type\t\t: 0x%02" PRIx8 "\n",
		 function,
		 allocation_table_type );
		libcnotify_printf(
		 "%s: type copy\t\t: 0x%02" PRIx8 "\n",
		 function,
		 allocation_table_type_copy );

		if( file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			byte_stream_copy_to_uint16_little_endian(
			 ( (pff_allocation_table_32bit_t *) allocation_table_data )->signature,
			 value_16bit );
			libcnotify_printf(
			 "%s: signature\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );

			libcnotify_printf(
			 "%s: back pointer\t: %" PRIu64 "\n",
			 function,
			 back_pointer_offset );

			libcnotify_printf(
			 "%s: checksum\t\t: 0x%" PRIx32 "\n",
			 function,
			 stored_checksum );
		}
		else if( ( file_type == LIBPFF_FILE_TYPE_64BIT )
		      || ( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
		{
			if( file_type == LIBPFF_FILE_TYPE_64BIT )
			{
				byte_stream_copy_to_uint16_little_endian(
				 ( (pff_allocation_table_64bit_t *) allocation_table_data )->signature,
				 value_16bit );
			}
			else
			{
				byte_stream_copy_to_uint16_little_endian(
				 ( (pff_allocation_table_64bit_4k_page_t *) allocation_table_data )->signature,
				 value_16bit );
			}
			libcnotify_printf(
			 "%s: signature\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );

			libcnotify_printf(
			 "%s: checksum\t\t: 0x%" PRIx32 "\n",
			 function,
			 stored_checksum );

			libcnotify_printf(
			 "%s: back pointer\t: %" PRIu64 "\n",
			 function,
			 back_pointer_offset );

			if( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE )
			{
				byte_stream_copy_to_uint64_little_endian(
				 ( (pff_allocation_table_64bit_4k_page_t *) allocation_table_data )->unknown1,
				 value_64bit );
				libcnotify_printf(
				 "%s: unknown1\t\t: 0x%08" PRIx64 "\n",
				 function,
				 value_64bit );
			}
		}
		libcnotify_printf(
		 "\n" );
	}
#endif
	if( libfmapi_checksum_calculate_weak_crc32(
	     &calculated_checksum,
	     table_data,
	     (size_t) table_data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
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
		 "%s: mismatch in checksum ( %" PRIu32 " != %" PRIu32 " ).",
		 function,
		 stored_checksum,
		 calculated_checksum );

/* TODO implement error tollerance */
		goto on_error;
	}
	if( allocation_table_type != allocation_table_type_copy )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
		 "%s: mismatch in allocation table type ( 0x%02" PRIx8 " != 0x%02" PRIx8 " ).",
		 function,
		 allocation_table_type,
		 allocation_table_type_copy );

/* TODO implement error tollerance */
		goto on_error;
	}
	if( ( allocation_table_type != LIBPFF_ALLOCATION_TABLE_TYPE_DATA )
	 && ( allocation_table_type != LIBPFF_ALLOCATION_TABLE_TYPE_PAGE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported allocation table type: 0x%08" PRIx32 "",
		 function,
		 allocation_table_type );

/* TODO implement error tollerance */
		goto on_error;
	}
	if( allocation_table_type == LIBPFF_ALLOCATION_TABLE_TYPE_PAGE )
	{
		/* The page type allocation has not yet been seen i.c.w. 4k pages */
		if( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported file type.",
			 function );

			goto on_error;
		}
		allocation_block_size = 512;
		back_pointer_offset  -= 0x200;
	}
	else if( allocation_table_type == LIBPFF_ALLOCATION_TABLE_TYPE_DATA )
	{
		allocation_block_size = 64;
	}
	for( table_data_index = 0;
	     table_data_index < table_data_size;
	     table_data_index++ )
	{
		allocation_table_entry = table_data[ table_data_index ];

		for( bit_index = 0;
		     bit_index < 8;
		     bit_index++ )
		{
			if( ( allocation_table_entry & 0x80 ) == 0 )
			{
				if( unallocated_size == 0 )
				{
					unallocated_offset = back_pointer_offset;
				}
				unallocated_size += allocation_block_size;
			}
			else if( unallocated_size > 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: unallocated block: 0x%08" PRIx64 " - 0x%08" PRIx64 " (%" PRIu64 ")\n",
					 function,
					 unallocated_offset,
					 unallocated_offset + unallocated_size,
					 unallocated_size );
				}
#endif
				if( libcdata_range_list_insert_range(
				     unallocated_block_list,
				     unallocated_offset,
				     unallocated_size,
				     NULL,
				     NULL,
				     NULL,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append unallocated block to list.",
					 function );

					goto on_error;
				}
				unallocated_size = 0;
			}
			allocation_table_entry <<= 1;

			back_pointer_offset += allocation_block_size;
		}
	}
	if( unallocated_size > 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unallocated block: 0x%08" PRIx64 " - 0x%08" PRIx64 " (%" PRIu64 ")\n",
			 function,
			 unallocated_offset,
			 unallocated_offset + unallocated_size,
			 unallocated_size );
		}
#endif
		if( libcdata_range_list_insert_range(
		     unallocated_block_list,
		     unallocated_offset,
		     unallocated_size,
		     NULL,
		     NULL,
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append unallocated block to list.",
			 function );

			goto on_error;
		}
		unallocated_size = 0;
	}
	memory_free(
	 allocation_table_data );

	return( 1 );

on_error:
	if( allocation_table_data != NULL )
	{
		memory_free(
		 allocation_table_data );
	}
	return( -1 );
}

