/*
 * OSS-Fuzz target for libpff record entry values
 *
 * Copyright (C) 2008-2026, Joachim Metz <joachim.metz@gmail.com>
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

#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>

/* Note that some of the OSS-Fuzz engines use C++
 */
extern "C" {

#include "ossfuzz_libbfio.h"
#include "ossfuzz_libpff.h"

#if !defined( LIBPFF_HAVE_BFIO )

/* Opens a file using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBPFF_EXTERN \
int libpff_file_open_file_io_handle(
      libpff_file_t *file,
      libbfio_handle_t *file_io_handle,
      int access_flags,
      libpff_error_t **error );

#endif /* !defined( LIBPFF_HAVE_BFIO ) */

/* Limits on the traversal to bound the time a single input takes
 */
#define FUZZER_SUB_ITEM_LIMIT		8
#define FUZZER_RECORD_SET_LIMIT		4
#define FUZZER_RECORD_ENTRY_LIMIT	16
#define FUZZER_MULTI_VALUE_LIMIT	8
#define FUZZER_STRING_SIZE_LIMIT	( (size_t) 0x4000 )

/* Decodes the data of a record entry using the typed accessors
 * Returns 1 if successful or -1 on error
 */
static int entry_decode_fuzzer(
              libpff_record_entry_t *record_entry )
{
	uint8_t value_boolean           = 0;
	uint16_t value_16bit            = 0;
	uint32_t value_32bit            = 0;
	uint64_t value_64bit            = 0;
	uint64_t value_filetime         = 0;
	uint64_t value_floatingtime     = 0;
	size64_t value_size             = 0;
	double value_floating_point     = 0;
	uint8_t *guid_data              = NULL;
	size_t guid_data_size           = 0;
	size_t utf8_string_size         = 0;
	size_t utf16_string_size        = 0;
	uint8_t *utf8_string            = NULL;
	uint16_t *utf16_string          = NULL;
	uint8_t *value_data             = NULL;
	size_t value_data_size          = 0;
	libpff_multi_value_t *multi_value = NULL;
	int result                      = 1;

	guid_data_size = 16;
	guid_data      = (uint8_t *) malloc( guid_data_size );

	value_data_size = 16;
	value_data      = (uint8_t *) malloc( value_data_size );

	/* When the value_data buffer is NULL or too small for the requested
	 * value, it is reallocated to the (capped) size
	 */
	if( value_data != NULL )
	{
		if( libpff_record_entry_get_data_size(
		     record_entry,
		     &value_data_size,
		     NULL ) == 1
		 && value_data_size > 0 )
		{
			if( value_data_size > (size_t) 0x00100000 )
			{
				value_data_size = (size_t) 0x00100000;
			}
			if( value_data_size > 16 )
			{
				uint8_t *new_value_data = (uint8_t *) malloc( value_data_size );

				if( new_value_data == NULL )
				{
					value_data_size = 16;
				}
				else
				{
					free( value_data );

					value_data = new_value_data;
				}
			}
			if( value_data != NULL
			 && libpff_record_entry_get_data(
				      record_entry,
				      value_data,
				      value_data_size,
				      NULL ) == 1 )
			{
				/* Use the value as a sink to keep it live
				 */
				value_boolean = value_data[ 0 ];
			}
		}
	}
	libpff_record_entry_get_data_as_boolean(
	 record_entry,
	 &value_boolean,
	 NULL );
	libpff_record_entry_get_data_as_16bit_integer(
	 record_entry,
	 &value_16bit,
	 NULL );
	libpff_record_entry_get_data_as_32bit_integer(
	 record_entry,
	 &value_32bit,
	 NULL );
	libpff_record_entry_get_data_as_64bit_integer(
	 record_entry,
	 &value_64bit,
	 NULL );
	libpff_record_entry_get_data_as_filetime(
	 record_entry,
	 &value_filetime,
	 NULL );
	libpff_record_entry_get_data_as_floatingtime(
	 record_entry,
	 &value_floatingtime,
	 NULL );
	libpff_record_entry_get_data_as_size(
	 record_entry,
	 &value_size,
	 NULL );
	libpff_record_entry_get_data_as_floating_point(
	 record_entry,
	 &value_floating_point,
	 NULL );
	if( guid_data != NULL )
	{
		libpff_record_entry_get_data_as_guid(
		 record_entry,
		 guid_data,
		 guid_data_size,
		 NULL );
	}
	if( libpff_record_entry_get_data_as_utf8_string_size(
	     record_entry,
	     &utf8_string_size,
	     NULL ) == 1 )
	{
		if( utf8_string_size > 0 )
		{
			if( utf8_string_size > (size_t) 0x00100000 )
			{
				utf8_string_size = (size_t) 0x00100000;
			}
			utf8_string = (uint8_t *) malloc( utf8_string_size );

			if( utf8_string != NULL )
			{
				if( libpff_record_entry_get_data_as_utf8_string(
				     record_entry,
				     utf8_string,
				     utf8_string_size,
				     NULL ) == 1 )
				{
					utf8_string[ utf8_string_size - 1 ] = 0;
				}
				free( utf8_string );
			}
			utf8_string = NULL;
		}
	}
	if( libpff_record_entry_get_data_as_utf16_string_size(
	     record_entry,
	     &utf16_string_size,
	     NULL ) == 1 )
	{
		if( utf16_string_size > 0 )
		{
			if( utf16_string_size > (size_t) 0x00100000 )
			{
				utf16_string_size = (size_t) 0x00100000;
			}
			utf16_string = (uint16_t *) malloc( utf16_string_size );

			if( utf16_string != NULL )
			{
				if( libpff_record_entry_get_data_as_utf16_string(
				     record_entry,
				     utf16_string,
				     utf16_string_size,
				     NULL ) == 1 )
				{
					utf16_string[ utf16_string_size / 2 - 1 ] = 0;
				}
				free( utf16_string );
			}
			utf16_string = NULL;
		}
	}
	if( libpff_record_entry_get_multi_value(
	     record_entry,
	     &multi_value,
	     NULL ) == 1 )
	{
		int number_of_values = 0;

		if( libpff_multi_value_get_number_of_values(
		     multi_value,
		     &number_of_values,
		     NULL ) == 1 )
		{
			for( int value_index = 0;
			     value_index < number_of_values;
			     value_index++ )
			{
				uint32_t value_type   = 0;
				uint8_t *value_data_2 = NULL;
				size_t value_data_size_2 = 0;

				if( value_index >= FUZZER_MULTI_VALUE_LIMIT )
				{
					break;
				}
				if( libpff_multi_value_get_value(
				     multi_value,
				     value_index,
				     &value_type,
				     &value_data_2,
				     &( value_data_size_2 ),
				     NULL ) == 1 )
				{
					if( value_data_size_2 > 0
					 && value_data_2 != NULL )
					{
						/* Use the value as a sink to keep it live
						 */
						value_boolean = value_data_2[ 0 ];
					}
				}
				else if( libpff_multi_value_get_value_32bit(
				             multi_value,
				             value_index,
				             &value_32bit,
				             NULL ) != 1 )
				{
					libpff_multi_value_get_value_64bit(
					 multi_value,
					 value_index,
					 &value_64bit,
					 NULL );
				}
			}
		}
		libpff_multi_value_free(
		 &multi_value,
		 NULL );
	}
	free( value_data );
	free( guid_data );

	return( result );
}

int LLVMFuzzerTestOneInput(
      const uint8_t *data,
      size_t size )
{
	libbfio_handle_t *file_io_handle = NULL;
	libpff_file_t *file              = NULL;
	libpff_item_t *root_folder       = NULL;
	libpff_item_t *sub_item          = NULL;
	libpff_record_set_t *record_set  = NULL;
	libpff_record_entry_t *record_entry = NULL;
	int number_of_sub_items          = 0;
	int number_of_record_sets        = 0;
	int number_of_entries            = 0;

	if( libbfio_memory_range_initialize(
	     &file_io_handle,
	     NULL ) != 1 )
	{
		return( 0 );
	}
	if( libbfio_memory_range_set(
	     file_io_handle,
	     (uint8_t *) data,
	     size,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libpff_file_initialize(
	     &file,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libpff_file_open_file_io_handle(
	     file,
	     file_io_handle,
	     LIBPFF_OPEN_READ,
	     NULL ) != 1 )
	{
		goto on_error_libpff_file;
	}
	if( libpff_file_get_root_folder(
	     file,
	     &root_folder,
	     NULL ) == 1 )
	{
		if( libpff_item_get_number_of_sub_items(
		     root_folder,
		     &number_of_sub_items,
		     NULL ) == 1 )
		{
			for( int sub_item_index = 0;
			     sub_item_index < number_of_sub_items;
			     sub_item_index++ )
			{
				if( sub_item_index >= FUZZER_SUB_ITEM_LIMIT )
				{
					break;
				}
				sub_item = NULL;

				if( libpff_item_get_sub_item(
				     root_folder,
				     sub_item_index,
				     &sub_item,
				     NULL ) != 1 )
				{
					continue;
				}
				if( libpff_item_get_number_of_record_sets(
				     sub_item,
				     &number_of_record_sets,
				     NULL ) == 1 )
				{
					for( int record_set_index = 0;
					     record_set_index < number_of_record_sets;
					     record_set_index++ )
					{
						if( record_set_index >= FUZZER_RECORD_SET_LIMIT )
						{
							break;
						}
						record_set = NULL;

						if( libpff_item_get_record_set_by_index(
						     sub_item,
						     record_set_index,
						     &record_set,
						     NULL ) != 1 )
						{
							continue;
						}
						if( libpff_record_set_get_number_of_entries(
						     record_set,
						     &number_of_entries,
						     NULL ) == 1
						 && number_of_entries > 0 )
						{
							if( ( size > 2 )
							 && ( sub_item_index == 0 )
							 && ( record_set_index == 0 ) )
							{
								uint32_t entry_type = (uint32_t) data[ 0 ]
								                    | ( (uint32_t) data[ 1 ] << 8 );

								static const uint32_t value_types[] = {
									  0x000b, /* BOOL */
									  0x0002, /* INT16 */
									  0x0003, /* INT32 */
									  0x0004, /* FLT32 */
									  0x0014, /* INT64 */
									  0x001f, /* STRING_UNICODE */
									  0x0040  /* FILETIME */
									};
								uint32_t value_type = value_types[ data[ 2 ] % 7 ];

								record_entry = NULL;

								libpff_record_set_get_entry_by_type(
								 record_set,
								 entry_type,
								 value_type,
								 &record_entry,
								 0,
								 NULL );

								if( record_entry != NULL )
								{
									entry_decode_fuzzer( record_entry );

									libpff_record_entry_free(
									 &record_entry,
									 NULL );
								}
								record_entry = NULL;

								libpff_record_set_get_entry_by_type(
								 record_set,
								 entry_type,
								 value_type,
								 &record_entry,
								 LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
								 NULL );

								if( record_entry != NULL )
								{
									entry_decode_fuzzer( record_entry );

									libpff_record_entry_free(
									 &record_entry,
									 NULL );
								}
							}
							for( int entry_index = 0;
							     entry_index < number_of_entries;
							     entry_index++ )
							{
								if( entry_index >= FUZZER_RECORD_ENTRY_LIMIT )
								{
									break;
								}
								record_entry = NULL;

								if( libpff_record_set_get_entry_by_index(
									     record_set,
									     entry_index,
									     &record_entry,
									     NULL ) == 1
								 && record_entry != NULL )
								{
									entry_decode_fuzzer( record_entry );

									libpff_record_entry_free(
									 &record_entry,
									 NULL );
								}
							}
						}
						libpff_record_set_free(
						 &record_set,
						 NULL );
					}
				}
				libpff_item_free(
				 &sub_item,
				 NULL );
			}
		}
		libpff_item_free(
		 &root_folder,
		 NULL );
	}
	libpff_file_close(
	 file,
	 NULL );

on_error_libpff_file:
	libpff_file_free(
	 &file,
	 NULL );

on_error_libbfio:
	libbfio_handle_free(
	 &file_io_handle,
	 NULL );

	return( 0 );
}

} /* extern "C" */
