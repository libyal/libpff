/*
 * Legacy functions
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
#include "libpff_item_values.h"
#include "libpff_mapi.h"
#include "libpff_mapi_value.h"
#include "libpff_multi_value.h"
#include "libpff_record_entry.h"
#include "libpff_table.h"
#include "libpff_types.h"

/* Retrieves a specific orphan item
 * Returns 1 if successful or -1 on error
 */
int libpff_file_get_orphan_item(
     libpff_file_t *file,
     int orphan_item_index,
     libpff_item_t **orphan_item,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *orphan_item_tree_node = NULL;
	libpff_internal_file_t *internal_file       = NULL;
	static char *function                       = "libpff_file_get_orphan_item";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libpff_internal_file_t *) file;

	if( internal_file->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
		 function );

		return( -1 );
	}
	if( orphan_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid orphan item.",
		 function );

		return( -1 );
	}
	if( libcdata_list_get_value_by_index(
	     internal_file->orphan_item_list,
	     orphan_item_index,
	     (intptr_t **) &orphan_item_tree_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve orphan item tree node: %d.",
		 function,
		 orphan_item_index );

		return( -1 );
	}
	if( libpff_item_initialize(
	     orphan_item,
	     internal_file->io_handle,
	     internal_file->file_io_handle,
	     internal_file->name_to_id_map_list,
	     internal_file->descriptors_index,
	     internal_file->offsets_index,
	     internal_file->item_tree,
	     orphan_item_tree_node,
	     LIBPFF_ITEM_FLAGS_DEFAULT,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create orphan item.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific recovered item
 * Returns 1 if successful or -1 on error
 */
int libpff_file_get_recovered_item(
     libpff_file_t *file,
     int recovered_item_index,
     libpff_item_t **recovered_item,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *recovered_item_tree_node = NULL;
	libpff_internal_file_t *internal_file          = NULL;
	static char *function                          = "libpff_file_get_recovered_item";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libpff_internal_file_t *) file;

	if( internal_file->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
		 function );

		return( -1 );
	}
	if( recovered_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid recovered item.",
		 function );

		return( -1 );
	}
	if( libcdata_list_get_value_by_index(
	     internal_file->recovered_item_list,
	     recovered_item_index,
	     (intptr_t **) &recovered_item_tree_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve recovered item tree node: %d.",
		 function,
		 recovered_item_index );

		return( -1 );
	}
	if( libpff_item_initialize(
	     recovered_item,
	     internal_file->io_handle,
	     internal_file->file_io_handle,
	     internal_file->name_to_id_map_list,
	     internal_file->descriptors_index,
	     internal_file->offsets_index,
	     internal_file->item_tree,
	     recovered_item_tree_node,
	     LIBPFF_ITEM_FLAGS_DEFAULT,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create recovered item.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Clones an item
 * Returns 1 if successful or -1 on error
 */
int libpff_item_clone(
     libpff_item_t **destination_item,
     libpff_item_t *source_item,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_source_item = NULL;
	static char *function                        = "libpff_item_clone";

	if( destination_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination item.",
		 function );

		return( -1 );
	}
	if( *destination_item != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination item already set.",
		 function );

		return( -1 );
	}
	if( source_item == NULL )
	{
		*destination_item = NULL;

		return( 1 );
	}
	internal_source_item = (libpff_internal_item_t *) source_item;

	if( libpff_item_initialize(
	     destination_item,
	     internal_source_item->io_handle,
	     internal_source_item->file_io_handle,
	     internal_source_item->name_to_id_map_list,
	     internal_source_item->descriptors_index,
	     internal_source_item->offsets_index,
	     internal_source_item->item_tree,
	     internal_source_item->item_tree_node,
	     internal_source_item->flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination item.",
		 function );

		return( -1 );
	}
	( (libpff_internal_item_t *) *destination_item )->type = internal_source_item->type;

	/* Clones the item values sub elements from the cached sub item values
	 */
	if( libpff_item_values_clone_copy(
	     ( (libpff_internal_item_t *) *destination_item )->item_values,
	     internal_source_item->item_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy item values.",
		 function );

		libpff_item_free(
		 destination_item,
		 NULL );

		return( -1 );
	}
	return( 1 );
}

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
 * to use the mapped entry value. In this case named properties are not
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
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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

