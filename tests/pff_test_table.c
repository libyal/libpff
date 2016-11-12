/*
 * Library table type testing program
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

#include "../libpff/libpff_table.h"

#if defined( __GNUC__ )

/* Tests the libpff_table_free function
 * Returns 1 if successful or 0 if not
 */
int pff_test_table_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libpff_table_free(
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

#if defined( __GNUC__ )

	/* TODO: add tests for libpff_table_initialize */

	PFF_TEST_RUN(
	 "libpff_table_free",
	 pff_test_table_free );

	/* TODO: add tests for libpff_table_clone */

	/* TODO: add tests for libpff_table_resize_record_entries */

	/* TODO: add tests for libpff_table_expand_record_entries */

	/* TODO: add tests for libpff_table_entries_get_record_entry_by_index */

	/* TODO: add tests for libpff_table_get_local_descriptors_value_by_identifier */

	/* TODO: add tests for libpff_table_get_index_value_by_reference */

	/* TODO: add tests for libpff_table_get_value_data_by_index_value */

	/* TODO: add tests for libpff_table_get_value_data_by_reference */

	/* TODO: add tests for libpff_table_clone_value_data_by_reference */

	/* TODO: add tests for libpff_table_get_number_of_record_sets */

	/* TODO: add tests for libpff_table_get_record_set_by_index */

	/* TODO: add tests for libpff_table_get_number_of_entries */

	/* TODO: add tests for libpff_table_get_entry_type_by_index */

	/* TODO: add tests for libpff_table_get_record_entry_by_index */

	/* TODO: add tests for libpff_table_get_record_entry_by_type */

	/* TODO: add tests for libpff_table_get_record_entry_by_utf8_name */

	/* TODO: add tests for libpff_table_get_record_entry_by_utf16_name */

	/* TODO: add tests for libpff_table_read */

	/* TODO: add tests for libpff_table_read_index */

	/* TODO: add tests for libpff_table_read_record_entries */

	/* TODO: add tests for libpff_table_read_values */

	/* TODO: add tests for libpff_table_read_6c_values */

	/* TODO: add tests for libpff_table_read_7c_values */

	/* TODO: add tests for libpff_table_read_8c_values */

	/* TODO: add tests for libpff_table_read_9c_values */

	/* TODO: add tests for libpff_table_read_a5_values */

	/* TODO: add tests for libpff_table_read_ac_values */

	/* TODO: add tests for libpff_table_read_bc_values */

	/* TODO: add tests for libpff_table_read_b5_header */

	/* TODO: add tests for libpff_table_read_6c_record_entries */

	/* TODO: add tests for libpff_table_read_7c_column_definitions */

	/* TODO: add tests for libpff_table_read_8c_record_entries */

	/* TODO: add tests for libpff_table_read_9c_record_entries */

	/* TODO: add tests for libpff_table_read_a5_record_entries */

	/* TODO: add tests for libpff_table_read_ac_column_definitions */

	/* TODO: add tests for libpff_table_read_bc_record_entries */

	/* TODO: add tests for libpff_table_values_array_get_value_data_by_entry_number */

	/* TODO: add tests for libpff_table_read_values_array */

	/* TODO: add tests for libpff_table_read_entry_value */

#endif /* defined( __GNUC__ ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

