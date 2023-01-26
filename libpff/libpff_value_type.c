/*
 * Values type functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libpff_definitions.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libuna.h"
#include "libpff_value_type.h"

/* Function to determine if there are zero bytes in a string
 * Trailing zero bytes not included
 * Returns 1 if the buffer contains zero bytes, 0 if not or -1 on error
 */
int libpff_value_type_string_contains_zero_bytes(
     const uint8_t *buffer,
     size_t buffer_size,
     libcerror_error_t **error )
{
	static char *function   = "libpff_value_type_string_contains_zero_bytes";
	size_t buffer_offset    = 0;
	uint8_t zero_byte_found = 0;

	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	for( buffer_offset = 0;
	     buffer_offset < buffer_size;
	     buffer_offset++ )
	{
		if( zero_byte_found == 0 )
		{
			if( buffer[ buffer_offset ] == 0 )
			{
				zero_byte_found = 1;
			}
		}
		else
		{
			if( buffer[ buffer_offset ] != 0 )
			{
				return( 1 );
			}
		}
	}
	return( 0 );
}

/* Converts the value data into a 32-bit value
 * Returns 1 if successful or -1 on error
 */
int libpff_value_type_copy_to_32bit(
     const uint8_t *value_data,
     size_t value_data_size,
     uint32_t *value_32bit,
     libcerror_error_t **error )
{
	static char *function = "libpff_value_type_copy_to_32bit";

	if( value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data.",
		 function );

		return( -1 );
	}
	if( value_32bit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value 32-bit.",
		 function );

		return( -1 );
	}
	/* The value data size of a 32-bit value is 4
	 */
	if( value_data_size != 4 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid value data size.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 value_data,
	 *value_32bit );

	return( 1 );
}

/* Converts the value data into a 64-bit value
 * Returns 1 if successful or -1 on error
 */
int libpff_value_type_copy_to_64bit(
     const uint8_t *value_data,
     size_t value_data_size,
     uint64_t *value_64bit,
     libcerror_error_t **error )
{
	static char *function = "libpff_value_type_copy_to_64bit";

	if( value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data.",
		 function );

		return( -1 );
	}
	if( value_64bit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value 64-bit.",
		 function );

		return( -1 );
	}
	/* The value data size of a 64-bit value is 8
	 */
	if( value_data_size != 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid value data size.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint64_little_endian(
	 value_data,
	 *value_64bit );

	return( 1 );
}

/* Determines the UTF-8 string size from the value data
 * Returns 1 if successful or -1 on error
 */
