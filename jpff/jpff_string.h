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

#if !defined( _JPFF_STRING_H )
#define _JPFF_STRING_H

#include <common.h>
#include <types.h>

#include <jni.h>

#if defined( __cplusplus )
extern "C" {
#endif

int jpff_string_get_narrow_string_size(
     JNIEnv *environment,
     jstring string,
     size_t *narrow_string_size );

int jpff_string_copy_to_narrow_string(
     JNIEnv *environment,
     jstring string,
     char *narrow_string,
     size_t narrow_string_size );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
int jpff_string_get_wide_string_size(
     JNIEnv *environment,
     jstring string,
     size_t *wide_string_size );

int jpff_string_copy_to_wide_string(
     JNIEnv *environment,
     jstring string,
     wchar_t *wide_string,
     size_t wide_string_size );
#endif

#if defined( __cplusplus )
}
#endif

#endif

