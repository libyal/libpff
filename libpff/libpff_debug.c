/*
 * Debug functions
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
#include <file_stream.h>
#include <memory.h>
#include <system_string.h>
#include <types.h>

#include "libpff_debug.h"
#include "libpff_definitions.h"
#include "libpff_libbfio.h"
#include "libpff_libcdata.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_libfguid.h"
#include "libpff_libfmapi.h"
#include "libpff_name_to_id_map.h"
#include "libpff_record_entry.h"
#include "libpff_record_set.h"

#include "pff_value_data.h"

#if defined( HAVE_DEBUG_OUTPUT )

/* Retrieves a string containing the name to id map table property type identifier
 */
const char *libpff_debug_get_name_to_id_map_property_type_identifier(
             uint32_t entry_type,
             uint32_t value_type )
{
        if( entry_type == 0x0001 )
        {
		switch( value_type )
		{
			case 0x0003:
				return( "PidTagNameidBucketCount" );
		}
	}
        else if( entry_type == 0x0002 )
        {
		switch( value_type )
		{
			case 0x0102:
				return( "PidTagNameidStreamGuid" );
		}
	}
	else if( entry_type == 0x0003 )
	{
		switch( value_type )
		{
			case 0x0102:
				return( "PidTagNameidStreamEntry" );
		}
	}
	else if( entry_type == 0x0004 )
	{
		switch( value_type )
		{
			case 0x0102:
				return( "PidTagNameidStreamString" );
		}
	}
	if( entry_type >= 0x1000 )
	{
		switch( value_type )
		{
			case 0x0102:
				return( "PidTagNameidBucketBase" );
		}
	}
	return( "_UNKNOWN_" );
}

/* Retrieves a string containing the name to id map table property type description
 */
const char *libpff_debug_get_name_to_id_map_property_type_description(
             uint32_t entry_type,
             uint32_t value_type )
{
        if( entry_type == 0x0001 )
        {
		switch( value_type )
		{
			case 0x0003:
				return( "Name to ID Map Number of Validation Entries" );
		}
	}
        else if( entry_type == 0x0002 )
        {
		switch( value_type )
		{
			case 0x0102:
				return( "Name to ID Map Class Identifiers" );
		}
	}
	else if( entry_type == 0x0003 )
	{
		switch( value_type )
		{
			case 0x0102:
				return( "Name to ID Map Entries" );
		}
	}
	else if( entry_type == 0x0004 )
	{
		switch( value_type )
		{
			case 0x0102:
				return( "Name to ID Map Strings" );
		}
	}
	if( entry_type >= 0x1000 )
	{
		switch( value_type )
		{
			case 0x0102:
				return( "Name to ID Map Validation Entry" );
		}
	}
	return( "UNKNOWN" );
}

/* Retrieves a string containing the encryption type
 */
const char *libpff_debug_get_encryption_type(
             uint8_t encryption_type )
{
	switch( encryption_type )
	{
		case LIBPFF_ENCRYPTION_TYPE_NONE:
			return( "None" );

		case LIBPFF_ENCRYPTION_TYPE_COMPRESSIBLE:
			return( "Compressible" );

		case LIBPFF_ENCRYPTION_TYPE_HIGH:
			return( "High" );
	}
	return( "UNKNOWN" );
}

/* Retrieves a string containing the node identifier type
 */
