/*
 * Library index_tree functions test program
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

#include "pff_test_functions.h"
#include "pff_test_libbfio.h"
#include "pff_test_libcerror.h"
#include "pff_test_libfcache.h"
#include "pff_test_libfdata.h"
#include "pff_test_libpff.h"
#include "pff_test_macros.h"
#include "pff_test_unused.h"

#include "../libpff/libpff_definitions.h"
#include "../libpff/libpff_index_tree.h"
#include "../libpff/libpff_io_handle.h"

uint8_t pff_test_index_tree_32bit_index_node_data[ 512 ] = {
	0x0c, 0x01, 0x00, 0x00, 0x40, 0x7c, 0x00, 0x00, 0x54, 0x00, 0x02, 0x00, 0x14, 0x01, 0x00, 0x00,
	0x00, 0x69, 0x00, 0x00, 0x26, 0x00, 0x02, 0x00, 0x24, 0x01, 0x00, 0x00, 0x00, 0xa0, 0x00, 0x00,
	0x70, 0x00, 0x02, 0x00, 0x2c, 0x01, 0x00, 0x00, 0x40, 0x89, 0x00, 0x00, 0x60, 0x00, 0x02, 0x00,
	0x38, 0x01, 0x00, 0x00, 0xc0, 0x89, 0x00, 0x00, 0x60, 0x00, 0x02, 0x00, 0x44, 0x01, 0x00, 0x00,
	0x80, 0xa0, 0x00, 0x00, 0x7a, 0x03, 0x02, 0x00, 0x48, 0x01, 0x00, 0x00, 0x40, 0xa4, 0x00, 0x00,
	0x0a, 0x01, 0x02, 0x00, 0x4e, 0x01, 0x00, 0x00, 0x00, 0x67, 0x00, 0x00, 0x10, 0x00, 0x02, 0x00,
	0x54, 0x01, 0x00, 0x00, 0x40, 0x7b, 0x00, 0x00, 0x58, 0x00, 0x02, 0x00, 0x60, 0x01, 0x00, 0x00,
	0x80, 0x8b, 0x00, 0x00, 0xf4, 0x00, 0x02, 0x00, 0x64, 0x01, 0x00, 0x00, 0x80, 0x8c, 0x00, 0x00,
	0x56, 0x01, 0x02, 0x00, 0x68, 0x01, 0x00, 0x00, 0x00, 0x8e, 0x00, 0x00, 0x8a, 0x01, 0x02, 0x00,
	0x6c, 0x01, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x84, 0x00, 0x02, 0x00, 0x84, 0x01, 0x00, 0x00,
	0xc0, 0x60, 0x00, 0x00, 0x8e, 0x00, 0x02, 0x00, 0x88, 0x01, 0x00, 0x00, 0x40, 0x64, 0x00, 0x00,
	0x96, 0x00, 0x02, 0x00, 0x8e, 0x01, 0x00, 0x00, 0x80, 0x61, 0x00, 0x00, 0x10, 0x00, 0x02, 0x00,
	0x90, 0x01, 0x00, 0x00, 0x80, 0xa5, 0x00, 0x00, 0x62, 0x02, 0x02, 0x00, 0xcc, 0x01, 0x00, 0x00,
	0xc0, 0x61, 0x00, 0x00, 0x08, 0x00, 0x02, 0x00, 0xdc, 0x01, 0x00, 0x00, 0xc0, 0x7f, 0x00, 0x00,
	0x28, 0x01, 0x02, 0x00, 0xe0, 0x01, 0x00, 0x00, 0xc0, 0x5c, 0x00, 0x00, 0x08, 0x00, 0x02, 0x00,
	0xec, 0x01, 0x00, 0x00, 0x00, 0x81, 0x00, 0x00, 0xac, 0x00, 0x02, 0x00, 0xf0, 0x01, 0x00, 0x00,
	0x00, 0x65, 0x00, 0x00, 0xbc, 0x00, 0x02, 0x00, 0xe8, 0x01, 0x00, 0x00, 0xc0, 0x65, 0x00, 0x00,
	0x14, 0x00, 0x02, 0x00, 0xe8, 0x01, 0x00, 0x00, 0xc0, 0x65, 0x00, 0x00, 0x14, 0x00, 0x02, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x16, 0x29, 0x0c, 0x00, 0x80, 0x80, 0x1f, 0x4b, 0x1f, 0x01, 0x00, 0x00, 0x0b, 0x7f, 0x16, 0xd3 };

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

/* Tests the libpff_index_tree_initialize function
 * Returns 1 if successful or 0 if not
 */
