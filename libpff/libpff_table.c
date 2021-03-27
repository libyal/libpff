/*
 * Table functions
 *
 * Copyright (C) 2008-2021, Joachim Metz <joachim.metz@gmail.com>
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
#include <system_string.h>
#include <types.h>

#include "libpff_column_definition.h"
#include "libpff_data_array.h"
#include "libpff_data_block.h"
#include "libpff_debug.h"
#include "libpff_definitions.h"
#include "libpff_index.h"
#include "libpff_io_handle.h"
#include "libpff_io_handle2.h"
#include "libpff_libbfio.h"
#include "libpff_libcdata.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_libfguid.h"
#include "libpff_libfmapi.h"
#include "libpff_libuna.h"
#include "libpff_local_descriptor_value.h"
#include "libpff_local_descriptors_tree.h"
#include "libpff_mapi.h"
#include "libpff_name_to_id_map.h"
#include "libpff_record_entry.h"
#include "libpff_record_set.h"
#include "libpff_reference_descriptor.h"
#include "libpff_table.h"
#include "libpff_table_block_index.h"
#include "libpff_table_index_value.h"
#include "libpff_types.h"
#include "libpff_unused.h"

#include "pff_table.h"

/* Creates a table
 * Make sure the value table is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_table_initialize(
     libpff_table_t **table,
     uint32_t descriptor_identifier,
     uint64_t data_identifier,
     uint64_t local_descriptors_identifier,
     uint8_t recovered,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_initialize";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( *table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid table value already set.",
		 function );

		return( -1 );
	}
	*table = memory_allocate_structure(
	          libpff_table_t );

	if( *table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create table.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *table,
	     0,
	     sizeof( libpff_table_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear table.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *table )->index_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create index array.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *table )->record_sets_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create record sets array.",
		 function );

		goto on_error;
	}
	( *table )->descriptor_identifier        = descriptor_identifier;
	( *table )->data_identifier              = data_identifier;
	( *table )->local_descriptors_identifier = local_descriptors_identifier;
	( *table )->recovered                    = recovered;

	return( 1 );

on_error:
	if( *table != NULL )
	{
		memory_free(
		 *table );

		*table = NULL;
	}
	return( -1 );
}

/* Frees a table
 * Returns 1 if successful or -1 on error
 */
int libpff_table_free(
     libpff_table_t **table,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_free";
	int result            = 1;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( *table != NULL )
	{
		if( ( *table )->descriptor_data_list != NULL )
		{
			if( libfdata_list_free(
			     &( ( *table )->descriptor_data_list ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free descriptor data list.",
				 function );

				result = -1;
			}
		}
		if( ( *table )->descriptor_data_cache != NULL )
		{
			if( libfcache_cache_free(
			     &( ( *table )->descriptor_data_cache ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free descriptor data cache.",
				 function );

				result = -1;
			}
		}
		if( ( *table )->local_descriptors_tree != NULL )
		{
			if( libfdata_tree_free(
			     &( ( *table )->local_descriptors_tree ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free local descriptors tree.",
				 function );

				result = -1;
			}
		}
		if( ( *table )->local_descriptors_cache != NULL )
		{
			if( libfcache_cache_free(
			     &( ( *table )->local_descriptors_cache ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free local descriptors cache.",
				 function );

				result = -1;
			}
		}
		if( ( *table )->values_array_data_list != NULL )
		{
			if( libfdata_list_free(
			     &( ( *table )->values_array_data_list ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free values array data list.",
				 function );

				result = -1;
			}
		}
		if( ( *table )->values_array_data_cache != NULL )
		{
			if( libfcache_cache_free(
			     &( ( *table )->values_array_data_cache ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free values array data cache.",
				 function );

				result = -1;
			}
		}
		if( libcdata_array_free(
		     &( ( *table )->index_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_table_block_index_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free index array.",
			 function );

			result = -1;
		}
		if( libcdata_array_free(
		     &( ( *table )->record_sets_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_internal_record_set_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free index array.",
			 function );

			result = -1;
		}
		memory_free(
		 *table );

		*table = NULL;
	}
	return( result );
}

/* Clones the existing table
 * Returns 1 if successful or -1 on error
 */
int libpff_table_clone(
     libpff_table_t **destination_table,
     libpff_table_t *source_table,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_clone";

	if( destination_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination table.",
		 function );

		return( -1 );
	}
	if( *destination_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination table value already set.",
		 function );

		return( -1 );
	}
	if( source_table == NULL )
	{
		*destination_table = NULL;

		return( 1 );
	}
	*destination_table = memory_allocate_structure(
	                      libpff_table_t );

	if( *destination_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination table.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *destination_table,
	     0,
	     sizeof( libpff_table_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear destination table.",
		 function );

		memory_free(
		 *destination_table );

		*destination_table = NULL;

		return( -1 );
	}
/* TODO clone index ? */
	if( libcdata_array_clone(
	     &( ( *destination_table )->record_sets_array ),
	     source_table->record_sets_array,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_internal_record_set_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libpff_record_set_clone,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to clone record sets array.",
		 function );

		goto on_error;
	}
	( *destination_table )->descriptor_identifier        = source_table->descriptor_identifier;
	( *destination_table )->data_identifier              = source_table->data_identifier;
	( *destination_table )->local_descriptors_identifier = source_table->local_descriptors_identifier;
	( *destination_table )->recovered                    = source_table->recovered;

/* TODO is this necessary or should it be re-read on demand ? */
	if( source_table->local_descriptors_tree != NULL )
	{
		if( libfdata_tree_clone(
		     &( ( *destination_table )->local_descriptors_tree ),
		     source_table->local_descriptors_tree,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination local descriptors tree.",
			 function );

			goto on_error;
		}
		if( libfcache_cache_clone(
		     &( ( *destination_table )->local_descriptors_cache ),
		     source_table->local_descriptors_cache,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination local descriptors cache.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( *destination_table != NULL )
	{
		libpff_table_free(
		 destination_table,
		 NULL );
	}
	return( -1 );
}

/* Resizes the record entries
 * Returns 1 if successful or -1 on error
 */
int libpff_table_resize_record_entries(
     libpff_table_t *table,
     int number_of_sets,
     int number_of_entries,
     int ascii_codepage,
     libcerror_error_t **error )
{
	libpff_record_set_t *record_set = NULL;
	static char *function           = "libpff_table_resize_record_entries";
	int last_number_of_sets         = 0;
	int last_number_of_entries      = 0;
	int set_index                   = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( number_of_sets < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid number of sets value less than zero.",
		 function );

		return( -1 );
	}
	if( number_of_entries < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid number of entries value less than zero.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     table->record_sets_array,
	     &last_number_of_sets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sets.",
		 function );

		return( -1 );
	}
	if( last_number_of_sets > 0 )
	{
		if( libcdata_array_get_entry_by_index(
		     table->record_sets_array,
		     0,
		     (intptr_t **) &record_set,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record set: 0.",
			 function );

			return( -1 );
		}
		if( libpff_record_set_get_number_of_entries(
		     record_set,
		     &last_number_of_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of entries of set: 0.",
			 function );

			return( -1 );
		}
		record_set = NULL;
	}
	if( libcdata_array_resize(
	     table->record_sets_array,
	     number_of_sets,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_internal_record_set_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize record sets array.",
		 function );

		goto on_error;
	}
	if( number_of_sets > last_number_of_sets )
	{
		for( set_index = last_number_of_sets;
		     set_index < number_of_sets;
		     set_index++ )
		{
			if( libpff_record_set_initialize(
			     &record_set,
			     last_number_of_entries,
			     ascii_codepage,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create record set: %d.",
				 function,
				 set_index );

				goto on_error;
			}
			if( libcdata_array_set_entry_by_index(
			     table->record_sets_array,
			     set_index,
			     (intptr_t *) record_set,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set record set: %d.",
				 function,
				 set_index );

				goto on_error;
			}
			record_set = NULL;
		}
		last_number_of_sets = number_of_sets;
	}
	if( last_number_of_sets > 0 )
	{
		for( set_index = 0;
		     set_index < last_number_of_sets;
		     set_index++ )
		{
			if( libcdata_array_get_entry_by_index(
			     table->record_sets_array,
			     set_index,
			     (intptr_t **) &record_set,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to retrieve record set: %d.",
				 function,
				 set_index );

				record_set = NULL;

				goto on_error;
			}
			if( libpff_record_set_resize(
			     record_set,
			     number_of_entries,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
				 "%s: unable to resize record set: %d.",
				 function,
				 set_index );

				record_set = NULL;

				goto on_error;
			}
		}
		last_number_of_entries = number_of_entries;
	}
	return( 1 );

on_error:
	if( record_set != NULL )
	{
		libpff_internal_record_set_free(
		 (libpff_internal_record_set_t **) &record_set,
		 NULL );
	}
	if( last_number_of_entries != number_of_entries )
	{
		while( set_index >= last_number_of_sets )
		{
			libcdata_array_get_entry_by_index(
			 table->record_sets_array,
			 set_index,
			 (intptr_t **) &record_set,
			 NULL );

			libpff_record_set_resize(
			 record_set,
			 last_number_of_entries,
			 NULL );

			set_index--;
		}
	}
	if( last_number_of_sets != number_of_sets )
	{
		libcdata_array_resize(
		 table->record_sets_array,
                 last_number_of_sets,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_internal_record_set_free,
		 NULL );
	}
	return( -1 );
}

/* Expands the record entries
 * Returns 1 if successful or -1 on error
 */
int libpff_table_expand_record_entries(
     libpff_table_t *table,
     int number_of_sets,
     int number_of_entries,
     int ascii_codepage,
     libcerror_error_t **error )
{
	libpff_record_set_t *record_set = NULL;
	static char *function           = "libpff_table_expand_record_entries";
	int last_number_of_sets         = 0;
	int last_number_of_entries      = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( number_of_sets < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid number of sets value less than zero.",
		 function );

		return( -1 );
	}
	if( number_of_entries < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid number of entries value less than zero.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     table->record_sets_array,
	     &last_number_of_sets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of record sets array entries.",
		 function );

		return( -1 );
	}
	if( last_number_of_sets > 0 )
	{
		if( libcdata_array_get_entry_by_index(
		     table->record_sets_array,
		     0,
		     (intptr_t **) &record_set,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record set: 0.",
			 function );

			return( -1 );
		}
		if( libpff_record_set_get_number_of_entries(
		     record_set,
		     &last_number_of_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of entries of set: 0.",
			 function );

			return( -1 );
		}
	}
	if( number_of_sets > ( (int) INT_MAX - last_number_of_sets ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: number of sets value out of bounds.",
		 function );

		return( -1 );
	}
	if( number_of_entries > ( (int) INT_MAX - last_number_of_entries ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: number of entries value out of bounds.",
		 function );

		return( -1 );
	}
	if( libpff_table_resize_record_entries(
	     table,
	     last_number_of_sets + number_of_sets,
	     last_number_of_entries + number_of_entries,
	     ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize record entries.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the local descriptor value for the specific identifier
 * Returns 1 if successful, 0 if no value was found or -1 on error
 */
int libpff_table_get_local_descriptors_value_by_identifier(
     libpff_table_t *table,
     libbfio_handle_t *file_io_handle,
     uint32_t descriptor_identifier,
     libpff_local_descriptor_value_t **local_descriptor_value,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_get_local_descriptors_value_by_identifier";
	int result            = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( table->local_descriptors_tree != NULL )
	{
		result = libpff_local_descriptors_tree_get_value_by_identifier(
			  table->local_descriptors_tree,
			  file_io_handle,
			  table->local_descriptors_cache,
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
	}
	return( result );
}

/* Retrieves the table index value for a specific reference
 * Returns 1 if successful or -1 on error
 */
int libpff_table_get_index_value_by_reference(
     libpff_table_t *table,
     uint32_t table_index_reference,
     libpff_io_handle_t *io_handle,
     libpff_table_index_value_t **table_index_value,
     libcerror_error_t **error )
{
	libpff_table_block_index_t *table_block_index = NULL;
	static char *function                         = "libpff_table_get_index_value_by_reference";
	uint16_t table_index_array_reference          = 0;
	uint16_t table_index_value_reference          = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( ( io_handle->file_type != LIBPFF_FILE_TYPE_32BIT )
	 && ( io_handle->file_type != LIBPFF_FILE_TYPE_64BIT )
	 && ( io_handle->file_type != LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file type.",
		 function );

		return( -1 );
	}
	if( table_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table index value.",
		 function );

		return( -1 );
	}
	if( ( table_index_reference & 0x0000001fUL ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported table index reference: 0x%08" PRIx32 " (0x%08" PRIx32 ").",
		 function,
		 table_index_reference & 0x0000001fUL,
		 table_index_reference );

		return( -1 );
	}
	/* Determine the index array reference
	 */
	if( ( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
	 || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT ) )
	{
		table_index_array_reference = (uint16_t) ( table_index_reference >> 16 );
	}
	else
	{
		table_index_array_reference = (uint16_t) ( table_index_reference >> 19 );
	}
	if( libcdata_array_get_entry_by_index(
	     table->index_array,
	     (int) table_index_array_reference,
	     (intptr_t **) &table_block_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve table index array entry: %" PRIu16 ".",
		 function,
		 table_index_array_reference );

		return( -1 );
	}
	if( ( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
	 || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT ) )
	{
		table_index_value_reference = (uint16_t) ( ( table_index_reference & 0x0000ffe0 ) >> 5 ) - 1;
	}
	else
	{
		table_index_value_reference = (uint16_t) ( ( table_index_reference & 0x0007ffe0 ) >> 5 ) - 1;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: retrieving table index array entry: %" PRIu16 " value: %" PRIu16 ".\n",
		 function,
		 table_index_array_reference,
		 table_index_value_reference );
	}
#endif
	if( libpff_table_block_index_get_value_by_index(
	     table_block_index,
	     table_index_value_reference,
	     table_index_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve table block index value: %" PRIu16 ".",
		 function,
                 table_index_value_reference );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the table value data for a specific index value
 * Returns 1 if successful or -1 on error
 */
int libpff_table_get_value_data_by_index_value(
     libpff_table_t *table,
     libpff_table_index_value_t *table_index_value,
     libbfio_handle_t *file_io_handle,
     uint8_t **value_data,
     size_t *value_data_size,
     libcerror_error_t **error )
{
	libpff_data_block_t *data_block = NULL;
	static char *function           = "libpff_table_get_value_data_by_index_value";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( table_index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
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
	/* Retrieve the corresponding data block
	 */
	if( libfdata_list_get_element_value_by_index(
	     table->descriptor_data_list,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) table->descriptor_data_cache,
	     (int) table_index_value->array_entry,
	     (intptr_t **) &data_block,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data block: %" PRIu32 ".",
		 function,
		 table_index_value->array_entry );

		return( -1 );
	}
	if( data_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing data block: %" PRIu32 ".",
		 function,
		 table_index_value->array_entry );

		return( -1 );
	}
	if( data_block->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data block: %" PRIu32 " - missing data.",
		 function,
		 table_index_value->array_entry );

		return( -1 );
	}
	if( (size_t) table_index_value->offset >= data_block->uncompressed_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: table value offset exceeds data block size.",
		 function );

		return( -1 );
	}
	if( ( (size_t) table_index_value->offset + (size_t) table_index_value->size ) >= data_block->uncompressed_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: table value size exceeds data block size.",
		 function );

		return( -1 );
	}
	*value_data      = &( data_block->data[ table_index_value->offset ] );
	*value_data_size = (size_t) table_index_value->size;

	return( 1 );
}

/* Retrieves the value data for a specific reference
 * Returns 1 if successful or -1 on error
 */
int libpff_table_get_value_data_by_reference(
     libpff_table_t *table,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t table_index_reference,
     uint8_t **value_data,
     size_t *value_data_size,
     libcerror_error_t **error )
{
	libpff_table_index_value_t *table_index_value = NULL;
	static char *function                         = "libpff_table_get_value_data_by_reference";

	/* Retrieve the index value of the record entries reference
	 */
	if( libpff_table_get_index_value_by_reference(
	     table,
	     table_index_reference,
	     io_handle,
	     &table_index_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve table index value.",
		 function );

		return( -1 );
	}
	if( libpff_table_get_value_data_by_index_value(
	     table,
	     table_index_value,
	     file_io_handle,
	     value_data,
	     value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data by index value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a copy of the value data for a specific reference
 * Returns 1 if successful or -1 on error
 */
int libpff_table_clone_value_data_by_reference(
     libpff_table_t *table,
     uint32_t table_index_reference,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint8_t **value_data,
     size_t *value_data_size,
     libcerror_error_t **error )
{
	uint8_t *table_value_data    = NULL;
	static char *function        = "libpff_table_clone_value_data_by_reference";
	size_t table_value_data_size = 0;

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
	if( libpff_table_get_value_data_by_reference(
	     table,
	     io_handle,
	     file_io_handle,
	     table_index_reference,
	     &table_value_data,
	     &table_value_data_size,
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
	if( ( table_value_data == NULL )
	 || ( table_value_data_size == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing values array data.",
		 function );

		goto on_error;
	}
	if( table_value_data_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid table value data size value exceeds maximum allocation size.",
		 function );

		goto on_error;
	}
	*value_data = (uint8_t *) memory_allocate(
	                           table_value_data_size );

	if( *value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create value data.",
		 function );

		goto on_error;
	}
	*value_data_size = table_value_data_size;

	if( memory_copy(
	     *value_data,
	     table_value_data,
	     table_value_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy value data.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *value_data != NULL )
	{
		memory_free(
		 *value_data );

		*value_data = NULL;
	}
	*value_data_size = 0;

	return( -1 );
}

/* Retrieves the number of record sets
 * Returns 1 if successful or -1 on error
 */
int libpff_table_get_number_of_record_sets(
     libpff_table_t *table,
     int *number_of_record_sets,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_get_number_of_record_sets";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     table->record_sets_array,
	     number_of_record_sets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of record sets array entries.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific record set
 * Returns 1 if successful or -1 on error
 */
int libpff_table_get_record_set_by_index(
     libpff_table_t *table,
     int record_set_index,
     libpff_record_set_t **record_set,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_get_record_set_by_index";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     table->record_sets_array,
	     record_set_index,
	     (intptr_t **) record_set,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record sets array entry: %d.",
		 function,
		 record_set_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of entries
 * Returns 1 if successful or -1 on error
 */
int libpff_table_get_number_of_entries(
     libpff_table_t *table,
     int *number_of_entries,
     libcerror_error_t **error )
{
	libpff_record_set_t *record_set = NULL;
	static char *function           = "libpff_table_get_number_of_entries";
	int number_of_sets              = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     table->record_sets_array,
	     &number_of_sets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of record sets array entries.",
		 function );

		return( -1 );
	}
	if( number_of_sets > 0 )
	{
		if( libcdata_array_get_entry_by_index(
		     table->record_sets_array,
		     0,
		     (intptr_t **) &record_set,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record set: 0.",
			 function );

			return( -1 );
		}
		if( libpff_record_set_get_number_of_entries(
		     record_set,
		     number_of_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of entries of set: 0.",
			 function );

			return( -1 );
		}
	}
	else
	{
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
		*number_of_entries = 0;
	}
	return( 1 );
}

/* Retrieves the entry and value type of a the entry matching the index from a table
 * Returns 1 if successful or -1 on error
 */
int libpff_table_get_entry_type_by_index(
     libpff_table_t *table,
     int set_index,
     int entry_index,
     uint32_t *entry_type,
     uint32_t *value_type,
     libpff_name_to_id_map_entry_t **name_to_id_map_entry,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *record_entry = NULL;
	static char *function                        = "libpff_table_get_entry_type_by_index";
	int number_of_sets                           = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
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
	if( libcdata_array_get_number_of_entries(
	     table->record_sets_array,
	     &number_of_sets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sets.",
		 function );

		return( -1 );
	}
	if( number_of_sets == 0 )
	{
		return( 0 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: retrieving table set: %d entry index: %d\n",
		 function,
		 set_index,
		 entry_index );
	}
#endif
	if( libpff_table_get_record_entry_by_index(
	     table,
	     set_index,
	     entry_index,
	     (libpff_record_entry_t **) &record_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record entry with set index: %d and entry index: %d.",
		 function,
		 set_index,
		 entry_index );

		return( -1 );
	}
	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing record entry with set index: %d and entry index: %d.",
		 function,
		 set_index,
		 entry_index );

		return( -1 );
	}
	if( record_entry->identifier.format != LIBPFF_RECORD_ENTRY_IDENTIFIER_FORMAT_MAPI_PROPERTY )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record entry identifier format: %" PRIu8 ".",
		 function,
		 record_entry->identifier.format );

		return( -1 );
	}
	*entry_type           = record_entry->identifier.entry_type;
	*value_type           = record_entry->identifier.value_type;
	*name_to_id_map_entry = (libpff_name_to_id_map_entry_t *) record_entry->name_to_id_map_entry;

	return( 1 );
}

/* Retrieves a specific record entry from the table.
 * Returns 1 if successful or -1 on error
 */
int libpff_table_get_record_entry_by_index(
     libpff_table_t *table,
     int set_index,
     int entry_index,
     libpff_record_entry_t **record_entry,
     libcerror_error_t **error )
{
	libpff_record_set_t *record_set = NULL;
	static char *function           = "libpff_table_get_record_entry_by_index";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     table->record_sets_array,
	     set_index,
	     (intptr_t **) &record_set,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record set: %d.",
		 function,
		 set_index );

		return( -1 );
	}
	if( libpff_record_set_get_entry_by_index(
	     record_set,
	     entry_index,
	     record_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %d from record set: %d.",
		 function,
		 entry_index,
		 set_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the record entry matching the entry and value type pair from the table.
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
int libpff_table_get_record_entry_by_type(
     libpff_table_t *table,
     int set_index,
     uint32_t entry_type,
     uint32_t value_type,
     libpff_record_entry_t **record_entry,
     uint8_t flags,
     libcerror_error_t **error )
{
	libpff_record_set_t *record_set = NULL;
	static char *function           = "libpff_table_get_record_entry_by_type";
	int number_of_sets              = 0;
	int result                      = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     table->record_sets_array,
	     &number_of_sets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sets.",
		 function );

		return( -1 );
	}
	if( number_of_sets == 0 )
	{
		return( 0 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: retrieving table set: %d entry type: 0x%04" PRIx32 "\n",
		 function,
		 set_index,
		 entry_type );
	}
#endif
	if( libcdata_array_get_entry_by_index(
	     table->record_sets_array,
	     set_index,
	     (intptr_t **) &record_set,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record set: %d.",
		 function,
		 set_index );

		return( -1 );
	}
	result = libpff_record_set_get_entry_by_type(
	          record_set,
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
		 "%s: unable to retrieve entry from record set: %d.",
		 function,
		 set_index );

		return( -1 );
	}
	return( result );
}

/* Retrieves the record entry matching the UTF-8 encoded name from the table.
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE flag is set
 * the value type is ignored and set. The default behavior is a strict
 * matching of the value type. In this case the value type must be filled
 * with the corresponding value type
 *
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_table_get_record_entry_by_utf8_name(
     libpff_table_t *table,
     int set_index,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint32_t value_type,
     libpff_record_entry_t **record_entry,
     uint8_t flags,
     libcerror_error_t **error )
{
	libpff_record_set_t *record_set = NULL;
	static char *function           = "libpff_table_get_record_entry_by_utf8_name";
	int number_of_sets              = 0;
	int result                      = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     table->record_sets_array,
	     &number_of_sets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sets.",
		 function );

		return( -1 );
	}
	if( number_of_sets == 0 )
	{
		return( 0 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
/* TODO add system string support
		libcnotify_printf(
		 "%s: retrieving table set: %d name: %s\n",
		 function,
		 set_index,
		 utf8_string );
*/
	}
#endif
	if( libcdata_array_get_entry_by_index(
	     table->record_sets_array,
	     set_index,
	     (intptr_t **) &record_set,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record set: %d.",
		 function,
		 set_index );

		return( -1 );
	}
	result = libpff_record_set_get_entry_by_utf8_name(
	          record_set,
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
		 "%s: unable to retrieve entry from record set: %d.",
		 function,
		 set_index );

		return( -1 );
	}
	return( result );
}

