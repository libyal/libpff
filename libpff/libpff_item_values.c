/*
 * Item values functions
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
#include "libpff_io_handle.h"
#include "libpff_item_values.h"
#include "libpff_libbfio.h"
#include "libpff_libcdata.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_libuna.h"
#include "libpff_local_descriptor_value.h"
#include "libpff_local_descriptors.h"
#include "libpff_offsets_index.h"
#include "libpff_record_entry.h"
#include "libpff_table.h"
#include "libpff_types.h"
#include "libpff_value_type.h"

/* Creates item values
 * Make sure the value item_values is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_item_values_initialize(
     libpff_item_values_t **item_values,
     uint32_t descriptor_identifier,
     uint64_t data_identifier,
     uint64_t local_descriptors_identifier,
     uint8_t recovered,
     libcerror_error_t **error )
{
	static char *function = "libpff_item_values_initialize";

	if( item_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item values.",
		 function );

		return( -1 );
	}
	if( *item_values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid iitem values value already set.",
		 function );

		return( -1 );
	}
	*item_values = memory_allocate_structure(
	                libpff_item_values_t );

	if( *item_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create item values.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *item_values,
	     0,
	     sizeof( libpff_item_values_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear item values.",
		 function );

		goto on_error;
	}
	( *item_values )->descriptor_identifier        = descriptor_identifier;
	( *item_values )->data_identifier              = data_identifier;
	( *item_values )->local_descriptors_identifier = local_descriptors_identifier;
	( *item_values )->recovered                    = recovered;

	return( 1 );

on_error:
	if( *item_values != NULL )
	{
		memory_free(
		 *item_values );

		*item_values = NULL;
	}
	return( -1 );
}

/* Frees item values
 * Returns 1 if successful or -1 on error
 */
int libpff_item_values_free(
     libpff_item_values_t **item_values,
     libcerror_error_t **error )
{
	static char *function = "libpff_item_values_free";
	int result            = 1;

	if( item_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item values.",
		 function );

		return( -1 );
	}
	if( *item_values != NULL )
	{
		if( ( *item_values )->table != NULL )
		{
			if( libpff_table_free(
			     &( ( *item_values )->table ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free table.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *item_values );

		*item_values = NULL;
	}
	return( result );
}

/* Clone copies the item values
 * Copies the values and creates sub elements if necessary
 * Returns 1 if successful or -1 on error
 */
int libpff_item_values_clone_copy(
     libpff_item_values_t *destination_item_values,
     libpff_item_values_t *source_item_values,
     libcerror_error_t **error )
{
	static char *function = "libpff_item_values_clone_copy";

	if( destination_item_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination item values.",
		 function );

		return( -1 );
	}
	if( destination_item_values->table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination item values - table already set.",
		 function );

		return( -1 );
	}
	if( source_item_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source item values.",
		 function );

		return( -1 );
	}
	if( libpff_table_clone(
	     &( destination_item_values->table ),
	     source_item_values->table,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to clone table.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the item values for a specific descriptor
 * Returns 1 if successful or -1 on error
 */
int libpff_item_values_read(
     libpff_item_values_t *item_values,
     libcdata_list_t *name_to_id_map_list,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     int debug_item_type,
     libcerror_error_t **error )
{
	static char *function = "libpff_item_values_read";

	if( item_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item values.",
		 function );

		return( -1 );
	}
	if( item_values->table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid item values - table already set.",
		 function );

		return( -1 );
	}
	if( libpff_table_initialize(
	     &( item_values->table ),
	     item_values->descriptor_identifier,
	     item_values->data_identifier,
	     item_values->local_descriptors_identifier,
	     item_values->recovered,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create table.",
		 function );

		goto on_error;
	}
	if( libpff_table_read(
	     item_values->table,
	     io_handle,
	     file_io_handle,
	     offsets_index,
	     name_to_id_map_list,
	     debug_item_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read table.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( item_values->table != NULL )
	{
		libpff_table_free(
		 &( item_values->table ),
		 NULL );
	}
	return( -1 );
}

/* Retrieves the local descriptor value for the specific identifier
 * Returns 1 if successful, 0 if no value was found or -1 on error
 */
int libpff_item_values_get_local_descriptors_value_by_identifier(
     libpff_item_values_t *item_values,
     libbfio_handle_t *file_io_handle,
     uint32_t descriptor_identifier,
     libpff_local_descriptor_value_t **local_descriptor_value,
     libcerror_error_t **error )
{
	static char *function = "libpff_item_values_get_local_descriptors_value_by_identifier";
	int result            = 0;

	if( item_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item values.",
		 function );

		return( -1 );
	}
	result = libpff_table_get_local_descriptors_value_by_identifier(
	          item_values->table,
	          file_io_handle,
	          (uint64_t) descriptor_identifier,
	          local_descriptor_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve local descriptor identifier: %" PRIu32 ".",
		 function,
		 descriptor_identifier );

		return( -1 );
	}
	return( result );
}

