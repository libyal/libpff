/*
 * Table header functions
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

#include "libpff_debug.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_table_header.h"

#include "pff_table.h"

/* Creates a table header
 * Make sure the value table_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_table_header_initialize(
     libpff_table_header_t **table_header,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_header_initialize";

	if( table_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table_header.",
		 function );

		return( -1 );
	}
	if( *table_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid table header value already set.",
		 function );

		return( -1 );
	}
	*table_header = memory_allocate_structure(
	                 libpff_table_header_t );

	if( *table_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create table header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *table_header,
	     0,
	     sizeof( libpff_table_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear table header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *table_header != NULL )
	{
		memory_free(
		 *table_header );

		*table_header = NULL;
	}
	return( -1 );
}

/* Frees a table header
 * Returns 1 if successful or -1 on error
 */
int libpff_table_header_free(
     libpff_table_header_t **table_header,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_header_free";

	if( table_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table header.",
		 function );

		return( -1 );
	}
	if( *table_header != NULL )
	{
		if( ( *table_header )->column_definitions_data != NULL )
		{
			memory_free(
			 ( *table_header )->column_definitions_data );
		}
		memory_free(
		 *table_header );

		*table_header = NULL;
	}
	return( 1 );
}

/* Reads the table header data
 * Returns 1 if successful or -1 on error
 */
int libpff_table_header_read_data(
     libpff_table_header_t *table_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_header_read_data";

	if( table_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table header.",
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
	if( ( data_size < sizeof( pff_table_t ) )
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
		 "%s: table header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( pff_table_t ),
		 0 );
	}
#endif
	if( ( (pff_table_t *) data )->signature != 0xec )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported table signature: 0x%02" PRIx8 ".",
		 function,
		 ( (pff_table_t *) data )->signature );

		return( -1 );
	}
	table_header->type = ( (pff_table_t *) data )->type;

	byte_stream_copy_to_uint32_little_endian(
	 ( (pff_table_t *) data )->value_reference,
	 table_header->table_value_reference );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table signature\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (pff_table_t *) data )->signature );

		libcnotify_printf(
		 "%s: table type\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 table_header->type );

		libcnotify_printf(
		 "%s: table value reference\t\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 table_header->table_value_reference,
		 libpff_debug_get_node_identifier_type(
		  (uint8_t) ( table_header->table_value_reference & 0x0000001fUL ) ) );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( ( table_header->type != 0x6c )
	 && ( table_header->type != 0x7c )
	 && ( table_header->type != 0x8c )
	 && ( table_header->type != 0x9c )
	 && ( table_header->type != 0xa5 )
	 && ( table_header->type != 0xac )
	 && ( table_header->type != 0xbc ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported table type: 0x%02" PRIx8 ".",
		 function,
		 table_header->type );

		return( -1 );
	}
	return( 1 );
}

/* Reads the 6c table header data
 * Returns 1 if successful or -1 on error
 */
