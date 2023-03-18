/*
 * Folder functions
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
#include <narrow_string.h>
#include <types.h>

#include "libpff_debug.h"
#include "libpff_definitions.h"
#include "libpff_descriptors_index.h"
#include "libpff_folder.h"
#include "libpff_index_value.h"
#include "libpff_item.h"
#include "libpff_item_descriptor.h"
#include "libpff_item_tree.h"
#include "libpff_item_values.h"
#include "libpff_libcdata.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_libfmapi.h"
#include "libpff_local_descriptor_value.h"
#include "libpff_mapi.h"
#include "libpff_record_entry.h"

#define LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS		0
#define LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES		1
#define LIBPFF_FOLDER_SUB_ITEM_SUB_ASSOCIATED_CONTENTS	2
#define LIBPFF_FOLDER_SUB_ITEM_UNKNOWNS			3

/* Retrieves the folder (container) type
 * Returns 1 if successful or -1 on error
 */
int libpff_folder_get_type(
     libpff_item_t *folder,
     uint8_t *type,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	char *container_class_string          = NULL;
	static char *function                 = "libpff_folder_get_type";
	size_t container_class_string_size    = 0;
	uint8_t folder_type                   = LIBPFF_ITEM_TYPE_UNDEFINED;
	int result                            = 0;

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

	if( type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid type.",
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
	          LIBPFF_ENTRY_TYPE_CONTAINER_CLASS,
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
		 "%s: unable to retrieve container class record entry.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		*type = (int) LIBPFF_ITEM_TYPE_UNDEFINED;

		return( 1 );
	}
	if( libpff_record_entry_get_data_as_utf8_string_size(
	     folder,
	     &container_class_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve container class string size.",
		 function );

		goto on_error;
	}
	if( container_class_string_size > 0 )
	{
		container_class_string = narrow_string_allocate(
		                          container_class_string_size );

		if( container_class_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create container class string.",
			 function );

			goto on_error;
		}
		if( libpff_record_entry_get_data_as_utf8_string(
		     folder,
		     (uint8_t *) container_class_string,
		     container_class_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve container class string.",
			 function );

			goto on_error;
		}
		if( container_class_string_size == 9 )
		{
			if( narrow_string_compare(
			     container_class_string,
			     "IPF.Note",
			     8 ) == 0 )
			{
				folder_type = LIBPFF_ITEM_TYPE_EMAIL;
			}
			else if( narrow_string_compare(
				  container_class_string,
				  "IPF.Task",
				  8 ) == 0 )
			{
				folder_type = LIBPFF_ITEM_TYPE_TASK;
			}
		}
		else if( container_class_string_size == 12 )
		{
			if( narrow_string_compare(
			     container_class_string,
			     "IPF.Contact",
			     11 ) == 0 )
			{
				folder_type = LIBPFF_ITEM_TYPE_CONTACT;
			}
			else if( narrow_string_compare(
				  container_class_string,
				  "IPF.Journal",
				  11 ) == 0 )
			{
				folder_type = LIBPFF_ITEM_TYPE_ACTIVITY;
			}
		}
		else if( container_class_string_size == 16 )
		{
			if( narrow_string_compare(
			     container_class_string,
			     "IPF.Appointment",
			     15 ) == 0 )
			{
				folder_type = LIBPFF_ITEM_TYPE_APPOINTMENT;
			}
			else if( narrow_string_compare(
				  container_class_string,
				  "IPF.StickyNote",
				  15 ) == 0 )
			{
				folder_type = LIBPFF_ITEM_TYPE_NOTE;
			}
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( ( libcnotify_verbose != 0 )
		 && ( folder_type == LIBPFF_ITEM_TYPE_UNDEFINED ) )
		{
			libcnotify_printf(
			 "%s: unsupported folder (container) type: %s\n",
			 function,
			 container_class_string );
		}
#endif
		memory_free(
		 container_class_string );

		container_class_string = NULL;
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
	*type = (int) folder_type;

	return( 1 );

on_error:
	if( container_class_string != NULL )
	{
		memory_free(
		 container_class_string );
	}
	if( record_entry != NULL )
	{
		libpff_record_entry_free(
		 &record_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * The size includes the end of string character
 * Returns 1 if successful, 0 if value is not available or -1 on error
 */
int libpff_folder_get_utf8_name_size(
     libpff_item_t *folder,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_folder_get_utf8_name_size";
	int result                            = 0;

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

	result = libpff_internal_item_get_entry_value_utf8_string_size(
	          internal_item,
	          LIBPFF_ENTRY_TYPE_DISPLAY_NAME,
	          internal_item->ascii_codepage,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of name as UTF-8 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if value is not available or -1 on error
 */
int libpff_folder_get_utf8_name(
     libpff_item_t *folder,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_folder_get_utf8_name";
	int result                            = 0;

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

	result = libpff_internal_item_get_entry_value_utf8_string(
	          internal_item,
	          LIBPFF_ENTRY_TYPE_DISPLAY_NAME,
	          internal_item->ascii_codepage,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name as UTF-8 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the size of the UTF-16 encoded name
 * The size includes the end of string character
 * Returns 1 if successful, 0 if value is not available or -1 on error
 */
int libpff_folder_get_utf16_name_size(
     libpff_item_t *folder,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_folder_get_utf16_name_size";
	int result                            = 0;

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

	result = libpff_internal_item_get_entry_value_utf16_string_size(
	          internal_item,
	          LIBPFF_ENTRY_TYPE_DISPLAY_NAME,
	          internal_item->ascii_codepage,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of name as UTF-16 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if value is not available or -1 on error
 */
int libpff_folder_get_utf16_name(
     libpff_item_t *folder,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_folder_get_utf16_name";
	int result                            = 0;

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

	result = libpff_internal_item_get_entry_value_utf16_string(
	          internal_item,
	          LIBPFF_ENTRY_TYPE_DISPLAY_NAME,
	          internal_item->ascii_codepage,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name as UTF-16 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Determine if the item has sub folders
 * Returns 1 if successful or -1 on error
 */
int libpff_folder_determine_sub_folders(
     libpff_internal_item_t *internal_item,
     libcerror_error_t **error )
{
	libpff_index_value_t *descriptor_index_value = NULL;
	static char *function                        = "libpff_folder_determine_sub_folders";
	uint32_t sub_folders_descriptor_identifier   = 0;
	int result                                   = 0;

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
	/* Determine if the item has sub folders
	 */
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ] == NULL )
	{
		sub_folders_descriptor_identifier = internal_item->item_descriptor->descriptor_identifier + 11;

		result = libpff_descriptors_index_get_index_value_by_identifier(
			  internal_item->descriptors_index,
			  internal_item->io_handle,
			  internal_item->file_io_handle,
			  sub_folders_descriptor_identifier,
			  internal_item->item_descriptor->recovered,
			  &descriptor_index_value,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve descriptor index value: %" PRIu32 ".",
			 function,
			 sub_folders_descriptor_identifier );

			return( -1 );
		}
		else if( result != 0 )
		{
			if( descriptor_index_value == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid descriptor index value.",
				 function );

				return( -1 );
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: descriptor identifier: %" PRIu64 " (%s), data: %" PRIu64 ", local descriptors: %" PRIu64 ", parent: %" PRIu32 "\n",
				 function,
				 descriptor_index_value->identifier,
				 libpff_debug_get_node_identifier_type(
				  (uint8_t) ( descriptor_index_value->identifier & 0x0000001fUL ) ),
				 descriptor_index_value->data_identifier,
				 descriptor_index_value->local_descriptors_identifier,
				 descriptor_index_value->parent_identifier );
			}
#endif
			/* Cache the sub folders items values for successive usage
			 */
			if( libpff_item_values_initialize(
			     &( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ] ),
			     sub_folders_descriptor_identifier,
			     descriptor_index_value->data_identifier,
			     descriptor_index_value->local_descriptors_identifier,
			     internal_item->item_descriptor->recovered,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create sub folders item values.",
				 function );

				return( -1 );
			}
			if( libpff_item_values_read(
			     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ],
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
				 "%s: unable to read descriptor identifier: %" PRIu32 ".",
				 function,
				 sub_folders_descriptor_identifier );

				return( -1 );
			}
		}
	}
	return( 1 );
}

/* Determine if the item has sub messages
 * Returns 1 if successful or -1 on error
 */
int libpff_folder_determine_sub_messages(
     libpff_internal_item_t *internal_item,
     libcerror_error_t **error )
{
	libpff_index_value_t *descriptor_index_value = NULL;
	static char *function                        = "libpff_folder_determine_sub_messages";
	uint32_t sub_messages_descriptor_identifier  = 0;
	int result                                   = 0;

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
	/* Determine if the item has sub messages
	 */
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ] == NULL )
	{
		sub_messages_descriptor_identifier = internal_item->item_descriptor->descriptor_identifier + 12;

		result = libpff_descriptors_index_get_index_value_by_identifier(
			  internal_item->descriptors_index,
			  internal_item->io_handle,
			  internal_item->file_io_handle,
			  sub_messages_descriptor_identifier,
			  internal_item->item_descriptor->recovered,
			  &descriptor_index_value,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve descriptor index value: %" PRIu32 ".",
			 function,
			 sub_messages_descriptor_identifier );

			return( -1 );
		}
		else if( result != 0 )
		{
			if( descriptor_index_value == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid descriptor index value.",
				 function );

				return( -1 );
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: descriptor identifier: %" PRIu64 " (%s), data: %" PRIu64 ", local descriptors: %" PRIu64 ", parent: %" PRIu32 "\n",
				 function,
				 descriptor_index_value->identifier,
				 libpff_debug_get_node_identifier_type(
				  (uint8_t) ( descriptor_index_value->identifier & 0x0000001fUL ) ),
				 descriptor_index_value->data_identifier,
				 descriptor_index_value->local_descriptors_identifier,
				 descriptor_index_value->parent_identifier );
			}
#endif
			/* Cache the sub messages items values for successive usage
			 */
			if( libpff_item_values_initialize(
			     &( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ] ),
			     sub_messages_descriptor_identifier,
			     descriptor_index_value->data_identifier,
			     descriptor_index_value->local_descriptors_identifier,
			     internal_item->item_descriptor->recovered,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create sub messages item values.",
				 function );

				return( -1 );
			}
			if( libpff_item_values_read(
			     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ],
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
				 "%s: unable to read descriptor identifier: %" PRIu32 ".",
				 function,
				 sub_messages_descriptor_identifier );

				return( -1 );
			}
		}
	}
	return( 1 );
}