const char *libpff_debug_get_node_identifier_type(
             uint8_t node_identifier_type )
{
	switch( node_identifier_type )
	{
		case LIBPFF_NODE_IDENTIFIER_TYPE_TABLE_VALUE:
			return( "Table value" );

		case LIBPFF_NODE_IDENTIFIER_TYPE_INTERNAL:
			return( "Internal" );

		case LIBPFF_NODE_IDENTIFIER_TYPE_FOLDER:
			return( "Folder" );

		case LIBPFF_NODE_IDENTIFIER_TYPE_SEARCH_FOLDER:
			return( "Search folder" );

		case LIBPFF_NODE_IDENTIFIER_TYPE_MESSAGE:
			return( "Message" );

		case LIBPFF_NODE_IDENTIFIER_TYPE_ATTACHMENT:
			return( "Attachment" );

		case LIBPFF_NODE_IDENTIFIER_TYPE_SEARCH_UPDATE_QUEUE:
			return( "Search update queue" );

		case LIBPFF_NODE_IDENTIFIER_TYPE_SEARCH_CRITERIA_OBJECT:
			return( "Search criteria object" );

		case LIBPFF_NODE_IDENTIFIER_TYPE_ASSOCIATED_CONTENT:
			return( "Associated content" );

		case LIBPFF_NODE_IDENTIFIER_TYPE_CONTENTS_TABLE_INDEX:
			return( "Contents table index" );

		case LIBPFF_NODE_IDENTIFIER_TYPE_INBOX:
			return( "Inbox" );

		case LIBPFF_NODE_IDENTIFIER_TYPE_OUTBOX:
			return( "Outbox" );

		case LIBPFF_NODE_IDENTIFIER_TYPE_SUB_FOLDERS:
			return( "Sub folders" );

		case LIBPFF_NODE_IDENTIFIER_TYPE_SUB_MESSAGES:
			return( "Sub messages" );

		case LIBPFF_NODE_IDENTIFIER_TYPE_SUB_ASSOCIATED_CONTENTS:
			return( "Sub associated contents" );

		case LIBPFF_NODE_IDENTIFIER_TYPE_SEARCH_CONTENTS_TABLE:
			return( "Search contents table" );

		case LIBPFF_NODE_IDENTIFIER_TYPE_ATTACHMENTS:
			return( "Attachments" );

		case LIBPFF_NODE_IDENTIFIER_TYPE_RECIPIENTS:
			return( "Reciepients" );

		case LIBPFF_NODE_IDENTIFIER_TYPE_SEARCH_TABLE:
			return( "Search table" );

		case LIBPFF_NODE_IDENTIFIER_TYPE_LOCAL_DESCRIPTOR_VALUE:
			return( "Local descriptor value" );
	}
	return( "UNKNOWN" );
}

/* Prints the record set
 * Returns 1 if successful or -1 on error
 */
int libpff_debug_print_record_set(
     libpff_record_set_t *record_set,
     libcdata_list_t *name_to_id_map_list,
     int debug_item_type,
     int ascii_codepage,
     libcerror_error_t **error )
{
	libpff_internal_record_set_t *internal_record_set = NULL;
	libpff_record_entry_t *record_entry               = NULL;
	static char *function                             = "libpff_debug_print_record_set";
	int entry_index                                   = 0;
	int number_of_entries                             = 0;

	if( record_set == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record set.",
		 function );

		return( -1 );
	}
	internal_record_set = (libpff_internal_record_set_t *) record_set;

	if( libcdata_array_get_number_of_entries(
	     internal_record_set->entries_array,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries.",
		 function );

		return( -1 );
	}
	libcnotify_printf(
	 "Record set number of entries\t: %d\n",
	 number_of_entries );

	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     internal_record_set->entries_array,
		     entry_index,
		     (intptr_t **) &record_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record entry: %d.",
			 function,
			 entry_index );

			return( -1 );
		}
		libcnotify_printf(
		 "Record set entry\t\t: %d (0x%08" PRIjx ")\n",
		 entry_index,
		 (intptr_t) record_entry );

		if( libpff_debug_print_record_entry(
		     record_entry,
		     name_to_id_map_list,
		     debug_item_type,
		     ascii_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print record entry: %d .",
			 function,
			 entry_index );

			return( -1 );
		}
		libcnotify_printf(
		 "\n" );
	}
	return( 1 );
}

/* Prints the record entry
 * Returns 1 if successful or -1 on error
 */
