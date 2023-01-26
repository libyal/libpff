/*
 * Common input functions for the pfftools
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
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "export_handle.h"
#include "pffinput.h"
#include "pfftools_libcerror.h"
#include "pfftools_libpff.h"

/* Determines the export format from a string
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int pffinput_determine_export_format(
     const system_character_t *string,
     int *export_format,
     libcerror_error_t **error )
{
	static char *function = "pffinput_determine_export_format";
	size_t string_length  = 0;
	int result            = 0;

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string string.",
		 function );

		return( -1 );
	}
	if( export_format == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export format.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	if( string_length == 3 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "all" ),
		     3 ) == 0 )
		{
			*export_format = EXPORT_FORMAT_ALL;
			result         = 1;
		}
		else if( system_string_compare(
			  string,
			  _SYSTEM_STRING( "ftk" ),
			  3 ) == 0 )
		{
			*export_format = EXPORT_FORMAT_FTK;
			result         = 1;
		}
		else if( system_string_compare(
			  string,
			  _SYSTEM_STRING( "htm" ),
			  3 ) == 0 )
		{
			*export_format = EXPORT_FORMAT_HTML;
			result         = 1;
		}
		else if( system_string_compare(
			  string,
			  _SYSTEM_STRING( "rtf" ),
			  3 ) == 0 )
		{
			*export_format = EXPORT_FORMAT_RTF;
			result         = 1;
		}
		else if( system_string_compare(
			  string,
			  _SYSTEM_STRING( "txt" ),
			  3 ) == 0 )
		{
			*export_format = EXPORT_FORMAT_TEXT;
			result         = 1;
		}
	}
	else if( string_length == 4 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "html" ),
		     4 ) == 0 )
		{
			*export_format = EXPORT_FORMAT_HTML;
			result         = 1;
		}
		else if( system_string_compare(
			  string,
			  _SYSTEM_STRING( "text" ),
			  4 ) == 0 )
		{
			*export_format = EXPORT_FORMAT_TEXT;
			result         = 1;
		}
	}
	return( result );
}