int libpff_table_header_read_6c_data(
     libpff_table_header_t *table_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_header_read_6c_data";

	if( table_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table header.",
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
	if( data_size != 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: unsupported data size: %" PRIzd ".",
		 function,
		 data_size );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: 6c table header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
	/* The 6c table header contains no type indicator
	 */
	byte_stream_copy_to_uint32_little_endian(
	 data,
	 table_header->b5_table_header_reference );

	byte_stream_copy_to_uint32_little_endian(
	 &( data[ 4 ] ),
	 table_header->values_array_reference );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: b5 table header reference\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 table_header->b5_table_header_reference,
		 libpff_debug_get_node_identifier_type(
		  (uint8_t) ( table_header->b5_table_header_reference & 0x0000001fUL ) ) );

		libcnotify_printf(
		 "%s: values array reference\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 table_header->values_array_reference,
		 libpff_debug_get_node_identifier_type(
		  (uint8_t) ( table_header->values_array_reference & 0x0000001fUL ) ) );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

/* Reads the 7c table header data
 * Returns 1 if successful or -1 on error
 */
int libpff_table_header_read_7c_data(
     libpff_table_header_t *table_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function               = "libpff_table_header_read_7c_data";
	size_t column_definitions_data_size = 0;
	size_t data_offset                  = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint16_t value_16bit                = 0;
#endif

	if( table_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table header.",
		 function );

		return( -1 );
	}
	if( table_header->column_definitions_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid table header - column definitions data value already set.",
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
	if( ( data_size < sizeof( pff_table_header_7c_t ) )
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
		 "%s: 7c table header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( pff_table_header_7c_t ),
		 0 );
	}
#endif
	if( ( (pff_table_header_7c_t *) data )->type != 0x7c )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported table header type: 0x%02x.",
		 function,
		 ( (pff_table_header_7c_t *) data )->type );

		goto on_error;
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (pff_table_header_7c_t *) data )->values_array_end_offset_cell_existence_block,
	 table_header->values_array_entry_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (pff_table_header_7c_t *) data )->b5_table_header_reference,
	 table_header->b5_table_header_reference );

	byte_stream_copy_to_uint32_little_endian(
	 ( (pff_table_header_7c_t *) data )->values_array_reference,
	 table_header->values_array_reference );

	table_header->number_of_column_definitions = (int) ( (pff_table_header_7c_t *) data )->number_of_column_definitions;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table header type\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (pff_table_header_7c_t *) data )->type );

		libcnotify_printf(
		 "%s: number of column definitions\t\t\t: %d\n",
		 function,
		 table_header->number_of_column_definitions );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_header_7c_t *) data )->values_array_end_offset_32bit_values,
		 value_16bit );
		libcnotify_printf(
		 "%s: values array end offset 32-bit values\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_header_7c_t *) data )->values_array_end_offset_16bit_values,
		 value_16bit );
		libcnotify_printf(
		 "%s: values array end offset 16-bit values\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_header_7c_t *) data )->values_array_end_offset_8bit_values,
		 value_16bit );
		libcnotify_printf(
		 "%s: values array end offset 8-bit values\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_header_7c_t *) data )->values_array_end_offset_cell_existence_block,
		 value_16bit );
		libcnotify_printf(
		 "%s: values array end offset cell existence block\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: b5 table header reference\t\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 table_header->b5_table_header_reference,
		 libpff_debug_get_node_identifier_type(
		  (uint8_t) ( table_header->b5_table_header_reference & 0x0000001fUL ) ) );

		libcnotify_printf(
		 "%s: values array reference\t\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 table_header->values_array_reference,
		 libpff_debug_get_node_identifier_type(
		  (uint8_t) ( table_header->values_array_reference & 0x0000001fUL ) ) );

		libcnotify_printf(
		 "%s: unknown1:\n",
		 function );
		libcnotify_print_data(
		 ( (pff_table_header_7c_t *) data )->unknown1,
		 4,
		 0 );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	data_offset += sizeof( pff_table_header_7c_t );

	column_definitions_data_size = data_size - sizeof( pff_table_header_7c_t );

	if( ( column_definitions_data_size == 0 )
	 || ( column_definitions_data_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid column definitions data size value out of bounds.",
		 function );

		goto on_error;
	}
	/* Read the column definitions in the 7c table header
	 */
	if( (size_t) table_header->number_of_column_definitions != ( column_definitions_data_size / sizeof( pff_table_column_definition_7c_t ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: mismatch in number of the column definitions and data size.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: column definitions data:\n",
		 function );
		libcnotify_print_data(
		 &( data[ data_offset ] ),
		 column_definitions_data_size,
		 0 );
	}
#endif
	/* Copy the column definitions data otherwise the data block can cache out
	 * while processing
	 */
	table_header->column_definitions_data = (uint8_t *) memory_allocate(
	                                                     column_definitions_data_size );

	if( table_header->column_definitions_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create column definitions data.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     table_header->column_definitions_data,
	     &( data[ data_offset ] ),
	     column_definitions_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy column definitions data.",
		 function );

		goto on_error;
	}
	table_header->column_definitions_data_size = column_definitions_data_size;

	return( 1 );

on_error:
	if( table_header->column_definitions_data != NULL )
	{
		memory_free(
		 table_header->column_definitions_data );

		table_header->column_definitions_data = NULL;
	}
	table_header->column_definitions_data_size = 0;

	return( -1 );
}

/* Reads the 9c table header data
 * Returns 1 if successful or -1 on error
 */
