/*
 * Library index_value type test program
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
#include "pff_test_libfdata.h"
#include "pff_test_libpff.h"
#include "pff_test_macros.h"
#include "pff_test_memory.h"
#include "pff_test_unused.h"

#include "../libpff/libpff_definitions.h"
#include "../libpff/libpff_index_value.h"
#include "../libpff/libpff_io_handle.h"

uint8_t pff_test_index_value_data1[ 32 ] = {
	0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

/* Tests the libpff_index_value_initialize function
 * Returns 1 if successful or 0 if not
 */
int pff_test_index_value_initialize(
     void )
{
	libcerror_error_t *error          = NULL;
	libpff_index_value_t *index_value = NULL;
	int result                        = 0;

#if defined( HAVE_PFF_TEST_MEMORY )
	int number_of_malloc_fail_tests   = 1;
	int number_of_memset_fail_tests   = 1;
	int test_number                   = 0;
#endif

	/* Test regular cases
	 */
	result = libpff_index_value_initialize(
	          &index_value,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "index_value",
	 index_value );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_index_value_free(
	          &index_value,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "index_value",
	 index_value );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libpff_index_value_initialize(
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

	index_value = (libpff_index_value_t *) 0x12345678UL;

	result = libpff_index_value_initialize(
	          &index_value,
	          &error );

	index_value = NULL;

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
		/* Test libpff_index_value_initialize with malloc failing
		 */
		pff_test_malloc_attempts_before_fail = test_number;

		result = libpff_index_value_initialize(
		          &index_value,
		          &error );

		if( pff_test_malloc_attempts_before_fail != -1 )
		{
			pff_test_malloc_attempts_before_fail = -1;

			if( index_value != NULL )
			{
				libpff_index_value_free(
				 &index_value,
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
			 "index_value",
			 index_value );

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
		/* Test libpff_index_value_initialize with memset failing
		 */
		pff_test_memset_attempts_before_fail = test_number;

		result = libpff_index_value_initialize(
		          &index_value,
		          &error );

		if( pff_test_memset_attempts_before_fail != -1 )
		{
			pff_test_memset_attempts_before_fail = -1;

			if( index_value != NULL )
			{
				libpff_index_value_free(
				 &index_value,
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
			 "index_value",
			 index_value );

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
	if( index_value != NULL )
	{
		libpff_index_value_free(
		 &index_value,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_index_value_free function
 * Returns 1 if successful or 0 if not
 */
int pff_test_index_value_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libpff_index_value_free(
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

/* Tests the libpff_index_value_compare function
 * Returns 1 if successful or 0 if not
 */
int pff_test_index_value_compare(
     void )
{
	libcerror_error_t *error                 = NULL;
	libpff_index_value_t *first_index_value  = NULL;
	libpff_index_value_t *second_index_value = NULL;
	int result                               = 0;

	/* Initialize test
	 */
	result = libpff_index_value_initialize(
	          &first_index_value,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "first_index_value",
	 first_index_value );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_index_value_initialize(
	          &second_index_value,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "second_index_value",
	 second_index_value );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libpff_index_value_compare(
	          first_index_value,
	          second_index_value,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBFDATA_COMPARE_EQUAL );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libpff_index_value_compare(
	          NULL,
	          second_index_value,
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

	result = libpff_index_value_compare(
	          first_index_value,
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
	result = libpff_index_value_free(
	          &second_index_value,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "second_index_value",
	 second_index_value );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_index_value_free(
	          &first_index_value,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "first_index_value",
	 first_index_value );

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
	if( second_index_value != NULL )
	{
		libpff_index_value_free(
		 &second_index_value,
		 NULL );
	}
	if( first_index_value != NULL )
	{
		libpff_index_value_free(
		 &first_index_value,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_index_value_read_data function
 * Returns 1 if successful or 0 if not
 */
int pff_test_index_value_read_data(
     void )
{
	libcerror_error_t *error          = NULL;
	libpff_index_value_t *index_value = NULL;
	libpff_io_handle_t *io_handle     = NULL;
	int result                        = 0;

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

	io_handle->file_type = LIBPFF_FILE_TYPE_64BIT;

	result = libpff_index_value_initialize(
	          &index_value,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "index_value",
	 index_value );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libpff_index_value_read_data(
	          index_value,
	          io_handle,
	          LIBPFF_INDEX_TYPE_DESCRIPTOR,
	          pff_test_index_value_data1,
	          32,
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
	result = libpff_index_value_read_data(
	          NULL,
	          io_handle,
	          LIBPFF_INDEX_TYPE_DESCRIPTOR,
	          pff_test_index_value_data1,
	          32,
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

	result = libpff_index_value_read_data(
	          index_value,
	          NULL,
	          LIBPFF_INDEX_TYPE_DESCRIPTOR,
	          pff_test_index_value_data1,
	          32,
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

	result = libpff_index_value_read_data(
	          index_value,
	          io_handle,
	          LIBPFF_INDEX_TYPE_DESCRIPTOR,
	          NULL,
	          32,
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

	result = libpff_index_value_read_data(
	          index_value,
	          io_handle,
	          LIBPFF_INDEX_TYPE_DESCRIPTOR,
	          pff_test_index_value_data1,
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

	result = libpff_index_value_read_data(
	          index_value,
	          io_handle,
	          LIBPFF_INDEX_TYPE_DESCRIPTOR,
	          pff_test_index_value_data1,
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

#if defined( HAVE_PFF_TEST_MEMORY )

	/* Test pff_test_index_value_read_data with memcpy failing
	 */
	pff_test_memcpy_attempts_before_fail = 0;

	result = libpff_index_value_read_data(
	          index_value,
	          io_handle,
	          LIBPFF_INDEX_TYPE_DESCRIPTOR,
	          pff_test_index_value_data1,
	          32,
	          &error );

	if( pff_test_memcpy_attempts_before_fail != -1 )
	{
		pff_test_memcpy_attempts_before_fail = -1;
	}
	else
	{
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
#endif /* defined( HAVE_PFF_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libpff_index_value_free(
	          &index_value,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "index_value",
	 index_value );

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
	if( index_value != NULL )
	{
		libpff_index_value_free(
		 &index_value,
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
	 "libpff_index_value_initialize",
	 pff_test_index_value_initialize );

	PFF_TEST_RUN(
	 "libpff_index_value_free",
	 pff_test_index_value_free );

	PFF_TEST_RUN(
	 "libpff_index_value_compare",
	 pff_test_index_value_compare );

	PFF_TEST_RUN(
	 "libpff_index_value_read_data",
	 pff_test_index_value_read_data );

#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */
}