int libpff_debug_print_record_entry(
     libpff_record_entry_t *record_entry,
     libcdata_list_t *name_to_id_map_list,
     int debug_item_type,
     int ascii_codepage,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_debug_print_record_entry";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

	if( libpff_debug_property_type_value_print(
	     name_to_id_map_list,
	     internal_record_entry->identifier.entry_type,
	     internal_record_entry->identifier.value_type,
	     internal_record_entry->value_data,
	     internal_record_entry->value_data_size,
	     debug_item_type,
	     ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print property type value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Prints the property type value
 * Returns 1 if successful or -1 on error
 */
int libpff_debug_property_type_value_print(
     libcdata_list_t *name_to_id_map_list,
     uint32_t entry_type,
     uint32_t value_type,
     uint8_t *value_data,
     size_t value_data_size,
     int debug_item_type,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libpff_debug_property_type_value_print";

	if( debug_item_type == LIBPFF_DEBUG_ITEM_TYPE_DEFAULT )
	{
		if( libpff_debug_mapi_value_print(
		     name_to_id_map_list,
		     entry_type,
		     value_type,
		     value_data,
		     value_data_size,
		     ascii_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print mapi value.",
			 function );

			return( -1 );
		}
		return( 1 );
	}
	else if( debug_item_type == LIBPFF_DEBUG_ITEM_TYPE_NAME_TO_ID_MAP )
	{
		if( libpff_debug_name_to_id_map_value_print(
		     entry_type,
		     value_type,
		     value_data,
		     value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print name to id map value.",
			 function );

			return( -1 );
		}
		return( 1 );
	}
	else if( value_data != NULL )
	{
		libcnotify_print_data(
		 value_data,
		 value_data_size,
		 0 );

		return( 1 );
	}
	return( 1 );
}

/* Prints the MAPI value
 * Returns 1 if successful or -1 on error
 */
int libpff_debug_mapi_value_print(
     libcdata_list_t *name_to_id_map_list,
     uint32_t entry_type,
     uint32_t value_type,
     uint8_t *value_data,
     size_t value_data_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	libcdata_list_element_t *list_element                        = NULL;
	libpff_internal_name_to_id_map_entry_t *name_to_id_map_entry = NULL;
	static char *function                                        = "libpff_debug_mapi_value_print";
	int element_index                                            = 0;
	int number_of_elements                                       = 0;

	if( name_to_id_map_list != NULL )
	{
		if( libcdata_list_get_number_of_elements(
		     name_to_id_map_list,
		     &number_of_elements,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of name to id map entries.",
			 function );

			return( -1 );
		}
		if( libcdata_list_get_first_element(
		     name_to_id_map_list,
		     &list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve first name to id map entry.",
			 function );

			return( -1 );
		}
		for( element_index = 0;
		     element_index < number_of_elements;
		     element_index++ )
		{
			if( list_element == NULL )
			{
				break;
			}
			if( libcdata_list_element_get_value(
			     list_element,
			     (intptr_t **) &name_to_id_map_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve name to id map entry: %d.",
				 function,
				 element_index );

				return( -1 );
			}
			if( ( name_to_id_map_entry != NULL )
			 && ( name_to_id_map_entry->identifier == entry_type )
			 && ( name_to_id_map_entry->type == LIBPFF_NAME_TO_ID_MAP_ENTRY_TYPE_NUMERIC ) )
			{
				entry_type = name_to_id_map_entry->numeric_value;

				break;
			}
			if( libcdata_list_element_get_next_element(
			     list_element,
			     &list_element,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve name to id map entry: %d next element.",
				 function,
				 element_index );

				return( -1 );
			}
		}
	}
	if( libfmapi_debug_print_value(
	     entry_type,
	     value_type,
	     value_data,
	     value_data_size,
	     ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print MAPI value.",
		 function );

		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );

		if( value_data != NULL )
		{
			libcnotify_print_data(
			 value_data,
			 value_data_size,
			 0 );
		}
		else
		{
			libcnotify_printf(
			 "<NULL>\n\n" );
		}
	}
	return( 1 );
}

/* Prints the name to id map value
 * Returns 1 if successful or -1 on error
 */
int libpff_debug_name_to_id_map_value_print(
     uint32_t entry_type,
     uint32_t value_type,
     uint8_t *value_data,
     size_t value_data_size,
     libcerror_error_t **error )
{
	system_character_t guid_string[ 48 ];

	libfguid_identifier_t *guid = NULL;
	static char *function       = "libpff_debug_name_to_id_map_value_print";
	uint32_t number_of_values   = 0;
	uint32_t value_32bit        = 0;
	uint32_t value_iterator     = 0;
	uint16_t value_16bit        = 0;
	int result                  = 0;

	if( value_type != 0x0102 )
	{
		if( value_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid value data.",
			 function );

			goto on_error;
		}
		libcnotify_print_data(
		 value_data,
		 value_data_size,
		 0 );
	}
	else if( entry_type == 0x0002 )
	{
		if( value_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid value data.",
			 function );

			goto on_error;
		}
		if( ( value_data_size % 16 ) == 0 )
		{
			number_of_values = (uint32_t) ( value_data_size / 16 );

			if( number_of_values > 0 )
			{
				if( libfguid_identifier_initialize(
				     &guid,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create GUID.",
					 function );

					goto on_error;
				}
				for( value_iterator = 0;
				     value_iterator < number_of_values;
				     value_iterator++ )
				{
					if( libfguid_identifier_copy_from_byte_stream(
					     guid,
					     value_data,
					     16,
					     LIBFGUID_ENDIAN_LITTLE,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
						 "%s: unable to copy byte stream to GUID.",
						 function );

						goto on_error;
					}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
					result = libfguid_identifier_copy_to_utf16_string(
						  guid,
						  (uint16_t *) guid_string,
						  48,
						  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
						  error );
#else
					result = libfguid_identifier_copy_to_utf8_string(
						  guid,
						  (uint8_t *) guid_string,
						  48,
						  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
						  error );
#endif
					if( result != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
						 "%s: unable to copy GUID to string.",
						 function );

						goto on_error;
					}
					libcnotify_printf(
					 "Class identifier: %02" PRIu32 "\t: %" PRIs_SYSTEM " (%s)\n",
					 value_iterator,
					 guid_string,
					 libfmapi_class_identifier_get_name(
					  value_data ) );

					value_data += 16;
				}
				if( libfguid_identifier_free(
				     &guid,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free GUID.",
					 function );

					goto on_error;
				}
			}
			libcnotify_printf(
			 "\n" );
		}
		else
		{
			libcnotify_print_data(
			 value_data,
			 value_data_size,
			 0 );
		}
	}
	else if( entry_type == 0x003 )
	{
		if( value_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid value data.",
			 function );

			goto on_error;
		}
		if( ( value_data_size % 8 ) == 0 )
		{
			number_of_values = (uint32_t) ( value_data_size / 8 );

			for( value_iterator = 0;
			     value_iterator < number_of_values;
			     value_iterator++ )
			{
				byte_stream_copy_to_uint32_little_endian(
				 value_data,
				 value_32bit );

				value_data += 4;

				byte_stream_copy_to_uint16_little_endian(
				 value_data,
				 value_16bit );

				value_data += 2;

				if( ( value_16bit & 0x00000001 ) == 0x00000001 )
				{
					libcnotify_printf(
					 "Entry: %04" PRIu32 " reference\t: 0x%08" PRIx32 "\n",
					 value_iterator,
					 value_32bit );
				}
				else
				{
					libcnotify_printf(
					 "Entry: %04" PRIu32 " value\t: 0x%08" PRIx32 "\n",
					 value_iterator,
					 value_32bit );
				}
				libcnotify_printf(
				 "Entry: %04" PRIu32 " type\t: %" PRIu16 "\n",
				 value_iterator,
				 value_16bit );

				byte_stream_copy_to_uint16_little_endian(
				 value_data,
				 value_16bit );

				libcnotify_printf(
				 "Entry: %04" PRIu32 " number\t: 0x%04" PRIx16 "\n",
				 value_iterator,
				 value_16bit );

				value_data += 2;
			}
			libcnotify_printf(
			 "\n" );
		}
		else
		{
			libcnotify_print_data(
			 value_data,
			 value_data_size,
			 0 );
		}
	}
	else if( entry_type >= 0x1000 )
	{
		if( value_data == NULL )
		{
			libcnotify_printf(
			 "\n" );
		}
		else if( ( value_data_size % 8 ) == 0 )
		{
			number_of_values = (uint32_t) ( value_data_size / 8 );

			for( value_iterator = 0;
			     value_iterator < number_of_values;
			     value_iterator++ )
			{
				byte_stream_copy_to_uint32_little_endian(
				 value_data,
				 value_32bit );

				value_data += 4;

				byte_stream_copy_to_uint16_little_endian(
				 value_data,
				 value_16bit );

				value_data += 2;

				if( ( value_16bit & 0x00000001 ) == 0x00000001 )
				{
					libcnotify_printf(
					 "Validation entry: %02" PRIu32 " checksum\t: 0x%08" PRIx32 "\n",
					 value_iterator,
					 value_32bit );
				}
				else
				{
					libcnotify_printf(
					 "Validation entry: %02" PRIu32 " value\t: 0x%08" PRIx32 "\n",
					 value_iterator,
					 value_32bit );
				}
				libcnotify_printf(
				 "Validation entry: %02" PRIu32 " type\t: %" PRIu16 "\n",
				 value_iterator,
				 value_16bit );

				byte_stream_copy_to_uint16_little_endian(
				 value_data,
				 value_16bit );

				libcnotify_printf(
				 "Validation entry: %02" PRIu32 " number\t: 0x%04" PRIx16 "\n",
				 value_iterator,
				 value_16bit );

				value_data += 2;
			}
			libcnotify_printf(
			 "\n" );
		}
		else
		{
			libcnotify_print_data(
			 value_data,
			 value_data_size,
			 0 );
		}
	}
	else
	{
		libcnotify_print_data(
		 value_data,
		 value_data_size,
		 0 );
	}
	return( 1 );

