/*
 * Name to ID map functions
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
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "libpff_codepage.h"
#include "libpff_debug.h"
#include "libpff_definitions.h"
#include "libpff_descriptors_index.h"
#include "libpff_mapi.h"
#include "libpff_name_to_id_map.h"
#include "libpff_io_handle.h"
#include "libpff_index_value.h"
#include "libpff_item_values.h"
#include "libpff_libbfio.h"
#include "libpff_libcdata.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_libfmapi.h"
#include "libpff_libuna.h"
#include "libpff_offsets_index.h"
#include "libpff_record_entry.h"
#include "libpff_types.h"
#include "libpff_unused.h"
#include "libpff_value_type.h"

#include "pff_value_data.h"

/* Creates a name to id map entry
 * Make sure the value name_to_id_map_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_name_to_id_map_entry_initialize(
     libpff_name_to_id_map_entry_t **name_to_id_map_entry,
     libcerror_error_t **error )
{
	libpff_internal_name_to_id_map_entry_t *internal_name_to_id_map_entry = NULL;
	static char *function                                                 = "libpff_name_to_id_map_entry_initialize";

	if( name_to_id_map_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name to id map entry.",
		 function );

		return( -1 );
	}
	if( *name_to_id_map_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid name to id map entry value already set.",
		 function );

		return( -1 );
	}
	internal_name_to_id_map_entry = memory_allocate_structure(
	                                 libpff_internal_name_to_id_map_entry_t );

	if( internal_name_to_id_map_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create name to id map entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_name_to_id_map_entry,
	     0,
	     sizeof( libpff_internal_name_to_id_map_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear name to id map entry.",
		 function );

		goto on_error;
	}
	*name_to_id_map_entry = (libpff_name_to_id_map_entry_t *) internal_name_to_id_map_entry;

	return( 1 );

on_error:
	if( internal_name_to_id_map_entry != NULL )
	{
		memory_free(
		 internal_name_to_id_map_entry );
	}
	return( -1 );
}

/* Frees a name to id map entry
 * Returns 1 if successful or -1 on error
 */
int libpff_name_to_id_map_entry_free(
     libpff_name_to_id_map_entry_t **name_to_id_map_entry,
     libcerror_error_t **error )
{
	libpff_internal_name_to_id_map_entry_t *internal_name_to_id_map_entry = NULL;
	static char *function                                                 = "libpff_name_to_id_map_entry_free";

	if( name_to_id_map_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name to id map entry.",
		 function );

		return( -1 );
	}
	if( *name_to_id_map_entry != NULL )
	{
		internal_name_to_id_map_entry = (libpff_internal_name_to_id_map_entry_t *) *name_to_id_map_entry;
		*name_to_id_map_entry         = NULL;

		if( ( internal_name_to_id_map_entry->type == LIBPFF_NAME_TO_ID_MAP_ENTRY_TYPE_STRING )
		 && ( internal_name_to_id_map_entry->string_value != NULL ) )
		{
			memory_free(
			 internal_name_to_id_map_entry->string_value );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( internal_name_to_id_map_entry->debug_string != NULL )
		{
			memory_free(
			 internal_name_to_id_map_entry->debug_string );
		}
#endif
		memory_free(
		 internal_name_to_id_map_entry );
	}
	return( 1 );
}