/* Determine if the item has sub associated contents
 * Returns 1 if successful or -1 on error
 */
int libpff_folder_determine_sub_associated_contents(
     libpff_internal_item_t *internal_item,
     libcerror_error_t **error )
{
	libpff_index_value_t *descriptor_index_value           = NULL;
	static char *function                                  = "libpff_folder_determine_sub_associated_contents";
	uint32_t sub_associated_contents_descriptor_identifier = 0;
	int result                                             = 0;

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
	/* Determine if the item has sub associated contents
	 */
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_ASSOCIATED_CONTENTS ] == NULL )
	{
		sub_associated_contents_descriptor_identifier = internal_item->item_descriptor->descriptor_identifier + 13;

		result = libpff_descriptors_index_get_index_value_by_identifier(
			  internal_item->descriptors_index,
			  internal_item->io_handle,
			  internal_item->file_io_handle,
			  sub_associated_contents_descriptor_identifier,
			  internal_item->item_descriptor->recovered,
			  &descriptor_index_value,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve descriptor index value: %" PRIu32 ".",
			 function,
			 sub_associated_contents_descriptor_identifier );

			return( -1 );
		}
		else if( result != 0 )
		{
			if( descriptor_index_value == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid descriptor index value.",
				 function );

				return( -1 );
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: descriptor identifier: %" PRIu64 " (%s), data: %" PRIu64 ", local descriptors: %" PRIu64 ", parent: %" PRIu32 "\n",
				 function,
				 descriptor_index_value->identifier,
				 libpff_debug_get_node_identifier_type(
				  (uint8_t) ( descriptor_index_value->identifier & 0x0000001fUL ) ),
				 descriptor_index_value->data_identifier,
				 descriptor_index_value->local_descriptors_identifier,
				 descriptor_index_value->parent_identifier );
			}
#endif
			/* Cache the sub associated contents items values for successive usage
			 */
			if( libpff_item_values_initialize(
			     &( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_ASSOCIATED_CONTENTS ] ),
			     sub_associated_contents_descriptor_identifier,
			     descriptor_index_value->data_identifier,
			     descriptor_index_value->local_descriptors_identifier,
			     internal_item->item_descriptor->recovered,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create sub associated contents item values.",
				 function );

				return( -1 );
			}
			if( libpff_item_values_read(
			     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_ASSOCIATED_CONTENTS ],
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
				 "%s: unable to read descriptor identifier: %" PRIu32 ".",
				 function,
				 sub_associated_contents_descriptor_identifier );

				return( -1 );
			}
		}
	}
	return( 1 );
}

/* Determine if the item has unknowns
 * Returns 1 if successful or -1 on error
 */
