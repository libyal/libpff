/*
 * Export handle
 *
 * Copyright (C) 2008-2021, Joachim Metz <joachim.metz@gmail.com>
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
#include <file_stream.h>
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "export_handle.h"
#include "item_file.h"
#include "mapi_property_definition.h"
#include "log_handle.h"
#include "pffinput.h"
#include "pfftools_libcerror.h"
#include "pfftools_libclocale.h"
#include "pfftools_libcnotify.h"
#include "pfftools_libcfile.h"
#include "pfftools_libcpath.h"
#include "pfftools_libfdatetime.h"
#include "pfftools_libfguid.h"
#include "pfftools_libfmapi.h"
#include "pfftools_libpff.h"

#define EXPORT_HANDLE_BUFFER_SIZE		8192
#define EXPORT_HANDLE_NOTIFY_STREAM		stdout

/* Creates an export handle
 * Make sure the value export_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int export_handle_initialize(
     export_handle_t **export_handle,
     libcerror_error_t **error )
{
	static char *function = "export_handle_initialize";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( *export_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid export handle value already set.",
		 function );

		return( -1 );
	}
	*export_handle = memory_allocate_structure(
	                  export_handle_t );

	if( *export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create export handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *export_handle,
	     0,
	     sizeof( export_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear export handle.",
		 function );

		goto on_error;
	}
	( *export_handle )->export_mode              = EXPORT_MODE_ITEMS;
	( *export_handle )->preferred_export_format  = EXPORT_FORMAT_TEXT;
	( *export_handle )->ascii_codepage           = LIBPFF_CODEPAGE_WINDOWS_1252;
	( *export_handle )->print_status_information = 1;
	( *export_handle )->notify_stream            = EXPORT_HANDLE_NOTIFY_STREAM;

	return( 1 );

on_error:
	if( *export_handle != NULL )
	{
		memory_free(
		 *export_handle );

		*export_handle = NULL;
	}
	return( -1 );
}

/* Frees an export handle
 * Returns 1 if successful or -1 on error
 */
int export_handle_free(
     export_handle_t **export_handle,
     libcerror_error_t **error )
{
	static char *function = "export_handle_free";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( *export_handle != NULL )
	{
		if( ( *export_handle )->target_path != NULL )
		{
			memory_free(
			 ( *export_handle )->target_path );
		}
		if( ( *export_handle )->items_export_path != NULL )
		{
			memory_free(
			 ( *export_handle )->items_export_path );
		}
		if( ( *export_handle )->orphans_export_path != NULL )
		{
			memory_free(
			 ( *export_handle )->orphans_export_path );
		}
		if( ( *export_handle )->recovered_export_path != NULL )
		{
			memory_free(
			 ( *export_handle )->recovered_export_path );
		}
		memory_free(
		 *export_handle );

		*export_handle = NULL;
	}
	return( 1 );
}

/* Signals the export handle to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int export_handle_signal_abort(
     export_handle_t *export_handle,
     libcerror_error_t **error )
{
	static char *function = "export_handle_signal_abort";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	export_handle->abort = 1;

	return( 1 );
}

/* Sets the export mode
 * Returns 1 if successful, 0 if unsupported values or -1 on error
 */
int export_handle_set_export_mode(
     export_handle_t *export_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "export_handle_set_export_mode";
	size_t string_length  = 0;
	int result            = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
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
			export_handle->export_mode = EXPORT_MODE_ALL;

			result = 1;
		}
	}
	else if( string_length == 5 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "debug" ),
		     5 ) == 0 )
		{
			export_handle->export_mode = EXPORT_MODE_DEBUG;

			result = 1;
		}
		else if( system_string_compare(
			  string,
			  _SYSTEM_STRING( "items" ),
			  5 ) == 0 )
		{
			export_handle->export_mode = EXPORT_MODE_ITEMS;

			result = 1;
		}
		else if( system_string_compare(
			  string,
			  _SYSTEM_STRING( "noatt" ),
			  5 ) == 0 )
		{
			export_handle->export_mode = EXPORT_MODE_ITEMS_NO_ATTACHMENTS;

			result = 1;
		}
	}
	else if( string_length == 9 )
	{
		if( system_string_compare(
		     string,
		     _SYSTEM_STRING( "recovered" ),
		     9 ) == 0 )
		{
			export_handle->export_mode = EXPORT_MODE_RECOVERED;

			result = 1;
		}
	}
	return( result );
}

/* Sets the preferred export format
 * Returns 1 if successful, 0 if unsupported values or -1 on error
 */
int export_handle_set_preferred_export_format(
     export_handle_t *export_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "export_handle_preferred_export_format";
	int result            = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	result = pffinput_determine_export_format(
		  string,
		  &( export_handle->preferred_export_format ),
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine preferred export format.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Sets the ascii codepage
 * Returns 1 if successful or -1 on error
 */
int export_handle_set_ascii_codepage(
     export_handle_t *export_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function  = "export_handle_set_ascii_codepage";
	size_t string_length   = 0;
	uint32_t feature_flags = 0;
	int result             = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	feature_flags = LIBCLOCALE_CODEPAGE_FEATURE_FLAG_HAVE_KOI8
	              | LIBCLOCALE_CODEPAGE_FEATURE_FLAG_HAVE_WINDOWS;

	string_length = system_string_length(
	                 string );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libclocale_codepage_copy_from_string_wide(
	          &( export_handle->ascii_codepage ),
	          string,
	          string_length,
	          feature_flags,
	          error );
#else
	result = libclocale_codepage_copy_from_string(
	          &( export_handle->ascii_codepage ),
	          string,
	          string_length,
	          feature_flags,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine ASCII codepage.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Sets the target path
 * Returns 1 if successful or -1 on error
 */
int export_handle_set_target_path(
     export_handle_t *export_handle,
     const system_character_t *target_path,
     libcerror_error_t **error )
{
	static char *function                = "export_handle_set_target_path";
	size_t target_path_length            = 0;

#if defined( WINAPI )
	system_character_t *full_target_path = NULL;
        size_t full_target_path_size         = 0;
	int result                           = 0;
#endif

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( target_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid target path.",
		 function );

		return( -1 );
	}
	if( export_handle->target_path != NULL )
	{
		memory_free(
		 export_handle->target_path );

		export_handle->target_path      = NULL;
		export_handle->target_path_size = 0;
	}
	target_path_length = system_string_length(
	                      target_path );

#if defined( WINAPI )
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libcpath_path_get_full_path_wide(
	          target_path,
                  target_path_length,
                  &full_target_path,
                  &full_target_path_size,
                  error );
#else
	result = libcpath_path_get_full_path(
	          target_path,
                  target_path_length,
                  &full_target_path,
                  &full_target_path_size,
                  error );
#endif
        if( result == -1 )
        {
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create extended-length target path.",
		 function );

		goto on_error;
        }
        else if( result != 0 )
        {
                target_path        = full_target_path;
                target_path_length = full_target_path_size - 1;
        }
#endif
	if( target_path_length > 0 )
	{
		export_handle->target_path = system_string_allocate(
		                              target_path_length + 1 );

		if( export_handle->target_path == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create target path.",
			 function );

			goto on_error;
		}
		if( system_string_copy(
		     export_handle->target_path,
		     target_path,
		     target_path_length ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy target path.",
			 function );

			goto on_error;
		}
		( export_handle->target_path )[ target_path_length ] = 0;

		export_handle->target_path_size = target_path_length + 1;
	}
#if defined( WINAPI )
	memory_free(
	 full_target_path );
#endif
	return( 1 );

on_error:
	if( export_handle->target_path != NULL )
	{
		memory_free(
		 export_handle->target_path );

		export_handle->target_path      = NULL;
		export_handle->target_path_size = 0;
	}
#if defined( WINAPI )
	if( full_target_path != NULL )
	{
		memory_free(
		 full_target_path );
	}
#endif
	return( -1 );
}

/* Sets an export path consisting of a base path and a suffix
 * Returns 1 if successful or -1 on error
 */
int export_handle_set_export_path(
     export_handle_t *export_handle,
     const system_character_t *base_path,
     size_t base_path_length,
     const system_character_t *suffix,
     size_t suffix_length,
     system_character_t **export_path,
     size_t *export_path_size,
     libcerror_error_t **error )
{
	static char *function = "export_handle_set_export_path";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( base_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid base path.",
		 function );

		return( -1 );
	}
	if( base_path_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid base path length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( suffix == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid suffix.",
		 function );

		return( -1 );
	}
	if( suffix_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid suffix length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( export_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export path.",
		 function );

		return( -1 );
	}
	if( export_path_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export path size.",
		 function );

		return( -1 );
	}
	if( *export_path != NULL )
	{
		memory_free(
		 *export_path );

		*export_path      = NULL;
		*export_path_size = 0;
	}
	*export_path_size = base_path_length + suffix_length + 1;

	*export_path = system_string_allocate(
	                *export_path_size );

	if( *export_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create export path.",
		 function );

		goto on_error;
	}
	if( system_string_copy(
	     *export_path,
	     base_path,
	     base_path_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy base path to item export path.",
		 function );

		goto on_error;
	}
	if( system_string_copy(
	     &( ( *export_path )[ base_path_length ] ),
	     suffix,
	     suffix_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy suffix to item export path.",
		 function );

		goto on_error;
	}
	( *export_path )[ *export_path_size - 1 ] = 0;

	return( 1 );

on_error:
	if( *export_path != NULL )
	{
		memory_free(
		 *export_path );

		*export_path      = NULL;
		*export_path_size = 0;
	}
	return( -1 );
}

/* Creates the items export path
 * Returns 1 if successful, 0 if already exists or -1 on error
 */
int export_handle_create_items_export_path(
     export_handle_t *export_handle,
     libcerror_error_t **error )
{
	static char *function = "export_handle_create_items_export_path";
	int result            = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( ( export_handle->export_mode == EXPORT_MODE_ALL )
	 || ( export_handle->export_mode == EXPORT_MODE_DEBUG )
	 || ( export_handle->export_mode == EXPORT_MODE_ITEMS )
	 || ( export_handle->export_mode == EXPORT_MODE_ITEMS_NO_ATTACHMENTS ) )
	{
		if( export_handle_set_export_path(
		     export_handle,
		     export_handle->target_path,
		     export_handle->target_path_size - 1,
		     _SYSTEM_STRING( ".export" ),
		     7,
		     &( export_handle->items_export_path ),
		     &( export_handle->items_export_path_size ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set items export path.",
			 function );

			return( -1 );
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libcfile_file_exists_wide(
			  export_handle->items_export_path,
			  error );
#else
		result = libcfile_file_exists(
			  export_handle->items_export_path,
			  error );
#endif
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_GENERIC,
			 "%s: unable to determine if %" PRIs_SYSTEM " exists.",
			 function,
			 export_handle->items_export_path );

			return( -1 );
		}
		else if( result == 1 )
		{
			return( 0 );
		}
	}
	return( 1 );
}

/* Creates the orphans export path
 * Returns 1 if successful, 0 if already exists or -1 on error
 */
int export_handle_create_orphans_export_path(
     export_handle_t *export_handle,
     libcerror_error_t **error )
{
	static char *function = "export_handle_create_orphans_export_path";
	int result            = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( ( export_handle->export_mode == EXPORT_MODE_ALL )
	 || ( export_handle->export_mode == EXPORT_MODE_RECOVERED ) )
	{
		if( export_handle_set_export_path(
		     export_handle,
		     export_handle->target_path,
		     export_handle->target_path_size - 1,
		     _SYSTEM_STRING( ".orphans" ),
		     8,
		     &( export_handle->orphans_export_path ),
		     &( export_handle->orphans_export_path_size ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set orphans export path.",
			 function );

			return( -1 );
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libcfile_file_exists_wide(
			  export_handle->orphans_export_path,
			  error );
#else
		result = libcfile_file_exists(
			  export_handle->orphans_export_path,
			  error );
#endif
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_GENERIC,
			 "%s: unable to determine if %" PRIs_SYSTEM " exists.",
			 function,
			 export_handle->orphans_export_path );

			return( -1 );
		}
		else if( result == 1 )
		{
			return( 0 );
		}
	}
	return( 1 );
}

/* Creates the recovered export path
 * Returns 1 if successful, 0 if already exists or -1 on error
 */
int export_handle_create_recovered_export_path(
     export_handle_t *export_handle,
     libcerror_error_t **error )
{
	static char *function = "export_handle_create_recovered_export_path";
	int result            = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( ( export_handle->export_mode == EXPORT_MODE_ALL )
	 || ( export_handle->export_mode == EXPORT_MODE_RECOVERED ) )
	{
		if( export_handle_set_export_path(
		     export_handle,
		     export_handle->target_path,
		     export_handle->target_path_size - 1,
		     _SYSTEM_STRING( ".recovered" ),
		     10,
		     &( export_handle->recovered_export_path ),
		     &( export_handle->recovered_export_path_size ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set recovered export path.",
			 function );

			return( -1 );
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libcfile_file_exists_wide(
			  export_handle->recovered_export_path,
			  error );
#else
		result = libcfile_file_exists(
			  export_handle->recovered_export_path,
			  error );
#endif
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_GENERIC,
			 "%s: unable to determine if %" PRIs_SYSTEM " exists.",
			 function,
			 export_handle->recovered_export_path );

			return( -1 );
		}
		else if( result == 1 )
		{
			return( 0 );
		}
	}
	return( 1 );
}

/* Creates the default item directory path
 * Returns 1 if successful or -1 on error
 */
int export_handle_create_default_item_directory(
     export_handle_t *export_handle,
     int item_index,
     const system_character_t *item_prefix,
     size_t item_prefix_length,
     const system_character_t *export_path,
     size_t export_path_length,
     system_character_t **item_directory_path,
     size_t *item_directory_path_size,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	system_character_t item_directory_name[ 64 ];

	static char *function             = "export_handle_create_default_item_directory";
	size_t item_directory_name_length = 0;
	int print_count                   = 0;
	int result                        = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( item_index < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid item index value less than zero.",
		 function );

		return( -1 );
	}
	if( item_prefix == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item prefix.",
		 function );

		return( -1 );
	}
	if( item_prefix_length > (size_t) 63 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid item prefix length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( item_directory_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item directory path.",
		 function );

		return( -1 );
	}
	/* Create the item directory
	 */
	print_count = system_string_sprintf(
	               item_directory_name,
	               64,
	               _SYSTEM_STRING( "%s%05d" ),
	               item_prefix,
	               item_index + 1 );

	if( ( print_count < 0 )
	 || ( (size_t) print_count > ( item_prefix_length + 5 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set item directory name.",
		 function );

		goto on_error;
	}
	item_directory_name[ item_prefix_length + 5 ] = 0;
	item_directory_name_length                    = item_prefix_length + 5;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libcpath_path_join_wide(
	          item_directory_path,
	          item_directory_path_size,
	          export_path,
	          export_path_length,
	          item_directory_name,
	          item_directory_name_length,
	          error );
#else
	result = libcpath_path_join(
	          item_directory_path,
	          item_directory_path_size,
	          export_path,
	          export_path_length,
	          item_directory_name,
	          item_directory_name_length,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable create item directory path.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libcfile_file_exists_wide(
	          *item_directory_path,
	          error );
#else
	result = libcfile_file_exists(
	          *item_directory_path,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_GENERIC,
		 "%s: unable to determine if %" PRIs_SYSTEM " exists.",
		 function,
		 *item_directory_path );

		goto on_error;
	}
	else if( result != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_GENERIC,
		 "%s: %" PRIs_SYSTEM " already exists.",
		 function,
		 *item_directory_path );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcpath_path_make_directory_wide(
	     *item_directory_path,
	     error ) != 1 )
#else
	if( libcpath_path_make_directory(
	     *item_directory_path,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to make directory: %" PRIs_SYSTEM ".",
		 function,
		 *item_directory_path );

		goto on_error;
	}
	log_handle_printf(
	 log_handle,
	 "Created directory: %" PRIs_SYSTEM ".\n",
	 *item_directory_path );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: created directory: %" PRIs_SYSTEM ".\n",
		 function,
		 *item_directory_path );
	}
#endif
	return( 1 );

on_error:
	if( *item_directory_path != NULL )
	{
		memory_free(
		 *item_directory_path );

		*item_directory_path      = NULL;
		*item_directory_path_size = 0;
	}
	return( -1 );
}

/* Creates an item file
 * Returns 1 if successful, 0 if the file already exists or -1 on error
 */
int export_handle_create_item_file(
     export_handle_t *export_handle,
     const system_character_t *path,
     size_t path_length,
     const system_character_t *filename,
     size_t filename_length,
     item_file_t **item_file,
     libcerror_error_t **error )
{
	system_character_t *item_file_path = NULL;
	static char *function              = "export_handle_create_item_file";
	size_t item_file_path_size         = 0;
	int result                         = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( item_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item file.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libcpath_path_join_wide(
	          &item_file_path,
	          &item_file_path_size,
	          path,
	          path_length,
	          filename,
	          filename_length,
	          error );
#else
	result = libcpath_path_join(
	          &item_file_path,
	          &item_file_path_size,
	          path,
	          path_length,
	          filename,
	          filename_length,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create item file path.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libcfile_file_exists_wide(
	          item_file_path,
	          error );
#else
	result = libcfile_file_exists(
	          item_file_path,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_GENERIC,
		 "%s: unable to determine if %" PRIs_SYSTEM " exists.",
		 function,
		 item_file_path );

		goto on_error;
	}
	else if( result != 0 )
	{
		memory_free(
		 item_file_path );

		return( 0 );
	}
	if( item_file_initialize(
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create item file.",
		 function );

		goto on_error;
	}
	if( item_file_open(
	     *item_file,
	     item_file_path,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open: %" PRIs_SYSTEM ".",
		 function,
		 item_file_path );

		goto on_error;
	}
	memory_free(
	 item_file_path );

	item_file_path = NULL;

	return( 1 );

on_error:
	if( *item_file != NULL )
	{
		item_file_free(
		 item_file,
		 NULL );
	}
	if( item_file_path != NULL )
	{
		memory_free(
		 item_file_path );
	}
	return( -1 );
}

/* Exports the item
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_item(
     export_handle_t *export_handle,
     libpff_item_t *item,
     int item_index,
     int number_of_items,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	system_character_t *entry_value_string = NULL;
	system_character_t *item_path          = NULL;
	static char *function                  = "export_handle_export_item";
	char *item_type_string                 = NULL;
	size_t entry_value_string_size         = 0;
	size_t item_path_size                  = 0;
	uint8_t item_type                      = 0;
	int result                             = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( libpff_item_get_type(
	     item,
	     &item_type,
	     error ) != 1 )
	{
		fprintf(
		 export_handle->notify_stream,
		 "Unable to retrieve type of item %d out of %d.\n",
		 item_index + 1,
		 number_of_items );

		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve item type.",
		 function );

		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );

		log_handle_printf(
		 log_handle,
		 "Unable to retrieve type of item %d out of %d.\n",
		 item_index + 1,
		 number_of_items );

		return( 1 );
	}
	switch( item_type )
	{
		case LIBPFF_ITEM_TYPE_ACTIVITY:
			item_type_string = "activity";

			if( export_handle->print_status_information != 0 )
			{
				fprintf(
				 export_handle->notify_stream,
				 "Exporting %s item %d out of %d.\n",
				 item_type_string,
				 item_index + 1,
				 number_of_items );
			}
			result = export_handle_export_activity(
			          export_handle,
			          item,
			          item_index,
			          export_path,
			          export_path_length,
			          log_handle,
			          error );
			break;

		case LIBPFF_ITEM_TYPE_APPOINTMENT:
			item_type_string = "appointment";

			if( export_handle->print_status_information != 0 )
			{
				fprintf(
				 export_handle->notify_stream,
				 "Exporting %s item %d out of %d.\n",
				 item_type_string,
				 item_index + 1,
				 number_of_items );
			}
			result = export_handle_export_appointment(
			          export_handle,
			          item,
			          item_index,
			          export_path,
			          export_path_length,
			          log_handle,
			          error );
			break;

		case LIBPFF_ITEM_TYPE_CONTACT:
			item_type_string = "contact";

			if( export_handle->print_status_information != 0 )
			{
				fprintf(
				 export_handle->notify_stream,
				 "Exporting %s item %d out of %d.\n",
				 item_type_string,
				 item_index + 1,
				 number_of_items );
			}
			result = export_handle_export_contact(
			          export_handle,
			          item,
			          item_index,
			          export_path,
			          export_path_length,
			          log_handle,
			          error );
			break;

		case LIBPFF_ITEM_TYPE_DISTRIBUTION_LIST:
			item_type_string = "distribution list";

			if( export_handle->print_status_information != 0 )
			{
				fprintf(
				 export_handle->notify_stream,
				 "Exporting %s item %d out of %d.\n",
				 item_type_string,
				 item_index + 1,
				 number_of_items );
			}
			result = export_handle_export_distribution_list(
			          export_handle,
			          item,
			          item_index,
			          export_path,
			          export_path_length,
			          log_handle,
			          error );
			break;

		case LIBPFF_ITEM_TYPE_DOCUMENT:
			item_type_string = "document";

			if( export_handle->print_status_information != 0 )
			{
				fprintf(
				 export_handle->notify_stream,
				 "Exporting %s item %d out of %d.\n",
				 item_type_string,
				 item_index + 1,
				 number_of_items );
			}
			result = export_handle_export_document(
			          export_handle,
			          item,
			          item_index,
			          export_path,
			          export_path_length,
			          log_handle,
			          error );
			break;

		case LIBPFF_ITEM_TYPE_CONFLICT_MESSAGE:
		case LIBPFF_ITEM_TYPE_EMAIL:
		case LIBPFF_ITEM_TYPE_EMAIL_SMIME:
			item_type_string = "email";

			if( export_handle->print_status_information != 0 )
			{
				fprintf(
				 export_handle->notify_stream,
				 "Exporting %s item %d out of %d.\n",
				 item_type_string,
				 item_index + 1,
				 number_of_items );
			}
			result = export_handle_export_email(
			          export_handle,
			          item,
			          item_index,
			          export_path,
			          export_path_length,
			          log_handle,
			          error );
			break;

		case LIBPFF_ITEM_TYPE_FOLDER:
			item_type_string = "folder";

			if( export_handle->print_status_information != 0 )
			{
				fprintf(
				 export_handle->notify_stream,
				 "Exporting %s item %d out of %d.\n",
				 item_type_string,
				 item_index + 1,
				 number_of_items );
			}
			result = export_handle_export_folder(
			          export_handle,
			          item,
			          item_index,
			          export_path,
			          export_path_length,
			          log_handle,
			          error );
			break;

		case LIBPFF_ITEM_TYPE_MEETING:
			item_type_string = "meeting";

			if( export_handle->print_status_information != 0 )
			{
				fprintf(
				 export_handle->notify_stream,
				 "Exporting %s item %d out of %d.\n",
				 item_type_string,
				 item_index + 1,
				 number_of_items );
			}
			result = export_handle_export_meeting(
			          export_handle,
			          item,
			          item_index,
			          export_path,
			          export_path_length,
			          log_handle,
			          error );
			break;

		case LIBPFF_ITEM_TYPE_NOTE:
			item_type_string = "note";

			if( export_handle->print_status_information != 0 )
			{
				fprintf(
				 export_handle->notify_stream,
				 "Exporting %s item %d out of %d.\n",
				 item_type_string,
				 item_index + 1,
				 number_of_items );
			}
			result = export_handle_export_note(
			          export_handle,
			          item,
			          item_index,
			          export_path,
			          export_path_length,
			          log_handle,
			          error );
			break;

		case LIBPFF_ITEM_TYPE_RSS_FEED:
			item_type_string = "RSS feed";

			if( export_handle->print_status_information != 0 )
			{
				fprintf(
				 export_handle->notify_stream,
				 "Exporting %s item %d out of %d.\n",
				 item_type_string,
				 item_index + 1,
				 number_of_items );
			}
			result = export_handle_export_rss_feed(
			          export_handle,
			          item,
			          item_index,
			          export_path,
			          export_path_length,
			          log_handle,
			          error );
			break;

		case LIBPFF_ITEM_TYPE_TASK:
			item_type_string = "task";

			if( export_handle->print_status_information != 0 )
			{
				fprintf(
				 export_handle->notify_stream,
				 "Exporting %s item %d out of %d.\n",
				 item_type_string,
				 item_index + 1,
				 number_of_items );
			}
			result = export_handle_export_task(
			          export_handle,
			          item,
			          item_index,
			          export_path,
			          export_path_length,
			          log_handle,
			          error );
			break;

		default:
			result = 0;

			break;
	}
	if( result == -1 )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unable to export %s item: %d out of: %d.\n",
			 function,
			 item_type_string,
			 item_index + 1,
			 number_of_items );
		}
		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );

		log_handle_printf(
		 log_handle,
		 "Unable to export %s item: %d out of: %d.\n",
		 item_type_string,
		 item_index + 1,
		 number_of_items );
	}
	else if( result == 0 )
	{
		if( export_handle->export_mode == EXPORT_MODE_DEBUG )
		{
			if( export_handle_create_default_item_directory(
			     export_handle,
			     item_index,
			     _SYSTEM_STRING( "Item" ),
			     4,
			     export_path,
			     export_path_length,
			     &item_path,
			     &item_path_size,
			     log_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable create item directory.",
				 function );

				goto on_error;
			}
			if( export_handle_export_item_values(
			     export_handle,
			     item,
			     _SYSTEM_STRING( "ItemValues.txt" ),
			     14,
			     item_path,
			     item_path_size - 1,
			     log_handle,
			     error ) != 1 )
			{
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: unable to export values of item: %d out of: %d.\n",
					 function,
					 item_index + 1,
					 number_of_items );
				}
				if( ( error != NULL )
				 && ( *error != NULL ) )
				{
					libcnotify_print_error_backtrace(
					 *error );
				}
				libcerror_error_free(
				 error );

				log_handle_printf(
				 log_handle,
				 "Unable to export values of item: %d out of: %d.\n",
				 item_index + 1,
				 number_of_items );
			}
			memory_free(
			 item_path );
		}
		else
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libpff_message_get_entry_value_utf16_string_size(
			          item,
			          LIBPFF_ENTRY_TYPE_MESSAGE_CLASS,
			          &entry_value_string_size,
			          error );
#else
			result = libpff_message_get_entry_value_utf8_string_size(
			          item,
			          LIBPFF_ENTRY_TYPE_MESSAGE_CLASS,
			          &entry_value_string_size,
			          error );
#endif
			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve message class size.",
				 function );

				goto on_error;
			}
			else if( result != 0 )
			{
				if( ( entry_value_string_size == 0 )
				 || ( entry_value_string_size > (size_t) SSIZE_MAX ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid entry value string size.",
					 function );

					goto on_error;
				}
				entry_value_string = system_string_allocate(
				                      entry_value_string_size );

				if( entry_value_string == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create entry value string.",
					 function );

					goto on_error;
				}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
				result = libpff_message_get_entry_value_utf16_string(
					  item,
				          LIBPFF_ENTRY_TYPE_MESSAGE_CLASS,
					  (uint16_t *) entry_value_string,
					  entry_value_string_size,
					  error );
#else
				result = libpff_message_get_entry_value_utf8_string(
					  item,
				          LIBPFF_ENTRY_TYPE_MESSAGE_CLASS,
					  (uint8_t *) entry_value_string,
					  entry_value_string_size,
					  error );
#endif
				if( result != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve message class.",
					 function );

					goto on_error;
				}
			}
			else
			{
				entry_value_string = _SYSTEM_STRING( "UNKNOWN" );
			}
			fprintf(
			 export_handle->notify_stream,
			 "Skipped item %d out of %d of type: %" PRIs_SYSTEM ".\n",
			 item_index + 1,
			 number_of_items,
			 entry_value_string );

			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: skipped export of item: %d out of: %d of type: %" PRIs_SYSTEM " (0x%02" PRIx8 ").\n",
				 function,
				 item_index + 1,
				 number_of_items,
				 entry_value_string,
				 item_type );
			}
			log_handle_printf(
			 log_handle,
			 "Skipped export of item: %d out of: %d of type: %" PRIs_SYSTEM " (0x%02" PRIx8 ").\n",
			 item_index + 1,
			 number_of_items,
			 entry_value_string,
			 item_type );

			if( result != 0 )
			{
				memory_free(
				 entry_value_string );

				entry_value_string = NULL;
			}
		}
	}
	return( 1 );

on_error:
	if( entry_value_string != NULL )
	{
		memory_free(
		 entry_value_string );
	}
	return( -1 );
}

/* Exports a specific record set to the item file
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_record_set_to_item_file(
     export_handle_t *export_handle,
     libpff_record_set_t *record_set,
     int record_set_index,
     item_file_t *item_file,
     libcerror_error_t **error )
{
	libpff_record_entry_t *record_entry = NULL;
	static char *function               = "export_handle_export_record_set_to_item_file";
	int number_of_record_entries        = 0;
	int record_entry_index              = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( libpff_record_set_get_number_of_entries(
	     record_set,
	     &number_of_record_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries.",
		 function );

		goto on_error;
	}
	for( record_entry_index = 0;
	     record_entry_index < number_of_record_entries;
	     record_entry_index++ )
	{
		if( libpff_record_set_get_entry_by_index(
		     record_set,
		     record_entry_index,
		     &record_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record entry: %d.",
			 function,
			 record_entry_index );

			goto on_error;
		}
		if( export_handle_export_record_entry_to_item_file(
		     export_handle,
		     record_entry,
		     record_set_index,
		     record_entry_index,
		     item_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to export record entry: %d.",
			 function,
			 record_entry_index );

			goto on_error;
		}
		if( libpff_record_entry_free(
		     &record_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free record entry: %d.",
			 function,
			 record_entry_index );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( record_entry != NULL )
	{
		libpff_record_entry_free(
		 &record_entry,
		 NULL );
	}
	return( -1 );
}

/* Exports a specific record entry to the item file
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_record_entry_to_item_file(
     export_handle_t *export_handle,
     libpff_record_entry_t *record_entry,
     int record_set_index,
     int record_entry_index,
     item_file_t *item_file,
     libcerror_error_t **error )
{
	libpff_name_to_id_map_entry_t *name_to_id_map_entry = NULL;
	system_character_t *name_to_id_map_entry_string     = NULL;
	uint8_t *value_data                                 = NULL;
	static char *function                               = "export_handle_export_record_entry_to_item_file";
	size_t name_to_id_map_entry_string_size             = 0;
	size_t value_data_size                              = 0;
	uint32_t entry_type                                 = 0;
	uint32_t name_to_id_map_entry_number                = 0;
	uint32_t value_type                                 = LIBPFF_VALUE_TYPE_UNSPECIFIED;
	uint8_t name_to_id_map_entry_type                   = 0;
	int result                                          = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libpff_multi_value_t *multi_value                   = NULL;
#endif

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( item_file_write_value_integer_32bit_as_decimal(
	     item_file,
	     _SYSTEM_STRING( "Set:\t\t\t\t" ),
	     (uint32_t) record_set_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write 32-bit integer value.",
		 function );

		goto on_error;
	}
	if( item_file_write_value_integer_32bit_as_decimal(
	     item_file,
	     _SYSTEM_STRING( "Entry:\t\t\t\t" ),
	     (uint32_t) record_entry_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write 32-bit integer value.",
		 function );

		goto on_error;
	}
	if( libpff_record_entry_get_entry_type(
	     record_entry,
	     &entry_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry type.",
		 function );

		goto on_error;
	}
	if( item_file_write_value_integer_32bit_as_hexadecimal(
	     item_file,
	     _SYSTEM_STRING( "Entry type:\t\t\t" ),
	     entry_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write 32-bit integer value.",
		 function );

		goto on_error;
	}
	if( libpff_record_entry_get_value_type(
	     record_entry,
	     &value_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value type.",
		 function );

		goto on_error;
	}
	if( item_file_write_value_integer_32bit_as_hexadecimal(
	     item_file,
	     _SYSTEM_STRING( "Value type:\t\t\t" ),
	     value_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write 32-bit integer value.",
		 function );

		goto on_error;
	}
	result = libpff_record_entry_get_name_to_id_map_entry(
	          record_entry,
	          &name_to_id_map_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name to ide map entry.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libpff_name_to_id_map_entry_get_type(
		     name_to_id_map_entry,
		     &name_to_id_map_entry_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve name to identifier map entry type.",
			 function );

			goto on_error;
		}
		if( name_to_id_map_entry_type == LIBPFF_NAME_TO_ID_MAP_ENTRY_TYPE_NUMERIC )
		{
			if( libpff_name_to_id_map_entry_get_number(
			     name_to_id_map_entry,
			     &name_to_id_map_entry_number,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve name to identifier map entry number.",
				 function );

				goto on_error;
			}
			if( item_file_write_value_integer_32bit_as_hexadecimal(
			     item_file,
			     _SYSTEM_STRING( "Maps to entry type:\t\t" ),
			     name_to_id_map_entry_number,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write 32-bit integer value.",
				 function );

				goto on_error;
			}
		}
		else if( name_to_id_map_entry_type == LIBPFF_NAME_TO_ID_MAP_ENTRY_TYPE_STRING )
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libpff_name_to_id_map_entry_get_utf16_string_size(
				  name_to_id_map_entry,
				  &name_to_id_map_entry_string_size,
				  error );
#else
			result = libpff_name_to_id_map_entry_get_utf8_string_size(
				  name_to_id_map_entry,
				  &name_to_id_map_entry_string_size,
				  error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve name to identifier map entry string size.",
				 function );

				goto on_error;
			}
			name_to_id_map_entry_string = system_string_allocate(
			                               name_to_id_map_entry_string_size );

			if( name_to_id_map_entry_string == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create name to identifier map entry string.",
				 function );

				goto on_error;
			}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libpff_name_to_id_map_entry_get_utf16_string(
				  name_to_id_map_entry,
				  (uint16_t *) name_to_id_map_entry_string,
				  name_to_id_map_entry_string_size,
				  error );
#else
			result = libpff_name_to_id_map_entry_get_utf8_string(
				  name_to_id_map_entry,
				  (uint8_t *) name_to_id_map_entry_string,
				  name_to_id_map_entry_string_size,
				  error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve name to identifier map entry string.",
				 function );

				goto on_error;
			}
			if( item_file_write_value_string(
			     item_file,
			     _SYSTEM_STRING( "Maps to entry:\t\t\t" ),
			     name_to_id_map_entry_string,
			     name_to_id_map_entry_string_size - 1,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write string.",
				 function );

				goto on_error;
			}
			memory_free(
			 name_to_id_map_entry_string );

			name_to_id_map_entry_string = NULL;
		}
	}
	if( libpff_record_entry_get_data_size(
	     record_entry,
	     &value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data size.",
		 function );

		goto on_error;
	}
	if( value_data_size == 0 )
	{
		if( item_file_write_new_line(
		     item_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write new line.",
			 function );

			goto on_error;
		}
	}
	else
	{
		value_data = (uint8_t *) memory_allocate(
		                          sizeof( uint8_t ) * value_data_size );

		if( value_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value data.",
			 function );

			goto on_error;
		}
		if( libpff_record_entry_get_data(
		     record_entry,
		     value_data,
		     value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value data.",
			 function );

			goto on_error;
		}
		if( item_file_write_value_description(
		     item_file,
		     _SYSTEM_STRING( "Value:" ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write string.",
			 function );

			goto on_error;
		}
		if( item_file_write_buffer_as_hexdump(
		     item_file,
		     value_data,
		     value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write buffer.",
			 function );

			goto on_error;
		}
		memory_free(
		 value_data );

		value_data = NULL;
	}
#if defined( HAVE_DEBUG_OUTPUT )
/* TODO merge this with "normal" value export */
	if( ( value_type & LIBPFF_VALUE_TYPE_MULTI_VALUE_FLAG ) != 0 )
	{
		if( libpff_record_entry_get_multi_value(
		     record_entry,
		     &multi_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry multi value.",
			 function );

			goto on_error;
		}
/* TODO do something useful with the multi value */
		if( libpff_multi_value_free(
		     &multi_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free multi value.",
			 function );

			goto on_error;
		}
	}
#endif
	return( 1 );

on_error:
	if( value_data != NULL )
	{
		memory_free(
		 value_data );
	}
	if( name_to_id_map_entry_string != NULL )
	{
		memory_free(
		 name_to_id_map_entry_string );
	}
	return( -1 );
}

