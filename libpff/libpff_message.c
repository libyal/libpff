/*
 * Message functions
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
#include <memory.h>
#include <types.h>

#include "libpff_debug.h"
#include "libpff_definitions.h"
#include "libpff_item.h"
#include "libpff_item_descriptor.h"
#include "libpff_item_tree.h"
#include "libpff_item_values.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_libfmapi.h"
#include "libpff_mapi.h"
#include "libpff_message.h"
#include "libpff_item.h"
#include "libpff_libuna.h"
#include "libpff_record_entry.h"
#include "libpff_value_type.h"

#define LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS	0
#define LIBPFF_MESSAGE_SUB_ITEM_RECIPIENTS	1

/* Creates a sub item attachments
 * Returns 1 if successful or -1 on error
 */
int libpff_message_initialize_sub_item_attachments(
     libpff_internal_item_t *internal_item,
     libpff_item_descriptor_t *item_descriptor,
     libpff_local_descriptor_value_t *local_descriptor_value,
     libcerror_error_t **error )
{
	libpff_item_descriptor_t *attachments_item_descriptor = NULL;
	static char *function                                 = "libpff_message_initialize_sub_item_attachments";

#if defined( HAVE_DEBUG_OUTPUT )
	const char *node_identifier_type                      = NULL;
#endif

	if( internal_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item.",
		 function );

		return( -1 );
	}
	if( item_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item descriptor.",
		 function );

		return( -1 );
	}
	if( local_descriptor_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid local descriptor value.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		node_identifier_type = libpff_debug_get_node_identifier_type(
		                        (uint8_t) ( local_descriptor_value->identifier & 0x0000001fUL ) );

		libcnotify_printf(
		 "%s: local descriptor identifier: %" PRIu64 " (%s), data: %" PRIu64 ", local descriptors: %" PRIu64 "\n",
		 function,
		 local_descriptor_value->identifier,
		 node_identifier_type,
		 local_descriptor_value->data_identifier,
		 local_descriptor_value->local_descriptors_identifier );
	}
#endif
	if( libpff_item_values_initialize(
	     &( internal_item->sub_item_values[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ] ),
	     (uint32_t) LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_ATTACHMENTS,
	     local_descriptor_value->data_identifier,
	     local_descriptor_value->local_descriptors_identifier ,
	     item_descriptor->recovered,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create attachments item values.",
		 function );

		goto on_error;
	}
	/* Cache the attachments items values for successive usage
	 */
	if( libpff_item_values_read(
	     internal_item->sub_item_values[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ],
	     internal_item->name_to_id_map_list,
	     internal_item->io_handle,
	     internal_item->file_io_handle,
	     internal_item->offsets_index,
	     LIBPFF_DEBUG_ITEM_TYPE_DEFAULT,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read attachments local descriptor recipients identifier: %" PRIu32 ".",
		 function,
		 LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_ATTACHMENTS );

		goto on_error;
	}
	if( internal_item->sub_item_values[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ]->table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid attachments item values - missing table.",
		 function );

		goto on_error;
	}
	/* Create an attachments item descriptor
	 */
	if( libpff_item_descriptor_initialize(
	     &attachments_item_descriptor,
	     (uint32_t) LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_ATTACHMENTS,
	     local_descriptor_value->data_identifier,
	     local_descriptor_value->local_descriptors_identifier ,
	     item_descriptor->recovered,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create attachments item descriptor.",
		 function );

		goto on_error;
	}
	if( libcdata_tree_node_initialize(
	     &( internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ] ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create attachments sub item tree node.",
		 function );

		goto on_error;
	}
	if( libcdata_tree_node_set_value(
	     internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ],
	     (intptr_t *) attachments_item_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set attachments item descriptor in attachments sub item tree node.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( attachments_item_descriptor != NULL )
	{
		memory_free(
		 attachments_item_descriptor );
	}
	libcdata_tree_node_free(
	 &( internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ] ),
	 NULL,
	 NULL );

	return( -1 );
}

/* Creates a sub item recipients
 * Returns 1 if successful or -1 on error
 */
int libpff_message_initialize_sub_item_recipients(
     libpff_internal_item_t *internal_item,
     libpff_item_descriptor_t *item_descriptor,
     libpff_local_descriptor_value_t *local_descriptor_value,
     libcerror_error_t **error )
{
	libpff_item_descriptor_t *recipients_item_descriptor = NULL;
	static char *function                                = "libpff_message_initialize_sub_item_recipients";

#if defined( HAVE_DEBUG_OUTPUT )
	const char *node_identifier_type                     = NULL;
#endif

	if( internal_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item.",
		 function );

		return( -1 );
	}
	if( item_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item descriptor.",
		 function );

		return( -1 );
	}
	if( local_descriptor_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid local descriptor value.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		node_identifier_type = libpff_debug_get_node_identifier_type(
		                        (uint8_t) ( local_descriptor_value->identifier & 0x0000001fUL ) ),

		libcnotify_printf(
		 "%s: local descriptor identifier: %" PRIu64 " (%s), data: %" PRIu64 ", local descriptors: %" PRIu64 "\n",
		 function,
		 local_descriptor_value->identifier,
		 node_identifier_type,
		 local_descriptor_value->data_identifier,
		 local_descriptor_value->local_descriptors_identifier );
	}
