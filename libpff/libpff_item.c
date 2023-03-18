/*
 * Item functions
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
#include <narrow_string.h>
#include <types.h>

#include "libpff_debug.h"
#include "libpff_definitions.h"
#include "libpff_descriptor_data_stream.h"
#include "libpff_descriptors_index.h"
#include "libpff_item_descriptor.h"
#include "libpff_item.h"
#include "libpff_item_tree.h"
#include "libpff_item_values.h"
#include "libpff_libbfio.h"
#include "libpff_libcdata.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_libfmapi.h"
#include "libpff_mapi.h"
#include "libpff_offsets_index.h"
#include "libpff_record_entry.h"
#include "libpff_table.h"
#include "libpff_types.h"
#include "libpff_value_type.h"

/* Creates an item
 * Make sure the value item is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_item_initialize(
     libpff_item_t **item,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcdata_list_t *name_to_id_map_list,
     libpff_descriptors_index_t *descriptors_index,
     libpff_offsets_index_t *offsets_index,
     libpff_item_tree_t *item_tree,
     libcdata_tree_node_t *item_tree_node,
     uint8_t flags,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item     = NULL;
	libpff_item_descriptor_t *item_descriptor = NULL;
	static char *function                     = "libpff_item_initialize";

	if( item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item.",
		 function );

		return( -1 );
	}
	if( *item != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid item value already set.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( ( flags & ~( LIBPFF_ITEM_FLAG_MANAGED_ITEM_TREE_NODE ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags: 0x%02" PRIx8 ".",
		 function,
		 flags );

		return( -1 );
	}
	if( libcdata_tree_node_get_value(
	     item_tree_node,
	     (intptr_t **) &item_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve item descriptor from item tree node.",
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
	internal_item = memory_allocate_structure(
	                 libpff_internal_item_t );

	if( internal_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create item.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_item,
	     0,
	     sizeof( libpff_internal_item_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear item.",
		 function );

		memory_free(
		 internal_item );

		return( -1 );
	}
	if( libpff_item_values_initialize(
	     &( internal_item->item_values ),
	     item_descriptor->descriptor_identifier,
	     item_descriptor->data_identifier,
	     item_descriptor->local_descriptors_identifier,
	     item_descriptor->recovered,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create item values.",
		 function );

		goto on_error;
	}
	if( ( flags & LIBPFF_ITEM_FLAG_MANAGED_ITEM_TREE_NODE ) == 0 )
	{
		internal_item->item_tree_node = item_tree_node;
	}
	else
	{
		if( libcdata_tree_node_clone(
		     &( internal_item->item_tree_node ),
		     item_tree_node,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_descriptor_free,
		     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libpff_item_descriptor_clone,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy item tree node.",
			 function );

			goto on_error;
		}
	}
	internal_item->type                = LIBPFF_ITEM_TYPE_UNDEFINED;
	internal_item->io_handle           = io_handle;
	internal_item->file_io_handle      = file_io_handle;
	internal_item->name_to_id_map_list = name_to_id_map_list;
	internal_item->descriptors_index   = descriptors_index;
	internal_item->offsets_index       = offsets_index;
	internal_item->item_tree           = item_tree;
	internal_item->item_descriptor     = item_descriptor;
	internal_item->ascii_codepage      = io_handle->ascii_codepage;
	internal_item->flags               = flags;

	*item = (libpff_item_t *) internal_item;

	return( 1 );

on_error:
	if( internal_item != NULL )
	{
		if( internal_item->item_values != NULL )
		{
			libpff_item_values_free(
			 &( internal_item->item_values ),
			 NULL );
		}
		memory_free(
		 internal_item );
	}
	return( -1 );
}

/* Frees an item
 * Returns 1 if successful or -1 on error
 */
