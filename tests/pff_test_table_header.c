/*
 * Library table_header type test program
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

#include "../libpff/libpff_table_header.h"

uint8_t pff_test_table_header_data[ 12 ] = {
	0x64, 0x1c, 0xec, 0xbc, 0x20, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00 };

uint8_t pff_test_table_header_6c_data[ 8 ] = {
	0x40, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00 };

uint8_t pff_test_table_header_7c_data[ 78 ] = {
	0x7c, 0x07, 0x18, 0x00, 0x18, 0x00, 0x19, 0x00, 0x1a, 0x00, 0x20, 0x00, 0x00, 0x00, 0x80, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x01, 0x30, 0x08, 0x00, 0x04, 0x02, 0x03, 0x00,
	0x02, 0x36, 0x0c, 0x00, 0x04, 0x03, 0x03, 0x00, 0x03, 0x36, 0x10, 0x00, 0x04, 0x04, 0x0b, 0x00,
	0x0a, 0x36, 0x18, 0x00, 0x01, 0x05, 0x1f, 0x00, 0x13, 0x36, 0x14, 0x00, 0x04, 0x06, 0x03, 0x00,
	0xf2, 0x67, 0x00, 0x00, 0x04, 0x00, 0x03, 0x00, 0xf3, 0x67, 0x04, 0x00, 0x04, 0x01 };

uint8_t pff_test_table_header_9c_data[ 4 ] = {
	0x40, 0x00, 0x00, 0x00 };

uint8_t pff_test_table_header_ac_data[ 40 ] = {
	0xac, 0x00, 0xd8, 0x00, 0xd8, 0x00, 0xdf, 0x00, 0xe6, 0x00, 0x20, 0x00, 0x00, 0x00, 0x3f, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0x00, 0x21, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xb0, 0x83, 0x4d, 0x0c };

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

/* Tests the libpff_table_header_initialize function
 * Returns 1 if successful or 0 if not
 */
