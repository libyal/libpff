/*
 * OSS-Fuzz target for libpff item type
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

int LLVMFuzzerTestOneInput(
     const uint8_t *data,
     size_t size )
{
	libbfio_handle_t *file_io_handle = NULL;
	libpff_file_t *file              = NULL;
	libpff_item_t *root_folder       = NULL;
	libpff_item_t *sub_item          = NULL;
	libpff_item_t *recipients        = NULL;
	uint8_t *message_body            = NULL;
	size_t message_body_size         = 0;
	int number_of_sub_items          = 0;
	int number_of_sub_folders        = 0;
	int number_of_sub_messages       = 0;
	int number_of_sub_contents       = 0;
	int number_of_attachments        = 0;

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
		     NULL ) != 1 )
		{
			goto on_error_libpff_root_folder;
		}
		for( int sub_item_index = 0;
		     sub_item_index < number_of_sub_items;
		     sub_item_index++ )
		{
			if( sub_item_index >= 8 )
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
				goto on_error_libpff_root_folder;
			}
			{
				uint32_t number_of_entries = 0;

				libpff_item_get_number_of_entries(
				 sub_item,
				 &number_of_entries,
				 NULL );

				libpff_folder_get_number_of_sub_folders(
				 sub_item,
				 &number_of_sub_folders,
				 NULL );

				libpff_folder_get_number_of_sub_messages(
				 sub_item,
				 &number_of_sub_messages,
				 NULL );

				libpff_folder_get_number_of_sub_associated_contents(
				 sub_item,
				 &number_of_sub_contents,
				 NULL );

				libpff_message_get_number_of_attachments(
				 sub_item,
				 &number_of_attachments,
				 NULL );
			}
			recipients = NULL;

			if( libpff_message_get_recipients(
			     sub_item,
			     &recipients,
			     NULL ) == 1
			 && recipients != NULL )
			{
				libpff_item_free(
				 &recipients,
				 NULL );
			}
			message_body_size = 0;

			if( libpff_message_get_plain_text_body_size(
			     sub_item,
			     &message_body_size,
			     NULL ) == 1
			 && message_body_size > 0 )
			{
				if( message_body_size > (size_t) 0x4000 )
				{
					message_body_size = (size_t) 0x4000;
				}
				message_body = (uint8_t *) malloc( message_body_size );

				if( message_body != NULL
				 && libpff_message_get_plain_text_body(
					      sub_item,
					      message_body,
					      message_body_size,
					      NULL ) == 1 )
				{
					/* Use the body as a sink to keep it live
					 */
					message_body[ message_body_size - 1 ] = 0;
				}
				free( message_body );

				message_body = NULL;
			}
			libpff_item_free(
			 &sub_item,
			 NULL );
		}
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