/* Read the name to id map
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_name_to_id_map_read(
     libcdata_list_t *name_to_id_map_list,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_descriptors_index_t *descriptors_index,
     libpff_offsets_index_t *offsets_index,
     libcerror_error_t **error )
{
	libpff_index_value_t *descriptor_index_value                         = NULL;
	libpff_item_values_t *item_values                                    = NULL;
	libpff_name_to_id_map_entry_t *name_to_id_map_entry                  = NULL;
	libpff_record_entry_t *name_to_id_map_class_identifiers_record_entry = NULL;
	libpff_record_entry_t *name_to_id_map_entries_record_entry           = NULL;
	libpff_record_entry_t *name_to_id_map_strings_record_entry           = NULL;
	uint8_t *name_to_id_map_class_identifiers_data                       = NULL;
	uint8_t *name_to_id_map_entries_data                                 = NULL;
	uint8_t *name_to_id_map_entry_data                                   = NULL;
	uint8_t *name_to_id_map_strings_record_data                          = NULL;
	static char *function                                                = "libpff_name_to_id_map_read";
	size_t name_to_id_map_class_identifiers_data_size                    = 0;
	size_t name_to_id_map_entries_data_size                              = 0;
	size_t name_to_id_map_strings_record_data_size                       = 0;
	uint32_t number_of_name_to_id_map_entries                            = 0;
	uint32_t name_to_id_map_entry_index                                  = 0;
	int result                                                           = 0;

	if( name_to_id_map_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name to id map list.",
		 function );

		return( -1 );
	}
	result = libpff_descriptors_index_get_index_value_by_identifier(
		  descriptors_index,
		  io_handle,
		  file_io_handle,
		  (uint32_t) LIBPFF_DESCRIPTOR_IDENTIFIER_NAME_TO_ID_MAP,
		  0,
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
		 (uint32_t) LIBPFF_DESCRIPTOR_IDENTIFIER_NAME_TO_ID_MAP );

#if defined( HAVE_DEBUG_OUTPUT )
		if( ( libcnotify_verbose != 0 )
		 && ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
#endif
		libcerror_error_free(
		 error );

		return( 0 );
	}
	else if( result == 0 )
	{
		return( 0 );
	}
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
	if( libpff_item_values_initialize(
	     &item_values,
	     LIBPFF_DESCRIPTOR_IDENTIFIER_NAME_TO_ID_MAP,
	     descriptor_index_value->data_identifier,
	     descriptor_index_value->local_descriptors_identifier,
	     0,
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
	if( libpff_index_value_free(
	     &descriptor_index_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free descriptor index value.",
		 function );

		goto on_error;
	}
	if( libpff_item_values_read(
	     item_values,
	     NULL,
	     io_handle,
	     file_io_handle,
	     offsets_index,
	     LIBPFF_DEBUG_ITEM_TYPE_NAME_TO_ID_MAP,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read name to id map item values.",
		 function );

		goto on_error;
	}
	if( item_values->table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item values - missing table.",
		 function );

		goto on_error;
	}
	if( libpff_table_get_record_entry_by_type(
	     item_values->table,
	     0,
	     LIBPFF_ENTRY_TYPE_NAME_TO_ID_MAP_ENTRIES,
	     LIBPFF_VALUE_TYPE_BINARY_DATA,
	     &name_to_id_map_entries_record_entry,
	     LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name to id map entries record entry.",
		 function );

		goto on_error;
	}
	if( libpff_record_entry_get_value_data(
	     name_to_id_map_entries_record_entry,
	     &name_to_id_map_entries_data,
	     &name_to_id_map_entries_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name to id map entries data.",
		 function );

		goto on_error;
	}
	if( name_to_id_map_entries_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing name to id map entries data.",
		 function );

		goto on_error;
	}
	if( ( name_to_id_map_entries_data_size == 0 )
	 || ( name_to_id_map_entries_data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid name to id map entries data size value out of bounds.",
		 function );

		goto on_error;
	}
	if( ( name_to_id_map_entries_data_size % 8 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported name to id map entries size.",
		 function );

		goto on_error;
	}
	number_of_name_to_id_map_entries = (uint32_t) ( name_to_id_map_entries_data_size / 8 );

	if( libpff_table_get_record_entry_by_type(
	     item_values->table,
	     0,
	     LIBPFF_ENTRY_TYPE_NAME_TO_ID_MAP_STRINGS,
	     LIBPFF_VALUE_TYPE_BINARY_DATA,
	     &name_to_id_map_strings_record_entry,
	     LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name to id map strings record entry.",
		 function );

		goto on_error;
	}
	if( libpff_table_get_record_entry_by_type(
	     item_values->table,
	     0,
	     LIBPFF_ENTRY_TYPE_NAME_TO_ID_MAP_CLASS_IDENTIFIERS,
	     LIBPFF_VALUE_TYPE_BINARY_DATA,
	     &name_to_id_map_class_identifiers_record_entry,
	     LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name to id map class identifiers record entry.",
		 function );

		goto on_error;
	}
	if( libpff_record_entry_get_value_data(
	     name_to_id_map_class_identifiers_record_entry,
	     &name_to_id_map_class_identifiers_data,
	     &name_to_id_map_class_identifiers_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name to id map class identifiers data.",
		 function );

		goto on_error;
	}
	if( name_to_id_map_class_identifiers_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing name to id map class identifiers data.",
		 function );

		goto on_error;
	}
	if( ( name_to_id_map_class_identifiers_data_size == 0 )
	 || ( name_to_id_map_class_identifiers_data_size > (size64_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid name to id map class identifiers data size value out of bounds.",
		 function );

		goto on_error;
	}
	name_to_id_map_entry_data = name_to_id_map_entries_data;

	for( name_to_id_map_entry_index = 0;
	     name_to_id_map_entry_index < number_of_name_to_id_map_entries;
	     name_to_id_map_entry_index++ )
	{
		if( libpff_record_entry_get_value_data(
		     name_to_id_map_strings_record_entry,
		     &name_to_id_map_strings_record_data,
		     &name_to_id_map_strings_record_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve name to id map strings record data.",
			 function );

			goto on_error;
		}
		if( libpff_name_to_id_map_entry_initialize(
		     &name_to_id_map_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create name to id map entry.",
			 function );

			goto on_error;
		}
		if( libpff_name_to_id_map_entry_read(
		     name_to_id_map_entry,
		     name_to_id_map_entry_data,
		     sizeof( pff_name_to_id_map_entry_t ),
		     name_to_id_map_class_identifiers_data,
		     name_to_id_map_class_identifiers_data_size,
		     name_to_id_map_strings_record_data,
		     name_to_id_map_strings_record_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read name to id map entry.",
			 function );

			goto on_error;
		}
		name_to_id_map_entry_data += sizeof( pff_name_to_id_map_entry_t );

		if( ( ( (libpff_internal_name_to_id_map_entry_t *) name_to_id_map_entry )->flags & LIBPFF_NAME_TO_ID_MAP_ENTRY_FLAG_IS_CORRUPTED ) != 0 )
		{
			io_handle->flags |= LIBPFF_IO_HANDLE_FLAG_IS_CORRUPTED;
		}
		if( libcdata_list_append_value(
		     name_to_id_map_list,
		     (intptr_t *) name_to_id_map_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append name to id map entry to list.",
			 function );

			goto on_error;
		}
		name_to_id_map_entry = NULL;
	}
	if( libpff_item_values_free(
	     &item_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free item values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( name_to_id_map_entry != NULL )
	{
		libpff_name_to_id_map_entry_free(
		 &name_to_id_map_entry,
		 NULL );
	}
	if( item_values != NULL )
	{
		libpff_item_values_free(
		 &item_values,
		 NULL );
	}
	if( descriptor_index_value != NULL )
	{
		libpff_index_value_free(
		 &descriptor_index_value,
		 NULL );
	}
	libcdata_list_empty(
	 name_to_id_map_list,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_name_to_id_map_entry_free,
	 NULL );

	return( -1 );
}

/* Read the name to id map entry
 * Returns 1 if successful or -1 on error
 */