int pff_test_table_header_initialize(
     void )
{
	libcerror_error_t *error            = NULL;
	libpff_table_header_t *table_header = NULL;
	int result                          = 0;

#if defined( HAVE_PFF_TEST_MEMORY )
	int number_of_malloc_fail_tests     = 1;
	int number_of_memset_fail_tests     = 1;
	int test_number                     = 0;
#endif

	/* Test regular cases
	 */
	result = libpff_table_header_initialize(
	          &table_header,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "table_header",
	 table_header );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_table_header_free(
	          &table_header,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "table_header",
	 table_header );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libpff_table_header_initialize(
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

	table_header = (libpff_table_header_t *) 0x12345678UL;

	result = libpff_table_header_initialize(
	          &table_header,
	          &error );

	table_header = NULL;

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
		/* Test libpff_table_header_initialize with malloc failing
		 */
		pff_test_malloc_attempts_before_fail = test_number;

		result = libpff_table_header_initialize(
		          &table_header,
		          &error );

		if( pff_test_malloc_attempts_before_fail != -1 )
		{
			pff_test_malloc_attempts_before_fail = -1;

			if( table_header != NULL )
			{
				libpff_table_header_free(
				 &table_header,
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
			 "table_header",
			 table_header );

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
		/* Test libpff_table_header_initialize with memset failing
		 */
		pff_test_memset_attempts_before_fail = test_number;

		result = libpff_table_header_initialize(
		          &table_header,
		          &error );

		if( pff_test_memset_attempts_before_fail != -1 )
		{
			pff_test_memset_attempts_before_fail = -1;

			if( table_header != NULL )
			{
				libpff_table_header_free(
				 &table_header,
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
			 "table_header",
			 table_header );

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
	if( table_header != NULL )
	{
		libpff_table_header_free(
		 &table_header,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_table_header_free function
 * Returns 1 if successful or 0 if not
 */
int pff_test_table_header_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libpff_table_header_free(
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

/* Tests the libpff_table_header_read_data function
 * Returns 1 if successful or 0 if not
 */
int pff_test_table_header_read_data(
     void )
{
	libcerror_error_t *error            = NULL;
	libpff_table_header_t *table_header = NULL;
	int result                          = 0;

	/* Initialize test
	 */
	result = libpff_table_header_initialize(
	          &table_header,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "table_header",
	 table_header );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libpff_table_header_read_data(
	          table_header,
	          pff_test_table_header_data,
	          12,
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
	result = libpff_table_header_read_data(
	          NULL,
	          pff_test_table_header_data,
	          12,
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

	result = libpff_table_header_read_data(
	          table_header,
	          NULL,
	          12,
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

	result = libpff_table_header_read_data(
	          table_header,
	          pff_test_table_header_data,
	          (size_t) SSIZE_MAX + 1,
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

	/* Test error case where signature is invalid
	 */
	pff_test_table_header_data[ 2 ] = 0xff;

	result = libpff_table_header_read_data(
	          table_header,
	          pff_test_table_header_data,
	          12,
	          &error );

	pff_test_table_header_data[ 2 ] = 0xec;

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where type is invalid
	 */
	pff_test_table_header_data[ 3 ] = 0xff;

	result = libpff_table_header_read_data(
	          table_header,
	          pff_test_table_header_data,
	          12,
	          &error );

	pff_test_table_header_data[ 3 ] = 0xbc;

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
	result = libpff_table_header_free(
	          &table_header,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "table_header",
	 table_header );

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
	if( table_header != NULL )
	{
		libpff_table_header_free(
		 &table_header,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_table_header_read_6c_data function
 * Returns 1 if successful or 0 if not
 */
int pff_test_table_header_read_6c_data(
     void )
{
	libcerror_error_t *error            = NULL;
	libpff_table_header_t *table_header = NULL;
	int result                          = 0;

	/* Initialize test
	 */
	result = libpff_table_header_initialize(
	          &table_header,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "table_header",
	 table_header );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libpff_table_header_read_6c_data(
	          table_header,
	          pff_test_table_header_6c_data,
	          8,
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
	result = libpff_table_header_read_6c_data(
	          NULL,
	          pff_test_table_header_6c_data,
	          8,
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

	result = libpff_table_header_read_6c_data(
	          table_header,
	          NULL,
	          8,
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

	result = libpff_table_header_read_6c_data(
	          table_header,
	          pff_test_table_header_6c_data,
	          (size_t) SSIZE_MAX + 1,
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
	result = libpff_table_header_free(
	          &table_header,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "table_header",
	 table_header );

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
	if( table_header != NULL )
	{
		libpff_table_header_free(
		 &table_header,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_table_header_read_7c_data function
 * Returns 1 if successful or 0 if not
 */
int pff_test_table_header_read_7c_data(
     void )
{
	libcerror_error_t *error            = NULL;
	libpff_table_header_t *table_header = NULL;
	int result                          = 0;

	/* Initialize test
	 */
	result = libpff_table_header_initialize(
	          &table_header,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "table_header",
	 table_header );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libpff_table_header_read_7c_data(
	          table_header,
	          pff_test_table_header_7c_data,
	          78,
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
	result = libpff_table_header_read_7c_data(
	          NULL,
	          pff_test_table_header_7c_data,
	          78,
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

	result = libpff_table_header_read_7c_data(
	          table_header,
	          NULL,
	          78,
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

	result = libpff_table_header_read_7c_data(
	          table_header,
	          pff_test_table_header_7c_data,
	          (size_t) SSIZE_MAX + 1,
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

	/* Test error case where type is invalid
	 */
	pff_test_table_header_7c_data[ 0 ] = 0xff;

	result = libpff_table_header_read_7c_data(
	          table_header,
	          pff_test_table_header_7c_data,
	          78,
	          &error );

	pff_test_table_header_7c_data[ 0 ] = 0x7c;

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
	result = libpff_table_header_free(
	          &table_header,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "table_header",
	 table_header );

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
	if( table_header != NULL )
	{
		libpff_table_header_free(
		 &table_header,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_table_header_read_9c_data function
 * Returns 1 if successful or 0 if not
 */
int pff_test_table_header_read_9c_data(
     void )
{
	libcerror_error_t *error            = NULL;
	libpff_table_header_t *table_header = NULL;
	int result                          = 0;

	/* Initialize test
	 */
	result = libpff_table_header_initialize(
	          &table_header,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "table_header",
	 table_header );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libpff_table_header_read_9c_data(
	          table_header,
	          pff_test_table_header_9c_data,
	          4,
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
	result = libpff_table_header_read_9c_data(
	          NULL,
	          pff_test_table_header_9c_data,
	          4,
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

	result = libpff_table_header_read_9c_data(
	          table_header,
	          NULL,
	          4,
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

	result = libpff_table_header_read_9c_data(
	          table_header,
	          pff_test_table_header_9c_data,
	          (size_t) SSIZE_MAX + 1,
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
	result = libpff_table_header_free(
	          &table_header,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "table_header",
	 table_header );

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
	if( table_header != NULL )
	{
		libpff_table_header_free(
		 &table_header,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_table_header_read_ac_data function
 * Returns 1 if successful or 0 if not
 */
int pff_test_table_header_read_ac_data(
     void )
{
	libcerror_error_t *error            = NULL;
	libpff_table_header_t *table_header = NULL;
	int result                          = 0;

	/* Initialize test
	 */
	result = libpff_table_header_initialize(
	          &table_header,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "table_header",
	 table_header );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libpff_table_header_read_ac_data(
	          table_header,
	          pff_test_table_header_ac_data,
	          40,
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
	result = libpff_table_header_read_ac_data(
	          NULL,
	          pff_test_table_header_ac_data,
	          40,
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

	result = libpff_table_header_read_ac_data(
	          table_header,
	          NULL,
	          40,
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

	result = libpff_table_header_read_ac_data(
	          table_header,
	          pff_test_table_header_ac_data,
	          (size_t) SSIZE_MAX + 1,
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

	/* Test error case where type is invalid
	 */
	pff_test_table_header_ac_data[ 0 ] = 0xff;

	result = libpff_table_header_read_ac_data(
	          table_header,
	          pff_test_table_header_ac_data,
	          40,
	          &error );

	pff_test_table_header_ac_data[ 0 ] = 0xac;

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
	result = libpff_table_header_free(
	          &table_header,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "table_header",
	 table_header );

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
	if( table_header != NULL )
	{
		libpff_table_header_free(
		 &table_header,
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
	 "libpff_table_header_initialize",
	 pff_test_table_header_initialize );

	PFF_TEST_RUN(
	 "libpff_table_header_free",
	 pff_test_table_header_free );

	PFF_TEST_RUN(
	 "libpff_table_header_read_data",
	 pff_test_table_header_read_data );

	PFF_TEST_RUN(
	 "libpff_table_header_read_6c_data",
	 pff_test_table_header_read_6c_data );

	PFF_TEST_RUN(
	 "libpff_table_header_read_7c_data",
	 pff_test_table_header_read_7c_data );

	PFF_TEST_RUN(
	 "libpff_table_header_read_9c_data",
	 pff_test_table_header_read_9c_data );

	PFF_TEST_RUN(
	 "libpff_table_header_read_ac_data",
	 pff_test_table_header_read_ac_data );

#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */
}

