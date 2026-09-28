/*
 * Library descriptors_index type test program
 *
 * Copyright (C) 2008-2026, Joachim Metz <joachim.metz@gmail.com>
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

#include "../libpff/libpff_descriptors_index.h"
#include "../libpff/libpff_index_value.h"
#include "../libpff/libpff_io_handle.h"

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

/* Tests the libpff_descriptors_index_free function
 * Returns 1 if successful or 0 if not
 */
int pff_test_descriptors_index_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libpff_descriptors_index_free(
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

/* Tests recovered descriptor lookup field preservation.
 * Returns 1 if successful or 0 if not.
 */
int pff_test_descriptors_index_get_index_value_by_identifier(
     void )
{
	libcerror_error_t *error                      = NULL;
	libpff_descriptors_index_t *descriptors_index = NULL;
	libpff_index_value_t *index_value             = NULL;
	libpff_index_value_t *recovered_value         = NULL;
	libpff_io_handle_t *io_handle                 = NULL;
	int result                                    = 0;

	result = libpff_io_handle_initialize(
	          &io_handle,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	result = libpff_descriptors_index_initialize(
	          &descriptors_index,
	          0,
	          0,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	result = libpff_index_value_initialize(
	          &recovered_value,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	recovered_value->identifier                   = 0x1234;
	recovered_value->data_identifier              = 0x5678;
	recovered_value->local_descriptors_identifier = 0x9abc;
	recovered_value->parent_identifier            = 0xdef0;

	result = libpff_descriptors_index_insert_recovered_index_value(
	          descriptors_index,
	          recovered_value,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	recovered_value = NULL;

	result = libpff_descriptors_index_get_index_value_by_identifier(
	          descriptors_index,
	          io_handle,
	          NULL,
	          0x1234,
	          1,
	          &index_value,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "index_value",
	 index_value );

	PFF_TEST_ASSERT_EQUAL_UINT64(
	 "index_value->data_identifier",
	 index_value->data_identifier,
	 (uint64_t) 0x5678 );

	PFF_TEST_ASSERT_EQUAL_UINT64(
	 "index_value->local_descriptors_identifier",
	 index_value->local_descriptors_identifier,
	 (uint64_t) 0x9abc );

	PFF_TEST_ASSERT_EQUAL_UINT32(
	 "index_value->parent_identifier",
	 index_value->parent_identifier,
	 (uint32_t) 0xdef0 );

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

	result = libpff_descriptors_index_free(
	          &descriptors_index,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	result = libpff_io_handle_free(
	          &io_handle,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

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
	if( recovered_value != NULL )
	{
		libpff_index_value_free(
		 &recovered_value,
		 NULL );
	}
	if( descriptors_index != NULL )
	{
		libpff_descriptors_index_free(
		 &descriptors_index,
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

	/* TODO: add tests for libpff_descriptors_index_initialize */

	PFF_TEST_RUN(
	 "libpff_descriptors_index_free",
	 pff_test_descriptors_index_free );

	/* TODO: add tests for libpff_descriptors_index_set_root_node */

	PFF_TEST_RUN(
	 "libpff_descriptors_index_get_index_value_by_identifier",
	 pff_test_descriptors_index_get_index_value_by_identifier );

#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}