int libpff_table_header_read_9c_data(
     libpff_table_header_t *table_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_header_read_9c_data";

	if( table_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table header.",
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
	if( data_size != 4 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: unsupported data size: %" PRIzd ".",
		 function,
		 data_size );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: 9c table header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
	/* The 9c table header contains no type indicator
	 */
	byte_stream_copy_to_uint32_little_endian(
	 data,
	 table_header->b5_table_header_reference );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: b5 table header reference\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 table_header->b5_table_header_reference,
		 libpff_debug_get_node_identifier_type(
		  (uint8_t) ( table_header->b5_table_header_reference & 0x0000001fUL ) ) );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

/* Reads the ac table header data
 * Returns 1 if successful or -1 on error
 */
int libpff_table_header_read_ac_data(
     libpff_table_header_t *table_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_header_read_ac_data";

#if defined( HAVE_DEBUG_OUTPUT )
	uint16_t value_16bit  = 0;
#endif

	if( table_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table header.",
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
	if( ( data_size < sizeof( pff_table_header_ac_t ) )
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
		 "%s: ac table header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( pff_table_header_ac_t ),
		 0 );
	}
#endif
	if( ( (pff_table_header_ac_t *) data )->type != 0xac )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported table header type: 0x%02x.",
		 function,
		 ( (pff_table_header_ac_t *) data )->type );

		return( -1 );
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (pff_table_header_ac_t *) data )->values_array_end_offset_cell_existence_block,
	 table_header->values_array_entry_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (pff_table_header_ac_t *) data )->b5_table_header_reference,
	 table_header->b5_table_header_reference );

	byte_stream_copy_to_uint32_little_endian(
	 ( (pff_table_header_ac_t *) data )->values_array_reference,
	 table_header->values_array_reference );

	byte_stream_copy_to_uint16_little_endian(
	 ( (pff_table_header_ac_t *) data )->number_of_column_definitions,
	 table_header->number_of_column_definitions );

	byte_stream_copy_to_uint32_little_endian(
	 ( (pff_table_header_ac_t *) data )->column_definitions_reference,
	 table_header->column_definitions_reference );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table header type\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (pff_table_header_ac_t *) data )->type );

		libcnotify_printf(
		 "%s: padding1\t\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (pff_table_header_ac_t *) data )->padding1 );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_header_ac_t *) data )->values_array_end_offset_32bit_values,
		 value_16bit );
		libcnotify_printf(
		 "%s: values array end offset 32-bit values\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_header_ac_t *) data )->values_array_end_offset_16bit_values,
		 value_16bit );
		libcnotify_printf(
		 "%s: values array end offset 16-bit values\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_header_ac_t *) data )->values_array_end_offset_8bit_values,
		 value_16bit );
		libcnotify_printf(
		 "%s: values array end offset 8-bit values\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (pff_table_header_ac_t *) data )->values_array_end_offset_cell_existence_block,
		 value_16bit );
		libcnotify_printf(
		 "%s: values array end offset cell existence block\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: b5 table header reference\t\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 table_header->b5_table_header_reference,
		 libpff_debug_get_node_identifier_type(
		  (uint8_t) ( table_header->b5_table_header_reference & 0x0000001fUL ) ) );

		libcnotify_printf(
		 "%s: values array reference\t\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 table_header->values_array_reference,
		 libpff_debug_get_node_identifier_type(
		  (uint8_t) ( table_header->values_array_reference & 0x0000001fUL ) ) );

		libcnotify_printf(
		 "%s: padding2:\n",
		 function );
		libcnotify_print_data(
		 ( (pff_table_header_ac_t *) data )->padding2,
		 4,
		 0 );

		libcnotify_printf(
		 "%s: number of column definitions\t\t\t: %d\n",
		 function,
		 table_header->number_of_column_definitions );

		libcnotify_printf(
		 "%s: column definitions reference\t\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 table_header->column_definitions_reference,
		 libpff_debug_get_node_identifier_type(
		  (uint8_t) ( table_header->column_definitions_reference & 0x0000001fUL ) ) );

		libcnotify_printf(
		 "%s: unknown2:\n",
		 function );
		libcnotify_print_data(
		 ( (pff_table_header_ac_t *) data )->unknown2,
		 12,
		 0 );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

/* Reads the b5 table header data
 * Returns 1 if successful or -1 on error
 */
int libpff_table_header_read_b5_data(
     libpff_table_header_t *table_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libpff_table_header_read_b5_data";

	if( table_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table header.",
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
	if( data_size != sizeof( pff_table_header_b5_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: unsupported data size: %" PRIzd ".",
		 function,
		 data_size );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: b5 table header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( pff_table_header_b5_t ),
		 0 );
	}
#endif
	if( ( (pff_table_header_b5_t *) data )->type != 0xb5 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported table header type: 0x%02x.",
		 function,
		 ( (pff_table_header_b5_t *) data )->type );

		return( -1 );
	}
	table_header->record_entry_identifier_size = ( (pff_table_header_b5_t *) data )->record_entry_identifier_size;
	table_header->record_entry_value_size      = ( (pff_table_header_b5_t *) data )->record_entry_value_size;
	table_header->record_entries_level         = ( (pff_table_header_b5_t *) data )->record_entries_level;

	byte_stream_copy_to_uint32_little_endian(
	 ( (pff_table_header_b5_t *) data )->record_entries_reference,
	 table_header->record_entries_reference );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table header type\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (pff_table_header_b5_t *) data )->type );

		libcnotify_printf(
		 "%s: record entry identifier size\t\t: %" PRIu8 "\n",
		 function,
		 table_header->record_entry_identifier_size );

		libcnotify_printf(
		 "%s: record entry value size\t\t: %" PRIu8 "\n",
		 function,
		 table_header->record_entry_value_size );

		libcnotify_printf(
		 "%s: record entries level\t\t\t: %" PRIu8 "\n",
		 function,
		 ( (pff_table_header_b5_t *) data )->record_entries_level );

		libcnotify_printf(
		 "%s: record entries reference\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 table_header->record_entries_reference,
		 libpff_debug_get_node_identifier_type(
		  (uint8_t) ( table_header->record_entries_reference & 0x0000001fUL ) ) );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

