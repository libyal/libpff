/*
 * Library local_descriptor_node type testing program
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

#include "../libpff/libpff_local_descriptor_node.h"

#if defined( __GNUC__ )

/* Tests the libpff_local_descriptor_node_initialize function
 * Returns 1 if successful or 0 if not
 */
int pff_test_local_descriptor_node_initialize(
     void )
{
	libcerror_error_t *error                              = NULL;
	libpff_local_descriptor_node_t *local_descriptor_node = NULL;
	int result                                            = 0;

#if defined( HAVE_PFF_TEST_MEMORY )
	int number_of_malloc_fail_tests                       = 1;
	int number_of_memset_fail_tests                       = 1;
	int test_number                                       = 0;
#endif

	/* Test regular cases
	 */
	result = libpff_local_descriptor_node_initialize(
	          &local_descriptor_node,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        PFF_TEST_ASSERT_IS_NOT_NULL(
         "local_descriptor_node",
         local_descriptor_node );

        PFF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libpff_local_descriptor_node_free(
	          &local_descriptor_node,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        PFF_TEST_ASSERT_IS_NULL(
         "local_descriptor_node",
         local_descriptor_node );

        PFF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libpff_local_descriptor_node_initialize(
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

	local_descriptor_node = (libpff_local_descriptor_node_t *) 0x12345678UL;

	result = libpff_local_descriptor_node_initialize(
	          &local_descriptor_node,
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

	local_descriptor_node = NULL;

#if defined( HAVE_PFF_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libpff_local_descriptor_node_initialize with malloc failing
		 */
		pff_test_malloc_attempts_before_fail = test_number;

		result = libpff_local_descriptor_node_initialize(
		          &local_descriptor_node,
		          &error );

		if( pff_test_malloc_attempts_before_fail != -1 )
		{
			pff_test_malloc_attempts_before_fail = -1;

			if( local_descriptor_node != NULL )
			{
				libpff_local_descriptor_node_free(
				 &local_descriptor_node,
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
			 "local_descriptor_node",
			 local_descriptor_node );

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
		/* Test libpff_local_descriptor_node_initialize with memset failing
		 */
		pff_test_memset_attempts_before_fail = test_number;

		result = libpff_local_descriptor_node_initialize(
		          &local_descriptor_node,
		          &error );

		if( pff_test_memset_attempts_before_fail != -1 )
		{
			pff_test_memset_attempts_before_fail = -1;

			if( local_descriptor_node != NULL )
			{
				libpff_local_descriptor_node_free(
				 &local_descriptor_node,
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
			 "local_descriptor_node",
			 local_descriptor_node );

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
	if( local_descriptor_node != NULL )
	{
		libpff_local_descriptor_node_free(
		 &local_descriptor_node,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_local_descriptor_node_free function
 * Returns 1 if successful or 0 if not
 */
int pff_test_local_descriptor_node_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libpff_local_descriptor_node_free(
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
	 "libpff_local_descriptor_node_initialize",
	 pff_test_local_descriptor_node_initialize );

	PFF_TEST_RUN(
	 "libpff_local_descriptor_node_free",
	 pff_test_local_descriptor_node_free );

	/* TODO: add tests for libpff_local_descriptor_node_get_entry_data */

	/* TODO: add tests for libpff_local_descriptor_node_get_entry_identifier */

	/* TODO: add tests for libpff_local_descriptor_node_get_entry_sub_node_identifier */

	/* TODO: add tests for libpff_local_descriptor_node_read */

	/* TODO: add tests for libpff_local_descriptor_node_read_element_data */

#endif /* defined( __GNUC__ ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

