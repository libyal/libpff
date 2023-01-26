/*
 * Library folder type test program
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

#include "../libpff/libpff_folder.h"

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

	/* TODO: add tests for libpff_folder_get_type */

	/* TODO: add tests for libpff_folder_get_utf8_name_size */

	/* TODO: add tests for libpff_folder_get_utf8_name */

	/* TODO: add tests for libpff_folder_get_utf16_name_size */

	/* TODO: add tests for libpff_folder_get_utf16_name */

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

	/* TODO: add tests for libpff_folder_determine_sub_folders */

	/* TODO: add tests for libpff_folder_determine_sub_messages */

	/* TODO: add tests for libpff_folder_determine_sub_associated_contents */

	/* TODO: add tests for libpff_folder_determine_unknowns */

#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */

	/* TODO: add tests for libpff_folder_get_number_of_sub_folders */

	/* TODO: add tests for libpff_folder_get_sub_folder */

	/* TODO: add tests for libpff_folder_get_sub_folder_by_utf8_name */

	/* TODO: add tests for libpff_folder_get_sub_folder_by_utf16_name */

	/* TODO: add tests for libpff_folder_get_sub_folders */

	/* TODO: add tests for libpff_folder_get_number_of_sub_messages */

	/* TODO: add tests for libpff_folder_get_sub_message */

	/* TODO: add tests for libpff_folder_get_sub_message_by_utf8_name */

	/* TODO: add tests for libpff_folder_get_sub_message_by_utf16_name */

	/* TODO: add tests for libpff_folder_get_sub_messages */

	/* TODO: add tests for libpff_folder_get_number_of_sub_associated_contents */

	/* TODO: add tests for libpff_folder_get_sub_associated_content */

	/* TODO: add tests for libpff_folder_get_sub_associated_contents */

	/* TODO: add tests for libpff_folder_get_unknowns */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

