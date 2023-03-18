/*
 * Library local_descriptors_node type test program
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

#include "pff_test_functions.h"
#include "pff_test_libbfio.h"
#include "pff_test_libcerror.h"
#include "pff_test_libpff.h"
#include "pff_test_macros.h"
#include "pff_test_memory.h"
#include "pff_test_unused.h"

#include "../libpff/libpff_definitions.h"
#include "../libpff/libpff_io_handle.h"
#include "../libpff/libpff_local_descriptors_node.h"

uint8_t pff_test_local_descriptors_node_data[ 64 ] = {
	0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x92, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x48, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	/* data block padding */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	/* data block footer */
	0x20, 0x00, 0x0e, 0x76, 0x62, 0x68, 0x10, 0xe6, 0x4e, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

/* Tests the libpff_local_descriptors_node_initialize function
 * Returns 1 if successful or 0 if not
 */
int pff_test_local_descriptors_node_initialize(
     void )
{
	libcerror_error_t *error                              = NULL;
	libpff_local_descriptors_node_t *local_descriptors_node = NULL;
	int result                                            = 0;

#if defined( HAVE_PFF_TEST_MEMORY )
	int number_of_malloc_fail_tests                       = 1;
	int number_of_memset_fail_tests                       = 1;
	int test_number                                       = 0;
#endif

	/* Test regular cases
	 */
	result = libpff_local_descriptors_node_initialize(
	          &local_descriptors_node,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "local_descriptors_node",
	 local_descriptors_node );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_local_descriptors_node_free(
	          &local_descriptors_node,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "local_descriptors_node",
	 local_descriptors_node );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libpff_local_descriptors_node_initialize(
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

	local_descriptors_node = (libpff_local_descriptors_node_t *) 0x12345678UL;

	result = libpff_local_descriptors_node_initialize(
	          &local_descriptors_node,
	          &error );

	local_descriptors_node = NULL;

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
		/* Test libpff_local_descriptors_node_initialize with malloc failing
		 */
		pff_test_malloc_attempts_before_fail = test_number;

		result = libpff_local_descriptors_node_initialize(
		          &local_descriptors_node,
		          &error );

		if( pff_test_malloc_attempts_before_fail != -1 )
		{
			pff_test_malloc_attempts_before_fail = -1;

			if( local_descriptors_node != NULL )
			{
				libpff_local_descriptors_node_free(
				 &local_descriptors_node,
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
			 "local_descriptors_node",
			 local_descriptors_node );

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
		/* Test libpff_local_descriptors_node_initialize with memset failing
		 */
		pff_test_memset_attempts_before_fail = test_number;

		result = libpff_local_descriptors_node_initialize(
		          &local_descriptors_node,
		          &error );

		if( pff_test_memset_attempts_before_fail != -1 )
		{
			pff_test_memset_attempts_before_fail = -1;

			if( local_descriptors_node != NULL )
			{
				libpff_local_descriptors_node_free(
				 &local_descriptors_node,
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
			 "local_descriptors_node",
			 local_descriptors_node );

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
	if( local_descriptors_node != NULL )
	{
		libpff_local_descriptors_node_free(
		 &local_descriptors_node,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_local_descriptors_node_free function
 * Returns 1 if successful or 0 if not
 */
int pff_test_local_descriptors_node_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libpff_local_descriptors_node_free(
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

/* Tests the libpff_local_descriptors_node_read_data function
 * Returns 1 if successful or 0 if not
 */
int pff_test_local_descriptors_node_read_data(
     void )
{
	libcerror_error_t *error                              = NULL;
	libpff_io_handle_t *io_handle                         = NULL;
	libpff_local_descriptors_node_t *local_descriptors_node = NULL;
	int result                                            = 0;

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

	result = libpff_local_descriptors_node_initialize(
	          &local_descriptors_node,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "local_descriptors_node",
	 local_descriptors_node );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libpff_local_descriptors_node_read_data(
	          local_descriptors_node,
	          io_handle,
	          pff_test_local_descriptors_node_data,
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
	result = libpff_local_descriptors_node_read_data(
	          NULL,
	          io_handle,
	          pff_test_local_descriptors_node_data,
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

	result = libpff_local_descriptors_node_read_data(
	          local_descriptors_node,
	          NULL,
	          pff_test_local_descriptors_node_data,
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

	result = libpff_local_descriptors_node_read_data(
	          local_descriptors_node,
	          io_handle,
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

	result = libpff_local_descriptors_node_read_data(
	          local_descriptors_node,
	          io_handle,
	          pff_test_local_descriptors_node_data,
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
	result = libpff_local_descriptors_node_free(
	          &local_descriptors_node,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "local_descriptors_node",
	 local_descriptors_node );

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
	if( local_descriptors_node != NULL )
	{
		libpff_local_descriptors_node_free(
		 &local_descriptors_node,
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

/* Tests the libpff_local_descriptors_node_read_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int pff_test_local_descriptors_node_read_file_io_handle(
     void )
{
	libbfio_handle_t *file_io_handle                      = NULL;
	libcerror_error_t *error                              = NULL;
	libpff_io_handle_t *io_handle                         = NULL;
	libpff_local_descriptors_node_t *local_descriptors_node = NULL;
	int result                                            = 0;

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

	result = libpff_local_descriptors_node_initialize(
	          &local_descriptors_node,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "local_descriptors_node",
	 local_descriptors_node );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO handle
	 */
	result = pff_test_open_file_io_handle(
	          &file_io_handle,
	          pff_test_local_descriptors_node_data,
	          64,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libpff_local_descriptors_node_read_file_io_handle(
	          local_descriptors_node,
	          io_handle,
	          file_io_handle,
	          0,
	          0,
	          0,
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
	result = libpff_local_descriptors_node_read_file_io_handle(
	          NULL,
	          io_handle,
	          file_io_handle,
	          0,
	          0,
	          0,
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

	result = libpff_local_descriptors_node_read_file_io_handle(
	          local_descriptors_node,
	          NULL,
	          file_io_handle,
	          0,
	          0,
	          0,
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

	result = libpff_local_descriptors_node_read_file_io_handle(
	          local_descriptors_node,
	          io_handle,
	          NULL,
	          0,
	          0,
	          0,
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

	/* Clean up file IO handle
	 */
	result = pff_test_close_file_io_handle(
	          &file_io_handle,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libpff_local_descriptors_node_free(
	          &local_descriptors_node,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "local_descriptors_node",
	 local_descriptors_node );

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
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	if( local_descriptors_node != NULL )
	{
		libpff_local_descriptors_node_free(
		 &local_descriptors_node,
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

/* Tests the libpff_local_descriptors_node_get_entry_data function
 * Returns 1 if successful or 0 if not
 */
int pff_test_local_descriptors_node_get_entry_data(
     libpff_local_descriptors_node_t *local_descriptors_node )
{
	libcerror_error_t *error = NULL;
	uint8_t *entry_data      = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libpff_local_descriptors_node_get_entry_data(
	          local_descriptors_node,
	          0,
	          &entry_data,
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
	result = libpff_local_descriptors_node_get_entry_data(
	          NULL,
	          0,
	          &entry_data,
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

	result = libpff_local_descriptors_node_get_entry_data(
	          local_descriptors_node,
	          -1,
	          &entry_data,
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

	result = libpff_local_descriptors_node_get_entry_data(
	          local_descriptors_node,
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

/* Tests the libpff_local_descriptors_node_get_entry_identifier function
 * Returns 1 if successful or 0 if not
 */
int pff_test_local_descriptors_node_get_entry_identifier(
     libpff_io_handle_t *io_handle,
     libpff_local_descriptors_node_t *local_descriptors_node )
{
	libcerror_error_t *error  = NULL;
	uint64_t entry_identifier = 0;
	int result                = 0;

	/* Test regular cases
	 */
	result = libpff_local_descriptors_node_get_entry_identifier(
	          local_descriptors_node,
	          io_handle,
	          0,
	          &entry_identifier,
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
	result = libpff_local_descriptors_node_get_entry_identifier(
	          NULL,
	          io_handle,
	          0,
	          &entry_identifier,
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

	result = libpff_local_descriptors_node_get_entry_identifier(
	          local_descriptors_node,
	          NULL,
	          0,
	          &entry_identifier,
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

	result = libpff_local_descriptors_node_get_entry_identifier(
	          local_descriptors_node,
	          io_handle,
	          -1,
	          &entry_identifier,
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

	result = libpff_local_descriptors_node_get_entry_identifier(
	          local_descriptors_node,
	          io_handle,
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

/* Tests the libpff_local_descriptors_node_get_entry_sub_node_identifier function
 * Returns 1 if successful or 0 if not
 */
int pff_test_local_descriptors_node_get_entry_sub_node_identifier(
     libpff_io_handle_t *io_handle,
     libpff_local_descriptors_node_t *local_descriptors_node )
{
	libcerror_error_t *error           = NULL;
	uint64_t entry_sub_node_identifier = 0;
	int result                         = 0;

	/* Test regular cases
	 */
	result = libpff_local_descriptors_node_get_entry_sub_node_identifier(
	          local_descriptors_node,
	          io_handle,
	          0,
	          &entry_sub_node_identifier,
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
	result = libpff_local_descriptors_node_get_entry_sub_node_identifier(
	          NULL,
	          io_handle,
	          0,
	          &entry_sub_node_identifier,
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

	result = libpff_local_descriptors_node_get_entry_sub_node_identifier(
	          local_descriptors_node,
	          NULL,
	          0,
	          &entry_sub_node_identifier,
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

	result = libpff_local_descriptors_node_get_entry_sub_node_identifier(
	          local_descriptors_node,
	          io_handle,
	          -1,
	          &entry_sub_node_identifier,
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

	result = libpff_local_descriptors_node_get_entry_sub_node_identifier(
	          local_descriptors_node,
	          io_handle,
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

	libcerror_error_t *error                              = NULL;
	libpff_io_handle_t *io_handle                         = NULL;
	libpff_local_descriptors_node_t *local_descriptors_node = NULL;
	int result                                            = 0;

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */
#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */

	PFF_TEST_UNREFERENCED_PARAMETER( argc )
	PFF_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

	PFF_TEST_RUN(
	 "libpff_local_descriptors_node_initialize",
	 pff_test_local_descriptors_node_initialize );

	PFF_TEST_RUN(
	 "libpff_local_descriptors_node_free",
	 pff_test_local_descriptors_node_free );

	PFF_TEST_RUN(
	 "libpff_local_descriptors_node_read_data",
	 pff_test_local_descriptors_node_read_data );

	PFF_TEST_RUN(
	 "libpff_local_descriptors_node_read_file_io_handle",
	 pff_test_local_descriptors_node_read_file_io_handle );

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

	io_handle->file_type = LIBPFF_FILE_TYPE_64BIT;

	result = libpff_local_descriptors_node_initialize(
	          &local_descriptors_node,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "local_descriptors_node",
	 local_descriptors_node );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_local_descriptors_node_read_data(
	          local_descriptors_node,
	          io_handle,
	          pff_test_local_descriptors_node_data,
	          32,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_local_descriptors_node_get_entry_data",
	 pff_test_local_descriptors_node_get_entry_data,
	 local_descriptors_node );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_local_descriptors_node_get_entry_identifier",
	 pff_test_local_descriptors_node_get_entry_identifier,
	 io_handle,
	 local_descriptors_node );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_local_descriptors_node_get_entry_sub_node_identifier",
	 pff_test_local_descriptors_node_get_entry_sub_node_identifier,
	 io_handle,
	 local_descriptors_node );

	/* Clean up
	 */
	result = libpff_local_descriptors_node_free(
	          &local_descriptors_node,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "local_descriptors_node",
	 local_descriptors_node );

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

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

on_error:
#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( local_descriptors_node != NULL )
	{
		libpff_local_descriptors_node_free(
		 &local_descriptors_node,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libpff_io_handle_free(
		 &io_handle,
		 NULL );
	}
#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */
}