/* Writes a specific record set value to the item file
 * Returns 1 if successful or -1 on error
 */
void export_handle_write_record_set_value_to_item_file(
      item_file_t *item_file,
      const system_character_t *description,
      libpff_record_set_t *record_set,
      uint32_t entry_type,
      uint32_t value_type,
      uint32_t format_flags,
      int (*write_to_item_file_function)(
             item_file_t *item_file,
             libpff_record_entry_t *record_entry,
             libcerror_error_t **error ) )
{
	libcerror_error_t *error = NULL;
	static char *function    = "export_handle_write_record_set_value_to_item_file";

	if( item_file_write_record_set_value(
	     item_file,
	     description,
	     record_set,
	     entry_type,
	     value_type,
	     format_flags,
	     write_to_item_file_function,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write record set value: 0x%04" PRIx32 " 0x%04" PRIx32 ".",
		 function,
		 entry_type,
		 value_type );

#if defined( HAVE_DEBUG_OUTPUT )
		if( error != NULL )
		{
			libcnotify_print_error_backtrace(
			 error );
		}
#endif
		libcerror_error_free(
		 &error );
	}
}

/* Exports the item values
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_item_values(
     export_handle_t *export_handle,
     libpff_item_t *item,
     const system_character_t *item_values_filename,
     size_t item_values_filename_length,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	libpff_record_set_t *record_set = NULL;
	item_file_t *item_file          = NULL;
	static char *function           = "export_handle_export_item_values";
	uint32_t number_of_entries      = 0;
	int number_of_record_sets       = 0;
	int record_set_index            = 0;
	int result                      = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( item_values_filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item value filename.",
		 function );

		return( -1 );
	}
	result = export_handle_create_item_file(
	          export_handle,
	          export_path,
	          export_path_length,
                  item_values_filename,
                  item_values_filename_length,
	          &item_file,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create item file.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		log_handle_printf(
		 log_handle,
		 "Skipping item values file: %" PRIs_SYSTEM " it already exists.\n",
		 item_values_filename );

		return( 1 );
	}
	if( libpff_item_get_number_of_record_sets(
	     item,
	     &number_of_record_sets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of record sets.",
		 function );

		goto on_error;
	}
/* TODO deprecate? */
	if( libpff_item_get_number_of_entries(
	     item,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries.",
		 function );

		goto on_error;
	}
	if( item_file_write_value_integer_32bit_as_decimal(
	     item_file,
	     _SYSTEM_STRING( "Number of sets:\t\t\t" ),
	     (uint32_t) number_of_record_sets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write 32-bit integer value.",
		 function );

		goto on_error;
	}
	if( item_file_write_value_integer_32bit_as_decimal(
	     item_file,
	     _SYSTEM_STRING( "Number of entries per set:\t" ),
	     number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write 32-bit integer value.",
		 function );

		goto on_error;
	}
	if( item_file_write_new_line(
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write new line.",
		 function );

		goto on_error;
	}
	for( record_set_index = 0;
	     record_set_index < number_of_record_sets;
	     record_set_index++ )
	{
		if( libpff_item_get_record_set_by_index(
		     item,
		     record_set_index,
		     &record_set,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of record set: %d.",
			 function,
			 record_set_index );

			goto on_error;
		}
		if( export_handle_export_record_set_to_item_file(
		     export_handle,
		     record_set,
		     record_set_index,
		     item_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to export record set: %d.",
			 function,
			 record_set_index );

			goto on_error;
		}
		if( libpff_record_set_free(
		     &record_set,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free record set: %d.",
			 function,
			 record_set_index );

			goto on_error;
		}
	}
	if( item_file_close(
	     item_file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close item file.",
		 function );

		goto on_error;
	}
	if( item_file_free(
	     &item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free item file.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( record_set != NULL )
	{
		libpff_record_set_free(
		 &record_set,
		 NULL );
	}
	if( item_file != NULL )
	{
		item_file_free(
		 &item_file,
		 NULL );
	}
	return( -1 );
}

/* Exports the sub items
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_sub_items(
     export_handle_t *export_handle,
     libpff_item_t *item,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	libpff_item_t *sub_item = NULL;
	static char *function   = "export_handle_export_sub_items";
	int number_of_sub_items = 0;
	int sub_item_iterator   = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( libpff_item_get_number_of_sub_items(
	     item,
	     &number_of_sub_items,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub items.",
		 function );

		return( -1 );
	}
	for( sub_item_iterator = 0;
	     sub_item_iterator < number_of_sub_items;
	     sub_item_iterator++ )
	{
		if( export_handle->abort != 0 )
		{
			return( -1 );
		}
		if( libpff_item_get_sub_item(
		     item,
		     sub_item_iterator,
		     &sub_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub item: %d.",
			 function,
			 sub_item_iterator + 1 );

			return( -1 );
		}
		if( export_handle_export_item(
		     export_handle,
		     sub_item,
		     sub_item_iterator,
		     number_of_sub_items,
		     export_path,
		     export_path_length,
		     log_handle,
		     error ) != 1 )
		{
			fprintf(
			 export_handle->notify_stream,
			 "Unable to export sub item %d out of %d.\n",
			 sub_item_iterator + 1,
			 number_of_sub_items );

			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export sub item: %d.\n",
				 function,
				 sub_item_iterator + 1 );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export sub item: %d.\n",
			 sub_item_iterator + 1 );
		}
		if( libpff_item_free(
		     &sub_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free sub item: %d.",
			 function,
			 sub_item_iterator + 1 );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves a record entry matching the entry and value type
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int export_handle_item_get_record_entry_by_type(
     export_handle_t *export_handle,
     libpff_item_t *item,
     int record_set_index,
     uint32_t entry_type,
     uint32_t value_type,
     libpff_record_set_t **record_set,
     libpff_record_entry_t **record_entry,
     uint8_t flags,
     libcerror_error_t **error )
{
	static char *function = "export_handle_item_get_record_entry_by_type";
	int result            = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( record_set == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record set.",
		 function );

		return( -1 );
	}
	if( libpff_item_get_record_set_by_index(
	     item,
	     record_set_index,
	     record_set,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record set: %d from item.",
		 function,
		 record_set_index );

		return( -1 );
	}
	result = libpff_record_set_get_entry_by_type(
	          *record_set,
	          entry_type,
	          value_type,
	          record_entry,
	          flags,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record entry from record set: %d.",
		 function,
		 record_set_index );

		return( -1 );
	}
	return( result );
}

/* Retrieves a 32-bit value matching the entry type
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int export_handle_record_set_get_value_32bit_by_type(
     export_handle_t *export_handle,
     libpff_record_set_t *record_set,
     uint32_t entry_type,
     uint32_t *value_32bit,
     libcerror_error_t **error )
{
	libpff_record_entry_t *record_entry = NULL;
	static char *function               = "export_handle_record_set_get_value_32bit";
	int result                          = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	result = libpff_record_set_get_entry_by_type(
	          record_set,
	          entry_type,
	          LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED,
	          &record_entry,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record entry: 0x%04" PRIx32 " 0x%04x from record set.",
		 function,
		 entry_type,
		 LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libpff_record_entry_get_data_as_32bit_integer(
		     record_entry,
		     value_32bit,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve 32-bit integer value.",
			 function );

			goto on_error;
		}
		if( libpff_record_entry_free(
		     &record_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free record entry.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( record_entry != NULL )
	{
		libpff_record_entry_free(
		 &record_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a 32-bit value matching the entry type
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int export_handle_item_get_value_32bit_by_type(
     export_handle_t *export_handle,
     libpff_item_t *item,
     int record_set_index,
     uint32_t entry_type,
     uint32_t *value_32bit,
     libcerror_error_t **error )
{
	libpff_record_entry_t *record_entry = NULL;
	libpff_record_set_t *record_set     = NULL;
	static char *function               = "export_handle_item_get_value_32bit";
	int result                          = 0;

	result = export_handle_item_get_record_entry_by_type(
	          export_handle,
	          item,
	          record_set_index,
	          entry_type,
	          LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED,
	          &record_set,
	          &record_entry,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record entry: 0x%04" PRIx32 " 0x%04x from record set: %d.",
		 function,
		 entry_type,
		 LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED,
		 record_set_index );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libpff_record_entry_get_data_as_32bit_integer(
		     record_entry,
		     value_32bit,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve 32-bit integer value.",
			 function );

			goto on_error;
		}
		if( libpff_record_entry_free(
		     &record_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free record entry.",
			 function );

			goto on_error;
		}
	}
	if( libpff_record_set_free(
	     &record_set,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free record set.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( record_entry != NULL )
	{
		libpff_record_entry_free(
		 &record_entry,
		 NULL );
	}
	if( record_set != NULL )
	{
		libpff_record_set_free(
		 &record_set,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the size of a string value matching the entry type
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int export_handle_item_get_value_string_size_by_type(
     export_handle_t *export_handle,
     libpff_item_t *item,
     int record_set_index,
     uint32_t entry_type,
     size_t *value_string_size,
     libcerror_error_t **error )
{
	libpff_record_entry_t *record_entry = NULL;
	libpff_record_set_t *record_set     = NULL;
	static char *function               = "export_handle_item_get_value_string_size_by_type";
	int result                          = 0;

	if( value_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value string size.",
		 function );

		return( -1 );
	}
	result = export_handle_item_get_record_entry_by_type(
	          export_handle,
	          item,
	          record_set_index,
	          entry_type,
	          0,
	          &record_set,
	          &record_entry,
	          LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record entry: 0x%04" PRIx32 " 0x%04x or 0x%04" PRIx32 " 0x%04x from record set: %d.",
		 function,
		 entry_type,
		 LIBPFF_VALUE_TYPE_STRING_ASCII,
		 entry_type,
		 LIBPFF_VALUE_TYPE_STRING_UNICODE,
		 record_set_index );

		goto on_error;
	}
	else if( result != 0 )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libpff_record_entry_get_data_as_utf16_string_size(
		     record_entry,
		     value_string_size,
		     error ) != 1 )
#else
		if( libpff_record_entry_get_data_as_utf8_string_size(
		     record_entry,
		     value_string_size,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string value size.",
			 function );

			goto on_error;
		}
		if( libpff_record_entry_free(
		     &record_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free record entry.",
			 function );

			goto on_error;
		}
	}
	if( libpff_record_set_free(
	     &record_set,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free record set.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( record_entry != NULL )
	{
		libpff_record_entry_free(
		 &record_entry,
		 NULL );
	}
	if( record_set != NULL )
	{
		libpff_record_set_free(
		 &record_set,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the size of a string value matching the entry type
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int export_handle_item_get_value_string_by_type(
     export_handle_t *export_handle,
     libpff_item_t *item,
     int record_set_index,
     uint32_t entry_type,
     system_character_t *value_string,
     size_t value_string_size,
     libcerror_error_t **error )
{
	libpff_record_entry_t *record_entry = NULL;
	libpff_record_set_t *record_set     = NULL;
	static char *function               = "export_handle_item_get_value_string_by_type";
	int result                          = 0;

	result = export_handle_item_get_record_entry_by_type(
	          export_handle,
	          item,
	          record_set_index,
	          entry_type,
	          0,
	          &record_set,
	          &record_entry,
	          LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record entry: 0x%04" PRIx32 " 0x%04x or 0x%04" PRIx32 " 0x%04x from record set: %d.",
		 function,
		 entry_type,
		 LIBPFF_VALUE_TYPE_STRING_ASCII,
		 entry_type,
		 LIBPFF_VALUE_TYPE_STRING_UNICODE,
		 record_set_index );

		goto on_error;
	}
	else if( result != 0 )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libpff_record_entry_get_data_as_utf16_string(
		     record_entry,
		     (uint16_t *) value_string,
		     value_string_size,
		     error ) != 1 )
#else
		if( libpff_record_entry_get_data_as_utf8_string(
		     record_entry,
		     (uint8_t *) value_string,
		     value_string_size,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string value.",
			 function );

			goto on_error;
		}
		if( libpff_record_entry_free(
		     &record_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free record entry.",
			 function );

			goto on_error;
		}
	}
	if( libpff_record_set_free(
	     &record_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free record set.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( record_entry != NULL )
	{
		libpff_record_entry_free(
		 &record_entry,
		 NULL );
	}
	if( record_set != NULL )
	{
		libpff_record_set_free(
		 &record_set,
		 NULL );
	}
	return( -1 );
}

/* Creates a string value matching the entry type
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int export_handle_item_create_value_string_by_type(
     export_handle_t *export_handle,
     libpff_item_t *item,
     int record_set_index,
     uint32_t entry_type,
     system_character_t **value_string,
     size_t *value_string_size,
     libcerror_error_t **error )
{
	libpff_record_entry_t *record_entry = NULL;
	libpff_record_set_t *record_set     = NULL;
	static char *function               = "export_handle_item_create_value_string_by_type";
	int result                          = 0;

	if( value_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value string.",
		 function );

		return( -1 );
	}
	if( *value_string != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid value string value already set.",
		 function );

		return( -1 );
	}
	if( value_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value string size.",
		 function );

		return( -1 );
	}
	result = export_handle_item_get_record_entry_by_type(
	          export_handle,
	          item,
	          record_set_index,
	          entry_type,
	          0,
	          &record_set,
	          &record_entry,
	          LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record entry: 0x%04" PRIx32 " 0x%04x or 0x%04" PRIx32 " 0x%04x from record set: %d.",
		 function,
		 entry_type,
		 LIBPFF_VALUE_TYPE_STRING_ASCII,
		 entry_type,
		 LIBPFF_VALUE_TYPE_STRING_UNICODE,
		 record_set_index );

		goto on_error;
	}
	else if( result != 0 )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libpff_record_entry_get_data_as_utf16_string_size(
		     record_entry,
		     value_string_size,
		     error ) != 1 )
#else
		if( libpff_record_entry_get_data_as_utf8_string_size(
		     record_entry,
		     value_string_size,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string value size.",
			 function );

			goto on_error;
		}
		if( *value_string_size > (size_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid value string size value exceeds maximum.",
			 function );

			goto on_error;
		}
		*value_string = system_string_allocate(
		                 *value_string_size );

		if( *value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libpff_record_entry_get_data_as_utf16_string(
		     record_entry,
		     (uint16_t *) *value_string,
		     *value_string_size,
		     error ) != 1 )
#else
		if( libpff_record_entry_get_data_as_utf8_string(
		     record_entry,
		     (uint8_t *) *value_string,
		     *value_string_size,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string size value.",
			 function );

			goto on_error;
		}
		if( libpff_record_entry_free(
		     &record_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free record entry.",
			 function );

			goto on_error;
		}
	}
	if( libpff_record_set_free(
	     &record_set,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free record set.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( record_entry != NULL )
	{
		libpff_record_entry_free(
		 &record_entry,
		 NULL );
	}
	if( record_set != NULL )
	{
		libpff_record_set_free(
		 &record_set,
		 NULL );
	}
	if( *value_string != NULL )
	{
		memory_free(
		 *value_string );

		*value_string = NULL;
	}
	*value_string_size = 0;

	return( -1 );
}

/* Exports the item to an item file
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_item_value_to_item_file(
     export_handle_t *export_handle,
     item_file_t *item_file,
     const system_character_t *description,
     libpff_item_t *item,
     int record_set_index,
     mapi_property_definitions_t *property_definitions,
     int number_of_property_definitions,
     libcerror_error_t **error )
{
	libpff_record_set_t *record_set                  = NULL;
	mapi_property_definitions_t *property_definition = NULL;
	static char *function                            = "export_handle_export_item_values_to_item_file";
	int property_definition_index                    = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( libpff_item_get_record_set_by_index(
	     item,
	     record_set_index,
	     &record_set,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record set: %d from item.",
		 function,
		 record_set_index );

		goto on_error;
	}
	if( item_file_write_value_description(
	     item_file,
	     description,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write description.",
		 function );

		goto on_error;
	}
	for( property_definition_index = 0;
	     property_definition_index < number_of_property_definitions;
	     property_definition_index++ )
	{
		property_definition = &( property_definitions[ property_definition_index ] );

		export_handle_write_record_set_value_to_item_file(
		 item_file,
		 property_definition->description,
		 record_set,
		 property_definition->entry_type,
		 property_definition->value_type,
		 property_definition->format_flags,
		 property_definition->write_to_item_file_function );
	}
	if( libpff_record_set_free(
	     &record_set,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free record set: %s.",
		 function,
		 record_set_index );

		goto on_error;
	}
	return( 1 );

on_error:
	if( record_set != NULL )
	{
		libpff_record_set_free(
		 &record_set,
		 NULL );
	}
	return( -1 );
}

/* Exports the Outlook message header
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_message_header(
     export_handle_t *export_handle,
     libpff_item_t *message,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	item_file_t *item_file = NULL;
	static char *function  = "export_handle_export_message_header";
	int result             = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	result = export_handle_create_item_file(
	          export_handle,
	          export_path,
	          export_path_length,
	          _SYSTEM_STRING( "OutlookHeaders.txt" ),
	          18,
	          &item_file,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create item file.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		log_handle_printf(
		 log_handle,
		 "Skipping message header file it already exists.\n" );

		return( 1 );
	}
	if( export_handle_export_message_header_to_item_file(
	     export_handle,
	     message,
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to export message header.",
		 function );

		goto on_error;
	}
	if( item_file_close(
	     item_file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close item file.",
		 function );

		goto on_error;
	}
	if( item_file_free(
	     &item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free item file.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( item_file != NULL )
	{
		item_file_free(
		 &item_file,
		 NULL );
	}
	return( -1 );
}

/* Exports the message flags to an item file
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_message_flags_to_item_file(
     item_file_t *item_file,
     libpff_record_entry_t *record_entry,
     libcerror_error_t **error )
{
	system_character_t *value_string = NULL;
	static char *function            = "export_handle_export_message_flags_to_item_file";
	size_t value_string_length       = 0;
	uint32_t value_32bit             = 0;

	if( libpff_record_entry_get_data_as_32bit_integer(
	     record_entry,
	     &value_32bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve 32-bit integer value.",
		 function );

		return( -1 );
	}
	if( item_file_write_integer_32bit_as_hexadecimal(
	     item_file,
	     value_32bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write 32-bit integer.",
		 function );

		return( -1 );
	}
	if( item_file_write_string(
	     item_file,
	     _SYSTEM_STRING( " (" ),
	     2,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write string.",
		 function );

		return( -1 );
	}
	if( ( value_32bit & LIBPFF_MESSAGE_FLAG_READ ) != 0 )
	{
		value_string        = _SYSTEM_STRING( "Read" );
		value_string_length = 4;

		value_32bit &= ~( LIBPFF_MESSAGE_FLAG_READ );
	}
	else
	{
		value_string        = _SYSTEM_STRING( "Unread" );
		value_string_length = 6;
	}
	if( item_file_write_string(
	     item_file,
	     value_string,
	     value_string_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write string.",
		 function );

		return( -1 );
	}
	if( value_32bit != 0 )
	{
		if( item_file_write_string(
		     item_file,
		     _SYSTEM_STRING( ", " ),
		     2,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write string.",
			 function );

			return( -1 );
		}
	}
	if( ( value_32bit & LIBPFF_MESSAGE_FLAG_UNMODIFIED ) != 0 )
	{
		if( item_file_write_string(
		     item_file,
		     _SYSTEM_STRING( "Unmodified" ),
		     10,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write string.",
			 function );

			return( -1 );
		}
		value_32bit &= ~( LIBPFF_MESSAGE_FLAG_UNMODIFIED );

		if( value_32bit != 0 )
		{
			if( item_file_write_string(
			     item_file,
			     _SYSTEM_STRING( ", " ),
			     2,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write string.",
				 function );

				return( -1 );
			}
		}
	}
	if( ( value_32bit & LIBPFF_MESSAGE_FLAG_SUBMIT ) != 0 )
	{
		if( item_file_write_string(
		     item_file,
		     _SYSTEM_STRING( "Submit" ),
		     6,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write string.",
			 function );

			return( -1 );
		}
		value_32bit &= ~( LIBPFF_MESSAGE_FLAG_SUBMIT );

		if( value_32bit != 0 )
		{
			if( item_file_write_string(
			     item_file,
			     _SYSTEM_STRING( ", " ),
			     2,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write string.",
				 function );

				return( -1 );
			}
		}
	}
	if( ( value_32bit & LIBPFF_MESSAGE_FLAG_UNSENT ) != 0 )
	{
		if( item_file_write_string(
		     item_file,
		     _SYSTEM_STRING( "Unsent" ),
		     6,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write string.",
			 function );

			return( -1 );
		}
		value_32bit &= ~( LIBPFF_MESSAGE_FLAG_UNSENT );

		if( value_32bit != 0 )
		{
			if( item_file_write_string(
			     item_file,
			     _SYSTEM_STRING( ", " ),
			     2,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write string.",
				 function );

				return( -1 );
			}
		}
	}
	if( ( value_32bit & LIBPFF_MESSAGE_FLAG_HAS_ATTACHMENTS ) != 0 )
	{
		if( item_file_write_string(
		     item_file,
		     _SYSTEM_STRING( "Has attachments" ),
		     15,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write string.",
			 function );

			return( -1 );
		}
		value_32bit &= ~( LIBPFF_MESSAGE_FLAG_HAS_ATTACHMENTS );

		if( value_32bit != 0 )
		{
			if( item_file_write_string(
			     item_file,
			     _SYSTEM_STRING( ", " ),
			     2,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write string.",
				 function );

				return( -1 );
			}
		}
	}
	if( ( value_32bit & LIBPFF_MESSAGE_FLAG_FROM_ME ) != 0 )
	{
		if( item_file_write_string(
		     item_file,
		     _SYSTEM_STRING( "From me" ),
		     7,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write string.",
			 function );

			return( -1 );
		}
		value_32bit &= ~( LIBPFF_MESSAGE_FLAG_FROM_ME );

		if( value_32bit != 0 )
		{
			if( item_file_write_string(
			     item_file,
			     _SYSTEM_STRING( ", " ),
			     2,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write string.",
				 function );

				return( -1 );
			}
		}
	}
	if( ( value_32bit & LIBPFF_MESSAGE_FLAG_ASSOCIATED ) != 0 )
	{
		if( item_file_write_string(
		     item_file,
		     _SYSTEM_STRING( "Associated" ),
		     10,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write string.",
			 function );

			return( -1 );
		}
		value_32bit &= ~( LIBPFF_MESSAGE_FLAG_ASSOCIATED );

		if( value_32bit != 0 )
		{
			if( item_file_write_string(
			     item_file,
			     _SYSTEM_STRING( ", " ),
			     2,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write string.",
				 function );

				return( -1 );
			}
		}
	}
	if( ( value_32bit & LIBPFF_MESSAGE_FLAG_RESEND ) != 0 )
	{
		if( item_file_write_string(
		     item_file,
		     _SYSTEM_STRING( "Resend" ),
		     6,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write string.",
			 function );

			return( -1 );
		}
		value_32bit &= ~( LIBPFF_MESSAGE_FLAG_RESEND );

		if( value_32bit != 0 )
		{
			if( item_file_write_string(
			     item_file,
			     _SYSTEM_STRING( ", " ),
			     2,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write string.",
				 function );

				return( -1 );
			}
		}
	}
	if( ( value_32bit & LIBPFF_MESSAGE_FLAG_RN_PENDING ) != 0 )
	{
		if( item_file_write_string(
		     item_file,
		     _SYSTEM_STRING( "RN pending" ),
		     10,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write string.",
			 function );

			return( -1 );
		}
		value_32bit &= ~( LIBPFF_MESSAGE_FLAG_RN_PENDING );

		if( value_32bit != 0 )
		{
			if( item_file_write_string(
			     item_file,
			     _SYSTEM_STRING( ", " ),
			     2,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write string.",
				 function );

				return( -1 );
			}
		}
	}
	if( ( value_32bit & LIBPFF_MESSAGE_FLAG_NRN_PENDING ) != 0 )
	{
		if( item_file_write_string(
		     item_file,
		     _SYSTEM_STRING( "NRN pending" ),
		     11,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write string.",
			 function );

			return( -1 );
		}
		value_32bit &= ~( LIBPFF_MESSAGE_FLAG_NRN_PENDING );

		if( value_32bit != 0 )
		{
			if( item_file_write_string(
			     item_file,
			     _SYSTEM_STRING( ", " ),
			     2,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write string.",
				 function );

				return( -1 );
			}
		}
	}
	if( value_32bit != 0 )
	{
		if( item_file_write_string(
		     item_file,
		     _SYSTEM_STRING( "Unknown: " ),
		     9,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write string.",
			 function );

			return( -1 );
		}
		if( item_file_write_integer_32bit_as_hexadecimal(
		     item_file,
		     value_32bit,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write 32-bit integer.",
			 function );

			return( -1 );
		}
	}
	if( item_file_write_string(
	     item_file,
	     _SYSTEM_STRING( ")" ),
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write string.",
		 function );

		return( -1 );
	}
	if( item_file_write_new_line(
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write new line.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Exports the message importance to an item file
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_message_importance_to_item_file(
     item_file_t *item_file,
     libpff_record_entry_t *record_entry,
     libcerror_error_t **error )
{
	system_character_t *value_string = NULL;
	static char *function            = "export_handle_export_message_importance_to_item_file";
	size_t value_string_length       = 0;
	uint32_t value_32bit             = 0;
	int unknown_value                = 0;

	if( libpff_record_entry_get_data_as_32bit_integer(
	     record_entry,
	     &value_32bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve 32-bit integer value.",
		 function );

		return( -1 );
	}
	if( value_32bit == (uint32_t) LIBPFF_MESSAGE_IMPORTANCE_TYPE_LOW )
	{
		value_string        = _SYSTEM_STRING( "Low" );
		value_string_length = 3;
	}
	else if( value_32bit == (uint32_t) LIBPFF_MESSAGE_IMPORTANCE_TYPE_NORMAL )
	{
		value_string        = _SYSTEM_STRING( "Normal" );
		value_string_length = 6;
	}
	else if( value_32bit == (uint32_t) LIBPFF_MESSAGE_IMPORTANCE_TYPE_HIGH )
	{
		value_string        = _SYSTEM_STRING( "High" );
		value_string_length = 4;
	}
	else
	{
		value_string        = _SYSTEM_STRING( "Unknown: " );
		value_string_length = 9;
		unknown_value       = 1;
	}
	if( item_file_write_string(
	     item_file,
	     value_string,
	     value_string_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write string.",
		 function );

		return( -1 );
	}
	if( unknown_value != 0 )
	{
		if( item_file_write_integer_32bit_as_hexadecimal(
		     item_file,
		     value_32bit,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write 32-bit integer.",
			 function );

			return( -1 );
		}
	}
	if( item_file_write_new_line(
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write new line.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Exports the message priority to an item file
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_message_priority_to_item_file(
     item_file_t *item_file,
     libpff_record_entry_t *record_entry,
     libcerror_error_t **error )
{
	system_character_t *value_string = NULL;
	static char *function            = "export_handle_export_message_priority_to_item_file";
	size_t value_string_length       = 0;
	uint32_t value_32bit             = 0;
	int unknown_value                = 0;

	if( libpff_record_entry_get_data_as_32bit_integer(
	     record_entry,
	     &value_32bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve 32-bit integer value.",
		 function );

		return( -1 );
	}
	if( value_32bit == (uint32_t) LIBPFF_MESSAGE_PRIORITY_TYPE_NON_URGENT )
	{
		value_string        = _SYSTEM_STRING( "Non Urgent" );
		value_string_length = 10;
	}
	else if( value_32bit == (uint32_t) LIBPFF_MESSAGE_PRIORITY_TYPE_NORMAL )
	{
		value_string        = _SYSTEM_STRING( "Normal" );
		value_string_length = 6;
	}
	else if( value_32bit == (uint32_t) LIBPFF_MESSAGE_PRIORITY_TYPE_URGENT )
	{
		value_string        = _SYSTEM_STRING( "Urgent" );
		value_string_length = 6;
	}
	else
	{
		value_string        = _SYSTEM_STRING( "Unknown: " );
		value_string_length = 9;
		unknown_value       = 1;
	}
	if( item_file_write_string(
	     item_file,
	     value_string,
	     value_string_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write string.",
		 function );

		return( -1 );
	}
	if( unknown_value != 0 )
	{
		if( item_file_write_integer_32bit_as_hexadecimal(
		     item_file,
		     value_32bit,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write 32-bit integer.",
			 function );

			return( -1 );
		}
	}
	if( item_file_write_new_line(
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write new line.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Exports the message sensitivity to an item file
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_message_sensitivity_to_item_file(
     item_file_t *item_file,
     libpff_record_entry_t *record_entry,
     libcerror_error_t **error )
{
	system_character_t *value_string = NULL;
	static char *function            = "export_handle_export_message_sensitivity_to_item_file";
	size_t value_string_length       = 0;
	uint32_t value_32bit             = 0;
	int unknown_value                = 0;

	if( libpff_record_entry_get_data_as_32bit_integer(
	     record_entry,
	     &value_32bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve 32-bit integer value.",
		 function );

		return( -1 );
	}
	if( value_32bit == (uint32_t) LIBPFF_MESSAGE_SENSITIVITY_TYPE_NONE )
	{
		value_string        = _SYSTEM_STRING( "None" );
		value_string_length = 4;
	}
	else if( value_32bit == (uint32_t) LIBPFF_MESSAGE_SENSITIVITY_TYPE_PERSONAL )
	{
		value_string        = _SYSTEM_STRING( "Personal" );
		value_string_length = 8;
	}
	else if( value_32bit == (uint32_t) LIBPFF_MESSAGE_SENSITIVITY_TYPE_PRIVATE )
	{
		value_string        = _SYSTEM_STRING( "Private" );
		value_string_length = 7;
	}
	else if( value_32bit == (uint32_t) LIBPFF_MESSAGE_SENSITIVITY_TYPE_CONFIDENTIAL )
	{
		value_string        = _SYSTEM_STRING( "Confidential" );
		value_string_length = 12;
	}
	else
	{
		value_string        = _SYSTEM_STRING( "Unknown: " );
		value_string_length = 9;
		unknown_value       = 1;
	}
	if( item_file_write_string(
	     item_file,
	     value_string,
	     value_string_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write string.",
		 function );

		return( -1 );
	}
	if( unknown_value != 0 )
	{
		if( item_file_write_integer_32bit_as_hexadecimal(
		     item_file,
		     value_32bit,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write 32-bit integer.",
			 function );

			return( -1 );
		}
	}
	if( item_file_write_new_line(
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write new line.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Exports the message status to an item file
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_message_status_to_item_file(
     item_file_t *item_file,
     libpff_record_entry_t *record_entry,
     libcerror_error_t **error )
{
	static char *function = "export_handle_export_message_status_to_item_file";
	uint32_t value_32bit  = 0;

	if( libpff_record_entry_get_data_as_32bit_integer(
	     record_entry,
	     &value_32bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve 32-bit integer value.",
		 function );

		return( -1 );
	}
	if( item_file_write_integer_32bit_as_hexadecimal(
	     item_file,
	     value_32bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write 32-bit integer.",
		 function );

		return( -1 );
	}
	if( item_file_write_string(
	     item_file,
	     _SYSTEM_STRING( " (" ),
	     2,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write string.",
		 function );

		return( -1 );
	}
	if( ( value_32bit & LIBPFF_MESSAGE_STATUS_FLAG_HIGHLIGHTED ) != 0 )
	{
		if( item_file_write_string(
		     item_file,
		     _SYSTEM_STRING( "Highlighted" ),
		     11,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write string.",
			 function );

			return( -1 );
		}
		value_32bit &= ~( LIBPFF_MESSAGE_STATUS_FLAG_HIGHLIGHTED );

		if( value_32bit != 0 )
		{
			if( item_file_write_string(
			     item_file,
			     _SYSTEM_STRING( ", " ),
			     2,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write string.",
				 function );

				return( -1 );
			}
		}
	}
	if( ( value_32bit & LIBPFF_MESSAGE_STATUS_FLAG_TAGGED ) != 0 )
	{
		if( item_file_write_string(
		     item_file,
		     _SYSTEM_STRING( "Tagged" ),
		     6,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write string.",
			 function );

			return( -1 );
		}
		value_32bit &= ~( LIBPFF_MESSAGE_STATUS_FLAG_TAGGED );

		if( value_32bit != 0 )
		{
			if( item_file_write_string(
			     item_file,
			     _SYSTEM_STRING( ", " ),
			     2,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write string.",
				 function );

				return( -1 );
			}
		}
	}
	if( ( value_32bit & LIBPFF_MESSAGE_STATUS_FLAG_HIDDEN ) != 0 )
	{
		if( item_file_write_string(
		     item_file,
		     _SYSTEM_STRING( "Hidden" ),
		     6,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write string.",
			 function );

			return( -1 );
		}
		value_32bit &= ~( LIBPFF_MESSAGE_STATUS_FLAG_HIDDEN );

		if( value_32bit != 0 )
		{
			if( item_file_write_string(
			     item_file,
			     _SYSTEM_STRING( ", " ),
			     2,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write string.",
				 function );

				return( -1 );
			}
		}
	}
	if( ( value_32bit & LIBPFF_MESSAGE_STATUS_FLAG_DELETED ) != 0 )
	{
		if( item_file_write_string(
		     item_file,
		     _SYSTEM_STRING( "Deleted" ),
		     7,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write string.",
			 function );

			return( -1 );
		}
		value_32bit &= ~( LIBPFF_MESSAGE_STATUS_FLAG_DELETED );

		if( value_32bit != 0 )
		{
			if( item_file_write_string(
			     item_file,
			     _SYSTEM_STRING( ", " ),
			     2,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write string.",
				 function );

				return( -1 );
			}
		}
	}
	if( ( value_32bit & LIBPFF_MESSAGE_STATUS_FLAG_DRAFT ) != 0 )
	{
		if( item_file_write_string(
		     item_file,
		     _SYSTEM_STRING( "Draft" ),
		     5,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write string.",
			 function );

			return( -1 );
		}
		value_32bit &= ~( LIBPFF_MESSAGE_STATUS_FLAG_DRAFT );

		if( value_32bit != 0 )
		{
			if( item_file_write_string(
			     item_file,
			     _SYSTEM_STRING( ", " ),
			     2,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write string.",
				 function );

				return( -1 );
			}
		}
	}
	if( ( value_32bit & LIBPFF_MESSAGE_STATUS_FLAG_ANSWERED ) != 0 )
	{
		if( item_file_write_string(
		     item_file,
		     _SYSTEM_STRING( "Answered" ),
		     8,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write string.",
			 function );

			return( -1 );
		}
		value_32bit &= ~( LIBPFF_MESSAGE_STATUS_FLAG_ANSWERED );

		if( value_32bit != 0 )
		{
			if( item_file_write_string(
			     item_file,
			     _SYSTEM_STRING( ", " ),
			     2,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write string.",
				 function );

				return( -1 );
			}
		}
	}
	if( value_32bit != 0 )
	{
		if( item_file_write_string(
		     item_file,
		     _SYSTEM_STRING( "Unknown: " ),
		     9,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write string.",
			 function );

			return( -1 );
		}
		if( item_file_write_integer_32bit_as_hexadecimal(
		     item_file,
		     value_32bit,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write 32-bit integer.",
			 function );

			return( -1 );
		}
	}
	if( item_file_write_string(
	     item_file,
	     _SYSTEM_STRING( ")" ),
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write string.",
		 function );

		return( -1 );
	}
	if( item_file_write_new_line(
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write new line.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Exports the message subject to an item file
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_message_subject_to_item_file(
     item_file_t *item_file,
     libpff_record_entry_t *record_entry,
     libcerror_error_t **error )
{
	system_character_t *value_string = NULL;
	static char *function            = "export_handle_export_message_subject_to_item_file";
	size_t value_string_size         = 0;
	int result                       = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libpff_record_entry_get_data_as_utf16_string_size(
	          record_entry,
	          &value_string_size,
	          error );
#else
	result = libpff_record_entry_get_data_as_utf8_string_size(
	          record_entry,
	          &value_string_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve string size.",
		 function );

		goto on_error;
	}
	else if( ( result != 0 )
	      && ( value_string_size > 0 ) )
	{
		value_string = system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libpff_record_entry_get_data_as_utf16_string(
		          record_entry,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libpff_record_entry_get_data_as_utf8_string(
		          record_entry,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve string.",
			 function );

			goto on_error;
		}
		/* Ignore the subject control codes for now
		 */
		if( value_string[ 0 ] < 0x20 )
		{
			result = item_file_write_string(
			          item_file,
			          &( value_string[ 2 ] ),
			          value_string_size - 3,
			          error );
		}
		else
		{
			result = item_file_write_string(
			          item_file,
			          value_string,
			          value_string_size - 1,
			          error );
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write subject.",
			 function );

			goto on_error;
		}
		memory_free(
		 value_string );

		value_string = NULL;
	}
	if( item_file_write_new_line(
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write new line.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	return( -1 );
}

/* Exports the Outlook message header to an item file
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_message_header_to_item_file(
     export_handle_t *export_handle,
     libpff_item_t *message,
     item_file_t *item_file,
     libcerror_error_t **error )
{
	mapi_property_definitions_t property_definitions[ 21 ] = {
		{ _SYSTEM_STRING( "Client submit time:\t\t\t" ), LIBPFF_ENTRY_TYPE_MESSAGE_CLIENT_SUBMIT_TIME, LIBPFF_VALUE_TYPE_FILETIME, 0, NULL },
		{ _SYSTEM_STRING( "Delivery time:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_MESSAGE_DELIVERY_TIME, LIBPFF_VALUE_TYPE_FILETIME, 0, NULL },
		{ _SYSTEM_STRING( "Creation time:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_MESSAGE_CREATION_TIME, LIBPFF_VALUE_TYPE_FILETIME, 0, NULL },
		{ _SYSTEM_STRING( "Modification time:\t\t\t" ), LIBPFF_ENTRY_TYPE_MESSAGE_MODIFICATION_TIME, LIBPFF_VALUE_TYPE_FILETIME, 0, NULL },
		{ _SYSTEM_STRING( "Size:\t\t\t\t\t" ), LIBPFF_ENTRY_TYPE_MESSAGE_SIZE, LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED, 0, NULL },
		{ _SYSTEM_STRING( "Flags:\t\t\t\t\t" ), LIBPFF_ENTRY_TYPE_MESSAGE_FLAGS, LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED, 0, &export_handle_export_message_flags_to_item_file },
		{ _SYSTEM_STRING( "Display name:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_DISPLAY_NAME, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Conversation topic:\t\t\t" ), LIBPFF_ENTRY_TYPE_MESSAGE_CONVERSATION_TOPIC, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Subject:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_MESSAGE_SUBJECT, LIBPFF_VALUE_TYPE_STRING, 0, &export_handle_export_message_subject_to_item_file },
		{ _SYSTEM_STRING( "Sender name:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_MESSAGE_SENDER_NAME, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Sender email address:\t\t\t" ), LIBPFF_ENTRY_TYPE_MESSAGE_SENDER_EMAIL_ADDRESS, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Sent representing name:\t\t\t" ), LIBPFF_ENTRY_TYPE_MESSAGE_SENT_REPRESENTING_NAME, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Sent representing email address:\t" ), LIBPFF_ENTRY_TYPE_MESSAGE_SENT_REPRESENTING_EMAIL_ADDRESS, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Importance:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_MESSAGE_IMPORTANCE, LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED, 0, &export_handle_export_message_importance_to_item_file },
		{ _SYSTEM_STRING( "Priority:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_MESSAGE_PRIORITY, LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED, 0, &export_handle_export_message_priority_to_item_file },
		{ _SYSTEM_STRING( "Sensitivity:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_MESSAGE_SENSITIVITY, LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED, 0, &export_handle_export_message_sensitivity_to_item_file },
		{ _SYSTEM_STRING( "Is a reminder:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_MESSAGE_IS_REMINDER, LIBPFF_VALUE_TYPE_BOOLEAN, 0, NULL },
		{ _SYSTEM_STRING( "Reminder time:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_MESSAGE_REMINDER_TIME, LIBPFF_VALUE_TYPE_FILETIME, 0, NULL },
		{ _SYSTEM_STRING( "Reminder signal time:\t\t\t" ), LIBPFF_ENTRY_TYPE_MESSAGE_REMINDER_SIGNAL_TIME, LIBPFF_VALUE_TYPE_FILETIME, 0, NULL },
		{ _SYSTEM_STRING( "Is private:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_MESSAGE_IS_PRIVATE, LIBPFF_VALUE_TYPE_BOOLEAN, 0, NULL },
		{ _SYSTEM_STRING( "Status:\t\t\t\t\t" ), LIBPFF_ENTRY_TYPE_MESSAGE_STATUS, LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED, 0, &export_handle_export_message_status_to_item_file } };

	static char *function = "export_handle_export_message_header_to_item_file";

	if( export_handle_export_item_value_to_item_file(
	     export_handle,
	     item_file,
	     _SYSTEM_STRING( "Message:" ),
	     message,
	     0,
	     (mapi_property_definitions_t *) &property_definitions,
	     21,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to export message item values to item file.",
		 function );

		return( -1 );
	}
	if( item_file_write_new_line(
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write new line.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Exports the message body
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_message_body(
     export_handle_t *export_handle,
     libpff_item_t *message,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	static char *function         = "export_handle_export_message_body";
	size_t message_html_body_size = 0;
	size_t message_rtf_body_size  = 0;
	size_t message_text_body_size = 0;
	int export_format             = 0;
	int has_html_body             = 0;
	int has_rtf_body              = 0;
	int has_text_body             = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	/* Determine the available message body types
	 */
	has_html_body = libpff_message_get_html_body_size(
	                 message,
	                 &message_html_body_size,
	                 error );

	if( has_html_body == -1 )
	{
		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );
	}
	has_rtf_body = libpff_message_get_rtf_body_size(
	                message,
	                &message_rtf_body_size,
	                error );

	if( has_rtf_body == -1 )
	{
		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );
	}
	has_text_body = libpff_message_get_plain_text_body_size(
	                 message,
	                 &message_text_body_size,
	                 error );

	if( has_text_body == -1 )
	{
		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );
	}
	/* Determine the message body export format
	 */
	if( export_handle->preferred_export_format == EXPORT_FORMAT_ALL )
	{
		export_format = EXPORT_FORMAT_ALL;
	}
	if( export_format == 0 )
	{
		if( ( has_text_body == 1 )
		 && ( message_text_body_size > 0 ) )
		{
			export_format = EXPORT_FORMAT_TEXT;
		}
		if( ( has_html_body == 1 )
		 && ( message_html_body_size > 0 )
		 && ( export_handle->preferred_export_format == EXPORT_FORMAT_HTML ) )
		{
			export_format = EXPORT_FORMAT_HTML;
		}
		if( ( has_rtf_body == 1 )
		 && ( message_rtf_body_size > 0 )
		 && ( export_handle->preferred_export_format == EXPORT_FORMAT_RTF ) )
		{
			export_format = EXPORT_FORMAT_RTF;
		}
	}
	if( export_format == 0 )
	{
		if( ( has_text_body == 1 )
		 && ( message_text_body_size > 0 ) )
		{
			export_format = EXPORT_FORMAT_TEXT;
		}
		else if( ( has_html_body == 1 )
		 && ( message_html_body_size > 0 ) )
		{
			export_format = EXPORT_FORMAT_HTML;
		}
		else if( ( has_rtf_body == 1 )
		 && ( message_rtf_body_size > 0 ) )
		{
			export_format = EXPORT_FORMAT_RTF;
		}
	}
	if( export_format == 0 )
	{
		if( has_text_body == 1 )
		{
			export_format = EXPORT_FORMAT_TEXT;
		}
		else if( has_html_body == 1 )
		{
			export_format = EXPORT_FORMAT_HTML;
		}
		else if( has_rtf_body == 1 )
		{
			export_format = EXPORT_FORMAT_RTF;
		}
	}
	if( export_format == EXPORT_FORMAT_ALL )
	{
		if( has_html_body == 1 )
		{
			if( export_handle_export_message_body_html(
			     export_handle,
			     message,
			     message_html_body_size,
			     export_path,
			     export_path_length,
			     log_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to export HTML message body.",
				 function );

				return( -1 );
			}
		}
		if( has_rtf_body == 1 )
		{
			if( export_handle_export_message_body_rtf(
			     export_handle,
			     message,
			     message_rtf_body_size,
			     export_path,
			     export_path_length,
			     log_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to export RTF message body.",
				 function );

				return( -1 );
			}
		}
		if( has_text_body == 1 )
		{
			if( export_handle_export_message_body_plain_text(
			     export_handle,
			     message,
			     message_text_body_size,
			     export_path,
			     export_path_length,
			     log_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to export plain text message body.",
				 function );

				return( -1 );
			}
		}
	}
	else if( export_format == EXPORT_FORMAT_HTML )
	{
		if( export_handle_export_message_body_html(
		     export_handle,
		     message,
		     message_html_body_size,
		     export_path,
		     export_path_length,
		     log_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to export HTML message body.",
			 function );

			return( -1 );
		}
	}
	else if( export_format == EXPORT_FORMAT_RTF )
	{
		if( export_handle_export_message_body_rtf(
		     export_handle,
		     message,
		     message_rtf_body_size,
		     export_path,
		     export_path_length,
		     log_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to export RTF message body.",
			 function );

			return( -1 );
		}
	}
	else if( export_format == EXPORT_FORMAT_TEXT )
	{
		if( export_handle_export_message_body_plain_text(
		     export_handle,
		     message,
		     message_text_body_size,
		     export_path,
		     export_path_length,
		     log_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to export plain text message body.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Exports the HTML message body
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_message_body_html(
     export_handle_t *export_handle,
     libpff_item_t *message,
     size_t message_html_body_size,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	system_character_t filename[ 13 ];

	item_file_t *item_file = NULL;
	static char *function  = "export_handle_export_message_body_html";
	size_t filename_size   = 0;
	int result             = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( system_string_copy(
	     filename,
	     _SYSTEM_STRING( "Message.html" ),
	     12 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set filename.",
		 function );

		goto on_error;
	}
	filename[ 12 ] = 0;
	filename_size  = 13;

	log_handle_printf(
	 log_handle,
	 "Saving HTML message body as: %" PRIs_SYSTEM "\n",
	 filename );

	result = export_handle_create_item_file(
	          export_handle,
	          export_path,
	          export_path_length,
	          filename,
	          filename_size - 1,
	          &item_file,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create item file.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		log_handle_printf(
		 log_handle,
		 "Skipping message body file: %" PRIs_SYSTEM " it already exists.",
		 filename );

		return( 1 );
	}
	if( export_handle_export_message_body_html_to_item_file(
	     export_handle,
	     message,
	     message_html_body_size,
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to export HTML message body.",
		 function );

		goto on_error;
	}
	if( item_file_close(
	     item_file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close item file.",
		 function );

		goto on_error;
	}
	if( item_file_free(
	     &item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free item file.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( item_file != NULL )
	{
		item_file_free(
		 &item_file,
		 NULL );
	}
	return( -1 );
}

/* Exports the HTML message body to an item file
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_message_body_html_to_item_file(
     export_handle_t *export_handle,
     libpff_item_t *message,
     size_t message_html_body_size,
     item_file_t *item_file,
     libcerror_error_t **error )
{
	uint8_t *value_string = NULL;
	static char *function = "export_handle_export_message_body_html_to_item_file";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( message_html_body_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid message HTML body size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( message_html_body_size > 0 )
	{
		value_string = (uint8_t *) memory_allocate(
		                            sizeof( uint8_t ) * message_html_body_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create HTML body.",
			 function );

			goto on_error;
		}
		if( libpff_message_get_html_body(
		     message,
		     value_string,
		     message_html_body_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve HTML body.",
			 function );

			goto on_error;
		}
		/* Do not write the end-of-string byte
		 */
		if( item_file_write_buffer(
		     item_file,
		     value_string,
		     message_html_body_size - 1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write HTML body.",
			 function );

			goto on_error;
		}
		memory_free(
		 value_string );

		value_string = NULL;
	}
	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	return( -1 );
}

/* Exports the RTF message body
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_message_body_rtf(
     export_handle_t *export_handle,
     libpff_item_t *message,
     size_t message_rtf_body_size,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	system_character_t filename[ 12 ];

	item_file_t *item_file = NULL;
	static char *function  = "export_handle_export_message_body_rtf";
	size_t filename_size   = 0;
	int result             = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( system_string_copy(
	     filename,
	     _SYSTEM_STRING( "Message.rtf" ),
	     11 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set RTF message body filename.",
		 function );

		goto on_error;
	}
	filename[ 11 ] = 0;
	filename_size  = 12;

	log_handle_printf(
	 log_handle,
	 "Saving RTF message body as: %" PRIs_SYSTEM "\n",
	 filename );

	result = export_handle_create_item_file(
	          export_handle,
	          export_path,
	          export_path_length,
	          filename,
	          filename_size - 1,
	          &item_file,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create item file.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		log_handle_printf(
		 log_handle,
		 "Skipping message body file: %" PRIs_SYSTEM " it already exists.",
		 filename );

		return( 1 );
	}
	if( export_handle_export_message_body_rtf_to_item_file(
	     export_handle,
	     message,
	     message_rtf_body_size,
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to export RTF message body.",
		 function );

		goto on_error;
	}
	if( item_file_close(
	     item_file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close item file.",
		 function );

		goto on_error;
	}
	if( item_file_free(
	     &item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free item file.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( item_file != NULL )
	{
		item_file_free(
		 &item_file,
		 NULL );
	}
	return( -1 );
}

/* Exports the RTF message body to an item file
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_message_body_rtf_to_item_file(
     export_handle_t *export_handle,
     libpff_item_t *message,
     size_t message_rtf_body_size,
     item_file_t *item_file,
     libcerror_error_t **error )
{
	uint8_t *value_string = NULL;
	static char *function = "export_handle_export_message_body_rtf_to_item_file";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( message_rtf_body_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid message RTF body size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( message_rtf_body_size > 0 )
	{
		value_string = (uint8_t *) memory_allocate(
					    sizeof( uint8_t ) * message_rtf_body_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create RTF body.",
			 function );

			goto on_error;
		}
		if( libpff_message_get_rtf_body(
		     message,
		     value_string,
		     message_rtf_body_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve RTF body.",
			 function );

			goto on_error;
		}
		/* Do not write the end of string byte
		 */
		if( item_file_write_buffer(
		     item_file,
		     value_string,
		     message_rtf_body_size - 1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write RTF body.",
			 function );

			goto on_error;
		}
		memory_free(
		 value_string );

		value_string = NULL;
	}
	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	return( -1 );
}

/* Exports the plain text message body
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_message_body_plain_text(
     export_handle_t *export_handle,
     libpff_item_t *message,
     size_t plain_text_body_size,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	system_character_t filename[ 12 ];

	item_file_t *item_file = NULL;
	static char *function  = "export_handle_export_message_body_plain_text";
	size_t filename_size   = 0;
	int result             = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( system_string_copy(
	     filename,
	     _SYSTEM_STRING( "Message.txt" ),
	     11 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set plain text message body filename.",
		 function );

		goto on_error;
	}
	filename[ 11 ] = 0;
	filename_size  = 12;

	log_handle_printf(
	 log_handle,
	 "Saving plain text message body as: %" PRIs_SYSTEM "\n",
	 filename );

	result = export_handle_create_item_file(
	          export_handle,
	          export_path,
	          export_path_length,
	          filename,
	          filename_size - 1,
	          &item_file,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create message body file.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		log_handle_printf(
		 log_handle,
		 "Skipping message body file: %" PRIs_SYSTEM " it already exists.",
		 filename );

		return( 1 );
	}
	if( export_handle_export_message_body_plain_text_to_item_file(
	     export_handle,
	     message,
	     plain_text_body_size,
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to export plain text message body.",
		 function );

		goto on_error;
	}
	if( item_file_close(
	     item_file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close item file.",
		 function );

		goto on_error;
	}
	if( item_file_free(
	     &item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free item file.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( item_file != NULL )
	{
		item_file_free(
		 &item_file,
		 NULL );
	}
	return( -1 );
}

/* Exports the plain text message body to an item file
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_message_body_plain_text_to_item_file(
     export_handle_t *export_handle,
     libpff_item_t *message,
     size_t plain_text_body_size,
     item_file_t *item_file,
     libcerror_error_t **error )
{
	uint8_t *plain_text_body = NULL;
	static char *function    = "export_handle_export_message_body_plain_text_to_item_file";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( plain_text_body_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid plain text body size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( plain_text_body_size > 0 )
	{
		plain_text_body = (uint8_t *) memory_allocate(
		                               sizeof( uint8_t ) * plain_text_body_size );

		if( plain_text_body == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create plain text body.",
			 function );

			goto on_error;
		}
		if( libpff_message_get_plain_text_body(
		     message,
		     plain_text_body,
		     plain_text_body_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve plain text body.",
			 function );

			goto on_error;
		}
		/* Do not write the end of string byte
		 */
		if( item_file_write_buffer(
		     item_file,
		     plain_text_body,
		     plain_text_body_size - 1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write plain text body.",
			 function );

			goto on_error;
		}
		memory_free(
		 plain_text_body );

		plain_text_body = NULL;
	}
	return( 1 );

