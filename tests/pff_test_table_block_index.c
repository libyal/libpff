/*
 * Library table_block_index type testing program
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

#include "../libpff/libpff_table_block_index.h"

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

/* Tests the libpff_table_block_index_initialize function
 * Returns 1 if successful or 0 if not
 */
int pff_test_table_block_index_initialize(
     void )
{
	libcerror_error_t *error                      = NULL;
	libpff_table_block_index_t *table_block_index = NULL;
	int result                                    = 0;

#if defined( HAVE_PFF_TEST_MEMORY )
	int number_of_malloc_fail_tests               = 1;
	int number_of_memset_fail_tests               = 1;
	int test_number                               = 0;
#endif

	/* Test regular cases
	 */
	result = libpff_table_block_index_initialize(
	          &table_block_index,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "table_block_index",
	 table_block_index );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_table_block_index_free(
	          &table_block_index,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "table_block_index",
	 table_block_index );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libpff_table_block_index_initialize(
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

	table_block_index = (libpff_table_block_index_t *) 0x12345678UL;

	result = libpff_table_block_index_initialize(
	          &table_block_index,
	          &error );

	table_block_index = NULL;

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
		/* Test libpff_table_block_index_initialize with malloc failing
		 */
		pff_test_malloc_attempts_before_fail = test_number;

		result = libpff_table_block_index_initialize(
		          &table_block_index,
		          &error );

		if( pff_test_malloc_attempts_before_fail != -1 )
		{
			pff_test_malloc_attempts_before_fail = -1;

			if( table_block_index != NULL )
			{
				libpff_table_block_index_free(
				 &table_block_index,
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
			 "table_block_index",
			 table_block_index );

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
		/* Test libpff_table_block_index_initialize with memset failing
		 */
		pff_test_memset_attempts_before_fail = test_number;

		result = libpff_table_block_index_initialize(
		          &table_block_index,
		          &error );

		if( pff_test_memset_attempts_before_fail != -1 )
		{
			pff_test_memset_attempts_before_fail = -1;

			if( table_block_index != NULL )
			{
				libpff_table_block_index_free(
				 &table_block_index,
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
			 "table_block_index",
			 table_block_index );

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
	if( table_block_index != NULL )
	{
		libpff_table_block_index_free(
		 &table_block_index,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_table_block_index_free function
 * Returns 1 if successful or 0 if not
 */
int pff_test_table_block_index_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libpff_table_block_index_free(
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

/* Tests the libpff_table_block_index_get_number_of_values function
 * Returns 1 if successful or 0 if not
 */
int pff_test_table_block_index_get_number_of_values(
     void )
{
	libcerror_error_t *error                      = NULL;
	libpff_table_block_index_t *table_block_index = NULL;
	uint16_t number_of_values                     = 0;
	int result                                    = 0;

	/* Initialize test
	 */
	result = libpff_table_block_index_initialize(
	          &table_block_index,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "table_block_index",
	 table_block_index );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libpff_table_block_index_get_number_of_values(
	          table_block_index,
	          &number_of_values,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_EQUAL_UINT16(
	 "number_of_values",
	 number_of_values,
	 0 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libpff_table_block_index_get_number_of_values(
	          NULL,
	          &number_of_values,
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

	result = libpff_table_block_index_get_number_of_values(
	          table_block_index,
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
	result = libpff_table_block_index_free(
	          &table_block_index,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "table_block_index",
	 table_block_index );

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
	if( table_block_index != NULL )
	{
		libpff_table_block_index_free(
		 &table_block_index,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_table_block_index_get_value_by_index function
 * Returns 1 if successful or 0 if not
 */
int pff_test_table_block_index_get_value_by_index(
     void )
{
	libcerror_error_t *error                      = NULL;
	libpff_table_block_index_t *table_block_index = NULL;
	libpff_table_index_value_t *table_index_value = NULL;
	uint16_t value_index                          = 0;
	int result                                    = 0;

	/* Initialize test
	 */
	result = libpff_table_block_index_initialize(
	          &table_block_index,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "table_block_index",
	 table_block_index );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_table_block_index_append_value(
	          table_block_index,
	          &value_index,
	          NULL,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_EQUAL_UINT16(
	 "value_index",
	 value_index,
	 0 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libpff_table_block_index_get_value_by_index(
	          table_block_index,
	          0,
	          &table_index_value,
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
	result = libpff_table_block_index_get_value_by_index(
	          NULL,
	          0,
	          &table_index_value,
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

	result = libpff_table_block_index_get_value_by_index(
	          table_block_index,
	          99,
	          &table_index_value,
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
	result = libpff_table_block_index_free(
	          &table_block_index,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "table_block_index",
	 table_block_index );

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
	if( table_block_index != NULL )
	{
		libpff_table_block_index_free(
		 &table_block_index,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_table_block_index_append_value function
 * Returns 1 if successful or 0 if not
 */
int pff_test_table_block_index_append_value(
     void )
{
	libcerror_error_t *error                      = NULL;
	libpff_table_block_index_t *table_block_index = NULL;
	uint16_t value_index                          = 0;
	int result                                    = 0;

	/* Initialize test
	 */
	result = libpff_table_block_index_initialize(
	          &table_block_index,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "table_block_index",
	 table_block_index );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libpff_table_block_index_append_value(
	          table_block_index,
	          &value_index,
	          NULL,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_EQUAL_UINT16(
	 "value_index",
	 value_index,
	 0 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libpff_table_block_index_append_value(
	          NULL,
	          &value_index,
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

	result = libpff_table_block_index_append_value(
	          table_block_index,
	          NULL,
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
	result = libpff_table_block_index_free(
	          &table_block_index,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "table_block_index",
	 table_block_index );

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
	if( table_block_index != NULL )
	{
		libpff_table_block_index_free(
		 &table_block_index,
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
	 "libpff_table_block_index_initialize",
	 pff_test_table_block_index_initialize );

	PFF_TEST_RUN(
	 "libpff_table_block_index_free",
	 pff_test_table_block_index_free );

	PFF_TEST_RUN(
	 "libpff_table_block_index_get_number_of_values",
	 pff_test_table_block_index_get_number_of_values );

	PFF_TEST_RUN(
	 "libpff_table_block_index_get_value_by_index",
	 pff_test_table_block_index_get_value_by_index );

	PFF_TEST_RUN(
	 "libpff_table_block_index_append_value",
	 pff_test_table_block_index_append_value );

#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

