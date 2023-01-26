/*
 * Debug functions
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

#if !defined( _LIBPFF_DEBUG_H )
#define _LIBPFF_DEBUG_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_libcdata.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum LIBPFF_DEBUG_ITEM_TYPES
{
	LIBPFF_DEBUG_ITEM_TYPE_DEFAULT,
	LIBPFF_DEBUG_ITEM_TYPE_NAME_TO_ID_MAP
};

#if defined( HAVE_DEBUG_OUTPUT )

const char *libpff_debug_get_name_to_id_map_property_type_identifier(
             uint32_t entry_type,
             uint32_t value_type );

const char *libpff_debug_get_name_to_id_map_property_type_description(
             uint32_t entry_type,
             uint32_t value_type );

const char *libpff_debug_get_encryption_type(
             uint8_t encryption_type );

const char *libpff_debug_get_node_identifier_type(
             uint8_t node_identifier_type );

int libpff_debug_print_record_set(
     libpff_record_set_t *record_set,
     libcdata_list_t *name_to_id_map_list,
     int debug_item_type,
     int ascii_codepage,
     libcerror_error_t **error );

int libpff_debug_print_record_entry(
     libpff_record_entry_t *record_entry,
     libcdata_list_t *name_to_id_map_list,
     int debug_item_type,
     int ascii_codepage,
     libcerror_error_t **error );

int libpff_debug_property_type_value_print(
     libcdata_list_t *name_to_id_map_list,
     uint32_t entry_type,
     uint32_t value_type,
     uint8_t *value_data,
     size_t value_data_size,
     int debug_item_type,
     int ascii_codepage,
     libcerror_error_t **error );

int libpff_debug_mapi_value_print(
     libcdata_list_t *name_to_id_map_list,
     uint32_t entry_type,
     uint32_t value_type,
     uint8_t *value_data,
     size_t value_data_size,
     int ascii_codepage,
     libcerror_error_t **error );

int libpff_debug_name_to_id_map_value_print(
     uint32_t entry_type,
     uint32_t value_type,
     uint8_t *value_data,
     size_t value_data_size,
     libcerror_error_t **error );

int libpff_debug_print_read_offsets(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

#endif /* defined( HAVE_DEBUG_OUTPUT ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_DEBUG_H ) */

