/*
 * Library io_handle2 functions test program
 *
 * Copyright (C) 2008-2020, Joachim Metz <joachim.metz@gmail.com>
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
#include "pff_test_libfcache.h"
#include "pff_test_libfdata.h"
#include "pff_test_libpff.h"
#include "pff_test_macros.h"
#include "pff_test_unused.h"

#include "../libpff/libpff_io_handle.h"
#include "../libpff/libpff_io_handle2.h"
#include "../libpff/libpff_offsets_index.h"

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

/* Tests the libpff_io_handle_read_descriptor_data_list function
 * Returns 1 if successful or 0 if not
 */
int pff_test_io_handle_read_descriptor_data_list(
     void )
{
	libcerror_error_t *error                 = NULL;
	libfcache_cache_t *descriptor_data_cache = NULL;
	libfdata_list_t *descriptor_data_list    = NULL;
	libpff_io_handle_t *io_handle            = NULL;
	libpff_offsets_index_t *offsets_index    = NULL;
	int result                               = 0;

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

	result = libpff_offsets_index_initialize(
	          &offsets_index,
	          io_handle,
	          NULL,
	          NULL,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "offsets_index",
	 offsets_index );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
/* TODO implement
	result = libpff_io_handle_read_descriptor_data_list(
	          io_handle,
	          NULL,
	          offsets_index,
	          0,
	          0,
	          0,
	          0,
	          &descriptor_data_list,
	          &descriptor_data_cache,
	          &error );

PFF_TEST_FPRINT_ERROR( error );

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
	result = libpff_io_handle_read_descriptor_data_list(
	          NULL,
	          NULL,
	          offsets_index,
	          0,
	          0,
	          0,
	          0,
	          &descriptor_data_list,
	          &descriptor_data_cache,
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

	result = libpff_io_handle_read_descriptor_data_list(
	          io_handle,
	          NULL,
	          NULL,
	          0,
	          0,
	          0,
	          0,
	          &descriptor_data_list,
	          &descriptor_data_cache,
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

	result = libpff_io_handle_read_descriptor_data_list(
	          io_handle,
	          NULL,
	          offsets_index,
	          0,
	          0,
	          0,
	          0,
	          NULL,
	          &descriptor_data_cache,
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

	result = libpff_io_handle_read_descriptor_data_list(
	          io_handle,
	          NULL,
	          offsets_index,
	          0,
	          0,
	          0,
	          0,
	          &descriptor_data_list,
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
	result = libpff_offsets_index_free(
	          &offsets_index,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "offsets_index",
	 offsets_index );

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
	if( offsets_index != NULL )
	{
		libpff_offsets_index_free(
		 &offsets_index,
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
	 "libpff_io_handle_read_descriptor_data_list",
	 pff_test_io_handle_read_descriptor_data_list );

#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */
}

