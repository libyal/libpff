/*
 * JNI functions
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

#if !defined( _JPFF_JNI_H )
#define _JPFF_JNI_H

#include <common.h>
#include <types.h>

#include <jni.h>

#if defined( __cplusplus )
extern "C" {
#endif

/* JNI class/object functions
 */
#define jpff_jni_environment_find_class( environment, class_name ) \
	( *environment )->FindClass( environment, class_name )

#define jpff_jni_environment_get_field_identifier( environment, jni_object, field_name, signature ) \
	( *environment )->GetFieldID( environment, jni_object, field_name, signature )

#define jpff_jni_environment_get_long_field( environment, jni_object, field_identifier ) \
	( *environment )->GetLongField( environment, jni_object, field_identifier )

#define jpff_jni_environment_set_long_field( environment, jni_object, field_identifier, value ) \
	( *environment )->SetLongField( environment, jni_object, field_identifier, value )

#define jpff_jni_environment_get_method_identifier( environment, jni_object, method_name, signature ) \
	( *environment )->GetMethodID( environment, jni_object, method_name, signature )

#define jpff_jni_environment_call_method( environment, jni_object, method_identifier ) \
	( *environment )->CallObjectMethod( environment, jni_object, method_identifier )

#define jpff_jni_environment_delete_local_reference( environment, jni_object ) \
	( *environment )->DeleteLocalRef( environment, jni_object )

/* JNI exception functions
 */
#define jpff_jni_environment_exception_clear( environment ) \
	( *environment )->ExceptionClear( environment )

#define jpff_jni_environment_exception_occurred( environment ) \
	( *environment )->ExceptionOccurred( environment )

#define jpff_jni_environment_throw_new( environment, execption, error_string ) \
	( *environment )->ThrowNew( environment, execption, error_string )

/* JNI array functions
 */
#define jpff_jni_environment_get_array_length( environment, array ) \
	( *environment )->GetArrayLength( environment, array )

/* JNI byte array functions
 */
#define jpff_jni_environment_get_byte_array_region( environment, byte_array, index, length, buffer ) \
	( *environment )->GetByteArrayRegion( environment, byte_array, index, length, buffer )

/* JNI string functions
 */
#define jpff_jni_environment_get_string_length( environment, string ) \
	( *environment )->GetStringLength( environment, string )

#define jpff_jni_environment_get_string_characters( environment, string, is_copy ) \
	( *environment )->GetStringChars( environment, string, is_copy )

#if defined( __cplusplus )
}
#endif

#endif

