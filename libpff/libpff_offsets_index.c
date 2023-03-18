/*
 * Offsets index functions
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

#include "libpff_definitions.h"
#include "libpff_index.h"
#include "libpff_index_value.h"
#include "libpff_index_values_list.h"
#include "libpff_libbfio.h"
#include "libpff_libcdata.h"
#include "libpff_libcerror.h"
#include "libpff_offsets_index.h"

/* Creates an offsets index
 * Make sure the value offsets_index is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_offsets_index_initialize(
     libpff_offsets_index_t **offsets_index,
     off64_t root_node_offset,
     uint64_t root_node_back_pointer,
     libcerror_error_t **error )
{
	static char *function = "libpff_offsets_index_initialize";

	if( offsets_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offsets index.",
		 function );

		return( -1 );
	}
	if( *offsets_index != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid offsets index value already set.",
		 function );

		return( -1 );
	}
	*offsets_index = memory_allocate_structure(
	                  libpff_offsets_index_t );

	if( *offsets_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create offsets index.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *offsets_index,
	     0,
	     sizeof( libpff_offsets_index_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear offsets index.",
		 function );

		memory_free(
		 *offsets_index );

		*offsets_index = NULL;

		return( -1 );
	}
	if( libpff_index_initialize(
	     &( ( *offsets_index )->index ),
	     LIBPFF_INDEX_TYPE_OFFSET,
	     root_node_offset,
	     root_node_back_pointer,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create index.",
		 function );

		goto on_error;
	}
	if( libcdata_btree_initialize(
	     &( ( *offsets_index )->recovered_index_values_tree ),
	     257,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create recovered index values B-tree.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *offsets_index != NULL )
	{
		if( ( *offsets_index )->recovered_index_values_tree != NULL )
		{
			libcdata_btree_free(
			 &( ( *offsets_index )->recovered_index_values_tree ),
			 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_index_values_list_free,
			 NULL );
		}
		if( ( *offsets_index )->index != NULL )
		{
			libpff_index_free(
			 &( ( *offsets_index )->index ),
			 NULL );
		}
		memory_free(
		 *offsets_index );

		*offsets_index = NULL;
	}
	return( -1 );
}

/* Frees an offsets index
 * Returns 1 if successful or -1 on error
 */
int libpff_offsets_index_free(
     libpff_offsets_index_t **offsets_index,
     libcerror_error_t **error )
{
	static char *function = "libpff_offsets_index_free";
	int result            = 1;

	if( offsets_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offsets index.",
		 function );

		return( -1 );
	}
	if( *offsets_index != NULL )
	{
		if( libcdata_btree_free(
		     &( ( *offsets_index )->recovered_index_values_tree ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_index_values_list_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free recovered index values tree.",
			 function );

			result = -1;
		}
		if( libpff_index_free(
		     &( ( *offsets_index )->index ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free index.",
			 function );

			result = -1;
		}
		memory_free(
		 *offsets_index );

		*offsets_index = NULL;
	}
	return( result );
}

/* Retrieves an index value for a specific identifier
 * Returns 1 if successful, 0 if no index value was found or -1 on error
 */
int libpff_offsets_index_get_index_value_by_identifier(
     libpff_offsets_index_t *offsets_index,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint64_t data_identifier,
     uint8_t recovered,
     int recovered_value_index,
     libpff_index_value_t **index_value,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *upper_node              = NULL;
	libpff_index_value_t *lookup_index_value      = NULL;
	libpff_index_value_t *safe_index_value        = NULL;
	libpff_index_values_list_t *index_values_list = NULL;
	static char *function                         = "libpff_offsets_index_get_index_value_by_identifier";
	uint64_t lookup_data_identifier               = 0;
	int result                                    = 0;

	if( offsets_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offsets index.",
		 function );

		return( -1 );
	}
	lookup_data_identifier = data_identifier & (uint64_t) LIBPFF_OFFSET_INDEX_IDENTIFIER_MASK;

	if( recovered == 0 )
	{
		result = libpff_index_get_value_by_identifier(
			  offsets_index->index,
			  io_handle,
			  file_io_handle,
			  lookup_data_identifier,
			  index_value,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve index value: %" PRIu64 " from index.",
			 function,
			 lookup_data_identifier );

			goto on_error;
		}
	}
	else
	{
		if( libpff_index_value_initialize(
		     &lookup_index_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create lookup index value.",
			 function );

			goto on_error;
		}
		lookup_index_value->identifier = lookup_data_identifier;

		result = libcdata_btree_get_value_by_value(
		          offsets_index->recovered_index_values_tree,
		          (intptr_t *) lookup_index_value,
		          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &libpff_index_values_list_compare,
		          &upper_node,
		          (intptr_t **) &index_values_list,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve recovered index value: %" PRIu64 " list from tree.",
			 function,
			 lookup_data_identifier );

			goto on_error;
		}
		else if( result != 0 )
		{
			if( libpff_index_values_list_get_value_by_index(
			     index_values_list,
			     recovered_value_index,
			     &safe_index_value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve retrieve recovered index value: %" PRIu64 " list entry: %d.",
				 function,
				 lookup_data_identifier,
				 recovered_value_index );

				goto on_error;
			}
			if( safe_index_value == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing recovered index value: %" PRIu64 " list entry: %d.",
				 function,
				 lookup_data_identifier,
				 recovered_value_index );

				goto on_error;
			}
			lookup_index_value->file_offset     = lookup_index_value->file_offset;
			lookup_index_value->data_size       = lookup_index_value->data_size;
			lookup_index_value->reference_count = lookup_index_value->reference_count;

			*index_value = lookup_index_value;
		}
	}
	return( result );

on_error:
	if( lookup_index_value != NULL )
	{
		libpff_index_value_free(
		 &lookup_index_value,
		 NULL );
	}
	return( -1 );
}

/* Inserts a recovered index value into the offsets index
 * Returns 1 if successful or -1 on error
 */
int libpff_offsets_index_insert_recovered_index_value(
     libpff_offsets_index_t *offsets_index,
     libpff_index_value_t *index_value,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *upper_node                       = NULL;
	libpff_index_values_list_t *existing_index_values_list = NULL;
	libpff_index_values_list_t *index_values_list          = NULL;
	static char *function                                  = "libpff_offsets_index_insert_recovered_index_value";
	int result                                             = 0;
	int value_index                                        = 0;

	if( offsets_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offsets index.",
		 function );

		return( -1 );
	}
	if( index_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index value.",
		 function );

		return( -1 );
	}
	if( libpff_index_values_list_initialize(
	     &index_values_list,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create index values list.",
		 function );

		goto on_error;
	}
	index_values_list->identifier = index_value->identifier;

	result = libcdata_btree_insert_value(
	          offsets_index->recovered_index_values_tree,
	          &value_index,
	          (intptr_t *) index_values_list,
	          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &libpff_index_values_list_compare,
	          &upper_node,
	          (intptr_t **) &existing_index_values_list,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to insert recovered offsets index value: %" PRIu64 " list.",
		 function,
		 index_value->identifier );

		goto on_error;
	}
	else if( result != 0 )
	{
		existing_index_values_list = index_values_list;
		index_values_list          = NULL;
	}
	else
	{
		if( libpff_index_values_list_free(
		     &index_values_list,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free index values list.",
			 function );

			goto on_error;
		}
	}
	if( libpff_index_values_list_append_value(
	     existing_index_values_list,
	     index_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append index value to list.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( index_values_list != NULL )
	{
		libpff_index_values_list_free(
		 &index_values_list,
		 NULL );
	}
	return( -1 );
}

