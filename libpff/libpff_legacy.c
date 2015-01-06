/*
 * Legacy functions
 *
 * Copyright (C) 2008-2015, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <types.h>

#include "libpff_debug.h"
#include "libpff_definitions.h"
#include "libpff_item.h"
#include "libpff_item_values.h"
#include "libpff_mapi.h"
#include "libpff_record_entry.h"
#include "libpff_table.h"
#include "libpff_types.h"

/* Retrieves the number of sets values
 * Returns 1 if successful or -1 on error
 */
int libpff_item_get_number_of_sets(
     libpff_item_t *item,
     uint32_t *number_of_sets,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_item_get_number_of_sets";
	int table_number_of_record_sets       = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
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
	if( number_of_sets == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of sets.",
		 function );

		return( -1 );
	}
	if( internal_item->item_values->table == NULL )
	{
		if( libpff_item_values_read(
		     internal_item->item_values,
		     internal_item->internal_file->name_to_id_map_list,
		     internal_item->internal_file->io_handle,
		     internal_item->file_io_handle,
		     internal_item->internal_file->offsets_index,
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
	     &table_number_of_record_sets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of record_sets.",
		 function );

		return( -1 );
	}
	*number_of_sets = (uint32_t) table_number_of_record_sets;

	return( 1 );
}

/* Retrieves the entry and value type of a specific entry
 * Returns 1 if successful or -1 on error
 */