#endif
	if( libpff_item_values_initialize(
	     &( internal_item->sub_item_values[ LIBPFF_MESSAGE_SUB_ITEM_RECIPIENTS ] ),
	     LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_RECIPIENTS,
	     local_descriptor_value->data_identifier,
	     local_descriptor_value->local_descriptors_identifier,
	     item_descriptor->recovered,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create recipients item values.",
		 function );

		goto on_error;
	}
	/* Cache the recipients items values for successive usage
	 */
	if( libpff_item_values_read(
	     internal_item->sub_item_values[ LIBPFF_MESSAGE_SUB_ITEM_RECIPIENTS ],
	     internal_item->name_to_id_map_list,
	     internal_item->io_handle,
	     internal_item->file_io_handle,
	     internal_item->offsets_index,
	     LIBPFF_DEBUG_ITEM_TYPE_DEFAULT,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read recipients local descriptor identifier: %" PRIu32 ".",
		 function,
		 LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_RECIPIENTS );

		goto on_error;
	}
	if( internal_item->sub_item_values[ LIBPFF_MESSAGE_SUB_ITEM_RECIPIENTS ]->table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid recipients item values - missing table.",
		 function );

		goto on_error;
	}
	/* Create an recipients item descriptor
	 */
	if( libpff_item_descriptor_initialize(
	     &recipients_item_descriptor,
	     LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_RECIPIENTS,
	     local_descriptor_value->data_identifier,
	     local_descriptor_value->local_descriptors_identifier ,
	     item_descriptor->recovered,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create recipients item descriptor.",
		 function );

		goto on_error;
	}
	if( libcdata_tree_node_initialize(
	     &( internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_RECIPIENTS ] ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create recipients sub item tree node.",
		 function );

		goto on_error;
	}
	if( libcdata_tree_node_set_value(
	     internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_RECIPIENTS ],
	     (intptr_t *) recipients_item_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set recipients item descriptor in recipients sub item tree node.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( recipients_item_descriptor != NULL )
	{
		memory_free(
		 recipients_item_descriptor );
	}
	libcdata_tree_node_free(
	 &( internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_RECIPIENTS ] ),
	 NULL,
	 NULL );

	return( -1 );
}

/* Retrieves the UTF-8 string size of a specific entry
 * The size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_message_get_entry_value_utf8_string_size(
     libpff_item_t *message,
     uint32_t entry_type,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_message_get_entry_value_utf8_string_size";
	int result                            = 0;

	if( message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) message;

	if( internal_item->message_codepage == 0 )
	{
		if( libpff_internal_item_get_entry_value_32bit_integer(
		     internal_item,
		     LIBPFF_ENTRY_TYPE_MESSAGE_CODEPAGE,
		     &( internal_item->message_codepage ),
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the message codepage.",
			 function );

			return( -1 );
		}
		if( internal_item->message_codepage == 0 )
		{
			internal_item->message_codepage = internal_item->ascii_codepage;
		}
	}
	result = libpff_internal_item_get_entry_value_utf8_string_size(
	          internal_item,
	          entry_type,
	          internal_item->message_codepage,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-8 string value of a specific entry
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_message_get_entry_value_utf8_string(
     libpff_item_t *message,
     uint32_t entry_type,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_message_get_entry_value_utf8_string";
	int result                            = 0;

	if( message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) message;

	if( internal_item->message_codepage == 0 )
	{
		if( libpff_internal_item_get_entry_value_32bit_integer(
		     internal_item,
		     LIBPFF_ENTRY_TYPE_MESSAGE_CODEPAGE,
		     &( internal_item->message_codepage ),
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the message codepage.",
			 function );

			return( -1 );
		}
		if( internal_item->message_codepage == 0 )
		{
			internal_item->message_codepage = internal_item->ascii_codepage;
		}
	}
	result = libpff_internal_item_get_entry_value_utf8_string(
	          internal_item,
	          entry_type,
	          internal_item->message_codepage,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-16 string size of a specific entry
 * The size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_message_get_entry_value_utf16_string_size(
     libpff_item_t *message,
     uint32_t entry_type,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_message_get_entry_value_utf16_string_size";
	int result                            = 0;

	if( message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) message;

	if( internal_item->message_codepage == 0 )
	{
		if( libpff_internal_item_get_entry_value_32bit_integer(
		     internal_item,
		     LIBPFF_ENTRY_TYPE_MESSAGE_CODEPAGE,
		     &( internal_item->message_codepage ),
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the message codepage.",
			 function );

			return( -1 );
		}
		if( internal_item->message_codepage == 0 )
		{
			internal_item->message_codepage = internal_item->ascii_codepage;
		}
	}
	result = libpff_internal_item_get_entry_value_utf16_string_size(
	          internal_item,
	          entry_type,
	          internal_item->message_codepage,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-16 string value of a specific entry
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_message_get_entry_value_utf16_string(
     libpff_item_t *message,
     uint32_t entry_type,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_message_get_entry_value_utf16_string";
	int result                            = 0;

	if( message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) message;

	if( internal_item->message_codepage == 0 )
	{
		if( libpff_internal_item_get_entry_value_32bit_integer(
		     internal_item,
		     LIBPFF_ENTRY_TYPE_MESSAGE_CODEPAGE,
		     &( internal_item->message_codepage ),
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the message codepage.",
			 function );

			return( -1 );
		}
		if( internal_item->message_codepage == 0 )
		{
			internal_item->message_codepage = internal_item->ascii_codepage;
		}
	}
	result = libpff_internal_item_get_entry_value_utf16_string(
	          internal_item,
	          entry_type,
	          internal_item->message_codepage,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the 64-bit FILETIME value containing message client submit time
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_message_get_client_submit_time(
     libpff_item_t *message,
     uint64_t *filetime,
     libcerror_error_t **error )
{
	static char *function = "libpff_message_get_client_submit_time";
	int result            = 0;

	result = libpff_internal_item_get_entry_value_filetime(
	          (libpff_internal_item_t *) message,
	          LIBPFF_ENTRY_TYPE_MESSAGE_CLIENT_SUBMIT_TIME,
	          filetime,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve client submit time.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the 64-bit FILETIME value containing message delivery time
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_message_get_delivery_time(
     libpff_item_t *message,
     uint64_t *filetime,
     libcerror_error_t **error )
{
	static char *function = "libpff_message_get_delivery_time";
	int result            = 0;

	result = libpff_internal_item_get_entry_value_filetime(
	          (libpff_internal_item_t *) message,
	          LIBPFF_ENTRY_TYPE_MESSAGE_DELIVERY_TIME,
	          filetime,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve delivery time.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the 64-bit FILETIME value containing message creation time
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_message_get_creation_time(
     libpff_item_t *message,
     uint64_t *filetime,
     libcerror_error_t **error )
{
	static char *function = "libpff_message_get_creation_time";
	int result            = 0;

	result = libpff_internal_item_get_entry_value_filetime(
	          (libpff_internal_item_t *) message,
	          LIBPFF_ENTRY_TYPE_MESSAGE_CREATION_TIME,
	          filetime,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve creation time.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the 64-bit FILETIME value containing message modification time
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_message_get_modification_time(
     libpff_item_t *message,
     uint64_t *filetime,
     libcerror_error_t **error )
{
	static char *function = "libpff_message_get_modification_time";
	int result            = 0;

	result = libpff_internal_item_get_entry_value_filetime(
	          (libpff_internal_item_t *) message,
	          LIBPFF_ENTRY_TYPE_MESSAGE_MODIFICATION_TIME,
	          filetime,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve modification time.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Determine if the message item has a specific attachment
 * Returns 1 if successful or -1 on error
 */