on_error:
	if( plain_text_body != NULL )
	{
		memory_free(
		 plain_text_body );
	}
	return( -1 );
}

/* Exports the Outlook message conversation index
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_message_conversation_index(
     export_handle_t *export_handle,
     libpff_item_t *message,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	item_file_t *item_file = NULL;
	static char *function  = "export_handle_export_message_conversation_index";
	int result             = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	result = export_handle_create_item_file(
	          export_handle,
	          export_path,
	          export_path_length,
	          _SYSTEM_STRING( "ConversationIndex.txt" ),
	          21,
	          &item_file,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create item file.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		log_handle_printf(
		 log_handle,
		 "Skipping conversation index file it already exists.\n" );

		return( 1 );
	}
	if( export_handle_export_message_conversation_index_to_item_file(
	     export_handle,
	     item_file,
	     message,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to export message conversation index.",
		 function );

		goto on_error;
	}
	if( item_file_close(
	     item_file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close item file.",
		 function );

		goto on_error;
	}
	if( item_file_free(
	     &item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free item file.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( item_file != NULL )
	{
		item_file_free(
		 &item_file,
		 NULL );
	}
	return( -1 );
}

/* Exports the Outlook message conversation index to an item file
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_message_conversation_index_to_item_file(
     export_handle_t *export_handle,
     item_file_t *item_file,
     libpff_item_t *message,
     libcerror_error_t **error )
{
	libpff_record_entry_t *record_entry = NULL;
	libpff_record_set_t *record_set     = NULL;
	uint8_t *data                       = NULL;
	static char *function               = "export_handle_export_message_conversation_index_to_item_file";
	size_t data_size                    = 0;
	int result                          = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( libpff_item_get_record_set_by_index(
	     message,
	     0,
	     &record_set,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record set: 0.",
		 function );

		goto on_error;
	}
	result = libpff_record_set_get_entry_by_type(
	          record_set,
	          LIBPFF_ENTRY_TYPE_MESSAGE_CONVERSATION_INDEX,
	          LIBPFF_VALUE_TYPE_BINARY_DATA,
	          &record_entry,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record entry: 0x%04 " PRIx32 " 0x%04" PRIx32 ".",
		 function,
		 LIBPFF_ENTRY_TYPE_MESSAGE_CONVERSATION_INDEX,
		 LIBPFF_VALUE_TYPE_BINARY_DATA );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libpff_record_entry_get_data_size(
		     record_entry,
		     &data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value data size.",
			 function );

			goto on_error;
		}
		data = (uint8_t *) memory_allocate(
		                    sizeof( uint8_t ) * data_size );

		if( data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create data.",
			 function );

			goto on_error;
		}
		if( libpff_record_entry_get_data(
		     record_entry,
		     data,
		     data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value data.",
			 function );

			goto on_error;
		}
		if( export_handle_export_message_conversation_index_data_to_item_file(
		     export_handle,
		     item_file,
		     data,
		     data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to export message conversation index.",
			 function );

			goto on_error;
		}
		memory_free(
		 data );

		data = NULL;
	}
	if( libpff_record_entry_free(
	     &record_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free record entry.",
		 function );

		goto on_error;
	}
	if( libpff_record_set_free(
	     &record_set,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free record set.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( data != NULL )
	{
		memory_free(
		 data );
	}
	if( record_entry != NULL )
	{
		libpff_record_entry_free(
		 &record_entry,
		 NULL );
	}
	if( record_set != NULL )
	{
		libpff_record_set_free(
		 &record_set,
		 NULL );
	}
	return( -1 );
}

/* Exports the Outlook message conversation index to an item file
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_message_conversation_index_data_to_item_file(
     export_handle_t *export_handle,
     item_file_t *item_file,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	uint8_t filetime_buffer[ 8 ];

	libfdatetime_filetime_t *delta_filetime = NULL;
	libfdatetime_filetime_t *filetime       = NULL;
	libfguid_identifier_t *guid             = NULL;
	static char *function                   = "export_handle_export_message_conversation_index_data_to_item_file";
	uint64_t value_64bit                    = 0;
	uint32_t data_offset                    = 0;
	int list_index                          = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( item_file_write_value_description(
	     item_file,
	     _SYSTEM_STRING( "Conversation index:" ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write value description.",
		 function );

		goto on_error;
	}
	if( data_size >= 22 )
	{
		/* According to MSDN the first byte is reserved
		 * and should always be 0x01 however it makes
		 * more sense that it's the most significant
		 * part of the current system filetime data
		 */
		if( data[ 0 ] == 0x01 )
		{
			if( libfdatetime_filetime_initialize(
			     &filetime,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create filetime.",
				 function );

				goto on_error;
			}
			if( libfdatetime_filetime_initialize(
			     &delta_filetime,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create delta filetime.",
				 function );

				goto on_error;
			}
			if( item_file_write_value_description(
			     item_file,
			     _SYSTEM_STRING( "Header block:" ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write value description.",
				 function );

				goto on_error;
			}
			/* The current system file data is in big-endian
			 */
			filetime_buffer[ 0 ] = 0;
			filetime_buffer[ 1 ] = 0;
			filetime_buffer[ 2 ] = data[ 5 ];
			filetime_buffer[ 3 ] = data[ 4 ];
			filetime_buffer[ 4 ] = data[ 3 ];
			filetime_buffer[ 5 ] = data[ 2 ];
			filetime_buffer[ 6 ] = data[ 1 ];
			filetime_buffer[ 7 ] = data[ 0 ];

			if( libfdatetime_filetime_copy_from_byte_stream(
			     filetime,
			     filetime_buffer,
			     8,
			     LIBFDATETIME_ENDIAN_LITTLE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_CONVERSION,
				 LIBCERROR_CONVERSION_ERROR_GENERIC,
				 "%s: unable to create filetime.",
				 function );

				goto on_error;
			}
			if( item_file_write_value_filetime(
			     item_file,
			     _SYSTEM_STRING( "\tFiletime:\t" ),
			     filetime,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write value filetime.",
				 function );

				goto on_error;
			}
			if( libfguid_identifier_initialize(
			     &guid,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create GUID.",
				 function );

				goto on_error;
			}
			/* Currently it is assumed that the GUID is in big-endian
			 */
			if( libfguid_identifier_copy_from_byte_stream(
			     guid,
			     &( data[ 6 ] ),
			     16,
			     LIBFGUID_ENDIAN_BIG,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy byte stream to GUID.",
				 function );

				goto on_error;
			}
			if( item_file_write_string(
			     item_file,
			     _SYSTEM_STRING( "\tGUID:\t\t" ),
			     8,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write string.",
				 function );

				goto on_error;
			}
			if( item_file_write_guid(
			     item_file,
			     guid,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write GUID.",
				 function );

				goto on_error;
			}
			if( item_file_write_new_line(
			     item_file,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write new line.",
				 function );

				goto on_error;
			}
			if( libfguid_identifier_free(
			     &guid,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free GUID.",
				 function );

				goto on_error;
			}
			list_index = 1;

			for( data_offset = 22;
			     data_offset < data_size;
			     data_offset += 5 )
			{
				if( item_file_write_value_integer_32bit_as_decimal(
				     item_file,
				     _SYSTEM_STRING( "Child block: " ),
				     (uint32_t) list_index,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to write 32-bit integer value.",
					 function );

					goto on_error;
				}
				/* Use a 64-bit value to be able to shift the bits in the right position
				 * current system time delta is in big-endian
				 */
				byte_stream_copy_to_uint32_little_endian(
				 &( data[ data_offset ] ),
				 value_64bit );

				/* Make sure only 31-bits are set and the rest is cleared
				 */
				value_64bit &= 0x07fffffffUL;

				/* Check if the highest bit is set
				 */
				if( ( data[ data_offset ] & 0x80 ) == 0 )
				{
					/* Discard the highest 15-bits and the lowest 18-bits
					 */
					value_64bit <<= 18;
				}
				else
				{
					/* Discard the highest 10-bits and the lowest 23-bits
					 */
					value_64bit <<= 23;
				}
				if( libfdatetime_filetime_copy_from_64bit(
				     delta_filetime,
				     value_64bit,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_CONVERSION,
					 LIBCERROR_CONVERSION_ERROR_GENERIC,
					 "%s: unable to create delta filetime.",
					 function );

					goto on_error;
				}
				/* The next child block uses the previous filetime as its base
				 * not the filetime in the header block as MSDN states
				 * so the filetime is updated
				 */
				if( libfdatetime_filetime_add(
				     filetime,
				     delta_filetime,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to create filetime.",
					 function );

					goto on_error;
				}
				if( item_file_write_value_filetime(
				     item_file,
				     _SYSTEM_STRING( "\tFiletime:\t" ),
				     filetime,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to write value filetime.",
					 function );

					goto on_error;
				}
				if( item_file_write_value_integer_32bit_as_decimal(
				     item_file,
				     _SYSTEM_STRING( "\tRandom number:\t" ),
				     (uint32_t) ( ( data[ data_offset + 4 ] & 0xf0 ) >> 4 ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to write 32-bit integer value.",
					 function );

					goto on_error;
				}
				if( item_file_write_value_integer_32bit_as_decimal(
				     item_file,
				     _SYSTEM_STRING( "\tSequence count:\t" ),
				     (uint32_t) ( data[ data_offset + 4 ] & 0x0f ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_WRITE_FAILED,
					 "%s: unable to write 32-bit integer value.",
					 function );

					goto on_error;
				}
				list_index++;
			}
			if( item_file_write_new_line(
			     item_file,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write new line.",
				 function );

				goto on_error;
			}
			if( libfdatetime_filetime_free(
			     &delta_filetime,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free delta filetime.",
				 function );

				goto on_error;
			}
			if( libfdatetime_filetime_free(
			     &filetime,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free filetime.",
				 function );

				goto on_error;
			}
		}
	}
	if( item_file_write_new_line(
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write new line.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( guid != NULL )
	{
		libfguid_identifier_free(
		 &guid,
		 NULL );
	}
	if( delta_filetime != NULL )
	{
		libfdatetime_filetime_free(
		 &delta_filetime,
		 NULL );
	}
	if( filetime != NULL )
	{
		libfdatetime_filetime_free(
		 &filetime,
		 NULL );
	}
	return( -1 );
}

/* Exports the transport message headers to a separate file
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_message_transport_headers(
     export_handle_t *export_handle,
     libpff_item_t *message,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	system_character_t transport_headers_filename[ 20 ];

	item_file_t *item_file                 = NULL;
	static char *function                  = "export_handle_export_message_transport_headers";
	size_t transport_headers_filename_size = 20;
	size_t message_transport_headers_size  = 0;
	int result                             = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	/* Determine the message transport headers size
	 */
	result = libpff_message_get_entry_value_utf8_string_size(
	          message,
	          LIBPFF_ENTRY_TYPE_MESSAGE_TRANSPORT_HEADERS,
	          &message_transport_headers_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable determine message transport headers size.",
		 function );

		goto on_error;
	}
	if( ( result == 0 )
	 || ( message_transport_headers_size == 0 ) )
	{
		return( 1 );
	}
	if( system_string_copy(
	     transport_headers_filename,
	     _SYSTEM_STRING( "InternetHeaders.txt" ),
	     19 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set message transport headers filename.",
		 function );

		goto on_error;
	}
	transport_headers_filename[ 19 ] = 0;

	log_handle_printf(
	 log_handle,
	 "Saving message transport headers as: %" PRIs_SYSTEM "\n",
	 transport_headers_filename );

	result = export_handle_create_item_file(
	          export_handle,
	          export_path,
	          export_path_length,
	          transport_headers_filename,
	          transport_headers_filename_size - 1,
	          &item_file,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create item file.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		log_handle_printf(
		 log_handle,
		 "Skipping message transport headers file: %" PRIs_SYSTEM " it already exists.",
		 transport_headers_filename );

		return( 1 );
	}
	if( export_handle_export_message_transport_headers_to_item_file(
	     export_handle,
	     message,
	     message_transport_headers_size,
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to export message transport headers.",
		 function );

		goto on_error;
	}
	if( item_file_close(
	     item_file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close item file.",
		 function );

		goto on_error;
	}
	if( item_file_free(
	     &item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free item file.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( item_file != NULL )
	{
		item_file_free(
		 &item_file,
		 NULL );
	}
	return( -1 );
}

/* Exports the transport message headers to an item file
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_message_transport_headers_to_item_file(
     export_handle_t *export_handle,
     libpff_item_t *message,
     size_t message_transport_headers_size,
     item_file_t *item_file,
     libcerror_error_t **error )
{
	uint8_t *entry_string = NULL;
	static char *function = "export_handle_export_message_transport_headers_to_item_file";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( message_transport_headers_size > 0 )
	{
		entry_string = (uint8_t *) memory_allocate(
					    sizeof( uint8_t ) * message_transport_headers_size );

		if( entry_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create transport headers.",
			 function );

			goto on_error;
		}
		if( libpff_message_get_entry_value_utf8_string(
		     message,
		     LIBPFF_ENTRY_TYPE_MESSAGE_TRANSPORT_HEADERS,
		     entry_string,
		     message_transport_headers_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve transport headers.",
			 function );

			goto on_error;
		}
		/* Do not write the end of string
		 */
		if( item_file_write_buffer(
		     item_file,
		     entry_string,
		     message_transport_headers_size - 1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write transport headers.",
			 function );

			goto on_error;
		}
		memory_free(
		 entry_string );

		entry_string = NULL;
	}
	return( 1 );

on_error:
	if( entry_string != NULL )
	{
		memory_free(
		 entry_string );
	}
	return( -1 );
}

/* Exports the attachments
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_attachments(
     export_handle_t *export_handle,
     libpff_item_t *item,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	libpff_item_t *attachment            = NULL;
	libpff_item_t *attachments           = NULL;
	system_character_t *attachments_path = NULL;
	static char *function                = "export_handle_export_attachments";
	size_t attachments_path_size         = 0;
	int attachment_index                 = 0;
	int number_of_attachments            = 0;
	int result                           = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item.",
		 function );

		return( -1 );
	}
	if( libpff_message_get_number_of_attachments(
	     item,
	     &number_of_attachments,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of attachments.",
		 function );

		goto on_error;
	}
	if( number_of_attachments > 0 )
	{
		/* Create the attachments directory
		 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libcpath_path_join_wide(
		          &attachments_path,
		          &attachments_path_size,
		          export_path,
		          export_path_length,
		          _SYSTEM_STRING( "Attachments" ),
		          11,
		          error );
#else
		result = libcpath_path_join(
		          &attachments_path,
		          &attachments_path_size,
		          export_path,
		          export_path_length,
		          _SYSTEM_STRING( "Attachments" ),
		          11,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable create attachments path.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libcfile_file_exists_wide(
			  attachments_path,
		          error );
#else
		result = libcfile_file_exists(
			  attachments_path,
		          error );
#endif
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_GENERIC,
			 "%s: unable to determine if %" PRIs_SYSTEM " exists.",
			 function,
			 attachments_path );

			goto on_error;
		}
		else if( result != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_GENERIC,
			 "%s: %" PRIs_SYSTEM " already exists.",
			 function,
			 attachments_path );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libcpath_path_make_directory_wide(
		     attachments_path,
		     error ) != 1 )
#else
		if( libcpath_path_make_directory(
		     attachments_path,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to make directory: %" PRIs_SYSTEM ".",
			 function,
			 attachments_path );

			goto on_error;
		}
		log_handle_printf(
		 log_handle,
		 "Created directory: %" PRIs_SYSTEM ".\n",
		 attachments_path );

		if( export_handle->dump_item_values != 0 )
		{
			result = libpff_message_get_attachments(
				  item,
				  &attachments,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve attachments.",
				 function );

				goto on_error;
			}
			else if( result == 1 )
			{
				if( export_handle_export_item_values(
				     export_handle,
				     attachments,
				     _SYSTEM_STRING( "ItemValues.txt" ),
				     14,
				     attachments_path,
				     attachments_path_size - 1,
				     log_handle,
				     error ) != 1 )
				{
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: unable to export item values.\n",
						 function );
					}
					if( ( error != NULL )
					 && ( *error != NULL ) )
					{
						libcnotify_print_error_backtrace(
						 *error );
					}
					libcerror_error_free(
					 error );

					log_handle_printf(
					 log_handle,
					 "Unable to export attachments item values.\n" );
				}
				if( libpff_item_free(
				     &attachments,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free attachments.",
					 function );

					goto on_error;
				}
			}
		}
		/* Create the attachments files
		 */
		for( attachment_index = 0;
		     attachment_index < number_of_attachments;
		     attachment_index++ )
		{
			if( export_handle->abort != 0 )
			{
				return( -1 );
			}
			if( export_handle->print_status_information != 0 )
			{
				fprintf(
				 export_handle->notify_stream,
				 "Exporting attachment %d out of %d.\n",
				 attachment_index + 1,
				 number_of_attachments );
			}
			if( libpff_message_get_attachment(
			     item,
			     attachment_index,
			     &attachment,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve attachment.",
				 function );

				goto on_error;
			}
			if( export_handle_export_attachment(
			     export_handle,
			     attachment,
			     attachment_index,
			     number_of_attachments,
			     attachments_path,
			     attachments_path_size - 1,
			     log_handle,
			     error ) != 1 )
			{
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: unable to export attachment: %d.\n",
					 function,
					 attachment_index + 1 );
				}
				if( ( error != NULL )
				 && ( *error != NULL ) )
				{
					libcnotify_print_error_backtrace(
					 *error );
				}
				libcerror_error_free(
				 error );

				log_handle_printf(
				 log_handle,
				 "Unable to export attachment: %d.\n",
				 attachment_index + 1 );
			}
			if( libpff_item_free(
			     &attachment,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free attachment.",
				 function );

				goto on_error;
			}
		}
		memory_free(
		 attachments_path );
	}
	return( 1 );