int libpff_value_type_get_utf8_string_size(
     const uint8_t *value_data,
     size_t value_data_size,
     uint8_t is_ascii_string,
     uint32_t ascii_codepage,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libpff_value_type_get_utf8_string_size";
	int result            = 0;

	if( utf8_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string size.",
		 function );

		return( -1 );
	}
	/* Internally an empty string is represented by a NULL reference
	 */
	if( value_data == NULL )
	{
		*utf8_string_size = 0;

		return( 1 );
	}
	/* Codepage 1200 represents Unicode
	 * If the codepage is 1200 find out if the string is encoded in UTF-8 or UTF-16 little-endian
	 */
	if( ( is_ascii_string != 0 )
	 && ( ascii_codepage == 1200 ) )
	{
		result = libpff_value_type_string_contains_zero_bytes(
			  value_data,
			  value_data_size,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if value type contains zero bytes.",
			 function );

			return( -1 );
		}
		else if( result != 0 )
		{
			is_ascii_string = 0;
		}
	}
	/* String is in UTF-16 little-endian
	 */
	if( is_ascii_string == 0 )
	{
		result = libuna_utf8_string_size_from_utf16_stream(
		          value_data,
		          value_data_size,
		          LIBUNA_ENDIAN_LITTLE,
		          utf8_string_size,
		          error );
	}
	/* Codepage 65000 represents UTF-7
	 */
	else if( ascii_codepage == 65000 )
	{
		result = libuna_utf8_string_size_from_utf7_stream(
			  value_data,
			  value_data_size,
			  utf8_string_size,
			  error );
	}
	/* Codepage 1200 or 65001 represents UTF-8
	 */
	else if( ( ascii_codepage == 1200 )
	      || ( ascii_codepage == 65001 ) )
	{
		result = libuna_utf8_string_size_from_utf8_stream(
			  value_data,
			  value_data_size,
			  utf8_string_size,
			  error );
	}
	else
	{
		/* TODO currently libuna uses the same numeric values for the codepages as PFF
		 * add a mapping function if this implementation changes
		 */
		result = libuna_utf8_string_size_from_byte_stream(
			  value_data,
			  value_data_size,
			  (int) ascii_codepage,
			  utf8_string_size,
			  error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Converts the value data into an UTF-8 string
 * Returns 1 if successful or -1 on error
 */
int libpff_value_type_copy_to_utf8_string(
     const uint8_t *value_data,
     size_t value_data_size,
     uint8_t is_ascii_string,
     uint32_t ascii_codepage,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libpff_value_type_copy_to_utf8_string";
	int result            = 0;

	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid UTF-8 string size value zero or less.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* Internally an empty string is represented by a NULL reference
	 */
	if( value_data == NULL )
	{
		utf8_string[ 0 ] = 0;

		return( 1 );
	}
	/* Codepage 1200 represents Unicode
	 * If the codepage is 1200 find out if the string is encoded in UTF-8 or UTF-16 little-endian
	 */
	if( ( is_ascii_string != 0 )
	 && ( ascii_codepage == 1200 ) )
	{
		result = libpff_value_type_string_contains_zero_bytes(
			  value_data,
			  value_data_size,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if value type contains zero bytes.",
			 function );

			return( -1 );
		}
		else if( result != 0 )
		{
			is_ascii_string = 0;
		}
	}
	/* String is in UTF-16 little-endian
	 */
	if( is_ascii_string == 0 )
	{
		result = libuna_utf8_string_copy_from_utf16_stream(
		          utf8_string,
		          utf8_string_size,
		          value_data,
		          value_data_size,
		          LIBUNA_ENDIAN_LITTLE,
		          error );
	}
	/* Codepage 65000 represents UTF-7
	 */
	else if( ascii_codepage == 65000 )
	{
		result = libuna_utf8_string_copy_from_utf7_stream(
		          utf8_string,
		          utf8_string_size,
		          value_data,
		          value_data_size,
		          error );
	}
	/* Codepage 1200 or 65001 represents UTF-8
	 */
	else if( ( ascii_codepage == 1200 )
	      || ( ascii_codepage == 65001 ) )
	{
		result = libuna_utf8_string_copy_from_utf8_stream(
		          utf8_string,
		          utf8_string_size,
		          value_data,
		          value_data_size,
		          error );
	}
	else
	{
		/* TODO currently libuna uses the same numeric values for the codepages as PFF
		 * add a mapping function if this implementation changes
		 */
		result = libuna_utf8_string_copy_from_byte_stream(
		          utf8_string,
		          utf8_string_size,
		          value_data,
		          value_data_size,
		          (int) ascii_codepage,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Determines the UTF-16 string size from the value data
 * Returns 1 if successful or -1 on error
 */
int libpff_value_type_get_utf16_string_size(
     const uint8_t *value_data,
     size_t value_data_size,
     uint8_t is_ascii_string,
     uint32_t ascii_codepage,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libpff_value_type_get_utf16_string_size";
	int result            = 0;

	if( utf16_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string size.",
		 function );

		return( -1 );
	}
	/* Internally an empty string is represented by a NULL reference
	 */
	if( value_data == NULL )
	{
		*utf16_string_size = 0;

		return( 1 );
	}
	/* Codepage 1200 represents Unicode
	 * If the codepage is 1200 find out if the string is encoded in UTF-8 or UTF-16 little-endian
	 */
	if( ( is_ascii_string != 0 )
	 && ( ascii_codepage == 1200 ) )
	{
		result = libpff_value_type_string_contains_zero_bytes(
			  value_data,
			  value_data_size,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if value type contains zero bytes.",
			 function );

			return( -1 );
		}
		else if( result != 0 )
		{
			is_ascii_string = 0;
		}
	}
	/* String is in UTF-16 little-endian
	 */
	if( is_ascii_string == 0 )
	{
		result = libuna_utf16_string_size_from_utf16_stream(
		          value_data,
		          value_data_size,
		          LIBUNA_ENDIAN_LITTLE,
		          utf16_string_size,
		          error );
	}
	/* Codepage 65000 represents UTF-7
	 */
	else if( ascii_codepage == 65000 )
	{
		result = libuna_utf16_string_size_from_utf7_stream(
		          value_data,
		          value_data_size,
		          utf16_string_size,
		          error );
	}
	/* Codepage 1200 or 65001 represents UTF-8
	 */
	else if( ( ascii_codepage == 1200 )
	      || ( ascii_codepage == 65001 ) )
	{
		result = libuna_utf16_string_size_from_utf8(
		          value_data,
		          value_data_size,
		          utf16_string_size,
		          error );
	}
	else
	{
		/* TODO currently libuna uses the same numeric values for the codepages as PFF
		 * add a mapping function if this implementation changes
		 */
		result = libuna_utf16_string_size_from_byte_stream(
		          value_data,
		          value_data_size,
		          (int) ascii_codepage,
		          utf16_string_size,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Converts the value data into an UTF-16 string
 * Returns 1 if successful or -1 on error
 */
int libpff_value_type_copy_to_utf16_string(
     const uint8_t *value_data,
     size_t value_data_size,
     uint8_t is_ascii_string,
     uint32_t ascii_codepage,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libpff_value_type_copy_to_utf16_string";
	int result            = 0;

	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid UTF-16 string size value zero or less.",
		 function );

		return( -1 );
	}
	if( utf16_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* Internally an empty string is represented by a NULL reference
	 */
	if( value_data == NULL )
	{
		utf16_string[ 0 ] = 0;

		return( 1 );
	}
	/* Codepage 1200 represents Unicode
	 * If the codepage is 1200 find out if the string is encoded in UTF-8 or UTF-16 little-endian
	 */
	if( ( is_ascii_string != 0 )
	 && ( ascii_codepage == 1200 ) )
	{
		result = libpff_value_type_string_contains_zero_bytes(
			  value_data,
			  value_data_size,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if value type contains zero bytes.",
			 function );

			return( -1 );
		}
		else if( result != 0 )
		{
			is_ascii_string = 0;
		}
	}
	/* String is in UTF-16 little-endian
	 */
	if( is_ascii_string == 0 )
	{
		result = libuna_utf16_string_copy_from_utf16_stream(
		          utf16_string,
		          utf16_string_size,
		          value_data,
		          value_data_size,
		          LIBUNA_ENDIAN_LITTLE,
		          error );
	}
	/* Codepage 65000 represents UTF-7
	 */
	else if( ascii_codepage == 65000 )
	{
		result = libuna_utf16_string_copy_from_utf7_stream(
		          utf16_string,
		          utf16_string_size,
		          value_data,
		          value_data_size,
		          error );
	}
	/* Codepage 1200 or 65001 represents UTF-8
	 */
	else if( ( ascii_codepage == 1200 )
	      || ( ascii_codepage == 65001 ) )
	{
		result = libuna_utf16_string_copy_from_utf8(
		          utf16_string,
		          utf16_string_size,
		          value_data,
		          value_data_size,
		          error );
	}
	else
	{
		/* TODO currently libuna uses the same numeric values for the codepages as PFF
		 * add a mapping function if this implementation changes
		 */
		result = libuna_utf16_string_copy_from_byte_stream(
		          utf16_string,
		          utf16_string_size,
		          value_data,
		          value_data_size,
		          (int) ascii_codepage,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Determines the binary data size from the value data
 * Returns 1 if successful or -1 on error
 */
int libpff_value_type_get_binary_data_size(
     const uint8_t *value_data,
     size_t value_data_size,
     size_t *binary_data_size,
     libcerror_error_t **error )
{
	static char *function = "libpff_value_type_get_binary_data_size";

	if( binary_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary data size.",
		 function );

		return( -1 );
	}
	/* Internally an empty binary data is represented by a NULL reference
	 */
	if( value_data == NULL )
	{
		*binary_data_size = 0;
	}
	else
	{
		*binary_data_size = value_data_size;
	}
	return( 1 );
}

/* Converts the value data into binary data
 * Returns 1 if successful or -1 on error
 */
int libpff_value_type_copy_to_binary_data(
     const uint8_t *value_data,
     size_t value_data_size,
     uint8_t *binary_data,
     size_t binary_data_size,
     libcerror_error_t **error )
{
	static char *function = "libpff_value_type_copy_to_binary_data";

	if( binary_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary data.",
		 function );

		return( -1 );
	}
	if( binary_data_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid binary data size value zero or less.",
		 function );

		return( -1 );
	}
	if( binary_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid binary data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* Internally empty binary data is represented by a NULL reference
	 */
	if( value_data != NULL )
	{
		if( binary_data_size < value_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: binary data is too small.",
			 function );

			return( -1 );
		}
		if( memory_copy(
		     binary_data,
		     value_data,
		     value_data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set binary data.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