/* Retrieves the record entry matching the UTF-16 encoded name from the table.
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE flag is set
 * the value type is ignored and set. The default behavior is a strict
 * matching of the value type. In this case the value type must be filled
 * with the corresponding value type
 *
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_table_get_record_entry_by_utf16_name(
     libpff_table_t *table,
     int set_index,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint32_t value_type,
     libpff_record_entry_t **record_entry,
     uint8_t flags,
     libcerror_error_t **error )
{
	libpff_record_set_t *record_set = NULL;
	static char *function           = "libpff_table_get_record_entry_by_utf16_name";
	int number_of_sets              = 0;
	int result                      = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     table->record_sets_array,
	     &number_of_sets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sets.",
		 function );

		return( -1 );
	}
	if( number_of_sets == 0 )
	{
		return( 0 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
/* TODO add system string support
		libcnotify_printf(
		 "%s: retrieving table set: %d name: %s\n",
		 function,
		 set_index,
		 utf16_entry_name );
*/
	}
#endif
	if( libcdata_array_get_entry_by_index(
	     table->record_sets_array,
	     set_index,
	     (intptr_t **) &record_set,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record set: %d.",
		 function,
		 set_index );

		return( -1 );
	}
	result = libpff_record_set_get_entry_by_utf16_name(
	          record_set,
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
		 "%s: unable to retrieve entry from record set: %d.",
		 function,
		 set_index );

		return( -1 );
	}
	return( result );
}

/* Reads a table and its values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read(
     libpff_table_t *table,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     libcdata_list_t *name_to_id_map_list,
     int debug_item_type,
     libcerror_error_t **error )
{
	libpff_data_block_t *data_block               = NULL;
	static char *function                         = "libpff_table_read";
	uint32_t table_value_reference                = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libpff_table_block_index_t *table_block_index = NULL;
	libpff_table_index_value_t *table_index_value = NULL;
	uint8_t *table_value_data                     = NULL;
	size_t table_value_data_size                  = 0;
	uint16_t number_of_table_index_values         = 0;
	uint16_t table_index_value_iterator           = 0;
	int number_of_table_index_array_entries       = 0;
	int table_index_array_iterator                = 0;
#endif

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( table->data_identifier == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table - missing data identifier.",
		 function );

		return( -1 );
	}
	if( table->local_descriptors_tree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid table - local descriptors tree already set.",
		 function );

		return( -1 );
	}
	if( table->local_descriptors_cache != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid table - local descriptors cache already set.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( table->local_descriptors_identifier > 0 )
	{
		if( libpff_local_descriptors_tree_read(
		     &( table->local_descriptors_tree ),
		     io_handle,
		     file_io_handle,
		     offsets_index,
		     table->descriptor_identifier,
		     table->local_descriptors_identifier,
		     table->recovered,
		     table->recovered_local_descriptors_identifier_value_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read local descriptors tree with identifier: %" PRIu64 ".",
			 function,
			 table->local_descriptors_identifier );

			return( -1 );
		}
		if( libfcache_cache_initialize(
		     &( table->local_descriptors_cache ),
		     LIBPFF_MAXIMUM_CACHE_ENTRIES_LOCAL_DESCRIPTORS_VALUES,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create local descriptors cache.",
			 function );

			if( table->local_descriptors_tree != NULL )
			{
				libfdata_tree_free(
				 &( table->local_descriptors_tree ),
				 NULL );
			}
			return( -1 );
		}
	}
	if( libpff_io_handle_read_descriptor_data_list(
	     io_handle,
	     file_io_handle,
	     offsets_index,
	     table->descriptor_identifier,
	     table->data_identifier,
	     table->recovered,
	     table->recovered_data_identifier_value_index,
	     &( table->descriptor_data_list ),
	     &( table->descriptor_data_cache ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read descriptor: %" PRIu32 " data: %" PRIu64 " list.",
		 function,
		 table->descriptor_identifier,
		 table->data_identifier );

		return( -1 );
	}
	/* Retrieve the first table data block
	 */
	if( libfdata_list_get_element_value_by_index(
	     table->descriptor_data_list,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) table->descriptor_data_cache,
	     0,
	     (intptr_t **) &data_block,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data block: 0.",
		 function );

		return( -1 );
	}
	if( data_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing data block: 0.",
		 function );

		return( -1 );
	}
	if( data_block->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data block: 0 - missing data.",
		 function );

		return( -1 );
	}
	if( data_block->uncompressed_data_size < sizeof( pff_table_t ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: descriptor data:\n",
			 function );
			libcnotify_print_data(
			 data_block->data,
			 data_block->uncompressed_data_size,
			 0 );
		}
#endif
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: data block: 0 too small to be a table.",
		 function );

		return( -1 );
	}
	if( libpff_table_read_header_data(
	     table,
	     data_block->data,
	     data_block->uncompressed_data_size,
	     &table_value_reference,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read table header.",
		 function );

		return( -1 );
	}
	if( ( table->type != 0x6c )
	 && ( table->type != 0x7c )
	 && ( table->type != 0x8c )
	 && ( table->type != 0x9c )
	 && ( table->type != 0xa5 )
	 && ( table->type != 0xac )
	 && ( table->type != 0xbc ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported table type: 0x%02" PRIx8 ".",
		 function,
		 table->type );

		return( -1 );
	}
	if( libpff_table_read_index(
	     table,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read table index.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libcdata_array_get_number_of_entries(
		     table->index_array,
		     &number_of_table_index_array_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of table index array entries.",
			 function );

			return( -1 );
		}
		for( table_index_array_iterator = 0;
		     table_index_array_iterator < number_of_table_index_array_entries;
		     table_index_array_iterator++ )
		{
			libcnotify_printf(
			 "%s: table index array entry: %d\n",
			 function,
			 table_index_array_iterator );

			if( libcdata_array_get_entry_by_index(
			     table->index_array,
			     table_index_array_iterator,
			     (intptr_t **) &table_block_index,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve table block index: %d.",
				 function,
				 table_index_array_iterator );

				return( -1 );
			}
			if( libpff_table_block_index_get_number_of_values(
			     table_block_index,
			     &number_of_table_index_values,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve number of table block index values.",
				 function );

				return( -1 );
			}
			for( table_index_value_iterator = 0;
			     table_index_value_iterator < number_of_table_index_values;
			     table_index_value_iterator++ )
			{
				if( libpff_table_block_index_get_value_by_index(
				     table_block_index,
				     table_index_value_iterator,
				     &table_index_value,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve table block index value: %" PRIu16 ".",
					 function,
					 table_index_value_iterator );

					return( -1 );
				}
				if( table_index_value == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing table index value: %" PRIu16 ".",
					 function,
					 table_index_value_iterator );

					return( -1 );
				}
				if( libpff_table_get_value_data_by_index_value(
				     table,
				     table_index_value,
				     file_io_handle,
				     &table_value_data,
				     &table_value_data_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve table value data by index value: %" PRIu16 ".",
					 function,
					 table_index_value_iterator );

					return( -1 );
				}
				libcnotify_printf(
				 "%s: table value: %" PRIu16 " at offset: %" PRIu16 " of size: %" PRIu16 "\n",
				 function,
				 table_index_value_iterator,
				 table_index_value->offset,
				 table_index_value->size );
				libcnotify_print_data(
				 table_value_data,
				 table_value_data_size,
				 0 );
			}
		}
		libcnotify_printf(
		 "\n" );
	}
#endif
	if( libpff_table_read_values(
	     table,
	     table_value_reference,
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
		 "%s: unable to read table values.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

/* Reads table index entries
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_index_entries(
     libpff_table_t *table,
     libpff_data_block_t *data_block,
     libpff_table_block_index_t *table_block_index,
     uint32_t table_array_entry_iterator,
     libcerror_error_t **error )
{
	libpff_table_index_value_t *table_index_value = NULL;
	static char *function                         = "libpff_table_read_index_entries";
	size_t data_offset                            = 0;
	uint32_t table_index_offsets_data_size        = 0;
	uint16_t table_block_value_index              = 0;
	uint16_t table_index_offset                   = 0;
	uint16_t table_index_value_iterator           = 0;
	uint16_t table_number_of_index_offsets        = 0;
	uint16_t table_number_of_unused_index_offsets = 0;
	uint16_t table_value_end_offset               = 0;
	uint16_t table_value_start_offset             = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( data_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data block.",
		 function );

		return( -1 );
	}
	if( data_block->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data block - missing data.",
		 function );

		return( -1 );
	}
	if( data_block->uncompressed_data_size < 4 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data block - uncompressed data size value out of bounds.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint16_little_endian(
	 data_block->data,
	 table_index_offset );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table index offset\t\t\t: %" PRIu16 "\n",
		 function,
		 table_index_offset );
	}
#endif
	if( ( table_index_offset == 0 )
	 || ( (uint32_t) table_index_offset >= ( data_block->uncompressed_data_size - 4 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid table index offset value out of bounds: %zd.",
		 function, table_index_offset );

		goto on_error;
	}
	/* Determine which values are in the table using the index
	 */
        data_offset = table_index_offset;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table index:\n",
		 function );
		libcnotify_print_data(
		 &( data_block->data[ data_offset ] ),
		 4,
		 0 );
	}
#endif
	byte_stream_copy_to_uint16_little_endian(
	 ( (pff_table_index_t *) &( data_block->data[ data_offset ] ) )->number_of_offsets,
	 table_number_of_index_offsets );

	byte_stream_copy_to_uint16_little_endian(
	 ( (pff_table_index_t *) &( data_block->data[ data_offset ] ) )->number_of_unused_offsets,
	 table_number_of_unused_index_offsets );

	data_offset += 4;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table number of index offsets\t\t: %" PRIu16 "\n",
		 function,
		 table_number_of_index_offsets );

		libcnotify_printf(
		 "%s: table number of unused index offsets\t: %" PRIu16 "\n",
		 function,
		 table_number_of_unused_index_offsets );
	}
#endif
	/* Fill table block index
	 * The table number of index items should be considered more of a last item number
	 * The table actually contains 1 additional table index value
	 */
	if( table_number_of_index_offsets > 0 )
	{
		if( ( table_index_offsets_data_size > ( data_block->uncompressed_data_size - 4 ) )
		 || ( (uint32_t) table_index_offset >= ( data_block->uncompressed_data_size - 4 - table_index_offsets_data_size ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid number of index offsets value out of bounds.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "\n" );

			libcnotify_printf(
			 "%s: table index offsets:\n",
			 function );
			libcnotify_print_data(
			 &( data_block->data[ data_offset ] ),
			 (size_t) table_index_offsets_data_size,
			 0 );
		}
#endif
		/* Fill the table index values
		 */
		byte_stream_copy_to_uint16_little_endian(
		 &( data_block->data[ data_offset ] ),
		 table_value_start_offset );

		data_offset += 2;

		for( table_index_value_iterator = 0;
		     table_index_value_iterator < table_number_of_index_offsets;
		     table_index_value_iterator++ )
		{
			byte_stream_copy_to_uint16_little_endian(
			 &( data_block->data[ data_offset ] ),
			 table_value_end_offset );

			data_offset += 2;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: table index value: %03" PRIu16 " offset\t\t: %" PRIu16 " - %" PRIu16 "\n",
				 function,
				 table_index_value_iterator,
				 table_value_start_offset,
				 table_value_end_offset );
			}
#endif
			if( table_value_start_offset > table_value_end_offset )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: table index start offset: %" PRIu16 " exceeds end offset: %" PRIu16 ".",
				 function,
				 table_value_start_offset,
				 table_value_end_offset );

				goto on_error;
			}
			if( libpff_table_index_value_initialize(
			     &table_index_value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create table index value: %" PRIu16 ".",
				 function,
				 table_index_value_iterator );

				goto on_error;
			}
/* TODO add function to set index values ? */
			table_index_value->array_entry = table_array_entry_iterator;
			table_index_value->offset      = table_value_start_offset;
			table_index_value->size        = table_value_end_offset - table_value_start_offset;

			if( libpff_table_block_index_append_value(
			     table_block_index,
			     &table_block_value_index,
			     table_index_value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set table block index value: %" PRIu16 ".",
				 function,
				 table_index_value_iterator );

				goto on_error;
			}
			table_index_value = NULL;

			table_value_start_offset = table_value_end_offset;
		}
		if( table_value_end_offset > table_index_offset )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: last table index value end offset: %" PRIu16 " exceeds table index offset: %" PRIu16 ".",
			 function,
			 table_value_end_offset,
			 table_index_offset );

			goto on_error;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( table_value_end_offset < table_index_offset )
		{
			libcnotify_printf(
			 "%s: last table index value end offset: %" PRIu16 " does not match table index offset: %" PRIu16 "\n",
			 function,
			 table_value_start_offset,
			 table_index_offset );

			libcnotify_print_data(
			 &( data_block->data[ table_value_end_offset ] ),
			 ( table_index_offset - table_value_end_offset ),
			 0 );
		}
		if( data_offset < (size_t) data_block->uncompressed_data_size )
		{
			libcnotify_printf(
			 "\n" );
			libcnotify_printf(
			 "%s: trailing data of size: %" PRIzd "\n",
			 function,
			 data_block->uncompressed_data_size - data_offset );
			libcnotify_print_data(
			 &( data_block->data[ data_offset ] ),
			 data_block->uncompressed_data_size - data_offset,
			 0 );
		}
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );

on_error:
	if( table_index_value != NULL )
	{
		libpff_table_index_value_free(
		 &table_index_value,
		 NULL );
	}
	return( -1 );
}

/* Reads the table index
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_index(
     libpff_table_t *table,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libpff_data_block_t *data_block               = NULL;
	libpff_table_block_index_t *table_block_index = NULL;
	static char *function                         = "libpff_table_read_index";
	int number_of_table_array_entries             = 0;
	int table_array_entry_iterator                = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	size_t table_data_offset                      = 0;
#endif

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( libfdata_list_get_number_of_elements(
	     table->descriptor_data_list,
	     &number_of_table_array_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of table array entries.",
		 function );

		goto on_error;
	}
	if( libcdata_array_resize(
	     table->index_array,
	     number_of_table_array_entries,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_table_block_index_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize table index array.",
		 function );

		goto on_error;
	}
	/* Iterate the table array to fill the table index array
	 */
	for( table_array_entry_iterator = 0;
	     table_array_entry_iterator < number_of_table_array_entries;
	     table_array_entry_iterator++ )
	{
		if( libfdata_list_get_element_value_by_index(
		     table->descriptor_data_list,
		     (intptr_t *) file_io_handle,
		     (libfdata_cache_t *) table->descriptor_data_cache,
		     table_array_entry_iterator,
		     (intptr_t **) &data_block,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data block: %d.",
			 function,
			 table_array_entry_iterator );

			goto on_error;
		}
		if( data_block == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing data block: %d.",
			 function,
			 table_array_entry_iterator );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table data offset\t\t\t\t: %" PRIzd "\n",
			 function,
			 table_data_offset );

			libcnotify_printf(
			 "%s: table data size\t\t\t\t: %" PRIzd "\n",
			 function,
			 data_block->uncompressed_data_size );
		}
#endif
		if( libpff_table_block_index_initialize(
		     &table_block_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create table block index.",
			 function );

			goto on_error;
		}
		if( libpff_table_read_index_entries(
		     table,
		     data_block,
		     table_block_index,
		     (uint32_t) table_array_entry_iterator,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read index entries.",
			 function );

			goto on_error;
		}
		if( libcdata_array_set_entry_by_index(
		     table->index_array,
		     table_array_entry_iterator,
		     (intptr_t *) table_block_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set table index array entry: %d.",
			 function,
			 table_array_entry_iterator );

			goto on_error;
		}
		table_block_index = NULL;

#if defined( HAVE_DEBUG_OUTPUT )
		table_data_offset += (size_t) data_block->uncompressed_data_size;
#endif
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
	if( table_block_index != NULL )
	{
		libpff_table_block_index_free(
		 &table_block_index,
		 NULL );
	}
	return( -1 );
}