on_error:
	if( attachments_path != NULL )
	{
		memory_free(
		 attachments_path );
	}
	return( -1 );
}

/* Exports the attachment
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_attachment(
     export_handle_t *export_handle,
     libpff_item_t *attachment,
     int attachment_index,
     int number_of_attachments,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	static char *function = "export_handle_export_attachment";
	int attachment_type   = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( libpff_attachment_get_type(
	     attachment,
	     &attachment_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attachment type.",
		 function );

		return( -1 );
	}
	if( ( attachment_type != LIBPFF_ATTACHMENT_TYPE_DATA )
	 && ( attachment_type != LIBPFF_ATTACHMENT_TYPE_ITEM )
	 && ( attachment_type != LIBPFF_ATTACHMENT_TYPE_REFERENCE ) )
	{
		fprintf(
		 export_handle->notify_stream,
		 "Unsupported attachment type.\n" );

		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unsupported attachment type.\n",
			 function );
		}
		log_handle_printf(
		 log_handle,
		 "Unsupported attachment type.\n" );

		return( 1 );
	}
	if( attachment_type == LIBPFF_ATTACHMENT_TYPE_REFERENCE )
	{
		fprintf(
		 export_handle->notify_stream,
		 "Attachment is stored externally.\n" );

		log_handle_printf(
		 log_handle,
		 "Attachment is stored externally.\n" );

		return( 1 );
	}
	if( attachment_type == LIBPFF_ATTACHMENT_TYPE_DATA )
	{
		if( export_handle_export_attachment_data(
		     export_handle,
		     attachment,
		     attachment_index,
		     number_of_attachments,
		     export_path,
		     export_path_length,
		     log_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to export attachment data.",
			 function );

			return( -1 );
		}
	}
	else if( attachment_type == LIBPFF_ATTACHMENT_TYPE_ITEM )
	{
		if( export_handle_export_attachment_item(
		     export_handle,
		     attachment,
		     attachment_index,
		     export_path,
		     export_path_length,
		     log_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to export attachment item.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves an attachment filename
 * If no attachment filename is found default to "#_Attachment.txt"
 * Returns 1 if successful or -1 on error
 */