int libpff_folder_determine_unknowns(
     libpff_internal_item_t *internal_item,
     libcerror_error_t **error )
{
	libpff_item_descriptor_t *unknowns_item_descriptor      = NULL;
	libpff_local_descriptor_value_t *local_descriptor_value = NULL;
	static char *function                                   = "libpff_folder_determine_unknowns";
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
	if( internal_item->sub_item_tree_node[ LIBPFF_FOLDER_SUB_ITEM_UNKNOWNS ] != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: unknowns sub item tree node already set.",
		 function );

		return( -1 );
	}
	/* Make sure the item values have been read
	 */
/* TODO is this still necessary ? */
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
	/* Determine if the item has unknowns
	 */
	result = libpff_item_values_get_local_descriptors_value_by_identifier(
		  internal_item->item_values,
		  internal_item->file_io_handle,
		  (uint32_t) LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_UNKNOWN_1718,
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
		 (uint32_t) LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_UNKNOWN_1718 );

		goto on_error;
	}
	else if( result != 0 )
	{
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
		if( libpff_item_values_initialize(
		     &( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_UNKNOWNS ] ),
		     LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_UNKNOWN_1718,
		     local_descriptor_value->data_identifier,
		     local_descriptor_value->local_descriptors_identifier,
		     internal_item->item_descriptor->recovered,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create unknowns item values.",
			 function );

			goto on_error;
		}
		/* Cache the unknowns items values for successive usage
		 */
		if( libpff_item_values_read(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_UNKNOWNS ],
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
			 "%s: unable to read local descriptor identifier: %" PRIu32 ".",
			 function,
			 LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_UNKNOWN_1718 );

			goto on_error;
		}
		if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_UNKNOWNS ]->table == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid unknowns item values - missing table.",
			 function );

			goto on_error;
		}
		/* Create an unknowns item descriptor
		 */
		if( libpff_item_descriptor_initialize(
		     &unknowns_item_descriptor,
		     LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_UNKNOWN_1718,
		     local_descriptor_value->data_identifier,
		     local_descriptor_value->local_descriptors_identifier,
		     internal_item->item_descriptor->recovered,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create unknowns item descriptor.",
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
		if( unknowns_item_descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing unknowns item descriptor.",
			 function );

			goto on_error;
		}
		if( libcdata_tree_node_initialize(
		     &( internal_item->sub_item_tree_node[ LIBPFF_FOLDER_SUB_ITEM_UNKNOWNS ] ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create unknowns sub item tree node.",
			 function );

			goto on_error;
		}
		if( libcdata_tree_node_set_value(
		     internal_item->sub_item_tree_node[ LIBPFF_FOLDER_SUB_ITEM_UNKNOWNS ],
		     (intptr_t *) unknowns_item_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set unknowns item descriptor in unknowns sub item tree node.",
			 function );

			goto on_error;
		}
		unknowns_item_descriptor = NULL;

		/* All the unknown data is in the unknowns item
		 * there are no sub items like for the attachments item
		 */
	}
	return( 1 );

on_error:
	if( internal_item->sub_item_tree_node[ LIBPFF_FOLDER_SUB_ITEM_UNKNOWNS ] != NULL )
	{
		libcdata_tree_node_free(
		 &( internal_item->sub_item_tree_node[ LIBPFF_FOLDER_SUB_ITEM_UNKNOWNS ] ),
		 NULL,
		 NULL );
	}
	if( unknowns_item_descriptor != NULL )
	{
		libpff_item_descriptor_free(
		 &unknowns_item_descriptor,
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

/* Retrieves the number of sub folders from a folder
 * Returns 1 if successful or -1 on error
 */
int libpff_folder_get_number_of_sub_folders(
     libpff_item_t *folder,
     int *number_of_sub_folders,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_folder_get_number_of_sub_folders";

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

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
	if( internal_item->type != LIBPFF_ITEM_TYPE_FOLDER )
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
	if( number_of_sub_folders == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of sub folders.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ] == NULL )
	{
		if( libpff_folder_determine_sub_folders(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sub folders.",
			 function );

			return( -1 );
		}
	}
	*number_of_sub_folders = 0;

	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ] != NULL )
	{
		if( libpff_item_values_get_number_of_record_sets(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ],
		     internal_item->name_to_id_map_list,
		     internal_item->io_handle,
		     internal_item->file_io_handle,
		     internal_item->offsets_index,
		     number_of_sub_folders,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine the number of sub folders.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the sub folder for the specific index from a folder
 * Returns 1 if successful or -1 on error
 */
int libpff_folder_get_sub_folder(
     libpff_item_t *folder,
     int sub_folder_index,
     libpff_item_t **sub_folder,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *sub_folder_tree_node = NULL;
	libpff_internal_item_t *internal_item      = NULL;
	libpff_record_entry_t *record_entry        = NULL;
	static char *function                      = "libpff_folder_get_sub_folder";
	uint32_t sub_folder_descriptor_identifier  = 0;

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

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
	if( internal_item->type != LIBPFF_ITEM_TYPE_FOLDER )
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
	if( sub_folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub folder.",
		 function );

		return( -1 );
	}
	if( *sub_folder != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub folder already set.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ] == NULL )
	{
		if( libpff_folder_determine_sub_folders(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sub folders.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ] != NULL )
	{
		if( libpff_item_values_get_record_entry_by_type(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ],
		     internal_item->name_to_id_map_list,
		     internal_item->io_handle,
		     internal_item->file_io_handle,
		     internal_item->offsets_index,
		     sub_folder_index,
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
			 "%s: unable to retrieve record entry.",
			 function );

			return( -1 );
		}
		if( libpff_record_entry_get_data_as_32bit_integer(
		     record_entry,
		     &sub_folder_descriptor_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve 32-bit integer value.",
			 function );

			return( -1 );
		}
		if( libpff_item_tree_get_sub_node_by_identifier(
		     internal_item->item_tree_node,
		     sub_folder_descriptor_identifier,
		     &sub_folder_tree_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub folder: %" PRIu32 " tree node.",
			 function,
			 sub_folder_descriptor_identifier );

			return( -1 );
		}
		if( libpff_item_initialize(
		     sub_folder,
		     internal_item->io_handle,
		     internal_item->file_io_handle,
		     internal_item->name_to_id_map_list,
		     internal_item->descriptors_index,
		     internal_item->offsets_index,
		     internal_item->item_tree,
		     sub_folder_tree_node,
		     LIBPFF_ITEM_FLAGS_DEFAULT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create sub folder.",
			 function );

			return( -1 );
		}
		if( *sub_folder == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid sub folder.",
			 function );

			return( -1 );
		}
		( (libpff_internal_item_t *) *sub_folder )->type = LIBPFF_ITEM_TYPE_FOLDER;
	}
	return( 1 );
}

