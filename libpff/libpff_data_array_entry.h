/*
 * Data array entry functions
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

#if !defined( _LIBPFF_DATA_ARRAY_ENTRY_H )
#define _LIBPFF_DATA_ARRAY_ENTRY_H

#include <common.h>
#include <types.h>

#include "libpff_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_data_array_entry libpff_data_array_entry_t;

struct libpff_data_array_entry
{
	/* Value to indicate if the data array data identifier
	 */
	uint64_t data_identifier;

	/* The (data block) flags
	 */
	uint8_t flags;
};

int libpff_data_array_entry_initialize(
     libpff_data_array_entry_t **data_array_entry,
     libcerror_error_t **error );

int libpff_data_array_entry_free(
     libpff_data_array_entry_t **data_array_entry,
     libcerror_error_t **error );

int libpff_data_array_entry_clone(
     libpff_data_array_entry_t **destination_data_array_entry,
     libpff_data_array_entry_t *source_data_array_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_DATA_ARRAY_ENTRY_H ) */