int export_handle_get_attachment_filename(
     export_handle_t *export_handle,
     libpff_item_t *attachment,
     int attachment_index,
     int number_of_attachments,
     system_character_t **attachment_filename,
     size_t *attachment_filename_size,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	system_character_t *long_filename  = NULL;
	system_character_t *name           = NULL;
	system_character_t *sanitized_name = NULL;
	static char *function              = "export_handle_get_attachment_filename";
	size_t long_filename_size          = 0;
	size_t name_index                  = 0;
	size_t name_length                 = 0;
	size_t name_size                   = 0;
	size_t sanitized_name_size         = 0;
	size_t string_index                = 0;
	int result                         = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( attachment_filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attachment filename.",
		 function );

		return( -1 );
	}
	if( *attachment_filename != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid attachment filename value already set.",
		 function );

		return( -1 );
	}
	if( attachment_filename_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attachment filename size.",
		 function );

		return( -1 );
	}
	result = export_handle_item_get_value_string_size_by_type(
	          export_handle,
	          attachment,
	          0,
	          LIBPFF_ENTRY_TYPE_ATTACHMENT_FILENAME_LONG,
	          &long_filename_size,
	          NULL );

	/* Reserve space for a leading decimal and a _
	 */
	while( number_of_attachments >= 10 )
	{
		number_of_attachments /= 10;

		name_index++;
	}
	if( long_filename_size > (size_t) ( SSIZE_MAX - ( 2 + name_index ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: attachment long filename size value exceeds maximum.",
		 function );

		goto on_error;
	}
	name_size = 2 + name_index + long_filename_size;

	if( name_size < ( name_index + 17 ) )
	{
		name_size = name_index + 17;
	}
	name = system_string_allocate(
	        name_size );

	if( name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create attachment filename.",
		 function );

		goto on_error;
	}
	string_index = name_index;

	/* Start with 1_ */
	attachment_index += 1;

	while( string_index > 0 )
	{
		name[ string_index-- ] = (system_character_t) ( '0' + ( attachment_index % 10 ) );

		attachment_index /= 10;
	}
	name[ 0 ] = (system_character_t) ( '0' + ( attachment_index % 10 ) );

	name_index++;

	name[ name_index++ ] = (system_character_t) '_';

	long_filename = &( name[ name_index ] );

/* TODO make this more efficient by directly operating on record_entry */
	result = export_handle_item_get_value_string_by_type(
	          export_handle,
	          attachment,
	          0,
	          LIBPFF_ENTRY_TYPE_ATTACHMENT_FILENAME_LONG,
	          long_filename,
	          long_filename_size,
	          NULL );

	if( result == 1 )
	{
		name_length = system_string_length(
		               name );

		if( name_length == 0 )
		{
			result = 0;
		}
		else
		{
			log_handle_printf(
			 log_handle,
			 "Saving attachment with filename: %" PRIs_SYSTEM "",
			 long_filename );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			if( libcpath_path_get_sanitized_filename_wide(
			     name,
			     name_length,
			     &sanitized_name,
			     &sanitized_name_size,
			     error ) != 1 )
#else
			if( libcpath_path_get_sanitized_filename(
			     name,
			     name_length,
			     &sanitized_name,
			     &sanitized_name_size,
			     error ) != 1 )
#endif
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable sanitize attachment filename.",
				 function );

				goto on_error;
			}
			memory_free(
			 name );

			name                = sanitized_name;
			name_size           = sanitized_name_size;
			sanitized_name      = NULL;
			sanitized_name_size = 0;

			log_handle_printf(
			 log_handle,
			 " as: %" PRIs_SYSTEM "\n",
			 name );
		}
	}
	if( result != 1 )
	{
		if( system_string_copy(
		     &( name[ name_index ] ),
		     _SYSTEM_STRING( "Attachment.txt" ),
		     14 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set attachment filename.",
			 function );

			goto on_error;
		}
		name[ name_index + 14 ] = 0;

		name_size = name_index + 15;

		log_handle_printf(
		 log_handle,
		 "Missing attachment filename defaulting to: %" PRIs_SYSTEM "\n",
		 name );
	}
	*attachment_filename      = name;
	*attachment_filename_size = name_size;

	return( 1 );

on_error:
	if( sanitized_name != NULL )
	{
		memory_free(
		 sanitized_name );
	}
	if( name != NULL )
	{
		memory_free(
		 name );
	}
	return( -1 );
}

/* Exports the attachment data
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_attachment_data(
     export_handle_t *export_handle,
     libpff_item_t *attachment,
     int attachment_index,
     int number_of_attachments,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	system_character_t *attachment_filename = NULL;
	system_character_t *target_path         = NULL;
	FILE *attachment_file_stream            = NULL;
	uint8_t *attachment_data                = NULL;
	static char *function                   = "export_handle_export_attachment_data";
	size64_t attachment_data_size           = 0;
	size_t attachment_filename_size         = 0;
	size_t read_size                        = 0;
	size_t target_path_size                 = 0;
	size_t write_count                      = 0;
	ssize_t read_count                      = 0;
	int result                              = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	/* Determine the attachment filename size
	 */
	if( export_handle_get_attachment_filename(
	     export_handle,
	     attachment,
	     attachment_index,
	     number_of_attachments,
	     &attachment_filename,
	     &attachment_filename_size,
	     log_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable retrieve attachment filename: %d.",
		 function,
		 attachment_index );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libcpath_path_join_wide(
	          &target_path,
	          &target_path_size,
	          export_path,
	          export_path_length,
	          attachment_filename,
	          attachment_filename_size - 1,
	          error );
#else
	result = libcpath_path_join(
	          &target_path,
	          &target_path_size,
	          export_path,
	          export_path_length,
	          attachment_filename,
	          attachment_filename_size - 1,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create target path.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libcfile_file_exists_wide(
		  target_path,
	          error );
#else
	result = libcfile_file_exists(
		  target_path,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_GENERIC,
		 "%s: unable to determine if %" PRIs_SYSTEM " exists.",
		 function,
		 target_path );

		goto on_error;
	}
	else if( result != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_GENERIC,
		 "%s: %" PRIs_SYSTEM " already exists.",
		 function,
		 target_path );

		goto on_error;
	}
	memory_free(
	 attachment_filename );

	attachment_filename = NULL;

	if( target_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing target path.",
		 function );

		goto on_error;
	}
	/* Create the attachment file
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	attachment_file_stream = file_stream_open_wide(
				  target_path,
				  _SYSTEM_STRING( FILE_STREAM_BINARY_OPEN_WRITE ) );
#else
	attachment_file_stream = file_stream_open(
				  target_path,
				  FILE_STREAM_BINARY_OPEN_WRITE );
#endif

	if( attachment_file_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open: %" PRIs_SYSTEM ".",
		 function,
		 target_path );

		goto on_error;
	}
	memory_free(
	 target_path );

	target_path = NULL;

	result = libpff_attachment_get_data_size(
		  attachment,
		  &attachment_data_size,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attachment data size.",
		 function );

		goto on_error;
	}
	/* If there is no attachment data an empty file is written
	 */
	if( ( result != 0 )
	 && ( attachment_data_size > 0 ) )
	{
		/* This function in not necessary for normal use
		 * but it was added for testing
		 */
		if( libpff_attachment_data_seek_offset(
		     attachment,
		     0,
		     SEEK_SET,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to seek the start of the attachment data.",
			 function );

			goto on_error;
		}
		attachment_data = (uint8_t *) memory_allocate(
					       sizeof( uint8_t ) * EXPORT_HANDLE_BUFFER_SIZE );

		if( attachment_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create attachment data.",
			 function );

			goto on_error;
		}
		while( attachment_data_size > 0 )
		{
			if( attachment_data_size >= EXPORT_HANDLE_BUFFER_SIZE )
			{
				read_size = EXPORT_HANDLE_BUFFER_SIZE;
			}
			else
			{
				read_size = (size_t) attachment_data_size;
			}
			attachment_data_size -= read_size;

			read_count = libpff_attachment_data_read_buffer(
				      attachment,
				      attachment_data,
				      read_size,
				      error );

			if( read_count != (ssize_t) read_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read attachment data.",
				 function );

				goto on_error;
			}
			write_count = file_stream_write(
				       attachment_file_stream,
				       attachment_data,
				       read_size );

			if( write_count != read_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write attachment data.",
				 function );

				goto on_error;
			}
		}
		memory_free(
		 attachment_data );

		attachment_data = NULL;
	}
	if( file_stream_close(
	     attachment_file_stream ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close attachment file.",
		 function );

		attachment_file_stream = NULL;

		goto on_error;
	}
	attachment_file_stream = NULL;

	return( 1 );

on_error:
	if( attachment_data != NULL )
	{
		memory_free(
		 attachment_data );
	}
	if( attachment_file_stream != NULL )
	{
		file_stream_close(
		 attachment_file_stream );
	}
	if( target_path != NULL )
	{
		memory_free(
		 target_path );
	}
	if( attachment_filename != NULL )
	{
		memory_free(
		 attachment_filename );
	}
	return( -1 );
}

/* Exports the attachment item
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_attachment_item(
     export_handle_t *export_handle,
     libpff_item_t *attachment,
     int attachment_index,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	libpff_item_t *attached_item            = NULL;
	system_character_t *attachment_filename = NULL;
	system_character_t *target_path         = NULL;
	static char *function                   = "export_handle_export_attachment_item";
	size_t attachment_filename_size         = 0;
	size_t target_path_size                 = 0;
	int print_count                         = 0;
	int result                              = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	attachment_filename = system_string_allocate(
	                       16 );

	if( attachment_filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create attachment filename.",
		 function );

		goto on_error;
	}
	print_count = system_string_sprintf(
		       attachment_filename,
		       16,
		       _SYSTEM_STRING( "Attachment%05d" ),
		       attachment_index + 1 );

	if( ( print_count < 0 )
	 || ( print_count > 16 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set attachment filename.",
		 function );

		goto on_error;
	}
	attachment_filename[ 15 ] = 0;
	attachment_filename_size  = 16;

	log_handle_printf(
	 log_handle,
	 "Missing attachment filename saving as: %" PRIs_SYSTEM "\n",
	 attachment_filename );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libcpath_path_join_wide(
	          &target_path,
	          &target_path_size,
	          export_path,
	          export_path_length,
	          attachment_filename,
	          attachment_filename_size - 1,
	          error );
#else
	result = libcpath_path_join(
	          &target_path,
	          &target_path_size,
	          export_path,
	          export_path_length,
	          attachment_filename,
	          attachment_filename_size - 1,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create target path.",
		 function );

		goto on_error;
	}
	memory_free(
	 attachment_filename );

	attachment_filename = NULL;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libcfile_file_exists_wide(
		  target_path,
	          error );
#else
	result = libcfile_file_exists(
		  target_path,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_GENERIC,
		 "%s: unable to determine if %" PRIs_SYSTEM " exists.",
		 function,
		 target_path );

		goto on_error;
	}
	else if( result != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_GENERIC,
		 "%s: %" PRIs_SYSTEM " already exists.",
		 function,
		 target_path );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcpath_path_make_directory_wide(
	     target_path,
	     error ) != 1 )
#else
	if( libcpath_path_make_directory(
	     target_path,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to make directory: %" PRIs_SYSTEM ".",
		 function,
		 target_path );

		goto on_error;
	}
	log_handle_printf(
	 log_handle,
	 "Created directory: %" PRIs_SYSTEM ".\n",
	 target_path );

	if( libpff_attachment_get_item(
	     attachment,
	     &attached_item,
	     error ) != 1 )
	{
		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );

		log_handle_printf(
		 log_handle,
		 "Missing attached item.\n" );
	}
	else
	{
		if( export_handle_export_item(
		     export_handle,
		     attached_item,
		     0,
		     1,
		     target_path,
		     target_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			fprintf(
			 export_handle->notify_stream,
			 "Unable to export attached item.\n" );

			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_OUTPUT,
			 LIBCERROR_OUTPUT_ERROR_GENERIC,
			 "%s: unable to export attached item.",
			 function );

			log_handle_printf(
			 log_handle,
			 "Unable to export attached item.\n" );
		}
	}
	memory_free(
	 target_path );

	target_path = NULL;

	if( libpff_item_free(
	     &attached_item,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free attached item.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( target_path != NULL )
	{
		memory_free(
		 target_path );
	}
	if( attachment_filename != NULL )
	{
		memory_free(
		 attachment_filename );
	}
	return( -1 );
}

/* Exports the recipients to a separate file
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_recipients(
     export_handle_t *export_handle,
     libpff_item_t *item,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	item_file_t *item_file       = NULL;
	libpff_item_t *recipients    = NULL;
	static char *function        = "export_handle_export_recipients";
	int number_of_recipients     = 0;
	int result                   = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item.",
		 function );

		return( -1 );
	}
	result = libpff_message_get_recipients(
	          item,
	          &recipients,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve recipients.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libpff_item_get_number_of_record_sets(
		     recipients,
		     &number_of_recipients,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to get number of recipients.",
			 function );

			goto on_error;
		}
		if( number_of_recipients > 0 )
		{
			if( export_handle->dump_item_values != 0 )
			{
				if( export_handle_export_item_values(
				     export_handle,
				     recipients,
				     _SYSTEM_STRING( "RecipientsItemValues.txt" ),
				     24,
				     export_path,
				     export_path_length,
				     log_handle,
				     error ) != 1 )
				{
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: unable to export item values.\n",
						 function );
					}
					if( ( error != NULL )
					 && ( *error != NULL ) )
					{
						libcnotify_print_error_backtrace(
						 *error );
					}
					libcerror_error_free(
					 error );

					log_handle_printf(
					 log_handle,
					 "Unable to export recipients item values.\n" );
				}
			}
			log_handle_printf(
			 log_handle,
			 "Saving %" PRIu32 "recipient(s) as: Recipients.txt\n",
			 number_of_recipients );

			result = export_handle_create_item_file(
				  export_handle,
				  export_path,
				  export_path_length,
				  _SYSTEM_STRING( "Recipients.txt" ),
				  14,
				  &item_file,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create recipients file.",
				 function );

				goto on_error;
			}
			else if( result == 0 )
			{
				log_handle_printf(
				 log_handle,
				 "Skipping recipients file it already exists.\n" );
			}
			else
			{
				if( export_handle_export_recipients_to_item_file(
				     export_handle,
				     recipients,
				     number_of_recipients,
				     item_file,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GENERIC,
					 "%s: unable to export recipients.",
					 function );

					goto on_error;
				}
			}
			if( item_file_close(
			     item_file,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close item file.",
				 function );

				goto on_error;
			}
			if( item_file_free(
			     &item_file,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free item file.",
				 function );

				goto on_error;
			}
		}
		if( libpff_item_free(
		     &recipients,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free recipients.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( item_file != NULL )
	{
		item_file_free(
		 &item_file,
		 NULL );
	}
	if( recipients != NULL )
	{
		libpff_item_free(
		 &recipients,
		 NULL );
	}
	return( 1 );
}

/* Exports the recipient type of a specific recipient to an item file
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_recipient_type_to_item_file(
     item_file_t *item_file,
     libpff_record_entry_t *record_entry,
     libcerror_error_t **error )
{
	system_character_t *value_string = NULL;
	static char *function            = "export_handle_export_recipient_type_to_item_file";
	size_t value_string_length       = 0;
	uint32_t value_32bit             = 0;
	int unknown_value                = 0;

	if( libpff_record_entry_get_data_as_32bit_integer(
	     record_entry,
	     &value_32bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve 32-bit integer value.",
		 function );

		return( -1 );
	}
	if( value_32bit == (uint32_t) LIBPFF_RECIPIENT_TYPE_ORIGINATOR )
	{
		value_string        = _SYSTEM_STRING( "Originator" );
		value_string_length = 10;
	}
	else if( value_32bit == (uint32_t) LIBPFF_RECIPIENT_TYPE_TO )
	{
		value_string        = _SYSTEM_STRING( "To" );
		value_string_length = 2;
	}
	else if( value_32bit == (uint32_t) LIBPFF_RECIPIENT_TYPE_CC )
	{
		value_string        = _SYSTEM_STRING( "CC" );
		value_string_length = 2;
	}
	else if( value_32bit == (uint32_t) LIBPFF_RECIPIENT_TYPE_BCC )
	{
		value_string        = _SYSTEM_STRING( "BCC" );
		value_string_length = 3;
	}
	else
	{
		value_string        = _SYSTEM_STRING( "Unknown (" );
		value_string_length = 9;
		unknown_value       = 1;
	}
	if( item_file_write_string(
	     item_file,
	     value_string,
	     value_string_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write string.",
		 function );

		return( -1 );
	}
	if( unknown_value != 0 )
	{
		if( item_file_write_integer_32bit_as_hexadecimal(
		     item_file,
		     value_32bit,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write 32-bit integer.",
			 function );

			return( -1 );
		}
		if( item_file_write_string(
		     item_file,
		     _SYSTEM_STRING( ")" ),
		     1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write string.",
			 function );

			return( -1 );
		}
	}
	if( item_file_write_new_line(
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write new line.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Exports the recipients to an item file
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_recipients_to_item_file(
     export_handle_t *export_handle,
     libpff_item_t *recipients,
     int number_of_recipients,
     item_file_t *item_file,
     libcerror_error_t **error )
{
	mapi_property_definitions_t property_definitions[ 5 ] = {
		{ _SYSTEM_STRING( "Display name:\t\t" ), LIBPFF_ENTRY_TYPE_DISPLAY_NAME, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Recipient display name:\t" ), LIBPFF_ENTRY_TYPE_RECIPIENT_DISPLAY_NAME, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Email address:\t\t" ), LIBPFF_ENTRY_TYPE_EMAIL_ADDRESS, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Address type:\t\t" ), LIBPFF_ENTRY_TYPE_ADDRESS_TYPE, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Recipient type:\t\t" ), LIBPFF_ENTRY_TYPE_RECIPIENT_TYPE, LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED, 0, &export_handle_export_recipient_type_to_item_file } };

	libpff_record_set_t *record_set                  = NULL;
	mapi_property_definitions_t *property_definition = NULL;
	static char *function                            = "export_handle_export_recipients_to_item_file";
	int property_definition_index                    = 0;
	int recipient_index                              = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( recipients == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid recipients.",
		 function );

		return( -1 );
	}
	if( export_handle->print_status_information != 0 )
	{
		if( number_of_recipients == 1 )
		{
			fprintf(
			 export_handle->notify_stream,
			 "Exporting recipient.\n" );
		}
		else
		{
			fprintf(
			 export_handle->notify_stream,
			 "Exporting %d recipients.\n",
			 number_of_recipients );
		}
	}
	for( recipient_index = 0;
	     recipient_index < number_of_recipients;
	     recipient_index++ )
	{
		if( libpff_item_get_record_set_by_index(
		     recipients,
		     recipient_index,
		     &record_set,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record set: %d from item.",
			 function,
			 recipient_index );

			goto on_error;
		}
/* TODO merge with export_handle_export_item_value_to_item_file ?
 * allow description to be NULL ?
 */
		for( property_definition_index = 0;
		     property_definition_index < 5;
		     property_definition_index++ )
		{
			property_definition = &( property_definitions[ property_definition_index ] );

			export_handle_write_record_set_value_to_item_file(
			 item_file,
			 property_definition->description,
			 record_set,
			 property_definition->entry_type,
			 property_definition->value_type,
			 property_definition->format_flags,
			 property_definition->write_to_item_file_function );
		}
		if( item_file_write_new_line(
		     item_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write new line.",
			 function );

			goto on_error;
		}
		if( libpff_record_set_free(
		     &record_set,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free record set: %d.",
			 function,
			 recipient_index );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( record_set != NULL )
	{
		libpff_record_set_free(
		 &record_set,
		 NULL );
	}
	return( -1 );
}

/* Exports the activity
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_activity(
     export_handle_t *export_handle,
     libpff_item_t *activity,
     int activity_index,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	item_file_t *item_file            = NULL;
	system_character_t *activity_path = NULL;
	static char *function             = "export_handle_export_activity";
	size_t activity_path_size         = 0;
	size_t plain_text_body_size       = 0;
	uint32_t identifier               = 0;
	int result                        = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( activity == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid activity.",
		 function );

		return( -1 );
	}
	if( export_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export path.",
		 function );

		return( -1 );
	}
	if( libpff_item_get_identifier(
	     activity,
	     &identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier.",
		 function );

		goto on_error;
	}
	log_handle_printf(
	 log_handle,
	 "Processing activity: %05d (identifier: %" PRIu32 ") in path: %" PRIs_SYSTEM "%c\n",
	 activity_index,
	 identifier,
	 export_path,
	 LIBCPATH_SEPARATOR );

	if( export_handle->export_mode == EXPORT_MODE_DEBUG )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: processing activity: %" PRIu32 "\n",
			 function,
			 identifier );
		}
	}
	if( export_handle_create_default_item_directory(
	     export_handle,
	     activity_index,
	     _SYSTEM_STRING( "Activity" ),
	     8,
	     export_path,
	     export_path_length,
	     &activity_path,
	     &activity_path_size,
	     log_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable create activity directory.",
		 function );

		goto on_error;
	}
	if( activity_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid activity directory path.",
		 function );

		goto on_error;
	}
	if( export_handle->dump_item_values != 0 )
	{
		if( export_handle_export_item_values(
		     export_handle,
		     activity,
		     _SYSTEM_STRING( "ItemValues.txt" ),
		     14,
		     activity_path,
		     activity_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export item values.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export activity item values.\n" );
		}
	}
	result = export_handle_create_item_file(
	          export_handle,
	          activity_path,
	          activity_path_size - 1,
	          _SYSTEM_STRING( "Activity.txt" ),
	          12,
	          &item_file,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create activity file.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		log_handle_printf(
		 log_handle,
		 "Skipping activity file it already exists.\n" );

		memory_free(
		 activity_path );

		return( 1 );
	}
	if( export_handle_export_message_header_to_item_file(
	     export_handle,
	     activity,
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to export message header.",
		 function );

		goto on_error;
	}
	if( item_file_write_value_description(
	     item_file,
	     _SYSTEM_STRING( "Activity:" ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write string.",
		 function );

		goto on_error;
	}
/* TODO determine the activity properties */

	result = libpff_message_get_plain_text_body_size(
	          activity,
	          &plain_text_body_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve plain text body size.",
		 function );

		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );
	}
	else if( result != 0 )
	{
		if( item_file_write_value_description(
		     item_file,
		     _SYSTEM_STRING( "\nBody:\n" ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write string.",
			 function );

			goto on_error;
		}
		if( export_handle_export_message_body_plain_text_to_item_file(
		     export_handle,
		     activity,
		     plain_text_body_size,
		     item_file,
		     error ) != 1 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export plain text message body.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export plain text message body.\n" );
		}
	}
	if( item_file_close(
	     item_file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close item file.",
		 function );

		goto on_error;
	}
	if( item_file_free(
	     &item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free item file.",
		 function );

		goto on_error;
	}
	if( export_handle_export_message_body(
	     export_handle,
	     activity,
	     activity_path,
	     activity_path_size - 1,
	     log_handle,
	     error ) != 1 )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unable to export message body.\n",
			 function );
		}
		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );

		log_handle_printf(
		 log_handle,
		 "Unable to export message body.\n" );
	}
	if( export_handle_export_recipients(
	     export_handle,
	     activity,
	     activity_path,
	     activity_path_size - 1,
	     log_handle,
	     error ) != 1 )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unable to export recipients.\n",
			 function );
		}
		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );

		log_handle_printf(
		 log_handle,
		 "Unable to export recipients.\n" );
	}
	if( export_handle->export_mode != EXPORT_MODE_ITEMS_NO_ATTACHMENTS )
	{
		if( export_handle_export_attachments(
		     export_handle,
		     activity,
		     activity_path,
		     activity_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export attachments.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export attachments.\n" );
		}
	}
	memory_free(
	 activity_path );

	return( 1 );

on_error:
	if( item_file != NULL )
	{
		item_file_free(
		 &item_file,
		 NULL );
	}
	if( activity_path != NULL )
	{
		memory_free(
		 activity_path );
	}
	return( -1 );
}