/* Retrieves the sub folder from a folder for the specific UTF-8 encoded name
 * This function uses the PidTagDisplayName MAPI property as the name
 * Returns 1 if successful, 0 if no such sub folder or -1 on error
 */
int libpff_folder_get_sub_folder_by_utf8_name(
     libpff_item_t *folder,
     uint8_t *utf8_sub_folder_name,
     size_t utf8_sub_folder_name_size,
     libpff_item_t **sub_folder,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *sub_folder_tree_node = NULL;
	libpff_internal_item_t *internal_item      = NULL;
	libpff_record_entry_t *record_entry        = NULL;
	static char *function                      = "libpff_folder_get_sub_folder_by_utf8_name";
	uint32_t sub_folder_descriptor_identifier  = 0;
	int number_of_sub_folders                  = 0;
	int result                                 = 0;
	int sub_folder_index                       = 0;

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

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
	if( internal_item->type != LIBPFF_ITEM_TYPE_FOLDER )
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
	if( utf8_sub_folder_name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 sub folder name.",
		 function );

		return( -1 );
	}
	if( utf8_sub_folder_name_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 sub folder name size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( sub_folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub folder.",
		 function );

		return( -1 );
	}
	if( *sub_folder != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub folder already set.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ] == NULL )
	{
		if( libpff_folder_determine_sub_folders(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sub folders.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ] != NULL )
	{
		if( libpff_item_values_get_number_of_record_sets(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ],
		     internal_item->name_to_id_map_list,
		     internal_item->io_handle,
		     internal_item->file_io_handle,
		     internal_item->offsets_index,
		     &number_of_sub_folders,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of sub folders.",
			 function );

			return( -1 );
		}
		for( sub_folder_index = 0;
		     sub_folder_index < number_of_sub_folders;
		     sub_folder_index++ )
		{
			if( libpff_item_values_get_record_entry_by_type(
			     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ],
			     internal_item->name_to_id_map_list,
			     internal_item->io_handle,
			     internal_item->file_io_handle,
			     internal_item->offsets_index,
			     sub_folder_index,
			     LIBPFF_ENTRY_TYPE_DISPLAY_NAME,
			     0,
			     &record_entry,
			     LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve record entry.",
				 function );

				return( -1 );
			}
			result = libpff_record_entry_compare_value_with_utf8_string_with_codepage(
			          record_entry,
			          internal_item->ascii_codepage,
			          utf8_sub_folder_name,
			          utf8_sub_folder_name_size,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to compare record entry with UTF-8 string.",
				 function );

				return( -1 );
			}
			else if( result != 0 )
			{
				break;
			}
		}
		if( sub_folder_index >= number_of_sub_folders )
		{
			return( 0 );
		}
		if( libpff_item_values_get_record_entry_by_type(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ],
		     internal_item->name_to_id_map_list,
		     internal_item->io_handle,
		     internal_item->file_io_handle,
		     internal_item->offsets_index,
		     sub_folder_index,
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
			 "%s: unable to retrieve record entry.",
			 function );

			return( -1 );
		}
		if( libpff_record_entry_get_data_as_32bit_integer(
		     record_entry,
		     &sub_folder_descriptor_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve 32-bit integer value.",
			 function );

			return( -1 );
		}
		if( libpff_item_tree_get_sub_node_by_identifier(
		     internal_item->item_tree_node,
		     sub_folder_descriptor_identifier,
		     &sub_folder_tree_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub folder: %" PRIu32 " tree node.",
			 function,
			 sub_folder_descriptor_identifier );

			return( -1 );
		}
		if( libpff_item_initialize(
		     sub_folder,
		     internal_item->io_handle,
		     internal_item->file_io_handle,
		     internal_item->name_to_id_map_list,
		     internal_item->descriptors_index,
		     internal_item->offsets_index,
		     internal_item->item_tree,
		     sub_folder_tree_node,
		     LIBPFF_ITEM_FLAGS_DEFAULT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create sub folder.",
			 function );

			return( -1 );
		}
		if( *sub_folder == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid sub folder.",
			 function );

			return( -1 );
		}
		( (libpff_internal_item_t *) *sub_folder )->type = LIBPFF_ITEM_TYPE_FOLDER;
	}
	return( 1 );
}

/* Retrieves the sub folder from a folder for the specific UTF-16 encoded name
 * This function uses the PidTagDisplayName MAPI property as the name
 * Returns 1 if successful, 0 if no such sub folder or -1 on error
 */
int libpff_folder_get_sub_folder_by_utf16_name(
     libpff_item_t *folder,
     uint16_t *utf16_sub_folder_name,
     size_t utf16_sub_folder_name_size,
     libpff_item_t **sub_folder,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *sub_folder_tree_node = NULL;
	libpff_internal_item_t *internal_item      = NULL;
	libpff_record_entry_t *record_entry        = NULL;
	static char *function                      = "libpff_folder_get_sub_folder_by_utf16_name";
	uint32_t sub_folder_descriptor_identifier  = 0;
	int number_of_sub_folders                  = 0;
	int result                                 = 0;
	int sub_folder_index                       = 0;

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

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
	if( internal_item->type != LIBPFF_ITEM_TYPE_FOLDER )
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
	if( utf16_sub_folder_name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 sub folder name.",
		 function );

		return( -1 );
	}
	if( utf16_sub_folder_name_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 sub folder name size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( sub_folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub folder.",
		 function );

		return( -1 );
	}
	if( *sub_folder != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub folder already set.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ] == NULL )
	{
		if( libpff_folder_determine_sub_folders(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sub folders.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ] != NULL )
	{
		if( libpff_item_values_get_number_of_record_sets(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ],
		     internal_item->name_to_id_map_list,
		     internal_item->io_handle,
		     internal_item->file_io_handle,
		     internal_item->offsets_index,
		     &number_of_sub_folders,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of sub folders.",
			 function );

			return( -1 );
		}
		for( sub_folder_index = 0;
		     sub_folder_index < number_of_sub_folders;
		     sub_folder_index++ )
		{
			if( libpff_item_values_get_record_entry_by_type(
			     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ],
			     internal_item->name_to_id_map_list,
			     internal_item->io_handle,
			     internal_item->file_io_handle,
			     internal_item->offsets_index,
			     sub_folder_index,
			     LIBPFF_ENTRY_TYPE_DISPLAY_NAME,
			     0,
			     &record_entry,
			     LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve record entry.",
				 function );

				return( -1 );
			}
			result = libpff_record_entry_compare_value_with_utf16_string_with_codepage(
			          record_entry,
			          internal_item->ascii_codepage,
			          utf16_sub_folder_name,
			          utf16_sub_folder_name_size,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to compare record entry with UTF-16 string.",
				 function );

				return( -1 );
			}
			else if( result != 0 )
			{
				break;
			}
		}
		if( sub_folder_index >= number_of_sub_folders )
		{
			return( 0 );
		}
		if( libpff_item_values_get_record_entry_by_type(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ],
		     internal_item->name_to_id_map_list,
		     internal_item->io_handle,
		     internal_item->file_io_handle,
		     internal_item->offsets_index,
		     sub_folder_index,
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
			 "%s: unable to retrieve record entry.",
			 function );

			return( -1 );
		}
		if( libpff_record_entry_get_data_as_32bit_integer(
		     record_entry,
		     &sub_folder_descriptor_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve 32-bit integer value.",
			 function );

			return( -1 );
		}
		if( libpff_item_tree_get_sub_node_by_identifier(
		     internal_item->item_tree_node,
		     sub_folder_descriptor_identifier,
		     &sub_folder_tree_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub folder: %" PRIu32 " tree node.",
			 function,
			 sub_folder_descriptor_identifier );

			return( -1 );
		}
		if( libpff_item_initialize(
		     sub_folder,
		     internal_item->io_handle,
		     internal_item->file_io_handle,
		     internal_item->name_to_id_map_list,
		     internal_item->descriptors_index,
		     internal_item->offsets_index,
		     internal_item->item_tree,
		     sub_folder_tree_node,
		     LIBPFF_ITEM_FLAGS_DEFAULT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create sub folder.",
			 function );

			return( -1 );
		}
		if( *sub_folder == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid sub folder.",
			 function );

			return( -1 );
		}
		( (libpff_internal_item_t *) *sub_folder )->type = LIBPFF_ITEM_TYPE_FOLDER;
	}
	return( 1 );
}

