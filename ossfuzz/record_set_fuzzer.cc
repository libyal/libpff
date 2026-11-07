/*
 * OSS-Fuzz target for libpff record set type
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

int LLVMFuzzerTestOneInput(
      const uint8_t *data,
      size_t size )
{
	static const uint32_t value_types[] = {
	      0x000b, /* BOOL */
	      0x0002, /* INT16 */
	      0x0003, /* INT32 */
	      0x0004, /* FLT32 */
	      0x0014, /* INT64 */
	      0x001f, /* STRING_UNICODE */
	      0x0040  /* FILETIME */
	};

	libbfio_handle_t *file_io_handle    = NULL;
	libpff_file_t *file                 = NULL;
	libpff_item_t *root_folder          = NULL;
	libpff_record_entry_t *record_entry = NULL;
	libpff_record_set_t *record_set     = NULL;
	uint32_t entry_type                 = 0;
	uint32_t value_type                 = 0;
	int number_of_entries               = 0;
	int number_of_record_sets           = 0;

	if( size > 2 )
	{
		/* Determines the entry type and value type from the input
		 */
		entry_type = (uint32_t) data[ 0 ]
		                    | ( (uint32_t) data[ 1 ] << 8 );
		value_type = value_types[ data[ 2 ] % 7 ];
	}
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
		if( libpff_item_get_number_of_record_sets(
		     root_folder,
		     &number_of_record_sets,
		     NULL ) != 1 )
		{
			goto on_error_libpff_root_folder;
		}
		if( number_of_record_sets > 0 )
		{
			if( libpff_item_get_record_set_by_index(
			     root_folder,
			     0,
			     &record_set,
			     NULL ) != 1 )
			{
				goto on_error_libpff_root_folder;
			}
			if( libpff_record_set_get_number_of_entries(
			     record_set,
			     &number_of_entries,
			     NULL ) != 1 )
			{
				goto on_error_libpff_record_set;
			}
			if( number_of_entries > 0 )
			{
				record_entry = NULL;

				if( libpff_record_set_get_entry_by_index(
				     record_set,
				     0,
				     &record_entry,
				     NULL ) == 1
				 && record_entry != NULL )
				{
					libpff_record_entry_free(
					 &record_entry,
					 NULL );
				}
			}
			record_entry = NULL;

			/* Retrieves an entry by entry type and value type
			 * using explicit value type matching
			 */
			libpff_record_set_get_entry_by_type(
			 record_set,
			 entry_type,
			 value_type,
			 &record_entry,
			 0,
			 NULL );
			if( record_entry != NULL )
			{
				libpff_record_entry_free(
				 &record_entry,
				 NULL );
			}
			record_entry = NULL;

			/* Retrieves an entry by entry type and value type
			 * matching any value type
			 */
			libpff_record_set_get_entry_by_type(
			 record_set,
			 entry_type,
			 value_type,
			 &record_entry,
			 LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
			 NULL );
			if( record_entry != NULL )
			{
				libpff_record_entry_free(
				 &record_entry,
				 NULL );
			}
		}
	on_error_libpff_record_set:
		libpff_record_set_free(
		 &record_set,
		 NULL );
	on_error_libpff_root_folder:
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
