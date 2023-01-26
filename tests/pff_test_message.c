/*
 * Library message type test program
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

#include "../libpff/libpff_message.h"

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

	/* TODO: add tests for libpff_message_initialize_sub_item_attachments */

	/* TODO: add tests for libpff_message_initialize_sub_item_recipients */

#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */

	/* TODO: add tests for libpff_message_get_entry_value_utf8_string_size */

	/* TODO: add tests for libpff_message_get_entry_value_utf8_string */

	/* TODO: add tests for libpff_message_get_entry_value_utf16_string_size */

	/* TODO: add tests for libpff_message_get_entry_value_utf16_string */

	/* TODO: add tests for libpff_message_get_client_submit_time */

	/* TODO: add tests for libpff_message_get_delivery_time */

	/* TODO: add tests for libpff_message_get_creation_time */

	/* TODO: add tests for libpff_message_get_modification_time */

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

	/* TODO: add tests for libpff_message_determine_attachment */

	/* TODO: add tests for libpff_message_determine_attachments */

#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */

	/* TODO: add tests for libpff_message_get_number_of_attachments */

	/* TODO: add tests for libpff_message_get_attachment */

	/* TODO: add tests for libpff_message_get_attachments */

#if defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT )

	/* TODO: add tests for libpff_message_determine_recipients */

#endif /* defined( __GNUC__ ) && !defined( LIBPFF_DLL_IMPORT ) */

	/* TODO: add tests for libpff_message_get_recipients */

	/* TODO: add tests for libpff_message_get_plain_text_body_size */

	/* TODO: add tests for libpff_message_get_plain_text_body */

	/* TODO: add tests for libpff_message_get_rtf_body_size */

	/* TODO: add tests for libpff_message_get_rtf_body */

	/* TODO: add tests for libpff_message_get_html_body_size */

	/* TODO: add tests for libpff_message_get_html_body */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