/* Exports the appointment
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_appointment(
     export_handle_t *export_handle,
     libpff_item_t *appointment,
     int appointment_index,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	mapi_property_definitions_t property_definitions[ 8 ] = {
		{ _SYSTEM_STRING( "Start time:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_APPOINTMENT_START_TIME, LIBPFF_VALUE_TYPE_FILETIME, 0, NULL },
		{ _SYSTEM_STRING( "End time:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_APPOINTMENT_END_TIME, LIBPFF_VALUE_TYPE_FILETIME, 0, NULL },
		{ _SYSTEM_STRING( "Duration:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_APPOINTMENT_DURATION, LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED, ITEM_FILE_FORMAT_FLAG_DURATION_IN_MINUTES, NULL },
		{ _SYSTEM_STRING( "Location:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_APPOINTMENT_LOCATION, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Recurrence pattern:\t\t\t" ), LIBPFF_ENTRY_TYPE_APPOINTMENT_RECURRENCE_PATTERN, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "First effective time:\t\t\t" ), LIBPFF_ENTRY_TYPE_APPOINTMENT_FIRST_EFFECTIVE_TIME, LIBPFF_VALUE_TYPE_FILETIME, 0, NULL },
		{ _SYSTEM_STRING( "Last effective time:\t\t\t" ), LIBPFF_ENTRY_TYPE_APPOINTMENT_LAST_EFFECTIVE_TIME, LIBPFF_VALUE_TYPE_FILETIME, 0, NULL },
/* TODO print human readable string */
		{ _SYSTEM_STRING( "Busy status:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_APPOINTMENT_BUSY_STATUS, LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED, ITEM_FILE_FORMAT_FLAG_HEXADECIMAL, NULL } };

	item_file_t *item_file               = NULL;
	system_character_t *appointment_path = NULL;
	static char *function                = "export_handle_export_appointment";
	size_t appointment_path_size         = 0;
	uint32_t identifier                  = 0;
	int result                           = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export path.",
		 function );

		return( -1 );
	}
	if( libpff_item_get_identifier(
	     appointment,
	     &identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier.",
		 function );

		goto on_error;
	}
	log_handle_printf(
	 log_handle,
	 "Processing appointment: %05d (identifier: %" PRIu32 ") in path: %" PRIs_SYSTEM "%c\n",
	 appointment_index,
	 identifier,
	 export_path,
	 LIBCPATH_SEPARATOR );

	if( export_handle->export_mode == EXPORT_MODE_DEBUG )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: processing appointment: %" PRIu32 "\n",
			 function,
			 identifier );
		}
	}
	if( export_handle_create_default_item_directory(
	     export_handle,
	     appointment_index,
	     _SYSTEM_STRING( "Appointment" ),
	     11,
	     export_path,
	     export_path_length,
	     &appointment_path,
	     &appointment_path_size,
	     log_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable create appointment directory.",
		 function );

		goto on_error;
	}
	if( export_handle->dump_item_values != 0 )
	{
		if( export_handle_export_item_values(
		     export_handle,
		     appointment,
		     _SYSTEM_STRING( "ItemValues.txt" ),
		     14,
		     appointment_path,
		     appointment_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export item values.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export appointment item values.\n" );
		}
	}
	result = export_handle_create_item_file(
	          export_handle,
	          appointment_path,
	          appointment_path_size - 1,
	          _SYSTEM_STRING( "Appointment.txt" ),
	          15,
	          &item_file,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create item file.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		log_handle_printf(
		 log_handle,
		 "Skipping appointment file it already exists.\n" );

		memory_free(
		 appointment_path );

		return( 1 );
	}
	if( export_handle_export_message_header_to_item_file(
	     export_handle,
	     appointment,
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to export message header.",
		 function );

		goto on_error;
	}
	if( export_handle_export_item_value_to_item_file(
	     export_handle,
	     item_file,
	     _SYSTEM_STRING( "Appointment:" ),
	     appointment,
	     0,
	     (mapi_property_definitions_t *) &property_definitions,
	     8,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to export appointment item values to item file.",
		 function );

		goto on_error;
	}
	if( item_file_close(
	     item_file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close item file.",
		 function );

		goto on_error;
	}
	if( item_file_free(
	     &item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free item file.",
		 function );

		goto on_error;
	}
	if( export_handle_export_recipients(
	     export_handle,
	     appointment,
	     appointment_path,
	     appointment_path_size - 1,
	     log_handle,
	     error ) != 1 )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unable to export recipients.\n",
			 function );
		}
		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );

		log_handle_printf(
		 log_handle,
		 "Unable to export recipients.\n" );
	}
	if( export_handle->export_mode != EXPORT_MODE_ITEMS_NO_ATTACHMENTS )
	{
		if( export_handle_export_attachments(
		     export_handle,
		     appointment,
		     appointment_path,
		     appointment_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export attachments.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export attachments.\n" );
		}
	}
	memory_free(
	 appointment_path );

	appointment_path = NULL;

	return( 1 );

on_error:
	if( item_file != NULL )
	{
		item_file_free(
		 &item_file,
		 NULL );
	}
	if( appointment_path != NULL )
	{
		memory_free(
		 appointment_path );
	}
	return( -1 );
}

/* Exports the contact
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_contact(
     export_handle_t *export_handle,
     libpff_item_t *contact,
     int contact_index,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	mapi_property_definitions_t property_definitions[ 23 ] = {
		{ _SYSTEM_STRING( "File under:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_ADDRESS_FILE_UNDER, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Given name:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_CONTACT_GIVEN_NAME, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Initials:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_CONTACT_INITIALS, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Surname:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_CONTACT_SURNAME, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Generational abbreviation:\t\t" ), LIBPFF_ENTRY_TYPE_CONTACT_GENERATIONAL_ABBREVIATION, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Title:\t\t\t\t\t" ), LIBPFF_ENTRY_TYPE_CONTACT_TITLE, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Callback phone number:\t\t" ), LIBPFF_ENTRY_TYPE_CONTACT_CALLBACK_PHONE_NUMBER, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Primary phone number:\t\t" ), LIBPFF_ENTRY_TYPE_CONTACT_PRIMARY_PHONE_NUMBER, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Home phone number:\t\t\t" ), LIBPFF_ENTRY_TYPE_CONTACT_HOME_PHONE_NUMBER, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Mobile phone number:\t\t\t" ), LIBPFF_ENTRY_TYPE_CONTACT_MOBILE_PHONE_NUMBER, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Company name:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_CONTACT_COMPANY_NAME, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Job title:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_CONTACT_JOB_TITLE, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Office location:\t\t\t" ), LIBPFF_ENTRY_TYPE_CONTACT_OFFICE_LOCATION, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Department name:\t\t\t" ), LIBPFF_ENTRY_TYPE_CONTACT_DEPARTMENT_NAME, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Postal address:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_CONTACT_POSTAL_ADDRESS, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Country:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_CONTACT_COUNTRY, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Locality:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_CONTACT_LOCALITY, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Business phone number 1:\t\t" ), LIBPFF_ENTRY_TYPE_CONTACT_BUSINESS_PHONE_NUMBER_1, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Business phone number 2:\t\t" ), LIBPFF_ENTRY_TYPE_CONTACT_BUSINESS_PHONE_NUMBER_2, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Business fax number:\t\t\t" ), LIBPFF_ENTRY_TYPE_CONTACT_BUSINESS_FAX_NUMBER, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Email address 1:\t\t\t" ), LIBPFF_ENTRY_TYPE_CONTACT_EMAIL_ADDRESS_1, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Email address 2:\t\t\t" ), LIBPFF_ENTRY_TYPE_CONTACT_EMAIL_ADDRESS_2, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Email address 3:\t\t\t" ), LIBPFF_ENTRY_TYPE_CONTACT_EMAIL_ADDRESS_3, LIBPFF_VALUE_TYPE_STRING, 0, NULL } };

	item_file_t *item_file           = NULL;
	system_character_t *contact_path = NULL;
	static char *function            = "export_handle_export_contact";
	size_t contact_path_size         = 0;
	size_t plain_text_body_size      = 0;
	uint32_t identifier              = 0;
	int result                       = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( contact == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid contact.",
		 function );

		return( -1 );
	}
	if( export_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export path.",
		 function );

		return( -1 );
	}
	if( libpff_item_get_identifier(
	     contact,
	     &identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier.",
		 function );

		goto on_error;
	}
	log_handle_printf(
	 log_handle,
	 "Processing contact: %05d (identifier: %" PRIu32 ") in path: %" PRIs_SYSTEM "%c\n",
	 contact_index,
	 identifier,
	 export_path,
	 LIBCPATH_SEPARATOR );

	if( export_handle->export_mode == EXPORT_MODE_DEBUG )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: processing contact: %" PRIu32 "\n",
			 function,
			 identifier );
		}
	}
	if( export_handle_create_default_item_directory(
	     export_handle,
	     contact_index,
	     _SYSTEM_STRING( "Contact" ),
	     7,
	     export_path,
	     export_path_length,
	     &contact_path,
	     &contact_path_size,
	     log_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable create contact directory.",
		 function );

		goto on_error;
	}
	if( export_handle->dump_item_values != 0 )
	{
		if( export_handle_export_item_values(
		     export_handle,
		     contact,
		     _SYSTEM_STRING( "ItemValues.txt" ),
		     14,
		     contact_path,
		     contact_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export item values.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export contact item values.\n" );
		}
	}
	result = export_handle_create_item_file(
	          export_handle,
	          contact_path,
	          contact_path_size - 1,
	          _SYSTEM_STRING( "Contact.txt" ),
	          11,
	          &item_file,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create item file.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		log_handle_printf(
		 log_handle,
		 "Skipping contact file it already exists.\n" );

		memory_free(
		 contact_path );

		return( 1 );
	}
	if( export_handle_export_message_header_to_item_file(
	     export_handle,
	     contact,
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to export message header.",
		 function );

		goto on_error;
	}
	if( export_handle_export_item_value_to_item_file(
	     export_handle,
	     item_file,
	     _SYSTEM_STRING( "Contact:" ),
	     contact,
	     0,
	     (mapi_property_definitions_t *) &property_definitions,
	     23,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to export contact item values to item file.",
		 function );

		goto on_error;
	}
	result = libpff_message_get_plain_text_body_size(
	          contact,
	          &plain_text_body_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve plain text body size.",
		 function );

		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );
	}
	else if( result != 0 )
	{
		if( item_file_write_value_description(
		     item_file,
		     _SYSTEM_STRING( "\nBody:\n" ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write string.",
			 function );

			goto on_error;
		}
		if( export_handle_export_message_body_plain_text_to_item_file(
		     export_handle,
		     contact,
		     plain_text_body_size,
		     item_file,
		     error ) != 1 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export plain text message body.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export plain text message body.\n" );
		}
	}
	if( item_file_close(
	     item_file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close item file.",
		 function );

		goto on_error;
	}
	if( item_file_free(
	     &item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free item file.",
		 function );

		goto on_error;
	}
	if( export_handle_export_message_body(
	     export_handle,
	     contact,
	     contact_path,
	     contact_path_size - 1,
	     log_handle,
	     error ) != 1 )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unable to export message body.\n",
			 function );
		}
		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );

		log_handle_printf(
		 log_handle,
		 "Unable to export message body.\n" );
	}
	if( export_handle_export_recipients(
	     export_handle,
	     contact,
	     contact_path,
	     contact_path_size - 1,
	     log_handle,
	     error ) != 1 )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unable to export recipients.\n",
			 function );
		}
		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );

		log_handle_printf(
		 log_handle,
		 "Unable to export recipients.\n" );
	}
	if( export_handle->export_mode != EXPORT_MODE_ITEMS_NO_ATTACHMENTS )
	{
		if( export_handle_export_attachments(
		     export_handle,
		     contact,
		     contact_path,
		     contact_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export attachments.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export attachments.\n" );
		}
	}
	memory_free(
	 contact_path );

	contact_path = NULL;

	return( 1 );

on_error:
	if( item_file != NULL )
	{
		item_file_free(
		 &item_file,
		 NULL );
	}
	if( contact_path != NULL )
	{
		memory_free(
		 contact_path );
	}
	return( -1 );
}

/* Exports the distribution list
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_distribution_list(
     export_handle_t *export_handle,
     libpff_item_t *distribution_list,
     int distribution_list_index,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	item_file_t *item_file                                 = NULL;
	libfmapi_entry_identifier_t *member_entry_identifier   = NULL;
	libfmapi_one_off_entry_identifier_t *member_identifier = NULL;
	libpff_multi_value_t *multi_value                      = NULL;
	libpff_record_entry_t *record_entry                    = NULL;
	libpff_record_set_t *record_set                        = NULL;
	system_character_t *distribution_list_path             = NULL;
	uint8_t *member_identifier_data                        = 0;
	static char *function                                  = "export_handle_export_distribution_list";
	size_t distribution_list_path_size                     = 0;
	size_t plain_text_body_size                            = 0;
	uint32_t identifier                                    = 0;
	int number_of_values                                   = 0;
	int result                                             = 0;
#ifdef TODO
	size_t member_identifier_data_size                     = 0;
	int value_index                                        = 0;
#endif

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( distribution_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid distribution list.",
		 function );

		return( -1 );
	}
	if( export_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export path.",
		 function );

		return( -1 );
	}
	if( libpff_item_get_identifier(
	     distribution_list,
	     &identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier.",
		 function );

		goto on_error;
	}
	log_handle_printf(
	 log_handle,
	 "Processing distribution list: %05d (identifier: %" PRIu32 ") in path: %" PRIs_SYSTEM "%c\n",
	 distribution_list_index,
	 identifier,
	 export_path,
	 LIBCPATH_SEPARATOR );

	if( export_handle->export_mode == EXPORT_MODE_DEBUG )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: processing distribution list: %" PRIu32 "\n",
			 function,
			 identifier );
		}
	}
	if( export_handle_create_default_item_directory(
	     export_handle,
	     distribution_list_index,
	     _SYSTEM_STRING( "DistributionList" ),
	     16,
	     export_path,
	     export_path_length,
	     &distribution_list_path,
	     &distribution_list_path_size,
	     log_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable create distribution list directory.",
		 function );

		goto on_error;
	}
	if( distribution_list_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid distribution list directory path.",
		 function );

		goto on_error;
	}
	if( export_handle->dump_item_values != 0 )
	{
		if( export_handle_export_item_values(
		     export_handle,
		     distribution_list,
		     _SYSTEM_STRING( "ItemValues.txt" ),
		     14,
		     distribution_list_path,
		     distribution_list_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export item values.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export distribution list item values.\n" );
		}
	}
	result = export_handle_create_item_file(
	          export_handle,
	          distribution_list_path,
	          distribution_list_path_size - 1,
	          _SYSTEM_STRING( "DistributionList.txt" ),
	          20,
	          &item_file,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create distribution list file.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		log_handle_printf(
		 log_handle,
		 "Skipping distribution list file it already exists.\n" );

		memory_free(
		 distribution_list_path );

		return( 1 );
	}
	if( export_handle_export_message_header_to_item_file(
	     export_handle,
	     distribution_list,
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to export message header.",
		 function );

		goto on_error;
	}
	if( item_file_write_value_description(
	     item_file,
	     _SYSTEM_STRING( "Distribution list:" ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write string.",
		 function );

		goto on_error;
	}
	if( item_file_write_item_value(
	     item_file,
	     distribution_list,
	     0,
	     LIBPFF_ENTRY_TYPE_DISTRIBUTION_LIST_NAME,
	     _SYSTEM_STRING( "Name:\t\t\t\t" ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write name.",
		 function );

#if defined( HAVE_DEBUG_OUTPUT )
		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
#endif
		libcerror_error_free(
		 error );
	}
/* TODO determine the distribution list properties */
	result = export_handle_item_get_record_entry_by_type(
	          export_handle,
	          distribution_list,
	          0,
	          LIBPFF_ENTRY_TYPE_DISTRIBUTION_LIST_MEMBER_ONE_OFF_ENTRY_IDENTIFIERS,
	          LIBPFF_VALUE_TYPE_UNSPECIFIED,
	          &record_set,
	          &record_entry,
	          LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve member one-off entry identifiers record entry: 0x%04" PRIx32 " from record set: 0.",
		 function,
		 LIBPFF_ENTRY_TYPE_DISTRIBUTION_LIST_MEMBER_ONE_OFF_ENTRY_IDENTIFIERS );

		goto on_error;
	}
	if( libpff_record_entry_get_multi_value(
	     record_entry,
	     &multi_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve multi-value from member one-off entry identifiers record entry.",
		 function );

		goto on_error;
	}
	if( libpff_multi_value_get_number_of_values(
	     multi_value,
	     &number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of member one-off entry identifiers.",
		 function );

		goto on_error;
	}
/* TODO work on distribution list support */
#ifdef TODO
	for( value_index = 0;
	     value_index < number_of_values;
	     value_index++ )
	{
		if( libpff_multi_value_get_value_binary_data_size(
		     multi_value,
		     value_index,
		     &member_identifier_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve member one-off entry identifier: %d data size.",
			 function,
			 value_index );

			goto on_error;
		}
		if( member_identifier_data_size > 0 )
		{
			member_identifier_data = (uint8_t *) memory_allocate(
			                                      sizeof( uint8_t ) * member_identifier_data_size );

			if( member_identifier_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create member identifier data.",
				 function );

				goto on_error;
			}
/* TODO refactor to separate function */
			if( libfmapi_entry_identifier_initialize(
			     &member_entry_identifier,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create member entry identifier: %d.",
				 function,
				 value_index );

				goto on_error;
			}
			if( libfmapi_entry_identifier_copy_from_byte_stream(
			     member_identifier,
			     member_identifier_data,
			     member_identifier_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy member entry identifier: %d from data.",
				 function,
				 value_index );

				goto on_error;
			}
/* TODO check service provider identifier */
			if( libfmapi_entry_identifier_free(
			     &member_entry_identifier,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free member entry identifier: %d.",
				 function,
				 value_index );

				goto on_error;
			}
			if( libfmapi_one_off_entry_identifier_initialize(
			     &member_identifier,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create member one-off entry identifier: %d.",
				 function,
				 value_index );

				goto on_error;
			}
			if( libfmapi_one_off_entry_identifier_copy_from_byte_stream(
			     member_identifier,
			     member_identifier_data,
			     member_identifier_data_size,
			     export_handle->ascii_codepage,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy member one-off entry identifier: %d from data.",
				 function,
				 value_index );

				goto on_error;
			}
/* TODO */
			if( libfmapi_one_off_entry_identifier_free(
			     &member_identifier,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free member one-off entry identifier: %d.",
				 function,
				 value_index );

				goto on_error;
			}
			memory_free(
			 member_identifier_data );

			member_identifier_data = NULL;
		}
	}
#endif
	if( libpff_multi_value_free(
	     &multi_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free multi value.",
		 function );

		goto on_error;
	}
	if( libpff_record_entry_free(
	     &record_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free record entry.",
		 function );

		goto on_error;
	}
	if( libpff_record_set_free(
	     &record_set,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free record set.",
		 function );

		goto on_error;
	}
	result = libpff_message_get_plain_text_body_size(
	          distribution_list,
	          &plain_text_body_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve plain text body size.",
		 function );

		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );
	}
	else if( result != 0 )
	{
		if( item_file_write_value_description(
		     item_file,
		     _SYSTEM_STRING( "\nBody:\n" ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write string.",
			 function );

			goto on_error;
		}
		if( export_handle_export_message_body_plain_text_to_item_file(
		     export_handle,
		     distribution_list,
		     plain_text_body_size,
		     item_file,
		     error ) != 1 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export plain text message body.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export plain text message body.\n" );
		}
	}
	if( item_file_close(
	     item_file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close item file.",
		 function );

		goto on_error;
	}
	if( item_file_free(
	     &item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free item file.",
		 function );

		goto on_error;
	}
	if( export_handle_export_message_body(
	     export_handle,
	     distribution_list,
	     distribution_list_path,
	     distribution_list_path_size - 1,
	     log_handle,
	     error ) != 1 )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unable to export message body.\n",
			 function );
		}
		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );

		log_handle_printf(
		 log_handle,
		 "Unable to export message body.\n" );
	}
	memory_free(
	 distribution_list_path );

	return( 1 );

on_error:
	if( member_identifier != NULL )
	{
		libfmapi_one_off_entry_identifier_free(
		 &member_identifier,
		 NULL );
	}
	if( member_identifier != NULL )
	{
		libfmapi_entry_identifier_free(
		 &member_entry_identifier,
		 NULL );
	}
	if( member_identifier_data != NULL )
	{
		memory_free(
		 member_identifier_data );
	}
	if( multi_value != NULL )
	{
		libpff_multi_value_free(
		 &multi_value,
		 NULL );
	}
	if( item_file != NULL )
	{
		item_file_free(
		 &item_file,
		 NULL );
	}
	if( distribution_list_path != NULL )
	{
		memory_free(
		 distribution_list_path );
	}
	return( -1 );
}

/* Exports the document
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_document(
     export_handle_t *export_handle,
     libpff_item_t *document,
     int document_index,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	item_file_t *item_file            = NULL;
	system_character_t *document_path = NULL;
	static char *function             = "export_handle_export_document";
	size_t document_path_size         = 0;
	uint32_t identifier               = 0;
	int result                        = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid document.",
		 function );

		return( -1 );
	}
	if( export_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export path.",
		 function );

		return( -1 );
	}
	if( libpff_item_get_identifier(
	     document,
	     &identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier.",
		 function );

		goto on_error;
	}
	log_handle_printf(
	 log_handle,
	 "Processing document: %05d (identifier: %" PRIu32 ") in path: %" PRIs_SYSTEM "%c\n",
	 document_index,
	 identifier,
	 export_path,
	 LIBCPATH_SEPARATOR );

	if( export_handle->export_mode == EXPORT_MODE_DEBUG )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: processing document: %" PRIu32 "\n",
			 function,
			 identifier );
		}
	}
	if( export_handle_create_default_item_directory(
	     export_handle,
	     document_index,
	     _SYSTEM_STRING( "Document" ),
	     8,
	     export_path,
	     export_path_length,
	     &document_path,
	     &document_path_size,
	     log_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable create document directory.",
		 function );

		goto on_error;
	}
	if( document_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid document directory path.",
		 function );

		goto on_error;
	}
	if( export_handle->dump_item_values != 0 )
	{
		if( export_handle_export_item_values(
		     export_handle,
		     document,
		     _SYSTEM_STRING( "ItemValues.txt" ),
		     14,
		     document_path,
		     document_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export item values.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export document item values.\n" );
		}
	}
	result = export_handle_create_item_file(
	          export_handle,
	          document_path,
	          document_path_size - 1,
	          _SYSTEM_STRING( "Document.txt" ),
	          12,
	          &item_file,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create item file.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		log_handle_printf(
		 log_handle,
		 "Skipping document file it already exists.\n" );

		memory_free(
		 document_path );

		return( 1 );
	}
	if( export_handle_export_message_header_to_item_file(
	     export_handle,
	     document,
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to export message header.",
		 function );

		goto on_error;
	}
/* TODO export document MAPI properties */
	if( item_file_close(
	     item_file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close item file.",
		 function );

		goto on_error;
	}
	if( item_file_free(
	     &item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free item file.",
		 function );

		goto on_error;
	}
	if( export_handle_export_recipients(
	     export_handle,
	     document,
	     document_path,
	     document_path_size - 1,
	     log_handle,
	     error ) != 1 )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unable to export recipients.\n",
			 function );
		}
		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );

		log_handle_printf(
		 log_handle,
		 "Unable to export recipients.\n" );
	}
	if( export_handle->export_mode != EXPORT_MODE_ITEMS_NO_ATTACHMENTS )
	{
		if( export_handle_export_attachments(
		     export_handle,
		     document,
		     document_path,
		     document_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export attachments.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export attachments.\n" );
		}
	}
	memory_free(
	 document_path );

	return( 1 );

on_error:
	if( item_file != NULL )
	{
		item_file_free(
		 &item_file,
		 NULL );
	}
	if( document_path != NULL )
	{
		memory_free(
		 document_path );
	}
	return( -1 );
}

