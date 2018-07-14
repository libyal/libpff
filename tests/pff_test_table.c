/*
 * Library table type test program
 *
 * Copyright (C) 2008-2018, Joachim Metz <joachim.metz@gmail.com>
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
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "pff_test_libcerror.h"
#include "pff_test_libpff.h"
#include "pff_test_macros.h"
#include "pff_test_memory.h"
#include "pff_test_unused.h"

#include "../libpff/libpff_table.h"

uint8_t pff_test_table_header_data[ 12 ] = {
	0x64, 0x1c, 0xec, 0xbc, 0x20, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00 };

uint8_t pff_test_table_6c_header_data[ 8 ] = {
	0x40, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00 };

uint8_t pff_test_table_7c_header_data[ 22 ] = {
	0x7c, 0x0f, 0x40, 0x00, 0x40, 0x00, 0x41, 0x00, 0x43, 0x00, 0x20, 0x00, 0x00, 0x00, 0x80, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

uint8_t pff_test_table_9c_header_data[ 4 ] = {
	0x40, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

/* Tests the libpff_table_initialize function
 * Returns 1 if successful or 0 if not
 */
int pff_test_table_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libpff_table_t *table           = NULL;
	int result                      = 0;

#if defined( HAVE_PFF_TEST_MEMORY )
	int number_of_malloc_fail_tests = 5;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libpff_table_initialize(
	          &table,
	          0,
	          0,
	          0,
	          0,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "table",
	 table );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_table_free(
	          &table,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "table",
	 table );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libpff_table_initialize(
	          NULL,
	          0,
	          0,
	          0,
	          0,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	table = (libpff_table_t *) 0x12345678UL;

	result = libpff_table_initialize(
	          &table,
	          0,
	          0,
	          0,
	          0,
	          &error );

	table = NULL;

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_PFF_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libpff_table_initialize with malloc failing
		 * Test libpff_table_initialize with malloc failing in libcdata_array_initialize (4x)
		 */
		pff_test_malloc_attempts_before_fail = test_number;

		result = libpff_table_initialize(
		          &table,
		          0,
		          0,
		          0,
		          0,
		          &error );

		if( pff_test_malloc_attempts_before_fail != -1 )
		{
			pff_test_malloc_attempts_before_fail = -1;

			if( table != NULL )
			{
				libpff_table_free(
				 &table,
				 NULL );
			}
		}
		else
		{
			PFF_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			PFF_TEST_ASSERT_IS_NULL(
			 "table",
			 table );

			PFF_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libpff_table_initialize with memset failing
		 */
		pff_test_memset_attempts_before_fail = test_number;

		result = libpff_table_initialize(
		          &table,
		          0,
		          0,
		          0,
		          0,
		          &error );

		if( pff_test_memset_attempts_before_fail != -1 )
		{
			pff_test_memset_attempts_before_fail = -1;

			if( table != NULL )
			{
				libpff_table_free(
				 &table,
				 NULL );
			}
		}
		else
		{
			PFF_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			PFF_TEST_ASSERT_IS_NULL(
			 "table",
			 table );

			PFF_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_PFF_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( table != NULL )
	{
		libpff_table_free(
		 &table,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_table_free function
 * Returns 1 if successful or 0 if not
 */
int pff_test_table_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libpff_table_free(
	          NULL,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libpff_table_clone function
 * Returns 1 if successful or 0 if not
 */
int pff_test_table_clone(
     void )
{
	libcerror_error_t *error          = NULL;
	libpff_table_t *destination_table = NULL;
	libpff_table_t *source_table      = NULL;
	int result                        = 0;

	/* Initialize test
	 */
	result = libpff_table_initialize(
	          &source_table,
	          0,
	          0,
	          0,
	          0,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "source_table",
	 source_table );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libpff_table_clone(
	          &destination_table,
	          source_table,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "destination_table",
	 destination_table );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_table_free(
	          &destination_table,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "destination_table",
	 destination_table );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_table_clone(
	          &destination_table,
	          NULL,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "destination_table",
	 destination_table );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libpff_table_clone(
	          NULL,
	          source_table,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libpff_table_free(
	          &source_table,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "source_table",
	 source_table );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( destination_table != NULL )
	{
		libpff_table_free(
		 &destination_table,
		 NULL );
	}
	if( source_table != NULL )
	{
		libpff_table_free(
		 &source_table,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_table_resize_record_entries function
 * Returns 1 if successful or 0 if not
 */
int pff_test_table_resize_record_entries(
     void )
{
	libcerror_error_t *error = NULL;
	libpff_table_t *table    = NULL;
	int result               = 0;

	/* Initialize test
	 */
	result = libpff_table_initialize(
	          &table,
	          0,
	          0,
	          0,
	          0,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "table",
	 table );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libpff_table_resize_record_entries(
	          table,
	          10,
	          10,
	          LIBPFF_CODEPAGE_WINDOWS_1251,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libpff_table_resize_record_entries(
	          NULL,
	          10,
	          10,
	          LIBPFF_CODEPAGE_WINDOWS_1251,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libpff_table_resize_record_entries(
	          table,
	          -1,
	          10,
	          LIBPFF_CODEPAGE_WINDOWS_1251,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libpff_table_resize_record_entries(
	          table,
	          10,
	          -1,
	          LIBPFF_CODEPAGE_WINDOWS_1251,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libpff_table_free(
	          &table,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "table",
	 table );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( table != NULL )
	{
		libpff_table_free(
		 &table,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_table_expand_record_entries function
 * Returns 1 if successful or 0 if not
 */
int pff_test_table_expand_record_entries(
     void )
{
	libcerror_error_t *error = NULL;
	libpff_table_t *table    = NULL;
	int result               = 0;

	/* Initialize test
	 */
	result = libpff_table_initialize(
	          &table,
	          0,
	          0,
	          0,
	          0,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "table",
	 table );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libpff_table_expand_record_entries(
	          table,
	          10,
	          10,
	          LIBPFF_CODEPAGE_WINDOWS_1251,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libpff_table_expand_record_entries(
	          NULL,
	          10,
	          10,
	          LIBPFF_CODEPAGE_WINDOWS_1251,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libpff_table_expand_record_entries(
	          table,
	          -1,
	          10,
	          LIBPFF_CODEPAGE_WINDOWS_1251,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libpff_table_expand_record_entries(
	          table,
	          10,
	          -1,
	          LIBPFF_CODEPAGE_WINDOWS_1251,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libpff_table_free(
	          &table,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "table",
	 table );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( table != NULL )
	{
		libpff_table_free(
		 &table,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_table_read_header_data function
 * Returns 1 if successful or 0 if not
 */
int pff_test_table_read_header_data(
     void )
{
	libcerror_error_t *error       = NULL;
	libpff_table_t *table          = NULL;
	uint32_t table_value_reference = 0;
	int result                     = 0;

	/* Initialize test
	 */
	result = libpff_table_initialize(
	          &table,
	          0,
	          0,
	          0,
	          0,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "table",
	 table );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libpff_table_read_header_data(
	          table,
	          pff_test_table_header_data,
	          12,
	          &table_value_reference,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libpff_table_read_header_data(
	          NULL,
	          pff_test_table_header_data,
	          12,
	          &table_value_reference,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libpff_table_read_header_data(
	          table,
	          NULL,
	          12,
	          &table_value_reference,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libpff_table_read_header_data(
	          table,
	          pff_test_table_header_data,
	          (size_t) SSIZE_MAX + 1,
	          &table_value_reference,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libpff_table_read_header_data(
	          table,
	          pff_test_table_header_data,
	          12,
	          NULL,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libpff_table_free(
	          &table,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "table",
	 table );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( table != NULL )
	{
		libpff_table_free(
		 &table,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_table_read_6c_header_data function
 * Returns 1 if successful or 0 if not
 */
int pff_test_table_read_6c_header_data(
     void )
{
	libcerror_error_t *error           = NULL;
	libpff_table_t *table              = NULL;
	uint32_t b5_table_header_reference = 0;
	uint32_t values_array_reference    = 0;
	int result                         = 0;

	/* Initialize test
	 */
	result = libpff_table_initialize(
	          &table,
	          0,
	          0,
	          0,
	          0,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "table",
	 table );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libpff_table_read_6c_header_data(
	          table,
	          pff_test_table_6c_header_data,
	          8,
	          &b5_table_header_reference,
	          &values_array_reference,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libpff_table_read_6c_header_data(
	          NULL,
	          pff_test_table_6c_header_data,
	          8,
	          &b5_table_header_reference,
	          &values_array_reference,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libpff_table_read_6c_header_data(
	          table,
	          NULL,
	          8,
	          &b5_table_header_reference,
	          &values_array_reference,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libpff_table_read_6c_header_data(
	          table,
	          pff_test_table_6c_header_data,
	          (size_t) SSIZE_MAX + 1,
	          &b5_table_header_reference,
	          &values_array_reference,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libpff_table_read_6c_header_data(
	          table,
	          pff_test_table_6c_header_data,
	          8,
	          NULL,
	          &values_array_reference,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libpff_table_read_6c_header_data(
	          table,
	          pff_test_table_6c_header_data,
	          8,
	          &b5_table_header_reference,
	          NULL,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libpff_table_free(
	          &table,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "table",
	 table );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( table != NULL )
	{
		libpff_table_free(
		 &table,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_table_read_7c_header_data function
 * Returns 1 if successful or 0 if not
 */
int pff_test_table_read_7c_header_data(
     void )
{
	libcerror_error_t *error           = NULL;
	libpff_table_t *table              = NULL;
	uint32_t b5_table_header_reference = 0;
	uint32_t values_array_reference    = 0;
	uint16_t values_array_entry_size   = 0;
	int number_of_column_definitions   = 0;
	int result                         = 0;

	/* Initialize test
	 */
	result = libpff_table_initialize(
	          &table,
	          0,
	          0,
	          0,
	          0,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "table",
	 table );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libpff_table_read_7c_header_data(
	          table,
	          pff_test_table_7c_header_data,
	          22,
	          &b5_table_header_reference,
	          &values_array_reference,
	          &values_array_entry_size,
	          &number_of_column_definitions,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libpff_table_read_7c_header_data(
	          NULL,
	          pff_test_table_7c_header_data,
	          22,
	          &b5_table_header_reference,
	          &values_array_reference,
	          &values_array_entry_size,
	          &number_of_column_definitions,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libpff_table_read_7c_header_data(
	          table,
	          NULL,
	          22,
	          &b5_table_header_reference,
	          &values_array_reference,
	          &values_array_entry_size,
	          &number_of_column_definitions,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libpff_table_read_7c_header_data(
	          table,
	          pff_test_table_7c_header_data,
	          (size_t) SSIZE_MAX + 1,
	          &b5_table_header_reference,
	          &values_array_reference,
	          &values_array_entry_size,
	          &number_of_column_definitions,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libpff_table_read_7c_header_data(
	          table,
	          pff_test_table_7c_header_data,
	          22,
	          NULL,
	          &values_array_reference,
	          &values_array_entry_size,
	          &number_of_column_definitions,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libpff_table_read_7c_header_data(
	          table,
	          pff_test_table_7c_header_data,
	          22,
	          &b5_table_header_reference,
	          NULL,
	          &values_array_entry_size,
	          &number_of_column_definitions,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libpff_table_read_7c_header_data(
	          table,
	          pff_test_table_7c_header_data,
	          22,
	          &b5_table_header_reference,
	          &values_array_reference,
	          NULL,
	          &number_of_column_definitions,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libpff_table_read_7c_header_data(
	          table,
	          pff_test_table_7c_header_data,
	          22,
	          &b5_table_header_reference,
	          &values_array_reference,
	          &values_array_entry_size,
	          NULL,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libpff_table_read_7c_header_data(
	          table,
	          pff_test_table_7c_header_data,
	          0,
	          &b5_table_header_reference,
	          &values_array_reference,
	          &values_array_entry_size,
	          &number_of_column_definitions,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libpff_table_free(
	          &table,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "table",
	 table );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( table != NULL )
	{
		libpff_table_free(
		 &table,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_table_read_9c_header_data function
 * Returns 1 if successful or 0 if not
 */
int pff_test_table_read_9c_header_data(
     void )
{
	libcerror_error_t *error           = NULL;
	libpff_table_t *table              = NULL;
	uint32_t b5_table_header_reference = 0;
	int result                         = 0;

	/* Initialize test
	 */
	result = libpff_table_initialize(
	          &table,
	          0,
	          0,
	          0,
	          0,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "table",
	 table );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libpff_table_read_9c_header_data(
	          table,
	          pff_test_table_9c_header_data,
	          4,
	          &b5_table_header_reference,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libpff_table_read_9c_header_data(
	          NULL,
	          pff_test_table_9c_header_data,
	          4,
	          &b5_table_header_reference,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libpff_table_read_9c_header_data(
	          table,
	          NULL,
	          4,
	          &b5_table_header_reference,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libpff_table_read_9c_header_data(
	          table,
	          pff_test_table_9c_header_data,
	          (size_t) SSIZE_MAX + 1,
	          &b5_table_header_reference,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libpff_table_read_9c_header_data(
	          table,
	          pff_test_table_9c_header_data,
	          4,
	          NULL,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libpff_table_free(
	          &table,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "table",
	 table );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( table != NULL )
	{
		libpff_table_free(
		 &table,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc PFF_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] PFF_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc PFF_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] PFF_TEST_ATTRIBUTE_UNUSED )
#endif
{
	PFF_TEST_UNREFERENCED_PARAMETER( argc )
	PFF_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

	PFF_TEST_RUN(
	 "libpff_table_initialize",
	 pff_test_table_initialize );

	PFF_TEST_RUN(
	 "libpff_table_free",
	 pff_test_table_free );

	PFF_TEST_RUN(
	 "libpff_table_clone",
	 pff_test_table_clone );

	PFF_TEST_RUN(
	 "libpff_table_resize_record_entries",
	 pff_test_table_resize_record_entries );

	PFF_TEST_RUN(
	 "libpff_table_expand_record_entries",
	 pff_test_table_expand_record_entries );

	/* TODO: add tests for libpff_table_entries_get_record_entry_by_index */

	/* TODO: add tests for libpff_table_get_local_descriptors_value_by_identifier */

	/* TODO: add tests for libpff_table_get_index_value_by_reference */

	/* TODO: add tests for libpff_table_get_value_data_by_index_value */

	/* TODO: add tests for libpff_table_get_value_data_by_reference */

	/* TODO: add tests for libpff_table_clone_value_data_by_reference */

	/* TODO: add tests for libpff_table_get_number_of_record_sets */

	/* TODO: add tests for libpff_table_get_record_set_by_index */

	/* TODO: add tests for libpff_table_get_number_of_entries */

	/* TODO: add tests for libpff_table_get_entry_type_by_index */

	/* TODO: add tests for libpff_table_get_record_entry_by_index */

	/* TODO: add tests for libpff_table_get_record_entry_by_type */

	/* TODO: add tests for libpff_table_get_record_entry_by_utf8_name */

	/* TODO: add tests for libpff_table_get_record_entry_by_utf16_name */

	/* TODO: add tests for libpff_table_read */

	/* TODO: add tests for libpff_table_read_index */

	/* TODO: add tests for libpff_table_read_record_entries */

	/* TODO: add tests for libpff_table_read_values */

	/* TODO: add tests for libpff_table_read_6c_values */

	/* TODO: add tests for libpff_table_read_7c_values */

	/* TODO: add tests for libpff_table_read_8c_values */

	/* TODO: add tests for libpff_table_read_9c_values */

	/* TODO: add tests for libpff_table_read_a5_values */

	/* TODO: add tests for libpff_table_read_ac_values */

	/* TODO: add tests for libpff_table_read_bc_values */

	PFF_TEST_RUN(
	 "libpff_table_read_header_data",
	 pff_test_table_read_header_data );

	PFF_TEST_RUN(
	 "libpff_table_read_6c_header_data",
	 pff_test_table_read_6c_header_data );

	PFF_TEST_RUN(
	 "libpff_table_read_7c_header_data",
	 pff_test_table_read_7c_header_data );

	PFF_TEST_RUN(
	 "libpff_table_read_9c_header_data",
	 pff_test_table_read_9c_header_data );

	/* TODO: add tests for libpff_table_read_ac_header_data */

	/* TODO: add tests for libpff_table_read_b5_header */

	/* TODO: add tests for libpff_table_read_6c_record_entries */

	/* TODO: add tests for libpff_table_read_7c_column_definitions */

	/* TODO: add tests for libpff_table_read_8c_record_entries */

	/* TODO: add tests for libpff_table_read_9c_record_entries */

	/* TODO: add tests for libpff_table_read_a5_record_entries */

	/* TODO: add tests for libpff_table_read_ac_column_definitions */

	/* TODO: add tests for libpff_table_read_bc_record_entries */

	/* TODO: add tests for libpff_table_values_array_get_value_data_by_entry_number */

	/* TODO: add tests for libpff_table_read_values_array */

	/* TODO: add tests for libpff_table_read_entry_value */

#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

