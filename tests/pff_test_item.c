/*
 * Library item type test program
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

#include "pff_test_libcdata.h"
#include "pff_test_libcerror.h"
#include "pff_test_libpff.h"
#include "pff_test_macros.h"
#include "pff_test_memory.h"
#include "pff_test_unused.h"

#include "../libpff/libpff_io_handle.h"
#include "../libpff/libpff_item.h"
#include "../libpff/libpff_item_descriptor.h"

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

/* Tests the libpff_item_initialize function
 * Returns 1 if successful or 0 if not
 */
int pff_test_item_initialize(
     void )
{
	libcdata_tree_node_t *item_tree_node      = NULL;
	libcerror_error_t *error                  = NULL;
	libpff_io_handle_t *io_handle             = NULL;
	libpff_item_t *item                       = NULL;
	libpff_item_descriptor_t *item_descriptor = NULL;
	int result                                = 0;

#if defined( HAVE_PFF_TEST_MEMORY )
	int number_of_malloc_fail_tests           = 1;
	int number_of_memset_fail_tests           = 1;
	int test_number                           = 0;
#endif

	/* Initialize test
	 */
	result = libpff_io_handle_initialize(
	          &io_handle,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_item_descriptor_initialize(
	          &item_descriptor,
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
	 "item_descriptor",
	 item_descriptor );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcdata_tree_node_initialize(
	          &item_tree_node,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "item_tree_node",
	 item_tree_node );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcdata_tree_node_set_value(
	          item_tree_node,
	          (intptr_t *) item_descriptor,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	item_descriptor = NULL;

	/* Test regular cases
	 */
	result = libpff_item_initialize(
	          &item,
	          io_handle,
	          NULL,
	          NULL,
	          NULL,
	          NULL,
	          NULL,
	          item_tree_node,
	          0,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "item",
	 item );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

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

	/* Test error cases
	 */
	result = libpff_item_initialize(
	          NULL,
	          io_handle,
	          NULL,
	          NULL,
	          NULL,
	          NULL,
	          NULL,
	          item_tree_node,
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

	item = (libpff_item_t *) 0x12345678UL;

	result = libpff_item_initialize(
	          &item,
	          io_handle,
	          NULL,
	          NULL,
	          NULL,
	          NULL,
	          NULL,
	          item_tree_node,
	          0,
	          &error );

	item = NULL;

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libpff_item_initialize(
	          &item,
	          NULL,
	          NULL,
	          NULL,
	          NULL,
	          NULL,
	          NULL,
	          item_tree_node,
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

	result = libpff_item_initialize(
	          &item,
	          io_handle,
	          NULL,
	          NULL,
	          NULL,
	          NULL,
	          NULL,
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

#if defined( HAVE_PFF_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libpff_item_initialize with malloc failing
		 */
		pff_test_malloc_attempts_before_fail = test_number;

		result = libpff_item_initialize(
		          &item,
		          io_handle,
		          NULL,
		          NULL,
		          NULL,
		          NULL,
		          NULL,
		          item_tree_node,
		          0,
		          &error );

		if( pff_test_malloc_attempts_before_fail != -1 )
		{
			pff_test_malloc_attempts_before_fail = -1;

			if( item != NULL )
			{
				libpff_item_free(
				 &item,
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
			 "item",
			 item );

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
		/* Test libpff_item_initialize with memset failing
		 */
		pff_test_memset_attempts_before_fail = test_number;

		result = libpff_item_initialize(
		          &item,
		          io_handle,
		          NULL,
		          NULL,
		          NULL,
		          NULL,
		          NULL,
		          item_tree_node,
		          0,
		          &error );

		if( pff_test_memset_attempts_before_fail != -1 )
		{
			pff_test_memset_attempts_before_fail = -1;

			if( item != NULL )
			{
				libpff_item_free(
				 &item,
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
			 "item",
			 item );

			PFF_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_PFF_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libcdata_tree_node_free(
	          &item_tree_node,
	          (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_descriptor_free,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "item_tree_node",
	 item_tree_node );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_io_handle_free(
	          &io_handle,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

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
	if( item != NULL )
	{
		libpff_item_free(
		 &item,
		 NULL );
	}
	if( item_tree_node != NULL )
	{
		libcdata_tree_node_free(
		 &item_tree_node,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_descriptor_free,
		 NULL );
	}
	if( item_descriptor != NULL )
	{
		libpff_item_descriptor_free(
		 &item_descriptor,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libpff_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */

/* Tests the libpff_item_free function
 * Returns 1 if successful or 0 if not
 */
int pff_test_item_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libpff_item_free(
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

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

/* Tests the libpff_item_clone function
 * Returns 1 if successful or 0 if not
 */
int pff_test_item_clone(
     void )
{
	libcdata_tree_node_t *item_tree_node      = NULL;
	libcerror_error_t *error                  = NULL;
	libpff_io_handle_t *io_handle             = NULL;
	libpff_item_t *destination_item           = NULL;
	libpff_item_t *source_item                = NULL;
	libpff_item_descriptor_t *item_descriptor = NULL;
	int result                                = 0;

#if defined( HAVE_PFF_TEST_MEMORY )
	int number_of_malloc_fail_tests           = 1;
	int number_of_memset_fail_tests           = 1;
	int test_number                           = 0;
#endif

	/* Initialize test
	 */
	result = libpff_io_handle_initialize(
	          &io_handle,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_item_descriptor_initialize(
	          &item_descriptor,
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
	 "item_descriptor",
	 item_descriptor );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcdata_tree_node_initialize(
	          &item_tree_node,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "item_tree_node",
	 item_tree_node );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcdata_tree_node_set_value(
	          item_tree_node,
	          (intptr_t *) item_descriptor,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	item_descriptor = NULL;

	result = libpff_item_initialize(
	          &source_item,
	          io_handle,
	          NULL,
	          NULL,
	          NULL,
	          NULL,
	          NULL,
	          item_tree_node,
	          0,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "source_item",
	 source_item );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libpff_item_clone(
	          &destination_item,
	          source_item,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "destination_item",
	 destination_item );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_item_free(
	          &destination_item,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "destination_item",
	 destination_item );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_item_clone(
	          &destination_item,
	          NULL,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "destination_item",
	 destination_item );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libpff_item_clone(
	          NULL,
	          source_item,
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

	destination_item = (libpff_item_t *) 0x12345678UL;

	result = libpff_item_clone(
	          &destination_item,
	          source_item,
	          &error );

	destination_item = NULL;

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
		/* Test libpff_item_clone with malloc failing
		 * Test libpff_item_clone with malloc failing in libcdata_array_clone
		 */
		pff_test_malloc_attempts_before_fail = test_number;

		result = libpff_item_clone(
		          &destination_item,
		          source_item,
		          &error );

		if( pff_test_malloc_attempts_before_fail != -1 )
		{
			pff_test_malloc_attempts_before_fail = -1;

			if( destination_item != NULL )
			{
				libpff_item_free(
				 &destination_item,
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
			 "destination_item",
			 destination_item );

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
		/* Test libpff_item_clone with memset failing
		 */
		pff_test_memset_attempts_before_fail = test_number;

		result = libpff_item_clone(
		          &destination_item,
		          source_item,
		          &error );

		if( pff_test_memset_attempts_before_fail != -1 )
		{
			pff_test_memset_attempts_before_fail = -1;

			if( destination_item != NULL )
			{
				libpff_item_free(
				 &destination_item,
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
			 "destination_item",
			 destination_item );

			PFF_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_PFF_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libcdata_tree_node_free(
	          &item_tree_node,
	          (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_descriptor_free,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "item_tree_node",
	 item_tree_node );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_item_free(
	          &source_item,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "source_item",
	 source_item );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_io_handle_free(
	          &io_handle,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

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
	if( destination_item != NULL )
	{
		libpff_item_free(
		 &destination_item,
		 NULL );
	}
	if( source_item != NULL )
	{
		libpff_item_free(
		 &source_item,
		 NULL );
	}
	if( item_tree_node != NULL )
	{
		libcdata_tree_node_free(
		 &item_tree_node,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_descriptor_free,
		 NULL );
	}
	if( item_descriptor != NULL )
	{
		libpff_item_descriptor_free(
		 &item_descriptor,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libpff_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_internal_item_determine_type function
 * Returns 1 if successful or 0 if not
 */
int pff_test_internal_item_determine_type(
     libpff_item_t *item )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libpff_internal_item_determine_type(
	          (libpff_internal_item_t *) item,
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
	result = libpff_internal_item_determine_type(
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

/* Tests the libpff_item_get_identifier function
 * Returns 1 if successful or 0 if not
 */
int pff_test_item_get_identifier(
     libpff_item_t *item )
{
	libcerror_error_t *error = NULL;
	uint32_t identifier      = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libpff_item_get_identifier(
	          item,
	          &identifier,
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
	result = libpff_item_get_identifier(
	          NULL,
	          &identifier,
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

	result = libpff_item_get_identifier(
	          item,
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

/* Tests the libpff_internal_item_get_entry_value_32bit_integer function
 * Returns 1 if successful or 0 if not
 */
int pff_test_internal_item_get_entry_value_32bit_integer(
     libpff_item_t *item )
{
	libcerror_error_t *error = NULL;
	uint32_t value_32bit     = 0;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO implement
	result = libpff_internal_item_get_entry_value_32bit_integer(
	          (libpff_internal_item_t *) item,
	          0,
	          &value_32bit,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );
*/

	/* Test error cases
	 */
	result = libpff_internal_item_get_entry_value_32bit_integer(
	          NULL,
	          0,
	          &value_32bit,
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

	result = libpff_internal_item_get_entry_value_32bit_integer(
	          (libpff_internal_item_t *) item,
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
#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )
#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )
	libcdata_tree_node_t *item_tree_node      = NULL;
	libcerror_error_t *error                  = NULL;
	libpff_io_handle_t *io_handle             = NULL;
	libpff_item_t *item                       = NULL;
	libpff_item_descriptor_t *item_descriptor = NULL;
	int result                                = 0;

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */
#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */

	PFF_TEST_UNREFERENCED_PARAMETER( argc )
	PFF_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

	PFF_TEST_RUN(
	 "libpff_item_initialize",
	 pff_test_item_initialize );

#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */

	PFF_TEST_RUN(
	 "libpff_item_free",
	 pff_test_item_free );

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

	PFF_TEST_RUN(
	 "libpff_item_clone",
	 pff_test_item_clone );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize test
	 */
	result = libpff_io_handle_initialize(
	          &io_handle,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_item_descriptor_initialize(
	          &item_descriptor,
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
	 "item_descriptor",
	 item_descriptor );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcdata_tree_node_initialize(
	          &item_tree_node,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "item_tree_node",
	 item_tree_node );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcdata_tree_node_set_value(
	          item_tree_node,
	          (intptr_t *) item_descriptor,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	item_descriptor = NULL;

	result = libpff_item_initialize(
	          &item,
	          io_handle,
	          NULL,
	          NULL,
	          NULL,
	          NULL,
	          NULL,
	          item_tree_node,
	          0,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "item",
	 item );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

/* TODO set up tests
	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_internal_item_determine_type",
	 pff_test_internal_item_determine_type,
	 item );
*/

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_item_get_identifier",
	 pff_test_item_get_identifier,
	 item );

	/* TODO: add tests for libpff_item_get_number_of_items */

	/* TODO: add tests for libpff_item_get_item_by_index */

	/* TODO: add tests for libpff_item_get_number_of_entries */

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_internal_item_get_entry_value_32bit_integer",
	 pff_test_internal_item_get_entry_value_32bit_integer,
	 item );

	/* TODO: add tests for libpff_internal_item_get_entry_value_filetime */

	/* TODO: add tests for libpff_internal_item_get_entry_value_utf8_string_size */

	/* TODO: add tests for libpff_internal_item_get_entry_value_utf8_string */

	/* TODO: add tests for libpff_internal_item_get_entry_value_utf16_string_size */

	/* TODO: add tests for libpff_internal_item_get_entry_value_utf16_string */

	/* TODO: add tests for libpff_internal_item_get_embedded_object_data */

	/* TODO: add tests for libpff_item_get_type */

	/* TODO: add tests for libpff_item_get_number_of_sub_items */

	/* TODO: add tests for libpff_item_get_sub_item */

	/* TODO: add tests for libpff_item_get_sub_item_by_identifier */

	/* Clean up
	 */
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

	result = libcdata_tree_node_free(
	          &item_tree_node,
	          (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_descriptor_free,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "item_tree_node",
	 item_tree_node );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_io_handle_free(
	          &io_handle,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */
#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )
#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( item != NULL )
	{
		libpff_item_free(
		 &item,
		 NULL );
	}
	if( item_tree_node != NULL )
	{
		libcdata_tree_node_free(
		 &item_tree_node,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_descriptor_free,
		 NULL );
	}
	if( item_descriptor != NULL )
	{
		libpff_item_descriptor_free(
		 &item_descriptor,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libpff_io_handle_free(
		 &io_handle,
		 NULL );
	}
#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */
#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */

	return( EXIT_FAILURE );
}