/* Retrieves the sub folders from a folder
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_folder_get_sub_folders(
     libpff_item_t *folder,
     libpff_item_t **sub_folders,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *sub_folders_item_tree_node = NULL;
	libpff_internal_item_t *internal_item            = NULL;
	static char *function                            = "libpff_folder_get_sub_folders";
	uint32_t sub_folders_descriptor_identifier       = 0;
	int result                                       = 0;

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

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
	if( internal_item->type != LIBPFF_ITEM_TYPE_FOLDER )
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
	if( sub_folders == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub folders.",
		 function );

		return( -1 );
	}
	if( *sub_folders != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub folders already set.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ] == NULL )
	{
		if( libpff_folder_determine_sub_folders(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sub folders.",
			 function );

			goto on_error;
		}
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ] == NULL )
	{
		return( 0 );
	}
	/* Determine the sub folders item descriptor identifier
	 */
	if( libpff_item_descriptor_get_descriptor_identifier(
	     internal_item->item_descriptor,
	     &sub_folders_descriptor_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve folder identifier.",
		 function );

		goto on_error;
	}
	sub_folders_descriptor_identifier += 11;

	/* Find sub folders tree node
	 */
	result = libpff_item_tree_get_node_by_identifier(
	          internal_item->item_tree,
	          sub_folders_descriptor_identifier,
	          &sub_folders_item_tree_node,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub folders tree node.",
		 function );

		goto on_error;
	}
	if( ( result == 0 )
	 || ( sub_folders_item_tree_node == NULL ) )
	{
		return( 0 );
	}
	if( libpff_item_initialize(
	     sub_folders,
	     internal_item->io_handle,
	     internal_item->file_io_handle,
	     internal_item->name_to_id_map_list,
	     internal_item->descriptors_index,
	     internal_item->offsets_index,
	     internal_item->item_tree,
	     sub_folders_item_tree_node,
	     LIBPFF_ITEM_FLAGS_DEFAULT,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sub folders.",
		 function );

		goto on_error;
	}
	if( *sub_folders == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid sub folders.",
		 function );

		goto on_error;
	}
	( (libpff_internal_item_t *) *sub_folders )->type = LIBPFF_ITEM_TYPE_SUB_FOLDERS;

	/* Clones the item values sub elements from the cached sub item values
	 */
	if( libpff_item_values_clone_copy(
	     ( (libpff_internal_item_t *) *sub_folders )->item_values,
	     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_FOLDERS ],
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy sub folders item values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *sub_folders != NULL )
	{
		libpff_item_free(
		 sub_folders,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the number of sub messages from a folder
 * Returns 1 if successful or -1 on error
 */
int libpff_folder_get_number_of_sub_messages(
     libpff_item_t *folder,
     int *number_of_sub_messages,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_folder_get_number_of_sub_messages";

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

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
	if( internal_item->type != LIBPFF_ITEM_TYPE_FOLDER )
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
	if( number_of_sub_messages == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of sub messages.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ] == NULL )
	{
		if( libpff_folder_determine_sub_messages(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sub messages.",
			 function );

			return( -1 );
		}
	}
	*number_of_sub_messages = 0;

	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ] != NULL )
	{
		if( libpff_item_values_get_number_of_record_sets(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ],
		     internal_item->name_to_id_map_list,
		     internal_item->io_handle,
		     internal_item->file_io_handle,
		     internal_item->offsets_index,
		     number_of_sub_messages,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine the number of sub messages.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the sub messages for the specific index from a folder
 * Returns 1 if successful or -1 on error
 */
int libpff_folder_get_sub_message(
     libpff_item_t *folder,
     int sub_message_index,
     libpff_item_t **sub_message,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *sub_message_tree_node = NULL;
	libpff_internal_item_t *internal_item       = NULL;
	libpff_record_entry_t *record_entry         = NULL;
	static char *function                       = "libpff_folder_get_sub_message";
	uint32_t sub_message_descriptor_identifier  = 0;

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

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
	if( internal_item->type != LIBPFF_ITEM_TYPE_FOLDER )
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
	if( sub_message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub message.",
		 function );

		return( -1 );
	}
	if( *sub_message != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub message already set.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ] == NULL )
	{
		if( libpff_folder_determine_sub_messages(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sub messages.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ] != NULL )
	{
		if( libpff_item_values_get_record_entry_by_type(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ],
		     internal_item->name_to_id_map_list,
		     internal_item->io_handle,
		     internal_item->file_io_handle,
		     internal_item->offsets_index,
		     (int) sub_message_index,
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
			 "%s: unable to retrieve record entry.",
			 function );

			return( -1 );
		}
		if( libpff_record_entry_get_data_as_32bit_integer(
		     record_entry,
		     &sub_message_descriptor_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve 32-bit integer value.",
			 function );

			return( -1 );
		}
		if( libpff_item_tree_get_sub_node_by_identifier(
		     internal_item->item_tree_node,
		     sub_message_descriptor_identifier,
		     &sub_message_tree_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub message tree node.",
			 function );

			return( -1 );
		}
		if( libpff_item_initialize(
		     sub_message,
		     internal_item->io_handle,
		     internal_item->file_io_handle,
		     internal_item->name_to_id_map_list,
		     internal_item->descriptors_index,
		     internal_item->offsets_index,
		     internal_item->item_tree,
		     sub_message_tree_node,
		     LIBPFF_ITEM_FLAGS_DEFAULT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create sub message.",
			 function );

			return( -1 );
		}
		if( *sub_message == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid sub message.",
			 function );

			return( -1 );
		}
		( (libpff_internal_item_t *) *sub_message )->type = LIBPFF_ITEM_TYPE_UNDEFINED;
	}
	return( 1 );
}

