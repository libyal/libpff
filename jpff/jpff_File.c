/*
 * Java object definition of the libpff file
 *
 * Copyright (c) 2010, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include <libpff.h>

#include <jni.h>

#include "jpff_File.h"
#include "jpff_jni.h"
#include "jpff_libcerror.h"
#include "jpff_string.h"

static jfieldID jpff_File_field_identifier = 0;

JNIEXPORT void JNICALL Java_jpff_File_internal_1class_1initialize(
                        JNIEnv *environment,
                        jclass file_class )
{
	if( ( environment == NULL )
	 || ( *environment == NULL ) )
	{
		return;
	}
	jpff_File_field_identifier = jpff_jni_environment_get_field_identifier(
	                              environment,
	                              file_class,
	                              "libpff_file",
	                              "J" );
}

JNIEXPORT void JNICALL Java_jpff_File_internal_1initialize(
                        JNIEnv *environment,
                        jobject file_object )
{
	jclass exception        = NULL;
	libcerror_t *error      = NULL;
	libpff_file_t *pff_file = NULL;
	static char *function   = "Java_jpff_File_internal_1initialize";

	if( ( environment == NULL )
	 || ( *environment == NULL ) )
	{
		return;
	}
	pff_file = (libpff_file_t *) jpff_jni_environment_get_long_field(
	                              environment,
	                              file_object,
	                              jpff_File_field_identifier );

	if( pff_file != NULL )
	{
		exception = jpff_jni_environment_find_class(
		             environment,
		             "jpff/Exception" );

		jpff_jni_environment_exception_clear(
		 environment );

		if( exception != NULL )
		{
			jpff_jni_environment_throw_new(
			 environment,
			 exception,
			 "Java_jpff_File_internal_1initialize: file already set" );
		}
		return;
	}
	if( libpff_file_initialize(
	     &pff_file,
	     &error ) != 1 )
	{
		exception = jpff_jni_environment_find_class(
		             environment,
		             "java/lang/OutOfMemoryError" );

		jpff_jni_environment_exception_clear(
		 environment );

		if( exception != NULL )
		{
			/* TODO something useful with error */
			jpff_jni_environment_throw_new(
			 environment,
			 exception,
			 "Java_jpff_File_internal_1initialize: unable to create file" );
		}
		libcerror_error_free(
		 &error );

		return;
	}
	jpff_jni_environment_set_long_field(
	 environment,
	 file_object,
	 jpff_File_field_identifier,
	 (jlong) pff_file );
}

JNIEXPORT void JNICALL Java_jpff_File_internal_1free(
                        JNIEnv *environment,
                        jobject file_object )
{
	jclass exception        = NULL;
	libcerror_t *error      = NULL;
	libpff_file_t *pff_file = NULL;
	static char *function   = "Java_jpff_File_internal_1free";

	if( ( environment == NULL )
	 || ( *environment == NULL ) )
	{
		return;
	}
	pff_file = (libpff_file_t *) jpff_jni_environment_get_long_field(
	                              environment,
	                              file_object,
	                              jpff_File_field_identifier );

	if( pff_file == NULL )
	{
		exception = jpff_jni_environment_find_class(
		             environment,
		             "jpff/Exception" );

		jpff_jni_environment_exception_clear(
		 environment );

		if( exception != NULL )
		{
			jpff_jni_environment_throw_new(
			 environment,
			 exception,
			 "Java_jpff_File_internal_1free: missing file" );
		}
		return;
	}
	jpff_jni_environment_set_long_field(
	 environment,
	 file_object,
	 jpff_File_field_identifier,
	 (jlong) NULL );

	if( libpff_file_free(
	     &pff_file,
	     &error ) != 1 )
	{
		/* TODO something useful with error */

		libcerror_error_free(
		 &error );

		return;
	}
}