/* Reads the record entries
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_record_entries(
     libpff_table_t *table,
     libcdata_array_t *record_entries_references_array,
     uint8_t record_entries_level,
     uint8_t record_entry_identifier_size,
     uint32_t record_entries_reference,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libpff_reference_descriptor_t *reference_descriptor = NULL;
	uint8_t *record_entries_data                        = NULL;
	uint8_t *record_entry_data                          = NULL;
	static char *function                               = "libpff_table_read_record_entries";
	size_t number_of_record_entries                     = 0;
	size_t record_entry_size                            = 0;
	size_t record_entries_data_size                     = 0;
	int record_entry_index                              = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	system_character_t guid_string[ 48 ];

	libfguid_identifier_t *guid                         = NULL;
	uint64_t record_entry_identifier                    = 0;
	int result                                          = 0;
#endif

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( ( record_entry_identifier_size != 2 )
	 && ( record_entry_identifier_size != 4 )
	 && ( record_entry_identifier_size != 8 )
	 && ( record_entry_identifier_size != 16 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record entry identifier size: %" PRIu8 ".",
		 function,
		 record_entry_identifier_size );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading record entries at level: %" PRIu8 " with reference: 0x%08" PRIx32 " (%s)\n",
		 function,
		 record_entries_level,
		 record_entries_reference,
		 libpff_debug_get_node_identifier_type(
		  (uint8_t) ( record_entries_reference & 0x0000001fUL ) ) );

		libcnotify_printf(
		 "\n" );
	}
#endif
	if( record_entries_reference == 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table contains no record entries.\n",
			 function );
		}
#endif
		return( 1 );
	}
	if( ( record_entries_reference & 0x0000001fUL ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record entries reference: 0x%08" PRIx32 " (0x%08" PRIx32 ").",
		 function,
		 record_entries_reference & 0x0000001fUL,
		 record_entries_reference );

		goto on_error;
	}
	if( record_entries_level == 0 )
	{
		if( libpff_reference_descriptor_initialize(
		     &reference_descriptor,
		     record_entries_reference,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create reference descriptor.",
			 function );

			goto on_error;
		}
		if( libcdata_array_append_entry(
		     record_entries_references_array,
		     &record_entry_index,
		     (intptr_t *) reference_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to append reference descriptor to array.",
			 function );

			goto on_error;
		}
		reference_descriptor = NULL;
	}
	else
	{
		if( libpff_table_clone_value_data_by_reference(
		     table,
		     record_entries_reference,
		     io_handle,
		     file_io_handle,
		     &record_entries_data,
		     &record_entries_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value data by reference.",
			 function );

			goto on_error;
		}
		if( ( record_entries_data == NULL )
		 || ( record_entries_data_size == 0 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing table value data.",
			 function );

			goto on_error;
		}
		record_entry_data = record_entries_data;
		record_entry_size = record_entry_identifier_size + 4;

		if( ( record_entries_data_size % record_entry_size ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported record entries size.",
			 function );

			goto on_error;
		}
		number_of_record_entries = record_entries_data_size / record_entry_size;

		for( record_entry_index = 0;
		     (size_t) record_entry_index < number_of_record_entries;
		     record_entry_index++ )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( ( record_entry_identifier_size == 2 )
				 || ( record_entry_identifier_size == 4 )
				 || ( record_entry_identifier_size == 8 ) )
				{
					if( record_entry_identifier_size == 2 )
					{
						byte_stream_copy_to_uint16_little_endian(
						 record_entry_data,
						 record_entry_identifier );
					}
					else if( record_entry_identifier_size == 4 )
					{
						byte_stream_copy_to_uint32_little_endian(
						 record_entry_data,
						 record_entry_identifier );
					}
					else if( record_entry_identifier_size == 8 )
					{
						byte_stream_copy_to_uint64_little_endian(
						 record_entry_data,
						 record_entry_identifier );
					}
					libcnotify_printf(
					 "%s: record entry: %03d at level: %" PRIu8 " identifier\t\t\t: 0x%08" PRIx64 "\n",
					 function,
					 record_entry_index,
					 record_entries_level,
					 record_entry_identifier );
				}
				else if( record_entry_identifier_size == 16 )
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
					if( libfguid_identifier_copy_from_byte_stream(
					     guid,
					     record_entry_data,
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
					libcnotify_printf(
					 "%s: record entry: %03d at level: %" PRIu8 " identifier\t\t\t: %" PRIs_SYSTEM "s\n",
					 function,
					 record_entry_index,
					 record_entries_level,
					 guid_string );
				}
			}
#endif
/* TODO use the record entry identifier to validate sub level record entries */
			record_entry_data += record_entry_identifier_size;

			byte_stream_copy_to_uint32_little_endian(
			 record_entry_data,
			 record_entries_reference );

			record_entry_data += 4;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: record entry: %03d at level: %" PRIu8 " reference\t\t\t: 0x%08" PRIx32 " (%s)\n",
				 function,
				 record_entry_index,
				 record_entries_level,
				 record_entries_reference,
				 libpff_debug_get_node_identifier_type(
				  (uint8_t) ( record_entries_reference & 0x0000001fUL ) ) );

				libcnotify_printf(
				 "\n" );
			}
#endif
			if( libpff_table_read_record_entries(
			     table,
			     record_entries_references_array,
			     record_entries_level - 1,
			     record_entry_identifier_size,
			     record_entries_reference,
			     io_handle,
			     file_io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read record entries.",
				 function );

				goto on_error;
			}
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "\n" );
		}
#endif
		memory_free(
		 record_entries_data );
	}
	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( guid != NULL )
	{
		libfguid_identifier_free(
		 &guid,
		 NULL );
	}
#endif
	if( record_entries_data != NULL )
	{
		memory_free(
		 record_entries_data );
	}
	if( reference_descriptor != NULL )
	{
		libpff_reference_descriptor_free(
		 &reference_descriptor,
		 NULL );
	}
	if( record_entries_references_array != NULL )
	{
		libcdata_array_empty(
		 record_entries_references_array,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_reference_descriptor_free,
		 NULL );
	}
	return( -1 );
}

/* Reads the table values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_values(
     libpff_table_t *table,
     uint32_t table_value_reference,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     libcdata_list_t *name_to_id_map_list,
     int debug_item_type,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_read_values";
	int result            = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	switch( table->type )
	{
		case 0x6c:
			result = libpff_table_read_6c_values(
				  table,
				  table_value_reference,
				  io_handle,
				  file_io_handle,
				  error );
			break;

		case 0x7c:
			result = libpff_table_read_7c_values(
				  table,
				  table_value_reference,
				  io_handle,
				  file_io_handle,
				  offsets_index,
				  name_to_id_map_list,
				  error );
			break;

		case 0x8c:
			result = libpff_table_read_8c_values(
				  table,
				  table_value_reference,
				  io_handle,
				  file_io_handle,
				  error );
			break;

		case 0x9c:
			result = libpff_table_read_9c_values(
				  table,
				  table_value_reference,
				  io_handle,
				  file_io_handle,
				  error );
			break;

		case 0xa5:
			result = libpff_table_read_a5_values(
				  table,
				  table_value_reference,
				  io_handle,
				  file_io_handle,
				  error );
			break;

		case 0xac:
			result = libpff_table_read_ac_values(
				  table,
				  table_value_reference,
				  io_handle,
				  file_io_handle,
				  offsets_index,
				  name_to_id_map_list,
				  error );
			break;

		case 0xbc:
			result = libpff_table_read_bc_values(
				  table,
				  table_value_reference,
				  io_handle,
				  file_io_handle,
				  offsets_index,
				  name_to_id_map_list,
				  debug_item_type,
				  error );
			break;
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read table values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the 6c table values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_6c_values(
     libpff_table_t *table,
     uint32_t table_header_reference,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libcdata_array_t *record_entries_references_array = NULL;
	uint8_t *table_header_data                        = NULL;
	static char *function                             = "libpff_table_read_6c_values";
	size_t table_header_data_size                     = 0;
	uint32_t b5_table_header_reference                = 0;
	uint32_t record_entries_reference                 = 0;
	uint32_t values_array_reference                   = 0;
	uint8_t record_entry_identifier_size              = 0;
	uint8_t record_entry_value_size                   = 0;
	uint8_t record_entries_level                      = 0;

	if( ( table_header_reference & 0x0000001fUL ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported table header reference: 0x%08" PRIx32 " (0x%08" PRIx32 ").",
		 function,
		 table_header_reference & 0x0000001fUL,
		 table_header_reference );

		return( -1 );
	}
	if( libpff_table_get_value_data_by_reference(
	     table,
	     io_handle,
	     file_io_handle,
	     table_header_reference,
	     &table_header_data,
	     &table_header_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve table header data.",
		 function );

		return( -1 );
	}
	if( libpff_table_read_6c_header_data(
	     table,
	     table_header_data,
	     table_header_data_size,
	     &b5_table_header_reference,
	     &values_array_reference,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read 6c table header.",
		 function );

		goto on_error;
	}
	if( libpff_table_read_b5_header(
	     table,
	     io_handle,
	     file_io_handle,
	     b5_table_header_reference,
	     &record_entry_identifier_size,
	     &record_entry_value_size,
	     &record_entries_level,
	     &record_entries_reference,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read b5 table header.",
		 function );

		goto on_error;
	}
	if( ( record_entry_identifier_size != 16 )
	 || ( record_entry_value_size != 2 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record entry identifier size: %" PRIu8 " and record entry value size: %" PRIu8 ".",
		 function,
		 record_entry_identifier_size,
		 record_entry_value_size );

		goto on_error;
	}
	/* Check if the table contains any entries
	 */
	if( ( record_entries_reference == 0 )
	 && ( values_array_reference == 0 ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table contains no entries.\n",
			 function );
		}
#endif
		goto on_error;
	}
	if( record_entries_reference == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: table contains value array but no record entries.",
		 function );

		goto on_error;
	}
	if( values_array_reference == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: table contains record entries but no value array.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &record_entries_references_array,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create record entries references array.",
		 function );

		goto on_error;
	}
	if( libpff_table_read_record_entries(
	     table,
	     record_entries_references_array,
	     record_entries_level,
	     record_entry_identifier_size,
	     record_entries_reference,
	     io_handle,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read record entries.",
		 function );

		goto on_error;
	}
	if( libpff_table_read_6c_record_entries(
	     table,
	     record_entries_references_array,
	     values_array_reference,
	     io_handle,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read 6c table record entries.",
		 function );

		goto on_error;
	}
	if( libcdata_array_free(
	     &record_entries_references_array,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_reference_descriptor_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free record entries references array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( record_entries_references_array != NULL )
	{
		libcdata_array_free(
		 &record_entries_references_array,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_reference_descriptor_free,
		 NULL );
	}
	return( -1 );
}

/* Reads the 7c table values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_7c_values(
     libpff_table_t *table,
     uint32_t table_header_reference,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     libcdata_list_t *name_to_id_map_list,
     libcerror_error_t **error )
{
	libcdata_array_t *column_definitions_array        = NULL;
	libcdata_array_t *record_entries_references_array = NULL;
	uint8_t *column_definitions_data                  = NULL;
	uint8_t *table_header_data                        = NULL;
	static char *function                             = "libpff_table_read_7c_values";
	size_t column_definitions_data_size               = 0;
	size_t table_header_data_size                     = 0;
	uint32_t b5_table_header_reference                = 0;
	uint32_t values_array_reference                   = 0;
	uint32_t record_entries_reference                 = 0;
	uint16_t values_array_entry_size                  = 0;
	uint8_t record_entry_identifier_size              = 0;
	uint8_t record_entry_value_size                   = 0;
	uint8_t record_entries_level                      = 0;
	int number_of_column_definitions                  = 0;

	if( ( table_header_reference & 0x0000001fUL ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported table header reference: 0x%08" PRIx32 " (0x%08" PRIx32 ").",
		 function,
		 table_header_reference & 0x0000001fUL,
		 table_header_reference );

		return( -1 );
	}
	if( libpff_table_get_value_data_by_reference(
	     table,
	     io_handle,
	     file_io_handle,
	     table_header_reference,
	     &table_header_data,
	     &table_header_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve table header data.",
		 function );

		return( -1 );
	}
	if( libpff_table_read_7c_header_data(
	     table,
	     table_header_data,
	     table_header_data_size,
	     &b5_table_header_reference,
	     &values_array_reference,
	     &values_array_entry_size,
	     &number_of_column_definitions,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read 7c table header.",
		 function );

		goto on_error;
	}
	table_header_data      += sizeof( pff_table_header_7c_t );
	table_header_data_size -= sizeof( pff_table_header_7c_t );

	/* Read the column definitions in the 7c table header
	 */
	if( ( (size_t) number_of_column_definitions * sizeof( pff_table_column_definition_7c_t ) ) != table_header_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: mismatch in number of the column definitions and data size.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: 7c column definitions data:\n",
		 function );
		libcnotify_print_data(
		 table_header_data,
		 table_header_data_size,
		 0 );
	}
#endif
	/* Copy the column definitions data otherwise the data block can cache out
	 * while processing
	 */
	column_definitions_data_size = table_header_data_size;

	if( ( column_definitions_data_size == 0 )
	 || ( column_definitions_data_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid column definitions data size value out of bounds.",
		 function );

		goto on_error;
	}
	column_definitions_data = (uint8_t *) memory_allocate(
	                                       column_definitions_data_size );

	if( column_definitions_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create column definitions data.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     column_definitions_data,
	     table_header_data,
	     table_header_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy column definitions data.",
		 function );

		goto on_error;
	}
	if( libpff_table_read_b5_header(
	     table,
	     io_handle,
	     file_io_handle,
	     b5_table_header_reference,
	     &record_entry_identifier_size,
	     &record_entry_value_size,
	     &record_entries_level,
	     &record_entries_reference,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read b5 table header.",
		 function );

		goto on_error;
	}
	if( ( record_entry_identifier_size != 4 )
	 || ( ( record_entry_value_size != 2 )
	  &&  ( record_entry_value_size != 4 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record entry identifier size: 0x%02" PRIx8 " and record entry value size: 0x%02" PRIx8 ".",
		 function,
		 record_entry_identifier_size,
		 record_entry_value_size );

		goto on_error;
	}
	/* Create the column definitions array
	 */
	if( libcdata_array_initialize(
	     &column_definitions_array,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create column definitions array.",
		 function );

		goto on_error;
	}
	if( libpff_table_read_7c_column_definitions(
	     table,
	     column_definitions_array,
	     column_definitions_data,
	     column_definitions_data_size,
	     number_of_column_definitions,
	     file_io_handle,
	     name_to_id_map_list,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read 7c table column definitions.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &record_entries_references_array,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create record entries references array.",
		 function );

		goto on_error;
	}
	if( libpff_table_read_record_entries(
	     table,
	     record_entries_references_array,
	     record_entries_level,
	     record_entry_identifier_size,
	     record_entries_reference,
	     io_handle,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read record entries.",
		 function );

		goto on_error;
	}
	if( number_of_column_definitions > 0 )
	{
		if( libpff_table_read_values_array(
		     table,
		     record_entries_references_array,
		     values_array_reference,
		     record_entry_identifier_size,
		     record_entry_value_size,
		     values_array_entry_size,
		     column_definitions_array,
		     io_handle,
		     file_io_handle,
		     offsets_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read values array.",
			 function );

			goto on_error;
		}
	}
	if( libcdata_array_free(
	     &record_entries_references_array,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_reference_descriptor_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free record entries references array.",
		 function );

		goto on_error;
	}
	if( libcdata_array_free(
	     &column_definitions_array,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_column_definition_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free the column definitions array.",
		 function );

		goto on_error;
	}
	memory_free(
	 column_definitions_data );

	return( 1 );

on_error:
	if( record_entries_references_array != NULL )
	{
		libcdata_array_free(
		 &record_entries_references_array,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_reference_descriptor_free,
		 NULL );
	}
	if( column_definitions_array != NULL )
	{
		libcdata_array_free(
		 &column_definitions_array,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_column_definition_free,
		 NULL );
	}
	if( column_definitions_data != NULL )
	{
		memory_free(
		 column_definitions_data );
	}
	return( -1 );
}

/* Reads the 8c table values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_8c_values(
     libpff_table_t *table,
     uint32_t b5_table_header_reference,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libcdata_array_t *record_entries_references_array = NULL;
	static char *function                             = "libpff_table_read_8c_values";
	uint32_t record_entries_reference                 = 0;
	uint8_t record_entry_identifier_size              = 0;
	uint8_t record_entry_value_size                   = 0;
	uint8_t record_entries_level                      = 0;

	/* Read the b5 table header
	 */
	if( libpff_table_read_b5_header(
	     table,
	     io_handle,
	     file_io_handle,
	     b5_table_header_reference,
	     &record_entry_identifier_size,
	     &record_entry_value_size,
	     &record_entries_level,
	     &record_entries_reference,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read b5 table header.",
		 function );

		goto on_error;
	}
	if( ( record_entry_identifier_size != 8 )
	 || ( record_entry_value_size != 4 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record entry identifier size: 0x%02" PRIx8 " and record entry value size: 0x%02" PRIx8 ".",
		 function,
		 record_entry_identifier_size,
		 record_entry_value_size );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &record_entries_references_array,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create record entries references array.",
		 function );

		goto on_error;
	}
	if( libpff_table_read_record_entries(
	     table,
	     record_entries_references_array,
	     record_entries_level,
	     record_entry_identifier_size,
	     record_entries_reference,
	     io_handle,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read record entries.",
		 function );

		goto on_error;
	}
	if( libpff_table_read_8c_record_entries(
	     table,
	     record_entries_references_array,
	     io_handle,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read table record entries.",
		 function );

		goto on_error;
	}
	if( libcdata_array_free(
	     &record_entries_references_array,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_reference_descriptor_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free record entries references array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( record_entries_references_array != NULL )
	{
		libcdata_array_free(
		 &record_entries_references_array,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_reference_descriptor_free,
		 NULL );
	}
	return( -1 );
}

/* Reads the 9c table values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_9c_values(
     libpff_table_t *table,
     uint32_t table_header_reference,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libcdata_array_t *record_entries_references_array = NULL;
	uint8_t *table_header_data                        = NULL;
	static char *function                             = "libpff_table_read_9c_values";
	size_t table_header_data_size                     = 0;
	uint32_t b5_table_header_reference                = 0;
	uint32_t record_entries_reference                 = 0;
	uint8_t record_entry_identifier_size              = 0;
	uint8_t record_entry_value_size                   = 0;
	uint8_t record_entries_level                      = 0;

	if( ( table_header_reference & 0x0000001fUL ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported table header reference: 0x%08" PRIx32 " (0x%08" PRIx32 ").",
		 function,
		 table_header_reference & 0x0000001fUL,
		 table_header_reference );

		return( -1 );
	}
	if( libpff_table_get_value_data_by_reference(
	     table,
	     io_handle,
	     file_io_handle,
	     table_header_reference,
	     &table_header_data,
	     &table_header_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve table header data.",
		 function );

		return( -1 );
	}
	if( libpff_table_read_9c_header_data(
	     table,
	     table_header_data,
	     table_header_data_size,
	     &b5_table_header_reference,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read 9c table header.",
		 function );

		goto on_error;
	}
	if( libpff_table_read_b5_header(
	     table,
	     io_handle,
	     file_io_handle,
	     b5_table_header_reference,
	     &record_entry_identifier_size,
	     &record_entry_value_size,
	     &record_entries_level,
	     &record_entries_reference,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read b5 table header.",
		 function );

		goto on_error;
	}
	if( ( record_entry_identifier_size != 16 )
	 || ( record_entry_value_size != 4 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record entry identifier size: 0x%02" PRIx8 " and record entry value size: 0x%02" PRIx8 ".",
		 function,
		 record_entry_identifier_size,
		 record_entry_value_size );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &record_entries_references_array,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create record entries references array.",
		 function );

		goto on_error;
	}
	if( libpff_table_read_record_entries(
	     table,
	     record_entries_references_array,
	     record_entries_level,
	     record_entry_identifier_size,
	     record_entries_reference,
	     io_handle,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read record entries.",
		 function );

		goto on_error;
	}
	if( libpff_table_read_9c_record_entries(
	     table,
	     record_entries_references_array,
	     io_handle,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read 9c table record entries.",
		 function );

		goto on_error;
	}
	if( libcdata_array_free(
	     &record_entries_references_array,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_reference_descriptor_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free record entries references array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( record_entries_references_array != NULL )
	{
		libcdata_array_free(
		 &record_entries_references_array,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_reference_descriptor_free,
		 NULL );
	}
	return( -1 );
}

/* Reads the a5 table values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_a5_values(
     libpff_table_t *table,
     uint32_t table_header_reference,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libpff_table_block_index_t *table_block_index = NULL;
	static char *function                         = "libpff_table_read_a5_values";
	uint16_t number_of_table_index_values         = 0;

	/* The a5 table contains no b5 table header
	 */
	if( table_header_reference != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported table header reference: 0x%08" PRIx32 ".",
		 function,
		 table_header_reference );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     table->index_array,
	     0,
	     (intptr_t **) &table_block_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve table index array entry: 0.",
		 function );

		return( -1 );
	}
	if( table_block_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing table block index: 0.",
		 function );

		return( -1 );
	}
	if( libpff_table_block_index_get_number_of_values(
	     table_block_index,
	     &number_of_table_index_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of table block index values.",
		 function );

		return( -1 );
	}
	if( number_of_table_index_values > 1 )
	{
		if( libpff_table_read_a5_record_entries(
		     table,
		     0x00000020,
		     io_handle,
		     file_io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read a5 table record entries.",
			 function );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	else if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table contains no entries.\n",
		 function );
	}