on_error:
	if( guid != NULL )
	{
		libfguid_identifier_free(
		 &guid,
		 NULL );
	}
	return( -1 );
}

/* Prints the read offsets
 * Returns 1 if successful or -1 on error
 */
int libpff_debug_print_read_offsets(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libpff_debug_print_read_offsets";
	off64_t offset        = 0;
	size64_t size         = 0;
	int number_of_offsets = 0;
	int offset_iterator   = 0;

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
	if( libbfio_handle_get_number_of_offsets_read(
	     file_io_handle,
	     &number_of_offsets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of offsets read.",
		 function );

		return( -1 );
	}
	libcnotify_printf(
	 "Offsets read:\n" );

	for( offset_iterator = 0;
	     offset_iterator < number_of_offsets;
	     offset_iterator++ )
	{
		if( libbfio_handle_get_offset_read(
		     file_io_handle,
		     offset_iterator,
		     &offset,
		     &size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset: %d.",
			 function,
			 offset_iterator );

			return( -1 );
		}
		libcnotify_printf(
		 "%08" PRIu64 " ( 0x%08" PRIx64 " ) - %08" PRIu64 " ( 0x%08" PRIx64 " ) size: %" PRIu64 "\n",
		 offset,
		 offset,
		 offset + size,
		 offset + size,
		 size );
	}
	libcnotify_printf(
	 "\n" );

	return( 1 );
}

#endif /* defined( HAVE_DEBUG_OUTPUT ) */