int libpff_message_determine_attachment(
     libpff_internal_item_t *internal_item,
     libpff_item_descriptor_t *item_descriptor,
     int attachment_index,
     libcerror_error_t **error )
{
	libpff_local_descriptor_value_t *local_descriptor_value = NULL;
	libpff_record_entry_t *record_entry                     = NULL;
	static char *function                                   = "libpff_message_determine_attachment";
	uint32_t local_descriptor_identifier                    = 0;
	int result                                              = 0;

	if( internal_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item.",
		 function );

		return( -1 );
	}
	if( item_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item descriptor.",
		 function );

		return( -1 );
	}
	if( libpff_item_values_get_record_entry_by_type(
	     internal_item->sub_item_values[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ],
	     internal_item->name_to_id_map_list,
	     internal_item->io_handle,
	     internal_item->file_io_handle,
	     internal_item->offsets_index,
	     attachment_index,
	     LIBPFF_ENTRY_TYPE_SUB_ITEM_IDENTIFIER,
	     LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED,
	     &record_entry,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record entry: %d.",
		 function,
		 attachment_index );

		goto on_error;
	}
	if( libpff_record_entry_get_data_as_32bit_integer(
	     record_entry,
	     &local_descriptor_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve 32-bit integer value.",
		 function );

		goto on_error;
	}
	if( libpff_record_entry_free(
	     &record_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free record entry.",
		 function );

		goto on_error;
	}
	result = libpff_item_values_get_local_descriptors_value_by_identifier(
		  internal_item->item_values,
		  internal_item->file_io_handle,
		  local_descriptor_identifier,
		  &local_descriptor_value,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve local descriptor identifier: %" PRIu32 ".",
		 function,
		 local_descriptor_identifier );

		goto on_error;
	}
/* TODO error tollerability flag an attachment as missing if result == 0 */
	else if( result == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing local descriptor identifier: %" PRIu32 ".",
		 function,
		 local_descriptor_identifier );

		goto on_error;
	}
	if( local_descriptor_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid local descriptor value.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: local descriptor identifier: %" PRIu64 " (%s), data: %" PRIu64 ", local descriptors: %" PRIu64 "\n",
		 function,
		 local_descriptor_value->identifier,
		 libpff_debug_get_node_identifier_type(
		  (uint8_t) ( local_descriptor_value->identifier & 0x0000001fUL ) ),
		 local_descriptor_value->data_identifier,
		 local_descriptor_value->local_descriptors_identifier );
	}
#endif
	/* Make the attachment item the sub item of the attachments
	 */
	if( libpff_item_tree_append_identifier(
	     internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ],
	     local_descriptor_identifier,
	     local_descriptor_value->data_identifier,
	     local_descriptor_value->local_descriptors_identifier,
	     item_descriptor->recovered,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append attachment item: %" PRIu32 " to attachments item tree node.",
		 function,
		 local_descriptor_identifier );

		goto on_error;
	}
	if( libpff_local_descriptor_value_free(
	     &local_descriptor_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free local descriptor values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( record_entry != NULL )
	{
		libpff_record_entry_free(
		 &record_entry,
		 NULL );
	}
	if( local_descriptor_value != NULL )
	{
		libpff_local_descriptor_value_free(
		 &local_descriptor_value,
		 NULL );
	}
	return( -1 );
}

/* Determine if the message item has attachments
 * Returns 1 if successful or -1 on error
 */
int libpff_message_determine_attachments(
     libpff_internal_item_t *internal_item,
     libcerror_error_t **error )
{
	libpff_local_descriptor_value_t *local_descriptor_value = NULL;
	static char *function                                   = "libpff_message_determine_attachments";
	int attachment_index                                    = 0;
	int number_of_attachments                               = 0;
	int result                                              = 0;

	if( internal_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item.",
		 function );

		return( -1 );
	}
	if( internal_item->item_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing item values.",
		 function );

		return( -1 );
	}
	if( internal_item->item_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing item descriptor.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ] != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: attachments sub item tree node already set.",
		 function );

		return( -1 );
	}
	/* Make sure the item values have been read
	 */
