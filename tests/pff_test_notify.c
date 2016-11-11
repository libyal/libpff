/*
 * Library notification functions test program
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

#include "pff_test_libpff.h"
#include "pff_test_macros.h"
#include "pff_test_unused.h"

/* Tests the libpff_notify_set_verbose function
 * Returns 1 if successful or 0 if not
 */
int pff_test_notify_set_verbose(
     void )
{
	/* Test invocation of function only
	 */
	libpff_notify_set_verbose(
	 0 );

	return( 1 );
}

/* Tests the libpff_notify_set_stream function
 * Returns 1 if successful or 0 if not
 */
int pff_test_notify_set_stream(
     void )
{
	/* Test invocation of function only
	 */
	libpff_notify_set_stream(
	 NULL,
	 NULL );

	return( 1 );
}

/* Tests the libpff_notify_stream_open function
 * Returns 1 if successful or 0 if not
 */
int pff_test_notify_stream_open(
     void )
{
	/* Test invocation of function only
	 */
	libpff_notify_stream_open(
	 NULL,
	 NULL );

	return( 1 );
}

/* Tests the libpff_notify_stream_close function
 * Returns 1 if successful or 0 if not
 */
int pff_test_notify_stream_close(
     void )
{
	/* Test invocation of function only
	 */
	libpff_notify_stream_close(
	 NULL );

	return( 1 );
}

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

	PFF_TEST_RUN(
	 "libpff_notify_set_verbose",
	 pff_test_notify_set_verbose )

	PFF_TEST_RUN(
	 "libpff_notify_set_stream",
	 pff_test_notify_set_stream )

	PFF_TEST_RUN(
	 "libpff_notify_stream_open",
	 pff_test_notify_stream_open )

	PFF_TEST_RUN(
	 "libpff_notify_stream_close",
	 pff_test_notify_stream_close )

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