int libpff_name_to_id_map_entry_read(
     libpff_name_to_id_map_entry_t *name_to_id_map_entry,
     uint8_t *name_to_id_map_entry_data,
     size_t name_to_id_map_entry_data_size,
     uint8_t *name_to_id_map_class_identifiers_data,
     size_t name_to_id_map_class_identifiers_data_size,
     uint8_t *name_to_id_map_strings_data,
     size_t name_to_id_map_strings_data_size,
     libcerror_error_t **error )
{
	libpff_internal_name_to_id_map_entry_t *internal_name_to_id_map_entry = NULL;
	uint8_t *name_to_id_map_string_data                                   = NULL;
	static char *function                                                 = "libpff_name_to_id_map_entry_read";
	size_t name_to_id_map_class_identifier_data_offset                    = 0;
	uint32_t name_to_id_map_entry_value                                   = 0;
	uint32_t name_to_id_map_string_size                                   = 0;
	uint16_t name_to_id_map_class_identifier_index                        = 0;
	uint16_t name_to_id_map_entry_type                                    = 0;
	uint16_t name_to_id_map_entry_number                                  = 0;
	int result                                                            = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t name_to_id_map_entry_index                                   = 0;
#endif

	if( name_to_id_map_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name to id map entry.",
		 function );

		return( -1 );
	}
	internal_name_to_id_map_entry = (libpff_internal_name_to_id_map_entry_t *) name_to_id_map_entry;

	if( name_to_id_map_entry_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name to id map entry data.",
		 function );

		return( -1 );
	}
	if( ( name_to_id_map_entry_data_size < sizeof( pff_name_to_id_map_entry_t ) )
	 || ( name_to_id_map_entry_data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid name to id map entry data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( name_to_id_map_class_identifiers_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name to id map class identifier data.",
		 function );

		return( -1 );
	}
	if( ( name_to_id_map_class_identifiers_data_size < 16 )
	 || ( name_to_id_map_class_identifiers_data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid name to id map class identifiers data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: name to id map entry data:\n",
		 function );
		libcnotify_print_data(
		 name_to_id_map_entry_data,
		 name_to_id_map_entry_data_size,
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (pff_name_to_id_map_entry_t *) name_to_id_map_entry_data )->entry_value,
	 name_to_id_map_entry_value );

	byte_stream_copy_to_uint16_little_endian(
	 ( (pff_name_to_id_map_entry_t *) name_to_id_map_entry_data )->entry_type,
	 name_to_id_map_entry_type );

	byte_stream_copy_to_uint16_little_endian(
	 ( (pff_name_to_id_map_entry_t *) name_to_id_map_entry_data )->entry_number,
	 name_to_id_map_entry_number );

	internal_name_to_id_map_entry->identifier = name_to_id_map_entry_number + 0x8000;

	if( name_to_id_map_entry_type > 5 )
	{
		name_to_id_map_class_identifier_index = (uint16_t) ( ( name_to_id_map_entry_type / 2 ) - 3 );

		name_to_id_map_class_identifier_data_offset = (size_t) name_to_id_map_class_identifier_index * 16;

		if( name_to_id_map_class_identifier_data_offset > ( name_to_id_map_class_identifiers_data_size - 16 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid name to id map class identifier index value exceeds class identifiers data size.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     internal_name_to_id_map_entry->guid,
		     &( name_to_id_map_class_identifiers_data[ name_to_id_map_class_identifier_data_offset ] ),
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy name to id map class identifier.",
			 function );

			 goto on_error;
		}
	}
	else if( name_to_id_map_entry_type == 5 )
	{
		if( memory_copy(
		     internal_name_to_id_map_entry->guid,
		     libfmapi_class_identifier_public_strings,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set public strings class identifier.",
			 function );

			 goto on_error;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: entry: %03d name to id map entry value\t\t: 0x%08" PRIx32 "\n",
		 function,
		 name_to_id_map_entry_index,
		 name_to_id_map_entry_value );

		if( name_to_id_map_entry_type > 5 )
		{
			libcnotify_printf(
			 "%s: entry: %03d name to id map entry type\t\t: 0x%04" PRIx16 " (class identifier: %02" PRIu16 ", class: %s)\n",
			 function,
			 name_to_id_map_entry_index,
			 name_to_id_map_entry_type,
			 name_to_id_map_class_identifier_index,
			 libfmapi_class_identifier_get_name(
			  internal_name_to_id_map_entry->guid ) );
		}
		else if( name_to_id_map_entry_type == 5 )
		{
			libcnotify_printf(
			 "%s: entry: %03d name to id map entry type\t\t: 0x%04" PRIx16 " (class: %s)\n",
			 function,
			 name_to_id_map_entry_index,
			 name_to_id_map_entry_type,
			 libfmapi_class_identifier_get_name(
			  internal_name_to_id_map_entry->guid ) );
		}
		else
		{
			libcnotify_printf(
			 "%s: entry: %03d name to id map entry type\t\t: 0x%04" PRIx16 "\n",
			 function,
			 name_to_id_map_entry_index,
			 name_to_id_map_entry_type );
		}
		libcnotify_printf(
		 "%s: entry: %03d name to id map entry number\t: 0x%04" PRIx16 " (0x%04" PRIx32 ")\n",
		 function,
		 name_to_id_map_entry_index,
		 name_to_id_map_entry_number,
		 internal_name_to_id_map_entry->identifier );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	/* The lowest bit of the name to id map entry type signifies
	 * that the name to id map entry value refers to the name to id map string table or the item values
	 */
	if( ( name_to_id_map_entry_type & 0x0001 ) == 0 )
	{
		internal_name_to_id_map_entry->type          = LIBPFF_NAME_TO_ID_MAP_ENTRY_TYPE_NUMERIC;
		internal_name_to_id_map_entry->numeric_value = name_to_id_map_entry_value;
		internal_name_to_id_map_entry->value_size    = 4;
	}
	else
	{
		if( internal_name_to_id_map_entry->string_value != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: invalid name to id map entry - string value already set.",
			 function );

			goto on_error;
		}
		/* The strings data can be NULL and therefore these bounds are checked on demand
		 */
		if( name_to_id_map_strings_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid name to id map strings data.",
			 function );

			goto on_error;
		}
		if( ( name_to_id_map_strings_data_size < 4 )
		 || ( name_to_id_map_strings_data_size > (size_t) SSIZE_MAX ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid name to id map strings data size value out of bounds.",
			 function );

			goto on_error;
		}
		if( name_to_id_map_entry_value >= ( name_to_id_map_strings_data_size - 4 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid name to id map entry value out of bounds.",
			 function );

			goto on_error;
		}
		name_to_id_map_string_data = &( name_to_id_map_strings_data[ name_to_id_map_entry_value ] );

		byte_stream_copy_to_uint32_little_endian(
		 name_to_id_map_string_data,
		 name_to_id_map_string_size );

		name_to_id_map_string_data += 4;

		internal_name_to_id_map_entry->type = LIBPFF_NAME_TO_ID_MAP_ENTRY_TYPE_STRING;

		if( (size_t) name_to_id_map_string_size > ( name_to_id_map_strings_data_size - ( name_to_id_map_entry_value + 4 ) ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: invalid name to id map string size value out of bounds.\n",
				 function );
			}
#endif
			/* Since the string does not contain an end-of-string character and the size
			 * does not contain a sane value mark the name to ID map entry as corrupted.
			 */
			internal_name_to_id_map_entry->flags |= LIBPFF_NAME_TO_ID_MAP_ENTRY_FLAG_IS_CORRUPTED;
		}
		else
		{
			if( ( name_to_id_map_string_size == 0 )
			 || ( name_to_id_map_string_size > (size32_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid name to id map string size value out of bounds.",
				 function );

				goto on_error;
			}
			result = libpff_value_type_string_contains_zero_bytes(
				  name_to_id_map_string_data,
				  (size_t) name_to_id_map_string_size,
				  error ) ;

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine name to id map entry string contains zero bytes.",
				 function );

				goto on_error;
			}
			else if( result == 0 )
			{
				internal_name_to_id_map_entry->is_ascii_string = 1;
			}
			internal_name_to_id_map_entry->string_value = (uint8_t *) memory_allocate(
			                                                           sizeof( uint8_t ) * (size_t) name_to_id_map_string_size );

			if( internal_name_to_id_map_entry->string_value == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create name to id map entry string.",
				 function );

				goto on_error;
			}
			internal_name_to_id_map_entry->value_size = (size_t) name_to_id_map_string_size;

			if( memory_copy(
			     internal_name_to_id_map_entry->string_value,
			     name_to_id_map_string_data,
			     internal_name_to_id_map_entry->value_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to set name to id map entry string.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( internal_name_to_id_map_entry->is_ascii_string == 0 )
				{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
					result = libuna_utf16_string_size_from_utf16_stream(
						  internal_name_to_id_map_entry->string_value,
						  internal_name_to_id_map_entry->value_size,
						  LIBPFF_ENDIAN_LITTLE,
						  &( internal_name_to_id_map_entry->debug_string_size ),
						  error );
#else
					result = libuna_utf8_string_size_from_utf16_stream(
						  internal_name_to_id_map_entry->string_value,
						  internal_name_to_id_map_entry->value_size,
						  LIBPFF_ENDIAN_LITTLE,
						  &( internal_name_to_id_map_entry->debug_string_size ),
						  error );
#endif
				}
				else
				{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
					result = libuna_utf8_string_size_from_byte_stream(
						  internal_name_to_id_map_entry->string_value,
						  internal_name_to_id_map_entry->value_size,
						  LIBUNA_CODEPAGE_ASCII,
						  &( internal_name_to_id_map_entry->debug_string_size ),
						  error );
#else
					result = libuna_utf8_string_size_from_byte_stream(
						  internal_name_to_id_map_entry->string_value,
						  internal_name_to_id_map_entry->value_size,
						  LIBUNA_CODEPAGE_ASCII,
						  &( internal_name_to_id_map_entry->debug_string_size ),
						  error );
#endif
				}
				if( result != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to determine name to id map entry string size.",
					 function );

					goto on_error;
				}
				internal_name_to_id_map_entry->debug_string = system_string_allocate(
				                                               internal_name_to_id_map_entry->debug_string_size );

				if( internal_name_to_id_map_entry->debug_string == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create UTF-8 name to id map entry string.",
					 function );

					goto on_error;
				}
				if( internal_name_to_id_map_entry->is_ascii_string == 0 )
				{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
					result = libuna_utf16_string_copy_from_utf16_stream(
						  (libuna_utf16_character_t *) internal_name_to_id_map_entry->debug_string,
						  internal_name_to_id_map_entry->debug_string_size,
						  internal_name_to_id_map_entry->string_value,
						  internal_name_to_id_map_entry->value_size,
						  LIBPFF_ENDIAN_LITTLE,
						  error );
#else
					result = libuna_utf8_string_copy_from_utf16_stream(
						  (libuna_utf8_character_t *) internal_name_to_id_map_entry->debug_string,
						  internal_name_to_id_map_entry->debug_string_size,
						  internal_name_to_id_map_entry->string_value,
						  internal_name_to_id_map_entry->value_size,
						  LIBPFF_ENDIAN_LITTLE,
						  error );
#endif
				}
				else
				{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
					result = libuna_utf16_string_copy_from_byte_stream(
						  (libuna_utf16_character_t *) internal_name_to_id_map_entry->debug_string,
						  internal_name_to_id_map_entry->debug_string_size,
						  internal_name_to_id_map_entry->string_value,
						  internal_name_to_id_map_entry->value_size,
						  LIBUNA_CODEPAGE_ASCII,
						  error );
#else
					result = libuna_utf8_string_copy_from_byte_stream(
						  (libuna_utf8_character_t *) internal_name_to_id_map_entry->debug_string,
						  internal_name_to_id_map_entry->debug_string_size,
						  internal_name_to_id_map_entry->string_value,
						  internal_name_to_id_map_entry->value_size,
						  LIBUNA_CODEPAGE_ASCII,
						  error );
#endif
				}
				if( result != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_CONVERSION,
					 LIBCERROR_CONVERSION_ERROR_GENERIC,
					 "%s: unable to set name to id map entry string.",
					 function );

					goto on_error;
				}
				libcnotify_printf(
				 "%s: entry: %03d name to id map entry string\t: %" PRIs_SYSTEM "\n",
				 function,
				 name_to_id_map_entry_index,
				 internal_name_to_id_map_entry->debug_string );
			}