/* Retrieves the sub message from a folder for the specific UTF-8 encoded name
 * This function uses the PidTagDisplayName MAPI property as the name
 * Returns 1 if successful, 0 if no such sub message or -1 on error
 */
int libpff_folder_get_sub_message_by_utf8_name(
     libpff_item_t *folder,
     uint8_t *utf8_sub_message_name,
     size_t utf8_sub_message_name_size,
     libpff_item_t **sub_message,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *sub_message_tree_node = NULL;
	libpff_internal_item_t *internal_item       = NULL;
	libpff_record_entry_t *record_entry         = NULL;
	static char *function                       = "libpff_folder_get_sub_message_by_utf8_name";
	uint32_t sub_message_descriptor_identifier  = 0;
	int number_of_sub_messages                  = 0;
	int result                                  = 0;
	int sub_message_index                       = 0;

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

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
	if( internal_item->type != LIBPFF_ITEM_TYPE_FOLDER )
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
	if( utf8_sub_message_name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 sub message name.",
		 function );

		return( -1 );
	}
	if( utf8_sub_message_name_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 sub message name size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( sub_message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub message.",
		 function );

		return( -1 );
	}
	if( *sub_message != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub message already set.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ] == NULL )
	{
		if( libpff_folder_determine_sub_messages(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sub messages.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ] != NULL )
	{
		if( libpff_item_values_get_number_of_record_sets(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ],
		     internal_item->name_to_id_map_list,
		     internal_item->io_handle,
		     internal_item->file_io_handle,
		     internal_item->offsets_index,
		     &number_of_sub_messages,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of sub messages.",
			 function );

			return( -1 );
		}
		for( sub_message_index = 0;
		     sub_message_index < number_of_sub_messages;
		     sub_message_index++ )
		{
			if( libpff_item_values_get_record_entry_by_type(
			     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ],
			     internal_item->name_to_id_map_list,
			     internal_item->io_handle,
			     internal_item->file_io_handle,
			     internal_item->offsets_index,
			     sub_message_index,
			     LIBPFF_ENTRY_TYPE_DISPLAY_NAME,
			     0,
			     &record_entry,
			     LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve record entry.",
				 function );

				return( -1 );
			}
			result = libpff_record_entry_compare_value_with_utf8_string_with_codepage(
			          record_entry,
			          internal_item->ascii_codepage,
			          utf8_sub_message_name,
			          utf8_sub_message_name_size,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to compare record entry with UTF-8 string.",
				 function );

				return( -1 );
			}
			else if( result != 0 )
			{
				break;
			}
		}
		if( sub_message_index >= number_of_sub_messages )
		{
			return( 0 );
		}
		if( libpff_item_values_get_record_entry_by_type(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ],
		     internal_item->name_to_id_map_list,
		     internal_item->io_handle,
		     internal_item->file_io_handle,
		     internal_item->offsets_index,
		     sub_message_index,
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
			 "%s: unable to retrieve record entry.",
			 function );

			return( -1 );
		}
		if( libpff_record_entry_get_data_as_32bit_integer(
		     record_entry,
		     &sub_message_descriptor_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve 32-bit integer value.",
			 function );

			return( -1 );
		}
		if( libpff_item_tree_get_sub_node_by_identifier(
		     internal_item->item_tree_node,
		     sub_message_descriptor_identifier,
		     &sub_message_tree_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub message tree node.",
			 function );

			return( -1 );
		}
		if( libpff_item_initialize(
		     sub_message,
		     internal_item->io_handle,
		     internal_item->file_io_handle,
		     internal_item->name_to_id_map_list,
		     internal_item->descriptors_index,
		     internal_item->offsets_index,
		     internal_item->item_tree,
		     sub_message_tree_node,
		     LIBPFF_ITEM_FLAGS_DEFAULT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create sub message.",
			 function );

			return( -1 );
		}
		if( *sub_message == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid sub message.",
			 function );

			return( -1 );
		}
		( (libpff_internal_item_t *) *sub_message )->type = LIBPFF_ITEM_TYPE_UNDEFINED;
	}
	return( 1 );
}

/* Retrieves the sub message from a folder for the specific UTF-16 encoded name
 * This function uses the PidTagDisplayName MAPI property as the name
 * Returns 1 if successful, 0 if no such sub message or -1 on error
 */