/* Retrieves the value of a specific entry
 *
 * The value type must be filled with the corresponding value type
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
 * Returns 1 if successful, 0 if not such value or -1 on error
 */
int libpff_item_get_entry_value(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint32_t *value_type,
     uint8_t **value_data,
     size_t *value_data_size,
     uint8_t flags,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value";
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
	if( value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data.",
		 function );

		return( -1 );
	}
	if( value_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data size.",
		 function );

		return( -1 );
	}
	if( ( flags & ~( LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE | LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP ) ) != 0 )
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
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
	          set_index,
	          entry_type,
	          *value_type,
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
		if( ( flags & LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE ) != 0 )
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
		if( libpff_record_entry_get_value_data(
		     record_entry,
		     value_data,
		     value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value data.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the value of a specific entry for an UTF-8 encoded name
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_item_get_entry_value_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     uint32_t *value_type,
     uint8_t **value_data,
     size_t *value_data_size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_by_utf8_name";
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
	if( value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data.",
		 function );

		return( -1 );
	}
	if( value_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data size.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_record_entry_by_utf8_name(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_value_data(
		     record_entry,
		     value_data,
		     value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value data.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the value of a specific entry for an UTF-16 encoded name
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_item_get_entry_value_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     uint32_t *value_type,
     uint8_t **value_data,
     size_t *value_data_size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_by_utf16_name";
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
	if( value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data.",
		 function );

		return( -1 );
	}
	if( value_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data size.",
		 function );

		return( -1 );
	}
	result = libpff_item_values_get_record_entry_by_utf16_name(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_value_data(
		     record_entry,
		     value_data,
		     value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value data.",
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
 * to use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if not available or -1 on error
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

	result = libpff_item_values_get_record_entry_by_type(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_boolean(
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
 * Returns 1 if successful, 0 if not available or -1 on error
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

	result = libpff_item_values_get_record_entry_by_utf8_name(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_boolean(
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
 * Returns 1 if successful, 0 if not available or -1 on error
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

	result = libpff_item_values_get_record_entry_by_utf16_name(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_boolean(
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
 * to use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if not available or -1 on error
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
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_16bit_integer(
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
 * Returns 1 if successful, 0 if not available or -1 on error
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

	result = libpff_item_values_get_record_entry_by_utf8_name(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_16bit_integer(
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
 * Returns 1 if successful, 0 if not available or -1 on error
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

	result = libpff_item_values_get_record_entry_by_utf16_name(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_16bit_integer(
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
 * to use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if not available or -1 on error
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
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_32bit_integer(
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
 * Returns 1 if successful, 0 if not available or -1 on error
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

	result = libpff_item_values_get_record_entry_by_utf8_name(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_32bit_integer(
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
 * Returns 1 if successful, 0 if not available or -1 on error
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

	result = libpff_item_values_get_record_entry_by_utf16_name(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_32bit_integer(
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
 * to use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if not available or -1 on error
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
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_64bit_integer(
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
 * Returns 1 if successful, 0 if not available or -1 on error
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

	result = libpff_item_values_get_record_entry_by_utf8_name(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_64bit_integer(
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
 * Returns 1 if successful, 0 if not available or -1 on error
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

	result = libpff_item_values_get_record_entry_by_utf16_name(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_64bit_integer(
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
 * to use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if not available or -1 on error
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
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_filetime(
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
 * Returns 1 if successful, 0 if not available or -1 on error
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

	result = libpff_item_values_get_record_entry_by_utf8_name(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_filetime(
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
 * Returns 1 if successful, 0 if not available or -1 on error
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

	result = libpff_item_values_get_record_entry_by_utf16_name(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_filetime(
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
 * to use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if not available or -1 on error
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
	size64_t size_entry_value             = 0;
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

	if( entry_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry value.",
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
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_size(
		     record_entry,
		     &size_entry_value,
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
		*entry_value = (size_t) size_entry_value;
	}
	return( result );
}

/* Retrieves the size value of a specific entry for an UTF-8 encoded name
 * Returns 1 if successful, 0 if not available or -1 on error
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
	size64_t size_entry_value             = 0;
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

	result = libpff_item_values_get_record_entry_by_utf8_name(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_size(
		     record_entry,
		     &size_entry_value,
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
		*entry_value = (size_t) size_entry_value;
	}
	return( result );
}

/* Retrieves the size value of a specific entry for an UTF-16 encoded name
 * Returns 1 if successful, 0 if not available or -1 on error
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
	size64_t size_entry_value             = 0;
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

	result = libpff_item_values_get_record_entry_by_utf16_name(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_size(
		     record_entry,
		     &size_entry_value,
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
		*entry_value = (size_t) size_entry_value;
	}
	return( result );
}

/* Retrieves the floating point value of a specific entry
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * to use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if not available or -1 on error
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
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_floating_point(
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
 * Returns 1 if successful, 0 if not available or -1 on error
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

	result = libpff_item_values_get_record_entry_by_utf8_name(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_floating_point(
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
 * Returns 1 if successful, 0 if not available or -1 on error
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

	result = libpff_item_values_get_record_entry_by_utf16_name(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_floating_point(
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
 * to use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if not available or -1 on error
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
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_utf8_string_size_with_codepage(
		     record_entry,
		     internal_item->ascii_codepage,
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
 * Returns 1 if successful, 0 if not available or -1 on error
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

	result = libpff_item_values_get_record_entry_by_utf8_name(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_utf8_string_size_with_codepage(
		     record_entry,
		     internal_item->ascii_codepage,
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
 * Returns 1 if successful, 0 if not available or -1 on error
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

	result = libpff_item_values_get_record_entry_by_utf16_name(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_utf8_string_size_with_codepage(
		     record_entry,
		     internal_item->ascii_codepage,
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
 * to use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if not available or -1 on error
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
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_utf8_string_with_codepage(
		     record_entry,
		     internal_item->ascii_codepage,
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
 * Returns 1 if successful, 0 if not available or -1 on error
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

	result = libpff_item_values_get_record_entry_by_utf8_name(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_utf8_string_with_codepage(
		     record_entry,
		     internal_item->ascii_codepage,
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
 * Returns 1 if successful, 0 if not available or -1 on error
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

	result = libpff_item_values_get_record_entry_by_utf16_name(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_utf8_string_with_codepage(
		     record_entry,
		     internal_item->ascii_codepage,
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
 * to use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if not available or -1 on error
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
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_utf16_string_size_with_codepage(
		     record_entry,
		     internal_item->ascii_codepage,
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
 * Returns 1 if successful, 0 if not available or -1 on error
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

	result = libpff_item_values_get_record_entry_by_utf8_name(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_utf16_string_size_with_codepage(
		     record_entry,
		     internal_item->ascii_codepage,
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
 * Returns 1 if successful, 0 if not available or -1 on error
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

	result = libpff_item_values_get_record_entry_by_utf16_name(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_utf16_string_size_with_codepage(
		     record_entry,
		     internal_item->ascii_codepage,
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
 * to use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if not available or -1 on error
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
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_utf16_string_with_codepage(
		     record_entry,
		     internal_item->ascii_codepage,
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
 * Returns 1 if successful, 0 if not available or -1 on error
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

	result = libpff_item_values_get_record_entry_by_utf8_name(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_utf16_string_with_codepage(
		     record_entry,
		     internal_item->ascii_codepage,
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
 * Returns 1 if successful, 0 if not available or -1 on error
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

	result = libpff_item_values_get_record_entry_by_utf16_name(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_data_as_utf16_string_with_codepage(
		     record_entry,
		     internal_item->ascii_codepage,
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

/* Retrieves the size of a binary data value of a specific entry
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * to use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_item_get_entry_value_binary_data_size(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     size_t *binary_data_size,
     uint8_t flags,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_binary_data_size";
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
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
	          set_index,
	          entry_type,
	          LIBPFF_VALUE_TYPE_BINARY_DATA,
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
		if( libpff_record_entry_get_data_size(
		     record_entry,
		     binary_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve binary data size.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the size of a binary data value of a specific entry for an UTF-8 encoded name
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_item_get_entry_value_binary_data_size_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     size_t *binary_data_size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_binary_data_size_by_utf8_name";
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

	result = libpff_item_values_get_record_entry_by_utf8_name(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
	          set_index,
	          utf8_entry_name,
	          utf8_entry_name_length,
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

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libpff_record_entry_get_data_size(
		     record_entry,
		     binary_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve binary data size.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the size of a binary data value of a specific entry for an UTF-16 encoded name
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_item_get_entry_value_binary_data_size_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     size_t *binary_data_size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_binary_data_size_by_utf16_name";
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

	result = libpff_item_values_get_record_entry_by_utf16_name(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
	          set_index,
	          utf16_entry_name,
	          utf16_entry_name_length,
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

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libpff_record_entry_get_data_size(
		     record_entry,
		     binary_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve binary data size.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the binary data value of a specific entry
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * to use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_item_get_entry_value_binary_data(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint8_t *binary_data,
     size_t binary_data_size,
     uint8_t flags,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_binary_data";
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
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
	          set_index,
	          entry_type,
	          LIBPFF_VALUE_TYPE_BINARY_DATA,
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
		if( libpff_record_entry_get_data(
		     record_entry,
		     binary_data,
		     binary_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve binary data.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the binary data value of a specific entry for an UTF-8 encoded name
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_item_get_entry_value_binary_data_by_utf8_name(
     libpff_item_t *item,
     int set_index,
     uint8_t *utf8_entry_name,
     size_t utf8_entry_name_length,
     uint8_t *binary_data,
     size_t binary_data_size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_binary_data_by_utf8_name";
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

	result = libpff_item_values_get_record_entry_by_utf8_name(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
	          set_index,
	          utf8_entry_name,
	          utf8_entry_name_length,
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

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libpff_record_entry_get_data(
		     record_entry,
		     binary_data,
		     binary_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve binary data.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the binary data value of a specific entry for an UTF-16 encoded name
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_item_get_entry_value_binary_data_by_utf16_name(
     libpff_item_t *item,
     int set_index,
     uint16_t *utf16_entry_name,
     size_t utf16_entry_name_length,
     uint8_t *binary_data,
     size_t binary_data_size,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_binary_data_by_utf16_name";
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

	result = libpff_item_values_get_record_entry_by_utf16_name(
	          internal_item->item_values,
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
	          set_index,
	          utf16_entry_name,
	          utf16_entry_name_length,
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

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libpff_record_entry_get_data(
		     record_entry,
		     binary_data,
		     binary_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve binary data.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the GUID value of a specific entry
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * to use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_item_get_entry_value_guid(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     uint8_t *guid,
     size_t guid_size,
     uint8_t flags,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_value_guid";
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
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
	          set_index,
	          entry_type,
	          LIBPFF_VALUE_TYPE_GUID,
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
		if( libpff_record_entry_get_data(
		     record_entry,
		     guid,
		     guid_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve binary data.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the multi value of a specific entry
 * Creates a new multi value
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * to use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_item_get_entry_multi_value(
     libpff_item_t *item,
     int set_index,
     uint32_t entry_type,
     libpff_multi_value_t **multi_value,
     uint8_t flags,
     libcerror_error_t **error )
{
	libpff_internal_item_t *internal_item = NULL;
	libpff_record_entry_t *record_entry   = NULL;
	static char *function                 = "libpff_item_get_entry_multi_value";
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
	          internal_item->name_to_id_map_list,
	          internal_item->io_handle,
	          internal_item->file_io_handle,
	          internal_item->offsets_index,
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
		if( libpff_record_entry_get_multi_value(
		     record_entry,
		     multi_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve multi value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the value data size
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_value_data_size(
     libpff_record_entry_t *record_entry,
     size_t *value_data_size,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_value_data_size";

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

	if( value_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data size.",
		 function );

		return( -1 );
	}
	*value_data_size = internal_record_entry->value_data_size;

	return( 1 );
}

/* Copies the value data
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_copy_value_data(
     libpff_record_entry_t *record_entry,
     uint8_t *value_data,
     size_t value_data_size,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_copy_value_data";

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

	if( internal_record_entry->value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record entry - missing value data.",
		 function );

		return( -1 );
	}
	if( value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data.",
		 function );

		return( -1 );
	}
	if( value_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid value data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( value_data_size < internal_record_entry->value_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid value data value too small.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     value_data,
	     internal_record_entry->value_data,
	     internal_record_entry->value_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy value data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the 8-bit boolean value
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_value_boolean(
     libpff_record_entry_t *record_entry,
     uint8_t *value_boolean,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_value_boolean";

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

	if( internal_record_entry->value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record entry - missing value data.",
		 function );

		return( -1 );
	}
	if( value_boolean == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value boolean.",
		 function );

		return( -1 );
	}
	/* The value data size of a boolean value is 1
	 */
	if( internal_record_entry->value_data_size != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value data size.",
		 function );

		return( -1 );
	}
	*value_boolean = internal_record_entry->value_data[ 0 ];

	return( 1 );
}

/* Retrieves the 16-bit value
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_value_16bit(
     libpff_record_entry_t *record_entry,
     uint16_t *value_16bit,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_value_16bit";

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

	if( internal_record_entry->value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record entry - missing value data.",
		 function );

		return( -1 );
	}
	if( value_16bit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value 16-bit.",
		 function );

		return( -1 );
	}
	/* The value data size of a 16-bit value is 2
	 */
	if( internal_record_entry->value_data_size != 2 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value data size.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint16_little_endian(
	 internal_record_entry->value_data,
	 *value_16bit );

	return( 1 );
}

/* Retrieves the 32-bit value
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_value_32bit(
     libpff_record_entry_t *record_entry,
     uint32_t *value_32bit,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_value_32bit";

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

	if( internal_record_entry->value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record entry - missing value data.",
		 function );

		return( -1 );
	}
	if( value_32bit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value 32-bit.",
		 function );

		return( -1 );
	}
	/* The value data size of a 32-bit value is 4
	 */
	if( internal_record_entry->value_data_size != 4 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value data size.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 internal_record_entry->value_data,
	 *value_32bit );

	return( 1 );
}

/* Retrieves the 64-bit value
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_value_64bit(
     libpff_record_entry_t *record_entry,
     uint64_t *value_64bit,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_value_64bit";

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

	if( internal_record_entry->value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record entry - missing value data.",
		 function );

		return( -1 );
	}
	if( value_64bit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value 64-bit.",
		 function );

		return( -1 );
	}
	/* The value data size of a 64-bit value is 8
	 */
	if( internal_record_entry->value_data_size != 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value data size.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint64_little_endian(
	 internal_record_entry->value_data,
	 *value_64bit );

	return( 1 );
}

/* Retrieves the 64-bit filetime value
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_value_filetime(
     libpff_record_entry_t *record_entry,
     uint64_t *value_64bit,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_value_filetime";

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

	if( internal_record_entry->value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record entry - missing value data.",
		 function );

		return( -1 );
	}
	if( value_64bit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value 64-bit.",
		 function );

		return( -1 );
	}
	/* The value data size of a 64-bit value is 8
	 */
	if( internal_record_entry->value_data_size != 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value data size.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint64_little_endian(
	 internal_record_entry->value_data,
	 *value_64bit );

	return( 1 );
}

/* Retrieves the size value
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_value_size(
     libpff_record_entry_t *record_entry,
     size_t *value_size,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_value_size";

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

	if( internal_record_entry->value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record entry - missing value data.",
		 function );

		return( -1 );
	}
	if( value_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value size.",
		 function );

		return( -1 );
	}
	/* The value data size of a size value is 4 or 8
	 */
	if( ( internal_record_entry->value_data_size != 4 )
	 && ( internal_record_entry->value_data_size != 8 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value data size.",
		 function );

		return( -1 );
	}
	if( internal_record_entry->value_data_size == 4 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 internal_record_entry->value_data,
		 *value_size );
	}
	else if( internal_record_entry->value_data_size == 8 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 internal_record_entry->value_data,
		 *value_size );
	}
	return( 1 );
}

/* Retrieves the floating point value
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_value_floating_point(
     libpff_record_entry_t *record_entry,
     double *value_floating_point,
     libcerror_error_t **error )
{
	byte_stream_float64_t value_double;
	byte_stream_float32_t value_float;

	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_value_floating_point";

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

	if( internal_record_entry->value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record entry - missing value data.",
		 function );

		return( -1 );
	}
	if( value_floating_point == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value floating point.",
		 function );

		return( -1 );
	}
	/* The value data size of a floating point value is 4 or 8
	 */
	if( ( internal_record_entry->value_data_size != 4 )
	 && ( internal_record_entry->value_data_size != 8 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value data size.",
		 function );

		return( -1 );
	}
	if( internal_record_entry->value_data_size == 4 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 internal_record_entry->value_data,
		 value_float.integer );

		*value_floating_point = (double) value_float.floating_point;
	}
	else if( internal_record_entry->value_data_size == 8 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 internal_record_entry->value_data,
		 value_double.integer );

		*value_floating_point = value_double.floating_point;
	}
	return( 1 );
}

/* Retrieves the UTF-8 string size
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_value_utf8_string_size(
     libpff_record_entry_t *record_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_value_utf8_string_size";

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

	if( libpff_mapi_value_get_data_as_utf8_string_size(
	     internal_record_entry->identifier.value_type,
	     internal_record_entry->value_data,
	     internal_record_entry->value_data_size,
	     internal_record_entry->ascii_codepage,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine size of value data as UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 string value
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_value_utf8_string(
     libpff_record_entry_t *record_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_value_utf8_string";

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

	if( libpff_mapi_value_get_data_as_utf8_string(
	     internal_record_entry->identifier.value_type,
	     internal_record_entry->value_data,
	     internal_record_entry->value_data_size,
	     internal_record_entry->ascii_codepage,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data as UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 string size
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_value_utf16_string_size(
     libpff_record_entry_t *record_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_value_utf16_string_size";

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

	if( libpff_mapi_value_get_data_as_utf16_string_size(
	     internal_record_entry->identifier.value_type,
	     internal_record_entry->value_data,
	     internal_record_entry->value_data_size,
	     internal_record_entry->ascii_codepage,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine size of value data as UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 string value
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_value_utf16_string(
     libpff_record_entry_t *record_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_value_utf16_string";

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

	if( libpff_mapi_value_get_data_as_utf16_string(
	     internal_record_entry->identifier.value_type,
	     internal_record_entry->value_data,
	     internal_record_entry->value_data_size,
	     internal_record_entry->ascii_codepage,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data as UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

