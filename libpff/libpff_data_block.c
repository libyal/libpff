/*
 * Data block functions
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
#include "libpff_compression.h"
#include "libpff_definitions.h"
#include "libpff_data_block.h"
#include "libpff_encryption.h"
#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_unused.h"

#include "pff_block.h"

/* Creates a data block
 * Make sure the value data_block is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_data_block_initialize(
     libpff_data_block_t **data_block,
     libpff_io_handle_t *io_handle,
     uint32_t descriptor_identifier,
     uint64_t data_identifier,
     libcerror_error_t **error )
{
	static char *function = "libpff_data_block_initialize";

	if( data_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data block.",
		 function );

		return( -1 );
	}
	if( *data_block != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data block value already set.",
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
	*data_block = memory_allocate_structure(
	               libpff_data_block_t );

	if( *data_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data block.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *data_block,
	     0,
	     sizeof( libpff_data_block_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data block.",
		 function );

		goto on_error;
	}
	( *data_block )->io_handle             = io_handle;
	( *data_block )->descriptor_identifier = descriptor_identifier;
	( *data_block )->data_identifier       = data_identifier;

	return( 1 );

on_error:
	if( *data_block != NULL )
	{
		memory_free(
		 *data_block );

		*data_block = NULL;
	}
	return( -1 );
}

/* Frees a data block
 * Returns 1 if successful or -1 on error
 */
int libpff_data_block_free(
     libpff_data_block_t **data_block,
     libcerror_error_t **error )
{
	static char *function = "libpff_data_block_free";

	if( data_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data block.",
		 function );

		return( -1 );
	}
	if( *data_block != NULL )
	{
		if( ( *data_block )->data != NULL )
		{
			memory_free(
			 ( *data_block )->data );
		}
		memory_free(
		 *data_block );

		*data_block = NULL;
	}
	return( 1 );
}

/* Clones the data block
 * Returns 1 if successful or -1 on error
 */
int libpff_data_block_clone(
     libpff_data_block_t **destination_data_block,
     libpff_data_block_t *source_data_block,
     libcerror_error_t **error )
{
	static char *function = "libpff_data_block_clone";

	if( destination_data_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination data block.",
		 function );

		return( -1 );
	}
	if( *destination_data_block != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination data block already set.",
		 function );

		return( -1 );
	}
	if( source_data_block == NULL )
	{
		*destination_data_block = NULL;

		return( 1 );
	}
	if( libpff_data_block_initialize(
	     destination_data_block,
	     source_data_block->io_handle,
	     source_data_block->descriptor_identifier,
	     source_data_block->data_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination data block.",
		 function );

		goto on_error;
	}
	if( *destination_data_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing destination data block.",
		 function );

		goto on_error;
	}
	if( ( source_data_block->data != NULL )
	 && ( source_data_block->data_size > 0 ) )
	{
		if( source_data_block->data_size > MEMORY_MAXIMUM_ALLOCATION_SIZE )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid source data block - data size value exceeds maximum.",
			 function );

			goto on_error;
		}
		( *destination_data_block )->data = (uint8_t *) memory_allocate(
		                                                 sizeof( uint8_t ) * source_data_block->data_size );

		if( ( *destination_data_block )->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create destination data block data.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_data_block )->data,
		     source_data_block->data,
		     source_data_block->data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy source data block data to destination.",
			 function );

			goto on_error;
		}
		( *destination_data_block )->data_size = source_data_block->data_size;
	}
	( *destination_data_block )->flags = source_data_block->flags;

	return( 1 );

on_error:
	if( *destination_data_block != NULL )
	{
		libpff_data_block_free(
		 destination_data_block,
		 NULL );
	}
	return( -1 );
}

/* Reads the data block footer
 * Returns 1 if successful or -1 on error
 */
