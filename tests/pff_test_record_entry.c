/*
 * Library record_entry type test program
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

#include "../libpff/libpff_record_entry.h"

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

/* Tests the libpff_record_entry_initialize function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_entry_initialize(
     void )
{
	libcerror_error_t *error            = NULL;
	libpff_record_entry_t *record_entry = NULL;
	int result                          = 0;

#if defined( HAVE_PFF_TEST_MEMORY )
	int number_of_malloc_fail_tests     = 1;
	int number_of_memset_fail_tests     = 1;
	int test_number                     = 0;
#endif

	/* Test regular cases
	 */
	result = libpff_record_entry_initialize(
	          &record_entry,
	          LIBPFF_CODEPAGE_WINDOWS_1251,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "record_entry",
	 record_entry );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_internal_record_entry_free(
	          (libpff_internal_record_entry_t **) &record_entry,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "record_entry",
	 record_entry );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libpff_record_entry_initialize(
	          NULL,
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

	record_entry = (libpff_record_entry_t *) 0x12345678UL;

	result = libpff_record_entry_initialize(
	          &record_entry,
	          LIBPFF_CODEPAGE_WINDOWS_1251,
	          &error );

	record_entry = NULL;

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
		/* Test libpff_record_entry_initialize with malloc failing
		 */
		pff_test_malloc_attempts_before_fail = test_number;

		result = libpff_record_entry_initialize(
		          &record_entry,
		          LIBPFF_CODEPAGE_WINDOWS_1251,
		          &error );

		if( pff_test_malloc_attempts_before_fail != -1 )
		{
			pff_test_malloc_attempts_before_fail = -1;

			if( record_entry != NULL )
			{
				libpff_internal_record_entry_free(
				 (libpff_internal_record_entry_t **) &record_entry,
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
			 "record_entry",
			 record_entry );

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
		/* Test libpff_record_entry_initialize with memset failing
		 */
		pff_test_memset_attempts_before_fail = test_number;

		result = libpff_record_entry_initialize(
		          &record_entry,
		          LIBPFF_CODEPAGE_WINDOWS_1251,
		          &error );

		if( pff_test_memset_attempts_before_fail != -1 )
		{
			pff_test_memset_attempts_before_fail = -1;

			if( record_entry != NULL )
			{
				libpff_internal_record_entry_free(
				 (libpff_internal_record_entry_t **) &record_entry,
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
			 "record_entry",
			 record_entry );

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
	if( record_entry != NULL )
	{
		libpff_internal_record_entry_free(
		 (libpff_internal_record_entry_t **) &record_entry,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */

/* Tests the libpff_record_entry_free function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_entry_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libpff_record_entry_free(
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

/* Tests the libpff_internal_record_entry_free function
 * Returns 1 if successful or 0 if not
 */
int pff_test_internal_record_entry_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libpff_internal_record_entry_free(
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

/* Tests the libpff_record_entry_clone function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_entry_clone(
     void )
{
	libcerror_error_t *error                        = NULL;
	libpff_record_entry_t *destination_record_entry = NULL;
	libpff_record_entry_t *source_record_entry      = NULL;
	int result                                      = 0;

#if defined( HAVE_PFF_TEST_MEMORY )
	int number_of_malloc_fail_tests                 = 2;
	int number_of_memcpy_fail_tests                 = 2;
	int test_number                                 = 0;
#endif

	/* Initialize test
	 */
	result = libpff_record_entry_initialize(
	          &source_record_entry,
	          LIBPFF_CODEPAGE_WINDOWS_1251,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "source_record_entry",
	 source_record_entry );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libpff_record_entry_clone(
	          &destination_record_entry,
	          source_record_entry,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "destination_record_entry",
	 destination_record_entry );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_internal_record_entry_free(
	          (libpff_internal_record_entry_t **) &destination_record_entry,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "destination_record_entry",
	 destination_record_entry );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_record_entry_clone(
	          &destination_record_entry,
	          NULL,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "destination_record_entry",
	 destination_record_entry );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libpff_record_entry_clone(
	          NULL,
	          source_record_entry,
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

	destination_record_entry = (libpff_record_entry_t *) 0x12345678UL;

	result = libpff_record_entry_clone(
	          &destination_record_entry,
	          source_record_entry,
	          &error );

	destination_record_entry = NULL;

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
		/* Test libpff_record_entry_clone with malloc failing in libpff_record_entry_initialize
		 * Test libpff_record_entry_clone with malloc failing
		 */
		pff_test_malloc_attempts_before_fail = test_number;

		result = libpff_record_entry_clone(
		          &destination_record_entry,
		          source_record_entry,
		          &error );

		if( pff_test_malloc_attempts_before_fail != -1 )
		{
			pff_test_malloc_attempts_before_fail = -1;

			if( destination_record_entry != NULL )
			{
				libpff_internal_record_entry_free(
				 (libpff_internal_record_entry_t **) &destination_record_entry,
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
			 "destination_record_entry",
			 destination_record_entry );

			PFF_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
	for( test_number = 0;
	     test_number < number_of_memcpy_fail_tests;
	     test_number++ )
	{
		/* Test libpff_record_entry_clone with memcpy failing
		 */
		pff_test_memcpy_attempts_before_fail = test_number;

		result = libpff_record_entry_clone(
		          &destination_record_entry,
		          source_record_entry,
		          &error );

		if( pff_test_memcpy_attempts_before_fail != -1 )
		{
			pff_test_memcpy_attempts_before_fail = -1;

			if( destination_record_entry != NULL )
			{
				libpff_internal_record_entry_free(
				 (libpff_internal_record_entry_t **) &destination_record_entry,
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
			 "destination_record_entry",
			 destination_record_entry );

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
	result = libpff_internal_record_entry_free(
	          (libpff_internal_record_entry_t **) &source_record_entry,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "source_record_entry",
	 source_record_entry );

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
	if( destination_record_entry != NULL )
	{
		libpff_internal_record_entry_free(
		 (libpff_internal_record_entry_t **) &destination_record_entry,
		 NULL );
	}
	if( source_record_entry != NULL )
	{
		libpff_internal_record_entry_free(
		 (libpff_internal_record_entry_t **) &source_record_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_record_entry_get_entry_type function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_entry_get_entry_type(
     libpff_record_entry_t *record_entry )
{
	libcerror_error_t *error = NULL;
	uint32_t entry_type      = 0;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO add test that returns entry type */
	result = libpff_record_entry_get_entry_type(
	          record_entry,
	          &entry_type,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libpff_record_entry_get_entry_type(
	          NULL,
	          &entry_type,
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

	result = libpff_record_entry_get_entry_type(
	          record_entry,
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

/* Tests the libpff_record_entry_get_value_type function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_entry_get_value_type(
     libpff_record_entry_t *record_entry )
{
	libcerror_error_t *error = NULL;
	uint32_t value_type      = 0;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libpff_record_entry_get_value_type(
	          NULL,
	          &value_type,
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

	result = libpff_record_entry_get_value_type(
	          record_entry,
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

/* Tests the libpff_record_entry_get_name_to_id_map_entry function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_entry_get_name_to_id_map_entry(
     libpff_record_entry_t *record_entry )
{
	libcerror_error_t *error                            = NULL;
	libpff_name_to_id_map_entry_t *name_to_id_map_entry = NULL;
	int result                                          = 0;

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libpff_record_entry_get_name_to_id_map_entry(
	          NULL,
	          &name_to_id_map_entry,
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

	result = libpff_record_entry_get_name_to_id_map_entry(
	          record_entry,
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

/* Tests the libpff_record_entry_get_data_size function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_entry_get_data_size(
     libpff_record_entry_t *record_entry )
{
	libcerror_error_t *error = NULL;
	size_t data_size         = 0;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libpff_record_entry_get_data_size(
	          NULL,
	          &data_size,
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

	result = libpff_record_entry_get_data_size(
	          record_entry,
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

/* Tests the libpff_record_entry_get_value_data function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_entry_get_value_data(
     libpff_record_entry_t *record_entry )
{
	libcerror_error_t *error = NULL;
	uint8_t *value_data      = NULL;
	size_t value_data_size   = 0;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libpff_record_entry_get_value_data(
	          NULL,
	          &value_data,
	          &value_data_size,
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

	result = libpff_record_entry_get_value_data(
	          record_entry,
	          NULL,
	          &value_data_size,
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

	result = libpff_record_entry_get_value_data(
	          record_entry,
	          &value_data,
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

/* Tests the libpff_record_entry_set_value_data function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_entry_set_value_data(
     void )
{
	uint8_t value_data[ 32 ];

	libcerror_error_t *error            = NULL;
	libpff_record_entry_t *record_entry = NULL;
	int result                          = 0;

	/* Initialize test
	 */
	result = libpff_record_entry_initialize(
	          &record_entry,
	          LIBPFF_CODEPAGE_WINDOWS_1251,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "record_entry",
	 record_entry );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libpff_record_entry_set_value_data(
	          record_entry,
	          (uint8_t *) "test value",
	          11,
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
	result = libpff_record_entry_set_value_data(
	          NULL,
	          value_data,
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

	result = libpff_record_entry_set_value_data(
	          record_entry,
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

	result = libpff_record_entry_set_value_data(
	          record_entry,
	          value_data,
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
	result = libpff_internal_record_entry_free(
	          (libpff_internal_record_entry_t **) &record_entry,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "record_entry",
	 record_entry );

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
	if( record_entry != NULL )
	{
		libpff_internal_record_entry_free(
		 (libpff_internal_record_entry_t **) &record_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_record_entry_get_data function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_entry_get_data(
     libpff_record_entry_t *record_entry )
{
	uint8_t data[ 32 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libpff_record_entry_get_data(
	          NULL,
	          data,
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

	result = libpff_record_entry_get_data(
	          record_entry,
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

	result = libpff_record_entry_get_data(
	          record_entry,
	          data,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libpff_record_entry_get_data_as_boolean function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_entry_get_data_as_boolean(
     libpff_record_entry_t *record_entry )
{
	libcerror_error_t *error = NULL;
	uint8_t value_boolean    = 0;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libpff_record_entry_get_data_as_boolean(
	          NULL,
	          &value_boolean,
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

	result = libpff_record_entry_get_data_as_boolean(
	          record_entry,
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

/* Tests the libpff_record_entry_get_data_as_16bit_integer function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_entry_get_data_as_16bit_integer(
     libpff_record_entry_t *record_entry )
{
	libcerror_error_t *error = NULL;
	uint16_t value_16bit     = 0;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libpff_record_entry_get_data_as_16bit_integer(
	          NULL,
	          &value_16bit,
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

	result = libpff_record_entry_get_data_as_16bit_integer(
	          record_entry,
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
	if( record_entry != NULL )
	{
		libpff_internal_record_entry_free(
		 (libpff_internal_record_entry_t **) &record_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_record_entry_get_data_as_32bit_integer function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_entry_get_data_as_32bit_integer(
     libpff_record_entry_t *record_entry )
{
	libcerror_error_t *error = NULL;
	uint32_t value_32bit     = 0;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libpff_record_entry_get_data_as_32bit_integer(
	          NULL,
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

	result = libpff_record_entry_get_data_as_32bit_integer(
	          record_entry,
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

/* Tests the libpff_record_entry_get_data_as_64bit_integer function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_entry_get_data_as_64bit_integer(
     libpff_record_entry_t *record_entry )
{
	libcerror_error_t *error = NULL;
	uint64_t value_64bit     = 0;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libpff_record_entry_get_data_as_64bit_integer(
	          NULL,
	          &value_64bit,
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

	result = libpff_record_entry_get_data_as_64bit_integer(
	          record_entry,
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

/* Tests the libpff_record_entry_get_data_as_filetime function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_entry_get_data_as_filetime(
     libpff_record_entry_t *record_entry )
{
	libcerror_error_t *error = NULL;
	uint64_t filetime        = 0;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libpff_record_entry_get_data_as_filetime(
	          NULL,
	          &filetime,
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

	result = libpff_record_entry_get_data_as_filetime(
	          record_entry,
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

/* Tests the libpff_record_entry_get_data_as_floatingtime function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_entry_get_data_as_floatingtime(
     libpff_record_entry_t *record_entry )
{
	libcerror_error_t *error = NULL;
	uint64_t floatingtime    = 0;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libpff_record_entry_get_data_as_floatingtime(
	          NULL,
	          &floatingtime,
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

	result = libpff_record_entry_get_data_as_floatingtime(
	          record_entry,
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

/* Tests the libpff_record_entry_get_data_as_size function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_entry_get_data_as_size(
     libpff_record_entry_t *record_entry )
{
	libcerror_error_t *error = NULL;
	size64_t value_size      = 0;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libpff_record_entry_get_data_as_size(
	          NULL,
	          &value_size,
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

	result = libpff_record_entry_get_data_as_size(
	          record_entry,
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

/* Tests the libpff_record_entry_get_data_as_floating_point function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_entry_get_data_as_floating_point(
     libpff_record_entry_t *record_entry )
{
	libcerror_error_t *error    = NULL;
	double value_floating_point = 0.0;
	int result                  = 0;

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libpff_record_entry_get_data_as_floating_point(
	          NULL,
	          &value_floating_point,
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

	result = libpff_record_entry_get_data_as_floating_point(
	          record_entry,
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

/* Tests the libpff_record_entry_get_data_as_utf8_string_size_with_codepage function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_entry_get_data_as_utf8_string_size_with_codepage(
     libpff_record_entry_t *record_entry )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libpff_record_entry_get_data_as_utf8_string_size_with_codepage(
	          NULL,
	          LIBPFF_CODEPAGE_WINDOWS_1251,
	          &utf8_string_size,
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

	result = libpff_record_entry_get_data_as_utf8_string_size_with_codepage(
	          record_entry,
	          LIBPFF_CODEPAGE_WINDOWS_1251,
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

/* Tests the libpff_record_entry_get_data_as_utf8_string_size function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_entry_get_data_as_utf8_string_size(
     libpff_record_entry_t *record_entry )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libpff_record_entry_get_data_as_utf8_string_size(
	          NULL,
	          &utf8_string_size,
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

	result = libpff_record_entry_get_data_as_utf8_string_size(
	          record_entry,
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

/* Tests the libpff_record_entry_get_data_as_utf8_string function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_entry_get_data_as_utf8_string(
     libpff_record_entry_t *record_entry )
{
	uint8_t utf8_string[ 32 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libpff_record_entry_get_data_as_utf8_string(
	          NULL,
	          utf8_string,
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

	result = libpff_record_entry_get_data_as_utf8_string(
	          record_entry,
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

	result = libpff_record_entry_get_data_as_utf8_string(
	          record_entry,
	          utf8_string,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libpff_record_entry_get_data_as_utf16_string_size_with_codepage function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_entry_get_data_as_utf16_string_size_with_codepage(
     libpff_record_entry_t *record_entry )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libpff_record_entry_get_data_as_utf16_string_size_with_codepage(
	          NULL,
	          LIBPFF_CODEPAGE_WINDOWS_1251,
	          &utf16_string_size,
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

	result = libpff_record_entry_get_data_as_utf16_string_size_with_codepage(
	          record_entry,
	          LIBPFF_CODEPAGE_WINDOWS_1251,
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

/* Tests the libpff_record_entry_get_data_as_utf16_string_size function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_entry_get_data_as_utf16_string_size(
     libpff_record_entry_t *record_entry )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libpff_record_entry_get_data_as_utf16_string_size(
	          NULL,
	          &utf16_string_size,
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

	result = libpff_record_entry_get_data_as_utf16_string_size(
	          record_entry,
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

/* Tests the libpff_record_entry_get_data_as_utf16_string function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_entry_get_data_as_utf16_string(
     libpff_record_entry_t *record_entry )
{
	uint16_t utf16_string[ 32 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libpff_record_entry_get_data_as_utf16_string(
	          NULL,
	          utf16_string,
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

	result = libpff_record_entry_get_data_as_utf16_string(
	          record_entry,
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

	result = libpff_record_entry_get_data_as_utf16_string(
	          record_entry,
	          utf16_string,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libpff_record_entry_get_data_as_guid function
 * Returns 1 if successful or 0 if not
 */
int pff_test_record_entry_get_data_as_guid(
     libpff_record_entry_t *record_entry )
{
	uint8_t guid_data[ 16 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libpff_record_entry_get_data_as_guid(
	          NULL,
	          guid_data,
	          16,
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

	result = libpff_record_entry_get_data_as_guid(
	          record_entry,
	          NULL,
	          16,
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

	result = libpff_record_entry_get_data_as_guid(
	          record_entry,
	          guid_data,
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

	libcerror_error_t *error            = NULL;
	libpff_record_entry_t *record_entry = NULL;
	int result                          = 0;

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */
#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */

	PFF_TEST_UNREFERENCED_PARAMETER( argc )
	PFF_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

	PFF_TEST_RUN(
	 "libpff_record_entry_initialize",
	 pff_test_record_entry_initialize );

#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */

	PFF_TEST_RUN(
	 "libpff_record_entry_free",
	 pff_test_record_entry_free );

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

	PFF_TEST_RUN(
	 "libpff_internal_record_entry_free",
	 pff_test_internal_record_entry_free );

	PFF_TEST_RUN(
	 "libpff_record_entry_clone",
	 pff_test_record_entry_clone );

	PFF_TEST_RUN(
	 "libpff_record_entry_set_value_data",
	 pff_test_record_entry_set_value_data );

	/* TODO: add tests for libpff_record_entry_set_value_data_from_list */

	/* TODO: add tests for libpff_record_entry_set_value_data_from_stream */

	/* TODO: add tests for libpff_record_entry_read_buffer */

	/* TODO: add tests for libpff_record_entry_seek_offset */

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize test
	 */
	result = libpff_record_entry_initialize(
	          &record_entry,
	          LIBPFF_CODEPAGE_WINDOWS_1251,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "record_entry",
	 record_entry );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_record_entry_set_value_data(
	          record_entry,
	          (uint8_t *) "test value",
	          11,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_record_entry_get_entry_type",
	 pff_test_record_entry_get_entry_type,
	 record_entry );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_record_entry_get_value_type",
	 pff_test_record_entry_get_value_type,
	 record_entry );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_record_entry_get_name_to_id_map_entry",
	 pff_test_record_entry_get_name_to_id_map_entry,
	 record_entry );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_record_entry_get_data_size",
	 pff_test_record_entry_get_data_size,
	 record_entry );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_record_entry_get_value_data",
	 pff_test_record_entry_get_value_data,
	 record_entry );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_record_entry_get_data",
	 pff_test_record_entry_get_data,
	 record_entry );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_record_entry_get_data_as_boolean",
	 pff_test_record_entry_get_data_as_boolean,
	 record_entry );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_record_entry_get_data_as_16bit_integer",
	 pff_test_record_entry_get_data_as_16bit_integer,
	 record_entry );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_record_entry_get_data_as_32bit_integer",
	 pff_test_record_entry_get_data_as_32bit_integer,
	 record_entry );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_record_entry_get_data_as_64bit_integer",
	 pff_test_record_entry_get_data_as_64bit_integer,
	 record_entry );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_record_entry_get_data_as_filetime",
	 pff_test_record_entry_get_data_as_filetime,
	 record_entry );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_record_entry_get_data_as_floatingtime",
	 pff_test_record_entry_get_data_as_floatingtime,
	 record_entry );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_record_entry_get_data_as_size",
	 pff_test_record_entry_get_data_as_size,
	 record_entry );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_record_entry_get_data_as_floating_point",
	 pff_test_record_entry_get_data_as_floating_point,
	 record_entry );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_record_entry_get_data_as_utf8_string_size_with_codepage",
	 pff_test_record_entry_get_data_as_utf8_string_size_with_codepage,
	 record_entry );

	/* TODO: add tests for libpff_record_entry_get_data_as_utf8_string_with_codepage */

	/* TODO: add tests for libpff_record_entry_compare_value_with_utf8_string_with_codepage */

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_record_entry_get_data_as_utf8_string_size",
	 pff_test_record_entry_get_data_as_utf8_string_size,
	 record_entry );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_record_entry_get_data_as_utf8_string",
	 pff_test_record_entry_get_data_as_utf8_string,
	 record_entry );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_record_entry_get_data_as_utf16_string_size_with_codepage",
	 pff_test_record_entry_get_data_as_utf16_string_size_with_codepage,
	 record_entry );

	/* TODO: add tests for libpff_record_entry_get_data_as_utf16_string_with_codepage */

	/* TODO: add tests for libpff_record_entry_compare_value_with_utf16_string_with_codepage */

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_record_entry_get_data_as_utf16_string_size",
	 pff_test_record_entry_get_data_as_utf16_string_size,
	 record_entry );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_record_entry_get_data_as_utf16_string",
	 pff_test_record_entry_get_data_as_utf16_string,
	 record_entry );


	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_record_entry_get_data_as_guid",
	 pff_test_record_entry_get_data_as_guid,
	 record_entry );

	/* TODO: add tests for libpff_record_entry_copy_object_identifier */

	/* TODO: add tests for libpff_record_entry_get_multi_value */

	/* Clean up
	 */
	result = libpff_internal_record_entry_free(
	          (libpff_internal_record_entry_t **) &record_entry,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "record_entry",
	 record_entry );

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
	if( record_entry != NULL )
	{
		libpff_internal_record_entry_free(
		 (libpff_internal_record_entry_t **) &record_entry,
		 NULL );
	}
#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */
#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */

	return( EXIT_FAILURE );
}