int libpff_item_get_entry_type(
     libpff_item_t *item,
     int set_index,
     int entry_index,
     uint32_t *entry_type,
     uint32_t *value_type,
     libpff_name_to_id_map_entry_t **name_to_id_map_entry,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	static char *function                 = "libpff_item_get_entry_type";

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
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
		if( libpff_item_values_read(
		     internal_item->item_values,
		     internal_item->internal_file->name_to_id_map_list,
		     internal_item->internal_file->io_handle,
		     internal_item->file_io_handle,
		     internal_item->internal_file->offsets_index,
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
	if( libpff_table_get_entry_type_by_index(
	     internal_item->item_values->table,
	     set_index,
	     entry_index,
	     entry_type,
	     value_type,
	     name_to_id_map_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry type.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the value type of a specific entry type
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * the use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if not such value or -1 on error
 */
int libpff_item_get_value_type(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint32_t *value_type,
     uint8_t flags,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_value_type";
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	if( value_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value type.",
		 function );

		return( -1 );
	}
	if( ( flags & ~( LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP ) ) != 0 )
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
	result = libpff_item_values_get_record_entry_by_type(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          entry_type,
	          *value_type,
	          &record_entry,
	          flags | LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
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
	else if( result != 0 )
	{
		if( libpff_record_entry_get_value_type(
		     record_entry,
		     value_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value type.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the boolean value of a specific entry
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * the use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_boolean(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint8_t *entry_value,
     uint8_t flags,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_boolean";
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_record_entry_by_type(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          entry_type,
	          LIBPFF_VALUE_TYPE_BOOLEAN,
	          &record_entry,
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
	else if( result != 0 )
	{
		if( libpff_record_entry_get_value_boolean(
		     record_entry,
		     entry_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve boolean value of record entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the boolean value of a specific entry for an UTF-8 encoded name
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_boolean_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     uint8_t *entry_value,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_boolean_by_utf8_name";
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_record_entry_by_utf8_name(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          utf8_entry_name,
	          utf8_entry_name_length,
	          LIBPFF_VALUE_TYPE_BOOLEAN,
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

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libpff_record_entry_get_value_boolean(
		     record_entry,
		     entry_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve boolean value of recored entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the boolean value of a specific entry for an UTF-16 encoded name
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_boolean_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     uint8_t *entry_value,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_boolean_by_utf16_name";
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_record_entry_by_utf16_name(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          utf16_entry_name,
	          utf16_entry_name_length,
	          LIBPFF_VALUE_TYPE_BOOLEAN,
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

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libpff_record_entry_get_value_boolean(
		     record_entry,
		     entry_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve boolean value of record entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the 16-bit value of a specific entry
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * the use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_16bit(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint16_t *entry_value,
     uint8_t flags,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_16bit";
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	if( ( flags & ~( LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP ) ) != 0 )
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
	result = libpff_item_values_get_record_entry_by_type(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          entry_type,
	          LIBPFF_VALUE_TYPE_INTEGER_16BIT_SIGNED,
	          &record_entry,
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
	else if( result != 0 )
	{
		if( libpff_record_entry_get_value_16bit(
		     record_entry,
		     entry_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve 16-bit value of record entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the 16-bit value of a specific entry for an UTF-8 encoded name
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_16bit_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     uint16_t *entry_value,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_16bit_by_utf8_name";
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_record_entry_by_utf8_name(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          utf8_entry_name,
	          utf8_entry_name_length,
	          LIBPFF_VALUE_TYPE_INTEGER_16BIT_SIGNED,
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

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libpff_record_entry_get_value_16bit(
		     record_entry,
		     entry_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve 16-bit value of record entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the 16-bit value of a specific entry for an UTF-16 encoded name
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_16bit_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     uint16_t *entry_value,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_16bit_by_utf16_name";
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_record_entry_by_utf16_name(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          utf16_entry_name,
	          utf16_entry_name_length,
	          LIBPFF_VALUE_TYPE_INTEGER_16BIT_SIGNED,
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

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libpff_record_entry_get_value_16bit(
		     record_entry,
		     entry_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve 16-bit value of record entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the 32-bit value of a specific entry
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * the use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_32bit(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint32_t *entry_value,
     uint8_t flags,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_32bit";
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	if( ( flags & ~( LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP ) ) != 0 )
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
	result = libpff_item_values_get_record_entry_by_type(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          entry_type,
	          LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED,
	          &record_entry,
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
	else if( result != 0 )
	{
		if( libpff_record_entry_get_value_32bit(
		     record_entry,
		     entry_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve 32-bit value of record entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the 32-bit value of a specific entry for an UTF-8 encoded name
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_32bit_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     uint32_t *entry_value,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_32bit_by_utf8_name";
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_record_entry_by_utf8_name(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          utf8_entry_name,
	          utf8_entry_name_length,
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
		 "%s: unable to retrieve record entry.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libpff_record_entry_get_value_32bit(
		     record_entry,
		     entry_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve 32-bit value of record entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the 32-bit value of a specific entry for an UTF-16 encoded name
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_32bit_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     uint32_t *entry_value,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_32bit_by_utf16_name";
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_record_entry_by_utf16_name(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          utf16_entry_name,
	          utf16_entry_name_length,
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
		 "%s: unable to retrieve record entry.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libpff_record_entry_get_value_32bit(
		     record_entry,
		     entry_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve 32-bit value of record entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the 64-bit value of a specific entry
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * the use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_64bit(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint64_t *entry_value,
     uint8_t flags,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_64bit";
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	if( ( flags & ~( LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP ) ) != 0 )
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
	result = libpff_item_values_get_record_entry_by_type(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          entry_type,
	          LIBPFF_VALUE_TYPE_INTEGER_64BIT_SIGNED,
	          &record_entry,
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
	else if( result != 0 )
	{
		if( libpff_record_entry_get_value_64bit(
		     record_entry,
		     entry_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve 64-bit value of record entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the 64-bit value of a specific entry for an UTF-8 encoded name
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_64bit_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     uint64_t *entry_value,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_64bit_by_utf8_name";
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_record_entry_by_utf8_name(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          utf8_entry_name,
	          utf8_entry_name_length,
	          LIBPFF_VALUE_TYPE_INTEGER_64BIT_SIGNED,
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

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libpff_record_entry_get_value_64bit(
		     record_entry,
		     entry_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve 64-bit value of record entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the 64-bit value of a specific entry for an UTF-16 encoded name
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_64bit_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     uint64_t *entry_value,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_64bit_by_utf16_name";
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_record_entry_by_utf16_name(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          utf16_entry_name,
	          utf16_entry_name_length,
	          LIBPFF_VALUE_TYPE_INTEGER_64BIT_SIGNED,
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

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libpff_record_entry_get_value_64bit(
		     record_entry,
		     entry_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve 64-bit value of record entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the 64-bit filetime value of a specific entry
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * the use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_filetime(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint64_t *entry_value,
     uint8_t flags,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_filetime";
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	if( ( flags & ~( LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP ) ) != 0 )
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
	result = libpff_item_values_get_record_entry_by_type(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          entry_type,
	          LIBPFF_VALUE_TYPE_FILETIME,
	          &record_entry,
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
	else if( result != 0 )
	{
		if( libpff_record_entry_get_value_64bit(
		     record_entry,
		     entry_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve 64-bit value of record entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the 64-bit filetime value of a specific entry for an UTF-8 encoded name
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_filetime_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     uint64_t *entry_value,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_filetime_by_utf8_name";
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_record_entry_by_utf8_name(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          utf8_entry_name,
	          utf8_entry_name_length,
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
		 "%s: unable to retrieve record entry.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libpff_record_entry_get_value_64bit(
		     record_entry,
		     entry_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve 64-bit value of record entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the 64-bit filetime value of a specific entry for an UTF-16 encoded name
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_filetime_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     uint64_t *entry_value,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_filetime_by_utf16_name";
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_record_entry_by_utf16_name(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          utf16_entry_name,
	          utf16_entry_name_length,
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
		 "%s: unable to retrieve record entry.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libpff_record_entry_get_value_64bit(
		     record_entry,
		     entry_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve 64-bit value of record entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the size value of a specific entry
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * the use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_size(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     size_t *entry_value,
     uint8_t flags,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_size";
	uint32_t value_type                   = 0;
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	if( ( flags & ~( LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP ) ) != 0 )
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
	result = libpff_item_values_get_record_entry_by_type(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          entry_type,
	          0,
	          &record_entry,
	          flags | LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
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

			return( -1 );
		}
		if( ( value_type != LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED )
		 && ( value_type != LIBPFF_VALUE_TYPE_INTEGER_64BIT_SIGNED ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported value type: 0x%04" PRIx32 ".",
			 function,
			 value_type );

			return( -1 );
		}
		if( libpff_record_entry_get_value_size(
		     record_entry,
		     entry_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve size value of record entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the size value of a specific entry for an UTF-8 encoded name
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_size_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     size_t *entry_value,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_size_by_utf8_name";
	uint32_t value_type                   = 0;
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_record_entry_by_utf8_name(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          utf8_entry_name,
	          utf8_entry_name_length,
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

		return( -1 );
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

			return( -1 );
		}
		if( ( value_type != LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED )
		 && ( value_type != LIBPFF_VALUE_TYPE_INTEGER_64BIT_SIGNED ) )
		{
			return( 0 );
		}
		if( libpff_record_entry_get_value_size(
		     record_entry,
		     entry_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve size value of record entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the size value of a specific entry for an UTF-16 encoded name
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_size_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     size_t *entry_value,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_size_by_utf16_name";
	uint32_t value_type                   = 0;
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_record_entry_by_utf16_name(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          utf16_entry_name,
	          utf16_entry_name_length,
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

		return( -1 );
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

			return( -1 );
		}
		if( ( value_type != LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED )
		 && ( value_type != LIBPFF_VALUE_TYPE_INTEGER_64BIT_SIGNED ) )
		{
			return( 0 );
		}
		if( libpff_record_entry_get_value_size(
		     record_entry,
		     entry_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve size value of record entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the floating point value of a specific entry
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * the use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_floating_point(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     double *entry_value,
     uint8_t flags,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_floating_point";
	uint32_t value_type                   = 0;
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	if( ( flags & ~( LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP ) ) != 0 )
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
	result = libpff_item_values_get_record_entry_by_type(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          entry_type,
	          0,
	          &record_entry,
	          flags | LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
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

			return( -1 );
		}
		if( ( value_type != LIBPFF_VALUE_TYPE_FLOAT_32BIT )
		 && ( value_type != LIBPFF_VALUE_TYPE_DOUBLE_64BIT ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported value type: 0x%04" PRIx32 ".",
			 function,
			 value_type );

			return( -1 );
		}
		if( libpff_record_entry_get_value_floating_point(
		     record_entry,
		     entry_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve floating point value of record entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the floating point value of a specific entry for an UTF-8 encoded name
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_floating_point_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     double *entry_value,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_floating_point_by_utf8_name";
	uint32_t value_type                   = 0;
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_record_entry_by_utf8_name(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          utf8_entry_name,
	          utf8_entry_name_length,
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

		return( -1 );
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

			return( -1 );
		}
		if( ( value_type != LIBPFF_VALUE_TYPE_FLOAT_32BIT )
		 && ( value_type != LIBPFF_VALUE_TYPE_DOUBLE_64BIT ) )
		{
			return( 0 );
		}
		if( libpff_record_entry_get_value_floating_point(
		     record_entry,
		     entry_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve floating point value of record entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the floating point value of a specific entry for an UTF-16 encoded name
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_floating_point_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     double *entry_value,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_floating_point_by_utf16_name";
	uint32_t value_type                   = 0;
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_record_entry_by_utf16_name(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          utf16_entry_name,
	          utf16_entry_name_length,
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

		return( -1 );
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

			return( -1 );
		}
		if( ( value_type != LIBPFF_VALUE_TYPE_FLOAT_32BIT )
		 && ( value_type != LIBPFF_VALUE_TYPE_DOUBLE_64BIT ) )
		{
			return( 0 );
		}
		if( libpff_record_entry_get_value_floating_point(
		     record_entry,
		     entry_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve floating point value of record entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the UTF-8 string size of a specific entry
 * The returned size includes the end of string character
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * the use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_utf8_string_size(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     size_t *utf8_string_size,
     uint8_t flags,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_utf8_string_size";
	uint32_t value_type                   = 0;
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( flags & ~( LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP ) ) != 0 )
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
	result = libpff_item_values_get_record_entry_by_type(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          entry_type,
	          0,
	          &record_entry,
	          flags | LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
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

			return( -1 );
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

			return( -1 );
		}
		if( libpff_record_entry_get_value_utf8_string_size_with_codepage(
		     record_entry,
		     internal_item->internal_file->io_handle->ascii_codepage,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record entry UTF-8 string size.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the UTF-8 string size of a specific entry for an UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_utf8_string_size_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_utf8_string_size_by_utf8_name";
	uint32_t value_type                   = 0;
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_record_entry_by_utf8_name(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          utf8_entry_name,
	          utf8_entry_name_length,
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

		return( -1 );
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

			return( -1 );
		}
		if( ( value_type != LIBPFF_VALUE_TYPE_STRING_ASCII )
		 && ( value_type != LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
		{
			return( 0 );
		}
		if( libpff_record_entry_get_value_utf8_string_size_with_codepage(
		     record_entry,
		     internal_item->internal_file->io_handle->ascii_codepage,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record entry UTF-8 string size.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the UTF-8 string size of a specific entry for an UTF-16 encoded name
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_utf8_string_size_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_utf8_string_size_by_utf16_name";
	uint32_t value_type                   = 0;
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_record_entry_by_utf16_name(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          utf16_entry_name,
	          utf16_entry_name_length,
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

		return( -1 );
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

			return( -1 );
		}
		if( ( value_type != LIBPFF_VALUE_TYPE_STRING_ASCII )
		 && ( value_type != LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
		{
			return( 0 );
		}
		if( libpff_record_entry_get_value_utf8_string_size_with_codepage(
		     record_entry,
		     internal_item->internal_file->io_handle->ascii_codepage,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record entry UTF-8 string size.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the UTF-8 string value of a specific entry
 * The function uses a codepage if necessary, it uses the codepage set for the library
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * the use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_utf8_string(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     uint8_t flags,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_utf8_string";
	uint32_t value_type                   = 0;
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( flags & ~( LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP ) ) != 0 )
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
	result = libpff_item_values_get_record_entry_by_type(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          entry_type,
	          0,
	          &record_entry,
	          flags | LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
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

			return( -1 );
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

			return( -1 );
		}
		if( libpff_record_entry_get_value_utf8_string_with_codepage(
		     record_entry,
		     internal_item->internal_file->io_handle->ascii_codepage,
		     utf8_string,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 string value of record entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the UTF-8 string value of a specific entry for an UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_utf8_string_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_utf8_string_by_utf8_name";
	uint32_t value_type                   = 0;
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_record_entry_by_utf8_name(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          utf8_entry_name,
	          utf8_entry_name_length,
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

		return( -1 );
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

			return( -1 );
		}
		if( ( value_type != LIBPFF_VALUE_TYPE_STRING_ASCII )
		 && ( value_type != LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
		{
			return( 0 );
		}
		if( libpff_record_entry_get_value_utf8_string_with_codepage(
		     record_entry,
		     internal_item->internal_file->io_handle->ascii_codepage,
		     utf8_string,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 string value of record entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the UTF-8 string value of a specific entry for an UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_utf8_string_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_utf8_string_by_utf16_name";
	uint32_t value_type                   = 0;
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_record_entry_by_utf16_name(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          utf16_entry_name,
	          utf16_entry_name_length,
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

		return( -1 );
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

			return( -1 );
		}
		if( ( value_type != LIBPFF_VALUE_TYPE_STRING_ASCII )
		 && ( value_type != LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
		{
			return( 0 );
		}
		if( libpff_record_entry_get_value_utf8_string_with_codepage(
		     record_entry,
		     internal_item->internal_file->io_handle->ascii_codepage,
		     utf8_string,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 string value of record entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the UTF-16 string size of a specific entry
 * The returned size includes the end of string character
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * the use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_utf16_string_size(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     size_t *utf16_string_size,
     uint8_t flags,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_utf16_string_size";
	uint32_t value_type                   = 0;
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( flags & ~( LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP ) ) != 0 )
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
	result = libpff_item_values_get_record_entry_by_type(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          entry_type,
	          0,
	          &record_entry,
	          flags | LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
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

			return( -1 );
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

			return( -1 );
		}
		if( libpff_record_entry_get_value_utf16_string_size_with_codepage(
		     record_entry,
		     internal_item->internal_file->io_handle->ascii_codepage,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record entry UTF-16 string size.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the UTF-16 string size of a specific entry for an UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_utf16_string_size_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_utf16_string_size_by_utf8_name";
	uint32_t value_type                   = 0;
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_record_entry_by_utf8_name(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          utf8_entry_name,
	          utf8_entry_name_length,
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

		return( -1 );
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

			return( -1 );
		}
		if( ( value_type != LIBPFF_VALUE_TYPE_STRING_ASCII )
		 && ( value_type != LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
		{
			return( 0 );
		}
		if( libpff_record_entry_get_value_utf16_string_size_with_codepage(
		     record_entry,
		     internal_item->internal_file->io_handle->ascii_codepage,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record entry UTF-16 string size.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the UTF-16 string size of a specific entry for an UTF-16 encoded name
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_utf16_string_size_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_utf16_string_size_by_utf16_name";
	uint32_t value_type                   = 0;
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_record_entry_by_utf16_name(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          utf16_entry_name,
	          utf16_entry_name_length,
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

		return( -1 );
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

			return( -1 );
		}
		if( ( value_type != LIBPFF_VALUE_TYPE_STRING_ASCII )
		 && ( value_type != LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
		{
			return( 0 );
		}
		if( libpff_record_entry_get_value_utf16_string_size_with_codepage(
		     record_entry,
		     internal_item->internal_file->io_handle->ascii_codepage,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record entry UTF-16 string size.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the UTF-16 string value of a specific entry
 * The function uses a codepage if necessary, it uses the codepage set for the library
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * the use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_utf16_string(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     uint8_t flags,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_utf16_string";
	uint32_t value_type                   = 0;
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( flags & ~( LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP ) ) != 0 )
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
	result = libpff_item_values_get_record_entry_by_type(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          entry_type,
	          0,
	          &record_entry,
	          flags | LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
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

			return( -1 );
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

			return( -1 );
		}
		if( libpff_record_entry_get_value_utf16_string_with_codepage(
		     record_entry,
		     internal_item->internal_file->io_handle->ascii_codepage,
		     utf16_string,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 string value of record entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the UTF-16 string value of a specific entry for an UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_utf16_string_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_utf16_string_by_utf8_name";
	uint32_t value_type                   = 0;
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_record_entry_by_utf8_name(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          utf8_entry_name,
	          utf8_entry_name_length,
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

		return( -1 );
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

			return( -1 );
		}
		if( ( value_type != LIBPFF_VALUE_TYPE_STRING_ASCII )
		 && ( value_type != LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
		{
			return( 0 );
		}
		if( libpff_record_entry_get_value_utf16_string_with_codepage(
		     record_entry,
		     internal_item->internal_file->io_handle->ascii_codepage,
		     utf16_string,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 string value of record entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the UTF-16 string value of a specific entry for an UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libpff_item_get_entry_value_utf16_string_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_utf16_string_by_utf16_name";
	uint32_t value_type                   = 0;
	int result                            = 0;

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

	if( internal_item->internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_item->internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item - invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_record_entry_by_utf16_name(
	          internal_item->item_values,
	          internal_item->internal_file->name_to_id_map_list,
	          internal_item->internal_file->io_handle,
	          internal_item->file_io_handle,
	          internal_item->internal_file->offsets_index,
	          set_index,
	          utf16_entry_name,
	          utf16_entry_name_length,
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

		return( -1 );
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

			return( -1 );
		}
		if( ( value_type != LIBPFF_VALUE_TYPE_STRING_ASCII )
		 && ( value_type != LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
		{
			return( 0 );
		}
		if( libpff_record_entry_get_value_utf16_string_with_codepage(
		     record_entry,
		     internal_item->internal_file->io_handle->ascii_codepage,
		     utf16_string,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 string value of record entry.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

