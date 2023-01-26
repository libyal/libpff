/*
 * Library record_set type test program
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

#include "../libpff/libpff_definitions.h"
#include "../libpff/libpff_mapi.h"
#include "../libpff/libpff_record_set.h"

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

/* Tests the libpff_record_set_initialize function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_set_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libpff_record_set_t *record_set = NULL;
	int result                      = 0;

#if defined( HAVE_PFF_TEST_MEMORY )
	int number_of_malloc_fail_tests = 4;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libpff_record_set_initialize(
	          &record_set,
	          1,
	          LIBPFF_CODEPAGE_WINDOWS_1251,
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

	result = libpff_internal_record_set_free(
	          (libpff_internal_record_set_t **) &record_set,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "record_set",
	 record_set );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libpff_record_set_initialize(
	          NULL,
	          1,
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

	record_set = (libpff_record_set_t *) 0x12345678UL;

	result = libpff_record_set_initialize(
	          &record_set,
	          1,
	          LIBPFF_CODEPAGE_WINDOWS_1251,
	          &error );

	record_set = NULL;

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libpff_record_set_initialize(
	          &record_set,
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

#if defined( HAVE_PFF_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libpff_record_set_initialize with malloc failing
		 * Test libpff_record_set_initialize with malloc failing in libcdata_array_initialize (2x)
		 * Test libpff_record_set_initialize with malloc failing in libpff_record_entry_initialize
		 */
		pff_test_malloc_attempts_before_fail = test_number;

		result = libpff_record_set_initialize(
		          &record_set,
		          1,
		          LIBPFF_CODEPAGE_WINDOWS_1251,
		          &error );

		if( pff_test_malloc_attempts_before_fail != -1 )
		{
			pff_test_malloc_attempts_before_fail = -1;

			if( record_set != NULL )
			{
				libpff_internal_record_set_free(
				 (libpff_internal_record_set_t **) &record_set,
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
			 "record_set",
			 record_set );

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
		/* Test libpff_record_set_initialize with memset failing
		 */
		pff_test_memset_attempts_before_fail = test_number;

		result = libpff_record_set_initialize(
		          &record_set,
		          1,
		          LIBPFF_CODEPAGE_WINDOWS_1251,
		          &error );

		if( pff_test_memset_attempts_before_fail != -1 )
		{
			pff_test_memset_attempts_before_fail = -1;

			if( record_set != NULL )
			{
				libpff_internal_record_set_free(
				 (libpff_internal_record_set_t **) &record_set,
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
			 "record_set",
			 record_set );

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
	if( record_set != NULL )
	{
		libpff_internal_record_set_free(
		 (libpff_internal_record_set_t **) &record_set,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */

/* Tests the libpff_record_set_free function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_set_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libpff_record_set_free(
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

/* Tests the libpff_internal_record_set_free function
 * Returns 1 if successful or 0 if not
 */
int pff_test_internal_record_set_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libpff_internal_record_set_free(
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

/* Tests the libpff_record_set_clone function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_set_clone(
     void )
{
	libcerror_error_t *error                    = NULL;
	libpff_record_set_t *destination_record_set = NULL;
	libpff_record_set_t *source_record_set      = NULL;
	int result                                  = 0;

#if defined( HAVE_PFF_TEST_MEMORY )
	int number_of_malloc_fail_tests             = 2;
	int number_of_memset_fail_tests             = 1;
	int test_number                             = 0;
#endif

	/* Initialize test
	 */
	result = libpff_record_set_initialize(
	          &source_record_set,
	          1,
	          LIBPFF_CODEPAGE_WINDOWS_1251,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "source_record_set",
	 source_record_set );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libpff_record_set_clone(
	          &destination_record_set,
	          source_record_set,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "destination_record_set",
	 destination_record_set );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_internal_record_set_free(
	          (libpff_internal_record_set_t **) &destination_record_set,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "destination_record_set",
	 destination_record_set );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_record_set_clone(
	          &destination_record_set,
	          NULL,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "destination_record_set",
	 destination_record_set );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libpff_record_set_clone(
	          NULL,
	          source_record_set,
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

	destination_record_set = (libpff_record_set_t *) 0x12345678UL;

	result = libpff_record_set_clone(
	          &destination_record_set,
	          source_record_set,
	          &error );

	destination_record_set = NULL;

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
		/* Test libpff_record_set_clone with malloc failing
		 * Test libpff_record_set_clone with malloc failing in libcdata_array_clone
		 */
		pff_test_malloc_attempts_before_fail = test_number;

		result = libpff_record_set_clone(
		          &destination_record_set,
		          source_record_set,
		          &error );

		if( pff_test_malloc_attempts_before_fail != -1 )
		{
			pff_test_malloc_attempts_before_fail = -1;

			if( destination_record_set != NULL )
			{
				libpff_internal_record_set_free(
				 (libpff_internal_record_set_t **) &destination_record_set,
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
			 "destination_record_set",
			 destination_record_set );

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
		/* Test libpff_record_set_clone with memset failing
		 */
		pff_test_memset_attempts_before_fail = test_number;

		result = libpff_record_set_clone(
		          &destination_record_set,
		          source_record_set,
		          &error );

		if( pff_test_memset_attempts_before_fail != -1 )
		{
			pff_test_memset_attempts_before_fail = -1;

			if( destination_record_set != NULL )
			{
				libpff_internal_record_set_free(
				 (libpff_internal_record_set_t **) &destination_record_set,
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
			 "destination_record_set",
			 destination_record_set );

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
	result = libpff_internal_record_set_free(
	          (libpff_internal_record_set_t **) &source_record_set,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "source_record_set",
	 source_record_set );

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
	if( destination_record_set != NULL )
	{
		libpff_internal_record_set_free(
		 (libpff_internal_record_set_t **) &destination_record_set,
		 NULL );
	}
	if( source_record_set != NULL )
	{
		libpff_internal_record_set_free(
		 (libpff_internal_record_set_t **) &source_record_set,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_record_set_resize function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_set_resize(
     void )
{
	libcerror_error_t *error        = NULL;
	libpff_record_set_t *record_set = NULL;
	int result                      = 0;

	/* Initialize test
	 */
	result = libpff_record_set_initialize(
	          &record_set,
	          1,
	          LIBPFF_CODEPAGE_WINDOWS_1251,
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

	/* Test regular cases
	 */
	result = libpff_record_set_resize(
	          record_set,
	          10,
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
	result = libpff_record_set_resize(
	          NULL,
	          10,
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
	result = libpff_internal_record_set_free(
	          (libpff_internal_record_set_t **) &record_set,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "record_set",
	 record_set );

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
	if( record_set != NULL )
	{
		libpff_internal_record_set_free(
		 (libpff_internal_record_set_t **) &record_set,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_record_set_get_number_of_entries function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_set_get_number_of_entries(
     libpff_record_set_t *record_set )
{
	libcerror_error_t *error = NULL;
	int number_of_entries    = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libpff_record_set_get_number_of_entries(
	          record_set,
	          &number_of_entries,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "number_of_entries",
	 number_of_entries,
	 10 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libpff_record_set_get_number_of_entries(
	          NULL,
	          &number_of_entries,
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

	result = libpff_record_set_get_number_of_entries(
	          record_set,
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

/* Tests the libpff_record_set_get_entry_by_index function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_set_get_entry_by_index(
     libpff_record_set_t *record_set )
{
	libcerror_error_t *error            = NULL;
	libpff_record_entry_t *record_entry = NULL;
	int result                          = 0;

	/* Test regular cases
	 */
	result = libpff_record_set_get_entry_by_index(
	          record_set,
	          0,
	          &record_entry,
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
	result = libpff_record_set_get_entry_by_index(
	          NULL,
	          0,
	          &record_entry,
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

	result = libpff_record_set_get_entry_by_index(
	          record_set,
	          99,
	          &record_entry,
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

/* Tests the libpff_record_set_get_entry_by_type function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_set_get_entry_by_type(
     libpff_record_set_t *record_set )
{
	libcerror_error_t *error            = NULL;
	libpff_record_entry_t *record_entry = NULL;
	int result                          = 0;

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libpff_record_set_get_entry_by_type(
	          NULL,
	          LIBPFF_ENTRY_TYPE_MESSAGE_CLASS,
	          LIBPFF_VALUE_TYPE_STRING_UNICODE,
	          &record_entry,
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

	result = libpff_record_set_get_entry_by_type(
	          record_set,
	          LIBPFF_ENTRY_TYPE_MESSAGE_CLASS,
	          LIBPFF_VALUE_TYPE_STRING_UNICODE,
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

/* Tests the libpff_record_set_get_entry_by_utf8_name function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_set_get_entry_by_utf8_name(
     libpff_record_set_t *record_set )
{
	uint8_t utf8_name[ 5 ]              = { 'T', 'e', 's', 't', 0 };
	libcerror_error_t *error            = NULL;
	libpff_record_entry_t *record_entry = NULL;
	int result                          = 0;

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libpff_record_set_get_entry_by_utf8_name(
	          NULL,
	          utf8_name,
	          4,
	          LIBPFF_VALUE_TYPE_STRING_UNICODE,
	          &record_entry,
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

	result = libpff_record_set_get_entry_by_utf8_name(
	          record_set,
	          utf8_name,
	          4,
	          LIBPFF_VALUE_TYPE_STRING_UNICODE,
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

/* Tests the libpff_record_set_get_entry_by_utf16_name function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_set_get_entry_by_utf16_name(
     libpff_record_set_t *record_set )
{
	uint16_t utf16_name[ 5 ]            = { 'T', 'e', 's', 't', 0 };
	libcerror_error_t *error            = NULL;
	libpff_record_entry_t *record_entry = NULL;
	int result                          = 0;

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libpff_record_set_get_entry_by_utf16_name(
	          NULL,
	          utf16_name,
	          4,
	          LIBPFF_VALUE_TYPE_STRING_UNICODE,
	          &record_entry,
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

	result = libpff_record_set_get_entry_by_utf16_name(
	          record_set,
	          utf16_name,
	          4,
	          LIBPFF_VALUE_TYPE_STRING_UNICODE,
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

	libcerror_error_t *error        = NULL;
	libpff_record_set_t *record_set = NULL;
	int result                      = 0;

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */
#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */

	PFF_TEST_UNREFERENCED_PARAMETER( argc )
	PFF_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

	PFF_TEST_RUN(
	 "libpff_record_set_initialize",
	 pff_test_record_set_initialize );

#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */

	PFF_TEST_RUN(
	 "libpff_record_set_free",
	 pff_test_record_set_free );

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

	PFF_TEST_RUN(
	 "libpff_internal_record_set_free",
	 pff_test_internal_record_set_free );

	PFF_TEST_RUN(
	 "libpff_record_set_clone",
	 pff_test_record_set_clone );

	PFF_TEST_RUN(
	 "libpff_record_set_resize",
	 pff_test_record_set_resize );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize test
	 */
	result = libpff_record_set_initialize(
	          &record_set,
	          10,
	          LIBPFF_CODEPAGE_WINDOWS_1251,
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

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_record_set_get_number_of_entries",
	 pff_test_record_set_get_number_of_entries,
	 record_set );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_record_set_get_entry_by_index",
	 pff_test_record_set_get_entry_by_index,
	 record_set );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_record_set_get_entry_by_type",
	 pff_test_record_set_get_entry_by_type,
	 record_set );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_record_set_get_entry_by_utf8_name",
	 pff_test_record_set_get_entry_by_utf8_name,
	 record_set );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_record_set_get_entry_by_utf16_name",
	 pff_test_record_set_get_entry_by_utf16_name,
	 record_set );

	/* Clean up
	 */
	result = libpff_internal_record_set_free(
	          (libpff_internal_record_set_t **) &record_set,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "record_set",
	 record_set );

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
	if( record_set != NULL )
	{
		libpff_internal_record_set_free(
		 (libpff_internal_record_set_t **) &record_set,
		 NULL );
	}
#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */
#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */

	return( EXIT_FAILURE );
}

