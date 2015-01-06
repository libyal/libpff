/*
 * Library set ASCII codepage testing program
 *
 * Copyright (C) 2008-2015, Joachim Metz <joachim.metz@gmail.com>
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

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <stdio.h>

#include "pff_test_libcerror.h"
#include "pff_test_libclocale.h"
#include "pff_test_libcstring.h"
#include "pff_test_libpff.h"

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libcerror_error_t *error = NULL;
	libpff_file_t *file      = NULL;
	size_t string_length     = 0;
	uint32_t feature_flags   = 0;
	int ascii_codepage       = 0;
	int result               = 0;

	if( argc != 2 )
	{
		fprintf(
		 stderr,
		 "Unsupported number of arguments.\n" );

		return( EXIT_FAILURE );
	}
	feature_flags = LIBCLOCALE_CODEPAGE_FEATURE_FLAG_HAVE_ISO_8859
	              | LIBCLOCALE_CODEPAGE_FEATURE_FLAG_HAVE_KOI8
	              | LIBCLOCALE_CODEPAGE_FEATURE_FLAG_HAVE_WINDOWS;

	string_length = libcstring_system_string_length(
	                 argv[ 1 ] );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libclocale_codepage_copy_from_string_wide(
	          &ascii_codepage,
	          argv[ 1 ],
	          string_length,
	          feature_flags,
	          &error );
#else
	result = libclocale_codepage_copy_from_string(
	          &ascii_codepage,
	          argv[ 1 ],
	          string_length,
	          feature_flags,
	          &error );
#endif
	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to determine ASCII codepage from: %" PRIs_LIBCSTRING_SYSTEM ".\n",
		 argv[ 1 ] );

		goto on_error;
	}
	if( libpff_file_initialize(
	     &file,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create file.\n" );

		goto on_error;
	}
	if( libpff_file_set_ascii_codepage(
	     file,
	     ascii_codepage,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to set codepage: %" PRIs_LIBCSTRING_SYSTEM ".\n",
		 argv[ 1 ] );

		goto on_error;
	}
	if( libpff_file_free(
	     &file,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free file.\n" );

		goto on_error;
	}
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcerror_error_backtrace_fprint(
		 error,
		 stdout );

		libcerror_error_free(
		 &error );
	}
	if( file != NULL )
	{
		libpff_file_free(
		 &file,
		 NULL );
	}
	return( -1 );
}