#endif
	return( 1 );
}

/* Reads the ac table values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_ac_values(
     libpff_table_t *table,
     uint32_t table_header_reference,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     libcdata_list_t *name_to_id_map_list,
     libcerror_error_t **error )
{
	libcdata_array_t *column_definitions_array        = NULL;
	libcdata_array_t *record_entries_references_array = NULL;
	uint8_t *table_header_data                        = NULL;
	static char *function                             = "libpff_table_read_ac_values";
	size_t table_header_data_size                     = 0;
	uint32_t b5_table_header_reference                = 0;
	uint32_t record_entries_reference                 = 0;
	uint32_t column_definitions_reference             = 0;
	uint32_t values_array_reference                   = 0;
	uint16_t values_array_entry_size                  = 0;
	uint8_t record_entry_identifier_size              = 0;
	uint8_t record_entry_value_size                   = 0;
	uint8_t record_entries_level                      = 0;
	int number_of_column_definitions                  = 0;

	if( ( table_header_reference & 0x0000001fUL ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported table header reference: 0x%08" PRIx32 " (0x%08" PRIx32 ").",
		 function,
		 table_header_reference & 0x0000001fUL,
		 table_header_reference );

		return( -1 );
	}
	if( libpff_table_get_value_data_by_reference(
	     table,
	     io_handle,
	     file_io_handle,
	     table_header_reference,
	     &table_header_data,
	     &table_header_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve table header data.",
		 function );

		return( -1 );
	}
	if( libpff_table_read_ac_header_data(
	     table,
	     table_header_data,
	     table_header_data_size,
	     &b5_table_header_reference,
	     &values_array_reference,
	     &column_definitions_reference,
	     &values_array_entry_size,
	     &number_of_column_definitions,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read ac table header.",
		 function );

		goto on_error;
	}
	if( libpff_table_read_b5_header(
	     table,
	     io_handle,
	     file_io_handle,
	     b5_table_header_reference,
	     &record_entry_identifier_size,
	     &record_entry_value_size,
	     &record_entries_level,
	     &record_entries_reference,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read b5 table header.",
		 function );

		goto on_error;
	}
	if( ( record_entry_identifier_size != 4 )
	 || ( record_entry_value_size != 4 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record entry identifier size: 0x%02" PRIx8 " and record entry value size: 0x%02" PRIx8 ".",
		 function,
		 record_entry_identifier_size,
		 record_entry_value_size );

		goto on_error;
	}
	/* Create the column definitions array
	 */
	if( libcdata_array_initialize(
	     &column_definitions_array,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create column definitions array.",
		 function );

		goto on_error;
	}
	if( libpff_table_read_ac_column_definitions(
	     table,
	     column_definitions_array,
	     column_definitions_reference,
	     number_of_column_definitions,
	     io_handle,
	     file_io_handle,
	     offsets_index,
	     name_to_id_map_list,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read ac table column definitions.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &record_entries_references_array,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create record entries reference array.",
		 function );

		goto on_error;
	}
	if( libpff_table_read_record_entries(
	     table,
	     record_entries_references_array,
	     record_entries_level,
	     record_entry_identifier_size,
	     record_entries_reference,
	     io_handle,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read record entries.",
		 function );

		goto on_error;
	}
	if( libcdata_array_get_number_of_entries(
	     table->index_array,
	     &number_of_column_definitions,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of column definitions.",
		 function );

		goto on_error;
	}
	if( number_of_column_definitions > 0 )
	{
		if( libpff_table_read_values_array(
		     table,
		     record_entries_references_array,
		     values_array_reference,
		     record_entry_identifier_size,
		     record_entry_value_size,
		     values_array_entry_size,
		     column_definitions_array,
		     io_handle,
		     file_io_handle,
		     offsets_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read values array.",
			 function );

			goto on_error;
		}
	}
	if( libcdata_array_free(
	     &column_definitions_array,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_column_definition_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free the column definitions array.",
		 function );

		goto on_error;
	}
	if( libcdata_array_free(
	     &record_entries_references_array,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_reference_descriptor_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free record entries reference array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( record_entries_references_array != NULL )
	{
		libcdata_array_free(
		 &record_entries_references_array,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_reference_descriptor_free,
		 NULL );
	}
	if( column_definitions_array != NULL )
	{
		libcdata_array_free(
		 &column_definitions_array,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_column_definition_free,
		 NULL );
	}
	return( -1 );
}

/* Reads the bc table values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_bc_values(
     libpff_table_t *table,
     uint32_t b5_table_header_reference,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     libcdata_list_t *name_to_id_map_list,
     int debug_item_type,
     libcerror_error_t **error )
{
	libcdata_array_t *record_entries_references_array = NULL;
	static char *function                             = "libpff_table_read_bc_values";
	uint32_t record_entries_reference                 = 0;
	uint8_t record_entry_identifier_size              = 0;
	uint8_t record_entry_value_size                   = 0;
	uint8_t record_entries_level                      = 0;

	/* Read the b5 table header
	 */
	if( libpff_table_read_b5_header(
	     table,
	     io_handle,
	     file_io_handle,
	     b5_table_header_reference,
	     &record_entry_identifier_size,
	     &record_entry_value_size,
	     &record_entries_level,
	     &record_entries_reference,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read b5 table header.",
		 function );

		goto on_error;
	}
	if( ( record_entry_identifier_size != 2 )
	 || ( record_entry_value_size != 6 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record entry identifier size: 0x%02" PRIx8 " and record entry value size: 0x%02" PRIx8 ".",
		 function,
		 record_entry_identifier_size,
		 record_entry_value_size );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &record_entries_references_array,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create record entries references array.",
		 function );

		goto on_error;
	}
	if( libpff_table_read_record_entries(
	     table,
	     record_entries_references_array,
	     record_entries_level,
	     record_entry_identifier_size,
	     record_entries_reference,
	     io_handle,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read record entries.",
		 function );

		goto on_error;
	}
	if( libpff_table_read_bc_record_entries(
	     table,
	     record_entries_references_array,
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
		 "%s: unable to read table record entries.",
		 function );

		goto on_error;
	}
	if( libcdata_array_free(
	     &record_entries_references_array,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_reference_descriptor_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free record entries references array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( record_entries_references_array != NULL )
	{
		libcdata_array_free(
		 &record_entries_references_array,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_reference_descriptor_free,
		 NULL );
	}
	return( -1 );
}

/* Reads the table header
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_header_data(
     libpff_table_t *table,
     const uint8_t *data,
     size_t data_size,
     uint32_t *table_value_reference,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_read_header_data";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( table_value_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table value reference.",
		 function );

		return( -1 );
	}
	if( data_size < sizeof( pff_table_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: unsupported table header of size: %" PRIzd ".",
		 function,
		 data_size );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( pff_table_t ),
		 0 );
	}
#endif
	if( ( (pff_table_t *) data )->signature != 0xec )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported table signature: 0x%02" PRIx8 ".",
		 function,
		 ( (pff_table_t *) data )->signature );

		return( -1 );
	}
	table->type = ( (pff_table_t *) data )->type;

	byte_stream_copy_to_uint32_little_endian(
	 ( (pff_table_t *) data )->value_reference,
	 *table_value_reference );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table signature\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (pff_table_t *) data )->signature );

		libcnotify_printf(
		 "%s: table type\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 table->type );

		libcnotify_printf(
		 "%s: table value reference\t\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 *table_value_reference,
		 libpff_debug_get_node_identifier_type(
		  (uint8_t) ( *table_value_reference & 0x0000001fUL ) ) );
	}
#endif
	return( 1 );
}

/* Reads the 6c table header
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_6c_header_data(
     libpff_table_t *table,
     const uint8_t *data,
     size_t data_size,
     uint32_t *b5_table_header_reference,
     uint32_t *values_array_reference,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_read_6c_header_data";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( b5_table_header_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid b5 table header reference.",
		 function );

		return( -1 );
	}
	if( values_array_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value array reference.",
		 function );

		return( -1 );
	}
	/* The 6c table header contains no type indicator
	 * to make sure the it is supported the size is checked
	 */
	if( data_size != 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: unsupported 6c table header of size: %" PRIzd ".",
		 function,
		 data_size );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: 6c table header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 data,
	 *b5_table_header_reference );

	byte_stream_copy_to_uint32_little_endian(
	 &( data[ 4 ] ),
	 *values_array_reference );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: b5 table header reference\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 *b5_table_header_reference,
		 libpff_debug_get_node_identifier_type(
		  (uint8_t) ( *b5_table_header_reference & 0x0000001fUL ) ) );

		libcnotify_printf(
		 "%s: values array reference\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 *values_array_reference,
		 libpff_debug_get_node_identifier_type(
		  (uint8_t) ( *values_array_reference & 0x0000001fUL ) ) );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	return( 1 );
}

/* Reads the 7c table header
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_7c_header_data(
     libpff_table_t *table,
     const uint8_t *data,
     size_t data_size,
     uint32_t *b5_table_header_reference,
     uint32_t *values_array_reference,
     uint16_t *values_array_entry_size,
     int *number_of_column_definitions,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_read_7c_header";

#if defined( HAVE_DEBUG_OUTPUT )
	uint16_t value_16bit  = 0;
#endif

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( b5_table_header_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid b5 table header reference.",
		 function );

		return( -1 );
	}
	if( values_array_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value array reference.",
		 function );

		return( -1 );
	}
	if( values_array_entry_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value array entry size.",
		 function );

		return( -1 );
	}
	if( number_of_column_definitions == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of column definitions.",
		 function );

		return( -1 );
	}
	if( data_size < sizeof( pff_table_header_7c_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: unsupported 7c table header of size: %" PRIzd ".",
		 function,
		 data_size );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: 7c table header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( pff_table_header_7c_t ),
		 0 );
	}
#endif
	if( ( (pff_table_header_7c_t *) data )->type != 0x7c )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported table header type: 0x%02x.",
		 function,
		 ( (pff_table_header_7c_t *) data )->type );

		return( -1 );
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (pff_table_header_7c_t *) data )->values_array_end_offset_cell_existence_block,
	 *values_array_entry_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (pff_table_header_7c_t *) data )->b5_table_header_reference,
	 *b5_table_header_reference );

	byte_stream_copy_to_uint32_little_endian(
	 ( (pff_table_header_7c_t *) data )->values_array_reference,
	 *values_array_reference );

	*number_of_column_definitions = (int) ( (pff_table_header_7c_t *) data )->number_of_column_definitions;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table header type\t\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (pff_table_header_7c_t *) data )->type );

		libcnotify_printf(
		 "%s: number of column definitions\t\t\t: %d\n",
		 function,
		 *number_of_column_definitions );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_header_7c_t *) data )->values_array_end_offset_32bit_values,
		 value_16bit );
		libcnotify_printf(
		 "%s: values array end offset 32-bit values\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_header_7c_t *) data )->values_array_end_offset_16bit_values,
		 value_16bit );
		libcnotify_printf(
		 "%s: values array end offset 16-bit values\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_header_7c_t *) data )->values_array_end_offset_8bit_values,
		 value_16bit );
		libcnotify_printf(
		 "%s: values array end offset 8-bit values\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_header_7c_t *) data )->values_array_end_offset_cell_existence_block,
		 value_16bit );
		libcnotify_printf(
		 "%s: values array end offset cell existence block\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: b5 table header reference\t\t\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 *b5_table_header_reference,
		 libpff_debug_get_node_identifier_type(
		  (uint8_t) ( *b5_table_header_reference & 0x0000001fUL ) ) );

		libcnotify_printf(
		 "%s: values array reference\t\t\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 *values_array_reference,
		 libpff_debug_get_node_identifier_type(
		  (uint8_t) ( *values_array_reference & 0x0000001fUL ) ) );

		libcnotify_printf(
		 "%s: unknown1:\n",
		 function );
		libcnotify_print_data(
		 ( (pff_table_header_7c_t *) data )->unknown1,
		 4,
		 0 );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	return( 1 );
}

/* Reads the 9c table header
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_9c_header_data(
     libpff_table_t *table,
     const uint8_t *data,
     size_t data_size,
     uint32_t *b5_table_header_reference,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_read_9c_header_data";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( b5_table_header_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid b5 table header reference.",
		 function );

		return( -1 );
	}
	/* The 9c table header contains no type indicator
	 * to make sure the it is supported the size is checked
	 */
	if( data_size != 4 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: unsupported 9c table header of size: %" PRIu16 ".",
		 function,
		 data_size );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: 9c table header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 data,
	 *b5_table_header_reference );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: b5 table header reference\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 *b5_table_header_reference,
		 libpff_debug_get_node_identifier_type(
		  (uint8_t) ( *b5_table_header_reference & 0x0000001fUL ) ) );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	return( 1 );
}

/* Reads the ac table header
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_ac_header_data(
     libpff_table_t *table,
     const uint8_t *data,
     size_t data_size,
     uint32_t *b5_table_header_reference,
     uint32_t *values_array_reference,
     uint32_t *column_definitions_reference,
     uint16_t *values_array_entry_size,
     int *number_of_column_definitions,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_read_ac_header_data";

#if defined( HAVE_DEBUG_OUTPUT )
	uint16_t value_16bit  = 0;
#endif

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( b5_table_header_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid b5 table header reference.",
		 function );

		return( -1 );
	}
	if( values_array_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value array reference.",
		 function );

		return( -1 );
	}
	if( column_definitions_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid column definitions reference.",
		 function );

		return( -1 );
	}
	if( values_array_entry_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value array entry size.",
		 function );

		return( -1 );
	}
	if( number_of_column_definitions == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of column definitions.",
		 function );

		return( -1 );
	}
	if( data_size < sizeof( pff_table_header_ac_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: unsupported ac table header of size: %" PRIzd ".",
		 function,
		 data_size );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: ac table header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( pff_table_header_ac_t ),
		 0 );
	}
#endif
	if( ( (pff_table_header_ac_t *) data )->type != 0xac )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported table header type: 0x%02x.",
		 function,
		 ( (pff_table_header_ac_t *) data )->type );

		return( -1 );
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (pff_table_header_ac_t *) data )->values_array_end_offset_cell_existence_block,
	 *values_array_entry_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (pff_table_header_ac_t *) data )->b5_table_header_reference,
	 *b5_table_header_reference );

	byte_stream_copy_to_uint32_little_endian(
	 ( (pff_table_header_ac_t *) data )->values_array_reference,
	 *values_array_reference );

	byte_stream_copy_to_uint16_little_endian(
	 ( (pff_table_header_ac_t *) data )->number_of_column_definitions,
	 *number_of_column_definitions );

	byte_stream_copy_to_uint32_little_endian(
	 ( (pff_table_header_ac_t *) data )->column_definitions_reference,
	 *column_definitions_reference );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table header type\t\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (pff_table_header_ac_t *) data )->type );

		libcnotify_printf(
		 "%s: padding1\t\t\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (pff_table_header_ac_t *) data )->padding1 );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_header_ac_t *) data )->values_array_end_offset_32bit_values,
		 value_16bit );
		libcnotify_printf(
		 "%s: values array end offset 32-bit values\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_header_ac_t *) data )->values_array_end_offset_16bit_values,
		 value_16bit );
		libcnotify_printf(
		 "%s: values array end offset 16-bit values\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_header_ac_t *) data )->values_array_end_offset_8bit_values,
		 value_16bit );
		libcnotify_printf(
		 "%s: values array end offset 8-bit values\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_header_ac_t *) data )->values_array_end_offset_cell_existence_block,
		 value_16bit );
		libcnotify_printf(
		 "%s: values array end offset cell existence block\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: b5 table header reference\t\t\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 *b5_table_header_reference,
		 libpff_debug_get_node_identifier_type(
		  (uint8_t) ( *b5_table_header_reference & 0x0000001fUL ) ) );

		libcnotify_printf(
		 "%s: values array reference\t\t\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 *values_array_reference,
		 libpff_debug_get_node_identifier_type(
		  (uint8_t) ( *values_array_reference & 0x0000001fUL ) ) );

		libcnotify_printf(
		 "%s: padding2:\n",
		 function );
		libcnotify_print_data(
		 ( (pff_table_header_ac_t *) data )->padding2,
		 4,
		 0 );

		libcnotify_printf(
		 "%s: number of column definitions\t\t\t: %d\n",
		 function,
		 *number_of_column_definitions );

		libcnotify_printf(
		 "%s: column definitions reference\t\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 *column_definitions_reference,
		 libpff_debug_get_node_identifier_type(
		  (uint8_t) ( *column_definitions_reference & 0x0000001fUL ) ) );

		libcnotify_printf(
		 "%s: unknown2:\n",
		 function );
		libcnotify_print_data(
		 ( (pff_table_header_ac_t *) data )->unknown2,
		 12,
		 0 );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	return( 1 );
}

/* Reads the b5 table header
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_b5_header(
     libpff_table_t *table,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t table_header_reference,
     uint8_t *record_entry_identifier_size,
     uint8_t *record_entry_value_size,
     uint8_t *record_entries_level,
     uint32_t *record_entries_reference,
     libcerror_error_t **error )
{
	uint8_t *table_header_data    = NULL;
	static char *function         = "libpff_table_read_b5_header";
	size_t table_header_data_size = 0;

	if( record_entry_identifier_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry identifier size.",
		 function );

		return( -1 );
	}
	if( record_entry_value_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry value size.",
		 function );

		return( -1 );
	}
	if( record_entries_level == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entries level.",
		 function );

		return( -1 );
	}
	if( record_entries_reference == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entries reference.",
		 function );

		return( -1 );
	}
	if( ( table_header_reference & 0x0000001fUL ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported table header reference: 0x%08" PRIx32 " (0x%08" PRIx32 ").",
		 function,
		 table_header_reference & 0x0000001fUL,
		 table_header_reference );

		return( -1 );
	}
	if( libpff_table_get_value_data_by_reference(
	     table,
	     io_handle,
	     file_io_handle,
	     table_header_reference,
	     &table_header_data,
	     &table_header_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve table header data.",
		 function );

		return( -1 );
	}
	if( ( table_header_data == NULL )
	 || ( table_header_data_size == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing table header data.",
		 function );

		return( -1 );
	}
	if( ( (pff_table_header_b5_t *) table_header_data )->type != 0xb5 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported table header type: 0x%02x.",
		 function,
		 ( (pff_table_header_b5_t *) table_header_data )->type );

		return( -1 );
	}
	*record_entry_identifier_size = ( (pff_table_header_b5_t *) table_header_data )->record_entry_identifier_size;
	*record_entry_value_size      = ( (pff_table_header_b5_t *) table_header_data )->record_entry_value_size;
	*record_entries_level         = ( (pff_table_header_b5_t *) table_header_data )->record_entries_level;

	byte_stream_copy_to_uint32_little_endian(
	 ( (pff_table_header_b5_t *) table_header_data )->record_entries_reference,
	 *record_entries_reference );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table header type\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (pff_table_header_b5_t *) table_header_data )->type );

		libcnotify_printf(
		 "%s: record entry identifier size\t\t: %" PRIu8 "\n",
		 function,
		 *record_entry_identifier_size );

		libcnotify_printf(
		 "%s: record entry value size\t\t\t: %" PRIu8 "\n",
		 function,
		 *record_entry_value_size );

		libcnotify_printf(
		 "%s: record entries level\t\t\t: %" PRIu8 "\n",
		 function,
		 ( (pff_table_header_b5_t *) table_header_data )->record_entries_level );

		libcnotify_printf(
		 "%s: record entries reference\t\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 *record_entries_reference,
		 libpff_debug_get_node_identifier_type(
		  (uint8_t) ( *record_entries_reference & 0x0000001fUL ) ) );

		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

/* Reads the 6c table record entries and their values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_6c_record_entries(
     libpff_table_t *table,
     libcdata_array_t *record_entries_references_array,
     uint32_t values_array_reference,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *record_entry        = NULL;
	libpff_reference_descriptor_t *reference_descriptor = NULL;
	uint8_t *record_entries_data                        = NULL;
	uint8_t *table_values_array_data                    = NULL;
	static char *function                               = "libpff_table_read_6c_record_entries";
	size_t number_of_record_entries                     = 0;
	size_t record_entries_data_size                     = 0;
	size_t table_values_array_data_size                 = 0;
	uint16_t values_array_number                        = 0;
	int number_of_record_entries_references             = 0;
	int record_entries_reference_index                  = 0;
	int record_entry_index                              = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	system_character_t guid_string[ 48 ];

	libfguid_identifier_t *guid                         = NULL;
	int result                                          = 0;
#endif

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( values_array_reference == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: table contains no value array.",
		 function );

		return( -1 );
	}
	if( ( values_array_reference & 0x0000001fUL ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported values array entries reference: 0x%08" PRIx32 " (0x%08" PRIx32 ").",
		 function,
		 values_array_reference & 0x0000001fUL,
		 values_array_reference );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( libpff_table_clone_value_data_by_reference(
	     table,
	     values_array_reference,
	     io_handle,
	     file_io_handle,
	     &table_values_array_data,
	     &table_values_array_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve values array data.",
		 function );

		goto on_error;
	}
	if( ( table_values_array_data == NULL )
	 || ( table_values_array_data_size == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing values array data.",
		 function );

		goto on_error;
	}
	if( libcdata_array_get_number_of_entries(
	     record_entries_references_array,
	     &number_of_record_entries_references,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of record entries references.",
		 function );

		goto on_error;
	}
	if( number_of_record_entries_references > 0 )
	{
		if( libpff_table_resize_record_entries(
		     table,
		     1,
		     0,
		     io_handle->ascii_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize record entries.",
			 function );

			goto on_error;
		}
		for( record_entries_reference_index = 0;
		     record_entries_reference_index < number_of_record_entries_references;
		     record_entries_reference_index++ )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: record entries reference: %d\n",
				 function,
				 record_entries_reference_index );
			}
#endif
			if( libcdata_array_get_entry_by_index(
			     record_entries_references_array,
			     record_entries_reference_index,
			     (intptr_t **) &reference_descriptor,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve record entries reference: %d.",
				 function,
				 record_entries_reference_index );

				goto on_error;
			}
			if( reference_descriptor == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing reference descriptor.",
				 function );

				goto on_error;
			}
			if( libpff_table_get_value_data_by_reference(
			     table,
			     io_handle,
			     file_io_handle,
			     reference_descriptor->value,
			     &record_entries_data,
			     &record_entries_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve record entries data.",
				 function );

				goto on_error;
			}
			if( ( record_entries_data == NULL )
			 || ( record_entries_data_size == 0 ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing record entries data.",
				 function );

				goto on_error;
			}
			if( ( record_entries_data_size % sizeof( pff_table_record_entry_6c_t ) ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported record entries data size.",
				 function );

				goto on_error;
			}
			number_of_record_entries = record_entries_data_size / sizeof( pff_table_record_entry_6c_t );

			if( number_of_record_entries > (size_t) INT_MAX )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
				 "%s: number of record entries value exceeds maximum.",
				 function );

				goto on_error;
			}
			if( libpff_table_expand_record_entries(
			     table,
			     0,
			     (int) number_of_record_entries,
			     io_handle->ascii_codepage,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
				 "%s: unable to expand record entries.",
				 function );

				goto on_error;
			}
			while( record_entries_data_size > 0 )
			{
				if( libpff_table_get_record_entry_by_index(
				     table,
				     0,
				     record_entry_index,
				     (libpff_record_entry_t **) &record_entry,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve record entry with set index: 0 and entry index: %d.",
					 function,
					 record_entry_index );

					goto on_error;
				}
				if( record_entry == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing record entry with set index: 0 and entry index: %d.",
					 function,
					 record_entry_index );

					return( -1 );
				}
				record_entry->identifier.format = LIBPFF_RECORD_ENTRY_IDENTIFIER_FORMAT_GUID;

				if( memory_copy(
				     record_entry->identifier.guid,
				     ( (pff_table_record_entry_6c_t *) record_entries_data )->record_entry_guid,
				     16 ) == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
					 "%s: unable to copy record entry identifier GUID.",
					 function );

					goto on_error;
				}
				byte_stream_copy_to_uint16_little_endian(
				 ( (pff_table_record_entry_6c_t *) record_entries_data )->values_array_number,
				 values_array_number );

				if( (size_t) ( 16 * values_array_number ) > table_values_array_data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: values array number exceeds table values array data size.",
					 function );

					goto on_error;
				}
				if( libpff_record_entry_set_value_data(
				     (libpff_record_entry_t *) record_entry,
				     &( table_values_array_data[ 16 * values_array_number ] ),
				     16,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set value data in record entry.",
					 function );

					goto on_error;
				}
/* TODO do something with values_array_number ? */

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
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
					if( libfguid_identifier_copy_from_byte_stream(
					     guid,
					     record_entry->identifier.guid,
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
					 "%s: table set: %03d entry: %03d record entry guid\t\t\t: %" PRIs_SYSTEM "\n",
					 function,
					 0,
					 record_entry_index,
					 guid_string );

					if( libfguid_identifier_copy_from_byte_stream(
					     guid,
					     &( table_values_array_data[ 16 * values_array_number ] ),
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
					libcnotify_printf(
					 "%s: table set: %03d entry: %03d record entry value guid\t\t: %" PRIs_SYSTEM "\n",
					 function,
					 0,
					 record_entry_index,
					 guid_string );

					libcnotify_printf(
					 "%s: table set: %03d entry: %03d record entry value identifier\t: 0x%04" PRIx16 "\n",
					 function,
					 0,
					 record_entry_index,
					 values_array_number );

					libcnotify_printf(
					 "\n" );
				}
#endif
				record_entries_data      += sizeof( pff_table_record_entry_6c_t );
				record_entries_data_size -= sizeof( pff_table_record_entry_6c_t );

				record_entry_index++;
			}
		}
	}
	memory_free(
	 table_values_array_data );

	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( guid != NULL )
	{
		libfguid_identifier_free(
		 &guid,
		 NULL );
	}
