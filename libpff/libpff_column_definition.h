/*
 * Column definition functions
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

#if !defined( _LIBPFF_COLUMN_DEFINITION_H )
#define _LIBPFF_COLUMN_DEFINITION_H

#include <common.h>
#include <types.h>

#include "libpff_libcerror.h"
#include "libpff_name_to_id_map.h"
#include "libpff_table.h"
#include "libpff_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_column_definition libpff_column_definition_t;

struct libpff_column_definition
{
	/* The entry type
	 */
	uint16_t entry_type;

	/* The value type
	 */
	uint16_t value_type;

	/* The values array entry offset
	 */
	uint16_t values_array_offset;

	/* The value array entry size
	 */
	uint16_t values_array_size;

	/* The record entry values table
	 */
	libpff_table_t *record_entry_values_table;

	/* The name to id map entry
	 */
	libpff_internal_name_to_id_map_entry_t *name_to_id_map_entry;
};

int libpff_column_definition_initialize(
     libpff_column_definition_t **column_definition,
     libcerror_error_t **error );

int libpff_column_definition_free(
     libpff_column_definition_t **column_definition,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_COLUMN_DEFINITION_H ) */

