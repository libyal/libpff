/*
 * Library read item testing program
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
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <stdio.h>

#include "pff_test_libcerror.h"
#include "pff_test_libpff.h"

/* Define to make pff_test_read_items generate verbose output
#define PFF_TEST_READ_ITEMS_VERBOSE
 */

int pff_test_read_items(
     libpff_item_t *item,
     libpff_error_t **error )
{
	libpff_item_t *sub_item             = NULL;
	libpff_record_entry_t *record_entry = NULL;
	libpff_record_set_t *record_set     = NULL;
	size_t value_string_size            = 0;
	int number_of_sub_items             = 0;
	int result                          = 0;
	int sub_item_index                  = 0;

	if( item == NULL )
	{
		fprintf(
		 stderr,
		 "Invalid item.\n" );

		return( -1 );
	}
	if( libpff_item_get_record_set_by_index(
	     item,
	     0,
	     &record_set,
	     error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve record set: 0.\n" );

		goto on_error;
	}
	result = libpff_record_set_get_entry_by_type(
	          record_set,
	          LIBPFF_ENTRY_TYPE_DISPLAY_NAME,
	          0,
	          &record_entry,
	          LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
	          error );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve display name record entry.\n" );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libpff_record_entry_get_data_as_utf8_string_size(
		     record_entry,
		     &value_string_size,
		     error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to retrieve display name size.\n" );

			goto on_error;
		}
	}
	if( libpff_record_entry_free(
	     &record_entry,
	     error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free record entry.\n" );

		goto on_error;
	}
	if( libpff_record_set_free(
	     &record_set,
	     error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free record set.\n" );

		goto on_error;
	}
	if( libpff_item_get_number_of_sub_items(
	     item,
	     &number_of_sub_items,
	     error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve number of sub items.\n" );

		goto on_error;
	}
	for( sub_item_index = 0;
	     sub_item_index < number_of_sub_items;
	     sub_item_index++ )
	{
		if( libpff_item_get_sub_item(
	             item,
		     sub_item_index,
		     &sub_item,
		     error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to retrieve sub item: %d.\n",
			 sub_item_index );

			goto on_error;
		}
		if( pff_test_read_items(
		     sub_item,
		     error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to read sub item: %d.\n",
			 sub_item_index );

			goto on_error;
		}
		if( libpff_item_free(
	             &sub_item,
		     error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to free sub item: %d.\n",
			 sub_item_index );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( sub_item != NULL )
	{
		libpff_item_free(
		 &sub_item,
		 NULL );
	}
	if( record_entry != NULL )
	{
		libpff_record_entry_free(
		 &record_entry,
		 NULL );
	}
	if( record_set != NULL )
	{
		libpff_record_set_free(
		 &record_set,
		 NULL );
	}
	return( -1 );
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc,
     wchar_t * const argv[])
#else
int main(
     int argc,
     char * const argv[])
#endif
{
	libpff_error_t *error         = NULL;
	libpff_file_t *file           = NULL;
	libpff_item_t *item           = NULL;
	system_character_t *source    = NULL;
	int number_of_recovered_items = 0;
	int result                    = 0;

	if( argc < 2 )
	{
		fprintf(
		 stderr,
		 "Missing filename.\n" );

		return( EXIT_FAILURE );
	}
	source = argv[ 1 ];

	if( libpff_file_initialize(
	     &file,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create file.\n" );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libpff_file_open_wide(
	          file,
	          source,
	          LIBPFF_OPEN_READ,
	          &error );
#else
	result = libpff_file_open(
	          file,
	          source,
	          LIBPFF_OPEN_READ,
	          &error );
#endif
	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open: %s.\n",
		 argv[ 1 ] );

		goto on_error;
	}
	result = libpff_file_get_root_folder(
	          file,
	          &item,
	          &error );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve root folder item.\n" );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( pff_test_read_items(
		     item,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to read root folder item.\n" );

			goto on_error;
		}
		if( libpff_item_free(
		     &item,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to free item.\n" );

			goto on_error;
		}
		if( libpff_file_recover_items(
		     file,
		     0,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to recover items.\n" );

			goto on_error;
		}
		if( libpff_file_get_number_of_recovered_items(
		     file,
		     &number_of_recovered_items,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to retrieve number of recover items.\n" );

			goto on_error;
		}
		fprintf(
		 stdout,
		 "Number of recovered items: %d\n",
		 number_of_recovered_items );
	}
	if( libpff_file_close(
	     file,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close file.\n" );

		goto on_error;
	}
	if( libpff_file_free(
	     &file,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free file.\n" );

		goto on_error;
	}
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libpff_error_backtrace_fprint(
		 error,
		 stderr );
		libpff_error_free(
		 &error );
	}
	if( item != NULL )
	{
		libpff_item_free(
		 &item,
		 NULL );
	}
	if( file != NULL )
	{
		libpff_file_free(
		 &file,
		 NULL );
	}
	return( EXIT_FAILURE );
}