int pff_test_index_tree_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libfdata_tree_t *index_tree     = NULL;
	libpff_io_handle_t *io_handle   = NULL;
	int result                      = 0;

#if defined( HAVE_PFF_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
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

	/* Test regular cases
	 */
	result = libpff_index_tree_initialize(
	          &index_tree,
	          io_handle,
	          NULL,
	          NULL,
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
	 "index_tree",
	 index_tree );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfdata_tree_free(
	          &index_tree,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "index_tree",
	 index_tree );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libpff_index_tree_initialize(
	          NULL,
	          io_handle,
	          NULL,
	          NULL,
	          0,
	          0,
	          0,
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

	index_tree = (libfdata_tree_t *) 0x12345678UL;

	result = libpff_index_tree_initialize(
	          &index_tree,
	          io_handle,
	          NULL,
	          NULL,
	          0,
	          0,
	          0,
	          0,
	          &error );

	index_tree = NULL;

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libpff_index_tree_initialize(
	          &index_tree,
	          NULL,
	          NULL,
	          NULL,
	          0,
	          0,
	          0,
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
		/* Test libpff_index_tree_initialize with malloc failing
		 */
		pff_test_malloc_attempts_before_fail = test_number;

		result = libpff_index_tree_initialize(
		          &index_tree,
		          io_handle,
		          NULL,
		          NULL,
		          0,
		          0,
		          0,
		          0,
		          &error );

		if( pff_test_malloc_attempts_before_fail != -1 )
		{
			pff_test_malloc_attempts_before_fail = -1;

			if( index_tree != NULL )
			{
				libfdata_tree_free(
				 &index_tree,
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
			 "index_tree",
			 index_tree );

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
		/* Test libpff_index_tree_initialize with memset failing
		 */
		pff_test_memset_attempts_before_fail = test_number;

		result = libpff_index_tree_initialize(
		          &index_tree,
		          io_handle,
		          NULL,
		          NULL,
		          0,
		          0,
		          0,
		          0,
		          &error );

		if( pff_test_memset_attempts_before_fail != -1 )
		{
			pff_test_memset_attempts_before_fail = -1;

			if( index_tree != NULL )
			{
				libfdata_tree_free(
				 &index_tree,
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
			 "index_tree",
			 index_tree );

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
	if( index_tree != NULL )
	{
		libfdata_tree_free(
		 &index_tree,
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

/* Tests the libpff_index_tree_get_number_of_leaf_nodes_by_identifier function
 * Returns 1 if successful or 0 if not
 */
int pff_test_index_tree_get_number_of_leaf_nodes_by_identifier(
     libfdata_tree_t *index_tree )
{
	libbfio_handle_t *file_io_handle     = NULL;
	libcerror_error_t *error             = NULL;
	libfcache_cache_t *cache             = NULL;
	int number_of_leaf_nodes             = 0;
	int result                           = 0;

	/* Initialize test
	 */
	result = libfcache_cache_initialize(
	          &cache,
	          1,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "cache",
	 cache );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO handle
	 */
	result = pff_test_open_file_io_handle(
	          &file_io_handle,
	          pff_test_index_tree_32bit_index_node_data,
	          512,
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
	result = libpff_index_tree_get_number_of_leaf_nodes_by_identifier(
	          index_tree,
	          file_io_handle,
	          cache,
	          0x0000012c,
	          &number_of_leaf_nodes,
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
	result = libpff_index_tree_get_number_of_leaf_nodes_by_identifier(
	          NULL,
	          file_io_handle,
	          cache,
	          0x0000012c,
	          &number_of_leaf_nodes,
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

	result = libpff_index_tree_get_number_of_leaf_nodes_by_identifier(
	          index_tree,
	          file_io_handle,
	          NULL,
	          0x0000012c,
	          &number_of_leaf_nodes,
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

	result = libpff_index_tree_get_number_of_leaf_nodes_by_identifier(
	          index_tree,
	          file_io_handle,
	          cache,
	          0x0000012c,
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
	result = libfcache_cache_free(
	          &cache,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "cache",
	 cache );

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
	if( cache != NULL )
	{
		libfcache_cache_free(
		 &cache,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_index_tree_node_get_number_of_leaf_nodes_by_identifier function
 * Returns 1 if successful or 0 if not
 */
int pff_test_index_tree_node_get_number_of_leaf_nodes_by_identifier(
     libfdata_tree_t *index_tree )
{
	libbfio_handle_t *file_io_handle      = NULL;
	libcerror_error_t *error              = NULL;
	libfcache_cache_t *cache              = NULL;
	libfdata_tree_node_t *index_tree_node = NULL;
	int number_of_leaf_nodes              = 0;
	int result                            = 0;

	/* Initialize test
	 */
	result = libfdata_tree_get_root_node(
	          index_tree,
	          &index_tree_node,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "index_tree_node",
	 index_tree_node );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfcache_cache_initialize(
	          &cache,
	          1,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "cache",
	 cache );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO handle
	 */
	result = pff_test_open_file_io_handle(
	          &file_io_handle,
	          pff_test_index_tree_32bit_index_node_data,
	          512,
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
	result = libpff_index_tree_node_get_number_of_leaf_nodes_by_identifier(
	          index_tree_node,
	          file_io_handle,
	          cache,
	          0x0000012c,
	          &number_of_leaf_nodes,
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
	result = libpff_index_tree_node_get_number_of_leaf_nodes_by_identifier(
	          NULL,
	          file_io_handle,
	          cache,
	          0x0000012c,
	          &number_of_leaf_nodes,
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

	result = libpff_index_tree_node_get_number_of_leaf_nodes_by_identifier(
	          index_tree_node,
	          file_io_handle,
	          NULL,
	          0x0000012c,
	          &number_of_leaf_nodes,
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

	result = libpff_index_tree_node_get_number_of_leaf_nodes_by_identifier(
	          index_tree_node,
	          file_io_handle,
	          cache,
	          0x0000012c,
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
	result = libfcache_cache_free(
	          &cache,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "cache",
	 cache );

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
	if( cache != NULL )
	{
		libfcache_cache_free(
		 &cache,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_index_tree_get_leaf_node_by_identifier function
 * Returns 1 if successful or 0 if not
 */
int pff_test_index_tree_get_leaf_node_by_identifier(
     libfdata_tree_t *index_tree )
{
	libbfio_handle_t *file_io_handle           = NULL;
	libcerror_error_t *error                   = NULL;
	libfcache_cache_t *cache                   = NULL;
	libfdata_tree_node_t *leaf_index_tree_node = NULL;
	int leaf_node_index                        = 0;
	int result                                 = 0;

	/* Initialize test
	 */
	result = libfcache_cache_initialize(
	          &cache,
	          1,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "cache",
	 cache );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO handle
	 */
	result = pff_test_open_file_io_handle(
	          &file_io_handle,
	          pff_test_index_tree_32bit_index_node_data,
	          512,
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
	result = libpff_index_tree_get_leaf_node_by_identifier(
	          index_tree,
	          file_io_handle,
	          cache,
	          0x0000012c,
	          &leaf_node_index,
	          &leaf_index_tree_node,
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
	result = libpff_index_tree_get_leaf_node_by_identifier(
	          NULL,
	          file_io_handle,
	          cache,
	          0x0000012c,
	          &leaf_node_index,
	          &leaf_index_tree_node,
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

	result = libpff_index_tree_get_leaf_node_by_identifier(
	          index_tree,
	          file_io_handle,
	          NULL,
	          0x0000012c,
	          &leaf_node_index,
	          &leaf_index_tree_node,
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

	result = libpff_index_tree_get_leaf_node_by_identifier(
	          index_tree,
	          file_io_handle,
	          cache,
	          0x0000012c,
	          NULL,
	          &leaf_index_tree_node,
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

	result = libpff_index_tree_get_leaf_node_by_identifier(
	          index_tree,
	          file_io_handle,
	          cache,
	          0x0000012c,
	          &leaf_node_index,
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
	result = libfcache_cache_free(
	          &cache,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "cache",
	 cache );

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
	if( cache != NULL )
	{
		libfcache_cache_free(
		 &cache,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_index_tree_node_get_leaf_node_by_identifier function
 * Returns 1 if successful or 0 if not
 */
int pff_test_index_tree_node_get_leaf_node_by_identifier(
     libfdata_tree_t *index_tree )
{
	libbfio_handle_t *file_io_handle           = NULL;
	libcerror_error_t *error                   = NULL;
	libfcache_cache_t *cache                   = NULL;
	libfdata_tree_node_t *index_tree_node      = NULL;
	libfdata_tree_node_t *leaf_index_tree_node = NULL;
	int leaf_node_index                        = 0;
	int result                                 = 0;

	/* Initialize test
	 */
	result = libfdata_tree_get_root_node(
	          index_tree,
	          &index_tree_node,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "index_tree_node",
	 index_tree_node );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfcache_cache_initialize(
	          &cache,
	          1,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "cache",
	 cache );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO handle
	 */
	result = pff_test_open_file_io_handle(
	          &file_io_handle,
	          pff_test_index_tree_32bit_index_node_data,
	          512,
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
	result = libpff_index_tree_node_get_leaf_node_by_identifier(
	          index_tree_node,
	          file_io_handle,
	          cache,
	          0x0000012c,
	          &leaf_node_index,
	          &leaf_index_tree_node,
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
	result = libpff_index_tree_node_get_leaf_node_by_identifier(
	          NULL,
	          file_io_handle,
	          cache,
	          0x0000012c,
	          &leaf_node_index,
	          &leaf_index_tree_node,
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

	result = libpff_index_tree_node_get_leaf_node_by_identifier(
	          index_tree_node,
	          file_io_handle,
	          NULL,
	          0x0000012c,
	          &leaf_node_index,
	          &leaf_index_tree_node,
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

	result = libpff_index_tree_node_get_leaf_node_by_identifier(
	          index_tree_node,
	          file_io_handle,
	          cache,
	          0x0000012c,
	          NULL,
	          &leaf_index_tree_node,
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

	result = libpff_index_tree_node_get_leaf_node_by_identifier(
	          index_tree_node,
	          file_io_handle,
	          cache,
	          0x0000012c,
	          &leaf_node_index,
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
	result = libfcache_cache_free(
	          &cache,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "cache",
	 cache );

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
	if( cache != NULL )
	{
		libfcache_cache_free(
		 &cache,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_index_tree_get_value_by_identifier function
 * Returns 1 if successful or 0 if not
 */
int pff_test_index_tree_get_value_by_identifier(
     libfdata_tree_t *index_tree )
{
	libbfio_handle_t *file_io_handle       = NULL;
	libcerror_error_t *error               = NULL;
	libfcache_cache_t *cache               = NULL;
	libpff_index_value_t *index_tree_value = NULL;
	int result                             = 0;

	/* Initialize test
	 */
	result = libfcache_cache_initialize(
	          &cache,
	          1,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "cache",
	 cache );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO handle
	 */
	result = pff_test_open_file_io_handle(
	          &file_io_handle,
	          pff_test_index_tree_32bit_index_node_data,
	          512,
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
	result = libpff_index_tree_get_value_by_identifier(
	          index_tree,
	          file_io_handle,
	          cache,
	          0x0000012c,
	          0,
	          &index_tree_value,
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
	result = libpff_index_tree_get_value_by_identifier(
	          NULL,
	          file_io_handle,
	          cache,
	          0x0000012c,
	          0,
	          &index_tree_value,
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

	result = libpff_index_tree_get_value_by_identifier(
	          index_tree,
	          file_io_handle,
	          NULL,
	          0x0000012c,
	          0,
	          &index_tree_value,
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

	result = libpff_index_tree_get_value_by_identifier(
	          index_tree,
	          file_io_handle,
	          cache,
	          0x0000012c,
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
	result = libfcache_cache_free(
	          &cache,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "cache",
	 cache );

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
	if( cache != NULL )
	{
		libfcache_cache_free(
		 &cache,
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
#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )
#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	libcerror_error_t *error             = NULL;
	libfcache_cache_t *index_node_cache  = NULL;
	libfdata_tree_t *index_tree          = NULL;
	libfdata_vector_t *index_node_vector = NULL;
	libpff_io_handle_t *io_handle        = NULL;
	int result                           = 0;
	int segment_index                    = 0;

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */
#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */

	PFF_TEST_UNREFERENCED_PARAMETER( argc )
	PFF_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

	PFF_TEST_RUN(
	 "libpff_index_tree_initialize",
	 pff_test_index_tree_initialize );

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

	io_handle->file_type = LIBPFF_FILE_TYPE_32BIT;

	result = libfdata_vector_initialize(
	          &index_node_vector,
	          512,
	          (intptr_t *) io_handle,
		  NULL,
	          NULL,
	          (int (*)(intptr_t *, intptr_t *, libfdata_vector_t *, libfdata_cache_t *, int, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libpff_io_handle_read_index_node,
	          NULL,
	          LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "index_node_vector",
	 index_node_vector );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfdata_vector_append_segment(
	          index_node_vector,
	          &segment_index,
	          0,
	          0,
	          512,
	          0,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfcache_cache_initialize(
	          &index_node_cache,
	          1,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "index_node_cache",
	 index_node_cache );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libpff_index_tree_initialize(
	          &index_tree,
	          io_handle,
	          index_node_vector,
	          index_node_cache,
	          LIBPFF_INDEX_TYPE_OFFSET,
	          0,
	          0,
	          0,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "index_tree",
	 index_tree );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfdata_tree_set_root_node(
	          index_tree,
	          0,
	          LIBPFF_OFFSETS_INDEX_TREE_ROOT_OFFSET,
	          0,
	          0,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_index_tree_get_number_of_leaf_nodes_by_identifier",
	 pff_test_index_tree_get_number_of_leaf_nodes_by_identifier,
	 index_tree );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_index_tree_node_get_number_of_leaf_nodes_by_identifier",
	 pff_test_index_tree_node_get_number_of_leaf_nodes_by_identifier,
	 index_tree );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_index_tree_get_leaf_node_by_identifier",
	 pff_test_index_tree_get_leaf_node_by_identifier,
	 index_tree );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_index_tree_node_get_leaf_node_by_identifier",
	 pff_test_index_tree_node_get_leaf_node_by_identifier,
	 index_tree );

	PFF_TEST_RUN_WITH_ARGS(
	 "libpff_index_tree_get_value_by_identifier",
	 pff_test_index_tree_get_value_by_identifier,
	 index_tree );

	/* TODO: add tests for libpff_index_tree_node_get_upper_branch_node_by_identifier */

	/* TODO: add tests for libpff_index_tree_insert_value */

	/* Clean up
	 */
	result = libfdata_tree_free(
	          &index_tree,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "index_tree",
	 index_tree );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfcache_cache_free(
	          &index_node_cache,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "index_node_cache",
	 index_node_cache );

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
	if( index_tree != NULL )
	{
		libfdata_tree_free(
		 &index_tree,
		 NULL );
	}
	if( index_node_cache != NULL )
	{
		libfcache_cache_free(
		 &index_node_cache,
		 NULL );
	}
	if( index_node_vector != NULL )
	{
		libfdata_vector_free(
		 &index_node_vector,
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