int libpff_folder_get_sub_message_by_utf16_name(
     libpff_item_t *folder,
     uint16_t *utf16_sub_message_name,
     size_t utf16_sub_message_name_size,
     libpff_item_t **sub_message,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *sub_message_tree_node = NULL;
	libpff_internal_item_t *internal_item       = NULL;
	libpff_record_entry_t *record_entry         = NULL;
	static char *function                       = "libpff_folder_get_sub_message_by_utf16_name";
	uint32_t sub_message_descriptor_identifier  = 0;
	int number_of_sub_messages                  = 0;
	int result                                  = 0;
	int sub_message_index                       = 0;

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

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
	if( internal_item->type != LIBPFF_ITEM_TYPE_FOLDER )
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
	if( utf16_sub_message_name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 sub message name.",
		 function );

		return( -1 );
	}
	if( utf16_sub_message_name_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 sub message name size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( sub_message == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub message.",
		 function );

		return( -1 );
	}
	if( *sub_message != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub message already set.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ] == NULL )
	{
		if( libpff_folder_determine_sub_messages(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sub messages.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ] != NULL )
	{
		if( libpff_item_values_get_number_of_record_sets(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ],
		     internal_item->name_to_id_map_list,
		     internal_item->io_handle,
		     internal_item->file_io_handle,
		     internal_item->offsets_index,
		     &number_of_sub_messages,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of sub messages.",
			 function );

			return( -1 );
		}
		for( sub_message_index = 0;
		     sub_message_index < number_of_sub_messages;
		     sub_message_index++ )
		{
			if( libpff_item_values_get_record_entry_by_type(
			     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ],
			     internal_item->name_to_id_map_list,
			     internal_item->io_handle,
			     internal_item->file_io_handle,
			     internal_item->offsets_index,
			     sub_message_index,
			     LIBPFF_ENTRY_TYPE_DISPLAY_NAME,
			     0,
			     &record_entry,
			     LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve record entry.",
				 function );

				return( -1 );
			}
			result = libpff_record_entry_compare_value_with_utf16_string_with_codepage(
			          record_entry,
			          internal_item->ascii_codepage,
			          utf16_sub_message_name,
			          utf16_sub_message_name_size,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to compare record entry with UTF-16 string.",
				 function );

				return( -1 );
			}
			else if( result != 0 )
			{
				break;
			}
		}
		if( sub_message_index >= number_of_sub_messages )
		{
			return( 0 );
		}
		if( libpff_item_values_get_record_entry_by_type(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ],
		     internal_item->name_to_id_map_list,
		     internal_item->io_handle,
		     internal_item->file_io_handle,
		     internal_item->offsets_index,
		     sub_message_index,
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
			 "%s: unable to retrieve record entry.",
			 function );

			return( -1 );
		}
		if( libpff_record_entry_get_data_as_32bit_integer(
		     record_entry,
		     &sub_message_descriptor_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve 32-bit integer value.",
			 function );

			return( -1 );
		}
		if( libpff_item_tree_get_sub_node_by_identifier(
		     internal_item->item_tree_node,
		     sub_message_descriptor_identifier,
		     &sub_message_tree_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub message tree node.",
			 function );

			return( -1 );
		}
		if( libpff_item_initialize(
		     sub_message,
		     internal_item->io_handle,
		     internal_item->file_io_handle,
		     internal_item->name_to_id_map_list,
		     internal_item->descriptors_index,
		     internal_item->offsets_index,
		     internal_item->item_tree,
		     sub_message_tree_node,
		     LIBPFF_ITEM_FLAGS_DEFAULT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create sub message.",
			 function );

			return( -1 );
		}
		if( *sub_message == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid sub message.",
			 function );

			return( -1 );
		}
		( (libpff_internal_item_t *) *sub_message )->type = LIBPFF_ITEM_TYPE_UNDEFINED;
	}
	return( 1 );
}

/* Retrieves the sub messages from a folder
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_folder_get_sub_messages(
     libpff_item_t *folder,
     libpff_item_t **sub_messages,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *sub_messages_item_tree_node = NULL;
	libpff_internal_item_t *internal_item             = NULL;
	static char *function                             = "libpff_folder_get_sub_messages";
	uint32_t sub_messages_descriptor_identifier       = 0;
	int result                                        = 0;

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

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
	if( internal_item->type != LIBPFF_ITEM_TYPE_FOLDER )
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
	if( sub_messages == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub messages.",
		 function );

		return( -1 );
	}
	if( *sub_messages != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub messages already set.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ] == NULL )
	{
		if( libpff_folder_determine_sub_messages(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sub messages.",
			 function );

			goto on_error;
		}
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ] == NULL )
	{
		return( 0 );
	}
	/* Determine the sub messages item descriptor identifier
	 */
	if( libpff_item_descriptor_get_descriptor_identifier(
	     internal_item->item_descriptor,
	     &sub_messages_descriptor_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve folder identifier.",
		 function );

		goto on_error;
	}
	sub_messages_descriptor_identifier += 12;

	/* Find sub messages tree node
	 */
	result = libpff_item_tree_get_node_by_identifier(
	          internal_item->item_tree,
	          sub_messages_descriptor_identifier,
	          &sub_messages_item_tree_node,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub messages tree node.",
		 function );

		goto on_error;
	}
	if( ( result == 0 )
	 || ( sub_messages_item_tree_node == NULL ) )
	{
		return( 0 );
	}
	if( libpff_item_initialize(
	     sub_messages,
	     internal_item->io_handle,
	     internal_item->file_io_handle,
	     internal_item->name_to_id_map_list,
	     internal_item->descriptors_index,
	     internal_item->offsets_index,
	     internal_item->item_tree,
	     sub_messages_item_tree_node,
	     LIBPFF_ITEM_FLAGS_DEFAULT,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sub messages.",
		 function );

		goto on_error;
	}
	if( *sub_messages == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid sub messages.",
		 function );

		goto on_error;
	}
	( (libpff_internal_item_t *) *sub_messages )->type = LIBPFF_ITEM_TYPE_SUB_MESSAGES;

	/* Clones the item values sub elements from the cached sub item values
	 */
	if( libpff_item_values_clone_copy(
	     ( (libpff_internal_item_t *) *sub_messages )->item_values,
	     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_MESSAGES ],
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy sub messages item values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *sub_messages != NULL )
	{
		libpff_item_free(
		 sub_messages,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the number of sub associated contents from a folder
 * Returns 1 if successful or -1 on error
 */
int libpff_folder_get_number_of_sub_associated_contents(
     libpff_item_t *folder,
     int *number_of_sub_associated_contents,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_folder_get_number_of_sub_associated_contents";

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

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
	if( internal_item->type != LIBPFF_ITEM_TYPE_FOLDER )
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
	if( number_of_sub_associated_contents == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of sub associated contents.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_ASSOCIATED_CONTENTS ] == NULL )
	{
		if( libpff_folder_determine_sub_associated_contents(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sub associated contents.",
			 function );

			return( -1 );
		}
	}
	*number_of_sub_associated_contents = 0;

	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_ASSOCIATED_CONTENTS ] != NULL )
	{
		if( libpff_item_values_get_number_of_record_sets(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_ASSOCIATED_CONTENTS ],
		     internal_item->name_to_id_map_list,
		     internal_item->io_handle,
		     internal_item->file_io_handle,
		     internal_item->offsets_index,
		     number_of_sub_associated_contents,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine the number of sub associated contents.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the sub associated contents for the specific index from a folder
 * Returns 1 if successful or -1 on error
 */
int libpff_folder_get_sub_associated_content(
     libpff_item_t *folder,
     int sub_associated_content_index,
     libpff_item_t **sub_associated_content,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *sub_associated_content_tree_node = NULL;
	libpff_internal_item_t *internal_item                  = NULL;
	libpff_record_entry_t *record_entry                    = NULL;
	static char *function                                  = "libpff_folder_get_sub_associated_content";
	uint32_t sub_associated_content_descriptor_identifier  = 0;

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

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
	if( internal_item->type != LIBPFF_ITEM_TYPE_FOLDER )
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
	if( sub_associated_content == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub associated content.",
		 function );

		return( -1 );
	}
	if( *sub_associated_content != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub associated content already set.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_ASSOCIATED_CONTENTS ] == NULL )
	{
		if( libpff_folder_determine_sub_associated_contents(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sub associated contents.",
			 function );

			return( -1 );
		}
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_ASSOCIATED_CONTENTS ] != NULL )
	{
		if( libpff_item_values_get_record_entry_by_type(
		     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_ASSOCIATED_CONTENTS ],
		     internal_item->name_to_id_map_list,
		     internal_item->io_handle,
		     internal_item->file_io_handle,
		     internal_item->offsets_index,
		     (int) sub_associated_content_index,
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
			 "%s: unable to retrieve record entry.",
			 function );

			return( -1 );
		}
		if( libpff_record_entry_get_data_as_32bit_integer(
		     record_entry,
		     &sub_associated_content_descriptor_identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve 32-bit integer value.",
			 function );

			return( -1 );
		}
		if( libpff_item_tree_get_sub_node_by_identifier(
		     internal_item->item_tree_node,
		     sub_associated_content_descriptor_identifier,
		     &sub_associated_content_tree_node,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub associated content tree node.",
			 function );

			return( -1 );
		}
		if( libpff_item_initialize(
		     sub_associated_content,
		     internal_item->io_handle,
		     internal_item->file_io_handle,
		     internal_item->name_to_id_map_list,
		     internal_item->descriptors_index,
		     internal_item->offsets_index,
		     internal_item->item_tree,
		     sub_associated_content_tree_node,
		     LIBPFF_ITEM_FLAGS_DEFAULT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create sub associated content.",
			 function );

			return( -1 );
		}
		if( *sub_associated_content == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid sub associated content.",
			 function );

			return( -1 );
		}
		( (libpff_internal_item_t *) *sub_associated_content )->type = LIBPFF_ITEM_TYPE_UNDEFINED;
	}
	return( 1 );
}