int libpff_data_block_read_footer_data(
     libpff_data_block_t *data_block,
     const uint8_t *data,
     size_t data_size,
     uint8_t file_type,
     libcerror_error_t **error )
{
	static char *function            = "libpff_data_block_read_footer_data";
	size_t data_block_footer_size    = 0;
	uint64_t data_block_back_pointer = 0;
	uint16_t data_block_signature    = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit             = 0;
	uint16_t value_16bit             = 0;
#endif

	if( data_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data block.",
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
		data_block_footer_size = sizeof( pff_block_footer_32bit_t );
	}
	else if( file_type == LIBPFF_FILE_TYPE_64BIT )
	{
		data_block_footer_size = sizeof( pff_block_footer_64bit_t );
	}
	else if( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE )
	{
		data_block_footer_size = sizeof( pff_block_footer_64bit_4k_page_t );
	}
	if( data_size < data_block_footer_size )
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
		 "%s: data block footer data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_block_footer_size,
		 0 );
	}
#endif
	if( file_type == LIBPFF_FILE_TYPE_32BIT )
	{
		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_block_footer_32bit_t *) data )->data_size,
		 data_block->data_size );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_block_footer_32bit_t *) data )->signature,
		 data_block_signature );

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_block_footer_32bit_t *) data )->back_pointer,
		 data_block_back_pointer );

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_block_footer_32bit_t *) data )->checksum,
		 data_block->stored_checksum );

		data_block->uncompressed_data_size = data_block->data_size;
	}
	else if( file_type == LIBPFF_FILE_TYPE_64BIT )
	{
		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_block_footer_64bit_t *) data )->data_size,
		 data_block->data_size );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_block_footer_64bit_t *) data )->signature,
		 data_block_signature );

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_block_footer_64bit_t *) data )->checksum,
		 data_block->stored_checksum );

		byte_stream_copy_to_uint64_little_endian(
		 ( (pff_block_footer_64bit_t *) data )->back_pointer,
		 data_block_back_pointer );

		data_block->uncompressed_data_size = data_block->data_size;
	}
	else if( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE )
	{
		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_block_footer_64bit_4k_page_t *) data )->data_size,
		 data_block->data_size );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_block_footer_64bit_4k_page_t *) data )->signature,
		 data_block_signature );

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_block_footer_64bit_4k_page_t *) data )->checksum,
		 data_block->stored_checksum );

		byte_stream_copy_to_uint64_little_endian(
		 ( (pff_block_footer_64bit_4k_page_t *) data )->back_pointer,
		 data_block_back_pointer );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_block_footer_64bit_4k_page_t *) data )->uncompressed_data_size,
		 data_block->uncompressed_data_size );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: data size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 data_block->data_size );

		libcnotify_printf(
		 "%s: signature\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 data_block_signature );

		if( file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			libcnotify_printf(
			 "%s: back pointer\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 data_block_back_pointer );

			libcnotify_printf(
			 "%s: data checksum\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 data_block->stored_checksum );
		}
		else if( ( file_type == LIBPFF_FILE_TYPE_64BIT )
		      || ( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
		{
			libcnotify_printf(
			 "%s: data checksum\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 data_block->stored_checksum );

			libcnotify_printf(
			 "%s: back pointer\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 data_block_back_pointer );
		}
		if( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE )
		{
			byte_stream_copy_to_uint16_little_endian(
			 ( (pff_block_footer_64bit_4k_page_t *) data )->unknown1,
			 value_16bit );
			libcnotify_printf(
			 "%s: unknown1\t\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );

			libcnotify_printf(
			 "%s: uncompressed data size\t\t: %" PRIu16 "\n",
			 function,
			 data_block->uncompressed_data_size );

			byte_stream_copy_to_uint32_little_endian(
			 ( (pff_block_footer_64bit_4k_page_t *) data )->unknown2,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown2\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );
		}
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

/* Reads the data block
 * Returns 1 if successful or -1 on error
 */
int libpff_data_block_read_file_io_handle(
     libpff_data_block_t *data_block,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     size32_t data_size,
     uint8_t file_type,
     libcerror_error_t **error )
{
	uint8_t *uncompressed_data            = NULL;
	static char *function                 = "libpff_data_block_read_file_io_handle";
	size_t data_block_footer_offset       = 0;
	size_t uncompressed_data_size         = 0;
	ssize_t read_count                    = 0;
	uint64_t data_block_back_pointer      = 0;
	uint32_t calculated_checksum          = 0;
	uint32_t data_block_data_size         = 0;
	uint32_t data_block_increment_size    = 0;
	uint32_t data_block_footer_size       = 0;
	uint32_t maximum_data_block_size      = 0;

#if defined( HAVE_VERBOSE_OUTPUT )
	size_t data_block_padding_size        = 0;
	uint32_t maximum_data_block_data_size = 0;
#endif

	if( data_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data block.",
		 function );

		return( -1 );
	}
	if( data_block->data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data block - data value already set.",
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
#if UINT32_MAX > SSIZE_MAX
	if( data_size > (size32_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: data size value exceeds maximum.",
		 function );

		return( -1 );
	}
#endif
	if( data_size != 0 )
	{
		if( file_type == LIBPFF_FILE_TYPE_32BIT )
		{
			data_block_footer_size    = (uint32_t) sizeof( pff_block_footer_32bit_t );
			data_block_increment_size = 64;
			maximum_data_block_size   = 8192;
		}
		else if( file_type == LIBPFF_FILE_TYPE_64BIT )
		{
			data_block_footer_size    = (uint32_t) sizeof( pff_block_footer_64bit_t );
			data_block_increment_size = 64;
			maximum_data_block_size   = 8192;
		}
		else if( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE )
		{
			data_block_footer_size    = (uint32_t) sizeof( pff_block_footer_64bit_4k_page_t );
			data_block_increment_size = 512;
/* TODO: this value is currently assumed based on the 512 x 8 = 4k page */
			maximum_data_block_size   = 65536;
		}
#if defined( HAVE_VERBOSE_OUTPUT )
		maximum_data_block_data_size = maximum_data_block_size - data_block_footer_size;
#endif
		data_block_data_size = (uint32_t) data_size / data_block_increment_size;

		if( ( (uint32_t) data_size % data_block_increment_size ) != 0 )
		{
			data_block_data_size += 1;
		}
		data_block_data_size *= data_block_increment_size;

		if( ( data_block_data_size - (uint32_t) data_size ) < data_block_footer_size )
		{
			data_block_data_size += data_block_increment_size;
		}
		if( ( data_block_data_size == 0 )
		 || ( data_block_data_size > maximum_data_block_size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid data block data size value out of bounds.",
			 function );

			goto on_error;
		}
		data_block->data = (uint8_t *) memory_allocate(
		                                sizeof( uint8_t ) * data_block_data_size );

		if( data_block->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create data block data.",
			 function );

			goto on_error;
		}
		data_block->data_size = data_block_data_size;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: reading data block at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
			 function,
			 file_offset,
			 file_offset );
		}
#endif
		read_count = libbfio_handle_read_buffer_at_offset(
		              file_io_handle,
		              data_block->data,
		              data_block->data_size,
		              file_offset,
		              error );

		if( read_count != (ssize_t) data_block->data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read data block data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 file_offset,
			 file_offset );

			goto on_error;
		}
		data_block_footer_offset = data_block->data_size - data_block_footer_size;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			data_block_padding_size = data_block_footer_offset - data_size;

			libcnotify_printf(
			 "%s: data block padding size\t\t: %" PRIzd "\n",
			 function,
			 data_block_padding_size );

			libcnotify_printf(
			 "%s: data block padding:\n",
			 function );
			libcnotify_print_data(
			 &( data_block->data[ data_size ] ),
			 data_block_padding_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif
		if( libpff_data_block_read_footer_data(
		     data_block,
		     &( data_block->data[ data_block_footer_offset ] ),
		     data_block_footer_size,
		     file_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read data block footer.",
			 function );

			goto on_error;
		}
#if defined( HAVE_VERBOSE_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( data_block->data_size > maximum_data_block_data_size )
			{
				libcnotify_printf(
				 "%s: data size: %" PRIu32 " exceeds format specified maximum: %" PRIu32 ".\n",
				 function,
				 data_block->data_size,
                                 maximum_data_block_data_size );
			}
		}
#endif
		if( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE )
		{
			if( ( data_block->data_size != 0 )
			 && ( data_block->uncompressed_data_size != 0 )
			 && ( data_block->data_size != data_block->uncompressed_data_size ) )
			{
				data_block->flags |= LIBPFF_DATA_BLOCK_FLAG_COMPRESSED;
			}
		}
		if( ( data_block->flags & LIBPFF_DATA_BLOCK_FLAG_VALIDATED ) == 0 )
		{
			if( data_block->data_size != 0 )
			{
				if( (size32_t) data_block->data_size != data_size )
				{
/* TODO flag size mismatch and error tollerance */
					data_block->flags |= LIBPFF_DATA_BLOCK_FLAG_SIZE_MISMATCH;

					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_INPUT,
					 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
					 "%s: mismatch in data size ( %" PRIu32 " != %" PRIu32 " ).",
					 function,
					 data_block->data_size,
					 data_size );

					goto on_error;
				}
			}
			if( data_block->stored_checksum != 0 )
			{
				if( libpff_checksum_calculate_weak_crc32(
				     &calculated_checksum,
				     data_block->data,
				     (size_t) data_size,
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
				if( data_block->stored_checksum != calculated_checksum )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: mismatch in data block checksum ( 0x%08" PRIx32 " != 0x%08" PRIx32 " ).\n",
						 function,
						 data_block->stored_checksum,
						 calculated_checksum );
					}
#endif
					data_block->flags |= LIBPFF_DATA_BLOCK_FLAG_CRC_MISMATCH;

/* TODO smart error handling */
				}
			}
			if( data_block_back_pointer != 0 )
			{
				if( data_block->data_identifier != data_block_back_pointer )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: mismatch in data identifier: %" PRIu64 " (0x%08" PRIx64 ") and back pointer: 0x%08" PRIx64 ".\n",
						 function,
						 data_block->data_identifier,
						 data_block->data_identifier,
						 data_block_back_pointer );
					}
#endif
					data_block->flags |= LIBPFF_DATA_BLOCK_FLAG_IDENTIFIER_MISMATCH;

/* TODO smart error handling */
				}
			}
			data_block->flags |= LIBPFF_DATA_BLOCK_FLAG_VALIDATED;
		}
/* TODO refactor after testing */
		if( ( data_block->flags & LIBPFF_DATA_BLOCK_FLAG_COMPRESSED ) != 0 )
		{
			uncompressed_data_size = (size_t) data_block->uncompressed_data_size;

			if( ( uncompressed_data_size == 0 )
			 || ( uncompressed_data_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid uncompressed data size value out of bounds.",
				 function );

				goto on_error;
			}
			uncompressed_data = (uint8_t *) memory_allocate(
			                                 sizeof( uint8_t ) * uncompressed_data_size );

			if( uncompressed_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create uncompressed data.",
				 function );

				goto on_error;
			}
			if( libpff_decompress_data(
			     data_block->data,
			     (size_t) data_block->data_size,
			     LIBPFF_COMPRESSION_METHOD_DEFLATE,
			     uncompressed_data,
			     &uncompressed_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
				 LIBCERROR_COMPRESSION_ERROR_DECOMPRESS_FAILED,
				 "%s: unable to decompress data block data.",
				 function );

				goto on_error;
			}
			memory_free(
			 data_block->data );

			data_block->data      = uncompressed_data;
			data_block->data_size = data_block->uncompressed_data_size;
			uncompressed_data     = NULL;
		}
	}
	return( 1 );

