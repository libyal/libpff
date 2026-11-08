/*
 * OSS-Fuzz target for libpff message type
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

/* Limits the number of sub items to walk
 * to bound the time a single input takes
 */
#define FUZZER_SUB_ITEM_LIMIT	8

/* Limits the depth of the sub item search
 * to bound the time a single input takes
 */
#define FUZZER_SEARCH_DEPTH	2

/* Limits the size of the message body buffer
 */
#define FUZZER_MESSAGE_BODY_SIZE_LIMIT	( (size_t) 0x4000 )

/* Determines if the specified item type is a message type
 * Returns 1 if a message type or 0 if not a message type
 */
static int message_fuzzer_is_message_type(
               uint8_t item_type )
{
	switch( item_type )
	{
		case LIBPFF_ITEM_TYPE_UNDEFINED:
		case LIBPFF_ITEM_TYPE_ATTACHMENT:
		case LIBPFF_ITEM_TYPE_ATTACHMENTS:
		case LIBPFF_ITEM_TYPE_COMMON:
		case LIBPFF_ITEM_TYPE_CONFIGURATION:
		case LIBPFF_ITEM_TYPE_FOLDER:
		case LIBPFF_ITEM_TYPE_RECIPIENTS:
		case LIBPFF_ITEM_TYPE_SUB_ASSOCIATED_CONTENTS:
		case LIBPFF_ITEM_TYPE_SUB_FOLDERS:
		case LIBPFF_ITEM_TYPE_SUB_MESSAGES:
		case LIBPFF_ITEM_TYPE_UNKNOWN:
			return( 0 );

		default:
			return( 1 );
	}
}

/* Searches for a message item within the parent item and its sub items
 * Returns 1 if successful, 0 if no message item was found or -1 on error
 */
static int message_fuzzer_find_message(
               libpff_item_t *parent_item,
               int search_depth,
               libpff_item_t **message )
{
	libpff_item_t *sub_item             = NULL;
	uint8_t item_type                  = 0;
	int number_of_sub_items           = 0;
	int number_of_sub_messages        = 0;
	int sub_item_index                = 0;

	if( parent_item == NULL
	 || search_depth < 0
	 || message == NULL )
	{
		return( -1 );
	}
	/* A folder may contain sub messages directly
	 */
	number_of_sub_messages = 0;

	if( libpff_folder_get_number_of_sub_messages(
	     parent_item,
	     &number_of_sub_messages,
	     NULL ) == 1
	 && number_of_sub_messages > 0 )
	{
		if( libpff_folder_get_sub_message(
		     parent_item,
		     0,
		     message,
		     NULL ) == 1
		 && ( *message != NULL ) )
		{
			return( 1 );
		}
	}
	if( search_depth == 0 )
	{
		return( 0 );
	}
	number_of_sub_items = 0;

	if( libpff_item_get_number_of_sub_items(
	     parent_item,
	     &number_of_sub_items,
	     NULL ) != 1
	 || number_of_sub_items <= 0 )
	{
		return( 0 );
	}
	for( sub_item_index = 0;
	     sub_item_index < number_of_sub_items;
	     sub_item_index++ )
	{
		if( sub_item_index >= FUZZER_SUB_ITEM_LIMIT )
		{
			break;
		}
		sub_item = NULL;

		if( libpff_item_get_sub_item(
		     parent_item,
		     sub_item_index,
		     &sub_item,
		     NULL ) != 1 )
		{
			continue;
		}
		if( message_fuzzer_find_message(
		     sub_item,
		     search_depth - 1,
		     message ) == 1 )
		{
			libpff_item_free(
			 &sub_item,
			 NULL );
			return( 1 );
		}
		item_type = 0;

		libpff_item_get_type(
		 sub_item,
		 &item_type,
		 NULL );
		/* The sub item may itself be a message,
		 * e.g. a message stored directly in a folder
		 */
		if( message_fuzzer_is_message_type(
		     item_type ) )
		{
			*message = sub_item;
			return( 1 );
		}
		libpff_item_free(
		 &sub_item,
		 NULL );
	}
	return( 0 );
}

int LLVMFuzzerTestOneInput(
      const uint8_t *data,
      size_t size )
{
	uint8_t *message_body        = NULL;

	libbfio_handle_t *file_io_handle = NULL;
	libpff_file_t *file              = NULL;
	libpff_item_t *message           = NULL;
	libpff_item_t *recipients        = NULL;
	libpff_item_t *root_folder       = NULL;
	size_t message_body_size         = 0;
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
		message = NULL;

		if( message_fuzzer_find_message(
		     root_folder,
		     FUZZER_SEARCH_DEPTH,
		     &message ) == 1
		 && message != NULL )
		{
			if( libpff_message_get_number_of_attachments(
			     message,
			     &number_of_attachments,
			     NULL ) != 1 )
			{
				goto on_error_libpff_message;
			}
			recipients = NULL;

			if( libpff_message_get_recipients(
			     message,
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
			     message,
			     &message_body_size,
			     NULL ) == 1
			 && message_body_size > 0 )
			{
				if( message_body_size > FUZZER_MESSAGE_BODY_SIZE_LIMIT )
				{
					message_body_size = FUZZER_MESSAGE_BODY_SIZE_LIMIT;
				}
				message_body = (uint8_t *) malloc( message_body_size );

				if( message_body != NULL
				 && libpff_message_get_plain_text_body(
					        message,
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
 on_error_libpff_message:
			libpff_item_free(
			 &message,
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
