/*
 * Library item_tree type test program
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

#include "pff_test_libbfio.h"
#include "pff_test_libcdata.h"
#include "pff_test_libcerror.h"
#include "pff_test_libpff.h"
#include "pff_test_macros.h"
#include "pff_test_memory.h"
#include "pff_test_unused.h"

#include "../libpff/libpff_item_tree.h"

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

/* Tests the libpff_item_tree_initialize function
 * Returns 1 if successful or 0 if not
 */
int pff_test_item_tree_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libpff_item_tree_t *item_tree   = NULL;
	int result                      = 0;

#if defined( HAVE_PFF_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libpff_item_tree_initialize(
	          &item_tree,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "item_tree",
	 item_tree );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_item_tree_free(
	          &item_tree,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "item_tree",
	 item_tree );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libpff_item_tree_initialize(
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

	item_tree = (libpff_item_tree_t *) 0x12345678UL;

	result = libpff_item_tree_initialize(
	          &item_tree,
	          &error );

	item_tree = NULL;

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
		/* Test libpff_item_tree_initialize with malloc failing
		 */
		pff_test_malloc_attempts_before_fail = test_number;

		result = libpff_item_tree_initialize(
		          &item_tree,
		          &error );

		if( pff_test_malloc_attempts_before_fail != -1 )
		{
			pff_test_malloc_attempts_before_fail = -1;

			if( item_tree != NULL )
			{
				libpff_item_tree_free(
				 &item_tree,
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
			 "item_tree",
			 item_tree );

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
		/* Test libpff_item_tree_initialize with memset failing
		 */
		pff_test_memset_attempts_before_fail = test_number;

		result = libpff_item_tree_initialize(
		          &item_tree,
		          &error );

		if( pff_test_memset_attempts_before_fail != -1 )
		{
			pff_test_memset_attempts_before_fail = -1;

			if( item_tree != NULL )
			{
				libpff_item_tree_free(
				 &item_tree,
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
			 "item_tree",
			 item_tree );

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
	if( item_tree != NULL )
	{
		libpff_item_tree_free(
		 &item_tree,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_item_tree_free function
 * Returns 1 if successful or 0 if not
 */
int pff_test_item_tree_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libpff_item_tree_free(
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

/* Tests the libpff_item_tree_node_free_recovered function
 * Returns 1 if successful or 0 if not
 */
int pff_test_item_tree_node_free_recovered(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libpff_item_tree_node_free_recovered(
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

/* Tests the libpff_item_tree_get_tree_node_by_identifier function
 * Returns 1 if successful or 0 if not
 */
int pff_test_item_tree_get_tree_node_by_identifier(
     void )
{
	libcdata_tree_node_t *item_tree_node        = NULL;
	libcdata_tree_node_t *result_item_tree_node = NULL;
	libcerror_error_t *error                    = NULL;
	int result                                  = 0;

	/* Test error cases
	 */
	result = libpff_item_tree_get_tree_node_by_identifier(
	          NULL,
	          0,
	          &result_item_tree_node,
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

	result = libpff_item_tree_get_tree_node_by_identifier(
	          item_tree_node,
	          0,
	          NULL,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libpff_item_tree_get_sub_node_by_identifier function
 * Returns 1 if successful or 0 if not
 */
int pff_test_item_tree_get_sub_node_by_identifier(
     void )
{
	libcdata_tree_node_t *item_tree_node = NULL;
	libcdata_tree_node_t *sub_node       = NULL;
	libcerror_error_t *error             = NULL;
	int result                           = 0;

	/* Test error cases
	 */
	result = libpff_item_tree_get_sub_node_by_identifier(
	          NULL,
	          0,
	          &sub_node,
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

	result = libpff_item_tree_get_sub_node_by_identifier(
	          item_tree_node,
	          0,
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
	 "libpff_item_tree_initialize",
	 pff_test_item_tree_initialize );

	PFF_TEST_RUN(
	 "libpff_item_tree_free",
	 pff_test_item_tree_free );

	PFF_TEST_RUN(
	 "libpff_item_tree_node_free_recovered",
	 pff_test_item_tree_node_free_recovered );

	PFF_TEST_RUN(
	 "libpff_item_tree_get_tree_node_by_identifier",
	 pff_test_item_tree_get_tree_node_by_identifier );

	PFF_TEST_RUN(
	 "libpff_item_tree_get_sub_node_by_identifier",
	 pff_test_item_tree_get_sub_node_by_identifier );

	/* TODO: add tests for libpff_item_tree_append_identifier */

	/* TODO: add tests for libpff_item_tree_create */

	/* TODO: add tests for libpff_item_tree_create_node_from_descriptor_index_node */

	/* TODO: add tests for libpff_item_tree_create_leaf_node_from_descriptor_index_value */

	/* TODO: add tests for libpff_item_tree_get_node_by_identifier */

#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */
}