/* Retrieves the number of item value sets
 * Returns 1 if successful or -1 on error
 */
int libpff_item_values_get_number_of_record_sets(
     libpff_item_values_t *item_values,
     libcdata_list_t *name_to_id_map_list,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     int *number_of_record_sets,
     libcerror_error_t **error )
{
	static char *function = "libpff_item_values_get_number_of_record_sets";

	if( item_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item values.",
		 function );

		return( -1 );
	}
	if( item_values->table == NULL )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: reading item values of descriptor: %" PRIu32 "\n",
			 function,
			 item_values->descriptor_identifier );
		}
#endif
		if( libpff_item_values_read(
		     item_values,
		     name_to_id_map_list,
		     io_handle,
		     file_io_handle,
		     offsets_index,
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
	}
	if( libpff_table_get_number_of_record_sets(
	     item_values->table,
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

/* Retrieves the record entry matching the entry and value type pair from the item values.
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE flag is set
 * the value type is ignored and set. The default behavior is a strict
 * matching of the value type. In this case the value type must be filled
 * with the corresponding value type
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * to use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_item_values_get_record_entry_by_type(
     libpff_item_values_t *item_values,
     libcdata_list_t *name_to_id_map_list,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     int record_set_index,
     uint32_t entry_type,
     uint32_t value_type,
     libpff_record_entry_t **record_entry,
     uint8_t flags,
     libcerror_error_t **error )
{
	static char *function = "libpff_item_values_get_record_entry_by_type";
	int result            = 0;

	if( item_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item values.",
		 function );

		return( -1 );
	}
	if( item_values->table == NULL )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: reading item values of descriptor: %" PRIu32 "\n",
			 function,
			 item_values->descriptor_identifier );
		}
#endif
		if( libpff_item_values_read(
		     item_values,
		     name_to_id_map_list,
		     io_handle,
		     file_io_handle,
		     offsets_index,
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
	}
	result = libpff_table_get_record_entry_by_type(
	          item_values->table,
	          record_set_index,
	          entry_type,
	          value_type,
	          record_entry,
	          flags,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record entry.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the record entry matching the UTF-8 encoded entry name from the item values.
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE flag is set
 * the value type is ignored and set. The default behavior is a strict
 * matching of the value type. In this case the value type must be filled
 * with the corresponding value type
 *
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_item_values_get_record_entry_by_utf8_name(
     libpff_item_values_t *item_values,
     libcdata_list_t *name_to_id_map_list,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     int record_set_index,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint32_t value_type,
     libpff_record_entry_t **record_entry,
     uint8_t flags,
     libcerror_error_t **error )
{
	static char *function = "libpff_item_values_get_record_entry_by_utf8_name";
	int result            = 0;

	if( item_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item values.",
		 function );

		return( -1 );
	}
	if( item_values->table == NULL )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: reading item values of descriptor: %" PRIu32 "\n",
			 function,
			 item_values->descriptor_identifier );
		}
#endif
		if( libpff_item_values_read(
		     item_values,
		     name_to_id_map_list,
		     io_handle,
		     file_io_handle,
		     offsets_index,
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
	}
	result = libpff_table_get_record_entry_by_utf8_name(
	          item_values->table,
	          record_set_index,
	          utf8_string,
	          utf8_string_length,
	          value_type,
	          record_entry,
	          flags,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record entry.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the record entry matching the UTF-16 encoded entry name from the item values.
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE flag is set
 * the value type is ignored and set. The default behavior is a strict
 * matching of the value type. In this case the value type must be filled
 * with the corresponding value type
 *
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_item_values_get_record_entry_by_utf16_name(
     libpff_item_values_t *item_values,
     libcdata_list_t *name_to_id_map_list,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     int record_set_index,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint32_t value_type,
     libpff_record_entry_t **record_entry,
     uint8_t flags,
     libcerror_error_t **error )
{
	static char *function = "libpff_item_values_get_record_entry_by_utf16_name";
	int result            = 0;

	if( item_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item values.",
		 function );

		return( -1 );
	}
	if( item_values->table == NULL )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: reading item values of descriptor: %" PRIu32 "\n",
			 function,
			 item_values->descriptor_identifier );
		}
#endif
		if( libpff_item_values_read(
		     item_values,
		     name_to_id_map_list,
		     io_handle,
		     file_io_handle,
		     offsets_index,
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
	}
	result = libpff_table_get_record_entry_by_utf16_name(
	          item_values->table,
	          record_set_index,
	          utf16_string,
	          utf16_string_length,
	          value_type,
	          record_entry,
	          flags,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record entry.",
		 function );

		return( -1 );
	}
	return( result );
}

