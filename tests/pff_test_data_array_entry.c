/*
 * Library data_array_entry type testing program
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

#include "../libpff/libpff_data_array_entry.h"

#if defined( __GNUC__ )

/* Tests the libpff_data_array_entry_initialize function
 * Returns 1 if successful or 0 if not
 */
int pff_test_data_array_entry_initialize(
     void )
{
	libcerror_error_t *error                    = NULL;
	libpff_data_array_entry_t *data_array_entry = NULL;
	int result                                  = 0;

#if defined( HAVE_PFF_TEST_MEMORY )
	int number_of_malloc_fail_tests             = 1;
	int number_of_memset_fail_tests             = 1;
	int test_number                             = 0;
#endif

	/* Test regular cases
	 */
	result = libpff_data_array_entry_initialize(
	          &data_array_entry,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        PFF_TEST_ASSERT_IS_NOT_NULL(
         "data_array_entry",
         data_array_entry );

        PFF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libpff_data_array_entry_free(
	          &data_array_entry,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        PFF_TEST_ASSERT_IS_NULL(
         "data_array_entry",
         data_array_entry );

        PFF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libpff_data_array_entry_initialize(
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

	data_array_entry = (libpff_data_array_entry_t *) 0x12345678UL;

	result = libpff_data_array_entry_initialize(
	          &data_array_entry,
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

	data_array_entry = NULL;

#if defined( HAVE_PFF_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libpff_data_array_entry_initialize with malloc failing
		 */
		pff_test_malloc_attempts_before_fail = test_number;

		result = libpff_data_array_entry_initialize(
		          &data_array_entry,
		          &error );

		if( pff_test_malloc_attempts_before_fail != -1 )
		{
			pff_test_malloc_attempts_before_fail = -1;

			if( data_array_entry != NULL )
			{
				libpff_data_array_entry_free(
				 &data_array_entry,
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
			 "data_array_entry",
			 data_array_entry );

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
		/* Test libpff_data_array_entry_initialize with memset failing
		 */
		pff_test_memset_attempts_before_fail = test_number;

		result = libpff_data_array_entry_initialize(
		          &data_array_entry,
		          &error );

		if( pff_test_memset_attempts_before_fail != -1 )
		{
			pff_test_memset_attempts_before_fail = -1;

			if( data_array_entry != NULL )
			{
				libpff_data_array_entry_free(
				 &data_array_entry,
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
			 "data_array_entry",
			 data_array_entry );

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
	if( data_array_entry != NULL )
	{
		libpff_data_array_entry_free(
		 &data_array_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_data_array_entry_free function
 * Returns 1 if successful or 0 if not
 */
int pff_test_data_array_entry_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libpff_data_array_entry_free(
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

/* Tests the libpff_data_array_entry_clone function
 * Returns 1 if successful or 0 if not
 */
int pff_test_data_array_entry_clone(
     void )
{
	libcerror_error_t *error                                = NULL;
	libpff_data_array_entry_t *destination_data_array_entry = NULL;
	libpff_data_array_entry_t *source_data_array_entry      = NULL;
	int result                                              = 0;

	/* Initialize test
	 */
	result = libpff_data_array_entry_initialize(
	          &source_data_array_entry,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        PFF_TEST_ASSERT_IS_NOT_NULL(
         "source_data_array_entry",
         source_data_array_entry );

        PFF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test regular cases
	 */
	result = libpff_data_array_entry_clone(
	          &destination_data_array_entry,
	          source_data_array_entry,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        PFF_TEST_ASSERT_IS_NOT_NULL(
         "destination_data_array_entry",
         destination_data_array_entry );

        PFF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libpff_data_array_entry_free(
	          &destination_data_array_entry,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        PFF_TEST_ASSERT_IS_NULL(
         "destination_data_array_entry",
         destination_data_array_entry );

        PFF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libpff_data_array_entry_clone(
	          &destination_data_array_entry,
	          NULL,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        PFF_TEST_ASSERT_IS_NULL(
         "destination_data_array_entry",
         destination_data_array_entry );

        PFF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libpff_data_array_entry_clone(
	          NULL,
	          source_data_array_entry,
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
	result = libpff_data_array_entry_free(
	          &source_data_array_entry,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        PFF_TEST_ASSERT_IS_NULL(
         "source_data_array_entry",
         source_data_array_entry );

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
	if( destination_data_array_entry != NULL )
	{
		libpff_data_array_entry_free(
		 &destination_data_array_entry,
		 NULL );
	}
	if( source_data_array_entry != NULL )
	{
		libpff_data_array_entry_free(
		 &source_data_array_entry,
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
	 "libpff_data_array_entry_initialize",
	 pff_test_data_array_entry_initialize );

	PFF_TEST_RUN(
	 "libpff_data_array_entry_free",
	 pff_test_data_array_entry_free );

	PFF_TEST_RUN(
	 "libpff_data_array_entry_clone",
	 pff_test_data_array_entry_clone );

#endif /* defined( __GNUC__ ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