#endif
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( internal_name_to_id_map_entry->debug_string != NULL )
	{
		memory_free(
		 internal_name_to_id_map_entry->debug_string );

		internal_name_to_id_map_entry->debug_string = NULL;
	}
#endif
	return( -1 );
}

/* Retrieves a specific name to map entry
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libpff_name_to_id_map_entry_get_entry_by_identifier(
     libcdata_list_t *name_to_id_map_list,
     uint32_t identifier,
     libpff_internal_name_to_id_map_entry_t **name_to_id_map_entry,
     libcerror_error_t **error )
{
	libcdata_list_element_t *list_element                               = NULL;
	libpff_internal_name_to_id_map_entry_t *search_name_to_id_map_entry = NULL;
	static char *function                                               = "libpff_name_to_id_map_entry_get_entry_by_identifier";
	int element_index                                                   = 0;
	int number_of_elements                                              = 0;

	if( name_to_id_map_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name to id map entry.",
		 function );

		return( -1 );
	}
	*name_to_id_map_entry = NULL;

	if( name_to_id_map_list == NULL )
	{
		return( 0 );
	}
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
		 "%s: unable to retrieve first name to id map entry list element.",
		 function );

		return( -1 );
	}
	for( element_index = 0;
	     element_index < number_of_elements;
	     element_index++ )
	{
/* TODO is this necessary? */
		if( list_element == NULL )
		{
			break;
		}
		if( libcdata_list_element_get_value(
		     list_element,
		     (intptr_t **) &search_name_to_id_map_entry,
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
		if( ( search_name_to_id_map_entry != NULL )
		 && ( search_name_to_id_map_entry->identifier == identifier ) )
		{
			*name_to_id_map_entry = search_name_to_id_map_entry;

			return( 1 );
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
			 "%s: unable to retrieve name to id map entry: %d next list element.",
			 function,
			 element_index );

			return( -1 );
		}
	}
	return( 0 );
}