int libpff_item_free(
     libpff_item_t **item,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_item_free";
	int result                            = 1;
	int sub_item_iterator                 = 0;

	if( item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item.",
		 function );

		return( -1 );
	}
	if( *item != NULL )
	{
		internal_item = (libpff_internal_item_t *) *item;
		*item         = NULL;

		/* The io_handle, file_io_handle, name_to_id_map_list, descriptors_index, offsets_index and item_tree reference is freed elsewhere
		 */
		/* If not managed the item_tree_node reference is freed elsewhere
		 */
		if( ( internal_item->flags & LIBPFF_ITEM_FLAG_MANAGED_ITEM_TREE_NODE ) != 0 )
		{
			if( internal_item->item_tree_node != NULL )
			{
				if( libcdata_tree_node_free(
				     &( internal_item->item_tree_node ),
				     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_descriptor_free,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free item tree node.",
					 function );

					return( -1 );
				}
			}
		}
		if( internal_item->item_values != NULL )
		{
			if( libpff_item_values_free(
			     &( internal_item->item_values ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free item values.",
				 function );

				result = -1;
			}
		}
		for( sub_item_iterator = 0;
		     sub_item_iterator < LIBPFF_ITEM_NUMBER_OF_SUB_ITEMS;
		     sub_item_iterator++ )
		{
			if( internal_item->sub_item_tree_node[ sub_item_iterator ] != NULL )
			{
				if( libcdata_tree_node_free(
				     &( internal_item->sub_item_tree_node[ sub_item_iterator ] ),
				     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_descriptor_free,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free sub item tree node: %d.",
					 function,
					 sub_item_iterator );

					result = -1;
				}
			}
		}
		for( sub_item_iterator = 0;
		     sub_item_iterator < LIBPFF_ITEM_NUMBER_OF_SUB_ITEMS;
		     sub_item_iterator++ )
		{
			if( internal_item->sub_item_values[ sub_item_iterator ] != NULL )
			{
				if( libpff_item_values_free(
				     &( internal_item->sub_item_values[ sub_item_iterator ] ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free sub item values: %d.",
					 function,
					 sub_item_iterator );

					result = -1;
				}
			}
		}
		if( internal_item->embedded_object_data_stream != NULL )
		{
			if( libfdata_stream_free(
			     &( internal_item->embedded_object_data_stream ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free embedded object data stream.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 internal_item );
	}
	return( result );
}

/* Determines the item type
 * Returns 1 if successful or -1 on error
 */
int libpff_internal_item_determine_type(
     libpff_internal_item_t *internal_item,
     libcerror_error_t **error )
{
	libpff_record_entry_t *record_entry = NULL;
	char *item_type_string              = NULL;
	static char *function               = "libpff_internal_item_determine_type";
	size_t item_type_string_size        = 0;
	uint32_t value_type                 = 0;
	int result                          = 0;

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
	if( internal_item->type != LIBPFF_ITEM_TYPE_UNDEFINED )
	{
		return( 1 );
	}
	result = libpff_item_values_get_record_entry_by_type(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
	          0,
	          LIBPFF_ENTRY_TYPE_MESSAGE_CLASS,
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
		 "%s: unable to retrieve record entry: 0x%04" PRIx32 ".",
		 function,
		 LIBPFF_ENTRY_TYPE_MESSAGE_CLASS );

		goto on_error;
	}
	else if( result == 0 )
	{
		/* The item does not contain a message class and therefore is a folder
		 */
		internal_item->type = LIBPFF_ITEM_TYPE_FOLDER;
	}
	else
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
		 && ( value_type != LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported value type: 0x%04" PRIx32 ".",
			 function,
			 value_type );

			goto on_error;
		}
		if( libpff_record_entry_get_data_as_utf8_string_size_with_codepage(
		     record_entry,
		     internal_item->ascii_codepage,
		     &item_type_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 string size.",
			 function );

			goto on_error;
		}
		if( item_type_string_size > 0 )
		{
			item_type_string = narrow_string_allocate(
			                    item_type_string_size );

			if( item_type_string == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create item type string.",
				 function );

				goto on_error;
			}
			if( libpff_record_entry_get_data_as_utf8_string_with_codepage(
			     record_entry,
			     internal_item->ascii_codepage,
			     (uint8_t *) item_type_string,
			     item_type_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve UTF-8 string of record entry.",
				 function );

				goto on_error;
			}
			if( item_type_string_size == 4 )
			{
				if( narrow_string_compare_no_case(
				     item_type_string,
				     "IPM",
				     3 ) == 0 )
				{
					internal_item->type = LIBPFF_ITEM_TYPE_COMMON;
				}
			}
			else if( item_type_string_size == 8 )
			{
				if( narrow_string_compare_no_case(
				     item_type_string,
				     "IPM.FAX",
				     7 ) == 0 )
				{
					internal_item->type = LIBPFF_ITEM_TYPE_FAX;
				}
			}
			else if( item_type_string_size == 9 )
			{
				if( narrow_string_compare_no_case(
				     item_type_string,
				     "IPM.Note",
				     8 ) == 0 )
				{
					internal_item->type = LIBPFF_ITEM_TYPE_EMAIL;
				}
				else if( narrow_string_compare_no_case(
					  item_type_string,
					  "IPM.Post",
					  8 ) == 0 )
				{
					internal_item->type = LIBPFF_ITEM_TYPE_POSTING_NOTE;
				}
				else if( narrow_string_compare_no_case(
					  item_type_string,
					  "IPM.Task",
					  8 ) == 0 )
				{
					internal_item->type = LIBPFF_ITEM_TYPE_TASK;
				}
			}
			else if( item_type_string_size == 12 )
			{
				if( narrow_string_compare_no_case(
				     item_type_string,
				     "IPM.Contact",
				     11 ) == 0 )
				{
					internal_item->type = LIBPFF_ITEM_TYPE_CONTACT;
				}
			}
			else if( item_type_string_size == 13 )
			{
				if( narrow_string_compare_no_case(
				     item_type_string,
				     "IPM.Activity",
				     12 ) == 0 )
				{
					internal_item->type = LIBPFF_ITEM_TYPE_ACTIVITY;
				}
				else if( narrow_string_compare_no_case(
					  item_type_string,
					  "IPM.DistList",
					  12 ) == 0 )
				{
					internal_item->type = LIBPFF_ITEM_TYPE_DISTRIBUTION_LIST;
				}
				else if( narrow_string_compare_no_case(
					  item_type_string,
					  "IPM.Note.Fax",
					  12 ) == 0 )
				{
					internal_item->type = LIBPFF_ITEM_TYPE_FAX;
				}
				else if( narrow_string_compare_no_case(
					  item_type_string,
					  "IPM.Post.RSS",
					  12 ) == 0 )
				{
					internal_item->type = LIBPFF_ITEM_TYPE_RSS_FEED;
				}
			}
			else if( item_type_string_size == 15 )
			{
				if( narrow_string_compare_no_case(
				     item_type_string,
				     "IPM.StickyNote",
				     15 ) == 0 )
				{
					internal_item->type = LIBPFF_ITEM_TYPE_NOTE;
				}
			}
			else if( item_type_string_size == 16 )
			{
				if( narrow_string_compare_no_case(
				     item_type_string,
				     "IPM.Appointment",
				     15 ) == 0 )
				{
					internal_item->type = LIBPFF_ITEM_TYPE_APPOINTMENT;
				}
			}
			else if( item_type_string_size == 19 )
			{
				if( narrow_string_compare_no_case(
				     item_type_string,
				     "IPM.Note.Voicemail",
				     18 ) == 0 )
				{
					internal_item->type = LIBPFF_ITEM_TYPE_VOICEMAIL;
				}
			}
			else if( item_type_string_size == 20 )
			{
				if( narrow_string_compare_no_case(
				     item_type_string,
				     "IPM.Note.Mobile.SMS",
				     19 ) == 0 )
				{
					internal_item->type = LIBPFF_ITEM_TYPE_SMS;
				}
				else if( narrow_string_compare_no_case(
					  item_type_string,
					  "IPM.Note.Mobile.MMS",
					  19 ) == 0 )
				{
					internal_item->type = LIBPFF_ITEM_TYPE_MMS;
				}
			}
			else if( item_type_string_size == 21 )
			{
				if( narrow_string_compare_no_case(
				     item_type_string,
				     "IPM.Conflict.Message",
				     20 ) == 0 )
				{
					internal_item->type = LIBPFF_ITEM_TYPE_CONFLICT_MESSAGE;
				}
			}
			else if( item_type_string_size == 53 )
			{
				if( narrow_string_compare_no_case(
				     item_type_string,
				     "IPM.OLE.CLASS.{00061055-0000-0000-C000-000000000046}",
				     52 ) == 0 )
				{
					internal_item->type = LIBPFF_ITEM_TYPE_APPOINTMENT;
				}
			}
			if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
			{
				if( item_type_string_size >= 12 )
				{
					if( narrow_string_compare_no_case(
					     item_type_string,
					     "IPM.Sharing",
					     11 ) == 0 )
					{
						internal_item->type = LIBPFF_ITEM_TYPE_SHARING;
					}
				}
			}
			if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
			{
				if( item_type_string_size >= 13 )
				{
					if( narrow_string_compare_no_case(
					     item_type_string,
					     "IPM.Document",
					     12 ) == 0 )
					{
						internal_item->type = LIBPFF_ITEM_TYPE_DOCUMENT;
					}
				}
			}
			if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
			{
				if( item_type_string_size >= 15 )
				{
					if( narrow_string_compare_no_case(
					     item_type_string,
					     "IPM.Note.SMIME",
					     14 ) == 0 )
					{
						internal_item->type = LIBPFF_ITEM_TYPE_EMAIL_SMIME;
					}
				}
			}
			if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
			{
				if( item_type_string_size >= 16 )
				{
					if( narrow_string_compare_no_case(
					     item_type_string,
					     "IPM.TaskRequest",
					     15 ) == 0 )
					{
						internal_item->type = LIBPFF_ITEM_TYPE_TASK_REQUEST;
					}
					else if( narrow_string_compare_no_case(
						  item_type_string,
						  "REPORT.IPM.Note",
						  15 ) == 0 )
					{
						internal_item->type = LIBPFF_ITEM_TYPE_EMAIL;
					}
				}
			}
			if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
			{
				if( item_type_string_size >= 18 )
				{
					if( narrow_string_compare_no_case(
					     item_type_string,
					     "IPM.Configuration",
					     17 ) == 0 )
					{
						internal_item->type = LIBPFF_ITEM_TYPE_CONFIGURATION;
					}
				}
			}
			if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
			{
				if( item_type_string_size >= 21 )
				{
					if( narrow_string_compare_no_case(
					     item_type_string,
					     "IPM.Schedule.Meeting",
					     20 ) == 0 )
					{
						internal_item->type = LIBPFF_ITEM_TYPE_MEETING;
					}
				}
			}
			if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
			{
				if( item_type_string_size >= 28 )
				{
					if( narrow_string_compare_no_case(
					     item_type_string,
					     "REPORT.IPM.Schedule.Meeting",
					     27 ) == 0 )
					{
						internal_item->type = LIBPFF_ITEM_TYPE_MEETING;
					}
				}
			}
			if( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED )
			{
				/* Consider any remaining IPM.Note as an e-mail
				 * should catch IPM.Note.StorageQuotaWarning
				 */
				if( item_type_string_size >= 10 )
				{
					if( narrow_string_compare_no_case(
					     item_type_string,
					     "IPM.Note.",
					     9 ) == 0 )
					{
						internal_item->type = LIBPFF_ITEM_TYPE_EMAIL;
					}
				}
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( ( libcnotify_verbose != 0 )
			 && ( internal_item->type == LIBPFF_ITEM_TYPE_UNDEFINED ) )
			{
				libcnotify_printf(
				 "%s: unsupported item type: %s\n",
				 function,
				 item_type_string );
			}
#endif
			memory_free(
			 item_type_string );
		}
	}
	return( 1 );

on_error:
	if( item_type_string != NULL )
	{
		memory_free(
		 item_type_string );
	}
	return( -1 );
}

/* Retrieves the identifier
 * Returns 1 if successful or -1 on error
 */
int libpff_item_get_identifier(
     libpff_item_t *item,
     uint32_t *identifier,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_item_get_identifier";

	if( item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) item;

	if( libpff_item_descriptor_get_descriptor_identifier(
	     internal_item->item_descriptor,
	     identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve descriptor identifier.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of record sets
 * Returns 1 if successful or -1 on error
 */
int libpff_item_get_number_of_record_sets(
     libpff_item_t *item,
     int *number_of_record_sets,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_item_get_number_of_record_sets";

	if( item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) item;

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
	if( internal_item->item_values->table == NULL )
	{
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

			return( -1 );
		}
		if( internal_item->item_values->table == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid item - invalid item values - missing table.",
			 function );

			return( -1 );
		}
	}
	if( libpff_table_get_number_of_record_sets(
	     internal_item->item_values->table,
	     number_of_record_sets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of record sets.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific record set
 * Returns 1 if successful or -1 on error
 */
int libpff_item_get_record_set_by_index(
     libpff_item_t *item,
     int record_set_index,
     libpff_record_set_t **record_set,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_item_get_record_set_by_index";

	if( item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) item;

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
	if( *record_set != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: record set already set.",
		 function );

		return( -1 );
	}
	if( internal_item->item_values->table == NULL )
	{
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

			return( -1 );
		}
		if( internal_item->item_values->table == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid item - invalid item values - missing table.",
			 function );

			return( -1 );
		}
	}
	if( libpff_table_get_record_set_by_index(
	     internal_item->item_values->table,
	     record_set_index,
	     record_set,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record set: %d.",
		 function,
		 record_set_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of entries (of a set)
 * All sets in an item contain the same number of entries
 * Returns 1 if successful or -1 on error
 */
int libpff_item_get_number_of_entries(
     libpff_item_t *item,
     uint32_t *number_of_entries,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_item_get_number_of_entries";
	int table_number_of_entries           = 0;

	if( item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) item;

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
	if( number_of_entries == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of entries.",
		 function );

		return( -1 );
	}
	if( internal_item->item_values->table == NULL )
	{
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

			return( -1 );
		}
		if( internal_item->item_values->table == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid item - invalid item values - missing table.",
			 function );

			return( -1 );
		}
	}
	if( libpff_table_get_number_of_entries(
	     internal_item->item_values->table,
	     &table_number_of_entries,
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
	*number_of_entries = (uint32_t) table_number_of_entries;

	return( 1 );
}

/* Retrieves a 32-bit integer value
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_internal_item_get_entry_value_32bit_integer(
     libpff_internal_item_t *internal_item,
     uint32_t entry_type,
     uint32_t *value_32bit,
     libcerror_error_t **error )
{
	libpff_record_entry_t *record_entry = NULL;
	static char *function               = "libpff_internal_item_get_entry_value_32bit_integer";
	int result                          = 0;

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
	result = libpff_item_values_get_record_entry_by_type(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
	          0,
	          entry_type,
	          LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED,
	          &record_entry,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record entry: 0x%04" PRIx32 " 0x%04" PRIx32 ".",
		 function,
		 entry_type,
		 LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libpff_record_entry_get_data_as_32bit_integer(
		     record_entry,
		     value_32bit,
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

/* Retrieves a 64-bit FILETIME entry value
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_internal_item_get_entry_value_filetime(
     libpff_internal_item_t *internal_item,
     uint32_t entry_type,
     uint64_t *filetime,
     libcerror_error_t **error )
{
	libpff_record_entry_t *record_entry = NULL;
	static char *function               = "libpff_internal_item_get_entry_value_filetime";
	int result                          = 0;

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
	result = libpff_item_values_get_record_entry_by_type(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
	          0,
	          entry_type,
	          LIBPFF_VALUE_TYPE_FILETIME,
	          &record_entry,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record entry: 0x%04" PRIx32 " 0x%04" PRIx32 ".",
		 function,
		 entry_type,
		 LIBPFF_VALUE_TYPE_FILETIME );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libpff_record_entry_get_data_as_filetime(
		     record_entry,
		     filetime,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve filetime value.",
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

/* Retrieves the UTF-8 string size of a specific entry
 * The size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_internal_item_get_entry_value_utf8_string_size(
     libpff_internal_item_t *internal_item,
     uint32_t entry_type,
     int ascii_codepage,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libpff_record_entry_t *record_entry = NULL;
	static char *function               = "libpff_internal_item_get_entry_value_utf8_string_size";
	uint32_t value_type                 = 0;
	int result                          = 0;

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
	result = libpff_item_values_get_record_entry_by_type(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
	          0,
	          entry_type,
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
		 "%s: unable to retrieve record entry: 0x%04" PRIx32 ".",
		 function,
		 entry_type );

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
		 && ( value_type != LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
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
		if( libpff_record_entry_get_data_as_utf8_string_size_with_codepage(
		     record_entry,
		     ascii_codepage,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 string size.",
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

/* Retrieves the UTF-8 string value of a specific entry
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_internal_item_get_entry_value_utf8_string(
     libpff_internal_item_t *internal_item,
     uint32_t entry_type,
     int ascii_codepage,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libpff_record_entry_t *record_entry = NULL;
	static char *function               = "libpff_internal_item_get_entry_value_utf8_string";
	uint32_t value_type                 = 0;
	int result                          = 0;

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
	result = libpff_item_values_get_record_entry_by_type(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
	          0,
	          entry_type,
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
		 "%s: unable to retrieve record entry: 0x%04" PRIx32 ".",
		 function,
		 entry_type );

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
		 && ( value_type != LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
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
		if( libpff_record_entry_get_data_as_utf8_string_with_codepage(
		     record_entry,
		     ascii_codepage,
		     utf8_string,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy record entry to UTF-8 string.",
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

/* Retrieves the UTF-16 string size of a specific entry
 * The size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_internal_item_get_entry_value_utf16_string_size(
     libpff_internal_item_t *internal_item,
     uint32_t entry_type,
     int ascii_codepage,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libpff_record_entry_t *record_entry = NULL;
	static char *function               = "libpff_internal_item_get_entry_value_utf16_string_size";
	uint32_t value_type                 = 0;
	int result                          = 0;

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
	result = libpff_item_values_get_record_entry_by_type(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
	          0,
	          entry_type,
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
		 "%s: unable to retrieve record entry: 0x%04" PRIx32 ".",
		 function,
		 entry_type );

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
		 && ( value_type != LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
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
		if( libpff_record_entry_get_data_as_utf16_string_size_with_codepage(
		     record_entry,
		     ascii_codepage,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 string size.",
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

/* Retrieves the UTF-16 string value of a specific entry
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_internal_item_get_entry_value_utf16_string(
     libpff_internal_item_t *internal_item,
     uint32_t entry_type,
     int ascii_codepage,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libpff_record_entry_t *record_entry = NULL;
	static char *function               = "libpff_internal_item_get_entry_value_utf16_string";
	uint32_t value_type                 = 0;
	int result                          = 0;

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
	result = libpff_item_values_get_record_entry_by_type(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
	          0,
	          entry_type,
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
		 "%s: unable to retrieve record entry: 0x%04" PRIx32 ".",
		 function,
		 entry_type );

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
		 && ( value_type != LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
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
		if( libpff_record_entry_get_data_as_utf16_string_with_codepage(
		     record_entry,
		     ascii_codepage,
		     utf16_string,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy record entry to UTF-16 string.",
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

/* Retrieves the embedded object data
 * Returns 1 if successful or -1 on error
 */
int libpff_internal_item_get_embedded_object_data(
     libpff_internal_item_t *internal_item,
     libpff_record_entry_t *record_entry,
     libcerror_error_t **error )
{
	libfcache_cache_t *embedded_object_data_cache           = NULL;
	libfdata_list_t *embedded_object_data_list              = NULL;
	libpff_local_descriptor_value_t *local_descriptor_value = NULL;
	static char *function                                   = "libpff_internal_item_get_embedded_object_data";
	uint32_t embedded_object_data_identifier                = 0;

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
	if( internal_item->item_values->table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - invalid item values - missing table.",
		 function );

		return( -1 );
	}
	if( internal_item->embedded_object_data_stream != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid item - embedded object data stream already set.",
		 function );

		return( -1 );
	}
	if( libpff_record_entry_copy_object_identifier(
	     record_entry,
	     &embedded_object_data_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy record entry object identifier.",
		 function );

		goto on_error;
	}
	/* The descriptor identifier is located in the local descriptors tree
	 */
	if( libpff_table_get_local_descriptors_value_by_identifier(
	     internal_item->item_values->table,
	     internal_item->file_io_handle,
	     (uint64_t) embedded_object_data_identifier,
	     &local_descriptor_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve local descriptor identifier: %" PRIu32 ".",
		 function,
		 embedded_object_data_identifier );

		goto on_error;
	}
	if( local_descriptor_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid local descriptor values.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: identifier: %" PRIu64 " (%s), data: %" PRIu64 ", local descriptors: %" PRIu64 "\n",
		 function,
		 local_descriptor_value->identifier,
		 libpff_debug_get_node_identifier_type(
		  (uint8_t) ( local_descriptor_value->identifier & 0x0000001fUL ) ),
		 local_descriptor_value->data_identifier,
		 local_descriptor_value->local_descriptors_identifier );
	}
#endif
	if( local_descriptor_value->data_identifier == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid local descriptor values - missing data identifier.",
		 function );

		goto on_error;
	}
/* TODO handle multiple recovered offset index values */
	if( libpff_table_read_descriptor_data_list(
	     internal_item->item_values->table,
	     internal_item->io_handle,
	     internal_item->file_io_handle,
	     internal_item->offsets_index,
	     embedded_object_data_identifier,
	     local_descriptor_value->data_identifier,
	     internal_item->item_values->recovered,
	     0,
	     &embedded_object_data_list,
	     &embedded_object_data_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read descriptor: %" PRIu32 " data: %" PRIu64 " list.",
		 function,
		 embedded_object_data_identifier,
		 local_descriptor_value->data_identifier );

		goto on_error;
	}
	if( libpff_descriptor_data_stream_initialize(
	     &( internal_item->embedded_object_data_stream ),
	     embedded_object_data_list,
	     embedded_object_data_cache,
	     LIBPFF_DESCRIPTOR_DATA_STREAM_DATA_HANDLE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create descriptor data stream.",
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
	return( 1 );

on_error:
	if( embedded_object_data_cache != NULL )
	{
		libfcache_cache_free(
		 &embedded_object_data_cache,
		 NULL );
	}
	if( embedded_object_data_list != NULL )
	{
		libfdata_list_free(
		 &embedded_object_data_list,
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

/* Retrieves the type
 * Determines the item type if neccessary
 * Returns 1 if successful or -1 on error
 */
int libpff_item_get_type(
     libpff_item_t *item,
     uint8_t *item_type,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_item_get_type";

	if( item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) item;

	if( item_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item type.",
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
	*item_type = internal_item->type;

	return( 1 );
}

/* Retrieves the number of sub items from a item
 * Returns 1 if successful or -1 on error
 */
int libpff_item_get_number_of_sub_items(
     libpff_item_t *item,
     int *number_of_sub_items,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_item_get_number_of_sub_items";

	if( item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) item;

	if( libcdata_tree_node_get_number_of_sub_nodes(
	     internal_item->item_tree_node,
	     number_of_sub_items,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub items.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the sub item for the specific index from a item
 * Returns 1 if successful or -1 on error
 */
int libpff_item_get_sub_item(
     libpff_item_t *item,
     int sub_item_index,
     libpff_item_t **sub_item,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *sub_item_tree_node      = NULL;
	libpff_internal_item_t *internal_item         = NULL;
	libpff_item_descriptor_t *sub_item_descriptor = NULL;
	static char *function                         = "libpff_item_get_sub_item";

	if( item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) item;

	if( sub_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub item.",
		 function );

		return( -1 );
	}
	if( *sub_item != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub item already set.",
		 function );

		return( -1 );
	}
	if( libcdata_tree_node_get_sub_node_by_index(
	     internal_item->item_tree_node,
             sub_item_index,
             &sub_item_tree_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub item tree node.",
		 function );

		return( -1 );
	}
	if( libcdata_tree_node_get_value(
	     sub_item_tree_node,
	     (intptr_t **) &sub_item_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub item descriptor.",
		 function );

		return( -1 );
	}
	if( libpff_item_initialize(
	     sub_item,
	     internal_item->io_handle,
	     internal_item->file_io_handle,
	     internal_item->name_to_id_map_list,
	     internal_item->descriptors_index,
	     internal_item->offsets_index,
	     internal_item->item_tree,
	     sub_item_tree_node,
	     LIBPFF_ITEM_FLAGS_DEFAULT,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize sub item.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the sub item for the specific identifier from a item
 * Returns 1 if successful, 0 if no such item or -1 on error
 */
int libpff_item_get_sub_item_by_identifier(
     libpff_item_t *item,
     uint32_t sub_item_identifier,
     libpff_item_t **sub_item,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *sub_item_tree_node      = NULL;
	libpff_internal_item_t *internal_item         = NULL;
	libpff_item_descriptor_t *sub_item_descriptor = NULL;
	static char *function                         = "libpff_item_get_sub_item_by_identifier";
	uint8_t flags                                 = 0;
	int result                                    = 0;

	if( item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item.",
		 function );

		return( -1 );
	}
	internal_item = (libpff_internal_item_t *) item;

	if( sub_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub item.",
		 function );

		return( -1 );
	}
	if( *sub_item != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub item already set.",
		 function );

		return( -1 );
	}
	result = libpff_item_tree_get_sub_node_by_identifier(
	          internal_item->item_tree_node,
                  sub_item_identifier,
                  &sub_item_tree_node,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub item tree node: %" PRIu32 ".",
		 function,
		 sub_item_identifier );

		return( -1 );
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	if( libcdata_tree_node_get_value(
	     sub_item_tree_node,
	     (intptr_t **) &sub_item_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub item descriptor.",
		 function );

		return( -1 );
	}
	if( libpff_item_initialize(
	     sub_item,
	     internal_item->io_handle,
	     internal_item->file_io_handle,
	     internal_item->name_to_id_map_list,
	     internal_item->descriptors_index,
	     internal_item->offsets_index,
	     internal_item->item_tree,
	     sub_item_tree_node,
	     LIBPFF_ITEM_FLAGS_DEFAULT | flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize sub item.",
		 function );

		return( -1 );
	}
	if( *sub_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing sub item.",
		 function );

		return( -1 );
	}
	if( ( flags & LIBPFF_ITEM_FLAG_MANAGED_ITEM_TREE_NODE ) != 0 )
	{
		if( internal_item->type != LIBPFF_ITEM_TYPE_FOLDER )
		{
			if( sub_item_identifier == LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_ATTACHMENTS )
			{
				( (libpff_internal_item_t *) *sub_item )->type = LIBPFF_ITEM_TYPE_ATTACHMENTS;
			}
			else if( sub_item_identifier == LIBPFF_LOCAL_DESCRIPTOR_IDENTIFIER_RECIPIENTS )
			{
				( (libpff_internal_item_t *) *sub_item )->type = LIBPFF_ITEM_TYPE_RECIPIENTS;
			}
			else
			{
				( (libpff_internal_item_t *) *sub_item )->type = LIBPFF_ITEM_TYPE_ATTACHMENT;
			}
		}
	}
	return( 1 );
}

