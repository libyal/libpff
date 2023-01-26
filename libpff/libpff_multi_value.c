/*
 * Multi value functions
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
#include <memory.h>
#include <types.h>

#include "libpff_definitions.h"
#include "libpff_libcerror.h"
#include "libpff_libfmapi.h"
#include "libpff_mapi.h"
#include "libpff_multi_value.h"
#include "libpff_value_type.h"
#include "libpff_types.h"

/* Creates a multi value
 * Make sure the value multi_value is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_multi_value_initialize(
     libpff_multi_value_t **multi_value,
     libcerror_error_t **error )
{
	libpff_internal_multi_value_t *internal_multi_value = NULL;
	static char *function                               = "libpff_multi_value_initialize";

	if( multi_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid multi value.",
		 function );

		return( -1 );
	}
	if( *multi_value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid multi value value already set.",
		 function );

		return( -1 );
	}
	internal_multi_value = memory_allocate_structure(
	                        libpff_internal_multi_value_t );

	if( internal_multi_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create multi value.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_multi_value,
	     0,
	     sizeof( libpff_internal_multi_value_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear multi value.",
		 function );

		goto on_error;
	}
	*multi_value = (libpff_multi_value_t *) internal_multi_value;

	return( 1 );

on_error:
	if( internal_multi_value != NULL )
	{
		memory_free(
		 internal_multi_value );
	}
	return( -1 );
}

/* Frees a multi value
 * Returns 1 if successful or -1 on error
 */
int libpff_multi_value_free(
     libpff_multi_value_t **multi_value,
     libcerror_error_t **error )
{
	static char *function = "libpff_multi_value_free";

	if( multi_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid multi value.",
		 function );

		return( -1 );
	}
	if( *multi_value != NULL )
	{
		if( ( (libpff_internal_multi_value_t *) *multi_value )->value_data != NULL )
		{
			memory_free(
			 ( (libpff_internal_multi_value_t *) *multi_value )->value_data );
		}
		if( ( (libpff_internal_multi_value_t *) *multi_value )->value_offset != NULL )
		{
			memory_free(
			 ( (libpff_internal_multi_value_t *) *multi_value )->value_offset );
		}
		if( ( (libpff_internal_multi_value_t *) *multi_value )->value_size != NULL )
		{
			memory_free(
			 ( (libpff_internal_multi_value_t *) *multi_value )->value_size );
		}
		memory_free(
		 *multi_value );

		*multi_value = NULL;
	}
	return( 1 );
}

/* Retrieves the number of values of the multi value
 * Returns 1 if successful or -1 on error
 */
int libpff_multi_value_get_number_of_values(
     libpff_multi_value_t *multi_value,
     int *number_of_values,
     libcerror_error_t **error )
{
	libpff_internal_multi_value_t *internal_multi_value = NULL;
	static char *function                               = "libpff_multi_value_get_number_of_entries";

	if( multi_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid multi value.",
		 function );

		return( -1 );
	}
	internal_multi_value = (libpff_internal_multi_value_t *) multi_value;

	if( number_of_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of values.",
		 function );

		return( -1 );
	}
	if( internal_multi_value->number_of_values > (uint32_t) INT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid number of values value exceeds maximum.",
		 function );

		return( -1 );
	}
	*number_of_values = (int) internal_multi_value->number_of_values;

	return( 1 );
}

/* Retrieves a specific value of the multi value
 * Returns 1 if successful or -1 on error
 */
int libpff_multi_value_get_value(
     libpff_multi_value_t *multi_value,
     int value_index,
     uint32_t *value_type,
     uint8_t **value_data,
     size_t *value_data_size,
     libcerror_error_t **error )
{
	libpff_internal_multi_value_t *internal_multi_value = NULL;
	static char *function                               = "libpff_multi_value_get_value";
	uint32_t value_offset                               = 0;

	if( multi_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid multi value.",
		 function );

		return( -1 );
	}
	internal_multi_value = (libpff_internal_multi_value_t *) multi_value;

	if( value_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value type.",
		 function );

		return( -1 );
	}
	if( ( value_index < 0 )
	 || ( value_index >= (int) internal_multi_value->number_of_values ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid value index.",
		 function );

		return( -1 );
	}
	value_offset = internal_multi_value->value_offset[ value_index ];

	/* Returns the value type without the multi value flag
	 */
	*value_type      = internal_multi_value->value_type & 0xefff;
	*value_data_size = internal_multi_value->value_size[ value_index ];

	if( internal_multi_value->value_size[ value_index ] == 0 )
	{
		*value_data = NULL;
	}
	else
	{
		*value_data = &( internal_multi_value->value_data[ value_offset ] );
	}
	return( 1 );
}

/* Retrieves the 32-bit value of a specific value of the multi value
 * Returns 1 if successful or -1 on error
 */