#endif
	if( table_values_array_data != NULL )
	{
		memory_free(
		 table_values_array_data );
	}
	return( -1 );
}

/* Reads the 7c table column definitions
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_7c_column_definitions(
     libpff_table_t *table,
     libcdata_array_t *column_definitions_array,
     uint8_t *column_definitions_data,
     size_t column_definitions_data_size,
     int number_of_column_definitions,
     libbfio_handle_t *file_io_handle LIBPFF_ATTRIBUTE_UNUSED,
     libcdata_list_t *name_to_id_map_list,
     libcerror_error_t **error )
{
	libpff_column_definition_t *column_definition        = NULL;
	libpff_column_definition_t *lookup_column_definition = NULL;
	static char *function                                = "libpff_table_read_7c_column_definitions";
	uint8_t column_definition_number                     = 0;
	int column_definition_index                          = 0;
	int result                                           = 0;

	LIBPFF_UNREFERENCED_PARAMETER( file_io_handle )

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( column_definitions_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid column definitions array.",
		 function );

		return( -1 );
	}
	if( column_definitions_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid column definitions data.",
		 function );

		return( -1 );
	}
	if( column_definitions_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid column definitions data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( number_of_column_definitions == 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table contains no column definitions.\n",
			 function );
		}
#endif
		return( 1 );
	}
	if( libcdata_array_resize(
	     column_definitions_array,
	     number_of_column_definitions,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_column_definition_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize column definition array.",
		 function );

		goto on_error;
	}
	for( column_definition_index = 0;
	     column_definition_index < number_of_column_definitions;
	     column_definition_index++ )
	{
		if( column_definitions_data_size < sizeof( pff_table_column_definition_7c_t ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid column definitions data size value out of bounds.",
			 function );

			goto on_error;
		}
		if( libpff_column_definition_initialize(
		     &column_definition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create column definition.",
			 function );

			goto on_error;
		}
		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_column_definition_7c_t *) column_definitions_data )->record_entry_type,
		 column_definition->entry_type );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_column_definition_7c_t *) column_definitions_data )->record_entry_value_type,
		 column_definition->value_type );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_column_definition_7c_t *) column_definitions_data )->values_array_offset,
		 column_definition->values_array_offset );

		column_definition->values_array_size = ( (pff_table_column_definition_7c_t *) column_definitions_data )->values_array_size;
		column_definition_number             = ( (pff_table_column_definition_7c_t *) column_definitions_data )->values_array_number;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: column definition: %03d record entry type\t: 0x%04" PRIx16 "",
			 function,
			 column_definition_index,
			 column_definition->entry_type );
		}
#endif
		if( ( column_definition->entry_type >= 0x8000 )
		 || ( column_definition->entry_type <= 0xfffe ) )
		{
			result = libpff_name_to_id_map_entry_get_entry_by_identifier(
			          name_to_id_map_list,
			          (uint32_t) column_definition->entry_type,
			          &( column_definition->name_to_id_map_entry ),
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve name to id map entry: %" PRIu16 ".",
				 function,
				 column_definition->entry_type );

				goto on_error;
			}
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( column_definition->name_to_id_map_entry != NULL )
			{
				if( column_definition->name_to_id_map_entry->type == LIBPFF_NAME_TO_ID_MAP_ENTRY_TYPE_STRING )
				{
					libcnotify_printf(
					 " maps to: %s (%s : %s)\n",
					 (char *) column_definition->name_to_id_map_entry->debug_string,
					 libfmapi_named_property_type_get_identifier(
					  column_definition->name_to_id_map_entry->guid,
					  (char *) column_definition->name_to_id_map_entry->debug_string,
					  column_definition->name_to_id_map_entry->value_size,
					  column_definition->value_type ),
					 libfmapi_named_property_type_get_description(
					  column_definition->name_to_id_map_entry->guid,
					  (char *) column_definition->name_to_id_map_entry->debug_string,
					  column_definition->name_to_id_map_entry->value_size,
					  column_definition->value_type ) );
				}
				else
				{
					libcnotify_printf(
					 " maps to: 0x%04" PRIx32 " (%s : %s)\n",
					 column_definition->name_to_id_map_entry->numeric_value,
					 libfmapi_property_type_get_identifier(
					  column_definition->name_to_id_map_entry->guid,
					  column_definition->name_to_id_map_entry->numeric_value,
					  column_definition->value_type ),
					 libfmapi_property_type_get_description(
					  column_definition->name_to_id_map_entry->guid,
					  column_definition->name_to_id_map_entry->numeric_value,
					  column_definition->value_type ) );
				}
			}
			else
			{
				libcnotify_printf(
				 " (%s : %s)\n",
				 libfmapi_property_type_get_identifier(
				  NULL,
				  column_definition->entry_type,
				  column_definition->value_type ),
				 libfmapi_property_type_get_description(
				  NULL,
				  column_definition->entry_type,
				  column_definition->value_type ) );
			}
			libcnotify_printf(
			 "%s: column definition: %03d record entry value type\t: 0x%04" PRIx16 " (%s : %s)\n",
			 function,
			 column_definition_index,
			 column_definition->value_type,
			 libfmapi_value_type_get_identifier(
			  column_definition->value_type ),
			 libfmapi_value_type_get_description(
			  column_definition->value_type ) );

			libcnotify_printf(
			 "%s: column definition: %03d values array offset\t: %" PRIu16 "\n",
			 function,
			 column_definition_index,
			 column_definition->values_array_offset );

			libcnotify_printf(
			 "%s: column definition: %03d values array size\t: %" PRIu16 "\n",
			 function,
			 column_definition_index,
			 column_definition->values_array_size );

			libcnotify_printf(
			 "%s: column definition: %03d values array number\t: %" PRIu8 "\n",
			 function,
			 column_definition_index,
			 column_definition_number );

			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( libcdata_array_get_entry_by_index(
		     column_definitions_array,
		     (int) column_definition_number,
		     (intptr_t **) &lookup_column_definition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve column definitions: %" PRIu8 " in array.",
			 function,
			 column_definition_number );

			goto on_error;
		}
		if( lookup_column_definition != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: column definitions: %" PRIu8 " already set in array.",
			 function,
			 column_definition_number );

			goto on_error;
		}
		if( libcdata_array_set_entry_by_index(
		     column_definitions_array,
		     (int) column_definition_number,
		     (intptr_t *) column_definition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set column definitions: %" PRIu8 " in array.",
			 function,
			 column_definition_number );

			goto on_error;
		}
		column_definition = NULL;

		column_definitions_data      += sizeof( pff_table_column_definition_7c_t );
		column_definitions_data_size -= sizeof( pff_table_column_definition_7c_t );
	}
	return( 1 );

on_error:
	if( column_definition != NULL )
	{
		libpff_column_definition_free(
		 &column_definition,
		 NULL );
	}
	return( -1 );
}

/* Reads the 8c table record entries and their values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_8c_record_entries(
     libpff_table_t *table,
     libcdata_array_t *record_entries_references_array,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libpff_reference_descriptor_t *reference_descriptor = NULL;
	libpff_internal_record_entry_t *record_entry        = NULL;
	uint8_t *record_entries_data                        = NULL;
	static char *function                               = "libpff_table_read_8c_record_entries";
	size_t number_of_record_entries                     = 0;
	size_t record_entries_data_size                     = 0;
	int number_of_record_entries_references             = 0;
	int record_entries_reference_index                  = 0;
	int record_entry_index                              = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit                                = 0;
#endif

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     record_entries_references_array,
	     &number_of_record_entries_references,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of record entries references.",
		 function );

		return( -1 );
	}
	if( number_of_record_entries_references > 0 )
	{
		if( libpff_table_resize_record_entries(
		     table,
		     1,
		     0,
		     io_handle->ascii_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize record entries.",
			 function );

			return( -1 );
		}
		for( record_entries_reference_index = 0;
		     record_entries_reference_index < number_of_record_entries_references;
		     record_entries_reference_index++ )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: record entries reference: %d\n",
				 function,
				 record_entries_reference_index );
			}
#endif
			if( libcdata_array_get_entry_by_index(
			     record_entries_references_array,
			     record_entries_reference_index,
			     (intptr_t **) &reference_descriptor,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve record entries reference: %d.",
				 function,
				 record_entries_reference_index );

				return( -1 );
			}
			if( reference_descriptor == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing reference descriptor.",
				 function );

				return( -1 );
			}
			if( libpff_table_get_value_data_by_reference(
			     table,
			     io_handle,
			     file_io_handle,
			     reference_descriptor->value,
			     &record_entries_data,
			     &record_entries_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve record entries data.",
				 function );

				return( -1 );
			}
			if( ( record_entries_data == NULL )
			 || ( record_entries_data_size == 0 ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing record entries data.",
				 function );

				return( -1 );
			}
			if( ( record_entries_data_size % sizeof( pff_table_record_entry_8c_t ) ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported record entries data size.",
				 function );

				return( -1 );
			}
			number_of_record_entries = record_entries_data_size / sizeof( pff_table_record_entry_8c_t );

			if( number_of_record_entries > (size_t) INT_MAX )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
				 "%s: number of record entries value exceeds maximum.",
				 function );

				return( -1 );
			}
			if( libpff_table_expand_record_entries(
			     table,
			     0,
			     (int) number_of_record_entries,
			     io_handle->ascii_codepage,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
				 "%s: unable to expand record entries.",
				 function );

				return( -1 );
			}
			while( record_entries_data_size > 0 )
			{
				if( libpff_table_get_record_entry_by_index(
				     table,
				     0,
				     record_entry_index,
				     (libpff_record_entry_t **) &record_entry,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve record entry with set index: 0 and entry index: %d.",
					 function,
					 record_entry_index );

					return( -1 );
				}
				if( record_entry == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing record entry with set index: 0 and entry index: %d.",
					 function,
					 record_entry_index );

					return( -1 );
				}
				record_entry->identifier.format = LIBPFF_RECORD_ENTRY_IDENTIFIER_FORMAT_SECURE4;

				byte_stream_copy_to_uint64_little_endian(
				 ( (pff_table_record_entry_8c_t *) record_entries_data )->identifier,
				 record_entry->identifier.secure4 );

/* TODO use a union for this value data ?  */
				if( libpff_record_entry_set_value_data(
				     (libpff_record_entry_t *) record_entry,
				     ( (pff_table_record_entry_8c_t *) record_entries_data )->descriptor_identifier,
				     sizeof( uint32_t ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set value data in record entry.",
					 function );

					return( -1 );
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: table set: %03d entry: %03d identifier\t\t\t: 0x%" PRIx64 "\n",
					 function,
					 0,
					 record_entry_index,
					 record_entry->identifier.secure4 );

					byte_stream_copy_to_uint32_little_endian(
					 ( (pff_table_record_entry_8c_t *) record_entries_data )->descriptor_identifier,
					 value_32bit );
					libcnotify_printf(
					 "%s: table set: %03d entry: %03d descriptor identifier\t: 0x%08" PRIx32 "\n",
					 function,
					 0,
					 record_entry_index,
					 value_32bit );

					libcnotify_printf(
					 "\n" );
				}
#endif
				record_entries_data      += sizeof( pff_table_record_entry_8c_t );
				record_entries_data_size -= sizeof( pff_table_record_entry_8c_t );

				record_entry_index++;
			}
		}
	}
	return( 1 );
}

/* Reads the 9c table record entries and their values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_9c_record_entries(
     libpff_table_t *table,
     libcdata_array_t *record_entries_references_array,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libpff_reference_descriptor_t *reference_descriptor = NULL;
	libpff_internal_record_entry_t *record_entry        = NULL;
	uint8_t *record_entries_data                        = NULL;
	static char *function                               = "libpff_table_read_9c_record_entries";
	size_t number_of_record_entries                     = 0;
	size_t record_entries_data_size                     = 0;
	int number_of_record_entries_references             = 0;
	int record_entries_reference_index                  = 0;
	int record_entry_index                              = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	system_character_t guid_string[ 48 ];

	libfguid_identifier_t *guid                         = NULL;
	uint32_t value_32bit                                = 0;
	int result                                          = 0;
#endif

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     record_entries_references_array,
	     &number_of_record_entries_references,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of record entries references.",
		 function );

		goto on_error;
	}
	if( number_of_record_entries_references > 0 )
	{
		if( libpff_table_resize_record_entries(
		     table,
		     1,
		     0,
		     io_handle->ascii_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize record entries.",
			 function );

			goto on_error;
		}
		for( record_entries_reference_index = 0;
		     record_entries_reference_index < number_of_record_entries_references;
		     record_entries_reference_index++ )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: record entries reference: %d\n",
				 function,
				 record_entries_reference_index );
			}
#endif
			if( libcdata_array_get_entry_by_index(
			     record_entries_references_array,
			     record_entries_reference_index,
			     (intptr_t **) &reference_descriptor,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve record entries reference: %d.",
				 function,
				 record_entries_reference_index );

				return( -1 );
			}
			if( reference_descriptor == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing reference descriptor.",
				 function );

				return( -1 );
			}
			if( libpff_table_get_value_data_by_reference(
			     table,
			     io_handle,
			     file_io_handle,
			     reference_descriptor->value,
			     &record_entries_data,
			     &record_entries_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve record entries data.",
				 function );

				return( -1 );
			}
			if( ( record_entries_data == NULL )
			 || ( record_entries_data_size == 0 ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing record entries data.",
				 function );

				return( -1 );
			}
			if( ( record_entries_data_size % sizeof( pff_table_record_entry_9c_t ) ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported record entries data size.",
				 function );

				goto on_error;
			}
			number_of_record_entries = record_entries_data_size / sizeof( pff_table_record_entry_9c_t );

			if( number_of_record_entries > (size_t) INT_MAX )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
				 "%s: number of record entries value exceeds maximum.",
				 function );

				goto on_error;
			}
			if( libpff_table_expand_record_entries(
			     table,
			     0,
			     (int) number_of_record_entries,
			     io_handle->ascii_codepage,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
				 "%s: unable to expand record entries.",
				 function );

				goto on_error;
			}
			while( record_entries_data_size > 0 )
			{
				if( libpff_table_get_record_entry_by_index(
				     table,
				     0,
				     record_entry_index,
				     (libpff_record_entry_t **) &record_entry,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve record entry with set index: 0 and entry index: %d.",
					 function,
					 record_entry_index );

					goto on_error;
				}
				if( record_entry == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing record entry with set index: 0 and entry index: %d.",
					 function,
					 record_entry_index );

					goto on_error;
				}
				record_entry->identifier.format = LIBPFF_RECORD_ENTRY_IDENTIFIER_FORMAT_GUID;

				if( memory_copy(
				     record_entry->identifier.guid,
				     ( (pff_table_record_entry_9c_t *) record_entries_data )->record_entry_guid,
				     16 ) == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
					 "%s: unable to copy record entry identifier GUID.",
					 function );

					goto on_error;
				}
/* TODO use a union for this value data ?  */
				if( libpff_record_entry_set_value_data(
				     (libpff_record_entry_t *) record_entry,
				     ( (pff_table_record_entry_9c_t *) record_entries_data )->descriptor_identifier,
				     sizeof( uint32_t ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set value data in record entry.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
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
					if( libfguid_identifier_copy_from_byte_stream(
					     guid,
					     record_entry->identifier.guid,
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
					libcnotify_printf(
					 "%s: table set: %03d entry: %03d record entry guid\t\t\t: %" PRIs_SYSTEM "\n",
					 function,
					 0,
					 record_entry_index,
					 guid_string );

					byte_stream_copy_to_uint32_little_endian(
					 ( (pff_table_record_entry_9c_t *) record_entries_data )->descriptor_identifier,
					 value_32bit );

					libcnotify_printf(
					 "%s: table set: %03d entry: %03d record entry descriptor identifier\t: 0x%08" PRIx32 "\n",
					 function,
					 0,
					 record_entry_index,
					 value_32bit );

					libcnotify_printf(
					 "\n" );
				}
#endif
				record_entries_data      += sizeof( pff_table_record_entry_9c_t );
				record_entries_data_size -= sizeof( pff_table_record_entry_9c_t );

				record_entry_index++;
			}
		}
	}
	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( guid != NULL )
	{
		libfguid_identifier_free(
		 &guid,
		 NULL );
	}
#endif
	return( -1 );
}

