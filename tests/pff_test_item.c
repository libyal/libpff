/*
 * Library item type testing program
 *
 * Copyright (C) 2008-2016, Joachim Metz <joachim.metz@gmail.com>
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

#include "../libpff/libpff_item.h"

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

#if defined( __GNUC__ )

#endif /* defined( __GNUC__ ) */

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

#if defined( __GNUC__ )

	/* TODO: add tests for libpff_item_initialize */

#endif /* defined( __GNUC__ ) */

	PFF_TEST_RUN(
	 "libpff_item_free",
	 pff_test_item_free );

#if defined( __GNUC__ )

	/* TODO: add tests for libpff_item_clone */

	/* TODO: add tests for libpff_item_determine_type */

	/* TODO: add tests for libpff_item_get_identifier */

	/* TODO: add tests for libpff_item_get_number_of_record_sets */

	/* TODO: add tests for libpff_item_get_record_set_by_index */

	/* TODO: add tests for libpff_item_get_number_of_entries */

	/* TODO: add tests for libpff_item_get_entry_multi_value */

	/* TODO: add tests for libpff_item_get_type */

	/* TODO: add tests for libpff_item_get_number_of_sub_items */

	/* TODO: add tests for libpff_item_get_sub_item */

	/* TODO: add tests for libpff_item_get_sub_item_by_identifier */

#endif /* defined( __GNUC__ ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