on_error:
	if( uncompressed_data != NULL )
	{
		memory_free(
		 uncompressed_data );
	}
	if( data_block->data != NULL )
	{
		memory_free(
		 data_block->data );

		data_block->data = NULL;
	}
	data_block->data_size = 0;

	return( -1 );
}

/* Reads the data block element data
 * Callback for the descriptor data list
 * Returns the number of bytes read if successful or -1 on error
 */
int libpff_data_block_read_element_data(
     libpff_data_block_t *data_block,
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
	static char *function = "libpff_data_block_read_element_data";

	LIBPFF_UNREFERENCED_PARAMETER( element_file_index )
	LIBPFF_UNREFERENCED_PARAMETER( element_flags )

	if( data_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data block.",
		 function );

		return( -1 );
	}
	if( data_block->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data block - missing IO handle.",
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
	if( data_block->data == NULL )
	{
		if( libpff_data_block_read_file_io_handle(
		     data_block,
		     file_io_handle,
		     element_offset,
		     (size32_t) element_size,
		     data_block->io_handle->file_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read data block data.",
			 function );

			return( -1 );
		}
		if( libpff_data_block_decrypt_data(
		     data_block,
		     read_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBCERROR_ENCRYPTION_ERROR_DECRYPT_FAILED,
			 "%s: unable to decrypt data block data.",
			 function );

			return( -1 );
		}
	}
	/* The data block is managed by the list and should not be managed by the cache as well
	 */
	if( libfdata_list_element_set_element_value(
	     list_element,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) cache,
	     (intptr_t *) data_block,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_data_block_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set data block as element value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Decrypts the data block data
 * Returns 1 if successful or -1 on error
 */
int libpff_data_block_decrypt_data(
     libpff_data_block_t *data_block,
     uint8_t read_flags,
     libcerror_error_t **error )
{
	static char *function        = "libpff_data_block_decrypt_data";
	ssize_t process_count        = 0;
	uint8_t decrypt_data         = 0;
	uint8_t encryption_type      = 0;
	uint8_t force_decryption     = 0;
	uint8_t node_identifier_type = 0;
	uint8_t node_contains_table  = 0;

	if( data_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data block.",
		 function );

		return( -1 );
	}
	if( data_block->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data block - missing IO handle.",
		 function );

		return( -1 );
	}
	if( data_block->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data block - missing data.",
		 function );

		return( -1 );
	}
	encryption_type = data_block->io_handle->encryption_type;

	/* Check if internal (unencrypted) flag in the (data) offset index identifier is not set
	 */
	if( ( data_block->data_identifier & LIBPFF_OFFSET_INDEX_IDENTIFIER_FLAG_INTERNAL ) == 0 )
	{
		decrypt_data = 1;
	}
	/* Check if data is encrypted
	 * Some 'invalid' files have an encryption type of none but contain encrypted data
	 * Although they are considered invalid by Outlook it is still possilble to read them
	 */
	if( ( encryption_type == LIBPFF_ENCRYPTION_TYPE_NONE )
	 && ( ( read_flags & LIBPFF_READ_FLAG_IGNORE_FORCE_DECRYPTION ) == 0 )
	 && ( data_block->data_size > 4 ) )
	{
		node_identifier_type = (uint8_t) ( data_block->descriptor_identifier & 0x0000001fUL );

		/* Check if the node identifier type consists of a table
		 */
		if( ( ( node_identifier_type == LIBPFF_NODE_IDENTIFIER_TYPE_INTERNAL )
		  && ( ( data_block->descriptor_identifier == LIBPFF_DESCRIPTOR_IDENTIFIER_MESSAGE_STORE )
		    || ( data_block->descriptor_identifier == LIBPFF_DESCRIPTOR_IDENTIFIER_NAME_TO_ID_MAP )
		    || ( data_block->descriptor_identifier == LIBPFF_DESCRIPTOR_IDENTIFIER_UNKNOWN_2049 )
		    || ( data_block->descriptor_identifier == LIBPFF_DESCRIPTOR_IDENTIFIER_UNKNOWN_2081 )
		    || ( data_block->descriptor_identifier == LIBPFF_DESCRIPTOR_IDENTIFIER_UNKNOWN_2113 )
		    || ( data_block->descriptor_identifier == LIBPFF_DESCRIPTOR_IDENTIFIER_UNKNOWN_3073 ) ) )
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
		if( node_contains_table != 0 )
		{
			/* Test if the data contains an unencrypted table
			 * a table consists of 0xec in the third byte
			 * and 0x6c, 0x7c, 0x8c, 0x9c, 0xa5, 0xac, 0xbc, 0xcc in the fourth
			 */
			if( ( data_block->data[ 2 ] != 0xec )
			 || ( ( data_block->data[ 3 ] != 0x6c )
			  &&  ( data_block->data[ 3 ] != 0x7c )
			  &&  ( data_block->data[ 3 ] != 0x8c )
			  &&  ( data_block->data[ 3 ] != 0x9c )
			  &&  ( data_block->data[ 3 ] != 0xa5 )
			  &&  ( data_block->data[ 3 ] != 0xac )
			  &&  ( data_block->data[ 3 ] != 0xbc )
			  &&  ( data_block->data[ 3 ] != 0xcc ) ) )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: table signature missing trying to force decryption.\n",
					 function );
				}
#endif
				force_decryption   = 1;
				encryption_type    = LIBPFF_ENCRYPTION_TYPE_COMPRESSIBLE;
				decrypt_data       = 1;
				data_block->flags |= LIBPFF_DATA_BLOCK_FLAG_DECRYPTION_FORCED;
			}
		}
		else if( data_block->io_handle->force_decryption == 1 )
		{
			/* Check if the internal (unencrypted) flag in (data) offset index identifier is not set
			 */
			if( ( data_block->data_identifier & LIBPFF_OFFSET_INDEX_IDENTIFIER_FLAG_INTERNAL ) == 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: decryption forced.\n",
					 function );
				}
