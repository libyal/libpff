/*
 * Library record_entry type testing program
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

#include "../libpff/libpff_record_entry.h"

#if defined( __GNUC__ )

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

	/* TODO: add tests for libpff_record_entry_initialize */

	PFF_TEST_RUN(
	 "libpff_record_entry_free",
	 pff_test_record_entry_free );

	/* TODO: add tests for libpff_record_entry_string_contains_zero_bytes */

	/* TODO: add tests for libpff_record_entry_clone */

	/* TODO: add tests for libpff_record_entry_get_entry_type */

	/* TODO: add tests for libpff_record_entry_get_name_to_id_map_entry */

	/* TODO: add tests for libpff_record_entry_get_value_type */

	/* TODO: add tests for libpff_record_entry_get_value_data_size */

	/* TODO: add tests for libpff_record_entry_get_value_data */

	/* TODO: add tests for libpff_record_entry_set_value_data */

	/* TODO: add tests for libpff_record_entry_set_value_data_from_list */

	/* TODO: add tests for libpff_record_entry_set_value_data_from_stream */

	/* TODO: add tests for libpff_record_entry_copy_value_data */

	/* TODO: add tests for libpff_record_entry_read_buffer */

	/* TODO: add tests for libpff_record_entry_seek_offset */

	/* TODO: add tests for libpff_record_entry_get_value_boolean */

	/* TODO: add tests for libpff_record_entry_get_value_16bit */

	/* TODO: add tests for libpff_record_entry_get_value_32bit */

	/* TODO: add tests for libpff_record_entry_get_value_64bit */

	/* TODO: add tests for libpff_record_entry_get_value_filetime */

	/* TODO: add tests for libpff_record_entry_get_value_size */

	/* TODO: add tests for libpff_record_entry_get_value_floating_point */

	/* TODO: add tests for libpff_record_entry_get_value_utf8_string_size_with_codepage */

	/* TODO: add tests for libpff_record_entry_get_value_utf8_string_with_codepage */

	/* TODO: add tests for libpff_record_entry_compare_value_with_utf8_string_with_codepage */

	/* TODO: add tests for libpff_record_entry_get_value_utf8_string_size */

	/* TODO: add tests for libpff_record_entry_get_value_utf8_string */

	/* TODO: add tests for libpff_record_entry_get_value_utf16_string_size_with_codepage */

	/* TODO: add tests for libpff_record_entry_get_value_utf16_string_with_codepage */

	/* TODO: add tests for libpff_record_entry_compare_value_with_utf16_string_with_codepage */

	/* TODO: add tests for libpff_record_entry_get_value_utf16_string_size */

	/* TODO: add tests for libpff_record_entry_get_value_utf16_string */

	/* TODO: add tests for libpff_record_entry_copy_object_identifier */

	/* TODO: add tests for libpff_record_entry_get_multi_value */


	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