/* Reads the a5 table record entry values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_a5_record_entries(
     libpff_table_t *table,
     uint32_t record_entries_reference,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *record_entry  = NULL;
	libpff_record_set_t *record_set               = NULL;
	libpff_table_block_index_t *table_block_index = NULL;
	libpff_table_index_value_t *table_index_value = NULL;
	uint8_t *table_value_data                     = NULL;
	static char *function                         = "libpff_table_read_a5_record_entries";
	size_t table_value_data_size                  = 0;
        uint16_t number_of_table_index_values         = 0;
	uint16_t table_index_value_iterator           = 0;
	int number_of_entries                         = 0;
	int number_of_sets                            = 0;
	int number_of_table_index_array_entries       = 0;
	int table_index_array_entries_iterator        = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	/* Check if the table contains any entries
	 */
	if( record_entries_reference == 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table contains no entries.\n",
			 function );
		}
#endif
		return( 1 );
	}
	if( ( record_entries_reference & 0x0000001fUL ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record entries reference: 0x%08" PRIx32 " (0x%08" PRIx32 ").",
		 function,
		 record_entries_reference & 0x0000001fUL,
		 record_entries_reference );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     table->record_sets_array,
	     &number_of_sets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sets.",
		 function );

		return( -1 );
	}
	if( number_of_sets > 0 )
	{
		if( libcdata_array_get_entry_by_index(
		     table->record_sets_array,
		     0,
		     (intptr_t **) &record_set,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record set: 0.",
			 function );

			return( -1 );
		}
		if( libpff_record_set_get_number_of_entries(
		     record_set,
		     &number_of_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of entries of set: 0.",
			 function );

			return( -1 );
		}
	}
	if( libcdata_array_get_number_of_entries(
	     table->index_array,
	     &number_of_table_index_array_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of table index array entries.",
		 function );

		return( -1 );
	}
	for( table_index_array_entries_iterator = 0;
	     table_index_array_entries_iterator < number_of_table_index_array_entries;
	     table_index_array_entries_iterator++ )
	{
		if( libcdata_array_get_entry_by_index(
		     table->index_array,
		     table_index_array_entries_iterator,
		     (intptr_t **) &table_block_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve table block index: %" PRIu16 ".",
			 function,
			 table_index_array_entries_iterator );

			return( -1 );
		}
		if( libpff_table_block_index_get_number_of_values(
		     table_block_index,
		     &number_of_table_index_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of table block index values.",
			 function );

			return( -1 );
		}
		if( ( number_of_table_index_array_entries > number_of_sets )
		 || ( number_of_table_index_values > number_of_entries ) )
		{
			if( libpff_table_resize_record_entries(
			     table,
			     number_of_table_index_array_entries,
			     number_of_table_index_values,
			     io_handle->ascii_codepage,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
				 "%s: unable to resize record entries.",
				 function );

				return( -1 );
			}
		}
		for( table_index_value_iterator = 0;
		     table_index_value_iterator < number_of_table_index_values;
		     table_index_value_iterator++ )
		{
			if( libpff_table_get_record_entry_by_index(
			     table,
			     table_index_array_entries_iterator,
			     table_index_value_iterator,
			     (libpff_record_entry_t **) &record_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve record entry with set index: %d and entry index: %" PRIu16 ".",
				 function,
				 table_index_array_entries_iterator,
				 table_index_value_iterator );

				return( -1 );
			}
			if( record_entry == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing record entry with set index: %d and entry index: %" PRIu16 ".",
				 function,
				 table_index_array_entries_iterator,
				 table_index_value_iterator );

				return( -1 );
			}
			if( libpff_table_block_index_get_value_by_index(
			     table_block_index,
			     table_index_value_iterator,
			     &table_index_value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve table block index value: %" PRIu16 ".",
				 function,
				 table_index_value_iterator );

				return( -1 );
			}
			if( table_index_value == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing table index value.",
				 function );

				return( -1 );
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: table set: %03" PRIu32 " entry: %03" PRIu16 " value at offset: %" PRIu32 " with size: %" PRIu16 ".\n",
				 function,
				 table_index_array_entries_iterator,
				 table_index_value_iterator,
				 table_index_value->offset,
				 table_index_value->size );
			}
#endif
			if( libpff_table_get_value_data_by_index_value(
			     table,
			     table_index_value,
			     file_io_handle,
			     &table_value_data,
			     &table_value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve table value data by index value: %" PRIu16 ".",
				 function,
				 table_index_value_iterator );

				return( -1 );
			}
			if( libpff_record_entry_set_value_data(
			     (libpff_record_entry_t *) record_entry,
			     table_value_data,
			     table_value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set value data in record entry.",
				 function );

				return( -1 );
			}
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
}

/* Reads the ac table column definitions
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_ac_column_definitions(
     libpff_table_t *table,
     libcdata_array_t *column_definitions_array,
     uint32_t column_definitions_reference,
     int number_of_column_definitions,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     libcdata_list_t *name_to_id_map_list,
     libcerror_error_t **error )
{
	libfcache_cache_t *column_definitions_data_cache         = NULL;
	libfdata_list_t *column_definitions_data_list            = NULL;
	libpff_column_definition_t *column_definition            = NULL;
	libpff_column_definition_t *lookup_column_definition     = NULL;
	libpff_data_block_t *column_definition_data_block        = NULL;
	libpff_data_block_t *column_definitions_data_block       = NULL;
	libpff_local_descriptor_value_t *local_descriptor_value  = NULL;
	pff_table_column_definition_ac_t *column_definition_data = NULL;
	static char *function                                    = "libpff_table_read_ac_column_definitions";
	size_t column_definition_data_offset                     = 0;
	size_t column_definition_data_size                       = 0;
	off64_t column_definition_data_block_offset              = 0;
	uint32_t record_entry_values_table_descriptor            = 0;
	uint16_t column_definition_number                        = 0;
	int column_definition_index                              = 0;
	int element_index                                        = 0;
	int result                                               = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( column_definitions_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid column definitions array.",
		 function );

		return( -1 );
	}
	if( number_of_column_definitions == 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table contains no column definitions.\n",
			 function );
		}
#endif
		return( 1 );
	}
	/* Read the column definitions
	 */
	result = libpff_local_descriptors_tree_get_value_by_identifier(
		  table->local_descriptors_tree,
		  file_io_handle,
		  table->local_descriptors_cache,
		  column_definitions_reference,
		  &local_descriptor_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve descriptor identifier: %" PRIu32 " from local descriptors.",
		 function,
		 column_definitions_reference );

		goto on_error;
	}
	else if( result == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing column definitions descriptor: 0x%08" PRIx32 " (%" PRIu32 ").",
		 function,
		 column_definitions_reference,
		 column_definitions_reference );

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
		 "%s: identifier: %" PRIu64 " (%s), data: %" PRIu64 ", local descriptors: %" PRIu64 "\n",
		 function,
		 local_descriptor_value->identifier,
		 libpff_debug_get_node_identifier_type(
		  (uint8_t) ( local_descriptor_value->identifier & 0x0000001fUL ) ),
		 local_descriptor_value->data_identifier,
		 local_descriptor_value->local_descriptors_identifier );
	}
#endif
/* TODO handle multiple recovered offset index values */
	if( libpff_io_handle_read_descriptor_data_list(
	     io_handle,
	     file_io_handle,
	     offsets_index,
	     column_definitions_reference,
	     local_descriptor_value->data_identifier,
	     table->recovered,
	     0,
	     &column_definitions_data_list,
	     &column_definitions_data_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read descriptor: %" PRIu32 " data: %" PRIu64 " list.",
		 function,
		 column_definitions_reference,
		 local_descriptor_value->data_identifier );

		goto on_error;
	}
	/* Retrieve the corresponding column definitions data reference segment
	 */
	if( libfdata_list_get_element_value_by_index(
	     column_definitions_data_list,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) column_definitions_data_cache,
	     0,
	     (intptr_t **) &column_definitions_data_block,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column definitions data block: 0.",
		 function );

		goto on_error;
	}
	if( column_definitions_data_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing column definitions data block: 0.",
		 function );

		goto on_error;
	}
	if( column_definitions_data_block->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid column definitions data block: 0 - missing data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: ac column definitions:\n",
		 function );
		libcnotify_print_data(
		 column_definitions_data_block->data,
		 column_definitions_data_block->uncompressed_data_size,
		 0 );
	}
#endif
	column_definition_data_size = (size_t) number_of_column_definitions * sizeof( pff_table_column_definition_ac_t );

	if( column_definition_data_size != (size_t) column_definitions_data_block->uncompressed_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: mismatch in number of column definitions and the data size.",
		 function );

		goto on_error;
	}
	if( libcdata_array_resize(
	     column_definitions_array,
	     number_of_column_definitions,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_column_definition_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize column definition array.",
		 function );

		goto on_error;
	}
	column_definition_data_offset = 0;

	for( column_definition_index = 0;
	     column_definition_index < number_of_column_definitions;
	     column_definition_index++ )
	{
		if( libpff_column_definition_initialize(
		     &column_definition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create column definition.",
			 function );

			goto on_error;
		}
		if( libfdata_list_get_element_value_at_offset(
		     column_definitions_data_list,
		     (intptr_t *) file_io_handle,
		     (libfdata_cache_t *) column_definitions_data_cache,
		     (off64_t) column_definition_data_offset,
		     &element_index,
		     &column_definition_data_block_offset,
		     (intptr_t **) &column_definition_data_block,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve column definition data block at offset: %" PRIzd ".",
			 function,
			 column_definition_data_offset );

			goto on_error;
		}
		if( column_definition_data_block == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing column definition data block at offset: %" PRIzd ".",
			 function,
			 column_definition_data_offset );

			goto on_error;
		}
		if( column_definition_data_block->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid column definition data block at offset: %" PRIzd " - missing data.",
			 function,
			 column_definition_data_offset );

			goto on_error;
		}
		if( column_definition_data_block_offset > column_definition_data_block->uncompressed_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid column definitions data block offset value out of bounds.",
			 function );

			goto on_error;
		}
		if( column_definition_data_block->uncompressed_data_size < sizeof( pff_table_column_definition_ac_t ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid column definitions data size value out of bounds.",
			 function );

			goto on_error;
		}
		column_definition_data = (pff_table_column_definition_ac_t *) &( column_definition_data_block->data[ column_definition_data_block_offset ] );

		byte_stream_copy_to_uint16_little_endian(
		 column_definition_data->record_entry_type,
		 column_definition->entry_type );

		byte_stream_copy_to_uint16_little_endian(
		 column_definition_data->record_entry_value_type,
		 column_definition->value_type );

		byte_stream_copy_to_uint16_little_endian(
		 column_definition_data->values_array_offset,
		 column_definition->values_array_offset );

		byte_stream_copy_to_uint16_little_endian(
		 column_definition_data->values_array_size,
		 column_definition->values_array_size );

		byte_stream_copy_to_uint16_little_endian(
		 column_definition_data->values_array_number,
		 column_definition_number );

		byte_stream_copy_to_uint32_little_endian(
		 column_definition_data->record_entry_values_table_descriptor,
		 record_entry_values_table_descriptor );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: column definition: %03d record entry type\t\t\t: 0x%04" PRIx16 "",
			 function,
			 column_definition_index,
			 column_definition->entry_type );
		}
#endif
		if( ( column_definition->entry_type >= 0x8000 )
		 || ( column_definition->entry_type <= 0xfffe ) )
		{
			result = libpff_name_to_id_map_entry_get_entry_by_identifier(
			          name_to_id_map_list,
			          (uint32_t) column_definition->entry_type,
			          &( column_definition->name_to_id_map_entry ),
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve name to id map entry: %" PRIu16 ".",
				 function,
				 column_definition->entry_type );

				goto on_error;
			}
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( column_definition->name_to_id_map_entry != NULL )
			{
				if( column_definition->name_to_id_map_entry->type == LIBPFF_NAME_TO_ID_MAP_ENTRY_TYPE_STRING )
				{
					libcnotify_printf(
					 " maps to: %s (%s : %s)\n",
					 (char *) column_definition->name_to_id_map_entry->debug_string,
					 libfmapi_named_property_type_get_identifier(
					  column_definition->name_to_id_map_entry->guid,
					  (char *) column_definition->name_to_id_map_entry->debug_string,
					  column_definition->name_to_id_map_entry->value_size,
					  column_definition->value_type ),
					 libfmapi_named_property_type_get_description(
					  column_definition->name_to_id_map_entry->guid,
					  (char *) column_definition->name_to_id_map_entry->debug_string,
					  column_definition->name_to_id_map_entry->value_size,
					  column_definition->value_type ) );
				}
				else
				{
					libcnotify_printf(
					 " maps to: 0x%04" PRIx32 " (%s : %s)\n",
					 column_definition->name_to_id_map_entry->numeric_value,
					 libfmapi_property_type_get_identifier(
					  column_definition->name_to_id_map_entry->guid,
					  column_definition->name_to_id_map_entry->numeric_value,
					  column_definition->value_type ),
					 libfmapi_property_type_get_description(
					  column_definition->name_to_id_map_entry->guid,
					  column_definition->name_to_id_map_entry->numeric_value,
					  column_definition->value_type ) );
				}
			}
			else
			{
				libcnotify_printf(
				 " (%s : %s)\n",
				 libfmapi_property_type_get_identifier(
				  NULL,
				  column_definition->entry_type,
				  column_definition->value_type ),
				 libfmapi_property_type_get_description(
				  NULL,
				  column_definition->entry_type,
				  column_definition->value_type ) );
			}
			libcnotify_printf(
			 "%s: column definition: %03d record entry value type\t\t: 0x%04" PRIx16 " (%s : %s)\n",
			 function,
			 column_definition_index,
			 column_definition->value_type,
			 libfmapi_value_type_get_identifier(
			  column_definition->value_type ),
			 libfmapi_value_type_get_description(
			  column_definition->value_type ) );

			libcnotify_printf(
			 "%s: column definition: %03d values array offset\t\t: %" PRIu16 "\n",
			 function,
			 column_definition_index,
			 column_definition->values_array_offset );

			libcnotify_printf(
			 "%s: column definition: %03d values array size\t\t: %" PRIu16 "\n",
			 function,
			 column_definition_index,
			 column_definition->values_array_size );

			libcnotify_printf(
			 "%s: column definition: %03d values array number\t\t: %" PRIu16 "\n",
			 function,
			 column_definition_index,
			 column_definition_number );

			libcnotify_printf(
			 "%s: padding1:\n",
			 function );
			libcnotify_print_data(
			 column_definition_data->padding1,
			 2,
			 0 );

			libcnotify_printf(
			 "%s: column definition: %03d record entry values table descriptor\t: %" PRIu32 "\n",
			 function,
			 column_definition_index,
			 record_entry_values_table_descriptor );

			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		/* Read the record entry values table if necessary
		 */
		if( record_entry_values_table_descriptor > 0 )
		{
			result = libpff_local_descriptors_tree_get_value_by_identifier(
				  table->local_descriptors_tree,
				  file_io_handle,
				  table->local_descriptors_cache,
				  record_entry_values_table_descriptor,
				  &local_descriptor_value,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve descriptor identifier: %" PRIu32 " from local descriptors.",
				 function,
				 record_entry_values_table_descriptor );

				goto on_error;
			}
			else if( result == 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing local descriptor identifier: %" PRIu32 ".",
				 function,
				 record_entry_values_table_descriptor );

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
				 "%s: invalid local descriptor identifier: %" PRIu32 " - missing data identifier.",
				 function,
				 record_entry_values_table_descriptor );

				goto on_error;
			}
			if( libpff_table_initialize(
			     &( column_definition->record_entry_values_table ),
			     record_entry_values_table_descriptor,
			     local_descriptor_value->data_identifier,
			     local_descriptor_value->local_descriptors_identifier,
			     table->recovered,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create record entry values table.",
				 function );

				goto on_error;
			}
			if( libpff_table_read(
			     column_definition->record_entry_values_table,
			     io_handle,
			     file_io_handle,
			     offsets_index,
			     name_to_id_map_list,
			     LIBPFF_DEBUG_ITEM_TYPE_DEFAULT,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read record entry values table.",
				 function );

				goto on_error;
			}
		}
		if( libcdata_array_get_entry_by_index(
		     column_definitions_array,
		     (int) column_definition_number,
		     (intptr_t **) &lookup_column_definition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve column definitions: %" PRIu8 " in array.",
			 function,
			 column_definition_number );

			goto on_error;
		}
		if( lookup_column_definition != NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: column definitions: %" PRIu8 " already set in array.",
			 function,
			 column_definition_number );

			goto on_error;
		}
		if( libcdata_array_set_entry_by_index(
		     column_definitions_array,
		     (int) column_definition_number,
		     (intptr_t *) column_definition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set column definition: %" PRIu16 " in array.",
			 function,
			 column_definition_number );

			goto on_error;
		}
		column_definition = NULL;

		column_definition_data_offset += sizeof( pff_table_column_definition_ac_t );
	}
	if( libfcache_cache_free(
	     &column_definitions_data_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free column definitions data cache.",
		 function );

		goto on_error;
	}
	if( libfdata_list_free(
	     &column_definitions_data_list,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free column definitions data list.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( column_definition != NULL )
	{
		libpff_column_definition_free(
		 &column_definition,
		 NULL );
	}
	if( column_definitions_data_cache != NULL )
	{
		libfcache_cache_free(
		 &column_definitions_data_cache,
		 NULL );
	}
	if( column_definitions_data_list != NULL )
	{
		libfdata_list_free(
		 &column_definitions_data_list,
		 NULL );
	}
	return( -1 );
}

/* Reads the bc table record entries and their values
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_bc_record_entries(
     libpff_table_t *table,
     libcdata_array_t *record_entries_references_array,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     libcdata_list_t *name_to_id_map_list,
     int debug_item_type,
     libcerror_error_t **error )
{
	libpff_reference_descriptor_t *reference_descriptor = NULL;
	uint8_t *record_entries_data                        = NULL;
	static char *function                               = "libpff_table_read_bc_record_entries";
	size_t number_of_record_entries                     = 0;
	size_t record_entries_data_size                     = 0;
	uint16_t record_entry_type                          = 0;
	uint16_t record_entry_value_type                    = 0;
	int number_of_record_entries_references             = 0;
	int record_entries_reference_index                  = 0;
	int record_entry_index                              = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     record_entries_references_array,
	     &number_of_record_entries_references,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of record entries references.",
		 function );

		return( -1 );
	}
	if( number_of_record_entries_references > 0 )
	{
		if( libpff_table_resize_record_entries(
		     table,
		     1,
		     0,
		     io_handle->ascii_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize record entries.",
			 function );

			return( -1 );
		}
		for( record_entries_reference_index = 0;
		     record_entries_reference_index < number_of_record_entries_references;
		     record_entries_reference_index++ )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: record entries reference: %d\n",
				 function,
				 record_entries_reference_index );
			}
#endif
			if( libcdata_array_get_entry_by_index(
			     record_entries_references_array,
			     record_entries_reference_index,
			     (intptr_t **) &reference_descriptor,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve record entries reference: %d.",
				 function,
				 record_entries_reference_index );

				return( -1 );
			}
			if( reference_descriptor == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing reference descriptor.",
				 function );

				return( -1 );
			}
			if( libpff_table_get_value_data_by_reference(
			     table,
			     io_handle,
			     file_io_handle,
			     reference_descriptor->value,
			     &record_entries_data,
			     &record_entries_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve record entries data.",
				 function );

				return( -1 );
			}
			if( ( record_entries_data == NULL )
			 || ( record_entries_data_size == 0 ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing record entries data.",
				 function );

				return( -1 );
			}
			if( ( record_entries_data_size % sizeof( pff_table_record_entry_bc_t ) ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported record entries data size.",
				 function );

				return( -1 );
			}
			number_of_record_entries = record_entries_data_size / sizeof( pff_table_record_entry_bc_t );

			if( number_of_record_entries > (size_t) INT_MAX )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
				 "%s: number of record entries value exceeds maximum.",
				 function );

				return( -1 );
			}
			if( libpff_table_expand_record_entries(
			     table,
			     0,
			     (int) number_of_record_entries,
			     io_handle->ascii_codepage,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
				 "%s: unable to expand record entries.",
				 function );

				return( -1 );
			}
			while( record_entries_data_size > 0 )
			{
				byte_stream_copy_to_uint16_little_endian(
				 ( (pff_table_record_entry_bc_t *) record_entries_data )->record_entry_type,
				 record_entry_type );

				byte_stream_copy_to_uint16_little_endian(
				 ( (pff_table_record_entry_bc_t *) record_entries_data )->record_entry_value_type,
				 record_entry_value_type );

				if( libpff_table_read_entry_value(
				     table,
				     0,
				     record_entry_index,
				     (uint32_t) record_entry_type,
				     (uint32_t) record_entry_value_type,
				     (uint8_t *) ( (pff_table_record_entry_bc_t *) record_entries_data )->record_entry_value,
				     4,
				     io_handle,
				     file_io_handle,
				     offsets_index,
				     name_to_id_map_list,
				     NULL,
				     NULL,
				     debug_item_type,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read entry value: %" PRIu32 ".",
					 function,
					 record_entry_index );

					return( -1 );
				}
				record_entries_data      += sizeof( pff_table_record_entry_bc_t );
				record_entries_data_size -= sizeof( pff_table_record_entry_bc_t );

				record_entry_index++;
			}
		}
	}
	return( 1 );
}

/* Retrieves a specific values array data entry
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libpff_table_values_array_get_value_data_by_entry_number(
     libpff_table_t *table,
     uint32_t values_array_reference,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     uint32_t values_array_entry_number,
     uint16_t values_array_entry_size,
     uint8_t **values_array_data,
     size_t *values_array_data_size,
     uint8_t read_flags,
     libcerror_error_t **error )
{
	libpff_data_block_t *data_block                         = NULL;
	libpff_local_descriptor_value_t *local_descriptor_value = NULL;
	static char *function                                   = "libpff_table_values_array_get_value_data_by_entry_number";
	size64_t values_array_block_size                        = 0;
	size_t values_array_data_offset                         = 0;
	int result                                              = 0;
	int values_array_block_index                            = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( values_array_entry_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid values array entry size value zero or less.",
		 function );

		return( -1 );
	}
	if( values_array_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values array data.",
		 function );

		return( -1 );
	}
	if( values_array_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values array data size.",
		 function );

		return( -1 );
	}
/* TODO find the right offset within the data */
	if( ( values_array_reference & 0x0000001fUL ) != 0 )
	{
		if( table->values_array_data_list == NULL )
		{
			result = libpff_local_descriptors_tree_get_value_by_identifier(
				  table->local_descriptors_tree,
				  file_io_handle,
				  table->local_descriptors_cache,
				  values_array_reference,
				  &local_descriptor_value,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve descriptor identifier: %" PRIu32 " from local descriptors.",
				 function,
				 values_array_reference );

				return( -1 );
			}
			else if( result == 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing values array descriptor: 0x%08" PRIx32 " (%" PRIu32 ").",
				 function,
				 values_array_reference,
				 values_array_reference );

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
/* TODO handle multiple recovered offset index values */
			if( libpff_io_handle_read_descriptor_data_list(
			     io_handle,
			     file_io_handle,
			     offsets_index,
			     values_array_reference,
			     local_descriptor_value->data_identifier,
			     table->recovered,
			     0,
			     &( table->values_array_data_list ),
			     &( table->values_array_data_cache ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read descriptor: %" PRIu32 " data: %" PRIu64 " list.",
				 function,
				 values_array_reference,
				 local_descriptor_value->data_identifier );

				return( -1 );
			}
			if( libfdata_list_get_mapped_size_by_index(
			     table->values_array_data_list,
			     0,
			     &values_array_block_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve mapped size of data block: 0.",
				 function );

				return( -1 );
			}
			table->value_array_entries_per_block = (int) ( values_array_block_size / values_array_entry_size );
		}
		if( table->value_array_entries_per_block == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid table - missing value array entries per block value.",
			 function );

			return( -1 );
		}
		values_array_block_index = values_array_entry_number / table->value_array_entries_per_block;

		/* Retrieve the corresponding data block
		 */
		if( libfdata_list_get_element_value_by_index(
		     table->values_array_data_list,
		     (intptr_t *) file_io_handle,
		     (libfdata_cache_t *) table->values_array_data_cache,
		     values_array_block_index,
		     (intptr_t **) &data_block,
		     read_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data block: %" PRIu32 ".",
			 function,
			 values_array_block_index );

			return( -1 );
		}
		if( data_block == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing data block: %" PRIu32 ".",
			 function,
			 values_array_block_index );

			return( -1 );
		}
		if( data_block->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid data block: %" PRIu32 " - missing data.",
			 function,
			 values_array_block_index );

			return( -1 );
		}
		values_array_data_offset = ( values_array_entry_number % table->value_array_entries_per_block )
		                         * values_array_entry_size;

		if( values_array_data_offset >= data_block->uncompressed_data_size )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: invalid values array entry number value out of bounds.",
			         function );
			}
