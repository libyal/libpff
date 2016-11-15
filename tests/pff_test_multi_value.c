/*
 * Library multi_value type testing program
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

#include "../libpff/libpff_multi_value.h"

#if defined( __GNUC__ )

/* Tests the libpff_multi_value_initialize function
 * Returns 1 if successful or 0 if not
 */
int pff_test_multi_value_initialize(
     void )
{
	libcerror_error_t *error          = NULL;
	libpff_multi_value_t *multi_value = NULL;
	int result                        = 0;

#if defined( HAVE_PFF_TEST_MEMORY )
	int number_of_malloc_fail_tests   = 1;
	int number_of_memset_fail_tests   = 1;
	int test_number                   = 0;
#endif

	/* Test regular cases
	 */
	result = libpff_multi_value_initialize(
	          &multi_value,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        PFF_TEST_ASSERT_IS_NOT_NULL(
         "multi_value",
         multi_value );

        PFF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libpff_multi_value_free(
	          &multi_value,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        PFF_TEST_ASSERT_IS_NULL(
         "multi_value",
         multi_value );

        PFF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libpff_multi_value_initialize(
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

	multi_value = (libpff_multi_value_t *) 0x12345678UL;

	result = libpff_multi_value_initialize(
	          &multi_value,
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

	multi_value = NULL;

#if defined( HAVE_PFF_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libpff_multi_value_initialize with malloc failing
		 */
		pff_test_malloc_attempts_before_fail = test_number;

		result = libpff_multi_value_initialize(
		          &multi_value,
		          &error );

		if( pff_test_malloc_attempts_before_fail != -1 )
		{
			pff_test_malloc_attempts_before_fail = -1;

			if( multi_value != NULL )
			{
				libpff_multi_value_free(
				 &multi_value,
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
			 "multi_value",
			 multi_value );

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
		/* Test libpff_multi_value_initialize with memset failing
		 */
		pff_test_memset_attempts_before_fail = test_number;

		result = libpff_multi_value_initialize(
		          &multi_value,
		          &error );

		if( pff_test_memset_attempts_before_fail != -1 )
		{
			pff_test_memset_attempts_before_fail = -1;

			if( multi_value != NULL )
			{
				libpff_multi_value_free(
				 &multi_value,
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
			 "multi_value",
			 multi_value );

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
	if( multi_value != NULL )
	{
		libpff_multi_value_free(
		 &multi_value,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) */

/* Tests the libpff_multi_value_free function
 * Returns 1 if successful or 0 if not
 */
int pff_test_multi_value_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libpff_multi_value_free(
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

#if defined( __GNUC__ )

/* Tests the libpff_multi_value_get_number_of_values function
 * Returns 1 if successful or 0 if not
 */
int pff_test_multi_value_get_number_of_values(
     void )
{
	libcerror_error_t *error          = NULL;
	libpff_multi_value_t *multi_value = NULL;
	int number_of_values              = 0;
	int number_of_values_is_set       = 0;
	int result                        = 0;

	/* Initialize test
	 */
	result = libpff_multi_value_initialize(
	          &multi_value,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "multi_value",
	 multi_value );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libpff_multi_value_get_number_of_values(
	          multi_value,
	          &number_of_values,
	          &error );

	PFF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	number_of_values_is_set = result;

	/* Test error cases
	 */
	result = libpff_multi_value_get_number_of_values(
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

	if( number_of_values_is_set != 0 )
	{
		result = libpff_multi_value_get_number_of_values(
		          multi_value,
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
	}
	/* Clean up
	 */
	result = libpff_multi_value_free(
	          &multi_value,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "multi_value",
	 multi_value );

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
	if( multi_value != NULL )
	{
		libpff_multi_value_free(
		 &multi_value,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) */

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

#if defined( __GNUC__ )

	PFF_TEST_RUN(
	 "libpff_multi_value_initialize",
	 pff_test_multi_value_initialize );

#endif /* defined( __GNUC__ ) */

	PFF_TEST_RUN(
	 "libpff_multi_value_free",
	 pff_test_multi_value_free );

#if defined( __GNUC__ )

	PFF_TEST_RUN(
	 "libpff_multi_value_get_number_of_values",
	 pff_test_multi_value_get_number_of_values );

#endif /* defined( __GNUC__ ) */

	/* TODO: add tests for libpff_multi_value_get_value */

	/* TODO: add tests for libpff_multi_value_get_value_32bit */

	/* TODO: add tests for libpff_multi_value_get_value_64bit */

	/* TODO: add tests for libpff_multi_value_get_value_filetime */

	/* TODO: add tests for libpff_multi_value_get_value_utf8_string_size */

	/* TODO: add tests for libpff_multi_value_get_value_utf8_string */

	/* TODO: add tests for libpff_multi_value_get_value_utf16_string_size */

	/* TODO: add tests for libpff_multi_value_get_value_utf16_string */

	/* TODO: add tests for libpff_multi_value_get_value_binary_data_size */

	/* TODO: add tests for libpff_multi_value_get_value_binary_data */

	/* TODO: add tests for libpff_multi_value_get_value_guid */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

