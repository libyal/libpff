/*
 * Library file type testing program
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
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "pff_test_libcerror.h"
#include "pff_test_libclocale.h"
#include "pff_test_libcsystem.h"
#include "pff_test_libpff.h"
#include "pff_test_libuna.h"
#include "pff_test_macros.h"
#include "pff_test_memory.h"

#if defined( HAVE_WIDE_SYSTEM_CHARACTER ) && SIZEOF_WCHAR_T != 2 && SIZEOF_WCHAR_T != 4
#error Unsupported size of wchar_t
#endif

/* Define to make pff_test_file generate verbose output
#define PFF_TEST_FILE_VERBOSE
 */

/* Retrieves source as a narrow string
 * Returns 1 if successful or -1 on error
 */
int pff_test_file_get_narrow_source(
     const system_character_t *source,
     char *narrow_string,
     size_t narrow_string_size,
     libcerror_error_t **error )
{
	static char *function     = "pff_test_file_get_narrow_source";
	size_t narrow_source_size = 0;
	size_t source_length      = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	int result                = 0;
#endif

	if( source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( narrow_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid narrow string.",
		 function );

		return( -1 );
	}
	if( narrow_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid narrow string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	source_length = system_string_length(
	                 source );

	if( source_length > (size_t) ( SSIZE_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid source length value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_size_from_utf32(
		          (libuna_utf32_character_t *) source,
		          source_length + 1,
		          &narrow_source_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_size_from_utf16(
		          (libuna_utf16_character_t *) source,
		          source_length + 1,
		          &narrow_source_size,
		          error );
#endif
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_size_from_utf32(
		          (libuna_utf32_character_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          &narrow_source_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_size_from_utf16(
		          (libuna_utf16_character_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          &narrow_source_size,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine narrow string size.",
		 function );

		return( -1 );
	}
#else
	narrow_source_size = source_length + 1;

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	if( narrow_string_size < narrow_source_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: narrow string too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_copy_from_utf32(
		          (libuna_utf8_character_t *) narrow_string,
		          narrow_string_size,
		          (libuna_utf32_character_t *) source,
		          source_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_copy_from_utf16(
		          (libuna_utf8_character_t *) narrow_string,
		          narrow_string_size,
		          (libuna_utf16_character_t *) source,
		          source_length + 1,
		          error );
#endif
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_copy_from_utf32(
		          (uint8_t *) narrow_string,
		          narrow_string_size,
		          libclocale_codepage,
		          (libuna_utf32_character_t *) source,
		          source_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_copy_from_utf16(
		          (uint8_t *) narrow_string,
		          narrow_string_size,
		          libclocale_codepage,
		          (libuna_utf16_character_t *) source,
		          source_length + 1,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set narrow string.",
		 function );

		return( -1 );
	}
#else
	if( system_string_copy(
	     narrow_string,
	     source,
	     source_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set narrow string.",
		 function );

		return( -1 );
	}
	narrow_string[ source_length ] = 0;

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves source as a wide string
 * Returns 1 if successful or -1 on error
 */
int pff_test_file_get_wide_source(
     const system_character_t *source,
     wchar_t *wide_string,
     size_t wide_string_size,
     libcerror_error_t **error )
{
	static char *function   = "pff_test_file_get_wide_source";
	size_t source_length    = 0;
	size_t wide_source_size = 0;

#if !defined( HAVE_WIDE_SYSTEM_CHARACTER )
	int result              = 0;
#endif

	if( source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( wide_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid wide string.",
		 function );

		return( -1 );
	}
	if( wide_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid wide string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	source_length = system_string_length(
	                 source );

	if( source_length > (size_t) ( SSIZE_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid source length value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	wide_source_size = source_length + 1;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_utf8(
		          (libuna_utf8_character_t *) source,
		          source_length + 1,
		          &wide_source_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_utf8(
		          (libuna_utf8_character_t *) source,
		          source_length + 1,
		          &wide_source_size,
		          error );
#endif
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_byte_stream(
		          (uint8_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          &wide_source_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_byte_stream(
		          (uint8_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          &wide_source_size,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine wide string size.",
		 function );

		return( -1 );
	}

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	if( wide_string_size < wide_source_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: wide string too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( system_string_copy(
	     wide_string,
	     source,
	     source_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set wide string.",
		 function );

		return( -1 );
	}
	wide_string[ source_length ] = 0;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_utf8(
		          (libuna_utf32_character_t *) wide_string,
		          wide_string_size,
		          (libuna_utf8_character_t *) source,
		          source_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_utf8(
		          (libuna_utf16_character_t *) wide_string,
		          wide_string_size,
		          (libuna_utf8_character_t *) source,
		          source_length + 1,
		          error );
#endif
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_byte_stream(
		          (libuna_utf32_character_t *) wide_string,
		          wide_string_size,
		          (uint8_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_byte_stream(
		          (libuna_utf16_character_t *) wide_string,
		          wide_string_size,
		          (uint8_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set wide string.",
		 function );

		return( -1 );
	}

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	return( 1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Creates and opens a source file
 * Returns 1 if successful or -1 on error
 */
int pff_test_file_open_source(
     libpff_file_t **file,
     const system_character_t *source,
     libcerror_error_t **error )
{
	static char *function = "pff_test_file_open_source";
	int result            = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( libpff_file_initialize(
	     file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize file.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libpff_file_open_wide(
	          *file,
	          source,
	          LIBPFF_OPEN_READ,
	          error );
#else
	result = libpff_file_open(
	          *file,
	          source,
	          LIBPFF_OPEN_READ,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *file != NULL )
	{
		libpff_file_free(
		 file,
		 NULL );
	}
	return( -1 );
}

/* Closes and frees a source file
 * Returns 1 if successful or -1 on error
 */
int pff_test_file_close_source(
     libpff_file_t **file,
     libcerror_error_t **error )
{
	static char *function = "pff_test_file_close_source";
	int result            = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( libpff_file_close(
	     *file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close file.",
		 function );

		result = -1;
	}
	if( libpff_file_free(
	     file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file.",
		 function );

		result = -1;
	}
	return( result );
}

/* Tests the libpff_file_initialize function
 * Returns 1 if successful or 0 if not
 */
int pff_test_file_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libpff_file_t *file             = NULL;
	int result                      = 0;

#if defined( HAVE_PFF_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libpff_file_initialize(
	          &file,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        PFF_TEST_ASSERT_IS_NOT_NULL(
         "file",
         file );

        PFF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libpff_file_free(
	          &file,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        PFF_TEST_ASSERT_IS_NULL(
         "file",
         file );

        PFF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libpff_file_initialize(
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

	file = (libpff_file_t *) 0x12345678UL;

	result = libpff_file_initialize(
	          &file,
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

	file = NULL;

#if defined( HAVE_PFF_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libpff_file_initialize with malloc failing
		 */
		pff_test_malloc_attempts_before_fail = test_number;

		result = libpff_file_initialize(
		          &file,
		          &error );

		if( pff_test_malloc_attempts_before_fail != -1 )
		{
			pff_test_malloc_attempts_before_fail = -1;

			if( file != NULL )
			{
				libpff_file_free(
				 &file,
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
			 "file",
			 file );

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
		/* Test libpff_file_initialize with memset failing
		 */
		pff_test_memset_attempts_before_fail = test_number;

		result = libpff_file_initialize(
		          &file,
		          &error );

		if( pff_test_memset_attempts_before_fail != -1 )
		{
			pff_test_memset_attempts_before_fail = -1;

			if( file != NULL )
			{
				libpff_file_free(
				 &file,
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
			 "file",
			 file );

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
	if( file != NULL )
	{
		libpff_file_free(
		 &file,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_file_free function
 * Returns 1 if successful or 0 if not
 */
int pff_test_file_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libpff_file_free(
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

/* Tests the libpff_file_open function
 * Returns 1 if successful or 0 if not
 */
int pff_test_file_open(
     const system_character_t *source )
{
	char narrow_source[ 256 ];

	libcerror_error_t *error = NULL;
	libpff_file_t *file      = NULL;
	int result               = 0;

	/* Initialize test
	 */
	result = pff_test_file_get_narrow_source(
	          source,
	          narrow_source,
	          256,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        PFF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libpff_file_initialize(
	          &file,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        PFF_TEST_ASSERT_IS_NOT_NULL(
         "file",
         file );

        PFF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test open
	 */
	result = libpff_file_open(
	          file,
	          narrow_source,
	          LIBPFF_OPEN_READ,
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
	result = libpff_file_open(
	          file,
	          narrow_source,
	          LIBPFF_OPEN_READ,
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
	result = libpff_file_free(
	          &file,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        PFF_TEST_ASSERT_IS_NULL(
         "file",
         file );

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
	if( file != NULL )
	{
		libpff_file_free(
		 &file,
		 NULL );
	}
	return( 0 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Tests the libpff_file_open_wide function
 * Returns 1 if successful or 0 if not
 */
int pff_test_file_open_wide(
     const system_character_t *source )
{
	wchar_t wide_source[ 256 ];

	libcerror_error_t *error = NULL;
	libpff_file_t *file      = NULL;
	int result               = 0;

	/* Initialize test
	 */
	result = pff_test_file_get_wide_source(
	          source,
	          wide_source,
	          256,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        PFF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libpff_file_initialize(
	          &file,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        PFF_TEST_ASSERT_IS_NOT_NULL(
         "file",
         file );

        PFF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test open
	 */
	result = libpff_file_open_wide(
	          file,
	          wide_source,
	          LIBPFF_OPEN_READ,
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
	result = libpff_file_open_wide(
	          file,
	          wide_source,
	          LIBPFF_OPEN_READ,
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
	result = libpff_file_free(
	          &file,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        PFF_TEST_ASSERT_IS_NULL(
         "file",
         file );

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
	if( file != NULL )
	{
		libpff_file_free(
		 &file,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Tests the libpff_file_close function
 * Returns 1 if successful or 0 if not
 */
int pff_test_file_close(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libpff_file_close(
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

/* Tests the libpff_file_open and libpff_file_close functions
 * Returns 1 if successful or 0 if not
 */
int pff_test_file_open_close(
     const system_character_t *source )
{
	libcerror_error_t *error = NULL;
	libpff_file_t *file      = NULL;
	int result               = 0;

	/* Initialize test
	 */
	result = libpff_file_initialize(
	          &file,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        PFF_TEST_ASSERT_IS_NOT_NULL(
         "file",
         file );

        PFF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test open and close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libpff_file_open_wide(
	          file,
	          source,
	          LIBPFF_OPEN_READ,
	          &error );
#else
	result = libpff_file_open(
	          file,
	          source,
	          LIBPFF_OPEN_READ,
	          &error );
#endif

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        PFF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libpff_file_close(
	          file,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

        PFF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test open and close a second time to validate clean up on close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libpff_file_open_wide(
	          file,
	          source,
	          LIBPFF_OPEN_READ,
	          &error );
#else
	result = libpff_file_open(
	          file,
	          source,
	          LIBPFF_OPEN_READ,
	          &error );
#endif

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        PFF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libpff_file_close(
	          file,
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
	result = libpff_file_free(
	          &file,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        PFF_TEST_ASSERT_IS_NULL(
         "file",
         file );

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
	if( file != NULL )
	{
		libpff_file_free(
		 &file,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_file_signal_abort function
 * Returns 1 if successful or 0 if not
 */
int pff_test_file_signal_abort(
     libpff_file_t *file )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libpff_file_signal_abort(
	          file,
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
	result = libpff_file_signal_abort(
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

/* Tests the libpff_file_get_size function
 * Returns 1 if successful or 0 if not
 */
int pff_test_file_get_size(
     libpff_file_t *file )
{
	libcerror_error_t *error = NULL;
	size64_t size            = 0;
	int result               = 0;
	int size_is_set          = 0;

	/* Test regular cases
	 */
	result = libpff_file_get_size(
	          file,
	          &size,
	          &error );

	PFF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	size_is_set = result;

	/* Test error cases
	 */
	result = libpff_file_get_size(
	          NULL,
	          &size,
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

	if( size_is_set != 0 )
	{
		result = libpff_file_get_size(
		          file,
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
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libpff_file_get_content_type function
 * Returns 1 if successful or 0 if not
 */
int pff_test_file_get_content_type(
     libpff_file_t *file )
{
	libcerror_error_t *error = NULL;
	uint8_t content_type     = 0;
	int content_type_is_set  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libpff_file_get_content_type(
	          file,
	          &content_type,
	          &error );

	PFF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	content_type_is_set = result;

	/* Test error cases
	 */
	result = libpff_file_get_content_type(
	          NULL,
	          &content_type,
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

	if( content_type_is_set != 0 )
	{
		result = libpff_file_get_content_type(
		          file,
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
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libpff_file_get_type function
 * Returns 1 if successful or 0 if not
 */
int pff_test_file_get_type(
     libpff_file_t *file )
{
	libcerror_error_t *error = NULL;
	uint8_t type             = 0;
	int result               = 0;
	int type_is_set          = 0;

	/* Test regular cases
	 */
	result = libpff_file_get_type(
	          file,
	          &type,
	          &error );

	PFF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	type_is_set = result;

	/* Test error cases
	 */
	result = libpff_file_get_type(
	          NULL,
	          &type,
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

	if( type_is_set != 0 )
	{
		result = libpff_file_get_type(
		          file,
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
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libpff_file_get_encryption_type function
 * Returns 1 if successful or 0 if not
 */
int pff_test_file_get_encryption_type(
     libpff_file_t *file )
{
	libcerror_error_t *error   = NULL;
	uint8_t encryption_type    = 0;
	int encryption_type_is_set = 0;
	int result                 = 0;

	/* Test regular cases
	 */
	result = libpff_file_get_encryption_type(
	          file,
	          &encryption_type,
	          &error );

	PFF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	encryption_type_is_set = result;

	/* Test error cases
	 */
	result = libpff_file_get_encryption_type(
	          NULL,
	          &encryption_type,
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

	if( encryption_type_is_set != 0 )
	{
		result = libpff_file_get_encryption_type(
		          file,
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
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libpff_file_get_ascii_codepage function
 * Returns 1 if successful or 0 if not
 */
int pff_test_file_get_ascii_codepage(
     libpff_file_t *file )
{
	libcerror_error_t *error  = NULL;
	int ascii_codepage        = 0;
	int ascii_codepage_is_set = 0;
	int result                = 0;

	/* Test regular cases
	 */
	result = libpff_file_get_ascii_codepage(
	          file,
	          &ascii_codepage,
	          &error );

	PFF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	ascii_codepage_is_set = result;

	/* Test error cases
	 */
	result = libpff_file_get_ascii_codepage(
	          NULL,
	          &ascii_codepage,
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

	if( ascii_codepage_is_set != 0 )
	{
		result = libpff_file_get_ascii_codepage(
		          file,
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
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libpff_file_set_ascii_codepage function
 * Returns 1 if successful or 0 if not
 */
int pff_test_file_set_ascii_codepage(
     libpff_file_t *file )
{
	int supported_codepages[ 15 ] = {
		LIBPFF_CODEPAGE_ASCII,
		LIBPFF_CODEPAGE_WINDOWS_874,
		LIBPFF_CODEPAGE_WINDOWS_932,
		LIBPFF_CODEPAGE_WINDOWS_936,
		LIBPFF_CODEPAGE_WINDOWS_949,
		LIBPFF_CODEPAGE_WINDOWS_950,
		LIBPFF_CODEPAGE_WINDOWS_1250,
		LIBPFF_CODEPAGE_WINDOWS_1251,
		LIBPFF_CODEPAGE_WINDOWS_1252,
		LIBPFF_CODEPAGE_WINDOWS_1253,
		LIBPFF_CODEPAGE_WINDOWS_1254,
		LIBPFF_CODEPAGE_WINDOWS_1255,
		LIBPFF_CODEPAGE_WINDOWS_1256,
		LIBPFF_CODEPAGE_WINDOWS_1257,
		LIBPFF_CODEPAGE_WINDOWS_1258 };

	int unsupported_codepages[ 17 ] = {
		LIBPFF_CODEPAGE_ISO_8859_1,
		LIBPFF_CODEPAGE_ISO_8859_2,
		LIBPFF_CODEPAGE_ISO_8859_3,
		LIBPFF_CODEPAGE_ISO_8859_4,
		LIBPFF_CODEPAGE_ISO_8859_5,
		LIBPFF_CODEPAGE_ISO_8859_6,
		LIBPFF_CODEPAGE_ISO_8859_7,
		LIBPFF_CODEPAGE_ISO_8859_8,
		LIBPFF_CODEPAGE_ISO_8859_9,
		LIBPFF_CODEPAGE_ISO_8859_10,
		LIBPFF_CODEPAGE_ISO_8859_11,
		LIBPFF_CODEPAGE_ISO_8859_13,
		LIBPFF_CODEPAGE_ISO_8859_14,
		LIBPFF_CODEPAGE_ISO_8859_15,
		LIBPFF_CODEPAGE_ISO_8859_16,
		LIBPFF_CODEPAGE_KOI8_R,
		LIBPFF_CODEPAGE_KOI8_U };

	libcerror_error_t *error = NULL;
	int codepage             = 0;
	int index                = 0;
	int result               = 0;

	/* Test set ASCII codepage
	 */
	for( index = 0;
	     index < 15;
	     index++ )
	{
		codepage = supported_codepages[ index ];

		result = libpff_file_set_ascii_codepage(
		          file,
		          codepage,
		          &error );

		PFF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        PFF_TEST_ASSERT_IS_NULL(
	         "error",
	         error );
	}
	/* Test error cases
	 */
	result = libpff_file_set_ascii_codepage(
	          NULL,
	          LIBPFF_CODEPAGE_ASCII,
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

	for( index = 0;
	     index < 17;
	     index++ )
	{
		codepage = unsupported_codepages[ index ];

		result = libpff_file_set_ascii_codepage(
		          file,
		          codepage,
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
	}
	/* Clean up
	 */
	result = libpff_file_set_ascii_codepage(
	          file,
	          LIBPFF_CODEPAGE_WINDOWS_1252,
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
	return( 0 );
}

/* Tests the libpff_file_get_root_item function
 * Returns 1 if successful or 0 if not
 */
int pff_test_file_get_root_item(
     libpff_file_t *file )
{
	libcerror_error_t *error = NULL;
	libpff_item_t *root_item = 0;
	int result               = 0;
	int root_item_is_set     = 0;

	/* Test regular cases
	 */
	result = libpff_file_get_root_item(
	          file,
	          &root_item,
	          &error );

	PFF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	root_item_is_set = result;

	if( root_item_is_set != 0 )
	{
		PFF_TEST_ASSERT_IS_NOT_NULL(
		 "root_item",
		 root_item );

		result = libpff_item_free(
		          &root_item,
		          &error );

		PFF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		PFF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	result = libpff_file_get_root_item(
	          NULL,
	          &root_item,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "root_item",
	 root_item );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( root_item_is_set != 0 )
	{
		result = libpff_file_get_root_item(
		          file,
		          NULL,
		          &error );

		PFF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		PFF_TEST_ASSERT_IS_NULL(
		 "root_item",
		 root_item );

		PFF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( root_item != NULL )
	{
		libpff_item_free(
		 &root_item,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_file_get_message_store function
 * Returns 1 if successful or 0 if not
 */
int pff_test_file_get_message_store(
     libpff_file_t *file )
{
	libcerror_error_t *error     = NULL;
	libpff_item_t *message_store = 0;
	int message_store_is_set     = 0;
	int result                   = 0;

	/* Test regular cases
	 */
	result = libpff_file_get_message_store(
	          file,
	          &message_store,
	          &error );

	PFF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	message_store_is_set = result;

	if( message_store_is_set != 0 )
	{
		PFF_TEST_ASSERT_IS_NOT_NULL(
		 "message_store",
		 message_store );

		result = libpff_item_free(
		          &message_store,
		          &error );

		PFF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		PFF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	result = libpff_file_get_message_store(
	          NULL,
	          &message_store,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "message_store",
	 message_store );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( message_store_is_set != 0 )
	{
		result = libpff_file_get_message_store(
		          file,
		          NULL,
		          &error );

		PFF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		PFF_TEST_ASSERT_IS_NULL(
		 "message_store",
		 message_store );

		PFF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( message_store != NULL )
	{
		libpff_item_free(
		 &message_store,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_file_get_name_to_id_map function
 * Returns 1 if successful or 0 if not
 */
int pff_test_file_get_name_to_id_map(
     libpff_file_t *file )
{
	libcerror_error_t *error      = NULL;
	libpff_item_t *name_to_id_map = 0;
	int name_to_id_map_is_set     = 0;
	int result                    = 0;

	/* Test regular cases
	 */
	result = libpff_file_get_name_to_id_map(
	          file,
	          &name_to_id_map,
	          &error );

	PFF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	name_to_id_map_is_set = result;

	if( name_to_id_map_is_set != 0 )
	{
		PFF_TEST_ASSERT_IS_NOT_NULL(
		 "name_to_id_map",
		 name_to_id_map );

		result = libpff_item_free(
		          &name_to_id_map,
		          &error );

		PFF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		PFF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	result = libpff_file_get_name_to_id_map(
	          NULL,
	          &name_to_id_map,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "name_to_id_map",
	 name_to_id_map );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( name_to_id_map_is_set != 0 )
	{
		result = libpff_file_get_name_to_id_map(
		          file,
		          NULL,
		          &error );

		PFF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		PFF_TEST_ASSERT_IS_NULL(
		 "name_to_id_map",
		 name_to_id_map );

		PFF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( name_to_id_map != NULL )
	{
		libpff_item_free(
		 &name_to_id_map,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_file_get_root_folder function
 * Returns 1 if successful or 0 if not
 */
int pff_test_file_get_root_folder(
     libpff_file_t *file )
{
	libcerror_error_t *error   = NULL;
	libpff_item_t *root_folder = 0;
	int result                 = 0;
	int root_folder_is_set     = 0;

	/* Test regular cases
	 */
	result = libpff_file_get_root_folder(
	          file,
	          &root_folder,
	          &error );

	PFF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	root_folder_is_set = result;

	if( root_folder_is_set != 0 )
	{
		PFF_TEST_ASSERT_IS_NOT_NULL(
		 "root_folder",
		 root_folder );

		result = libpff_item_free(
		          &root_folder,
		          &error );

		PFF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		PFF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	result = libpff_file_get_root_folder(
	          NULL,
	          &root_folder,
	          &error );

	PFF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "root_folder",
	 root_folder );

	PFF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( root_folder_is_set != 0 )
	{
		result = libpff_file_get_root_folder(
		          file,
		          NULL,
		          &error );

		PFF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		PFF_TEST_ASSERT_IS_NULL(
		 "root_folder",
		 root_folder );

		PFF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( root_folder != NULL )
	{
		libpff_item_free(
		 &root_folder,
		 NULL );
	}
	return( 0 );
}

/* Tests the libpff_file_get_number_of_orphan_items function
 * Returns 1 if successful or 0 if not
 */
int pff_test_file_get_number_of_orphan_items(
     libpff_file_t *file )
{
	libcerror_error_t *error          = NULL;
	int number_of_orphan_items        = 0;
	int number_of_orphan_items_is_set = 0;
	int result                        = 0;

	/* Test regular cases
	 */
	result = libpff_file_get_number_of_orphan_items(
	          file,
	          &number_of_orphan_items,
	          &error );

	PFF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	number_of_orphan_items_is_set = result;

	/* Test error cases
	 */
	result = libpff_file_get_number_of_orphan_items(
	          NULL,
	          &number_of_orphan_items,
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

	if( number_of_orphan_items_is_set != 0 )
	{
		result = libpff_file_get_number_of_orphan_items(
		          file,
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
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libpff_file_get_number_of_recovered_items function
 * Returns 1 if successful or 0 if not
 */
int pff_test_file_get_number_of_recovered_items(
     libpff_file_t *file )
{
	libcerror_error_t *error             = NULL;
	int number_of_recovered_items        = 0;
	int number_of_recovered_items_is_set = 0;
	int result                           = 0;

	/* Test regular cases
	 */
	result = libpff_file_get_number_of_recovered_items(
	          file,
	          &number_of_recovered_items,
	          &error );

	PFF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	PFF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	number_of_recovered_items_is_set = result;

	/* Test error cases
	 */
	result = libpff_file_get_number_of_recovered_items(
	          NULL,
	          &number_of_recovered_items,
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

	if( number_of_recovered_items_is_set != 0 )
	{
		result = libpff_file_get_number_of_recovered_items(
		          file,
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
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc,
     wchar_t * const argv[] )
#else
int main(
     int argc,
     char * const argv[] )
#endif
{
	libcerror_error_t *error   = NULL;
	libpff_file_t *file        = NULL;
	system_character_t *source = NULL;
	system_integer_t option    = 0;
	int result                 = 0;

	while( ( option = libcsystem_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM ".\n",
				 argv[ optind - 1 ] );

				return( EXIT_FAILURE );
		}
	}
	if( optind < argc )
	{
		source = argv[ optind ];
	}
#if defined( HAVE_DEBUG_OUTPUT ) && defined( PFF_TEST_FILE_VERBOSE )
	libpff_notify_set_verbose(
	 1 );
	libpff_notify_set_stream(
	 stderr,
	 NULL );
#endif

	PFF_TEST_RUN(
	 "libpff_file_initialize",
	 pff_test_file_initialize );

	PFF_TEST_RUN(
	 "libpff_file_free",
	 pff_test_file_free );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )
	if( source != NULL )
	{
		PFF_TEST_RUN_WITH_ARGS(
		 "libpff_file_open",
		 pff_test_file_open,
		 source );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

		PFF_TEST_RUN_WITH_ARGS(
		 "libpff_file_open_wide",
		 pff_test_file_open_wide,
		 source );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBPFF_HAVE_BFIO )

		/* TODO add test for libpff_file_open_file_io_handle */

#endif /* defined( LIBPFF_HAVE_BFIO ) */

		PFF_TEST_RUN(
		 "libpff_file_close",
		 pff_test_file_close );

		PFF_TEST_RUN_WITH_ARGS(
		 "libpff_file_open_close",
		 pff_test_file_open_close,
		 source );

		/* Initialize test
		 */
		result = pff_test_file_open_source(
		          &file,
		          source,
		          &error );

		PFF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        PFF_TEST_ASSERT_IS_NOT_NULL(
	         "file",
	         file );

	        PFF_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		PFF_TEST_RUN_WITH_ARGS(
		 "libpff_file_signal_abort",
		 pff_test_file_signal_abort,
		 file );

#if defined( __GNUC__ )

		/* TODO: add tests for libpff_file_open_read */

		/* TODO: add tests for libpff_file_read_allocation_tables */

#endif /* defined( __GNUC__ ) */

		/* TODO: add tests for libpff_file_is_corrupted */

		/* TODO: add tests for libpff_file_recover_items */

		PFF_TEST_RUN_WITH_ARGS(
		 "libpff_file_get_size",
		 pff_test_file_get_size,
		 file );

		PFF_TEST_RUN_WITH_ARGS(
		 "libpff_file_get_content_type",
		 pff_test_file_get_content_type,
		 file );

		PFF_TEST_RUN_WITH_ARGS(
		 "libpff_file_get_type",
		 pff_test_file_get_type,
		 file );

		PFF_TEST_RUN_WITH_ARGS(
		 "libpff_file_get_encryption_type",
		 pff_test_file_get_encryption_type,
		 file );

		PFF_TEST_RUN_WITH_ARGS(
		 "libpff_file_get_ascii_codepage",
		 pff_test_file_get_ascii_codepage,
		 file );

		PFF_TEST_RUN_WITH_ARGS(
		 "libpff_file_set_ascii_codepage",
		 pff_test_file_set_ascii_codepage,
		 file );

		/* TODO: add tests for libpff_file_get_number_of_unallocated_blocks */

		/* TODO: add tests for libpff_file_get_unallocated_block */

		PFF_TEST_RUN_WITH_ARGS(
		 "libpff_file_get_root_item",
		 pff_test_file_get_root_item,
		 file );

		PFF_TEST_RUN_WITH_ARGS(
		 "libpff_file_get_message_store",
		 pff_test_file_get_message_store,
		 file );

		PFF_TEST_RUN_WITH_ARGS(
		 "libpff_file_get_name_to_id_map",
		 pff_test_file_get_name_to_id_map,
		 file );

		PFF_TEST_RUN_WITH_ARGS(
		 "libpff_file_get_root_folder",
		 pff_test_file_get_root_folder,
		 file );

		/* TODO: add tests for libpff_file_get_item_by_identifier */

		PFF_TEST_RUN_WITH_ARGS(
		 "libpff_file_get_number_of_orphan_items",
		 pff_test_file_get_number_of_orphan_items,
		 file );

		/* TODO: add tests for libpff_file_get_orphan_item */

		/* TODO: add tests for libpff_file_get_number_of_recovered_items */

		/* TODO: add tests for libpff_file_get_recovered_item */

		/* Clean up
		 */
		result = pff_test_file_close_source(
		          &file,
		          &error );

		PFF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 0 );

		PFF_TEST_ASSERT_IS_NULL(
	         "file",
	         file );

	        PFF_TEST_ASSERT_IS_NULL(
	         "error",
	         error );
	}
#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( file != NULL )
	{
		pff_test_file_close_source(
		 &file,
		 NULL );
	}
	return( EXIT_FAILURE );
}