#endif
			return( 0 );
		}
		*values_array_data      = data_block->data + values_array_data_offset;
		*values_array_data_size = (size_t) values_array_entry_size;
	}
	else
	{
		if( libpff_table_get_value_data_by_reference(
		     table,
		     io_handle,
		     file_io_handle,
		     values_array_reference,
		     values_array_data,
		     values_array_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value data by reference.",
			 function );

			return( -1 );
		}
		if( ( *values_array_data == NULL )
		 || ( *values_array_data_size == 0 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing values array data.",
			 function );

			return( -1 );
		}
		values_array_data_offset = (size_t) values_array_entry_number * (size_t) values_array_entry_size;

		if( ( values_array_data_offset >= *values_array_data_size )
		 || ( values_array_entry_size > ( *values_array_data_size - values_array_data_offset ) ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: invalid values array entry number: %" PRIu32 " value out of bounds.\n",
			         function,
				 values_array_entry_number );
			}
#endif
			return( 0 );
		}
		*values_array_data     += values_array_data_offset;
		*values_array_data_size = (size_t) values_array_entry_size;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: values array data entry: %" PRIu32 ":\n",
		 function,
		 values_array_entry_number );
		libcnotify_print_data(
		 *values_array_data,
		 *values_array_data_size,
		 0 );
	}
#endif
	return( 1 );
}

/* Reads the table values array
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_values_array(
     libpff_table_t *table,
     libcdata_array_t *record_entries_references_array,
     uint32_t values_array_reference,
     uint8_t record_entry_identifier_size,
     uint8_t record_entry_value_size,
     uint16_t values_array_entry_size,
     libcdata_array_t *column_definitions_array,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     libcerror_error_t **error )
{
	libpff_column_definition_t *column_definition       = NULL;
	libpff_reference_descriptor_t *reference_descriptor = NULL;
	uint8_t *record_entries_data                        = NULL;
	uint8_t *record_entry_data                          = NULL;
	uint8_t *record_entry_values_data                   = NULL;
	static char *function                               = "libpff_table_read_values_array";
	size_t number_of_record_entries                     = 0;
	size_t record_entries_data_size                     = 0;
	size_t record_entry_size                            = 0;
	size_t record_entry_values_data_size                = 0;
	uint32_t record_entry_values_array_identifier       = 0;
	uint32_t record_entry_values_array_number           = 0;
	uint32_t table_values_array_identifier              = 0;
	int column_definition_index                         = 0;
	int number_of_column_definitions                    = 0;
	int number_of_record_entries_references             = 0;
	int number_of_sets                                  = 0;
	int record_entries_reference_index                  = 0;
	int record_entry_index                              = 0;
	int result                                          = 0;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( ( record_entry_identifier_size != 4 )
	 || ( ( record_entry_value_size != 2 )
	  &&  ( record_entry_value_size != 4 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record entry identifier size: 0x%02" PRIx8 " and record entry value size: 0x%02" PRIx8 ".",
		 function,
		 record_entry_identifier_size,
		 record_entry_value_size );

		return( -1 );
	}
	if( column_definitions_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid column definitions array.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     record_entries_references_array,
	     &number_of_record_entries_references,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of record entries references.",
		 function );

		goto on_error;
	}
	/* Check if the table contains any entries
	 */
	if( ( number_of_record_entries_references == 0 )
	 && ( values_array_reference == 0 ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table contains no entries.\n",
			 function );
		}
#endif
		return( 1 );
	}
	if( number_of_record_entries_references == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: table contains value array but no record entries.",
		 function );

		goto on_error;
	}
	if( values_array_reference == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: table contains record entries but no value array.",
		 function );

		goto on_error;
	}
	if( libcdata_array_get_number_of_entries(
	     column_definitions_array,
	     &number_of_column_definitions,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of column definitions.",
		 function );

		goto on_error;
	}
	record_entry_size = record_entry_identifier_size + record_entry_value_size;

	if( libpff_table_resize_record_entries(
	     table,
	     0,
	     0,
	     io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize record entries.",
		 function );

		goto on_error;
	}
	for( record_entries_reference_index = 0;
	     record_entries_reference_index < number_of_record_entries_references;
	     record_entries_reference_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     record_entries_references_array,
		     record_entries_reference_index,
		     (intptr_t **) &reference_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record entries reference: %d.",
			 function,
			 record_entries_reference_index );

			goto on_error;
		}
		if( reference_descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing reference descriptor.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: record entries reference: %d\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 record_entries_reference_index,
			 reference_descriptor->value );
		}
#endif
		if( libpff_table_clone_value_data_by_reference(
		     table,
		     reference_descriptor->value,
		     io_handle,
		     file_io_handle,
		     &record_entries_data,
		     &record_entries_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record entries data.",
			 function );

			goto on_error;
		}
		if( ( record_entries_data == NULL )
		 || ( record_entries_data_size == 0 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing record entries data.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: record entries data:\n",
			 function );
			libcnotify_print_data(
			 record_entries_data,
			 record_entries_data_size,
			 0 );
		}
#endif
		if( ( record_entries_data_size % record_entry_size ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported record entries data size.",
			 function );

			goto on_error;
		}
		number_of_record_entries = record_entries_data_size / record_entry_size;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: number of record entries\t\t\t: %" PRIzd "\n",
			 function,
			 number_of_record_entries );
		}
#endif
		if( number_of_record_entries > (size_t) ( INT_MAX - record_entry_index ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: number of record entries value exceeds maximum.",
			 function );

			goto on_error;
		}
		if( number_of_record_entries >= (size_t) ( number_of_sets - record_entry_index ) )
		{
			number_of_sets = record_entry_index + (int) number_of_record_entries;

			if( libpff_table_resize_record_entries(
			     table,
			     number_of_sets,
			     number_of_column_definitions,
			     io_handle->ascii_codepage,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
				 "%s: unable to resize record entries.",
				 function );

				goto on_error;
			}
		}
		record_entry_data = record_entries_data;

		while( record_entries_data_size > 0 )
		{
			byte_stream_copy_to_uint32_little_endian(
			 record_entry_data,
			 record_entry_values_array_identifier );

			record_entry_data        += 4;
			record_entries_data_size -= 4;

			if( record_entry_value_size == 2 )
			{
				byte_stream_copy_to_uint16_little_endian(
				 record_entry_data,
				 record_entry_values_array_number );

				record_entry_data        += 2;
				record_entries_data_size -= 2;
			}
			else if( record_entry_value_size == 4 )
			{
				byte_stream_copy_to_uint32_little_endian(
				 record_entry_data,
				 record_entry_values_array_number );

				record_entry_data        += 4;
				record_entries_data_size -= 4;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: record entry: %03" PRIu32 " values array identifier\t: 0x%08" PRIx32 "\n",
				 function,
				 record_entry_index,
				 record_entry_values_array_identifier );

				libcnotify_printf(
				 "%s: record entry: %03" PRIu32 " values array number\t\t: %" PRIu32 "\n",
				 function,
				 record_entry_index,
				 record_entry_values_array_number );

				libcnotify_printf(
				 "\n" );
			}
#endif
			result = libpff_table_values_array_get_value_data_by_entry_number(
			          table,
			          values_array_reference,
			          io_handle,
			          file_io_handle,
			          offsets_index,
			          record_entry_values_array_number,
			          values_array_entry_size,
			          &record_entry_values_data,
			          &record_entry_values_data_size,
			          0,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve record entry values data for index: %" PRIu32 ".",
				 function,
				 record_entry_values_array_number );

				goto on_error;
			}
			else if( result == 0 )
			{
				record_entry_index++;

				table->flags |= LIBPFF_TABLE_FLAG_MISSING_RECORD_ENTRY_DATA;

				continue;
			}
			if( record_entry_values_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing record entry values data.",
				 function );

				goto on_error;
			}
			if( record_entry_values_data_size < (size_t) values_array_entry_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid table values data size value out of bounds.",
				 function );

				goto on_error;
			}
			/* If the value array numbers are not stored sequential
			 * resize the record entries to the required size.
			 * Make sure the value entry exists first.
			 */
#if SIZEOF_INT <= 4
			if( record_entry_values_array_number >= (uint32_t) number_of_sets )
#else
			if( (int) record_entry_values_array_number >= number_of_sets )
#endif
			{
#if SIZEOF_INT <= 4
				if( record_entry_values_array_number > (uint32_t) ( INT_MAX - 1 ) )
#else
				if( (int) record_entry_values_array_number > (int) ( INT_MAX - 1 ) )
#endif
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
					 "%s: number of record entries value exceeds maximum.",
					 function );

					goto on_error;
				}
				number_of_sets = (int) ( record_entry_values_array_number + 1 );

				if( libpff_table_resize_record_entries(
				     table,
				     number_of_sets,
				     number_of_column_definitions,
				     io_handle->ascii_codepage,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
					 "%s: unable to resize record entries.",
					 function );

					goto on_error;
				}
			}
			for( column_definition_index = 0;
			     column_definition_index < number_of_column_definitions;
			     column_definition_index++ )
			{
				if( libcdata_array_get_entry_by_index(
				     column_definitions_array,
				     column_definition_index,
				     (intptr_t **) &column_definition,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve column definitions array entry: %d.",
					 function,
					 column_definition_index );

					goto on_error;
				}
				if( column_definition == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing column definition: %d.",
					 function,
					 column_definition_index );

					goto on_error;
				}
				/* For some unknown reason when the values array is read
				 * the data array is padded with zero or remnant values
				 * therefore the values array entries do not align
				 * this check is makes sure the aligment is correct
				 */
				if( column_definition_index == 0 )
				{
					if( column_definition->values_array_offset != 0 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
						 "%s: unsupported first column definition values array offset.",
						 function );

						goto on_error;
					}
					if( column_definition->values_array_size != 4 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
						 "%s: unsupported first column definition values array size.",
						 function );

						goto on_error;
					}
					byte_stream_copy_to_uint32_little_endian(
					 record_entry_values_data,
					 table_values_array_identifier );

					/* If decryption was forced reread the entry without decryption
					 */
					if( ( io_handle->force_decryption != 0 )
					 && ( record_entry_values_array_identifier != table_values_array_identifier ) )
					{
						result = libpff_table_values_array_get_value_data_by_entry_number(
						          table,
						          values_array_reference,
						          io_handle,
						          file_io_handle,
						          offsets_index,
						          record_entry_values_array_number,
						          values_array_entry_size,
						          &record_entry_values_data,
						          &record_entry_values_data_size,
						          LIBFDATA_READ_FLAG_IGNORE_CACHE | LIBPFF_READ_FLAG_IGNORE_FORCE_DECRYPTION,
						          error );

						if( result != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
							 "%s: unable to retrieve record entry values data for index: %" PRIu32 ".",
							 function,
							 record_entry_values_array_number );

							goto on_error;
						}
						if( record_entry_values_data == NULL )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
							 "%s: missing record entry values data.",
							 function );

							goto on_error;
						}
						if( record_entry_values_data_size < (size_t) values_array_entry_size )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
							 "%s: invalid table values data size value out of bounds.",
							 function );

							goto on_error;
						}
						byte_stream_copy_to_uint32_little_endian(
						 record_entry_values_data,
						 table_values_array_identifier );
					}
					if( record_entry_values_array_identifier != table_values_array_identifier )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
						 "%s: mismatch in values array identifier (0x%08" PRIx32 " != 0x%08" PRIx32 ").",
						 function,
						 record_entry_values_array_identifier,
						 table_values_array_identifier );

						goto on_error;
					}
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: values array data:\n",
						 function );
						libcnotify_print_data(
						 record_entry_values_data,
						 values_array_entry_size,
						 0 );
					}
#endif
				}
				if( column_definition->values_array_offset > values_array_entry_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid column definition values array offset value exceeds values array size.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: values array data at offset: %" PRIu16 " of size: %" PRIu16 "\n",
					 function,
					 column_definition->values_array_offset,
					 column_definition->values_array_size );
					libcnotify_print_data(
					 &( record_entry_values_data[ column_definition->values_array_offset ] ),
					 (size_t) column_definition->values_array_size,
					 0 );
				}
#endif
				/* To prevent multiple lookups the name to id map is not passed
				 */
				if( libpff_table_read_entry_value(
				     table,
				     record_entry_index,
				     column_definition_index,
				     column_definition->entry_type,
				     column_definition->value_type,
				     &( record_entry_values_data[ column_definition->values_array_offset ] ),
				     (size_t) column_definition->values_array_size,
				     io_handle,
				     file_io_handle,
				     offsets_index,
				     NULL,
				     column_definition->name_to_id_map_entry,
				     column_definition->record_entry_values_table,
				     LIBPFF_DEBUG_ITEM_TYPE_DEFAULT,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read entry value: %" PRIu32 ".",
					 function,
					 record_entry_values_array_number );

					goto on_error;
				}
			}
			record_entry_index++;
		}
		memory_free(
		 record_entries_data );

		record_entries_data = NULL;
	}
	return( 1 );

on_error:
	if( record_entries_data != NULL )
	{
		memory_free(
		 record_entries_data );
	}
	return( -1 );
}

/* Reads a table record entry value
 * Returns 1 if successful or -1 on error
 */
