/*
 * Java string functions
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
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include <jni.h>

#include "jpff_jni.h"

static jmethodID MID_String_getBytes = NULL;

/* Determines the C narrow string size of a Java string
 * Returns 1 if successful or -1 on error
 */
int jpff_string_get_narrow_string_size(
     JNIEnv *environment,
     jstring string,
     size_t *narrow_string_size )
{
	jbyteArray byte_array = NULL;
	jclass string_class   = NULL;
	jthrowable exception  = NULL;
	static char *function = "jpff_string_get_narrow_string_size";

	if( ( environment == NULL )
	 || ( *environment == NULL ) )
	{
		return( -1 );
	}
	if( string == NULL )
	{
		return( -1 );
	}
	if( narrow_string_size == NULL )
	{
		return( -1 );
	}
	if( MID_String_getBytes == NULL )
	{
		string_class = jpff_jni_environment_find_class(
		                environment,
		                "java/lang/String" );

		if( string_class == 0 )
		{
			return( -1 );
		}
		MID_String_getBytes = jpff_jni_environment_get_method_identifier(
		                       environment,
		                       string_class,
		                       "getBytes",
		                       "()[B" );

		if( MID_String_getBytes == NULL )
		{
			return( -1 );
		}
	}
	byte_array = jpff_jni_environment_call_method(
	              environment,
	              string,
	              MID_String_getBytes );

	exception = jpff_jni_environment_exception_occurred(
	             environment );

	if( exception != NULL )
	{
		jpff_jni_environment_delete_local_reference(
		 environment,
		 byte_array );

		jpff_jni_environment_delete_local_reference(
		 environment,
		 exception );

		return( -1 );
	}
	*narrow_string_size = (size_t) jpff_jni_environment_get_array_length(
	                                environment,
	                                byte_array );

	*narrow_string_size += 1;

	jpff_jni_environment_delete_local_reference(
	 environment,
	 byte_array );

	return( 1 );
}

/* Copies the Java string to a C narrow string
 * Returns 1 if successful or -1 on error
 */
int jpff_string_copy_to_narrow_string(
     JNIEnv *environment,
     jstring string,
     char *narrow_string,
     size_t narrow_string_size )
{
	jbyteArray byte_array = NULL;
	jclass string_class   = NULL;
	jthrowable exception  = NULL;
	static char *function = "jpff_string_copy_to_narrow_string";

	if( ( environment == NULL )
	 || ( *environment == NULL ) )
	{
		return( -1 );
	}
	if( string == NULL )
	{
		return( -1 );
	}
	if( narrow_string == NULL )
	{
		return( -1 );
	}
	if( narrow_string_size > (size_t) SSIZE_MAX )
	{
		return( -1 );
	}
	if( MID_String_getBytes == NULL )
	{
		string_class = jpff_jni_environment_find_class(
		                environment,
		                "java/lang/String" );

		if( string_class == 0 )
		{
			return( -1 );
		}
		MID_String_getBytes = jpff_jni_environment_get_method_identifier(
		                       environment,
		                       string_class,
		                       "getBytes",
		                       "()[B" );

		if(MID_String_getBytes == NULL )
		{
			return( -1 );
		}
	}
	byte_array = jpff_jni_environment_call_method(
	              environment,
	              string,
	              MID_String_getBytes );

	exception = jpff_jni_environment_exception_occurred(
	             environment );

	if( exception != NULL )
	{
		jpff_jni_environment_delete_local_reference(
		 environment,
		 byte_array );

		jpff_jni_environment_delete_local_reference(
		 environment,
		 exception );

		return( -1 );
	}
	jpff_jni_environment_get_byte_array_region(
	 environment,
	 byte_array,
	 0,
	 narrow_string_size - 1,
	 (jbyte *) narrow_string );

	narrow_string[ narrow_string_size - 1 ] = 0;

	jpff_jni_environment_delete_local_reference(
	 environment,
	 byte_array );

	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Determines the C wide string size of a Java string
 * Returns 1 if successful or -1 on error
 */
int jpff_string_get_wide_string_size(
     JNIEnv *environment,
     jstring string,
     size_t *wide_string_size )
{
	static char *function = "jpff_string_get_wide_string_size";

	if( ( environment == NULL )
	 || ( *environment == NULL ) )
	{
		return( -1 );
	}
	if( string == NULL )
	{
		return( -1 );
	}
	if( wide_string_size == NULL )
	{
		return( -1 );
	}
	*wide_string_size = (size_t) jpff_jni_environment_get_string_length(
	                              environment,
	                              string );

	*wide_string_size += 1;

	return( 1 );
}

/* Copies the Java string to a C wide string
 * Returns 1 if successful or -1 on error
 */
int jpff_string_copy_to_wide_string(
     JNIEnv *environment,
     jstring string,
     wchar_t *wide_string,
     size_t wide_string_size )
{
	jchar* character_array = NULL;
	jthrowable exception   = NULL;
	static char *function  = "jpff_string_copy_to_wide_string";

	if( ( environment == NULL )
	 || ( *environment == NULL ) )
	{
		return( -1 );
	}
	if( string == NULL )
	{
		return( -1 );
	}
	if( wide_string == NULL )
	{
		return( -1 );
	}
	if( wide_string_size > (size_t) SSIZE_MAX )
	{
		return( -1 );
	}
	character_array = jpff_jni_environment_get_string_characters(
	                   environment,
	                   string,
	                   NULL );

	if( memory_copy(
	     wide_string,
	     character_array,
	     sizeof( wchar_t ) * wide_string_size ) == NULL )
	{
		jpff_jni_environment_delete_local_reference(
		 environment,
		 character_array );

		return( -1 );
	}
	jpff_jni_environment_delete_local_reference(
	 environment,
	 character_array );

	return( 1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