#endif
				encryption_type    = LIBPFF_ENCRYPTION_TYPE_COMPRESSIBLE;
				decrypt_data       = 1;
				data_block->flags |= LIBPFF_DATA_BLOCK_FLAG_DECRYPTION_FORCED;
			}
		}
	}
	if( decrypt_data != 0 )
	{
		process_count = libpff_encryption_decrypt(
				 encryption_type,
				 (uint32_t) data_block->data_identifier,
				 data_block->data,
				 (size_t) data_block->data_size,
				 error );

		if( process_count != (ssize_t) data_block->data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBCERROR_ENCRYPTION_ERROR_DECRYPT_FAILED,
			 "%s: unable to decrypt data block data.",
			 function );

			return( -1 );
		}
		if( force_decryption != 0 )
		{
			/* Test if the data contains an unencrypted table
			 * a table consists of 0xec in the third byte
			 * and 0x6c, 0x7c, 0x8c, 0x9c, 0xa5, 0xac, 0xbc, 0xcc in the fourth
			 */
			if( ( data_block->data[ 2 ] == 0xec )
			 && ( ( data_block->data[ 3 ] == 0x6c )
			  ||  ( data_block->data[ 3 ] == 0x7c )
			  ||  ( data_block->data[ 3 ] == 0x8c )
			  ||  ( data_block->data[ 3 ] == 0x9c )
			  ||  ( data_block->data[ 3 ] == 0xa5 )
			  ||  ( data_block->data[ 3 ] == 0xac )
			  ||  ( data_block->data[ 3 ] == 0xbc )
			  ||  ( data_block->data[ 3 ] == 0xcc ) ) )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: compressible encrypted data detected while encryption type is none - decryption forced.\n",
					 function );
				}
#endif
				data_block->io_handle->force_decryption = 1;
			}
		}
	}
	return( 1 );
}

