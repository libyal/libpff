/*
 * Library read item testing program
 *
 * Copyright (C) 2008-2026, Joachim Metz <joachim.metz@gmail.com>
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
#include "pff_test_macros.h"

/* Define to make pff_test_read_items generate verbose output
#define PFF_TEST_READ_ITEMS_VERBOSE
 */

int pff_test_read_items(
     libpff_item_t *item )
{
	libpff_error_t *error               = NULL;
	libpff_item_t *sub_item             = NULL;
	libpff_record_entry_t *record_entry = NULL;
	libpff_record_set_t *record_set     = NULL;
	size_t value_string_size            = 0;
	int number_of_sub_items             = 0;
	int result                          = 0;
	int sub_item_index                  = 0;

	result = libpff_item_get_record_set_by_index(
	          item,
	          0,
	          &record_set,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "record_set",
	 record_set );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_record_set_get_entry_by_type(
	          record_set,
	          LIBPFF_ENTRY_TYPE_DISPLAY_NAME,
	          0,
	          &record_entry,
	          LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
	          &error );

	PFF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "record_entry",
	 record_entry );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( result != 0 )
	{
		result = libpff_record_entry_get_data_as_utf8_string_size(
		          record_entry,
		          &value_string_size,
		          &error );

		PFF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		PFF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	result = libpff_record_entry_free(
	          &record_entry,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "record_entry",
	 record_entry );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	result = libpff_record_set_free(
	          &record_set,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "record_set",
	 record_set );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	result = libpff_item_get_number_of_sub_items(
	          item,
	          &number_of_sub_items,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	for( sub_item_index = 0;
	     sub_item_index < number_of_sub_items;
	     sub_item_index++ )
	{
		result = libpff_item_get_sub_item(
		          item,
		          sub_item_index,
		          &sub_item,
		          &error );

		PFF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		PFF_TEST_ASSERT_IS_NOT_NULL(
		 "sub_item",
		 sub_item );

		PFF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		result = pff_test_read_items(
		          sub_item );

		PFF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		result = libpff_item_free(
		          &sub_item,
		          &error );

		PFF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		PFF_TEST_ASSERT_IS_NULL(
		 "sub_item",
		 sub_item );

		PFF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
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

	result = libpff_file_initialize(
	          &file,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "file",
	 file );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

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
	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_file_get_root_folder(
	          file,
	          &item,
	          &error );

	PFF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( result != 0 )
	{
		PFF_TEST_ASSERT_IS_NOT_NULL(
		 "item",
		 item );

		result = pff_test_read_items(
		          item );

		PFF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		result = libpff_item_free(
		          &item,
		          &error );

		PFF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		PFF_TEST_ASSERT_IS_NULL(
		 "item",
		 item );

		PFF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	result = libpff_file_recover_items(
	          file,
	          0,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_file_get_number_of_recovered_items(
	          file,
	          &number_of_recovered_items,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_file_close(
	          file,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_file_free(
	          &file,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "file",
	 file );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

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

