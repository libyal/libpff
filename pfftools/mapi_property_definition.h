/*
 * MAPI property definition
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

#if !defined( _MAPI_PROPERTY_DEFINITION_H )
#define _MAPI_PROPERTY_DEFINITION_H

#include <common.h>
#include <types.h>

#include "item_file.h"
#include "pfftools_libpff.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct mapi_property_definitions mapi_property_definitions_t;

struct mapi_property_definitions
{
	/* The description
	 */
	const system_character_t *description;

	/* The entry type
	 */
	uint32_t entry_type;

	/* The value type
	 */
	uint32_t value_type;

	/* The format flags
	 */
	uint32_t format_flags;

	/* The write to item file function
	 */
	int (*write_to_item_file_function)(
	       item_file_t *item_file,
	       libpff_record_entry_t *record_entry,
	       libcerror_error_t **error );
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _MAPI_PROPERTY_DEFINITION_H ) */