/* Retrieves the type
 * Returns 1 if successful or -1 on error
 */
int libpff_name_to_id_map_entry_get_type(
     libpff_name_to_id_map_entry_t *name_to_id_map_entry,
     uint8_t *entry_type,
     libcerror_error_t **error )
{
	libpff_internal_name_to_id_map_entry_t *internal_name_to_id_map_entry = NULL;
	static char *function                                                 = "libpff_name_to_id_map_entry_get_type";

	if( name_to_id_map_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name to id map entry.",
		 function );

		return( -1 );
	}
	internal_name_to_id_map_entry = (libpff_internal_name_to_id_map_entry_t *) name_to_id_map_entry;

	if( entry_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry type.",
		 function );

		return( -1 );
	}
	*entry_type = internal_name_to_id_map_entry->type;

	return( 1 );
}

/* Retrieves the number
 * Returns 1 if successful or -1 on error
 */
int libpff_name_to_id_map_entry_get_number(
     libpff_name_to_id_map_entry_t *name_to_id_map_entry,
     uint32_t *number,
     libcerror_error_t **error )
{
	libpff_internal_name_to_id_map_entry_t *internal_name_to_id_map_entry = NULL;
	static char *function                                                 = "libpff_name_to_id_map_entry_get_number";

	if( name_to_id_map_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name to id map entry.",
		 function );

		return( -1 );
	}
	internal_name_to_id_map_entry = (libpff_internal_name_to_id_map_entry_t *) name_to_id_map_entry;

	if( internal_name_to_id_map_entry->type != LIBPFF_NAME_TO_ID_MAP_ENTRY_TYPE_NUMERIC )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported name to id map entry type.",
		 function );

		return( -1 );
	}
	if( number == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number.",
		 function );

		return( -1 );
	}
	*number = internal_name_to_id_map_entry->numeric_value;

	return( 1 );
}

