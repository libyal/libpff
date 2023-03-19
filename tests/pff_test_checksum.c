/*
 * Library checksum functions test program
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
#include "pff_test_unused.h"

#include "../libpff/libpff_checksum.h"

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

/* Tests the libpff_checksum_initialize_crc32_table function
 * Returns 1 if successful or 0 if not
 */
int pff_test_checksum_initialize_crc32_table(
     void )
{
	/* Test invocation of function only
	 */
	libpff_checksum_initialize_crc32_table(
	 0xedb88320UL );

	return( 1 );
}

/* Tests the libpff_checksum_calculate_crc32 function
 * Returns 1 if successful or 0 if not
 */
int pff_test_checksum_calculate_crc32(
     void )
{
	uint8_t data[ 16 ] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

	libcerror_error_t *error = NULL;
	uint32_t checksum        = 0;
	int result               = 0;

	libpff_checksum_crc32_table_computed = 0;

	/* Test regular cases
	 */
	result = libpff_checksum_calculate_crc32(
	          &checksum,
	          data,
	          16,
	          0,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_EQUAL_UINT32(
	 "checksum",
	 checksum,
	 (uint32_t) 0xcecee288UL );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libpff_checksum_calculate_crc32(
	          NULL,
	          data,
	          16,
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

	result = libpff_checksum_calculate_crc32(
	          &checksum,
	          NULL,
	          16,
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

	result = libpff_checksum_calculate_crc32(
	          &checksum,
	          data,
	          (size_t) SSIZE_MAX + 1,
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

/* Tests the libpff_checksum_calculate_weak_crc32 function
 * Returns 1 if successful or 0 if not
 */
int pff_test_checksum_calculate_weak_crc32(
     void )
{
	uint8_t data[ 16 ] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

	libcerror_error_t *error = NULL;
	uint32_t checksum        = 0;
	int result               = 0;

	libpff_checksum_crc32_table_computed = 0;

	/* Test regular cases
	 */
	result = libpff_checksum_calculate_weak_crc32(
	          &checksum,
	          data,
	          16,
	          0,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_EQUAL_UINT32(
	 "checksum",
	 checksum,
	 (uint32_t) 0x2275a9ddUL );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libpff_checksum_calculate_weak_crc32(
	          NULL,
	          data,
	          16,
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

	result = libpff_checksum_calculate_weak_crc32(
	          &checksum,
	          NULL,
	          16,
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

	result = libpff_checksum_calculate_weak_crc32(
	          &checksum,
	          data,
	          (size_t) SSIZE_MAX + 1,
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
	PFF_TEST_UNREFERENCED_PARAMETER( argc )
	PFF_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

	PFF_TEST_RUN(
	 "libpff_checksum_initialize_crc32_table",
	 pff_test_checksum_initialize_crc32_table );

	PFF_TEST_RUN(
	 "libpff_checksum_calculate_crc32",
	 pff_test_checksum_calculate_crc32 );

	PFF_TEST_RUN(
	 "libpff_checksum_calculate_weak_crc32",
	 pff_test_checksum_calculate_weak_crc32 );

#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */
}