/* Exports the email
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_email(
     export_handle_t *export_handle,
     libpff_item_t *email,
     int email_index,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	system_character_t *email_path    = NULL;
	uint8_t *email_filename           = NULL;
	static char *function             = "export_handle_export_email";
	size_t email_filename_size        = 0;
	size_t email_html_body_size       = 0;
	size_t email_path_size            = 0;
	size_t email_plain_text_body_size = 0;
	size_t email_rtf_body_size        = 0;
	uint32_t identifier               = 0;
	int export_format                 = 0;
	int has_html_body                 = 0;
	int has_rtf_body                  = 0;
	int has_text_body                 = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( export_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export path.",
		 function );

		return( -1 );
	}
	if( libpff_item_get_identifier(
	     email,
	     &identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier.",
		 function );

		goto on_error;
	}
	log_handle_printf(
	 log_handle,
	 "Processing email: %05d (identifier: %" PRIu32 ") in path: %" PRIs_SYSTEM "%c\n",
	 email_index,
	 identifier,
	 export_path,
	 LIBCPATH_SEPARATOR );

	if( export_handle->export_mode == EXPORT_MODE_DEBUG )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: processing email: %" PRIu32 "\n",
			 function,
			 identifier );
		}
	}
	if( export_handle_create_default_item_directory(
	     export_handle,
	     email_index,
	     _SYSTEM_STRING( "Message" ),
	     7,
	     export_path,
	     export_path_length,
	     &email_path,
	     &email_path_size,
	     log_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable create email directory.",
		 function );

		goto on_error;
	}
	if( email_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid email directory path.",
		 function );

		goto on_error;
	}
	if( export_handle->dump_item_values != 0 )
	{
		if( export_handle_export_item_values(
		     export_handle,
		     email,
		     _SYSTEM_STRING( "ItemValues.txt" ),
		     14,
		     email_path,
		     email_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export item values.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export email item values.\n" );
		}
	}
	/* Determine the available message body types
	 */
	has_html_body = libpff_message_get_html_body_size(
	                 email,
	                 &email_html_body_size,
	                 error );

	if( has_html_body == -1 )
	{
		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );
	}
	has_rtf_body = libpff_message_get_rtf_body_size(
	                email,
	                &email_rtf_body_size,
	                error );

	if( has_rtf_body == -1 )
	{
		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );
	}
	has_text_body = libpff_message_get_plain_text_body_size(
	                 email,
	                 &email_plain_text_body_size,
	                 error );

	if( has_text_body == -1 )
	{
		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );
	}
	/* Determine the message body export format
	 */
	if( export_handle->preferred_export_format == EXPORT_FORMAT_ALL )
	{
		export_format = EXPORT_FORMAT_ALL;
	}
	if( export_format == 0 )
	{
		if( ( has_text_body == 1 )
		 && ( email_plain_text_body_size > 0 ) )
		{
			if( export_handle->preferred_export_format == EXPORT_FORMAT_FTK )
			{
				export_format = EXPORT_FORMAT_FTK;
			}
			else if( export_handle->preferred_export_format == EXPORT_FORMAT_TEXT )
			{
				export_format = EXPORT_FORMAT_TEXT;
			}
		}
		if( ( has_html_body == 1 )
		 && ( email_html_body_size > 0 )
		 && ( export_handle->preferred_export_format == EXPORT_FORMAT_HTML ) )
		{
			export_format = EXPORT_FORMAT_HTML;
		}
		if( ( has_rtf_body == 1 )
		 && ( email_rtf_body_size > 0 )
		 && ( export_handle->preferred_export_format == EXPORT_FORMAT_RTF ) )
		{
			export_format = EXPORT_FORMAT_RTF;
		}
	}
	if( export_format == 0 )
	{
		if( ( has_text_body == 1 )
		 && ( email_plain_text_body_size > 0 ) )
		{
			export_format = EXPORT_FORMAT_TEXT;
		}
		else if( ( has_html_body == 1 )
		 && ( email_html_body_size > 0 ) )
		{
			export_format = EXPORT_FORMAT_HTML;
		}
		else if( ( has_rtf_body == 1 )
		 && ( email_rtf_body_size > 0 ) )
		{
			export_format = EXPORT_FORMAT_RTF;
		}
	}
	if( export_format == 0 )
	{
		if( has_text_body == 1 )
		{
			export_format = EXPORT_FORMAT_TEXT;
		}
		else if( has_html_body == 1 )
		{
			export_format = EXPORT_FORMAT_HTML;
		}
		else if( has_rtf_body == 1 )
		{
			export_format = EXPORT_FORMAT_RTF;
		}
	}
	if( export_format != EXPORT_FORMAT_FTK )
	{
		if( export_handle_export_message_header(
		     export_handle,
		     email,
		     email_path,
		     email_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export message header.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export message header.\n" );
		}
		if( export_handle_export_message_conversation_index(
		     export_handle,
		     email,
		     email_path,
		     email_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export message conversation index.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export message conversation index.\n" );
		}
	}
	if( export_format == EXPORT_FORMAT_FTK )
	{
		if( libpff_message_get_entry_value_utf8_string_size(
		     email,
		     LIBPFF_ENTRY_TYPE_EMAIL_EML_FILENAME,
		     &email_filename_size,
		     NULL ) == 1 )
		{
			if( email_filename_size > (size_t) SSIZE_MAX )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
				 "%s: email filename size value exceeds maximum.",
				 function );

				goto on_error;
			}
		}
		if( email_filename_size < 13 )
		{
			email_filename_size = 13;
		}
		email_filename = (uint8_t *) memory_allocate(
					      sizeof( uint8_t ) * email_filename_size );

		if( email_filename == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create email filename.",
			 function );

			goto on_error;
		}
		if( libpff_message_get_entry_value_utf8_string(
		     email,
		     LIBPFF_ENTRY_TYPE_EMAIL_EML_FILENAME,
		     email_filename,
		     email_filename_size,
		     NULL ) == 1 )
		{
			log_handle_printf(
			 log_handle,
			 "Email filename: %s\n",
			 (char *) email_filename );
		}
		if( export_handle_export_email_ftk(
		     export_handle,
		     email,
		     email_plain_text_body_size,
		     email_path,
		     email_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to export ftk email.",
			 function );

			goto on_error;
		}
		memory_free(
		 email_filename );

		email_filename = NULL;
	}
	else
	{
		if( export_handle_export_recipients(
		     export_handle,
		     email,
		     email_path,
		     email_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export recipients.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export recipients.\n" );
		}
		if( export_handle_export_message_transport_headers(
		     export_handle,
		     email,
		     email_path,
		     email_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to export transport headers.",
			 function );

			goto on_error;
		}
		if( export_handle_export_message_body(
		     export_handle,
		     email,
		     email_path,
		     email_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to export message body.",
			 function );

			goto on_error;
		}
	}
	if( export_handle->export_mode != EXPORT_MODE_ITEMS_NO_ATTACHMENTS )
	{
		/* Export the attachments
		 */
		if( export_handle_export_attachments(
		     export_handle,
		     email,
		     email_path,
		     email_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export attachments.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export attachments.\n" );
		}
	}
	memory_free(
	 email_path );

	return( 1 );

on_error:
	if( email_filename != NULL )
	{
		memory_free(
		 email_filename );
	}
	if( email_path != NULL )
	{
		memory_free(
		 email_path );
	}
	return( -1 );
}

/* Exports the email for ftk
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_email_ftk(
     export_handle_t *export_handle,
     libpff_item_t *email,
     size_t plain_text_body_size,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	system_character_t email_filename[ 12 ];

	item_file_t *item_file                = NULL;
	libpff_item_t *recipients             = NULL;
	static char *function                 = "export_handle_export_email_ftk";
	size_t email_filename_size            = 0;
	size_t message_transport_headers_size = 0;
	int number_of_recipients              = 0;
	int result                            = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( plain_text_body_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid email plain text body size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( export_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export path.",
		 function );

		return( -1 );
	}
	if( system_string_copy(
	     email_filename,
	     _SYSTEM_STRING( "Message.txt" ),
	     11 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set email filename.",
		 function );

		goto on_error;
	}
	email_filename[ 11 ] = 0;
	email_filename_size  = 12;

	log_handle_printf(
	 log_handle,
	 "Saving email as: %" PRIs_SYSTEM "\n",
	 email_filename );

	result = export_handle_create_item_file(
	          export_handle,
	          export_path,
	          export_path_length,
	          email_filename,
	          email_filename_size - 1,
	          &item_file,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create item file.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		log_handle_printf(
		 log_handle,
		 "Skipping email file: %" PRIs_SYSTEM " it already exists.",
		 email_filename );

		return( 1 );
	}
	if( export_handle_export_message_header_to_item_file(
	     export_handle,
	     email,
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to export message header.",
		 function );

		goto on_error;
	}
	if( export_handle_export_message_conversation_index_to_item_file(
	     export_handle,
	     item_file,
	     email,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to export message conversation index.",
		 function );

		goto on_error;
	}
	/* Export the recipients
	 */
	result = libpff_message_get_recipients(
	          email,
	          &recipients,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve recipients.",
		 function );

		goto on_error;
	}
	else if( result == 1 )
	{
		if( libpff_item_get_number_of_record_sets(
		     recipients,
		     &number_of_recipients,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to get number of recipients.",
			 function );

			goto on_error;
		}
		if( number_of_recipients > 0 )
		{
			if( item_file_write_value_description(
			     item_file,
			     _SYSTEM_STRING( "Recipients:" ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_WRITE_FAILED,
				 "%s: unable to write string.",
				 function );

				goto on_error;
			}
			if( export_handle_export_recipients_to_item_file(
			     export_handle,
			     recipients,
			     number_of_recipients,
			     item_file,
			     error ) != 1 )
			{
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: unable to export recipients.\n",
					 function );
				}
				if( ( error != NULL )
				 && ( *error != NULL ) )
				{
					libcnotify_print_error_backtrace(
					 *error );
				}
				libcerror_error_free(
				 error );

				log_handle_printf(
				 log_handle,
				 "Unable to export recipients.\n" );
			}
		}
		if( libpff_item_free(
		     &recipients,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free recipients.",
			 function );

			goto on_error;
		}
	}
	/* Determine the message transport headers size
	 */
	result = libpff_message_get_entry_value_utf8_string_size(
	          email,
	          LIBPFF_ENTRY_TYPE_MESSAGE_TRANSPORT_HEADERS,
	          &message_transport_headers_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable determine transport headers size.",
		 function );

		goto on_error;
	}
	if( message_transport_headers_size > 0 )
	{
		if( item_file_write_value_description(
		     item_file,
		     _SYSTEM_STRING( "Internet headers:" ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write string.",
			 function );

			goto on_error;
		}
		if( export_handle_export_message_transport_headers_to_item_file(
		     export_handle,
		     email,
		     message_transport_headers_size,
		     item_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to export message transport headers.",
			 function );

			goto on_error;
		}
	}
	if( export_handle_export_message_body_plain_text_to_item_file(
	     export_handle,
	     email,
	     plain_text_body_size,
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to export plain text message body.",
		 function );

		goto on_error;
	}
	if( item_file_close(
	     item_file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close item file.",
		 function );

		goto on_error;
	}
	if( item_file_free(
	     &item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free item file.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( recipients != NULL )
	{
		libpff_item_free(
		 &recipients,
		 NULL );
	}
	if( item_file != NULL )
	{
		item_file_free(
		 &item_file,
		 NULL );
	}
	return( -1 );
}

/* Exports the meeting
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_meeting(
     export_handle_t *export_handle,
     libpff_item_t *meeting,
     int meeting_index,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	item_file_t *item_file           = NULL;
	system_character_t *meeting_path = NULL;
	static char *function            = "export_handle_export_meeting";
	size_t meeting_path_size         = 0;
	size_t plain_text_body_size      = 0;
	uint32_t identifier              = 0;
	int result                       = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( meeting == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid meeting.",
		 function );

		return( -1 );
	}
	if( export_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export path.",
		 function );

		return( -1 );
	}
	if( libpff_item_get_identifier(
	     meeting,
	     &identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier.",
		 function );

		goto on_error;
	}
	log_handle_printf(
	 log_handle,
	 "Processing meeting: %05d (identifier: %" PRIu32 ") in path: %" PRIs_SYSTEM "%c\n",
	 meeting_index,
	 identifier,
	 export_path,
	 LIBCPATH_SEPARATOR );

	if( export_handle->export_mode == EXPORT_MODE_DEBUG )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: processing meeting: %" PRIu32 "\n",
			 function,
			 identifier );
		}
	}
	if( export_handle_create_default_item_directory(
	     export_handle,
	     meeting_index,
	     _SYSTEM_STRING( "Meeting" ),
	     7,
	     export_path,
	     export_path_length,
	     &meeting_path,
	     &meeting_path_size,
	     log_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable create meeting directory.",
		 function );

		goto on_error;
	}
	if( meeting_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid meeting directory path.",
		 function );

		goto on_error;
	}
	if( export_handle->dump_item_values != 0 )
	{
		if( export_handle_export_item_values(
		     export_handle,
		     meeting,
		     _SYSTEM_STRING( "ItemValues.txt" ),
		     14,
		     meeting_path,
		     meeting_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export item values.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export meeting item values.\n" );
		}
	}
	result = export_handle_create_item_file(
	          export_handle,
	          meeting_path,
	          meeting_path_size - 1,
	          _SYSTEM_STRING( "Meeting.txt" ),
	          11,
	          &item_file,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create item file.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		log_handle_printf(
		 log_handle,
		 "Skipping meeting file it already exists.\n" );

		memory_free(
		 meeting_path );

		return( 1 );
	}
	if( export_handle_export_message_header_to_item_file(
	     export_handle,
	     meeting,
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to export message header.",
		 function );

		goto on_error;
	}
	if( item_file_write_value_description(
	     item_file,
	     _SYSTEM_STRING( "Meeting:" ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write string.",
		 function );

		goto on_error;
	}
/* TODO determine the meeting properties */

	result = libpff_message_get_plain_text_body_size(
	          meeting,
	          &plain_text_body_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve plain text body size.",
		 function );

		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );
	}
	else if( result != 0 )
	{
		if( item_file_write_value_description(
		     item_file,
		     _SYSTEM_STRING( "\nBody:\n" ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write string.",
			 function );

			goto on_error;
		}
		if( export_handle_export_message_body_plain_text_to_item_file(
		     export_handle,
		     meeting,
		     plain_text_body_size,
		     item_file,
		     error ) != 1 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export plain text message body.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export plain text message body.\n" );
		}
	}
	if( item_file_close(
	     item_file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close item file.",
		 function );

		goto on_error;
	}
	if( item_file_free(
	     &item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free item file.",
		 function );

		goto on_error;
	}
	if( export_handle_export_message_body(
	     export_handle,
	     meeting,
	     meeting_path,
	     meeting_path_size - 1,
	     log_handle,
	     error ) != 1 )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unable to export message body.\n",
			 function );
		}
		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );

		log_handle_printf(
		 log_handle,
		 "Unable to export message body.\n" );
	}
	memory_free(
	 meeting_path );

	meeting_path = NULL;

	return( 1 );

on_error:
	if( item_file != NULL )
	{
		item_file_free(
		 &item_file,
		 NULL );
	}
	if( meeting_path != NULL )
	{
		memory_free(
		 meeting_path );
	}
	return( -1 );
}

/* Exports the note
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_note(
     export_handle_t *export_handle,
     libpff_item_t *note,
     int note_index,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	item_file_t *item_file        = NULL;
	system_character_t *note_path = NULL;
	static char *function         = "export_handle_export_note";
	size_t note_path_size         = 0;
	size_t plain_text_body_size   = 0;
	uint32_t identifier           = 0;
	int result                    = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( note == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid note.",
		 function );

		return( -1 );
	}
	if( export_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export path.",
		 function );

		return( -1 );
	}
	if( libpff_item_get_identifier(
	     note,
	     &identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier.",
		 function );

		goto on_error;
	}
	log_handle_printf(
	 log_handle,
	 "Processing note: %05d (identifier: %" PRIu32 ") in path: %" PRIs_SYSTEM "%c\n",
	 note_index,
	 identifier,
	 export_path,
	 LIBCPATH_SEPARATOR );

	if( export_handle->export_mode == EXPORT_MODE_DEBUG )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: processing note: %" PRIu32 "\n",
			 function,
			 identifier );
		}
	}
	if( export_handle_create_default_item_directory(
	     export_handle,
	     note_index,
	     _SYSTEM_STRING( "Note" ),
	     4,
	     export_path,
	     export_path_length,
	     &note_path,
	     &note_path_size,
	     log_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable create note directory.",
		 function );

		goto on_error;
	}
	if( note_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid note directory path.",
		 function );

		goto on_error;
	}
	if( export_handle->dump_item_values != 0 )
	{
		if( export_handle_export_item_values(
		     export_handle,
		     note,
		     _SYSTEM_STRING( "ItemValues.txt" ),
		     14,
		     note_path,
		     note_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export item values.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export note item values.\n" );
		}
	}
	result = export_handle_create_item_file(
	          export_handle,
	          note_path,
	          note_path_size - 1,
	          _SYSTEM_STRING( "Note.txt" ),
	          8,
	          &item_file,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create note file.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		log_handle_printf(
		 log_handle,
		 "Skipping note file it already exists.\n" );

		memory_free(
		 note_path );

		return( 1 );
	}
	if( export_handle_export_message_header_to_item_file(
	     export_handle,
	     note,
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to export message header.",
		 function );

		goto on_error;
	}
	if( item_file_write_value_description(
	     item_file,
	     _SYSTEM_STRING( "Note:" ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write string.",
		 function );

		goto on_error;
	}
/* TODO determine the note properties */

	result = libpff_message_get_plain_text_body_size(
	          note,
	          &plain_text_body_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve plain text body size.",
		 function );

		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );
	}
	else if( result != 0 )
	{
		if( item_file_write_value_description(
		     item_file,
		     _SYSTEM_STRING( "\nBody:\n" ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write string.",
			 function );

			goto on_error;
		}
		if( export_handle_export_message_body_plain_text_to_item_file(
		     export_handle,
		     note,
		     plain_text_body_size,
		     item_file,
		     error ) != 1 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export plain text message body.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export plain text message body.\n" );
		}
	}
	if( item_file_close(
	     item_file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close item file.",
		 function );

		goto on_error;
	}
	if( item_file_free(
	     &item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free item file.",
		 function );

		goto on_error;
	}
	if( export_handle_export_message_body(
	     export_handle,
	     note,
	     note_path,
	     note_path_size - 1,
	     log_handle,
	     error ) != 1 )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unable to export message body.\n",
			 function );
		}
		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );

		log_handle_printf(
		 log_handle,
		 "Unable to export message body.\n" );
	}
	if( export_handle_export_recipients(
	     export_handle,
	     note,
	     note_path,
	     note_path_size - 1,
	     log_handle,
	     error ) != 1 )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unable to export recipients.\n",
			 function );
		}
		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );

		log_handle_printf(
		 log_handle,
		 "Unable to export recipients.\n" );
	}
	if( export_handle->export_mode != EXPORT_MODE_ITEMS_NO_ATTACHMENTS )
	{
		/* Export the attachments
		 */
		if( export_handle_export_attachments(
		     export_handle,
		     note,
		     note_path,
		     note_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export attachments.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export attachments.\n" );
		}
	}
	memory_free(
	 note_path );

	return( 1 );

on_error:
	if( item_file != NULL )
	{
		item_file_free(
		 &item_file,
		 NULL );
	}
	if( note_path != NULL )
	{
		memory_free(
		 note_path );
	}
	return( -1 );
}

/* Exports the RSS feed
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_rss_feed(
     export_handle_t *export_handle,
     libpff_item_t *rss_feed,
     int rss_feed_index,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	item_file_t *item_file            = NULL;
	system_character_t *rss_feed_path = NULL;
	static char *function             = "export_handle_export_rss_feed";
	size_t plain_text_body_size       = 0;
	size_t rss_feed_path_size         = 0;
	uint32_t identifier               = 0;
	int result                        = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( rss_feed == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid RSS feed.",
		 function );

		return( -1 );
	}
	if( export_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export path.",
		 function );

		return( -1 );
	}
	if( libpff_item_get_identifier(
	     rss_feed,
	     &identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier.",
		 function );

		goto on_error;
	}
	log_handle_printf(
	 log_handle,
	 "Processing RSS feed: %05d (identifier: %" PRIu32 ") in path: %" PRIs_SYSTEM "%c\n",
	 rss_feed_index,
	 identifier,
	 export_path,
	 LIBCPATH_SEPARATOR );

	if( export_handle->export_mode == EXPORT_MODE_DEBUG )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: processing RSS feed: %" PRIu32 "\n",
			 function,
			 identifier );
		}
	}
	if( export_handle_create_default_item_directory(
	     export_handle,
	     rss_feed_index,
	     _SYSTEM_STRING( "Feed" ),
	     4,
	     export_path,
	     export_path_length,
	     &rss_feed_path,
	     &rss_feed_path_size,
	     log_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable create RSS feed directory.",
		 function );

		goto on_error;
	}
	if( rss_feed_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid RSS feed directory path.",
		 function );

		goto on_error;
	}
	if( export_handle->dump_item_values != 0 )
	{
		if( export_handle_export_item_values(
		     export_handle,
		     rss_feed,
		     _SYSTEM_STRING( "ItemValues.txt" ),
		     14,
		     rss_feed_path,
		     rss_feed_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export item values.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export RSS feed item values.\n" );
		}
	}
	result = export_handle_create_item_file(
	          export_handle,
	          rss_feed_path,
	          rss_feed_path_size - 1,
	          _SYSTEM_STRING( "Feed.txt" ),
	          8,
	          &item_file,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create RSS feed file.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		log_handle_printf(
		 log_handle,
		 "Skipping RSS feed file it already exists.\n" );

		memory_free(
		 rss_feed_path );

		return( 1 );
	}
	if( export_handle_export_message_header_to_item_file(
	     export_handle,
	     rss_feed,
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to export message header.",
		 function );

		goto on_error;
	}
	if( item_file_write_value_description(
	     item_file,
	     _SYSTEM_STRING( "RSS feed:" ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write string.",
		 function );

		goto on_error;
	}
/* TODO determine the RSS feed properties */

	result = libpff_message_get_plain_text_body_size(
	          rss_feed,
	          &plain_text_body_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve plain text body size.",
		 function );

		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );
	}
	else if( result != 0 )
	{
		if( item_file_write_value_description(
		     item_file,
		     _SYSTEM_STRING( "\nBody:\n" ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write string.",
			 function );

			goto on_error;
		}
		if( export_handle_export_message_body_plain_text_to_item_file(
		     export_handle,
		     rss_feed,
		     plain_text_body_size,
		     item_file,
		     error ) != 1 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export plain text message body.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export plain text message body.\n" );
		}
	}
	if( item_file_close(
	     item_file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close item file.",
		 function );

		goto on_error;
	}
	if( item_file_free(
	     &item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free item file.",
		 function );

		goto on_error;
	}
	if( export_handle_export_message_body(
	     export_handle,
	     rss_feed,
	     rss_feed_path,
	     rss_feed_path_size - 1,
	     log_handle,
	     error ) != 1 )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unable to export message body.\n",
			 function );
		}
		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );

		log_handle_printf(
		 log_handle,
		 "Unable to export message body.\n" );
	}
	if( export_handle_export_recipients(
	     export_handle,
	     rss_feed,
	     rss_feed_path,
	     rss_feed_path_size - 1,
	     log_handle,
	     error ) != 1 )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unable to export recipients.\n",
			 function );
		}
		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );

		log_handle_printf(
		 log_handle,
		 "Unable to export recipients.\n" );
	}
	if( export_handle->export_mode != EXPORT_MODE_ITEMS_NO_ATTACHMENTS )
	{
		if( export_handle_export_attachments(
		     export_handle,
		     rss_feed,
		     rss_feed_path,
		     rss_feed_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export attachments.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export attachments.\n" );
		}
	}
	memory_free(
	 rss_feed_path );

	return( 1 );

on_error:
	if( item_file != NULL )
	{
		item_file_free(
		 &item_file,
		 NULL );
	}
	if( rss_feed_path != NULL )
	{
		memory_free(
		 rss_feed_path );
	}
	return( -1 );
}

/* Exports the task
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_task(
     export_handle_t *export_handle,
     libpff_item_t *task,
     int task_index,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	mapi_property_definitions_t property_definitions[ 9 ] = {
		{ _SYSTEM_STRING( "Email address 3:\t\t\t" ), LIBPFF_ENTRY_TYPE_CONTACT_EMAIL_ADDRESS_3, LIBPFF_VALUE_TYPE_STRING, 0, NULL },
		{ _SYSTEM_STRING( "Start date:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_TASK_START_DATE, LIBPFF_VALUE_TYPE_FILETIME, 0, NULL },
		{ _SYSTEM_STRING( "Due date:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_TASK_DUE_DATE, LIBPFF_VALUE_TYPE_FILETIME, 0, NULL },
		{ _SYSTEM_STRING( "Status:\t\t\t\t\t" ), LIBPFF_ENTRY_TYPE_TASK_STATUS, LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED, ITEM_FILE_FORMAT_FLAG_HEXADECIMAL, NULL },
		{ _SYSTEM_STRING( "Percentage complete:\t\t\t" ), LIBPFF_ENTRY_TYPE_TASK_PERCENTAGE_COMPLETE, LIBPFF_VALUE_TYPE_FLOATING_POINT, 0, NULL },
		{ _SYSTEM_STRING( "Actual effort:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_TASK_ACTUAL_EFFORT, LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED, ITEM_FILE_FORMAT_FLAG_DURATION_IN_MINUTES, NULL },
		{ _SYSTEM_STRING( "Total effort:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_TASK_TOTAL_EFFORT, LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED, ITEM_FILE_FORMAT_FLAG_DURATION_IN_MINUTES, NULL },
		{ _SYSTEM_STRING( "Is complete:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_TASK_IS_COMPLETE, LIBPFF_VALUE_TYPE_BOOLEAN, 0, NULL },
		{ _SYSTEM_STRING( "Version:\t\t\t\t" ), LIBPFF_ENTRY_TYPE_TASK_VERSION, LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED, 0, NULL } };

	item_file_t *item_file        = NULL;
	system_character_t *task_path = NULL;
	static char *function         = "export_handle_export_task";
	size_t plain_text_body_size   = 0;
	size_t task_path_size         = 0;
	uint32_t identifier           = 0;
	int result                    = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( task == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid task.",
		 function );

		return( -1 );
	}
	if( export_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export path.",
		 function );

		return( -1 );
	}
	if( libpff_item_get_identifier(
	     task,
	     &identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier.",
		 function );

		goto on_error;
	}
	log_handle_printf(
	 log_handle,
	 "Processing task: %05d (identifier: %" PRIu32 ") in path: %" PRIs_SYSTEM "%c\n",
	 task_index,
	 identifier,
	 export_path,
	 LIBCPATH_SEPARATOR );

	if( export_handle->export_mode == EXPORT_MODE_DEBUG )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: processing task: %" PRIu32 "\n",
			 function,
			 identifier );
		}
	}
	if( export_handle_create_default_item_directory(
	     export_handle,
	     task_index,
	     _SYSTEM_STRING( "Task" ),
	     4,
	     export_path,
	     export_path_length,
	     &task_path,
	     &task_path_size,
	     log_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable create task directory.",
		 function );

		goto on_error;
	}
	if( task_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid task directory path.",
		 function );

		goto on_error;
	}
	if( export_handle->dump_item_values != 0 )
	{
		if( export_handle_export_item_values(
		     export_handle,
		     task,
		     _SYSTEM_STRING( "ItemValues.txt" ),
		     14,
		     task_path,
		     task_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export item values.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export task item values.\n" );
		}
	}
	result = export_handle_create_item_file(
	          export_handle,
	          task_path,
	          task_path_size - 1,
	          _SYSTEM_STRING( "Task.txt" ),
	          8,
	          &item_file,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create item file.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		log_handle_printf(
		 log_handle,
		 "Skipping task file it already exists.\n" );

		memory_free(
		 task_path );

		return( 1 );
	}
	if( export_handle_export_message_header_to_item_file(
	     export_handle,
	     task,
	     item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to export message header.",
		 function );

		goto on_error;
	}
	if( export_handle_export_item_value_to_item_file(
	     export_handle,
	     item_file,
	     _SYSTEM_STRING( "Task:" ),
	     task,
	     0,
	     (mapi_property_definitions_t *) &property_definitions,
	     9,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to export task item values to item file.",
		 function );

		goto on_error;
	}
	result = libpff_message_get_plain_text_body_size(
	          task,
	          &plain_text_body_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve plain text body size.",
		 function );

		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );
	}
	else if( result != 0 )
	{
		if( item_file_write_value_description(
		     item_file,
		     _SYSTEM_STRING( "\nBody:\n" ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write string.",
			 function );

			goto on_error;
		}
		if( export_handle_export_message_body_plain_text_to_item_file(
		     export_handle,
		     task,
		     plain_text_body_size,
		     item_file,
		     error ) != 1 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export plain text message body.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export plain text message body.\n" );
		}
	}
	if( item_file_close(
	     item_file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close item file.",
		 function );

		goto on_error;
	}
	if( item_file_free(
	     &item_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free item file.",
		 function );

		goto on_error;
	}
	if( export_handle_export_message_body(
	     export_handle,
	     task,
	     task_path,
	     task_path_size - 1,
	     log_handle,
	     error ) != 1 )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unable to export message body.\n",
			 function );
		}
		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );

		log_handle_printf(
		 log_handle,
		 "Unable to export message body.\n" );
	}
	if( export_handle_export_recipients(
	     export_handle,
	     task,
	     task_path,
	     task_path_size - 1,
	     log_handle,
	     error ) != 1 )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unable to export recipients.\n",
			 function );
		}
		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );

		log_handle_printf(
		 log_handle,
		 "Unable to export recipients.\n" );
	}
	if( export_handle->export_mode != EXPORT_MODE_ITEMS_NO_ATTACHMENTS )
	{
		if( export_handle_export_attachments(
		     export_handle,
		     task,
		     task_path,
		     task_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export attachments.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export attachments.\n" );
		}
	}
	memory_free(
	 task_path );

	task_path = NULL;

	return( 1 );

on_error:
	if( item_file != NULL )
	{
		item_file_free(
		 &item_file,
		 NULL );
	}
	if( task_path != NULL )
	{
		memory_free(
		 task_path );
	}
	return( -1 );
}

/* Retrieves a folder name
 * If no folder name is found default to "Folder%05d"
 * Returns 1 if successful or -1 on error
 */
