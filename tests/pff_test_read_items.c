/*
 * Library read item testing program
 *
 * Copyright (C) 2008-2016, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>

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
	libpff_item_t *sub_item  = NULL;
	size_t value_string_size = 0;
	int number_of_sub_items  = 0;
	int result               = 0;
	int sub_item_index       = 0;

	if( item == NULL )
	{
		fprintf(
		 stderr,
		 "Invalid item.\n" );

		return( -1 );
	}
	result = libpff_item_get_utf8_display_name_size(
	          item,
	          &value_string_size,
	          error );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve display name size.\n" );

		return( -1 );
	}
	if( libpff_item_get_number_of_sub_items(
	     item,
	     &number_of_sub_items,
	     error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve number of sub items.\n" );

		return( -1 );
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
	return( -1 );
}

int main( int argc, char * const argv[] )
{
	libpff_error_t *error         = NULL;
	libpff_file_t *file           = NULL;
	libpff_item_t *item           = NULL;
	int number_of_recovered_items = 0;
	int result                    = 0;

	if( argc < 2 )
	{
		fprintf(
		 stderr,
		 "Missing filename.\n" );

		return( EXIT_FAILURE );
	}
	if( libpff_file_initialize(
	     &file,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create file.\n" );

		goto on_error;
	}
	if( libpff_file_open(
	     file,
	     argv[ 1 ],
	     LIBPFF_OPEN_READ,
	     &error ) != 1 )
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
	if( file != NULL )
	{
		libpff_file_close(
		 file,
		 NULL );
		libpff_file_free(
		 &file,
		 NULL );
	}
	return( EXIT_FAILURE );
}

