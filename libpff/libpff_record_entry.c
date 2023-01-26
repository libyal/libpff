/*
 * Record entry functions
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
#include "libpff_descriptor_data_stream.h"
#include "libpff_libcerror.h"
#include "libpff_libuna.h"
#include "libpff_mapi.h"
#include "libpff_mapi_value.h"
#include "libpff_multi_value.h"
#include "libpff_record_entry.h"
#include "libpff_record_entry_identifier.h"

/* Creates a record entry
 * Make sure the value record_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_initialize(
     libpff_record_entry_t **record_entry,
     int ascii_codepage,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_initialize";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	if( *record_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid record entry value already set.",
		 function );

		return( -1 );
	}
	internal_record_entry = memory_allocate_structure(
	                         libpff_internal_record_entry_t );

	if( internal_record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create record entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_record_entry,
	     0,
	     sizeof( libpff_internal_record_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear record entry.",
		 function );

		goto on_error;
	}
	internal_record_entry->ascii_codepage = ascii_codepage;

	*record_entry = (libpff_record_entry_t *) internal_record_entry;

	return( 1 );

on_error:
	if( internal_record_entry != NULL )
	{
		memory_free(
		 internal_record_entry );
	}
	return( -1 );
}

/* Frees a record entry
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_free(
     libpff_record_entry_t **record_entry,
     libcerror_error_t **error )
{
	static char *function = "libpff_record_entry_free";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	if( *record_entry != NULL )
	{
		*record_entry = NULL;
	}
	return( 1 );
}

/* Frees a record entry
 * Returns 1 if successful or -1 on error
 */
