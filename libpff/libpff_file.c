/*
 * File functions
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
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include "libpff_codepage.h"
#include "libpff_debug.h"
#include "libpff_definitions.h"
#include "libpff_descriptors_index.h"
#include "libpff_file.h"
#include "libpff_file_header.h"
#include "libpff_folder.h"
#include "libpff_io_handle.h"
#include "libpff_item.h"
#include "libpff_item_descriptor.h"
#include "libpff_item_tree.h"
#include "libpff_libbfio.h"
#include "libpff_libcdata.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_name_to_id_map.h"
#include "libpff_offsets_index.h"
#include "libpff_recover.h"
#include "libpff_types.h"

/* Creates a file
 * Make sure the value file is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_file_initialize(
     libpff_file_t **file,
     libcerror_error_t **error )
{
	libpff_internal_file_t *internal_file = NULL;
	static char *function                 = "libpff_file_initialize";

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
	if( *file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file value already set.",
		 function );

		return( -1 );
	}
	internal_file = memory_allocate_structure(
	                 libpff_internal_file_t );

	if( internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_file,
	     0,
	     sizeof( libpff_internal_file_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file.",
		 function );

		goto on_error;
	}
	if( libpff_io_handle_initialize(
	     &( internal_file->io_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
	*file = (libpff_file_t *) internal_file;

	return( 1 );

on_error:
	if( internal_file != NULL )
	{
		memory_free(
		 internal_file );
	}
	return( -1 );
}

/* Frees a file
 * Returns 1 if successful or -1 on error
 */
int libpff_file_free(
     libpff_file_t **file,
     libcerror_error_t **error )
{
	libpff_internal_file_t *internal_file = NULL;
	static char *function                 = "libpff_file_free";
	int result                            = 1;

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
	if( *file != NULL )
	{
		internal_file = (libpff_internal_file_t *) *file;

		if( internal_file->file_io_handle != NULL )
		{
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
		}
		*file = NULL;

		if( libpff_io_handle_free(
		     &( internal_file->io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free IO handle.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_file );
	}
	return( result );
}

/* Signals the file to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libpff_file_signal_abort(
     libpff_file_t *file,
     libcerror_error_t **error )
{
	libpff_internal_file_t *internal_file = NULL;
	static char *function                 = "libpff_file_signal_abort";

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
	internal_file = (libpff_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	internal_file->io_handle->abort = 1;

	return( 1 );
}

/* Opens a file
 * Returns 1 if successful or -1 on error
 */
int libpff_file_open(
     libpff_file_t *file,
     const char *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle      = NULL;
	libpff_internal_file_t *internal_file = NULL;
	static char *function                 = "libpff_file_open";
	size_t filename_length                = 0;

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
	internal_file = (libpff_internal_file_t *) file;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file IO handle.",
                 function );

		goto on_error;
	}
#endif
	filename_length = narrow_string_length(
	                   filename );

	if( libbfio_file_set_name(
	     file_io_handle,
	     filename,
	     filename_length + 1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set filename in file IO handle.",
                 function );

		goto on_error;
	}
	if( libpff_file_open_file_io_handle(
	     file,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file: %s.",
		 function,
		 filename );

		goto on_error;
	}
	internal_file->file_io_handle_created_in_library = 1;

	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Opens a file
 * Returns 1 if successful or -1 on error
 */
int libpff_file_open_wide(
     libpff_file_t *file,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle      = NULL;
	libpff_internal_file_t *internal_file = NULL;
	static char *function                 = "libpff_file_open_wide";
	size_t filename_length                = 0;

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
	internal_file = (libpff_internal_file_t *) file;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file IO handle.",
                 function );

		goto on_error;
	}
#endif
	filename_length = wide_string_length(
	                   filename );

	if( libbfio_file_set_name_wide(
	     file_io_handle,
	     filename,
	     filename_length + 1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set filename in file IO handle.",
                 function );

		goto on_error;
	}
	if( libpff_file_open_file_io_handle(
	     file,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file: %ls.",
		 function,
		 filename );

		goto on_error;
	}
	internal_file->file_io_handle_created_in_library = 1;

	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Opens a file using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libpff_file_open_file_io_handle(
     libpff_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error )
{
	libpff_internal_file_t *internal_file = NULL;
	static char *function                 = "libpff_file_open_file_io_handle";
	int bfio_access_flags                 = 0;
	int file_io_handle_is_open            = 0;

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
	internal_file = (libpff_internal_file_t *) file;

	if( internal_file->file_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - file IO handle already set.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBPFF_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBPFF_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBPFF_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBPFF_ACCESS_FLAG_READ ) != 0 )
	{
		bfio_access_flags = LIBBFIO_ACCESS_FLAG_READ;
	}
	file_io_handle_is_open = libbfio_handle_is_open(
	                          file_io_handle,
	                          error );

	if( file_io_handle_is_open == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to determine if file IO handle is open.",
		 function );

		goto on_error;
	}
	else if( file_io_handle_is_open == 0 )
	{
		if( libbfio_handle_open(
		     file_io_handle,
		     bfio_access_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open file IO handle.",
			 function );

			goto on_error;
		}
		internal_file->file_io_handle_opened_in_library = 1;
	}
	if( libpff_internal_file_open_read(
	     internal_file,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from file handle.",
		 function );

		goto on_error;
	}
	internal_file->file_io_handle = file_io_handle;

	return( 1 );

