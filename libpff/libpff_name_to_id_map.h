/*
 * Name to ID map functions
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

#if !defined( _LIBPFF_NAME_TO_ID_MAP_H )
#define _LIBPFF_NAME_TO_ID_MAP_H

#include <common.h>
#include <types.h>

#include "libpff_descriptors_index.h"
#include "libpff_extern.h"
#include "libpff_io_handle.h"
#include "libpff_libbfio.h"
#include "libpff_libcdata.h"
#include "libpff_libcerror.h"
#include "libpff_libcnotify.h"
#include "libpff_offsets_index.h"
#include "libpff_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_internal_name_to_id_map_entry libpff_internal_name_to_id_map_entry_t;

struct libpff_internal_name_to_id_map_entry
{
	/* The identifier
	 */
	uint32_t identifier;

	/* The entry type
	 */
	uint8_t type;

	union
	{
		/* The numeric value
		 */
		uint32_t numeric_value;

		/* The string value
		 */
		uint8_t *string_value;
	};

	/* The value size
	 */
	size_t value_size;

	/* Value to indicate if the string value is an ASCII string
	 */
	uint8_t is_ascii_string;

	/* The GUID containing the class identifier
	 */
	uint8_t guid[ 16 ];

	/* Various flags
	 */
	uint8_t flags;

#if defined( HAVE_DEBUG_OUTPUT )
	/* The debug string value
	 */
	system_character_t *debug_string;

	/* The debug string size
	 */
	size_t debug_string_size;

#endif
};

int libpff_name_to_id_map_entry_initialize(
     libpff_name_to_id_map_entry_t **name_to_id_map_entry,
     libcerror_error_t **error );

int libpff_name_to_id_map_entry_free(
     libpff_name_to_id_map_entry_t **name_to_id_map_entry,
     libcerror_error_t **error );

int libpff_name_to_id_map_read(
     libcdata_list_t *name_to_id_map_list,
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_descriptors_index_t *descriptors_index,
     libpff_offsets_index_t *offsets_index,
     libcerror_error_t **error );

int libpff_name_to_id_map_entry_read(
     libpff_name_to_id_map_entry_t *name_to_id_map_entry,
     uint8_t *name_to_id_map_entry_data,
     size_t name_to_id_map_entry_data_size,
     uint8_t *name_to_id_map_class_identifiers_data,
     size_t name_to_id_map_class_identifiers_data_size,
     uint8_t *name_to_id_map_strings_data,
     size_t name_to_id_map_strings_data_size,
     libcerror_error_t **error );

int libpff_name_to_id_map_entry_get_entry_by_identifier(
     libcdata_list_t *name_to_id_map_list,
     uint32_t identifier,
     libpff_internal_name_to_id_map_entry_t **name_to_id_map_entry,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_name_to_id_map_entry_get_type(
     libpff_name_to_id_map_entry_t *name_to_id_map_entry,
     uint8_t *entry_type,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_name_to_id_map_entry_get_number(
     libpff_name_to_id_map_entry_t *name_to_id_map_entry,
     uint32_t *number,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_name_to_id_map_entry_get_utf8_string_size(
     libpff_name_to_id_map_entry_t *name_to_id_map_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_name_to_id_map_entry_get_utf8_string(
     libpff_name_to_id_map_entry_t *name_to_id_map_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_name_to_id_map_entry_get_utf16_string_size(
     libpff_name_to_id_map_entry_t *name_to_id_map_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_name_to_id_map_entry_get_utf16_string(
     libpff_name_to_id_map_entry_t *name_to_id_map_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_name_to_id_map_entry_get_guid(
     libpff_name_to_id_map_entry_t *name_to_id_map_entry,
     uint8_t *guid,
     size_t size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_NAME_TO_ID_MAP_H ) */