int export_handle_get_folder_name(
     export_handle_t *export_handle,
     libpff_item_t *folder,
     int folder_index,
     system_character_t **folder_name,
     size_t *folder_name_size,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	system_character_t *name           = NULL;
	system_character_t *sanitized_name = NULL;
	static char *function              = "export_handle_get_folder_name";
	size_t name_length                 = 0;
	size_t name_size                   = 0;
	size_t sanitized_name_size         = 0;
	int print_count                    = 0;
	int result                         = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( folder_name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder name.",
		 function );

		return( -1 );
	}
	if( *folder_name != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid folder name value already set.",
		 function );

		return( -1 );
	}
	if( folder_name_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder name size.",
		 function );

		return( -1 );
	}
	result = export_handle_item_create_value_string_by_type(
	          export_handle,
	          folder,
	          0,
	          LIBPFF_ENTRY_TYPE_DISPLAY_NAME,
	          &name,
	          &name_size,
	          NULL );

	if( result == 1 )
	{
		name_length = system_string_length(
		               name );

		if( name_length == 0 )
		{
			result = 0;
		}
		else
		{
			log_handle_printf(
			 log_handle,
			 "Saving folder with name: %" PRIs_SYSTEM "",
			 name );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			if( libcpath_path_get_sanitized_filename_wide(
			     name,
			     name_length,
			     &sanitized_name,
			     &sanitized_name_size,
			     error ) != 1 )
#else
			if( libcpath_path_get_sanitized_filename(
			     name,
			     name_length,
			     &sanitized_name,
			     &sanitized_name_size,
			     error ) != 1 )
#endif
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable sanitize folder name.",
				 function );

				goto on_error;
			}
			memory_free(
			 name );

			name                = sanitized_name;
			name_size           = sanitized_name_size;
			sanitized_name      = NULL;
			sanitized_name_size = 0;

			log_handle_printf(
			 log_handle,
			 " as: %" PRIs_SYSTEM "\n",
			 name );
		}
	}
	if( result != 1 )
	{
		if( ( name == NULL )
		 || ( name_size < 12 ) )
		{
			if( name != NULL )
			{
				memory_free(
				 name );
			}
			name_size = 12;

			name = system_string_allocate(
			        name_size );

			if( name == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create folder name.",
				 function );

				goto on_error;
			}
		}
		print_count = system_string_sprintf(
		               name,
		               12,
		               _SYSTEM_STRING( "Folder%05d" ),
		               folder_index + 1 );

		if( ( print_count < 0 )
		 || ( print_count > 12 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set folder name.",
			 function );

			goto on_error;
		}
		( name )[ 11 ] = 0;

		log_handle_printf(
		 log_handle,
		 "Missing folder name defaulting to: %" PRIs_SYSTEM "\n",
		 name );
	}
	*folder_name      = name;
	*folder_name_size = name_size;

	return( 1 );

on_error:
	if( sanitized_name != NULL )
	{
		memory_free(
		 sanitized_name );
	}
	if( name != NULL )
	{
		memory_free(
		 name );
	}
	return( -1 );
}

/* Exports the folder
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_folder(
     export_handle_t *export_handle,
     libpff_item_t *folder,
     int folder_index,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	system_character_t *folder_name = NULL;
	system_character_t *target_path = NULL;
	static char *function           = "export_handle_export_folder";
	size_t folder_name_size         = 0;
	size_t target_path_size         = 0;
	uint32_t identifier             = 0;
	int print_count                 = 0;
	int result                      = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	if( export_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export path.",
		 function );

		return( -1 );
	}
	if( libpff_item_get_identifier(
	     folder,
	     &identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier.",
		 function );

		goto on_error;
	}
	log_handle_printf(
	 log_handle,
	 "Processing folder: %05d (identifier: %" PRIu32 ") in path: %" PRIs_SYSTEM "%c\n",
	 folder_index,
	 identifier,
	 export_path,
	 LIBCPATH_SEPARATOR );

	if( export_handle->export_mode == EXPORT_MODE_DEBUG )
	{
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: processing folder: %" PRIu32 "\n",
			 function,
			 identifier );
		}
	}
	/* Create the folder directory
	 */
	if( export_handle_get_folder_name(
	     export_handle,
	     folder,
	     folder_index,
	     &folder_name,
	     &folder_name_size,
	     log_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable retrieve folder name: %d.",
		 function,
		 folder_index );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libcpath_path_join_wide(
	          &target_path,
	          &target_path_size,
	          export_path,
	          export_path_length,
	          folder_name,
	          folder_name_size - 1,
	          error );
#else
	result = libcpath_path_join(
	          &target_path,
	          &target_path_size,
	          export_path,
	          export_path_length,
	          folder_name,
	          folder_name_size - 1,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create target path.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libcfile_file_exists_wide(
	          target_path,
	          error );
#else
	result = libcfile_file_exists(
	          target_path,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_GENERIC,
		 "%s: unable to determine if %" PRIs_SYSTEM " exists.",
		 function,
		 target_path );

		goto on_error;
	}
	else if( result != 0 )
	{
		memory_free(
		 target_path );

		target_path = NULL;

		print_count = system_string_sprintf(
		               folder_name,
		               12,
		               _SYSTEM_STRING( "Folder%05d" ),
		               folder_index + 1 );

		if( ( print_count < 0 )
		 || ( print_count > 12 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set folder name.",
			 function );

			goto on_error;
		}
		folder_name[ 11 ] = 0;
		folder_name_size  = 12;

		log_handle_printf(
		 log_handle,
		 "Folder already exists defaulting to: %" PRIs_SYSTEM "\n",
		 folder_name );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libcpath_path_join_wide(
		          &target_path,
		          &target_path_size,
		          export_path,
		          export_path_length,
		          folder_name,
		          folder_name_size - 1,
		          error );
#else
		result = libcpath_path_join(
		          &target_path,
		          &target_path_size,
		          export_path,
		          export_path_length,
		          folder_name,
		          folder_name_size - 1,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create target path.",
			 function );

			goto on_error;
		}
	}
	memory_free(
	 folder_name );

	folder_name = NULL;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcpath_path_make_directory_wide(
	     target_path,
	     error ) != 1 )
#else
	if( libcpath_path_make_directory(
	     target_path,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to make directory: %" PRIs_SYSTEM ".",
		 function,
		 target_path );

		goto on_error;
	}
	log_handle_printf(
	 log_handle,
	 "Created directory: %" PRIs_SYSTEM ".\n",
	 target_path );

	if( export_handle->dump_item_values != 0 )
	{
		if( export_handle_export_item_values(
		     export_handle,
		     folder,
		     _SYSTEM_STRING( "ItemValues.txt" ),
		     14,
		     target_path,
		     target_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export item values.\n",
				 function );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export folder item values.\n" );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		/* Export the folder unknowns
		 */
		if( export_handle_export_unknowns(
		     export_handle,
		     folder,
		     error ) != 1 )
		{
			libcerror_error_free(
			 error );
		}
#endif
		/* Export the folder sub items
		 */
		if( export_handle_export_sub_items(
		     export_handle,
		     folder,
		     target_path,
		     target_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_OUTPUT,
			 LIBCERROR_OUTPUT_ERROR_GENERIC,
			 "%s: unable to export sub items.",
			 function );

			goto on_error;
		}
	}
	else
	{
		/* Export the folder sub folders
		 */
		if( export_handle_export_sub_folders(
		     export_handle,
		     folder,
		     target_path,
		     target_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			fprintf(
			 export_handle->notify_stream,
			 "Unable to export sub folders.\n" );

			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_OUTPUT,
			 LIBCERROR_OUTPUT_ERROR_GENERIC,
			 "%s: unable to export sub folders.",
			 function );

			log_handle_printf(
			 log_handle,
			 "Unable to export sub folders.\n" );

			goto on_error;
		}
		/* Export the folder sub messages
		 */
		if( export_handle_export_sub_messages(
		     export_handle,
		     folder,
		     target_path,
		     target_path_size - 1,
		     log_handle,
		     error ) != 1 )
		{
			fprintf(
			 export_handle->notify_stream,
			 "Unable to export sub messages.\n" );

			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_OUTPUT,
			 LIBCERROR_OUTPUT_ERROR_GENERIC,
			 "%s: unable to export sub messages.",
			 function );

			log_handle_printf(
			 log_handle,
			 "Unable to export sub messages.\n" );

			goto on_error;
		}
/* TODO add support for sub associated contents, ignore them for now */
	}
	memory_free(
	 target_path );

	target_path = NULL;

	return( 1 );

on_error:
	if( target_path != NULL )
	{
		memory_free(
		 target_path );
	}
	if( folder_name != NULL )
	{
		memory_free(
		 folder_name );
	}
	return( -1 );
}

/* Exports the sub folders
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_sub_folders(
     export_handle_t *export_handle,
     libpff_item_t *folder,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	libpff_item_t *sub_folder = NULL;
	static char *function     = "export_handle_export_sub_folders";
	int number_of_sub_folders = 0;
	int sub_folder_iterator   = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	if( export_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export path.",
		 function );

		return( -1 );
	}
	if( libpff_folder_get_number_of_sub_folders(
	     folder,
	     &number_of_sub_folders,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub folders.",
		 function );

		return( -1 );
	}
	for( sub_folder_iterator = 0;
	     sub_folder_iterator < number_of_sub_folders;
	     sub_folder_iterator++ )
	{
		if( export_handle->abort != 0 )
		{
			return( -1 );
		}
		if( libpff_folder_get_sub_folder(
		     folder,
		     sub_folder_iterator,
		     &sub_folder,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub folder: %d.",
			 function,
			 sub_folder_iterator + 1 );

			return( -1 );
		}
		if( export_handle_export_folder(
		     export_handle,
		     sub_folder,
		     sub_folder_iterator,
		     export_path,
		     export_path_length,
		     log_handle,
		     error ) != 1 )
		{
			fprintf(
			 export_handle->notify_stream,
			 "Unable to export sub folder %d out of %d.\n",
			 sub_folder_iterator + 1,
			 number_of_sub_folders );

			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export sub folder: %d.\n",
				 function,
				 sub_folder_iterator + 1 );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export sub folder: %d.\n",
			 sub_folder_iterator + 1 );
		}
		if( libpff_item_free(
		     &sub_folder,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free sub folder: %d.",
			 function,
			 sub_folder_iterator + 1 );

			return( -1 );
		}
	}
	return( 1 );
}

/* Exports the sub messages
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_sub_messages(
     export_handle_t *export_handle,
     libpff_item_t *folder,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	libpff_item_t *sub_message = NULL;
	static char *function      = "export_handle_export_sub_messages";
	int number_of_sub_messages = 0;
	int sub_message_iterator   = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	if( export_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export path.",
		 function );

		return( -1 );
	}
	if( libpff_folder_get_number_of_sub_messages(
	     folder,
	     &number_of_sub_messages,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub messages.",
		 function );

		return( -1 );
	}
	for( sub_message_iterator = 0;
	     sub_message_iterator < number_of_sub_messages;
	     sub_message_iterator++ )
	{
		if( export_handle->abort != 0 )
		{
			return( -1 );
		}
		if( libpff_folder_get_sub_message(
		     folder,
		     sub_message_iterator,
		     &sub_message,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub message: %d out of %d.",
			 function,
			 sub_message_iterator + 1,
			 number_of_sub_messages );

			return( -1 );
		}
		if( export_handle_export_item(
		     export_handle,
		     sub_message,
		     sub_message_iterator,
		     number_of_sub_messages,
		     export_path,
		     export_path_length,
		     log_handle,
		     error ) != 1 )
		{
			fprintf(
			 export_handle->notify_stream,
			 "Unable to export sub message %d out of %d.\n",
			 sub_message_iterator + 1,
			 number_of_sub_messages );

			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unable to export sub message: %d.\n",
				 function,
				 sub_message_iterator + 1 );
			}
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
			libcerror_error_free(
			 error );

			log_handle_printf(
			 log_handle,
			 "Unable to export sub message: %d.\n",
			 sub_message_iterator + 1 );
		}
		if( libpff_item_free(
		     &sub_message,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free sub message: %d.",
			 function,
			 sub_message_iterator + 1 );

			return( -1 );
		}
	}
	return( 1 );
}

/* Exports the unknowns to a separate file
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_unknowns(
     export_handle_t *export_handle,
     libpff_item_t *folder,
     libcerror_error_t **error )
{
	libpff_item_t *unknowns = NULL;
	static char *function   = "export_handle_export_unknowns";
	int number_of_unknowns  = 0;
	int unknown_iterator    = 0;
	int result              = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
	if( folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid folder.",
		 function );

		return( -1 );
	}
	/* Export the unknowns
	 */
	result = libpff_folder_get_unknowns(
	          folder,
	          &unknowns,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve unknowns.",
		 function );

		return( -1 );
	}
	else if( result == 1 )
	{
		if( libpff_item_get_number_of_record_sets(
		     unknowns,
		     &number_of_unknowns,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to get number of unknowns.",
			 function );

			libpff_item_free(
			 &unknowns,
			 NULL );

			return( -1 );
		}
		if( number_of_unknowns > 0 )
		{
			for( unknown_iterator = 0;
			     unknown_iterator < number_of_unknowns;
			     unknown_iterator++ )
			{
/* TODO implement */
			}
		}
		if( libpff_item_free(
		     &unknowns,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free unknowns.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Exports the items
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_items(
     export_handle_t *export_handle,
     libpff_file_t *file,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	libpff_item_t *pff_root_item = NULL;
	static char *function        = "export_handle_export_items";
	int number_of_sub_items      = 0;
	int result                   = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
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
	if( export_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export path.",
		 function );

		return( -1 );
	}
	fprintf(
	 export_handle->notify_stream,
	 "Exporting items.\n" );

	if( export_handle->export_mode == EXPORT_MODE_DEBUG )
	{
		result = libpff_file_get_root_item(
		          file,
		          &pff_root_item,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root item.",
			 function );

			goto on_error;
		}
	}
	else
	{
		result = libpff_file_get_root_folder(
		          file,
		          &pff_root_item,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root folder.",
			 function );

			goto on_error;
		}
	}
	if( result != 0 )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libcpath_path_make_directory_wide(
		     export_path,
		     error ) != 1 )
#else
		if( libcpath_path_make_directory(
		     export_path,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to make directory: %" PRIs_SYSTEM ".",
			 function,
			 export_path );

			goto on_error;
		}
		log_handle_printf(
		 log_handle,
		 "Created directory: %" PRIs_SYSTEM ".\n",
		 export_path );

		if( libpff_item_get_number_of_sub_items(
		     pff_root_item,
		     &number_of_sub_items,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of sub items.",
			 function );

			goto on_error;
		}
		if( number_of_sub_items > 0 )
		{
			if( export_handle_export_sub_items(
			     export_handle,
			     pff_root_item,
			     export_path,
			     export_path_length,
			     log_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_OUTPUT,
				 LIBCERROR_OUTPUT_ERROR_GENERIC,
				 "%s: unable to export root item.",
				 function );

				goto on_error;
			}
			if( libpff_item_free(
			     &pff_root_item,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free root item.",
				 function );

				goto on_error;
			}
			fprintf(
			 export_handle->notify_stream,
			 "\n" );
		}
	}
	return( 1 );

on_error:
	if( pff_root_item != NULL )
	{
		libpff_item_free(
		 &pff_root_item,
		 NULL );
	}
	return( -1 );
}

/* Exports the orphan items
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_orphan_items(
     export_handle_t *export_handle,
     libpff_file_t *file,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	libpff_item_t *pff_orphan_item = NULL;
	static char *function          = "export_handle_export_orphan_items";
	int orphan_item_iterator       = 0;

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
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
	if( export_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export path.",
		 function );

		return( -1 );
	}
	if( libpff_file_get_number_of_orphan_items(
	     file,
	     &( export_handle->number_of_orphan_items ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of orphan items.",
		 function );

		return( -1 );
	}
	if( export_handle->number_of_orphan_items > 0 )
	{
		fprintf(
		 export_handle->notify_stream,
		 "Exporting orphan items.\n" );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libcpath_path_make_directory_wide(
		     export_path,
		     error ) != 1 )
#else
		if( libcpath_path_make_directory(
		     export_path,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to make directory: %" PRIs_SYSTEM ".",
			 function,
			 export_path );

			return( -1 );
		}
		log_handle_printf(
		 log_handle,
		 "Created directory: %" PRIs_SYSTEM ".\n",
		 export_path );

		for( orphan_item_iterator = 0;
		     orphan_item_iterator < export_handle->number_of_orphan_items;
		     orphan_item_iterator++ )
		{
			if( export_handle->abort != 0 )
			{
				return( -1 );
			}
			if( libpff_file_get_orphan_item_by_index(
			     file,
			     orphan_item_iterator,
			     &pff_orphan_item,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve orphan item: %d.",
				 function,
				 orphan_item_iterator + 1 );

				return( -1 );
			}
			if( pff_orphan_item == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid orphan item: %d.",
				 function,
				 orphan_item_iterator + 1 );

				return( -1 );
			}
			if( export_handle_export_item(
			     export_handle,
			     pff_orphan_item,
			     orphan_item_iterator,
			     export_handle->number_of_orphan_items,
			     export_path,
			     export_path_length,
			     log_handle,
			     error ) != 1 )
			{
				fprintf(
				 export_handle->notify_stream,
				 "Unable to export orphan item %d out of %d.\n",
				 orphan_item_iterator + 1,
				 export_handle->number_of_orphan_items );

				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: unable to export orphan item: %d.\n",
					 function,
					 orphan_item_iterator + 1 );
				}
				if( ( error != NULL )
				 && ( *error != NULL ) )
				{
					libcnotify_print_error_backtrace(
					 *error );
				}
				libcerror_error_free(
				 error );

				log_handle_printf(
				 log_handle,
				 "Unable to export orphan item: %d.\n",
				 orphan_item_iterator + 1 );
			}
			else
			{
				export_handle->number_of_exported_orphan_items++;
			}
			if( libpff_item_free(
			     &pff_orphan_item,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free orphan item: %d.",
				 function,
				 orphan_item_iterator + 1 );

				return( -1 );
			}
		}
		fprintf(
		 export_handle->notify_stream,
		 "\n" );
	}
	return( 1 );
}

/* Exports the recovered items
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_recovered_items(
     export_handle_t *export_handle,
     libpff_file_t *file,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	libpff_item_t *pff_recovered_item = NULL;
	static char *function             = "export_handle_export_recovered_items";
	int recovered_item_iterator       = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t identifier               = 0;
#endif

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
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
	if( export_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export path.",
		 function );

		return( -1 );
	}
	if( libpff_file_get_number_of_recovered_items(
	     file,
	     &( export_handle->number_of_recovered_items ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of recovered items.",
		 function );

		return( -1 );
	}
	if( export_handle->number_of_recovered_items > 0 )
	{
		fprintf(
		 export_handle->notify_stream,
		 "Exporting recovered items.\n" );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libcpath_path_make_directory_wide(
		     export_path,
		     error ) != 1 )
#else
		if( libcpath_path_make_directory(
		     export_path,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to make directory: %" PRIs_SYSTEM ".",
			 function,
			 export_path );

			return( -1 );
		}
		log_handle_printf(
		 log_handle,
		 "Created directory: %" PRIs_SYSTEM ".\n",
		 export_path );

		for( recovered_item_iterator = 0;
		     recovered_item_iterator < export_handle->number_of_recovered_items;
		     recovered_item_iterator++ )
		{
			if( export_handle->abort != 0 )
			{
				return( -1 );
			}
			if( libpff_file_get_recovered_item_by_index(
			     file,
			     recovered_item_iterator,
			     &pff_recovered_item,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve recovered item: %d.",
				 function,
				 recovered_item_iterator + 1 );

				return( -1 );
			}
			if( pff_recovered_item == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid recovered item: %d.",
				 function,
				 recovered_item_iterator + 1 );

				return( -1 );
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( libpff_item_get_identifier(
				     pff_recovered_item,
				     &identifier,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve recoverd item: %d identifier.",
					 function,
					 recovered_item_iterator + 1 );

					libpff_item_free(
					 &pff_recovered_item,
					 NULL );

					return( -1 );
				}
				libcnotify_printf(
				 "%s: processing recovered item: %" PRIu32 "\n",
				 function,
				 identifier );
			}
#endif
			if( export_handle_export_item(
			     export_handle,
			     pff_recovered_item,
			     recovered_item_iterator,
			     export_handle->number_of_recovered_items,
			     export_path,
			     export_path_length,
			     log_handle,
			     error ) != 1 )
			{
				fprintf(
				 export_handle->notify_stream,
				 "Unable to export recovered item %d out of %d.\n",
				 recovered_item_iterator + 1,
				 export_handle->number_of_recovered_items );

				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: unable to export recovered item: %d.\n",
					 function,
					 recovered_item_iterator + 1 );
				}
				if( ( error != NULL )
				 && ( *error != NULL ) )
				{
					libcnotify_print_error_backtrace(
					 *error );
				}
				libcerror_error_free(
				 error );

				log_handle_printf(
				 log_handle,
				 "Unable to export recovered item: %d.\n",
				 recovered_item_iterator + 1 );
			}
			else
			{
				export_handle->number_of_exported_recovered_items++;
			}
			if( libpff_item_free(
			     &pff_recovered_item,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free recovered item: %d.",
				 function,
				 recovered_item_iterator + 1 );

				return( -1 );
			}
		}
		fprintf(
		 export_handle->notify_stream,
		 "\n" );
	}
	return( 1 );
}

/* Exports the items in the file according to the export mode
 * Returns 1 if successful or -1 on error
 */
int export_handle_export_file(
     export_handle_t *export_handle,
     libpff_file_t *file,
     log_handle_t *log_handle,
     libcerror_error_t **error )
{
	static char *function = "export_handle_export_file";

	if( export_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid export handle.",
		 function );

		return( -1 );
	}
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
	if( ( export_handle->export_mode != EXPORT_MODE_DEBUG )
	 && ( export_handle->export_mode != EXPORT_MODE_ITEMS )
	 && ( export_handle->export_mode != EXPORT_MODE_ITEMS_NO_ATTACHMENTS ) )
	{
		fprintf(
		 export_handle->notify_stream,
		 "Recovering items.\n" );

/* TODO set recovery flags somewhere */
		if( libpff_file_recover_items(
		     file,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to recover items.",
			 function );

			return( -1 );
		}
	}
	if( export_handle->export_mode != EXPORT_MODE_RECOVERED )
	{
		if( export_handle->abort != 0 )
		{
			export_handle->abort = 0;

			return( -1 );
		}
		if( export_handle_export_items(
		     export_handle,
		     file,
		     export_handle->items_export_path,
		     export_handle->items_export_path_size - 1,
		     log_handle,
		     error ) != 1 )
                {
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to export items.",
			 function );

			return( -1 );
		}
	}
	if( ( export_handle->export_mode != EXPORT_MODE_DEBUG )
	 && ( export_handle->export_mode != EXPORT_MODE_ITEMS )
	 && ( export_handle->export_mode != EXPORT_MODE_ITEMS_NO_ATTACHMENTS ) )
	{
		if( export_handle->abort != 0 )
		{
			export_handle->abort = 0;

			return( -1 );
		}
		if( export_handle_export_orphan_items(
		     export_handle,
		     file,
		     export_handle->orphans_export_path,
		     export_handle->orphans_export_path_size - 1,
		     log_handle,
		     error ) != 1 )
                {
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to export orphan items.",
			 function );

			return( -1 );
		}
		if( export_handle->abort != 0 )
		{
			export_handle->abort = 0;

			return( -1 );
		}
		if( export_handle_export_recovered_items(
		     export_handle,
		     file,
		     export_handle->recovered_export_path,
		     export_handle->recovered_export_path_size - 1,
		     log_handle,
		     error ) != 1 )
                {
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to export recovered items.",
			 function );

			return( -1 );
		}
	}
	if( export_handle->abort != 0 )
	{
		export_handle->abort = 0;
	}
	return( 1 );
}

