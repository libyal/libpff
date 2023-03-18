/*
 * Local descriptor value functions
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
#include "libpff_io_handle.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_local_descriptor_value.h"

#include "pff_local_descriptors_node.h"

/* Creates a local descriptor value
 * Make sure the value local_descriptor_value is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptor_value_initialize(
     libpff_local_descriptor_value_t **local_descriptor_value,
     libcerror_error_t **error )
{
	static char *function = "libpff_local_descriptor_value_initialize";

	if( local_descriptor_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local descriptor value.",
		 function );

		return( -1 );
	}
	if( *local_descriptor_value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid local descriptor value value already set.",
		 function );

		return( -1 );
	}
	*local_descriptor_value = memory_allocate_structure(
	                           libpff_local_descriptor_value_t );

	if( *local_descriptor_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create local descriptor value.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *local_descriptor_value,
	     0,
	     sizeof( libpff_local_descriptor_value_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear local descriptor value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *local_descriptor_value != NULL )
	{
		memory_free(
		 *local_descriptor_value );

		*local_descriptor_value = NULL;
	}
	return( -1 );
}

/* Frees a local descriptor value
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptor_value_free(
     libpff_local_descriptor_value_t **local_descriptor_value,
     libcerror_error_t **error )
{
	static char *function = "libpff_local_descriptor_value_free";

	if( local_descriptor_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local descriptor value.",
		 function );

		return( -1 );
	}
	if( *local_descriptor_value != NULL )
	{
		memory_free(
		 *local_descriptor_value );

		*local_descriptor_value = NULL;
	}
	return( 1 );
}

/* Reads the local descriptor value data
 * Returns 1 if successful or -1 on error
 */
int libpff_local_descriptor_value_read_data(
     libpff_local_descriptor_value_t *local_descriptor_value,
     libpff_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libpff_local_descriptor_value_read_data";
	size_t value_size     = 0;

	if( local_descriptor_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local descriptor value.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
	{
		value_size = sizeof( pff_local_descriptor_leaf_node_entry_type_32bit_t );
	}
	else if( ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
	      || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
	{
		value_size = sizeof( pff_local_descriptor_leaf_node_entry_type_64bit_t );
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
	if( ( data_size < value_size )
	 || ( data_size > SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: unsupported data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: local descriptor value data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 value_size,
		 0 );
	}
#endif
	if( io_handle->file_type == LIBPFF_FILE_TYPE_32BIT )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_local_descriptor_leaf_node_entry_type_32bit_t *) data )->identifier,
		 local_descriptor_value->identifier );

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_local_descriptor_leaf_node_entry_type_32bit_t *) data )->data_identifier,
		 local_descriptor_value->data_identifier );

		byte_stream_copy_to_uint32_little_endian(
		 ( (pff_local_descriptor_leaf_node_entry_type_32bit_t *) data )->local_descriptors_identifier,
		 local_descriptor_value->local_descriptors_identifier );
	}
	else if( ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT )
	      || ( io_handle->file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE ) )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (pff_local_descriptor_leaf_node_entry_type_64bit_t *) data )->identifier,
		 local_descriptor_value->identifier );

		byte_stream_copy_to_uint64_little_endian(
		 ( (pff_local_descriptor_leaf_node_entry_type_64bit_t *) data )->data_identifier,
		 local_descriptor_value->data_identifier );

		byte_stream_copy_to_uint64_little_endian(
		 ( (pff_local_descriptor_leaf_node_entry_type_64bit_t *) data )->local_descriptors_identifier,
		 local_descriptor_value->local_descriptors_identifier );
	}
	/* Ignore the upper 32-bit of descriptor identifiers
	 */
	local_descriptor_value->identifier &= 0xffffffffUL;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: identifier\t\t\t\t: %" PRIu64 " (0x%08" PRIx64 ")\n",
		 function,
		 local_descriptor_value->identifier,
		 local_descriptor_value->identifier );

		libcnotify_printf(
		 "%s: data identifier\t\t\t\t: %" PRIu64 " (0x%08" PRIx64 ")\n",
		 function,
		 local_descriptor_value->data_identifier,
		 local_descriptor_value->data_identifier );

		libcnotify_printf(
		 "%s: local descriptors identifier\t\t\t: %" PRIu64 " (0x%08" PRIx64 ")\n",
		 function,
		 local_descriptor_value->local_descriptors_identifier,
		 local_descriptor_value->local_descriptors_identifier );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