/* Retrieves the sub associated contents from a folder
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_folder_get_sub_associated_contents(
     libpff_item_t *folder,
     libpff_item_t **sub_associated_contents,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *sub_associated_contents_item_tree_node = NULL;
	libpff_internal_item_t *internal_item                        = NULL;
	static char *function                                        = "libpff_folder_get_sub_associated_contents";
	uint32_t sub_associated_contents_descriptor_identifier       = 0;
	int result                                                   = 0;

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

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
	if( internal_item->type != LIBPFF_ITEM_TYPE_FOLDER )
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
	if( sub_associated_contents == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub associated contents.",
		 function );

		return( -1 );
	}
	if( *sub_associated_contents != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub associated contents already set.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_ASSOCIATED_CONTENTS ] == NULL )
	{
		if( libpff_folder_determine_sub_associated_contents(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine sub associated contents.",
			 function );

			goto on_error;
		}
	}
	if( internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_ASSOCIATED_CONTENTS ] == NULL )
	{
		return( 0 );
	}
	/* Determine the sub associated contents item descriptor identifier
	 */
	if( libpff_item_descriptor_get_descriptor_identifier(
	     internal_item->item_descriptor,
	     &sub_associated_contents_descriptor_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve folder identifier.",
		 function );

		goto on_error;
	}
	sub_associated_contents_descriptor_identifier += 13;

	/* Find sub associated contents tree node
	 */
	result = libpff_item_tree_get_node_by_identifier(
	          internal_item->item_tree,
	          sub_associated_contents_descriptor_identifier,
	          &sub_associated_contents_item_tree_node,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub associated contents tree node.",
		 function );

		goto on_error;
	}
	if( ( result == 0 )
	 || ( sub_associated_contents_item_tree_node == NULL ) )
	{
		return( 0 );
	}
	if( libpff_item_initialize(
	     sub_associated_contents,
	     internal_item->io_handle,
	     internal_item->file_io_handle,
	     internal_item->name_to_id_map_list,
	     internal_item->descriptors_index,
	     internal_item->offsets_index,
	     internal_item->item_tree,
	     sub_associated_contents_item_tree_node,
	     LIBPFF_ITEM_FLAGS_DEFAULT,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sub associated contents.",
		 function );

		goto on_error;
	}
	if( *sub_associated_contents == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid sub associated contents.",
		 function );

		goto on_error;
	}
	( (libpff_internal_item_t *) *sub_associated_contents )->type = LIBPFF_ITEM_TYPE_SUB_ASSOCIATED_CONTENTS;

	/* Clones the item values sub elements from the cached sub item values
	 */
	if( libpff_item_values_clone_copy(
	     ( (libpff_internal_item_t *) *sub_associated_contents )->item_values,
	     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_SUB_ASSOCIATED_CONTENTS ],
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy sub associated contents item values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *sub_associated_contents != NULL )
	{
		libpff_item_free(
		 sub_associated_contents,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the unknowns from a folder
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_folder_get_unknowns(
     libpff_item_t *folder,
     libpff_item_t **unknowns,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_folder_get_unknowns";

	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) folder;

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
	if( internal_item->type != LIBPFF_ITEM_TYPE_FOLDER )
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
	if( unknowns == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid unknowns.",
		 function );

		return( -1 );
	}
	if( *unknowns != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: unknowns already set.",
		 function );

		return( -1 );
	}
	if( internal_item->sub_item_tree_node[ LIBPFF_FOLDER_SUB_ITEM_UNKNOWNS ] == NULL )
	{
		if( libpff_folder_determine_unknowns(
		     internal_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine unknowns.",
			 function );

			goto on_error;
		}
	}
	if( internal_item->sub_item_tree_node[ LIBPFF_FOLDER_SUB_ITEM_UNKNOWNS ] == NULL )
	{
		return( 0 );
	}
	if( libpff_item_initialize(
	     unknowns,
	     internal_item->io_handle,
	     internal_item->file_io_handle,
	     internal_item->name_to_id_map_list,
	     internal_item->descriptors_index,
	     internal_item->offsets_index,
	     internal_item->item_tree,
	     internal_item->sub_item_tree_node[ LIBPFF_FOLDER_SUB_ITEM_UNKNOWNS ],
	     LIBPFF_ITEM_FLAGS_DEFAULT | LIBPFF_ITEM_FLAG_MANAGED_ITEM_TREE_NODE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create unknowns.",
		 function );

		goto on_error;
	}
	if( *unknowns == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid unknowns.",
		 function );

		goto on_error;
	}
	( (libpff_internal_item_t *) *unknowns )->type = LIBPFF_ITEM_TYPE_UNKNOWN;

	/* Clones the item values sub elements from the cached sub item values
	 */
	if( libpff_item_values_clone_copy(
	     ( (libpff_internal_item_t *) *unknowns )->item_values,
	     internal_item->sub_item_values[ LIBPFF_FOLDER_SUB_ITEM_UNKNOWNS ],
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy unknowns item values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *unknowns != NULL )
	{
		libpff_item_free(
		 unknowns,
		 NULL );
	}
	return( -1 );
}