int libpff_internal_record_entry_free(
     libpff_internal_record_entry_t **internal_record_entry,
     libcerror_error_t **error )
{
	static char *function = "libpff_internal_record_entry_free";

	if( internal_record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	if( *internal_record_entry != NULL )
	{
		if( ( *internal_record_entry )->value_data != NULL )
		{
			memory_free(
			 ( *internal_record_entry )->value_data );
		}
		memory_free(
		 *internal_record_entry );

		*internal_record_entry = NULL;
	}
	return( 1 );
}

/* Clones the record entry
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_clone(
     libpff_record_entry_t **destination_record_entry,
     libpff_record_entry_t *source_record_entry,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_destination_record_entry = NULL;
	libpff_internal_record_entry_t *internal_source_record_entry      = NULL;
	static char *function                                             = "libpff_record_entry_clone";

	if( destination_record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	if( *destination_record_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination record entry value already set.",
		 function );

		return( -1 );
	}
	if( source_record_entry == NULL )
	{
		*destination_record_entry = NULL;

		return( 1 );
	}
	internal_source_record_entry = (libpff_internal_record_entry_t *) source_record_entry;

	if( libpff_record_entry_initialize(
	     (libpff_record_entry_t **) &internal_destination_record_entry,
	     internal_source_record_entry->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination record entry.",
		 function );

		goto on_error;
	}
	if( internal_destination_record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing destination record entry.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     &( internal_destination_record_entry->identifier ),
	     &( internal_source_record_entry->identifier ),
	     sizeof( libpff_record_entry_identifier_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy identifier.",
		 function );

		goto on_error;
	}
	internal_destination_record_entry->value_data      = NULL;
	internal_destination_record_entry->value_data_size = 0;

	if( internal_source_record_entry->value_data != NULL )
	{
		internal_destination_record_entry->value_data = (uint8_t *) memory_allocate(
		                                                             internal_source_record_entry->value_data_size );

		if( internal_destination_record_entry->value_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value data.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     internal_destination_record_entry->value_data,
		     internal_source_record_entry->value_data,
		     internal_source_record_entry->value_data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy value data.",
			 function );

			goto on_error;
		}
		internal_destination_record_entry->value_data_size = internal_source_record_entry->value_data_size;
	}
	internal_destination_record_entry->name_to_id_map_entry = internal_source_record_entry->name_to_id_map_entry;
	internal_destination_record_entry->flags                = internal_source_record_entry->flags;

	*destination_record_entry = (libpff_record_entry_t *) internal_destination_record_entry;

	return( 1 );

on_error:
	if( internal_destination_record_entry != NULL )
	{
		libpff_internal_record_entry_free(
		 &internal_destination_record_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the entry type
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_record_entry_get_entry_type(
     libpff_record_entry_t *record_entry,
     uint32_t *entry_type,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_entry_type";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

	if( entry_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry type.",
		 function );

		return( -1 );
	}
	if( internal_record_entry->identifier.format != LIBPFF_RECORD_ENTRY_IDENTIFIER_FORMAT_MAPI_PROPERTY )
	{
		return( 0 );
	}
	*entry_type = internal_record_entry->identifier.entry_type;

	return( 1 );
}

/* Retrieves the value type
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_record_entry_get_value_type(
     libpff_record_entry_t *record_entry,
     uint32_t *value_type,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_value_type";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

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
	if( internal_record_entry->identifier.format != LIBPFF_RECORD_ENTRY_IDENTIFIER_FORMAT_MAPI_PROPERTY )
	{
		return( 0 );
	}
	*value_type = internal_record_entry->identifier.value_type;

	return( 1 );
}

/* Retrieves the name to ID map entry
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_record_entry_get_name_to_id_map_entry(
     libpff_record_entry_t *record_entry,
     libpff_name_to_id_map_entry_t **name_to_id_map_entry,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_name_to_id_map_entry";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

	if( name_to_id_map_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name to ID map entry.",
		 function );

		return( -1 );
	}
	if( ( internal_record_entry->identifier.format != LIBPFF_RECORD_ENTRY_IDENTIFIER_FORMAT_MAPI_PROPERTY )
	 || ( internal_record_entry->name_to_id_map_entry == NULL ) )
	{
		return( 0 );
	}
	*name_to_id_map_entry = (libpff_name_to_id_map_entry_t *) internal_record_entry->name_to_id_map_entry;

	return( 1 );
}

/* Retrieves the data size
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_data_size(
     libpff_record_entry_t *record_entry,
     size_t *data_size,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_data_size";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

	if( data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data size.",
		 function );

		return( -1 );
	}
	*data_size = internal_record_entry->value_data_size;

	return( 1 );
}

/* Retrieves the value data
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_value_data(
     libpff_record_entry_t *record_entry,
     uint8_t **value_data,
     size_t *value_data_size,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_value_data";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

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
	if( value_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data size.",
		 function );

		return( -1 );
	}
	*value_data      = internal_record_entry->value_data;
	*value_data_size = internal_record_entry->value_data_size;

	return( 1 );
}

/* Sets the value data in the record entry
 * The function makes a local copy
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_set_value_data(
     libpff_record_entry_t *record_entry,
     const uint8_t *value_data,
     size_t value_data_size,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_set_value_data";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

	if( internal_record_entry->value_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid record entry - value data already set.",
		 function );

		return( -1 );
	}
	if( value_data_size > 0 )
	{
		if( value_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid value data.",
			 function );

			goto on_error;
		}
		if( value_data_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid value data size value exceeds maximum allocation size.",
			 function );

			goto on_error;
		}
		internal_record_entry->value_data = (uint8_t *) memory_allocate(
		                                                 value_data_size );

		if( internal_record_entry->value_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value data.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     internal_record_entry->value_data,
		     value_data,
		     value_data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy value data.",
			 function );

			goto on_error;
		}
		internal_record_entry->value_data_size = value_data_size;
	}
	return( 1 );

on_error:
	if( internal_record_entry->value_data != NULL )
	{
		memory_free(
		 internal_record_entry->value_data );

		internal_record_entry->value_data = NULL;
	}
	internal_record_entry->value_data_size = 0;

	return( -1 );
}

/* Sets the value data from the list in the record entry
 * The function makes a local copy
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_set_value_data_from_list(
     libpff_record_entry_t *record_entry,
     libbfio_handle_t *file_io_handle,
     libfdata_list_t *value_data_list,
     libfcache_cache_t *value_data_cache,
     libcerror_error_t **error )
{
	libfdata_stream_t *value_data_stream = NULL;
	static char *function                = "libpff_record_entry_set_value_data_from_list";

	if( libpff_descriptor_data_stream_initialize(
	     &value_data_stream,
	     value_data_list,
	     value_data_cache,
	     LIBPFF_DESCRIPTOR_DATA_STREAM_DATA_HANDLE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create descriptor data stream.",
		 function );

		goto on_error;
	}
	if( libpff_record_entry_set_value_data_from_stream(
	     record_entry,
	     file_io_handle,
	     value_data_stream,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set value data from stream.",
		 function );

		goto on_error;
	}
	if( libfdata_stream_free(
	     &value_data_stream,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free descriptor data stream.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( value_data_stream != NULL )
	{
		libfdata_stream_free(
		 &value_data_stream,
		 NULL );
	}
	return( -1 );
}

/* Sets the value data from the stream in the record entry
 * The function makes a local copy
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_set_value_data_from_stream(
     libpff_record_entry_t *record_entry,
     libbfio_handle_t *file_io_handle,
     libfdata_stream_t *value_data_stream,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_set_value_data_from_stream";
	size64_t value_data_size                              = 0;
	ssize_t read_count                                    = 0;

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

	if( internal_record_entry->value_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid record entry - value data already set.",
		 function );

		return( -1 );
	}
	if( libfdata_stream_get_size(
	     value_data_stream,
	     &value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data stream size.",
		 function );

		return( -1 );
	}
	if( value_data_size > 0 )
	{
		if( value_data_size > (size64_t) MEMORY_MAXIMUM_ALLOCATION_SIZE )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid value data size value exceeds maximum allocation size.",
			 function );

			goto on_error;
		}
		internal_record_entry->value_data = (uint8_t *) memory_allocate(
		                                                 (size_t) value_data_size );

		if( internal_record_entry->value_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value data.",
			 function );

			goto on_error;
		}
		internal_record_entry->value_data_size = (size_t) value_data_size;

		read_count = libfdata_stream_read_buffer_at_offset(
			      value_data_stream,
			      (intptr_t *) file_io_handle,
			      internal_record_entry->value_data,
			      internal_record_entry->value_data_size,
			      0,
			      0,
			      error );

		if( read_count != (ssize_t) internal_record_entry->value_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read buffer from value data stream at offset: 0 (0x00000000).",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( internal_record_entry->value_data != NULL )
	{
		memory_free(
		 internal_record_entry->value_data );

		internal_record_entry->value_data = NULL;
	}
	internal_record_entry->value_data_size = 0;

	return( -1 );
}

/* Retrieves the data
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_data(
     libpff_record_entry_t *record_entry,
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_data";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

	if( internal_record_entry->value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record entry - missing value data.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( data_size < internal_record_entry->value_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid data value too small.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     data,
	     internal_record_entry->value_data,
	     internal_record_entry->value_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy value data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the data as a boolean value
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_data_as_boolean(
     libpff_record_entry_t *record_entry,
     uint8_t *value_boolean,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_data_as_boolean";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

	if( internal_record_entry->identifier.value_type != LIBPFF_VALUE_TYPE_BOOLEAN )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value type.",
		 function );

		return( -1 );
	}
	if( internal_record_entry->value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record entry - missing value data.",
		 function );

		return( -1 );
	}
	if( internal_record_entry->value_data_size != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value data size.",
		 function );

		return( -1 );
	}
	if( value_boolean == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value boolean.",
		 function );

		return( -1 );
	}
	*value_boolean = internal_record_entry->value_data[ 0 ];

	return( 1 );
}

/* Retrieves the data as a 16-bit integer value
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_data_as_16bit_integer(
     libpff_record_entry_t *record_entry,
     uint16_t *value_16bit,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_data_as_16bit_integer";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

	if( internal_record_entry->identifier.value_type != LIBPFF_VALUE_TYPE_INTEGER_16BIT_SIGNED )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value type.",
		 function );

		return( -1 );
	}
	if( internal_record_entry->value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record entry - missing value data.",
		 function );

		return( -1 );
	}
	if( internal_record_entry->value_data_size != 2 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value data size.",
		 function );

		return( -1 );
	}
	if( value_16bit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value 16-bit.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint16_little_endian(
	 internal_record_entry->value_data,
	 *value_16bit );

	return( 1 );
}

/* Retrieves the data as a 32-bit integer value
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_data_as_32bit_integer(
     libpff_record_entry_t *record_entry,
     uint32_t *value_32bit,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_data_as_32bit_integer";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

	if( internal_record_entry->identifier.value_type != LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value type.",
		 function );

		return( -1 );
	}
	if( internal_record_entry->value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record entry - missing value data.",
		 function );

		return( -1 );
	}
	if( internal_record_entry->value_data_size != 4 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value data size.",
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
	byte_stream_copy_to_uint32_little_endian(
	 internal_record_entry->value_data,
	 *value_32bit );

	return( 1 );
}

/* Retrieves the data as a 64-bit integer value
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_data_as_64bit_integer(
     libpff_record_entry_t *record_entry,
     uint64_t *value_64bit,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_data_as_64bit_integer";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

	if( internal_record_entry->identifier.value_type != LIBPFF_VALUE_TYPE_INTEGER_64BIT_SIGNED )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value type.",
		 function );

		return( -1 );
	}
	if( internal_record_entry->value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record entry - missing value data.",
		 function );

		return( -1 );
	}
	if( internal_record_entry->value_data_size != 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value data size.",
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
	byte_stream_copy_to_uint64_little_endian(
	 internal_record_entry->value_data,
	 *value_64bit );

	return( 1 );
}

/* Retrieves the data as a 64-bit FILETIME value
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_data_as_filetime(
     libpff_record_entry_t *record_entry,
     uint64_t *filetime,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_data_as_filetime";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

	if( internal_record_entry->identifier.value_type != LIBPFF_VALUE_TYPE_FILETIME )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value type.",
		 function );

		return( -1 );
	}
	if( internal_record_entry->value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record entry - missing value data.",
		 function );

		return( -1 );
	}
	if( internal_record_entry->value_data_size != 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value data size.",
		 function );

		return( -1 );
	}
	if( filetime == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filetime.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint64_little_endian(
	 internal_record_entry->value_data,
	 *filetime );

	return( 1 );
}

/* Retrieves the data as a 64-bit floatingtime value
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_data_as_floatingtime(
     libpff_record_entry_t *record_entry,
     uint64_t *floatingtime,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_data_as_floatingtime";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

	if( internal_record_entry->identifier.value_type != LIBPFF_VALUE_TYPE_FLOATINGTIME )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value type.",
		 function );

		return( -1 );
	}
	if( internal_record_entry->value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record entry - missing value data.",
		 function );

		return( -1 );
	}
	if( internal_record_entry->value_data_size != 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value data size.",
		 function );

		return( -1 );
	}
	if( floatingtime == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid floatingtime.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint64_little_endian(
	 internal_record_entry->value_data,
	 *floatingtime );

	return( 1 );
}

/* Retrieves the data as a size value
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_data_as_size(
     libpff_record_entry_t *record_entry,
     size64_t *value_size,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_data_as_size";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

	if( ( internal_record_entry->identifier.value_type != LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED )
	 && ( internal_record_entry->identifier.value_type != LIBPFF_VALUE_TYPE_INTEGER_64BIT_SIGNED ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value type.",
		 function );

		return( -1 );
	}
	if( internal_record_entry->value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record entry - missing value data.",
		 function );

		return( -1 );
	}
	if( value_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value size.",
		 function );

		return( -1 );
	}
	if( ( internal_record_entry->value_data_size != 4 )
	 && ( internal_record_entry->value_data_size != 8 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value data size.",
		 function );

		return( -1 );
	}
	if( internal_record_entry->value_data_size == 4 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 internal_record_entry->value_data,
		 *value_size );
	}
	else if( internal_record_entry->value_data_size == 8 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 internal_record_entry->value_data,
		 *value_size );
	}
	return( 1 );
}

/* Retrieves the data as a floating point value
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_data_as_floating_point(
     libpff_record_entry_t *record_entry,
     double *value_floating_point,
     libcerror_error_t **error )
{
	byte_stream_float64_t value_double;
	byte_stream_float32_t value_float;

	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_data_as_floating_point";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

	if( ( internal_record_entry->identifier.value_type != LIBPFF_VALUE_TYPE_FLOAT_32BIT )
	 && ( internal_record_entry->identifier.value_type != LIBPFF_VALUE_TYPE_DOUBLE_64BIT ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value type.",
		 function );

		return( -1 );
	}
	if( internal_record_entry->value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record entry - missing value data.",
		 function );

		return( -1 );
	}
	if( value_floating_point == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value floating point.",
		 function );

		return( -1 );
	}
	if( ( internal_record_entry->value_data_size != 4 )
	 && ( internal_record_entry->value_data_size != 8 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value data size.",
		 function );

		return( -1 );
	}
	if( internal_record_entry->value_data_size == 4 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 internal_record_entry->value_data,
		 value_float.integer );

		*value_floating_point = (double) value_float.floating_point;
	}
	else if( internal_record_entry->value_data_size == 8 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 internal_record_entry->value_data,
		 value_double.integer );

		*value_floating_point = value_double.floating_point;
	}
	return( 1 );
}

/* Retrieves the UTF-8 string size
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_data_as_utf8_string_size_with_codepage(
     libpff_record_entry_t *record_entry,
     int ascii_codepage,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_data_as_utf8_string_size_with_codepage";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

	if( libpff_mapi_value_get_data_as_utf8_string_size(
	     internal_record_entry->identifier.value_type,
	     internal_record_entry->value_data,
	     internal_record_entry->value_data_size,
	     ascii_codepage,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine size of value data as UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 string value
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_data_as_utf8_string_with_codepage(
     libpff_record_entry_t *record_entry,
     int ascii_codepage,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_data_as_utf8_string_with_codepage";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

	if( libpff_mapi_value_get_data_as_utf8_string(
	     internal_record_entry->identifier.value_type,
	     internal_record_entry->value_data,
	     internal_record_entry->value_data_size,
	     ascii_codepage,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data as UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Compares the value data with an UTF-8 string
 * Returns 1 if the strings are equal, 0 if not or -1 on error
 */
int libpff_record_entry_compare_value_with_utf8_string_with_codepage(
     libpff_record_entry_t *record_entry,
     int ascii_codepage,
     const uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_compare_value_with_utf8_string_with_codepage";
	uint8_t is_ascii_string                               = 0;
	int result                                            = 0;

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

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
	if( ( internal_record_entry->value_data == NULL )
	 || ( internal_record_entry->value_data_size == 0 ) )
	{
		return( 0 );
	}
	if( internal_record_entry->identifier.value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
	{
		is_ascii_string = 1;
	}
	/* Codepage 1200 represents Unicode
	 * If the codepage is 1200 find out if the string is encoded in UTF-8 or UTF-16 little-endian
	 */
	if( ( is_ascii_string != 0 )
	 && ( ascii_codepage == 1200 ) )
	{
		result = libpff_mapi_value_data_contains_zero_bytes(
			  internal_record_entry->value_data,
			  internal_record_entry->value_data_size,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if value data contains zero bytes.",
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
		result = libuna_utf8_string_compare_with_utf16_stream(
			  utf8_string,
			  utf8_string_size,
			  internal_record_entry->value_data,
			  internal_record_entry->value_data_size,
			  LIBUNA_ENDIAN_LITTLE,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare UTF-8 string with UTF-16 stream.",
			 function );

			return( -1 );
		}
	}
	/* Codepage 65000 represents UTF-7
	 */
	else if( ascii_codepage == 65000 )
	{
		result = libuna_utf8_string_compare_with_utf7_stream(
			  utf8_string,
			  utf8_string_size,
			  internal_record_entry->value_data,
			  internal_record_entry->value_data_size,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare UTF-8 string with UTF-7 stream.",
			 function );

			return( -1 );
		}
	}
	/* Codepage 1200 or 65001 represents UTF-8
	 */
	else if( ( ascii_codepage == 1200 )
	      || ( ascii_codepage == 65001 ) )
	{
		result = libuna_utf8_string_compare_with_utf8_stream(
			  utf8_string,
			  utf8_string_size,
			  internal_record_entry->value_data,
			  internal_record_entry->value_data_size,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare UTF-8 string with UTF-8 stream.",
			 function );

			return( -1 );
		}
	}
	else
	{
		/* TODO currently libuna uses the same numeric values for the codepages as PFF
		 * add a mapping function if this implementation changes
		 */
		result = libuna_utf8_string_compare_with_byte_stream(
			  utf8_string,
			  utf8_string_size,
			  internal_record_entry->value_data,
			  internal_record_entry->value_data_size,
			  ascii_codepage,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare UTF-8 string with byte stream.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the data formatted as an UTF-8 string
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_data_as_utf8_string_size(
     libpff_record_entry_t *record_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_data_as_utf8_string_size";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

	if( ( internal_record_entry->identifier.value_type != LIBPFF_VALUE_TYPE_STRING_ASCII )
	 && ( internal_record_entry->identifier.value_type != LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value type.",
		 function );

		return( -1 );
	}
	if( libpff_mapi_value_get_data_as_utf8_string_size(
	     internal_record_entry->identifier.value_type,
	     internal_record_entry->value_data,
	     internal_record_entry->value_data_size,
	     internal_record_entry->ascii_codepage,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine size of value data as UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the data formatted as an UTF-8 string
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_data_as_utf8_string(
     libpff_record_entry_t *record_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_data_as_utf8_string";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

	if( ( internal_record_entry->identifier.value_type != LIBPFF_VALUE_TYPE_STRING_ASCII )
	 && ( internal_record_entry->identifier.value_type != LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value type.",
		 function );

		return( -1 );
	}
	if( libpff_mapi_value_get_data_as_utf8_string(
	     internal_record_entry->identifier.value_type,
	     internal_record_entry->value_data,
	     internal_record_entry->value_data_size,
	     internal_record_entry->ascii_codepage,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data as UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 string size
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_data_as_utf16_string_size_with_codepage(
     libpff_record_entry_t *record_entry,
     int ascii_codepage,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_data_as_utf16_string_size_with_codepage";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

	if( libpff_mapi_value_get_data_as_utf16_string_size(
	     internal_record_entry->identifier.value_type,
	     internal_record_entry->value_data,
	     internal_record_entry->value_data_size,
	     ascii_codepage,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine size of value data as UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 string value
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_data_as_utf16_string_with_codepage(
     libpff_record_entry_t *record_entry,
     int ascii_codepage,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_data_as_utf16_string_with_codepage";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

	if( libpff_mapi_value_get_data_as_utf16_string(
	     internal_record_entry->identifier.value_type,
	     internal_record_entry->value_data,
	     internal_record_entry->value_data_size,
	     ascii_codepage,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data as UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Compares the value data with an UTF-16 string
 * Returns 1 if the strings are equal, 0 if not or -1 on error
 */
int libpff_record_entry_compare_value_with_utf16_string_with_codepage(
     libpff_record_entry_t *record_entry,
     int ascii_codepage,
     const uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_compare_value_with_utf16_string_with_codepage";
	uint8_t is_ascii_string                               = 0;
	int result                                            = 0;

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

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
	if( ( internal_record_entry->value_data == NULL )
	 || ( internal_record_entry->value_data_size == 0 ) )
	{
		return( 0 );
	}
	if( internal_record_entry->identifier.value_type == LIBPFF_VALUE_TYPE_STRING_ASCII )
	{
		is_ascii_string = 1;
	}
	/* Codepage 1200 represents Unicode
	 * If the codepage is 1200 find out if the string is encoded in UTF-8 or UTF-16 little-endian
	 */
	if( ( is_ascii_string != 0 )
	 && ( ascii_codepage == 1200 ) )
	{
		result = libpff_mapi_value_data_contains_zero_bytes(
			  internal_record_entry->value_data,
			  internal_record_entry->value_data_size,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if value data contains zero bytes.",
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
		result = libuna_utf16_string_compare_with_utf16_stream(
		          utf16_string,
		          utf16_string_size,
			  internal_record_entry->value_data,
			  internal_record_entry->value_data_size,
		          LIBUNA_ENDIAN_LITTLE,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare UTF-16 string with UTF-16 stream.",
			 function );

			return( -1 );
		}
	}
	/* Codepage 65000 represents UTF-7
	 */
	else if( ascii_codepage == 65000 )
	{
		result = libuna_utf16_string_compare_with_utf7_stream(
			  utf16_string,
			  utf16_string_size,
			  internal_record_entry->value_data,
			  internal_record_entry->value_data_size,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare UTF-16 string with UTF-7 stream.",
			 function );

			return( -1 );
		}
	}
	/* Codepage 1200 or 65001 represents UTF-8
	 */
	else if( ( ascii_codepage == 1200 )
	      || ( ascii_codepage == 65001 ) )
	{
		result = libuna_utf16_string_compare_with_utf8_stream(
			  utf16_string,
			  utf16_string_size,
			  internal_record_entry->value_data,
			  internal_record_entry->value_data_size,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare UTF-16 string with UTF-8 stream.",
			 function );

			return( -1 );
		}
	}
	else
	{
		/* TODO currently libuna uses the same numeric values for the codepages as PFF
		 * add a mapping function if this implementation changes
		 */
		result = libuna_utf16_string_compare_with_byte_stream(
			  utf16_string,
			  utf16_string_size,
			  internal_record_entry->value_data,
			  internal_record_entry->value_data_size,
			  ascii_codepage,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare UTF-16 string with byte stream.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the data formatted as an UTF-16 string
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_data_as_utf16_string_size(
     libpff_record_entry_t *record_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_data_as_utf16_string_size";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

	if( ( internal_record_entry->identifier.value_type != LIBPFF_VALUE_TYPE_STRING_ASCII )
	 && ( internal_record_entry->identifier.value_type != LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value type.",
		 function );

		return( -1 );
	}
	if( libpff_mapi_value_get_data_as_utf16_string_size(
	     internal_record_entry->identifier.value_type,
	     internal_record_entry->value_data,
	     internal_record_entry->value_data_size,
	     internal_record_entry->ascii_codepage,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine size of value data as UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the data formatted as an UTF-16 string
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_data_as_utf16_string(
     libpff_record_entry_t *record_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_data_as_utf16_string";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

	if( ( internal_record_entry->identifier.value_type != LIBPFF_VALUE_TYPE_STRING_ASCII )
	 && ( internal_record_entry->identifier.value_type != LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value type.",
		 function );

		return( -1 );
	}
	if( libpff_mapi_value_get_data_as_utf16_string(
	     internal_record_entry->identifier.value_type,
	     internal_record_entry->value_data,
	     internal_record_entry->value_data_size,
	     internal_record_entry->ascii_codepage,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data as UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the data as a GUID value
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_get_data_as_guid(
     libpff_record_entry_t *record_entry,
     uint8_t *guid_data,
     size_t guid_data_size,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_data_as_guid";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

	if( internal_record_entry->identifier.value_type != LIBPFF_VALUE_TYPE_GUID )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid record entry - unsupported value type.",
		 function );

		return( -1 );
	}
	if( internal_record_entry->value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record entry - missing value data.",
		 function );

		return( -1 );
	}
	if( internal_record_entry->value_data_size != 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value data size.",
		 function );

		return( -1 );
	}
	if( guid_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid GUID data.",
		 function );

		return( -1 );
	}
	if( guid_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: GUID data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( guid_data_size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: GUID data size value too small.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     guid_data,
	     internal_record_entry->value_data,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy value data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Copies the value data to an object identifier
 * Returns 1 if successful or -1 on error
 */
int libpff_record_entry_copy_object_identifier(
     libpff_record_entry_t *record_entry,
     uint32_t *object_identifier,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_copy_object_identifier";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

	if( internal_record_entry->identifier.value_type != LIBPFF_VALUE_TYPE_OBJECT )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value type.",
		 function );

		return( -1 );
	}
	if( internal_record_entry->value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record entry - missing value data.",
		 function );

		return( -1 );
	}
	if( object_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid object identifier.",
		 function );

		return( -1 );
	}
	/* The value data size of an object is 8
	 * where the first 4 bytes are the identifier
	 */
	if( internal_record_entry->value_data_size != 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value data size.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 internal_record_entry->value_data,
	 *object_identifier );

	return( 1 );
}

/* Retrieves the multi value
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_record_entry_get_multi_value(
     libpff_record_entry_t *record_entry,
     libpff_multi_value_t **multi_value,
     libcerror_error_t **error )
{
	libpff_internal_multi_value_t *internal_multi_value   = NULL;
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_get_entry_value";
	size_t value_size                                     = 0;
	uint32_t number_of_values                             = 0;
	uint32_t value_index                                  = 0;
	uint32_t value_type                                   = 0;

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

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
		 "%s: multi value already set.",
		 function );

		return( -1 );
	}
	if( ( internal_record_entry->value_data == NULL )
	 || ( internal_record_entry->value_data_size == 0 ) )
	{
		return( 0 );
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
	if( ( value_type & LIBPFF_VALUE_TYPE_MULTI_VALUE_FLAG ) == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value type: 0x%04" PRIx32 ".",
		 function,
		 value_type );

		goto on_error;
	}
	if( libpff_multi_value_initialize(
	     multi_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create multi value.",
		 function );

		goto on_error;
	}
	internal_multi_value = (libpff_internal_multi_value_t *) *multi_value;

	internal_multi_value->value_type = value_type;

/* TODO is there a need to copy the multi value data ? use fvalue data handle instead ? */

	/* Internally an empty multi value is represented by a NULL reference
	 */
	if( internal_record_entry->value_data != NULL )
	{
		if( ( internal_record_entry->value_data_size == 0 )
		 || ( internal_record_entry->value_data_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid record entry - value data size value out of bounds.",
			 function );

			goto on_error;
		}
		internal_multi_value->value_data = (uint8_t *) memory_allocate(
								sizeof( uint8_t ) * internal_record_entry->value_data_size );

		if( internal_multi_value->value_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create multi value data.",
			 function );

			goto on_error;
		}
		internal_multi_value->value_data_size = internal_record_entry->value_data_size;

		if( memory_copy(
		     internal_multi_value->value_data,
		     internal_record_entry->value_data,
		     internal_multi_value->value_data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to set multi value data.",
			 function );

			goto on_error;
		}
		switch( internal_multi_value->value_type )
		{
			case LIBPFF_VALUE_TYPE_MULTI_VALUE_INTEGER_16BIT_SIGNED:
				value_size = 2;
				break;

			case LIBPFF_VALUE_TYPE_MULTI_VALUE_INTEGER_32BIT_SIGNED:
				value_size = 4;
				break;

			case LIBPFF_VALUE_TYPE_MULTI_VALUE_INTEGER_64BIT_SIGNED:
			case LIBPFF_VALUE_TYPE_MULTI_VALUE_FILETIME:
				value_size = 8;
				break;

			case LIBPFF_VALUE_TYPE_MULTI_VALUE_GUID:
				value_size = 16;
				break;

			case LIBPFF_VALUE_TYPE_MULTI_VALUE_STRING_ASCII:
			case LIBPFF_VALUE_TYPE_MULTI_VALUE_STRING_UNICODE:
			case LIBPFF_VALUE_TYPE_MULTI_VALUE_BINARY_DATA:
				/* The first 4 bytes contain the number of values
				 */
				byte_stream_copy_from_uint32_little_endian(
				 internal_record_entry->value_data,
				 internal_multi_value->number_of_values );

				break;

			default:
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported value type: 0x%04" PRIx32 ".",
				 function,
				 internal_multi_value->value_type );

				goto on_error;
		}
		if( value_size != 0 )
		{
			if( ( internal_record_entry->value_data_size % value_size ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: value data size: %" PRIzd " not a multitude of value size: %" PRIzd ".",
				 function,
				 internal_record_entry->value_data_size,
				 value_size );

				goto on_error;
			}
			number_of_values = internal_record_entry->value_data_size / value_size;

#if SIZE_MAX > UINT32_MAX
			if( number_of_values > UINT32_MAX )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
				 "%s: number of values value exceeds maximum.\n",
				 function );

				goto on_error;
			}
#endif
			internal_multi_value->number_of_values = (uint32_t) number_of_values;
		}
		if( internal_multi_value->number_of_values > 0 )
		{
			if( ( internal_multi_value->number_of_values > (size_t) ( MEMORY_MAXIMUM_ALLOCATION_SIZE / sizeof( uint32_t ) ) )
			 || ( internal_multi_value->number_of_values > (size_t) ( MEMORY_MAXIMUM_ALLOCATION_SIZE / sizeof( size_t ) ) ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
				 "%s: invalid multi value - number of values exceeds maximum allocatio size.",
				 function );

				goto on_error;
			}
			internal_multi_value->value_offset = (uint32_t *) memory_allocate(
									   sizeof( uint32_t ) * internal_multi_value->number_of_values );

			if( internal_multi_value->value_offset == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create multi value offsets.",
				 function );

				goto on_error;
			}
			internal_multi_value->value_size = (size_t *) memory_allocate(
								       sizeof( size_t ) * internal_multi_value->number_of_values );

			if( internal_multi_value->value_offset == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create multi value offsets.",
				 function );

				goto on_error;
			}
			for( value_index = 0;
			     value_index < internal_multi_value->number_of_values;
			     value_index++ )
			{
				if( ( internal_multi_value->value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_STRING_ASCII )
				 || ( internal_multi_value->value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_STRING_UNICODE )
				 || ( internal_multi_value->value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_BINARY_DATA ) )
				{
					internal_record_entry->value_data += 4;

					byte_stream_copy_to_uint32_little_endian(
					 internal_record_entry->value_data,
					 internal_multi_value->value_offset[ value_index ] );

					if( internal_multi_value->value_offset[ value_index ] > internal_multi_value->value_data_size )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
						 "%s: value offset: %" PRIu32 " exceeds value data size: %" PRIzd ".",
						 function,
						 internal_multi_value->value_offset[ value_index ],
						 internal_multi_value->value_data_size );

						goto on_error;
					}
					if( value_index > 0 )
					{
						internal_multi_value->value_size[ value_index - 1 ] = internal_multi_value->value_offset[ value_index ]
						                                                    - internal_multi_value->value_offset[ value_index - 1 ];
					}
				}
				else
				{
					internal_multi_value->value_offset[ value_index ] = (uint32_t) ( value_index * value_size );
					internal_multi_value->value_size[ value_index ]   = value_size;
				}
			}
			if( ( internal_multi_value->value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_STRING_ASCII )
			 || ( internal_multi_value->value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_STRING_UNICODE )
			 || ( internal_multi_value->value_type == LIBPFF_VALUE_TYPE_MULTI_VALUE_BINARY_DATA ) )
			{
				internal_multi_value->value_size[ value_index - 1 ] = internal_multi_value->value_data_size
				                                                    - internal_multi_value->value_offset[ value_index - 1 ];
			}
/* TODO refactor
			internal_multi_value->ascii_codepage = ascii_codepage;
*/
		}
	}
	return( 1 );

on_error:
	if( *multi_value != NULL )
	{
		libpff_multi_value_free(
		 multi_value,
		 NULL );
	}
	return( -1 );
}

/* Reads value data from the current offset into a buffer
 * Returns the number of bytes read or -1 on error
 */
ssize_t libpff_record_entry_read_buffer(
         libpff_record_entry_t *record_entry,
         uint8_t *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_read_buffer";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

	if( internal_record_entry->value_data_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid record entry - value data offset value out of bounds.",
		 function );

		return( -1 );
	}
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
	if( ( internal_record_entry->value_data == NULL )
	 || ( internal_record_entry->value_data_offset >= (off64_t) internal_record_entry->value_data_size ) )
	{
		return( 0 );
	}
	if( ( buffer_size > internal_record_entry->value_data_size )
	 || ( ( (size_t) internal_record_entry->value_data_offset + buffer_size ) > internal_record_entry->value_data_size ) )
	{
		buffer_size = (size_t) ( internal_record_entry->value_data_size - internal_record_entry->value_data_offset );
	}
	if( memory_copy(
	     buffer,
	     &( internal_record_entry->value_data[ internal_record_entry->value_data_offset ] ),
	     buffer_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy value data to buffer.",
		 function );

		return( -1 );
	}
	internal_record_entry->value_data_offset += buffer_size;

	return( (ssize_t) buffer_size );
}

/* Seeks a certain offset of the value data
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libpff_record_entry_seek_offset(
         libpff_record_entry_t *record_entry,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	static char *function                                 = "libpff_record_entry_seek_offset";

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	internal_record_entry = (libpff_internal_record_entry_t *) record_entry;

	if( internal_record_entry->value_data_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid record entry - value data offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( whence != SEEK_CUR )
	 && ( whence != SEEK_END )
	 && ( whence != SEEK_SET ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported whence.",
		 function );

		return( -1 );
	}
	if( whence == SEEK_CUR )
	{
		offset += internal_record_entry->value_data_offset;
	}
	else if( whence == SEEK_END )
	{
		offset += (off64_t) internal_record_entry->value_data_size;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: seeking offset: %" PRIi64 ".\n",
		 function,
		 offset );
	}
#endif
	if( offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: offset value out of bounds.",
		 function );

		return( -1 );
	}
	internal_record_entry->value_data_offset = offset;

	return( offset );
}