JNIEXPORT void JNICALL Java_jpff_File_internal_1open(
                        JNIEnv *environment,
                        jobject file_object,
                        jstring filename,
                        jint access_flags )
{
	jclass exception             = NULL;
	libcerror_t *error           = NULL;
	libpff_file_t *pff_file      = NULL;
	static char *function        = "Java_jpff_File_internal_1open";

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	wchar_t *wide_filename      = NULL;
	size_t wide_filename_size   = 0;
#else
	char *narrow_filename       = NULL;
	size_t narrow_filename_size = 0;
#endif

	if( ( environment == NULL )
	 || ( *environment == NULL ) )
	{
		return;
	}
	if( filename == NULL )
	{
		exception = jpff_jni_environment_find_class(
		             environment,
		             "jpff/Exception" );

		jpff_jni_environment_exception_clear(
		 environment );

		if( exception != NULL )
		{
			jpff_jni_environment_throw_new(
			 environment,
			 exception,
			 "Java_jpff_File_internal_1open: invalid filename" );
		}
		return;
	}
	pff_file = (libpff_file_t *) jpff_jni_environment_get_long_field(
	                              environment,
	                              file_object,
	                              jpff_File_field_identifier );

	if( pff_file == NULL )
	{
		exception = jpff_jni_environment_find_class(
		             environment,
		             "jpff/Exception" );

		jpff_jni_environment_exception_clear(
		 environment );

		if( exception != NULL )
		{
			jpff_jni_environment_throw_new(
			 environment,
			 exception,
			 "Java_jpff_File_internal_1open: missing file" );
		}
		return;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( jpff_string_get_wide_string_size(
	     environment,
	     filename,
	     &wide_filename_size ) != 1 )
#else
	if( jpff_string_get_narrow_string_size(
	     environment,
	     filename,
	     &narrow_filename_size ) != 1 )
#endif
	{
		exception = jpff_jni_environment_find_class(
		             environment,
		             "jpff/Exception" );

		jpff_jni_environment_exception_clear(
		 environment );

		if( exception != NULL )
		{
			jpff_jni_environment_throw_new(
			 environment,
			 exception,
			 "Java_jpff_File_internal_1open: unable to determine narrow filename size" );
		}
		return;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	wide_filename = (wchar_t *) memory_allocate(
	                             sizeof( wchar_t ) * wide_filename_size );
#else
	narrow_filename = (char *) memory_allocate(
	                            sizeof( char ) * narrow_filename_size );
#endif

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( jpff_string_copy_to_wide_string(
	     environment,
	     filename,
	     wide_filename,
	     wide_filename_size ) != 1 )
#else
	if( jpff_string_copy_to_narrow_string(
	     environment,
	     filename,
	     narrow_filename,
	     narrow_filename_size ) != 1 )
#endif
	{
		exception = jpff_jni_environment_find_class(
		             environment,
		             "jpff/Exception" );

		jpff_jni_environment_exception_clear(
		 environment );

		if( exception != NULL )
		{
			jpff_jni_environment_throw_new(
			 environment,
			 exception,
			 "Java_jpff_File_internal_1open: unable to determine narrow filename size" );
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		memory_free(
		 wide_filename );
#else
		memory_free(
		 narrow_filename );
#endif
		return;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libpff_file_open_wide(
	     pff_file,
	     wide_filename,
	     access_flags,
	     &error ) != 1 )
#else
	if( libpff_file_open(
	     pff_file,
	     narrow_filename,
	     access_flags,
	     &error ) != 1 )
#endif
	{
		exception = jpff_jni_environment_find_class(
		             environment,
		             "java/io/IOException" );

		jpff_jni_environment_exception_clear(
		 environment );

		if( exception != NULL )
		{
			/* TODO something useful with error */
			jpff_jni_environment_throw_new(
			 environment,
			 exception,
			 "Java_jpff_File_internal_1open: unable to open file" );
		}
		libcerror_error_free(
		 &error );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		memory_free(
		 wide_filename );
#else
		memory_free(
		 narrow_filename );
#endif
		return;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	memory_free(
	 wide_filename );
#else
	memory_free(
	 narrow_filename );
#endif
}

JNIEXPORT void JNICALL Java_jpff_File_internal_1close(
                        JNIEnv *environment,
                        jobject file_object )
{
	jclass exception        = NULL;
	libcerror_t *error      = NULL;
	libpff_file_t *pff_file = NULL;
	static char *function   = "Java_jpff_File_internal_1close";

	if( ( environment == NULL )
	 || ( *environment == NULL ) )
	{
		return;
	}
	pff_file = (libpff_file_t *) jpff_jni_environment_get_long_field(
	                              environment,
	                              file_object,
	                              jpff_File_field_identifier );

	if( pff_file == NULL )
	{
		exception = jpff_jni_environment_find_class(
		             environment,
		             "jpff/Exception" );

		jpff_jni_environment_exception_clear(
		 environment );

		if( exception != NULL )
		{
			jpff_jni_environment_throw_new(
			 environment,
			 exception,
			 "Java_jpff_File_internal_1close: missing file" );
		}
		return;
	}
	if( libpff_file_close(
	     pff_file,
	     &error ) != 0 )
	{
		exception = jpff_jni_environment_find_class(
		             environment,
		             "java/io/IOException" );

		jpff_jni_environment_exception_clear(
		 environment );

		if( exception != NULL )
		{
			/* TODO something useful with error */
			jpff_jni_environment_throw_new(
			 environment,
			 exception,
			 "Java_jpff_File_internal_1close: unable to close file" );
		}
		libcerror_error_free(
		 &error );

		return;
	}
}

