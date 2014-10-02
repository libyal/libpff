/*
 * Legacy functions
 *
 * Copyright (c) 2008-2014, Joachim Metz <joachim.metz@gmail.com>
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
#include "libpff_item.h"
#include "libpff_item_values.h"
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