int libpff_multi_value_get_value_32bit(
     libpff_multi_value_t *multi_value,
     int value_index,
     uint32_t *value,
     libcerror_error_t **error )
{
	uint8_t *value_data    = NULL;
	static char *function  = "libpff_multi_value_get_value_32bit";
	size_t value_data_size = 0;
	uint32_t value_type    = LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED;
	int result             = 0;

	result = libpff_multi_value_get_value(
	          multi_value,
	          value_index,
	          &value_type,
	          &value_data,
	          &value_data_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value.",
		 function );
	}
	else if( result == 1 )
	{
		if( libpff_value_type_copy_to_32bit(
		     value_data,
		     value_data_size,
		     value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set 32-bit value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the 64-bit value of a specific value of the multi value
 * Returns 1 if successful or -1 on error
 */
int libpff_multi_value_get_value_64bit(
     libpff_multi_value_t *multi_value,
     int value_index,
     uint64_t *value,
     libcerror_error_t **error )
{
	uint8_t *value_data    = NULL;
	static char *function  = "libpff_multi_value_get_value_64bit";
	size_t value_data_size = 0;
	uint32_t value_type    = LIBPFF_VALUE_TYPE_INTEGER_64BIT_SIGNED;
	int result             = 0;

	result = libpff_multi_value_get_value(
	          multi_value,
	          value_index,
	          &value_type,
	          &value_data,
	          &value_data_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value.",
		 function );
	}
	else if( result == 1 )
	{
		if( libpff_value_type_copy_to_64bit(
		     value_data,
		     value_data_size,
		     value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set 64-bit value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the 64-bit filetime value of a specific value of the multi value
 * Returns 1 if successful or -1 on error
 */
int libpff_multi_value_get_value_filetime(
     libpff_multi_value_t *multi_value,
     int value_index,
     uint64_t *filetime,
     libcerror_error_t **error )
{
	uint8_t *value_data    = NULL;
	static char *function  = "libpff_multi_value_get_value_filetime";
	size_t value_data_size = 0;
	uint32_t value_type    = LIBPFF_VALUE_TYPE_FILETIME;
	int result             = 0;

	result = libpff_multi_value_get_value(
	          multi_value,
	          value_index,
	          &value_type,
	          &value_data,
	          &value_data_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value.",
		 function );
	}
	else if( result == 1 )
	{
		if( libpff_value_type_copy_to_64bit(
		     value_data,
		     value_data_size,
		     filetime,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set filetime value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the UTF-8 string size of a specific value of the multi value
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libpff_multi_value_get_value_utf8_string_size(
     libpff_multi_value_t *multi_value,
     int value_index,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	uint8_t *value_data     = NULL;
	static char *function   = "libpff_multi_value_get_value_utf8_string_size";
	size_t value_data_size  = 0;
	uint32_t value_type     = 0;
	uint8_t is_ascii_string = 0;
	int result              = 0;

	result = libpff_multi_value_get_value(
	          multi_value,
	          value_index,
	          &value_type,
	          &value_data,
	          &value_data_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value.",
		 function );
	}
	else if( result == 1 )
	{
		if( ( value_type != LIBPFF_VALUE_TYPE_STRING_ASCII )
		 && ( value_type != LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported string value type: 0x%04" PRIx32 ".",
			 function,
			 value_type );

			return( -1 );
		}
		if( value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
		{
			is_ascii_string = 1;
		}
		if( libpff_value_type_get_utf8_string_size(
		     value_data,
		     value_data_size,
		     is_ascii_string,
		     ( (libpff_internal_multi_value_t *) multi_value )->ascii_codepage,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set UTF-8 string size.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the UTF-8 string value of a specific value of the multi value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libpff_multi_value_get_value_utf8_string(
     libpff_multi_value_t *multi_value,
     int value_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	uint8_t *value_data     = NULL;
	static char *function   = "libpff_multi_value_get_value_utf8_string";
	size_t value_data_size  = 0;
	uint32_t value_type     = 0;
	uint8_t is_ascii_string = 0;
	int result              = 0;

	result = libpff_multi_value_get_value(
	          multi_value,
	          value_index,
	          &value_type,
	          &value_data,
	          &value_data_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value.",
		 function );
	}
	else if( result == 1 )
	{
		if( ( value_type != LIBPFF_VALUE_TYPE_STRING_ASCII )
		 && ( value_type != LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported string value type: 0x%04" PRIx32 ".",
			 function,
			 value_type );

			return( -1 );
		}
		if( value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
		{
			is_ascii_string = 1;
		}
		if( libpff_value_type_copy_to_utf8_string(
		     value_data,
		     value_data_size,
		     is_ascii_string,
		     ( (libpff_internal_multi_value_t *) multi_value )->ascii_codepage,
		     utf8_string,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set UTF-8 string.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the UTF-16 string size of a specific value of the multi value
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libpff_multi_value_get_value_utf16_string_size(
     libpff_multi_value_t *multi_value,
     int value_index,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	uint8_t *value_data     = NULL;
	static char *function   = "libpff_multi_value_get_value_utf16_string_size";
	size_t value_data_size  = 0;
	uint32_t value_type     = 0;
	uint8_t is_ascii_string = 0;
	int result              = 0;

	result = libpff_multi_value_get_value(
	          multi_value,
	          value_index,
	          &value_type,
	          &value_data,
	          &value_data_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value.",
		 function );
	}
	else if( result == 1 )
	{
		if( ( value_type != LIBPFF_VALUE_TYPE_STRING_ASCII )
		 && ( value_type != LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported string value type: 0x%04" PRIx32 ".",
			 function,
			 value_type );

			return( -1 );
		}
		if( value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
		{
			is_ascii_string = 1;
		}
		if( libpff_value_type_get_utf16_string_size(
		     value_data,
		     value_data_size,
		     is_ascii_string,
		     ( (libpff_internal_multi_value_t *) multi_value )->ascii_codepage,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set UTF-16 string size.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the UTF-16 string value of a specific value of the multi value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libpff_multi_value_get_value_utf16_string(
     libpff_multi_value_t *multi_value,
     int value_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	uint8_t *value_data     = NULL;
	static char *function   = "libpff_multi_value_get_value_utf16_string";
	size_t value_data_size  = 0;
	uint32_t value_type     = 0;
	uint8_t is_ascii_string = 0;
	int result              = 0;

	result = libpff_multi_value_get_value(
	          multi_value,
	          value_index,
	          &value_type,
	          &value_data,
	          &value_data_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value.",
		 function );
	}
	else if( result == 1 )
	{
		if( ( value_type != LIBPFF_VALUE_TYPE_STRING_ASCII )
		 && ( value_type != LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported string value type: 0x%04" PRIx32 ".",
			 function,
			 value_type );

			return( -1 );
		}
		if( value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
		{
			is_ascii_string = 1;
		}
		if( libpff_value_type_copy_to_utf16_string(
		     value_data,
		     value_data_size,
		     is_ascii_string,
		     ( (libpff_internal_multi_value_t *) multi_value )->ascii_codepage,
		     utf16_string,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set UTF-16 string.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the size of a binary data value of a specific value of the multi value
 * Returns 1 if successful or -1 on error
 */
int libpff_multi_value_get_value_binary_data_size(
     libpff_multi_value_t *multi_value,
     int value_index,
     size_t *size,
     libcerror_error_t **error )
{
	uint8_t *value_data    = NULL;
	static char *function  = "libpff_multi_value_get_value_binary_data_size";
	size_t value_data_size = 0;
	uint32_t value_type    = LIBPFF_VALUE_TYPE_BINARY_DATA;
	int result             = 0;

	result = libpff_multi_value_get_value(
	          multi_value,
	          value_index,
	          &value_type,
	          &value_data,
	          &value_data_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value.",
		 function );
	}
	else if( result == 1 )
	{
		if( libpff_value_type_get_binary_data_size(
		     value_data,
		     value_data_size,
		     size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set binary data size.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the binary data value of a specific value of the multi value
 * Returns 1 if successful or -1 on error
 */
int libpff_multi_value_get_value_binary_data(
     libpff_multi_value_t *multi_value,
     int value_index,
     uint8_t *binary_data,
     size_t size,
     libcerror_error_t **error )
{
	uint8_t *value_data    = NULL;
	static char *function  = "libpff_multi_value_get_value_binary_data";
	size_t value_data_size = 0;
	uint32_t value_type    = LIBPFF_VALUE_TYPE_BINARY_DATA;
	int result             = 0;

	result = libpff_multi_value_get_value(
	          multi_value,
	          value_index,
	          &value_type,
	          &value_data,
	          &value_data_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value.",
		 function );
	}
	else if( result == 1 )
	{
		if( libpff_value_type_copy_to_binary_data(
		     value_data,
		     value_data_size,
		     binary_data,
		     size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set binary data.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the GUID value of a specific value of the multi value
 * Returns 1 if successful or -1 on error
 */
int libpff_multi_value_get_value_guid(
     libpff_multi_value_t *multi_value,
     int value_index,
     uint8_t *guid,
     size_t size,
     libcerror_error_t **error )
{
	uint8_t *value_data    = NULL;
	static char *function  = "libpff_multi_value_get_value_guid";
	size_t value_data_size = 0;
	uint32_t value_type    = LIBPFF_VALUE_TYPE_GUID;
	int result             = 0;

	result = libpff_multi_value_get_value(
	          multi_value,
	          value_index,
	          &value_type,
	          &value_data,
	          &value_data_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value.",
		 function );
	}
	else if( result == 1 )
	{
		if( libpff_value_type_copy_to_binary_data(
		     value_data,
		     value_data_size,
		     guid,
		     size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set GUID.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