on_error:
	if( ( file_io_handle_is_open == 0 )
	 && ( internal_file->file_io_handle_opened_in_library != 0 ) )
	{
		libbfio_handle_close(
		 file_io_handle,
		 error );

		internal_file->file_io_handle_opened_in_library = 0;
	}
	internal_file->file_io_handle = NULL;

	return( -1 );
}

/* Closes a file
 * Returns 0 if successful or -1 on error
 */
int libpff_file_close(
     libpff_file_t *file,
     libcerror_error_t **error )
{
	libpff_internal_file_t *internal_file = NULL;
	static char *function                 = "libpff_file_close";
	int result                            = 0;

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
	internal_file = (libpff_internal_file_t *) file;

	if( internal_file->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( internal_file->file_io_handle_created_in_library != 0 )
		{
			if( libpff_debug_print_read_offsets(
			     internal_file->file_io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print the read offsets.",
				 function );

				result = -1;
			}
		}
	}
#endif
	if( internal_file->file_io_handle_opened_in_library != 0 )
	{
		if( libbfio_handle_close(
		     internal_file->file_io_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close file IO handle.",
			 function );

			result = -1;
		}
		internal_file->file_io_handle_opened_in_library = 0;
	}
	if( internal_file->file_io_handle_created_in_library != 0 )
	{
		if( libbfio_handle_free(
		     &( internal_file->file_io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file IO handle.",
			 function );

			result = -1;
		}
		internal_file->file_io_handle_created_in_library = 0;
	}
	internal_file->file_io_handle = NULL;

	if( libpff_io_handle_clear(
	     internal_file->io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		result = -1;
	}
	if( libpff_file_header_free(
	     &( internal_file->file_header ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file header.",
		 function );

		result = -1;
	}
	if( libpff_descriptors_index_free(
	     &( internal_file->descriptors_index ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free descriptors index.",
		 function );

		result = -1;
	}
	if( libpff_offsets_index_free(
	     &( internal_file->offsets_index ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free offsets index.",
		 function );

		result = -1;
	}
	if( libpff_item_tree_free(
	     &( internal_file->item_tree ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free item tree.",
		 function );

		result = -1;
	}
	internal_file->root_folder_item_tree_node = NULL;

	if( libcdata_list_free(
	     &( internal_file->orphan_item_list ),
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_tree_node_free_recovered,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free orphan item list.",
		 function );

		result = -1;
	}
	if( libcdata_list_free(
	     &( internal_file->name_to_id_map_list ),
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_name_to_id_map_entry_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free name to id map list.",
		 function );

		result = -1;
	}
	if( internal_file->recovered_item_list != NULL )
	{
		if( libcdata_list_free(
		     &( internal_file->recovered_item_list ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_tree_node_free_recovered,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free recovered item list.",
			 function );

			result = -1;
		}
	}
	if( internal_file->unallocated_data_block_list != NULL )
	{
		if( libcdata_range_list_free(
		     &( internal_file->unallocated_data_block_list ),
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free unallocated data block list.",
			 function );

			result = -1;
		}
	}
	if( internal_file->unallocated_page_block_list != NULL )
	{
		if( libcdata_range_list_free(
		     &( internal_file->unallocated_page_block_list ),
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free unallocated page block list.",
			 function );

			result = -1;
		}
	}
	return( result );
}

/* Opens a file for reading
 * Returns 1 if successful or -1 on error
 */
int libpff_internal_file_open_read(
     libpff_internal_file_t *internal_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libpff_internal_file_open_read";
	int result            = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	size_t page_size      = 0;
#endif

	if( internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( internal_file->file_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - file header value already set.",
		 function );

		return( -1 );
	}
	if( internal_file->descriptors_index != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - descriptors index value already set.",
		 function );

		return( -1 );
	}
	if( internal_file->offsets_index != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - offsets index value already set.",
		 function );

		return( -1 );
	}
	if( internal_file->item_tree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - item tree value already set.",
		 function );

		return( -1 );
	}
	if( internal_file->root_folder_item_tree_node != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - root folder item tree root node value already set.",
		 function );

		return( -1 );
	}
	if( internal_file->orphan_item_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - orphan item list value already set.",
		 function );

		return( -1 );
	}
	if( internal_file->name_to_id_map_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - name to id map list value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading file header:\n" );
	}
#endif
	if( libpff_file_header_initialize(
	     &( internal_file->file_header ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file header.",
		 function );

		goto on_error;
	}
	if( libpff_file_header_read_file_io_handle(
	     internal_file->file_header,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header data.",
		 function );

		goto on_error;
	}
	internal_file->io_handle->encryption_type = internal_file->file_header->encryption_type;
	internal_file->io_handle->file_size       = internal_file->file_header->file_size;
	internal_file->io_handle->file_type       = internal_file->file_header->file_type;

	if( ( internal_file->io_handle->encryption_type != LIBPFF_ENCRYPTION_TYPE_NONE )
	 && ( internal_file->io_handle->encryption_type != LIBPFF_ENCRYPTION_TYPE_COMPRESSIBLE )
	 && ( internal_file->io_handle->encryption_type != LIBPFF_ENCRYPTION_TYPE_HIGH ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported encryption type: 0x%02x",
		 function,
		 internal_file->io_handle->encryption_type );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: file type\t\t\t\t: %" PRIu8 "\n",
		 function,
		 internal_file->io_handle->file_type );

		if( internal_file->io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE )
		{
			page_size = 4096;
		}
		else
		{
			page_size = 512;
		}
		libcnotify_printf(
		 "%s: page size\t\t\t\t: %" PRIzd "\n",
		 function,
		 page_size );

		libcnotify_printf(
		 "\n" );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Creating item tree:\n" );
	}
#endif
	if( libpff_descriptors_index_initialize(
	     &( internal_file->descriptors_index ),
	     internal_file->file_header->descriptors_index_root_node_offset,
	     internal_file->file_header->descriptors_index_root_node_back_pointer,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create descriptors index.",
		 function );

		goto on_error;
	}
	if( libpff_offsets_index_initialize(
	     &( internal_file->offsets_index ),
	     internal_file->file_header->offsets_index_root_node_offset,
	     internal_file->file_header->offsets_index_root_node_back_pointer,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create offsets index.",
		 function );

		goto on_error;
	}
	if( libcdata_list_initialize(
	     &( internal_file->orphan_item_list ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create orphan item list.",
		 function );

		goto on_error;
	}
	if( libpff_item_tree_initialize(
	     &( internal_file->item_tree ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create item tree.",
		 function );

		goto on_error;
	}
	if( libpff_item_tree_create(
             internal_file->item_tree,
             internal_file->io_handle,
	     file_io_handle,
	     internal_file->descriptors_index,
	     internal_file->orphan_item_list,
	     &( internal_file->root_folder_item_tree_node ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create item tree.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Name to ID map:\n" );
	}
#endif
	if( libcdata_list_initialize(
	     &( internal_file->name_to_id_map_list ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create name to id map list.",
		 function );

		goto on_error;
	}
	result = libpff_name_to_id_map_read(
		  internal_file->name_to_id_map_list,
		  internal_file->io_handle,
		  file_io_handle,
		  internal_file->descriptors_index,
		  internal_file->offsets_index,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read name to id map.",
		 function );

		goto on_error;
	}
/* TODO flag missing name to id map if 0 */
	return( 1 );

on_error:
	if( internal_file->name_to_id_map_list != NULL )
	{
		libcdata_list_free(
		 &( internal_file->name_to_id_map_list ),
		 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_name_to_id_map_entry_free,
		 NULL );
	}
	internal_file->root_folder_item_tree_node = NULL;

	if( internal_file->orphan_item_list != NULL )
	{
		libcdata_list_free(
		 &( internal_file->orphan_item_list ),
		 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_item_tree_node_free_recovered,
		 NULL );
	}
	if( internal_file->item_tree != NULL )
	{
		libpff_item_tree_free(
		 &( internal_file->item_tree ),
		 NULL );
	}
	if( internal_file->offsets_index != NULL )
	{
		libpff_offsets_index_free(
		 &( internal_file->offsets_index ),
		 NULL );
	}
	if( internal_file->descriptors_index != NULL )
	{
		libpff_descriptors_index_free(
		 &( internal_file->descriptors_index ),
		 NULL );
	}
	if( internal_file->file_header != NULL )
	{
		libpff_file_header_free(
		 &( internal_file->file_header ),
		 NULL );
	}
	return( -1 );
}

/* Reads the allocation tables
 * Returns 1 if successful or -1 on error
 */
int libpff_internal_file_read_allocation_tables(
     libpff_internal_file_t *internal_file,
     libcerror_error_t **error )
{
	static char *function = "libpff_internal_file_read_allocation_tables";

	if( internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( internal_file->io_handle->file_type != LIBPFF_FILE_TYPE_32BIT )
	 && ( internal_file->io_handle->file_type != LIBPFF_FILE_TYPE_64BIT )
	 && ( internal_file->io_handle->file_type != LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file type.",
		 function );

		return( -1 );
	}
	if( internal_file->read_allocation_tables != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - allocation tables already set.",
		 function );

		return( -1 );
	}
	if( internal_file->unallocated_data_block_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - allocaled data block list already set.",
		 function );

		return( -1 );
	}
	if( internal_file->unallocated_page_block_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - allocaled page block list already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading the unallocated data blocks:\n" );
	}
#endif
	if( libcdata_range_list_initialize(
	     &( internal_file->unallocated_data_block_list ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create unallocated data block list.",
		 function );

		goto on_error;
	}
	if( libpff_io_handle_read_unallocated_data_blocks(
	     internal_file->io_handle,
	     internal_file->file_io_handle,
	     internal_file->unallocated_data_block_list,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read unallocated data blocks.",
		 function );

		goto on_error;
	}
	if( ( internal_file->io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
	 || ( internal_file->io_handle->file_type == LIBPFF_FILE_TYPE_64BIT ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "Reading the unallocated page blocks:\n" );
		}
#endif
		if( libcdata_range_list_initialize(
		     &( internal_file->unallocated_page_block_list ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create unallocated page block list.",
			 function );

			goto on_error;
		}
		if( libpff_io_handle_read_unallocated_page_blocks(
		     internal_file->io_handle,
		     internal_file->file_io_handle,
		     internal_file->unallocated_page_block_list,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read unallocated page blocks.",
			 function );

			goto on_error;
		}
	}
	internal_file->read_allocation_tables = 1;

	return( 1 );

on_error:
	if( internal_file->unallocated_page_block_list != NULL )
	{
		libcdata_range_list_free(
		 &( internal_file->unallocated_page_block_list ),
		 NULL,
		 NULL );
	}
	if( internal_file->unallocated_data_block_list != NULL )
	{
		libcdata_range_list_free(
		 &( internal_file->unallocated_data_block_list ),
		 NULL,
		 NULL );
	}
	return( -1 );
}

/* Determine if the file corrupted
 * Returns 1 if corrupted, 0 if not or -1 on error
 */
int libpff_file_is_corrupted(
     libpff_file_t *file,
     libcerror_error_t **error )
{
	libpff_internal_file_t *internal_file = NULL;
	static char *function                 = "libpff_file_is_corrupted";

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
	internal_file = (libpff_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( internal_file->io_handle->flags & LIBPFF_IO_HANDLE_FLAG_IS_CORRUPTED ) != 0 )
	{
		return( 1 );
	}
	return( 0 );
}

/* Recovers deleted items
 * By default only the unallocated space is checked for recoverable items
 * Returns 1 if successful or -1 on error
 */
int libpff_file_recover_items(
     libpff_file_t *file,
     uint8_t recovery_flags,
     libcerror_error_t **error )
{
	libpff_internal_file_t *internal_file = NULL;
	static char *function                 = "libpff_file_recover_items";
	int result                            = 0;

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
	internal_file = (libpff_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_file->recovered_item_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - recovered item list already set.",
		 function );

		return( -1 );
	}
	if( internal_file->read_allocation_tables == 0 )
	{
		if( libpff_internal_file_read_allocation_tables(
		     internal_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read allocation tables.",
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

/* TODO set recovery_flags |= LIBPFF_RECOVERY_FLAG_IGNORE_ALLOCATION_DATA ? */
		}
	}
	if( libcdata_list_initialize(
	     &( internal_file->recovered_item_list ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create recovered item list.",
		 function );

		return( -1 );
	}
	result = libpff_recover_items(
	          internal_file->io_handle,
	          internal_file->file_io_handle,
	          internal_file->descriptors_index,
	          internal_file->offsets_index,
	          internal_file->unallocated_data_block_list,
	          internal_file->unallocated_page_block_list,
	          internal_file->recovered_item_list,
	          recovery_flags,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to recover items.",
		 function );

		return( -1 );
	}
        if( internal_file->io_handle->abort != 0 )
        {
                internal_file->io_handle->abort = 0;
        }
	return( result );
}

/* Retrieves the file size
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_file_get_size(
     libpff_file_t *file,
     size64_t *size,
     libcerror_error_t **error )
{
	libpff_internal_file_t *internal_file = NULL;
	static char *function                 = "libpff_file_get_size";

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
	internal_file = (libpff_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size.",
		 function );

		return( -1 );
	}
	if( internal_file->file_io_handle == NULL )
	{
		return( 0 );
	}
	*size = internal_file->io_handle->file_size;

	return( 1 );
}

/* Retrieves the content type
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_file_get_content_type(
     libpff_file_t *file,
     uint8_t *content_type,
     libcerror_error_t **error )
{
	libpff_internal_file_t *internal_file = NULL;
	static char *function                 = "libpff_file_get_content_type";

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
	internal_file = (libpff_internal_file_t *) file;

	if( internal_file->file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file header.",
		 function );

		return( -1 );
	}
	if( content_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid content type.",
		 function );

		return( -1 );
	}
	if( internal_file->file_io_handle == NULL )
	{
		return( 0 );
	}
	*content_type = (uint8_t) internal_file->file_header->file_content_type;

	return( 1 );
}

/* Retrieves the file type
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_file_get_type(
     libpff_file_t *file,
     uint8_t *type,
     libcerror_error_t **error )
{
	libpff_internal_file_t *internal_file = NULL;
	static char *function                 = "libpff_file_get_type";

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
	internal_file = (libpff_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid type.",
		 function );

		return( -1 );
	}
	if( internal_file->file_io_handle == NULL )
	{
		return( 0 );
	}
	*type = internal_file->io_handle->file_type;

	return( 1 );
}

/* Retrieves the encryption type
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_file_get_encryption_type(
     libpff_file_t *file,
     uint8_t *encryption_type,
     libcerror_error_t **error )
{
	libpff_internal_file_t *internal_file = NULL;
	static char *function                 = "libpff_file_get_encryption_type";

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
	internal_file = (libpff_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( encryption_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encryption type.",
		 function );

		return( -1 );
	}
	if( internal_file->file_io_handle == NULL )
	{
		return( 0 );
	}
	*encryption_type = internal_file->io_handle->encryption_type;

	return( 1 );
}

/* Retrieves the ASCII codepage
 * Returns 1 if successful or -1 on error
 */
int libpff_file_get_ascii_codepage(
     libpff_file_t *file,
     int *ascii_codepage,
     libcerror_error_t **error )
{
	libpff_internal_file_t *internal_file = NULL;
	static char *function                 = "libpff_file_get_ascii_codepage";

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
	internal_file = (libpff_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ascii_codepage == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid ASCII codepage.",
		 function );

		return( -1 );
	}
	*ascii_codepage = internal_file->io_handle->ascii_codepage;

	return( 1 );
}

/* Sets the ASCII codepage
 * Returns 1 if successful or -1 on error
 */
int libpff_file_set_ascii_codepage(
     libpff_file_t *file,
     int ascii_codepage,
     libcerror_error_t **error )
{
	libpff_internal_file_t *internal_file = NULL;
	static char *function                 = "libpff_file_set_ascii_codepage";

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
	internal_file = (libpff_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( ascii_codepage != LIBPFF_CODEPAGE_ASCII )
	 && ( ascii_codepage != LIBPFF_CODEPAGE_WINDOWS_874 )
	 && ( ascii_codepage != LIBPFF_CODEPAGE_WINDOWS_932 )
	 && ( ascii_codepage != LIBPFF_CODEPAGE_WINDOWS_936 )
	 && ( ascii_codepage != LIBPFF_CODEPAGE_WINDOWS_949 )
	 && ( ascii_codepage != LIBPFF_CODEPAGE_WINDOWS_950 )
	 && ( ascii_codepage != LIBPFF_CODEPAGE_WINDOWS_1250 )
	 && ( ascii_codepage != LIBPFF_CODEPAGE_WINDOWS_1251 )
	 && ( ascii_codepage != LIBPFF_CODEPAGE_WINDOWS_1252 )
	 && ( ascii_codepage != LIBPFF_CODEPAGE_WINDOWS_1253 )
	 && ( ascii_codepage != LIBPFF_CODEPAGE_WINDOWS_1254 )
	 && ( ascii_codepage != LIBPFF_CODEPAGE_WINDOWS_1255 )
	 && ( ascii_codepage != LIBPFF_CODEPAGE_WINDOWS_1256 )
	 && ( ascii_codepage != LIBPFF_CODEPAGE_WINDOWS_1257 )
	 && ( ascii_codepage != LIBPFF_CODEPAGE_WINDOWS_1258 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported ASCII codepage.",
		 function );

		return( -1 );
	}
	internal_file->io_handle->ascii_codepage = ascii_codepage;

	return( 1 );
}

/* Retrieves the number of unallocated blocks
 * Returns 1 if successful or -1 on error
 */
int libpff_file_get_number_of_unallocated_blocks(
     libpff_file_t *file,
     int unallocated_block_type,
     int *number_of_unallocated_blocks,
     libcerror_error_t **error )
{
	libcdata_range_list_t *unallocated_block_list = NULL;
	libpff_internal_file_t *internal_file         = NULL;
	static char *function                         = "libpff_file_get_number_of_unallocated_blocks";

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
	if( ( unallocated_block_type != LIBPFF_UNALLOCATED_BLOCK_TYPE_DATA )
	 && ( unallocated_block_type != LIBPFF_UNALLOCATED_BLOCK_TYPE_PAGE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported unallocated block type.",
		 function );

		return( -1 );
	}
	internal_file = (libpff_internal_file_t *) file;

	if( internal_file->read_allocation_tables == 0 )
	{
		if( libpff_internal_file_read_allocation_tables(
		     internal_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read allocation tables.",
			 function );

			return( -1 );
		}
	}
	if( unallocated_block_type == LIBPFF_UNALLOCATED_BLOCK_TYPE_DATA )
	{
		unallocated_block_list = internal_file->unallocated_data_block_list;
	}
	else if( unallocated_block_type == LIBPFF_UNALLOCATED_BLOCK_TYPE_PAGE )
	{
		unallocated_block_list = internal_file->unallocated_page_block_list;
	}
	if( unallocated_block_list == NULL )
	{
		if( number_of_unallocated_blocks == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid number of unallocated blocks.",
			 function );

			return( -1 );
		}
		*number_of_unallocated_blocks = 0;
	}
	else
	{
		if( libcdata_range_list_get_number_of_elements(
		     unallocated_block_list,
		     number_of_unallocated_blocks,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of unallocated blocks.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves a specific unallocated block
 * Returns 1 if successful or -1 on error
 */
int libpff_file_get_unallocated_block(
     libpff_file_t *file,
     int unallocated_block_type,
     int unallocated_block_index,
     off64_t *offset,
     size64_t *size,
     libcerror_error_t **error )
{
	libcdata_range_list_t *unallocated_block_list = NULL;
	libpff_internal_file_t *internal_file         = NULL;
	intptr_t *value                               = NULL;
	static char *function                         = "libpff_file_get_unallocated_block";

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
	if( ( unallocated_block_type != LIBPFF_UNALLOCATED_BLOCK_TYPE_DATA )
	 && ( unallocated_block_type != LIBPFF_UNALLOCATED_BLOCK_TYPE_PAGE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported unallocated block type.",
		 function );

		return( -1 );
	}
	internal_file = (libpff_internal_file_t *) file;

	if( unallocated_block_type == LIBPFF_UNALLOCATED_BLOCK_TYPE_DATA )
	{
		unallocated_block_list = internal_file->unallocated_data_block_list;
	}
	else if( unallocated_block_type == LIBPFF_UNALLOCATED_BLOCK_TYPE_PAGE )
	{
		unallocated_block_list = internal_file->unallocated_page_block_list;
	}
	if( unallocated_block_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing unallocated block list.",
		 function );

		return( -1 );
	}
	if( internal_file->read_allocation_tables == 0 )
	{
		if( libpff_internal_file_read_allocation_tables(
		     internal_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read allocation tables.",
			 function );

			return( -1 );
		}
	}
	if( libcdata_range_list_get_range_by_index(
	     unallocated_block_list,
	     unallocated_block_index,
	     (uint64_t *) offset,
	     (uint64_t *) size,
	     &value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve range list element: %d.",
		 function,
		 unallocated_block_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the root item
 * Returns 1 if successful or -1 on error
 */
int libpff_file_get_root_item(
     libpff_file_t *file,
     libpff_item_t **root_item,
     libcerror_error_t **error )
{
	libpff_internal_file_t *internal_file = NULL;
	static char *function                 = "libpff_file_get_root_item";

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
	internal_file = (libpff_internal_file_t *) file;

	if( internal_file->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
		 function );

		return( -1 );
	}
	if( internal_file->item_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing item tree.",
		 function );

		return( -1 );
	}
	if( root_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid root item.",
		 function );

		return( -1 );
	}
	if( *root_item != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: root item already set.",
		 function );

		return( -1 );
	}
	if( libpff_item_initialize(
	     root_item,
	     internal_file->io_handle,
	     internal_file->file_io_handle,
	     internal_file->name_to_id_map_list,
	     internal_file->descriptors_index,
	     internal_file->offsets_index,
	     internal_file->item_tree,
	     internal_file->item_tree->root_node,
	     LIBPFF_ITEM_FLAGS_DEFAULT,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create root item.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the message store
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_file_get_message_store(
     libpff_file_t *file,
     libpff_item_t **message_store,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *message_store_item_tree_node = NULL;
	libpff_internal_file_t *internal_file              = NULL;
	static char *function                              = "libpff_file_get_message_store";
	int result                                         = 0;

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
	internal_file = (libpff_internal_file_t *) file;

	if( internal_file->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
		 function );

		return( -1 );
	}
	if( message_store == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid message store.",
		 function );

		return( -1 );
	}
	if( *message_store != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: message store already set.",
		 function );

		return( -1 );
	}
	result = libpff_item_tree_get_node_by_identifier(
	          internal_file->item_tree,
	          LIBPFF_DESCRIPTOR_IDENTIFIER_MESSAGE_STORE,
                  &message_store_item_tree_node,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve message store item tree node: %" PRIu32 ".",
		 function,
		 LIBPFF_DESCRIPTOR_IDENTIFIER_MESSAGE_STORE );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libpff_item_initialize(
		     message_store,
		     internal_file->io_handle,
		     internal_file->file_io_handle,
		     internal_file->name_to_id_map_list,
		     internal_file->descriptors_index,
		     internal_file->offsets_index,
		     internal_file->item_tree,
		     message_store_item_tree_node,
		     LIBPFF_ITEM_FLAGS_DEFAULT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create message store.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the name to id map
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_file_get_name_to_id_map(
     libpff_file_t *file,
     libpff_item_t **name_to_id_map,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *name_to_id_map_item_tree_node = NULL;
	libpff_internal_file_t *internal_file               = NULL;
	static char *function                               = "libpff_file_get_name_to_id_map";
	int result                                          = 0;

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
	internal_file = (libpff_internal_file_t *) file;

	if( internal_file->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
		 function );

		return( -1 );
	}
	if( name_to_id_map == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name to id map.",
		 function );

		return( -1 );
	}
	if( *name_to_id_map != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: name to id map already set.",
		 function );

		return( -1 );
	}
	result = libpff_item_tree_get_node_by_identifier(
	          internal_file->item_tree,
	          LIBPFF_DESCRIPTOR_IDENTIFIER_NAME_TO_ID_MAP,
                  &name_to_id_map_item_tree_node,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name to ip map item tree node: %" PRIu32 ".",
		 function,
		 LIBPFF_DESCRIPTOR_IDENTIFIER_NAME_TO_ID_MAP );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libpff_item_initialize(
		     name_to_id_map,
		     internal_file->io_handle,
		     internal_file->file_io_handle,
		     internal_file->name_to_id_map_list,
		     internal_file->descriptors_index,
		     internal_file->offsets_index,
		     internal_file->item_tree,
		     name_to_id_map_item_tree_node,
		     LIBPFF_ITEM_FLAGS_DEFAULT,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create name to id map.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the root folder
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_file_get_root_folder(
     libpff_file_t *file,
     libpff_item_t **root_folder,
     libcerror_error_t **error )
{
	libpff_internal_file_t *internal_file = NULL;
	static char *function                 = "libpff_file_get_root_folder";

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
	internal_file = (libpff_internal_file_t *) file;

	if( internal_file->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
		 function );

		return( -1 );
	}
	if( root_folder == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid root folder.",
		 function );

		return( -1 );
	}
	if( *root_folder != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: root folder already set.",
		 function );

		return( -1 );
	}
	if( internal_file->root_folder_item_tree_node == NULL )
	{
		return( 0 );
	}
	if( libpff_item_initialize(
	     root_folder,
	     internal_file->io_handle,
	     internal_file->file_io_handle,
	     internal_file->name_to_id_map_list,
	     internal_file->descriptors_index,
	     internal_file->offsets_index,
	     internal_file->item_tree,
	     internal_file->root_folder_item_tree_node,
	     LIBPFF_ITEM_FLAGS_DEFAULT,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create root folder.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves an item for a specific identifier
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_file_get_item_by_identifier(
     libpff_file_t *file,
     uint32_t item_identifier,
     libpff_item_t **item,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *item_tree_node  = NULL;
	libpff_internal_file_t *internal_file = NULL;
	static char *function                 = "libpff_file_get_item_by_identifier";
	int result                            = 0;

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
	internal_file = (libpff_internal_file_t *) file;

	if( internal_file->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
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
	if( *item != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: item already set.",
		 function );

		return( -1 );
	}
	result = libpff_item_tree_get_node_by_identifier(
	          internal_file->item_tree,
                  item_identifier,
                  &item_tree_node,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve item tree node: %" PRIu32 ".",
		 function,
		 item_identifier );

		return( -1 );
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	if( item_tree_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid item tree node.",
		 function );

		return( -1 );
	}
	if( libpff_item_initialize(
	     item,
	     internal_file->io_handle,
	     internal_file->file_io_handle,
	     internal_file->name_to_id_map_list,
	     internal_file->descriptors_index,
	     internal_file->offsets_index,
	     internal_file->item_tree,
	     item_tree_node,
	     LIBPFF_ITEM_FLAGS_DEFAULT,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize item.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of orphan items
 * Returns 1 if successful or -1 on error
 */
int libpff_file_get_number_of_orphan_items(
     libpff_file_t *file,
     int *number_of_orphan_items,
     libcerror_error_t **error )
{
	libpff_internal_file_t *internal_file = NULL;
	static char *function                 = "libpff_file_get_number_of_orphan_items";

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
	internal_file = (libpff_internal_file_t *) file;

	if( internal_file->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
		 function );

		return( -1 );
	}
	if( libcdata_list_get_number_of_elements(
	     internal_file->orphan_item_list,
	     number_of_orphan_items,
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
	return( 1 );
}

/* Retrieves a specific orphan item
 * Returns 1 if successful or -1 on error
 */
int libpff_file_get_orphan_item_by_index(
     libpff_file_t *file,
     int orphan_item_index,
     libpff_item_t **orphan_item,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *orphan_item_tree_node = NULL;
	libpff_internal_file_t *internal_file       = NULL;
	static char *function                       = "libpff_file_get_orphan_item_by_index";

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
	internal_file = (libpff_internal_file_t *) file;

	if( internal_file->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
		 function );

		return( -1 );
	}
	if( orphan_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid orphan item.",
		 function );

		return( -1 );
	}
	if( libcdata_list_get_value_by_index(
	     internal_file->orphan_item_list,
	     orphan_item_index,
	     (intptr_t **) &orphan_item_tree_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve orphan item tree node: %d.",
		 function,
		 orphan_item_index );

		return( -1 );
	}
	if( libpff_item_initialize(
	     orphan_item,
	     internal_file->io_handle,
	     internal_file->file_io_handle,
	     internal_file->name_to_id_map_list,
	     internal_file->descriptors_index,
	     internal_file->offsets_index,
	     internal_file->item_tree,
	     orphan_item_tree_node,
	     LIBPFF_ITEM_FLAGS_DEFAULT,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create orphan item.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of recovered items
 * Returns 1 if successful or -1 on error
 */
int libpff_file_get_number_of_recovered_items(
     libpff_file_t *file,
     int *number_of_recovered_items,
     libcerror_error_t **error )
{
	libpff_internal_file_t *internal_file = NULL;
	static char *function                 = "libpff_file_get_number_of_recovered_items";

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
	internal_file = (libpff_internal_file_t *) file;

	if( internal_file->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
		 function );

		return( -1 );
	}
	if( libcdata_list_get_number_of_elements(
	     internal_file->recovered_item_list,
	     number_of_recovered_items,
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
	return( 1 );
}

/* Retrieves a specific recovered item
 * Returns 1 if successful or -1 on error
 */
int libpff_file_get_recovered_item_by_index(
     libpff_file_t *file,
     int recovered_item_index,
     libpff_item_t **recovered_item,
     libcerror_error_t **error )
{
	libcdata_tree_node_t *recovered_item_tree_node = NULL;
	libpff_internal_file_t *internal_file          = NULL;
	static char *function                          = "libpff_file_get_recovered_item_by_index";

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
	internal_file = (libpff_internal_file_t *) file;

	if( internal_file->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
		 function );

		return( -1 );
	}
	if( recovered_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid recovered item.",
		 function );

		return( -1 );
	}
	if( libcdata_list_get_value_by_index(
	     internal_file->recovered_item_list,
	     recovered_item_index,
	     (intptr_t **) &recovered_item_tree_node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve recovered item tree node: %d.",
		 function,
		 recovered_item_index );

		return( -1 );
	}
	if( libpff_item_initialize(
	     recovered_item,
	     internal_file->io_handle,
	     internal_file->file_io_handle,
	     internal_file->name_to_id_map_list,
	     internal_file->descriptors_index,
	     internal_file->offsets_index,
	     internal_file->item_tree,
	     recovered_item_tree_node,
	     LIBPFF_ITEM_FLAGS_DEFAULT,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create recovered item.",
		 function );

		return( -1 );
	}
	return( 1 );
}

