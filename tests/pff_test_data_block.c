/*
 * Library data_block type test program
 *
 * Copyright (C) 2008-2018, Joachim Metz <joachim.metz@gmail.com>
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

#include "../libpff/libpff_data_block.h"

uint8_t pff_test_data_block_footer_32bit[ 12 ] = {
	0x68, 0x05, 0xbe, 0x6f, 0x38, 0x0f, 0x00, 0x00, 0x7b, 0x67, 0x66, 0x1d };

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

/* Tests the libpff_data_block_free function
 * Returns 1 if successful or 0 if not
 */
int pff_test_data_block_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libpff_data_block_free(
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

/* Tests the libpff_data_block_read_footer_data function
 * Returns 1 if successful or 0 if not
 */
int pff_test_data_block_read_footer_data(
     void )
{
	libpff_data_block_t *data_block = NULL;
	libcerror_error_t *error        = NULL;
	int result                      = 0;

	/* Initialize test
	 */

	/* Test regular cases
	 */

	/* Test error cases
	 */
	result = libpff_data_block_read_footer_data(
	          NULL,
	          pff_test_data_block_footer_32bit,
	          12,
	          LIBPFF_FILE_TYPE_32BIT,
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

	result = libpff_data_block_read_footer_data(
	          data_block,
	          NULL,
	          12,
	          LIBPFF_FILE_TYPE_32BIT,
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

	result = libpff_data_block_read_footer_data(
	          data_block,
	          pff_test_data_block_footer_32bit,
	          (size_t) SSIZE_MAX + 1,
	          LIBPFF_FILE_TYPE_32BIT,
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

	result = libpff_data_block_read_footer_data(
	          data_block,
	          pff_test_data_block_footer_32bit,
	          12,
	          0xff,
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

	result = libpff_data_block_read_footer_data(
	          data_block,
	          pff_test_data_block_footer_32bit,
	          0,
	          LIBPFF_FILE_TYPE_32BIT,
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

	/* TODO: add tests for libpff_data_block_initialize */

	PFF_TEST_RUN(
	 "libpff_data_block_free",
	 pff_test_data_block_free );

	/* TODO: add tests for libpff_data_block_clone */

	PFF_TEST_RUN(
	 "libpff_data_block_read_footer_data",
	 pff_test_data_block_read_footer_data );

	/* TODO: add tests for libpff_data_block_read_file_io_handle */

	/* TODO: add tests for libpff_data_block_read_element_data */

	/* TODO: add tests for libpff_data_block_decrypt_data */

#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