/* TODO is this still needed ? */
	if( internal_item->item_values->table == NULL )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: reading item values of descriptor: %" PRIu32 "\n",
			 function,
			 internal_item->item_descriptor->descriptor_identifier );
		}
#endif
		if( libpff_item_values_read(
		     internal_item->item_values,
		     internal_item->name_to_id_map_list,
		     internal_item->io_handle,
		     internal_item->file_io_handle,
		     internal_item->offsets_index,
		     LIBPFF_DEBUG_ITEM_TYPE_DEFAULT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read item values.",
			 function );

			goto on_error;
		}
		if( internal_item->item_values->table == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid item values - missing table.",
			 function );

			goto on_error;
		}
	}
	/* Determine if the item has attachments
	 */
	result = libpff_item_values_get_local_descriptors_value_by_identifier(
		  internal_item->item_values,
		  internal_item->file_io_handle,
		  (uint32_t) LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_ATTACHMENTS,
		  &local_descriptor_value,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve local descriptor identifier: %" PRIu32 ".",
		 function,
		 (uint32_t) LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_ATTACHMENTS );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libpff_message_initialize_sub_item_attachments(
		     internal_item,
		     internal_item->item_descriptor,
		     local_descriptor_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create sub item attachments.",
			 function );

			goto on_error;
		}
		if( libpff_local_descriptor_value_free(
		     &local_descriptor_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free local descriptor values.",
			 function );

			goto on_error;
		}
		if( libpff_item_values_get_number_of_record_sets(
		     internal_item->sub_item_values[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ],
		     internal_item->name_to_id_map_list,
		     internal_item->io_handle,
		     internal_item->file_io_handle,
		     internal_item->offsets_index,
		     &number_of_attachments,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine the number of attachments.",
			 function );

			goto on_error;
		}
		for( attachment_index = 0;
		     attachment_index < number_of_attachments;
		     attachment_index++ )
		{
			if( libpff_message_determine_attachment(
			     internal_item,
			     internal_item->item_descriptor,
			     attachment_index,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine attachment: %d.",
				 function,
				 attachment_index );

				goto on_error;
			}
		}
	}
	return( 1 );

on_error:
	if( local_descriptor_value != NULL )
	{
		libpff_local_descriptor_value_free(
		 &local_descriptor_value,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the number of attachments from a message item
 * Returns 1 if successful or -1 on error
 */
int libpff_message_get_number_of_attachments(
     libpff_item_t *message,
     int *number_of_attachments,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_message_get_number_of_attachments";

	if( message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) message;

	if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
	{
		if( libpff_internal_item_determine_type(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine item type.",
			 function );

			return( -1 );
		}
	}
	if( ( internal_item->type == LIBPFF_ITEM_TYPE_ATTACHMENT )
	 || ( internal_item->type == LIBPFF_ITEM_TYPE_ATTACHMENTS )
	 || ( internal_item->type == LIBPFF_ITEM_TYPE_FOLDER )
	 || ( internal_item->type == LIBPFF_ITEM_TYPE_RECIPIENTS ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported item type: 0x%08" PRIx32 "",
		 function,
		 internal_item->type );

		return( -1 );
	}
	if( internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ] == NULL )
	{
		if( libpff_message_determine_attachments(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine attachments.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ] != NULL )
	{
		if( libcdata_tree_node_get_number_of_sub_nodes(
		     internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ],
		     number_of_attachments,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of sub nodes.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( number_of_attachments == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid number of attachments.",
			 function );

			return( -1 );
		}
		*number_of_attachments = 0;
	}
	return( 1 );
}

/* Retrieves the attachment for the specific index from a message item
 * Returns 1 if successful or -1 on error
 */
int libpff_message_get_attachment(
     libpff_item_t *message,
     int attachment_index,
     libpff_item_t **attachment,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *attachment_tree_node = NULL;
	libpff_internal_item_t *internal_item      = NULL;
	static char *function                      = "libpff_message_get_attachment";

	if( message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) message;

	if( internal_item->item_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid message - missing item values.",
		 function );

		return( -1 );
	}
	if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
	{
		if( libpff_internal_item_determine_type(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine item type.",
			 function );

			return( -1 );
		}
	}
	if( ( internal_item->type == LIBPFF_ITEM_TYPE_ATTACHMENT )
	 || ( internal_item->type == LIBPFF_ITEM_TYPE_ATTACHMENTS )
	 || ( internal_item->type == LIBPFF_ITEM_TYPE_FOLDER )
	 || ( internal_item->type == LIBPFF_ITEM_TYPE_RECIPIENTS ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported item type: 0x%08" PRIx32 "",
		 function,
		 internal_item->type );

		return( -1 );
	}
	if( attachment == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attachment.",
		 function );

		return( -1 );
	}
	if( *attachment != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: attachment already set.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ] == NULL )
	{
		if( libpff_message_determine_attachments(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine attachments.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ] != NULL )
	{
		if( (uint64_t) attachment_index > (uint64_t) UINT32_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid attachment index value out of bounds.",
			 function );

			return( -1 );
		}
		if( libcdata_tree_node_get_sub_node_by_index(
		     internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ],
		     attachment_index,
		     &attachment_tree_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attachment item tree node.",
			 function );

			return( -1 );
		}
		if( libpff_item_initialize(
		     attachment,
		     internal_item->io_handle,
		     internal_item->file_io_handle,
		     internal_item->name_to_id_map_list,
		     internal_item->descriptors_index,
		     internal_item->offsets_index,
		     internal_item->item_tree,
		     attachment_tree_node,
		     LIBPFF_ITEM_FLAGS_DEFAULT | LIBPFF_ITEM_FLAG_MANAGED_ITEM_TREE_NODE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create attachment.",
			 function );

			return( -1 );
		}
		if( *attachment == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid attachment.",
			 function );

			return( -1 );
		}
		( (libpff_internal_item_t *) *attachment )->type = LIBPFF_ITEM_TYPE_ATTACHMENT;
	}
	return( 1 );
}