int libpff_table_read_entry_value(
     libpff_table_t *table,
     int set_index,
     int entry_index,
     uint32_t record_entry_type,
     uint32_t record_entry_value_type,
     uint8_t *record_entry_value,
     size_t record_entry_value_size,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     libcdata_list_t *name_to_id_map_list,
     libpff_internal_name_to_id_map_entry_t *name_to_id_map_entry,
     libpff_table_t *record_entry_values_table,
     int debug_item_type LIBPFF_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libfcache_cache_t *value_data_cache                     = NULL;
	libfdata_list_t *value_data_list                        = NULL;
	libpff_local_descriptor_value_t *local_descriptor_value = NULL;
	libpff_internal_record_entry_t *record_entry            = NULL;
	libpff_internal_record_entry_t *value_record_entry      = NULL;
	libpff_table_index_value_t *table_index_value           = NULL;
	uint8_t *record_entry_value_data                        = NULL;
	static char *function                                   = "libpff_table_read_entry_value";
	size_t record_entry_value_data_size                     = 0;
	uint64_t entry_value                                    = 0;
	uint16_t table_index_array_reference                    = 0;
	uint16_t table_index_value_reference                    = 0;
	int result                                              = 0;

	LIBPFF_UNREFERENCED_PARAMETER( debug_item_type )

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( record_entry_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry value.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( ( io_handle->file_type != LIBPFF_FILE_TYPE_32BIT )
	 && ( io_handle->file_type != LIBPFF_FILE_TYPE_64BIT )
	 && ( io_handle->file_type != LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file type.",
		 function );

		return( -1 );
	}
	if( libpff_table_get_record_entry_by_index(
	     table,
	     set_index,
	     entry_index,
	     (libpff_record_entry_t **) &record_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record entry with set index: %d and entry index: %d.",
		 function,
		 set_index,
		 entry_index );

		goto on_error;
	}
	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing record entry with set index: %d and entry index: %d.",
		 function,
		 set_index,
		 entry_index );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table set: %03" PRIu32 " entry: %03" PRIu32 " record entry type\t\t\t: 0x%04" PRIx32 "",
		 function,
		 set_index,
		 entry_index,
		 record_entry_type );
	}
#endif
	if( ( record_entry_type >= 0x8000 )
	 || ( record_entry_type <= 0xfffe ) )
	{
		/* The corresponding name to id map entry was already determined
		 */
		if( name_to_id_map_entry != NULL )
		{
			record_entry->name_to_id_map_entry = name_to_id_map_entry;
		}
		else if( name_to_id_map_list != NULL )
		{
			result = libpff_name_to_id_map_entry_get_entry_by_identifier(
			          name_to_id_map_list,
			          record_entry_type,
			          &( record_entry->name_to_id_map_entry ),
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve name to id map entry: %" PRIu32 ".",
				 function,
				 record_entry_type );

				goto on_error;
			}
		}
	}
	record_entry->identifier.format     = LIBPFF_RECORD_ENTRY_IDENTIFIER_FORMAT_MAPI_PROPERTY;
	record_entry->identifier.entry_type = record_entry_type;
	record_entry->identifier.value_type = record_entry_value_type;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( debug_item_type == LIBPFF_DEBUG_ITEM_TYPE_NAME_TO_ID_MAP )
		{
			libcnotify_printf(
			 " (%s : %s)\n",
			 libpff_debug_get_name_to_id_map_property_type_identifier(
			  record_entry_type,
			  record_entry_value_type ),
			 libpff_debug_get_name_to_id_map_property_type_description(
			  record_entry_type,
			  record_entry_value_type ) );
		}
		else if( record_entry->name_to_id_map_entry != NULL )
		{
			if( record_entry->name_to_id_map_entry->type == LIBPFF_NAME_TO_ID_MAP_ENTRY_TYPE_STRING )
			{
				libcnotify_printf(
				 " maps to: %s (%s : %s)\n",
				 (char *) record_entry->name_to_id_map_entry->debug_string,
				 libfmapi_named_property_type_get_identifier(
				  record_entry->name_to_id_map_entry->guid,
				  (char *) record_entry->name_to_id_map_entry->debug_string,
				  record_entry->name_to_id_map_entry->value_size,
				  record_entry->identifier.value_type ),
				 libfmapi_named_property_type_get_description(
				  record_entry->name_to_id_map_entry->guid,
				  (char *) record_entry->name_to_id_map_entry->debug_string,
				  record_entry->name_to_id_map_entry->value_size,
				  record_entry->identifier.value_type ) );
			}
			else
			{
				libcnotify_printf(
				 " maps to: 0x%04" PRIx32 " (%s : %s)\n",
				 record_entry->name_to_id_map_entry->numeric_value,
				 libfmapi_property_type_get_identifier(
				  record_entry->name_to_id_map_entry->guid,
				  record_entry->name_to_id_map_entry->numeric_value,
				  record_entry->identifier.value_type ),
				 libfmapi_property_type_get_description(
				  record_entry->name_to_id_map_entry->guid,
				  record_entry->name_to_id_map_entry->numeric_value,
				  record_entry->identifier.value_type ) );
			}
		}
		else
		{
			libcnotify_printf(
			 " (%s : %s)\n",
			 libfmapi_property_type_get_identifier(
			  NULL,
			  record_entry->identifier.entry_type,
			  record_entry->identifier.value_type ),
			 libfmapi_property_type_get_description(
			  NULL,
			  record_entry->identifier.entry_type,
			  record_entry->identifier.value_type ) );
		}
		libcnotify_printf(
		 "%s: table set: %03" PRIu32 " entry: %03" PRIu32 " record entry value type\t\t: 0x%04" PRIx16 " (%s : %s)\n",
		 function,
		 set_index,
		 entry_index,
		 record_entry_value_type,
		 libfmapi_value_type_get_identifier(
		  record_entry->identifier.value_type ),
		 libfmapi_value_type_get_description(
		  record_entry->identifier.value_type ) );
	}
#endif
	if( record_entry_value_size == 1 )
	{
		entry_value = record_entry_value[ 0 ];
	}
	else if( record_entry_value_size == 2 )
	{
		byte_stream_copy_to_uint16_little_endian(
		 record_entry_value,
		 entry_value );
	}
	else if( record_entry_value_size == 4 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 record_entry_value,
		 entry_value );
	}
	else if( record_entry_value_size == 8 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 record_entry_value,
		 entry_value );
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported record entry value size: %" PRIu8 ".",
		 function,
		 record_entry_value_size );

		goto on_error;
	}
	/* Check if there is a record entry values (a5) table
	 */
	if( record_entry_values_table != NULL )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: table set: %03" PRIu32 " entry: %03" PRIu32 " record entry values table reference\t: 0x%08" PRIx64 "\n",
			 function,
			 set_index,
			 entry_index,
			 entry_value );
		}
#endif
/* TODO check entry value type */
		if( ( entry_value & 0x0000001fUL ) != 0 )
		{
			result = libpff_local_descriptors_tree_get_value_by_identifier(
				  table->local_descriptors_tree,
				  file_io_handle,
				  table->local_descriptors_cache,
				  (uint32_t) entry_value,
				  &local_descriptor_value,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve descriptor identifier: %" PRIu32 " from local descriptors.",
				 function,
				 (uint32_t) entry_value );

				goto on_error;
			}
			else if( result == 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: missing descriptor: %" PRIu32 " - marked as missing.\n",
					 function,
					 (uint32_t) entry_value );
				}
#endif
				record_entry->flags |= LIBPFF_RECORD_ENTRY_FLAG_MISSING_DATA_DESCRIPTOR;
				table->flags        |= LIBPFF_TABLE_FLAG_MISSING_RECORD_ENTRY_DATA;
			}
			else
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
					 "%s: identifier: %" PRIu64 " (%s), data: %" PRIu64 ", local descriptors: %" PRIu64 "\n",
					 function,
					 local_descriptor_value->identifier,
					 libpff_debug_get_node_identifier_type(
					  (uint8_t) ( local_descriptor_value->identifier & 0x0000001fUL ) ),
					 local_descriptor_value->data_identifier,
					 local_descriptor_value->local_descriptors_identifier );
				}
#endif
/* TODO handle multiple recovered offset index values */
				if( libpff_io_handle_read_descriptor_data_list(
				     io_handle,
				     file_io_handle,
				     offsets_index,
				     (uint32_t) entry_value,
				     local_descriptor_value->data_identifier,
				     table->recovered,
				     0,
				     &value_data_list,
				     &value_data_cache,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read record entry value data with descriptor: %" PRIu32 " - marked as missing.",
					 function,
					 (uint32_t) entry_value );

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

					/* If the data descriptor could not be read mark it as missing
					 * and give it an empty value data reference
					 */
					record_entry->flags |= LIBPFF_RECORD_ENTRY_FLAG_MISSING_DATA_DESCRIPTOR;
					table->flags        |= LIBPFF_TABLE_FLAG_MISSING_RECORD_ENTRY_DATA;
				}
			}
		}
		/* Check if the entry value is a value within the record entry values (a5) table
		 */
		else if( entry_value > 0 )
		{
			if( ( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
			 || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT ) )
			{
				table_index_array_reference = (uint16_t) ( entry_value >> 16 );
				table_index_value_reference = (uint16_t) ( ( entry_value & 0x0000ffe0 ) >> 5 ) - 1;
			}
			else
			{
				table_index_array_reference = (uint16_t) ( entry_value >> 19 );
				table_index_value_reference = (uint16_t) ( ( entry_value & 0x0007ffe0 ) >> 5 ) - 1;
			}
			/* The record entry value reference needs to be transformed into a table set and entry
			 * value. Table array entries have been stored as separate sets.
			 */
			result = libpff_table_get_record_entry_by_index(
				  record_entry_values_table,
				  (int) table_index_array_reference,
				  (int) table_index_value_reference,
				  (libpff_record_entry_t **) &value_record_entry,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve record entry value index: 0x%08" PRIx64 " (set: %" PRIu16 ", entry: %" PRIu16 ")",
				 function,
				 entry_value,
				 table_index_array_reference,
				 table_index_value_reference );

				goto on_error;
			}
			/* A missing record entry value reference signifies an empty value (NULL)
			 */
			else if( result != 0 )
			{
				if( libpff_record_entry_get_data_size(
				     (libpff_record_entry_t *) value_record_entry,
				     &record_entry_value_data_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve record entry value data size.",
					 function );

					goto on_error;
				}
				if( record_entry_value_data_size == 0 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing value record entry value data.",
					 function );

					goto on_error;
				}
			}
		}
	}
	else
	{
		/* The Boolean (0x000b)
		 * is 1 byte of size in the 7c table
		 * is 4 bytes of size in the bc table
		 *
		 * the first byte contains the value
		 * the value is 0x00 if false or true otherwise
		 */
		if( record_entry_value_type == LIBPFF_VALUE_TYPE_BOOLEAN )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: table set: %03" PRIu32 " entry: %03" PRIu32 " record entry value\t\t\t: 0x%08" PRIx64 "\n",
				 function,
				 set_index,
				 entry_index,
				 entry_value );

				if( ( record_entry_value_size != 1 )
				 && ( record_entry_value_size != 4 ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported value type: 0x%08" PRIx32 " with value size: %" PRIu8 ".",
					 function,
					 record_entry_value_type,
					 record_entry_value_size );

					goto on_error;
				}
			}
#endif
			/* The first byte in the record entry value buffer
			 * contains the actual value of the boolean
			 */
			record_entry_value_data      = record_entry_value;
			record_entry_value_data_size = sizeof( uint8_t );
		}
		/* The Integer 16-bit signed (0x0002)
		 * is 2 bytes of size in the 7c table
		 * is 4 bytes of size in the bc table
		 *
		 * the first two bytes contains the value
		 */
		else if( record_entry_value_type == LIBPFF_VALUE_TYPE_INTEGER_16BIT_SIGNED )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: table set: %03" PRIu32 " entry: %03" PRIu32 " record entry value\t\t\t: 0x%08" PRIx64 "\n",
				 function,
				 set_index,
				 entry_index,
				 entry_value );

				if( ( record_entry_value_size != 2 )
				 && ( record_entry_value_size != 4 ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported value type: 0x%08" PRIx32 " with value size: %" PRIu8 ".",
					 function,
					 record_entry_value_type,
					 record_entry_value_size );

					goto on_error;
				}
			}
#endif
			record_entry_value_data      = record_entry_value;
			record_entry_value_data_size = sizeof( uint16_t );
		}
		/* The Integer 32-bit signed (0x0003)
		 *     Floating point single precision (0x0004)
		 *     Error scode (0x000a)
		 *
		 * is 4 bytes of size in the 7c and bc table
		 */
		else if( ( record_entry_value_type == LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_FLOAT_32BIT )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_ERROR ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: table set: %03" PRIu32 " entry: %03" PRIu32 " record entry value\t\t\t: 0x%08" PRIx64 "\n",
				 function,
				 set_index,
				 entry_index,
				 entry_value );

				if( record_entry_value_size != 4 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: unsupported value type: 0x%08" PRIx32 " with value size: %" PRIu8 ".",
					 function,
					 record_entry_value_type,
					 record_entry_value_size );

					goto on_error;
				}
			}
#endif
			record_entry_value_data      = record_entry_value;
			record_entry_value_data_size = sizeof( uint32_t );
		}
		/* The Floating point double precision (0x0005)
		 *     Currency (64-bit) (0x0006)
		 *     Application time (64-bit) (0x0007)
		 *     Integer 64-bit signed (0x0014)
		 *     Windows Filetime (64-bit) (0x0040)
		 *
		 * is 8 bytes of size in the 7c table
		 */
		else if( ( record_entry_value_size == 8 )
		      && ( ( record_entry_value_type == LIBPFF_VALUE_TYPE_DOUBLE_64BIT )
		       || ( record_entry_value_type == LIBPFF_VALUE_TYPE_CURRENCY )
		       || ( record_entry_value_type == LIBPFF_VALUE_TYPE_APPLICATION_TIME )
		       || ( record_entry_value_type == LIBPFF_VALUE_TYPE_INTEGER_64BIT_SIGNED )
		       || ( record_entry_value_type == LIBPFF_VALUE_TYPE_FILETIME ) ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: table set: %03" PRIu32 " entry: %03" PRIu32 " record entry value\t\t\t: 0x%08" PRIx64 "\n",
				 function,
				 set_index,
				 entry_index,
				 entry_value );
			}
#endif
			record_entry_value_data      = record_entry_value;
			record_entry_value_data_size = sizeof( uint64_t );
		}
		/* These values are references in the bc table
		 */
		else if( ( record_entry_value_type == LIBPFF_VALUE_TYPE_DOUBLE_64BIT )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_CURRENCY )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_APPLICATION_TIME )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_OBJECT )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_INTEGER_64BIT_SIGNED )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_STRING_UNICODE )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_FILETIME )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_GUID )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_SERVER_IDENTIFIER )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_RESTRICTION )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_RULE_ACTION )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_BINARY_DATA )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_INTEGER_16BIT_SIGNED )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_INTEGER_32BIT_SIGNED )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_FLOAT_32BIT )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_DOUBLE_64BIT )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_CURRENCY )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_APPLICATION_TIME )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_INTEGER_64BIT_SIGNED )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_STRING_ASCII )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_STRING_UNICODE )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_FILETIME )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_GUID )
		      || ( record_entry_value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_BINARY_DATA ) )
		{
			/* Check if the entry value is a referenced local descriptor
			 */
/* TODO check entry value type */
			if( ( entry_value & 0x0000001fUL ) != 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: table set: %03" PRIu32 " entry: %03" PRIu32 " record entry value reference\t\t: %" PRIu64 " (%s)\n",
					 function,
					 set_index,
					 entry_index,
					 entry_value,
					 libpff_debug_get_node_identifier_type(
					  (uint8_t) ( entry_value & 0x0000001fUL ) ) );
				}
#endif
				if( entry_value > (uint64_t) UINT32_MAX )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
					 "%s: entry value reference value exceeds maximum.",
					 function );

					goto on_error;
				}
				result = libpff_local_descriptors_tree_get_value_by_identifier(
					  table->local_descriptors_tree,
					  file_io_handle,
					  table->local_descriptors_cache,
					  (uint32_t) entry_value,
					  &local_descriptor_value,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve descriptor identifier: %" PRIu32 " from local descriptors.",
					 function,
					 (uint32_t) entry_value );

					goto on_error;
				}
				else if( result == 0 )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: missing descriptor: %" PRIu32 " - marked as missing.\n",
						 function,
						 (uint32_t) entry_value );
					}
#endif
					record_entry->flags |= LIBPFF_RECORD_ENTRY_FLAG_MISSING_DATA_DESCRIPTOR;
					table->flags        |= LIBPFF_TABLE_FLAG_MISSING_RECORD_ENTRY_DATA;
				}
				else
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
						 "%s: identifier: %" PRIu64 " (%s), data: %" PRIu64 ", local descriptors: %" PRIu64 "\n",
						 function,
						 local_descriptor_value->identifier,
						 libpff_debug_get_node_identifier_type(
						  (uint8_t) ( local_descriptor_value->identifier & 0x0000001fUL ) ),
						 local_descriptor_value->data_identifier,
						 local_descriptor_value->local_descriptors_identifier );
					}
#endif
/* TODO handle multiple recovered offset index values */
					if( libpff_io_handle_read_descriptor_data_list(
					     io_handle,
					     file_io_handle,
					     offsets_index,
					     (uint32_t) entry_value,
					     local_descriptor_value->data_identifier,
					     table->recovered,
					     0,
					     &value_data_list,
					     &value_data_cache,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_READ_FAILED,
						 "%s: unable to read record entry value data with descriptor: %" PRIu32 " - marked as missing.",
						 function,
						 (uint32_t) entry_value );

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

						/* If the data descriptor could not be read mark it as missing
						 * and give it an empty value data reference
						 */
						record_entry->flags |= LIBPFF_RECORD_ENTRY_FLAG_MISSING_DATA_DESCRIPTOR;
						table->flags        |= LIBPFF_TABLE_FLAG_MISSING_RECORD_ENTRY_DATA;
					}
				}
			}
			/* Check if the entry value is empty
			 */
			else if( entry_value == 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: table set: %03" PRIu32 " entry: %03" PRIu32 " record entry value\t\t\t: <NULL>\n",
					 function,
					 set_index,
					 entry_index );
				}
#endif
			}
			/* Otherwise the entry value is a referenced table value
			 */
			else
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: table set: %03" PRIu32 " entry: %03" PRIu32 " record entry value (reference)\t\t: 0x%08" PRIx64 "\n",
					 function,
					 set_index,
					 entry_index,
					 entry_value );
					libcnotify_printf(
					 "\n" );
				}
#endif
				/* Fetch the record entry value reference
				 */
				result = libpff_table_get_index_value_by_reference(
					  table,
					  (uint32_t) entry_value,
					  io_handle,
					  &table_index_value,
					  error );

				if( result != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve record entry value reference: 0x%08" PRIx64 ".",
					 function,
					 entry_value );

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

					record_entry->flags |= LIBPFF_RECORD_ENTRY_FLAG_MISSING_DATA_DESCRIPTOR;
					table->flags        |= LIBPFF_TABLE_FLAG_MISSING_RECORD_ENTRY_DATA;
				}
				else
				{
					if( libpff_table_get_value_data_by_index_value(
					     table,
					     table_index_value,
					     file_io_handle,
					     &record_entry_value_data,
					     &record_entry_value_data_size,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve table value data by index value.",
						 function );

						goto on_error;
					}
				}
			}
		}
		else
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: table set: %03" PRIu32 " entry: %03" PRIu32 " record entry value (reference)\t\t: 0x%08" PRIx64 "\n",
				 function,
				 set_index,
				 entry_index,
				 entry_value );
				libcnotify_printf(
				 "\n" );
			}
#endif
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported value type: 0x%08" PRIx32 " with value size: %" PRIu8 ".",
			 function,
			 record_entry_value_type,
			 record_entry_value_size );

			goto on_error;
		}
	}
/* TODO is this check necessary do entry values get read more than once ? */
	if( record_entry->value_data == NULL )
	{
		if( value_data_list != NULL )
		{
			result = libpff_record_entry_set_value_data_from_list(
			          (libpff_record_entry_t *) record_entry,
			          file_io_handle,
			          value_data_list,
			          value_data_cache,
			          error );
		}
		else
		{
			result = libpff_record_entry_set_value_data(
			          (libpff_record_entry_t *) record_entry,
			          record_entry_value_data,
			          record_entry_value_data_size,
			          error );
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set value data in record entry.",
			 function );

			goto on_error;
		}
	}
	if( value_data_cache != NULL )
	{
		if( libfcache_cache_free(
		     &value_data_cache,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free value data cache.",
			 function );

			goto on_error;
		}
	}
	if( value_data_list != NULL )
	{
		if( libfdata_list_free(
		     &value_data_list,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free value data list.",
			 function );

			goto on_error;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libpff_debug_print_record_entry(
		     (libpff_record_entry_t *) record_entry,
		     name_to_id_map_list,
		     debug_item_type,
		     io_handle->ascii_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print record entry value reference.",
			 function );

			goto on_error;
		}
	}
#endif
	return( 1 );

on_error:
	if( value_data_cache != NULL )
	{
		libfcache_cache_free(
		 &value_data_cache,
		 NULL );
	}
	if( value_data_list != NULL )
	{
		libfdata_list_free(
		 &value_data_list,
		 NULL );
	}
	return( -1 );
}

