/*
 * OSS-Fuzz target for libpff item recovery
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

/* Limits on the traversal to bound the time a single input takes
 */
#define FUZZER_RECOVERED_ITEM_LIMIT	4
#define FUZZER_ORPHAN_ITEM_LIMIT	4

int LLVMFuzzerTestOneInput(
      const uint8_t *data,
      size_t size )
{
	libbfio_handle_t *file_io_handle = NULL;
	libpff_file_t *file              = NULL;
	libpff_item_t *recovered_item    = NULL;
	libpff_item_t *orphan_item       = NULL;
	libpff_record_set_t *record_set  = NULL;
	libpff_record_entry_t *record_entry = NULL;
	uint8_t recovery_flags           = 0;
	int number_of_record_sets        = 0;
	int number_of_entries            = 0;
	int number_of_recovered_items    = 0;
	int number_of_orphan_items       = 0;

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
	/* Recovers deleted items
	 * Must be called immediately after open, before any item access
	 */
	if( size > 0 )
	{
		recovery_flags = data[ 0 ] & 0x03;
	}
	if( libpff_file_recover_items(
	     file,
	     recovery_flags,
	     NULL ) == 1 )
	{
		if( libpff_file_get_number_of_recovered_items(
		     file,
		     &number_of_recovered_items,
		     NULL ) == 1 )
		{
			for( int recovered_item_index = 0;
			     recovered_item_index < number_of_recovered_items;
			     recovered_item_index++ )
			{
				if( recovered_item_index >= FUZZER_RECOVERED_ITEM_LIMIT )
				{
					break;
				}
				recovered_item = NULL;

				if( libpff_file_get_recovered_item_by_index(
				     file,
				     recovered_item_index,
				     &recovered_item,
				     NULL ) == 1
				 && recovered_item != NULL )
				{
					if( libpff_item_get_number_of_record_sets(
					     recovered_item,
					     &number_of_record_sets,
					     NULL ) == 1
					 && number_of_record_sets > 0 )
					{
						if( libpff_item_get_record_set_by_index(
						     recovered_item,
						     0,
						     &record_set,
						     NULL ) == 1
						 && record_set != NULL )
						{
							if( libpff_record_set_get_number_of_entries(
							     record_set,
							     &number_of_entries,
							     NULL ) == 1
							)
							{
								record_entry = NULL;

								if( libpff_record_set_get_entry_by_index(
								     record_set,
								     0,
								     &record_entry,
								     NULL ) == 1
								)
								{
									libpff_record_entry_free(
									&record_entry,
									NULL );
								}
							}
							libpff_record_set_free(
							 &record_set,
							 NULL );
						}
					}
					libpff_item_free(
					 &recovered_item,
					 NULL );
				}
			}
		}
		if( libpff_file_get_number_of_orphan_items(
		     file,
		     &number_of_orphan_items,
		     NULL ) == 1 )
		{
			for( int orphan_item_index = 0;
			     orphan_item_index < number_of_orphan_items;
			     orphan_item_index++ )
			{
				if( orphan_item_index >= FUZZER_ORPHAN_ITEM_LIMIT )
				{
					break;
				}
				orphan_item = NULL;

				if( libpff_file_get_orphan_item_by_index(
				     file,
				     orphan_item_index,
				     &orphan_item,
				     NULL ) == 1
				 && orphan_item != NULL )
				{
					if( libpff_item_get_number_of_record_sets(
					     orphan_item,
					     &number_of_record_sets,
					     NULL ) == 1
					 && number_of_record_sets > 0 )
					{
						if( libpff_item_get_record_set_by_index(
						     orphan_item,
						     0,
						     &record_set,
						     NULL ) == 1
						 && record_set != NULL )
						{
							if( libpff_record_set_get_number_of_entries(
							     record_set,
							     &number_of_entries,
							     NULL ) == 1
							)
							{
								record_entry = NULL;

								if( libpff_record_set_get_entry_by_index(
								     record_set,
								     0,
								     &record_entry,
								     NULL ) == 1
								)
								{
									libpff_record_entry_free(
									&record_entry,
									NULL );
								}
							}
							libpff_record_set_free(
							 &record_set,
							 NULL );
						}
					}
					libpff_item_free(
					 &orphan_item,
					 NULL );
				}
			}
		}
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