/* Retrieves the attachments from a message item
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_message_get_attachments(
     libpff_item_t *message,
     libpff_item_t **attachments,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_message_get_attachments";

	if( message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) message;

	if( internal_item->item_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid message - missing item values.",
		 function );

		return( -1 );
	}
	if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
	{
		if( libpff_internal_item_determine_type(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine item type.",
			 function );

			return( -1 );
		}
	}
	if( ( internal_item->type == LIBPFF_ITEM_TYPE_ATTACHMENT )
	 || ( internal_item->type == LIBPFF_ITEM_TYPE_ATTACHMENTS )
	 || ( internal_item->type == LIBPFF_ITEM_TYPE_FOLDER )
	 || ( internal_item->type == LIBPFF_ITEM_TYPE_RECIPIENTS ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported item type: 0x%08" PRIx32 "",
		 function,
		 internal_item->type );

		return( -1 );
	}
	if( attachments == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attachments.",
		 function );

		return( -1 );
	}
	if( *attachments != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: attachments already set.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ] == NULL )
	{
		if( libpff_message_determine_attachments(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine attachments.",
			 function );

			goto on_error;
		}
	}
	if( internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ] == NULL )
	{
		return( 0 );
	}
	if( libpff_item_initialize(
	     attachments,
	     internal_item->io_handle,
	     internal_item->file_io_handle,
	     internal_item->name_to_id_map_list,
	     internal_item->descriptors_index,
	     internal_item->offsets_index,
	     internal_item->item_tree,
	     internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ],
	     LIBPFF_ITEM_FLAGS_DEFAULT | LIBPFF_ITEM_FLAG_MANAGED_ITEM_TREE_NODE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create attachments.",
		 function );

		goto on_error;
	}
	if( *attachments == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid attachments.",
		 function );

		goto on_error;
	}
	( (libpff_internal_item_t *) *attachments )->type = LIBPFF_ITEM_TYPE_ATTACHMENTS;

	/* Clones the item values sub elements from the cached sub item values
	 */
	if( libpff_item_values_clone_copy(
	     ( (libpff_internal_item_t *) *attachments )->item_values,
	     internal_item->sub_item_values[ LIBPFF_MESSAGE_SUB_ITEM_ATTACHMENTS ],
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy attachments item values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *attachments != NULL )
	{
		libpff_item_free(
		 attachments,
		 NULL );
	}
	return( -1 );
}

/* Determine if the message item has recipients
 * Returns 1 if successful or -1 on error
 */
int libpff_message_determine_recipients(
     libpff_internal_item_t *internal_item,
     libcerror_error_t **error )
{
	libpff_local_descriptor_value_t *local_descriptor_value = NULL;
	static char *function                                   = "libpff_message_determine_recipients";
	int result                                              = 0;

	if( internal_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item.",
		 function );

		return( -1 );
	}
	if( internal_item->item_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing item values.",
		 function );

		return( -1 );
	}
	if( internal_item->item_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing item descriptor.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_RECIPIENTS ] != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: recipients sub item tree node already set.",
		 function );

		return( -1 );
	}
	/* Make sure the item values have been read
	 */
	if( internal_item->item_values->table == NULL )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: reading item values of descriptor: %" PRIu32 "\n",
			 function,
			 internal_item->item_descriptor->descriptor_identifier );
		}
#endif
		if( libpff_item_values_read(
		     internal_item->item_values,
		     internal_item->name_to_id_map_list,
		     internal_item->io_handle,
		     internal_item->file_io_handle,
		     internal_item->offsets_index,
		     LIBPFF_DEBUG_ITEM_TYPE_DEFAULT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read item values.",
			 function );

			goto on_error;
		}
		if( internal_item->item_values->table == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid item values - missing table.",
			 function );

			goto on_error;
		}
	}
	/* Determine if the item has recipients
	 */
	result = libpff_item_values_get_local_descriptors_value_by_identifier(
		  internal_item->item_values,
		  internal_item->file_io_handle,
		  (uint32_t) LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_RECIPIENTS,
		  &local_descriptor_value,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve local descriptor identifier: %" PRIu32 ".",
		 function,
		 (uint32_t) LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_RECIPIENTS );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libpff_message_initialize_sub_item_recipients(
		     internal_item,
		     internal_item->item_descriptor,
		     local_descriptor_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create sub item recipients.",
			 function );

			goto on_error;
		}
		if( libpff_local_descriptor_value_free(
		     &local_descriptor_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free local descriptor values.",
			 function );

			goto on_error;
		}
		/* All the recipient data is in the recipients item
		 * there are no sub items like for the attachments item
		 */
	}
	return( 1 );