/* Retrieves the UTF-8 string size
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libpff_name_to_id_map_entry_get_utf8_string_size(
     libpff_name_to_id_map_entry_t *name_to_id_map_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libpff_internal_name_to_id_map_entry_t *internal_name_to_id_map_entry = NULL;
	static char *function                                                 = "libpff_name_to_id_map_entry_get_utf8_string_size";

	if( name_to_id_map_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name to id map entry.",
		 function );

		return( -1 );
	}
	internal_name_to_id_map_entry = (libpff_internal_name_to_id_map_entry_t *) name_to_id_map_entry;

	if( internal_name_to_id_map_entry->type != LIBPFF_NAME_TO_ID_MAP_ENTRY_TYPE_STRING )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported name to id map entry type.",
		 function );

		return( -1 );
	}
	if( internal_name_to_id_map_entry->is_ascii_string == 0 )
	{
		if( libuna_utf8_string_size_from_utf16_stream(
		     internal_name_to_id_map_entry->string_value,
		     internal_name_to_id_map_entry->value_size,
		     LIBPFF_ENDIAN_LITTLE,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine name to id map entry UTF-8 string size.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( libuna_utf8_string_size_from_byte_stream(
		     internal_name_to_id_map_entry->string_value,
		     internal_name_to_id_map_entry->value_size,
		     LIBUNA_CODEPAGE_ASCII,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine name to id map entry UTF-8 string size.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-8 string
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libpff_name_to_id_map_entry_get_utf8_string(
     libpff_name_to_id_map_entry_t *name_to_id_map_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libpff_internal_name_to_id_map_entry_t *internal_name_to_id_map_entry = NULL;
	static char *function                                                 = "libpff_name_to_id_map_entry_get_utf8_string";

	if( name_to_id_map_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name to id map entry.",
		 function );

		return( -1 );
	}
	internal_name_to_id_map_entry = (libpff_internal_name_to_id_map_entry_t *) name_to_id_map_entry;

	if( internal_name_to_id_map_entry->type != LIBPFF_NAME_TO_ID_MAP_ENTRY_TYPE_STRING )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported name to id map entry type.",
		 function );

		return( -1 );
	}
	if( internal_name_to_id_map_entry->is_ascii_string == 0 )
	{
		if( libuna_utf8_string_copy_from_utf16_stream(
		     utf8_string,
		     utf8_string_size,
		     internal_name_to_id_map_entry->string_value,
		     internal_name_to_id_map_entry->value_size,
		     LIBPFF_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set UTF-8 name to id map entry string.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( libuna_utf8_string_copy_from_byte_stream(
		     utf8_string,
		     utf8_string_size,
		     internal_name_to_id_map_entry->string_value,
		     internal_name_to_id_map_entry->value_size,
		     LIBUNA_CODEPAGE_ASCII,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set UTF-8 name to id map entry string.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-16 string size
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libpff_name_to_id_map_entry_get_utf16_string_size(
     libpff_name_to_id_map_entry_t *name_to_id_map_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libpff_internal_name_to_id_map_entry_t *internal_name_to_id_map_entry = NULL;
	static char *function                                                 = "libpff_name_to_id_map_entry_get_utf16_string_size";

	if( name_to_id_map_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name to id map entry.",
		 function );

		return( -1 );
	}
	internal_name_to_id_map_entry = (libpff_internal_name_to_id_map_entry_t *) name_to_id_map_entry;

	if( internal_name_to_id_map_entry->type != LIBPFF_NAME_TO_ID_MAP_ENTRY_TYPE_STRING )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported name to id map entry type.",
		 function );

		return( -1 );
	}
	if( internal_name_to_id_map_entry->is_ascii_string == 0 )
	{
		if( libuna_utf16_string_size_from_utf16_stream(
		     internal_name_to_id_map_entry->string_value,
		     internal_name_to_id_map_entry->value_size,
		     LIBPFF_ENDIAN_LITTLE,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine name to id map entry UTF-16 string size.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( libuna_utf16_string_size_from_byte_stream(
		     internal_name_to_id_map_entry->string_value,
		     internal_name_to_id_map_entry->value_size,
		     LIBUNA_CODEPAGE_ASCII,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine name to id map entry UTF-16 string size.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-16 string
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libpff_name_to_id_map_entry_get_utf16_string(
     libpff_name_to_id_map_entry_t *name_to_id_map_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libpff_internal_name_to_id_map_entry_t *internal_name_to_id_map_entry = NULL;
	static char *function                                                 = "libpff_name_to_id_map_entry_get_utf16_string";

	if( name_to_id_map_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name to id map entry.",
		 function );

		return( -1 );
	}
	internal_name_to_id_map_entry = (libpff_internal_name_to_id_map_entry_t *) name_to_id_map_entry;

	if( internal_name_to_id_map_entry->type != LIBPFF_NAME_TO_ID_MAP_ENTRY_TYPE_STRING )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported name to id map entry type.",
		 function );

		return( -1 );
	}
	if( internal_name_to_id_map_entry->is_ascii_string == 0 )
	{
		if( libuna_utf16_string_copy_from_utf16_stream(
		     utf16_string,
		     utf16_string_size,
		     internal_name_to_id_map_entry->string_value,
		     internal_name_to_id_map_entry->value_size,
		     LIBPFF_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set UTF-16 name to id map entry string.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( libuna_utf16_string_copy_from_byte_stream(
		     utf16_string,
		     utf16_string_size,
		     internal_name_to_id_map_entry->string_value,
		     internal_name_to_id_map_entry->value_size,
		     LIBUNA_CODEPAGE_ASCII,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set UTF-16 name to id map entry string.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the GUID
 * Returns 1 if successful or -1 on error
 */
int libpff_name_to_id_map_entry_get_guid(
     libpff_name_to_id_map_entry_t *name_to_id_map_entry,
     uint8_t *guid,
     size_t size,
     libcerror_error_t **error )
{
	libpff_internal_name_to_id_map_entry_t *internal_name_to_id_map_entry = NULL;
	static char *function                                                 = "libpff_name_to_id_map_entry_get_guid";

	if( name_to_id_map_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name to id map entry.",
		 function );

		return( -1 );
	}
	internal_name_to_id_map_entry = (libpff_internal_name_to_id_map_entry_t *) name_to_id_map_entry;

	if( guid == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid GUID.",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: size is too small.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     guid,
	     internal_name_to_id_map_entry->guid,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set GUID.",
		 function );

		return( -1 );
	}
	return( 1 );
}