on_error:
	if( local_descriptor_value != NULL )
	{
		libpff_local_descriptor_value_free(
		 &local_descriptor_value,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the recipients from a message item
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_message_get_recipients(
     libpff_item_t *message,
     libpff_item_t **recipients,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_message_get_recipients";

	if( message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) message;

	if( internal_item->item_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid message - missing item values.",
		 function );

		return( -1 );
	}
	if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
	{
		if( libpff_internal_item_determine_type(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine item type.",
			 function );

			return( -1 );
		}
	}
	if( ( internal_item->type == LIBPFF_ITEM_TYPE_ATTACHMENT )
	 || ( internal_item->type == LIBPFF_ITEM_TYPE_ATTACHMENTS )
	 || ( internal_item->type == LIBPFF_ITEM_TYPE_FOLDER )
	 || ( internal_item->type == LIBPFF_ITEM_TYPE_RECIPIENTS ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported item type: 0x%08" PRIx32 "",
		 function,
		 internal_item->type );

		return( -1 );
	}
	if( recipients == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid recipients.",
		 function );

		return( -1 );
	}
	if( *recipients != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: recipients already set.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_RECIPIENTS ] == NULL )
	{
		if( libpff_message_determine_recipients(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine recipients.",
			 function );

			goto on_error;
		}
	}
	if( internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_RECIPIENTS ] == NULL )
	{
		return( 0 );
	}
	if( libpff_item_initialize(
	     recipients,
	     internal_item->io_handle,
	     internal_item->file_io_handle,
	     internal_item->name_to_id_map_list,
	     internal_item->descriptors_index,
	     internal_item->offsets_index,
	     internal_item->item_tree,
	     internal_item->sub_item_tree_node[ LIBPFF_MESSAGE_SUB_ITEM_RECIPIENTS ],
	     LIBPFF_ITEM_FLAGS_DEFAULT | LIBPFF_ITEM_FLAG_MANAGED_ITEM_TREE_NODE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create recipients.",
		 function );

		goto on_error;
	}
	if( *recipients == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid recipients.",
		 function );

		goto on_error;
	}
	( (libpff_internal_item_t *) *recipients )->type = LIBPFF_ITEM_TYPE_RECIPIENTS;

	/* Clones the item values sub elements from the cached sub item values
	 */
	if( libpff_item_values_clone_copy(
	     ( (libpff_internal_item_t *) *recipients )->item_values,
	     internal_item->sub_item_values[ LIBPFF_MESSAGE_SUB_ITEM_RECIPIENTS ],
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy recipients item values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *recipients != NULL )
	{
		libpff_item_free(
		 recipients,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the plain text message body size
 * Size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_message_get_plain_text_body_size(
     libpff_item_t *message,
     size_t *size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_message_get_plain_text_body_size";
	uint32_t message_codepage             = 0;
	uint32_t value_type                   = 0;
	int ascii_codepage                    = 0;
	int result                            = 0;

	if( message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) message;

	if( size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size.",
		 function );

		return( -1 );
	}
	if( internal_item->message_codepage == 0 )
	{
		if( libpff_internal_item_get_entry_value_32bit_integer(
		     internal_item,
		     LIBPFF_ENTRY_TYPE_MESSAGE_CODEPAGE,
		     &( internal_item->message_codepage ),
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the message codepage.",
			 function );

			goto on_error;
		}
		if( internal_item->message_codepage == 0 )
		{
			internal_item->message_codepage = internal_item->ascii_codepage;
		}
	}
	if( internal_item->message_body_codepage == 0 )
	{
		if( libpff_internal_item_get_entry_value_32bit_integer(
		     internal_item,
		     LIBPFF_ENTRY_TYPE_MESSAGE_BODY_CODEPAGE,
		     &( internal_item->message_body_codepage ),
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the message body codepage.",
			 function );

			goto on_error;
		}
		if( internal_item->message_body_codepage == 0 )
		{
			internal_item->message_body_codepage = internal_item->message_codepage;
		}
	}
	result = libpff_item_values_get_record_entry_by_type(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
	          0,
		  LIBPFF_ENTRY_TYPE_MESSAGE_BODY_PLAIN_TEXT,
	          0,
	          &record_entry,
	          LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record entry.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libpff_record_entry_get_value_type(
		     record_entry,
		     &value_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value type.",
			 function );

			goto on_error;
		}
		if( ( value_type != LIBPFF_VALUE_TYPE_STRING_ASCII )
		 && ( value_type != LIBPFF_VALUE_TYPE_STRING_UNICODE )
		 && ( value_type != LIBPFF_VALUE_TYPE_BINARY_DATA ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported value type: 0x%04" PRIx32 ".",
			 function,
			 value_type );

			goto on_error;
		}
		if( ( value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
		 || ( value_type == LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
		{
/* TODO ignore the message body codepage for now */
			ascii_codepage = internal_item->message_codepage;

			if( libpff_record_entry_get_data_as_utf8_string_size_with_codepage(
			     record_entry,
			     ascii_codepage,
			     size,
			     error ) != 1 )
			{
				message_codepage = internal_item->message_codepage;

				/* Sometimes the message codepage is not available
				 */
				if( ( message_codepage == internal_item->message_body_codepage )
				 || ( message_codepage == 0 ) )
				{
					message_codepage = internal_item->ascii_codepage;
				}
				/* Sometimes the message body codepade is not the one used to encode
				 * the message body, so try the message codepage as well
				 */
				if( ( (uint32_t) ascii_codepage == message_codepage )
				 || ( libpff_record_entry_get_data_as_utf8_string_size_with_codepage(
				       record_entry,
				       message_codepage,
				       size,
				       error ) != 1 ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to determine message body size.",
					 function );

					goto on_error;
				}
				libcerror_error_free(
				 error );
			}
		}
		else if( value_type == LIBPFF_VALUE_TYPE_BINARY_DATA )
		{
			if( libpff_record_entry_get_data_size(
			     record_entry,
			     size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve binary data size.",
				 function );

				goto on_error;
			}
			/* Add the end of string character
			 */
			*size += 1;
		}
		if( libpff_record_entry_free(
		     &record_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free record entry.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( record_entry != NULL )
	{
		libpff_record_entry_free(
		 &record_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the plain text message body
 * The body is encoded in UTF-8
 * The function uses a codepage if necessary, it tries to read PidTagInternetCodepage first,
 * after that PidTagMessageCodepage and if not available it uses the codepage set for the library
 * Size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_message_get_plain_text_body(
     libpff_item_t *message,
     uint8_t *message_body,
     size_t size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_message_get_plain_text_body";
	size_t value_data_size                = 0;
	uint32_t message_codepage             = 0;
	uint32_t value_type                   = 0;
	int ascii_codepage                    = 0;
	int result                            = 0;

	if( message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) message;

	if( message_body == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message body.",
		 function );

		return( -1 );
	}
	if( internal_item->message_codepage == 0 )
	{
		if( libpff_internal_item_get_entry_value_32bit_integer(
		     internal_item,
		     LIBPFF_ENTRY_TYPE_MESSAGE_CODEPAGE,
		     &( internal_item->message_codepage ),
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the message codepage.",
			 function );

			goto on_error;
		}
		if( internal_item->message_codepage == 0 )
		{
			internal_item->message_codepage = internal_item->ascii_codepage;
		}
	}
	if( internal_item->message_body_codepage == 0 )
	{
		if( libpff_internal_item_get_entry_value_32bit_integer(
		     internal_item,
		     LIBPFF_ENTRY_TYPE_MESSAGE_BODY_CODEPAGE,
		     &( internal_item->message_body_codepage ),
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the message body codepage.",
			 function );

			goto on_error;
		}
		if( internal_item->message_body_codepage == 0 )
		{
			internal_item->message_body_codepage = internal_item->message_codepage;
		}
	}
	result = libpff_item_values_get_record_entry_by_type(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
	          0,
		  LIBPFF_ENTRY_TYPE_MESSAGE_BODY_PLAIN_TEXT,
	          0,
	          &record_entry,
	          LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record entry.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libpff_record_entry_get_value_type(
		     record_entry,
		     &value_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value type.",
			 function );

			goto on_error;
		}
		if( ( value_type != LIBPFF_VALUE_TYPE_STRING_ASCII )
		 && ( value_type != LIBPFF_VALUE_TYPE_STRING_UNICODE )
		 && ( value_type != LIBPFF_VALUE_TYPE_BINARY_DATA ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported value type: 0x%04" PRIx32 ".",
			 function,
			 value_type );

			goto on_error;
		}
		if( ( value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
		 || ( value_type == LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
		{
/* TODO ignore the message body codepage for now */
			ascii_codepage = internal_item->message_codepage;

			if( libpff_record_entry_get_data_as_utf8_string_with_codepage(
			     record_entry,
			     ascii_codepage,
			     message_body,
			     size,
			     error ) != 1 )
			{
				message_codepage = internal_item->message_codepage;

				/* Sometimes the message codepage is not available
				 */
				if( ( message_codepage == internal_item->message_body_codepage )
				 || ( message_codepage == 0 ) )
				{
					message_codepage = internal_item->ascii_codepage;
				}
				/* Sometimes the message body codepade is not the one used to encode
				 * the message body, so try the message codepage as well
				 */
				if( ( (uint32_t) ascii_codepage == message_codepage )
				 || ( libpff_record_entry_get_data_as_utf8_string_with_codepage(
				       record_entry,
				       message_codepage,
				       message_body,
				       size,
				       error ) != 1 ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_CONVERSION,
					 LIBCERROR_CONVERSION_ERROR_GENERIC,
					 "%s: unable to set message body.",
					 function );

					goto on_error;
				}
				libcerror_error_free(
				 error );
			}
		}
		else if( value_type == LIBPFF_VALUE_TYPE_BINARY_DATA )
		{
			if( libpff_record_entry_get_data_size(
			     record_entry,
			     &value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve binary data size.",
				 function );

				goto on_error;
			}
			if( size < value_data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: message body too small.",
				 function );

				goto on_error;
			}
			if( libpff_record_entry_get_data(
			     record_entry,
			     message_body,
			     size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve binary data.",
				 function );

				goto on_error;
			}
			/* Add the end of string character
			 */
			message_body[ value_data_size ] = 0;
		}
		if( libpff_record_entry_free(
		     &record_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free record entry.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( record_entry != NULL )
	{
		libpff_record_entry_free(
		 &record_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the RTF message body size
 * Size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_message_get_rtf_body_size(
     libpff_item_t *message,
     size_t *size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	uint8_t *value_data                   = NULL;
	static char *function                 = "libpff_message_get_rtf_body_size";
	size_t value_data_size                = 0;
	int result                            = 0;

	if( message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) message;

	if( size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_record_entry_by_type(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
	          0,
		  LIBPFF_ENTRY_TYPE_MESSAGE_BODY_COMPRESSED_RTF,
	          LIBPFF_VALUE_TYPE_BINARY_DATA,
	          &record_entry,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record entry.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libpff_record_entry_get_value_data(
		     record_entry,
		     &value_data,
		     &value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value data.",
			 function );

			goto on_error;
		}
		if( libfmapi_lzfu_get_uncompressed_data_size(
		     value_data,
		     value_data_size,
		     size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
			 LIBCERROR_COMPRESSION_ERROR_DECOMPRESS_FAILED,
			 "%s: unable to determine uncompressed value data size.",
			 function );

			goto on_error;
		}
		if( libpff_record_entry_free(
		     &record_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free record entry.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( record_entry != NULL )
	{
		libpff_record_entry_free(
		 &record_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the RTF message body
 * The body is encoded in UTF-8
 * Size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_message_get_rtf_body(
     libpff_item_t *message,
     uint8_t *message_body,
     size_t size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	uint8_t *value_data                   = NULL;
	static char *function                 = "libpff_message_get_rtf_body";
	size_t value_data_size                = 0;
	int result                            = 0;

	if( message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) message;

	if( message_body == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message body.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_record_entry_by_type(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
	          0,
		  LIBPFF_ENTRY_TYPE_MESSAGE_BODY_COMPRESSED_RTF,
	          LIBPFF_VALUE_TYPE_BINARY_DATA,
	          &record_entry,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record entry.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libpff_record_entry_get_value_data(
		     record_entry,
		     &value_data,
		     &value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value data.",
			 function );

			goto on_error;
		}
		if( libfmapi_lzfu_decompress(
		     value_data,
		     value_data_size,
		     message_body,
		     &size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
			 LIBCERROR_COMPRESSION_ERROR_DECOMPRESS_FAILED,
			 "%s: unable to decompress value data.",
			 function );

			goto on_error;
		}
		if( libpff_record_entry_free(
		     &record_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free record entry.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( record_entry != NULL )
	{
		libpff_record_entry_free(
		 &record_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the HTML message body size
 * Size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_message_get_html_body_size(
     libpff_item_t *message,
     size_t *size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	uint8_t *value_data                   = NULL;
	static char *function                 = "libpff_message_get_html_body_size";
	uint32_t value_type                   = 0;
	int result                            = 0;

	if( message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) message;

	if( size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_record_entry_by_type(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
	          0,
		  LIBPFF_ENTRY_TYPE_MESSAGE_BODY_HTML,
	          0,
	          &record_entry,
	          LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record entry.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libpff_record_entry_get_value_type(
		     record_entry,
		     &value_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value type.",
			 function );

			goto on_error;
		}
		if( ( value_type != LIBPFF_VALUE_TYPE_STRING_ASCII )
		 && ( value_type != LIBPFF_VALUE_TYPE_STRING_UNICODE )
		 && ( value_type != LIBPFF_VALUE_TYPE_BINARY_DATA ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported value type: 0x%04" PRIx32 ".",
			 function,
			 value_type );

			goto on_error;
		}
		if( libpff_record_entry_get_value_data(
		     record_entry,
		     &value_data,
		     size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value data.",
			 function );

			goto on_error;
		}
		if( value_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing value data.",
			 function );

			goto on_error;
		}
		if( value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
		{
			if( value_data[ *size - 1 ] != 0 )
			{
				/* Add the end of string byte
				 */
				*size += 1;
			}
		}
		else if( value_type == LIBPFF_VALUE_TYPE_STRING_UNICODE )
		{
			if( ( value_data[ *size - 1 ] != 0 )
			 || ( value_data[ *size - 2 ] != 0 ) )
			{
				/* Add the end of string byte
				 */
				*size += 2;
			}
		}
		else if( value_type == LIBPFF_VALUE_TYPE_BINARY_DATA )
		{
			if( value_data[ *size - 1 ] != 0 )
			{
				/* Add the end of string byte
				 */
				*size += 1;
			}
		}
		if( libpff_record_entry_free(
		     &record_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free record entry.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( record_entry != NULL )
	{
		libpff_record_entry_free(
		 &record_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the HTML message body
 * Size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_message_get_html_body(
     libpff_item_t *message,
     uint8_t *message_body,
     size_t size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	uint8_t *value_data                   = NULL;
	static char *function                 = "libpff_message_get_html_body";
	size_t value_data_size                = 0;
	uint32_t value_type                   = 0;
	int result                            = 0;

	if( message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) message;

	if( message_body == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message body.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_record_entry_by_type(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
	          0,
		  LIBPFF_ENTRY_TYPE_MESSAGE_BODY_HTML,
	          0,
	          &record_entry,
	          LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record entry.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libpff_record_entry_get_value_type(
		     record_entry,
		     &value_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value type.",
			 function );

			goto on_error;
		}
		if( ( value_type != LIBPFF_VALUE_TYPE_STRING_ASCII )
		 && ( value_type != LIBPFF_VALUE_TYPE_STRING_UNICODE )
		 && ( value_type != LIBPFF_VALUE_TYPE_BINARY_DATA ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported value type: 0x%04" PRIx32 ".",
			 function,
			 value_type );

			goto on_error;
		}
		if( libpff_record_entry_get_value_data(
		     record_entry,
		     &value_data,
		     &value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value data.",
			 function );

			goto on_error;
		}
		if( value_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing value data.",
			 function );

			goto on_error;
		}
		if( size < value_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: message body too small.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     message_body,
		     value_data,
		     value_data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy value data to message body .",
			 function );

			goto on_error;
		}
		if( value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
		{
			if( value_data[ value_data_size - 1 ] != 0 )
			{
				if( ( size + 1 ) < value_data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: message body too small.",
					 function );

					goto on_error;
				}
				message_body[ value_data_size ] = 0;
			}
		}
		else if( value_type == LIBPFF_VALUE_TYPE_STRING_UNICODE )
		{
			if( ( value_data[ value_data_size - 1 ] != 0 )
			 || ( value_data[ value_data_size - 2 ] != 0 ) )
			{
				if( ( size + 2 ) < value_data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: message body too small.",
					 function );

					goto on_error;
				}
				message_body[ value_data_size     ] = 0;
				message_body[ value_data_size + 1 ] = 0;
			}
		}
		else if( value_type == LIBPFF_VALUE_TYPE_BINARY_DATA )
		{
			if( value_data[ value_data_size - 1 ] != 0 )
			{
				if( ( size + 1 ) < value_data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: message body too small.",
					 function );

					goto on_error;
				}
				message_body[ value_data_size ] = 0;
			}
		}
		if( libpff_record_entry_free(
		     &record_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free record entry.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( record_entry != NULL )
	{
		libpff_record_entry_free(
		 &record_entry,
		 NULL );
	}
	return( -1 );
}

