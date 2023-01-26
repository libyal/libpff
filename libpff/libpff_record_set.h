/*
 * Record set functions
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

#if !defined( _LIBPFF_RECORD_SET_H )
#define _LIBPFF_RECORD_SET_H

#include <common.h>
#include <types.h>

#include "libpff_extern.h"
#include "libpff_libcdata.h"
#include "libpff_libcerror.h"
#include "libpff_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_internal_record_set libpff_internal_record_set_t;

struct libpff_internal_record_set
{
	/* The (record) entries array
	 */
	libcdata_array_t *entries_array;

	/* The codepage of the extended ASCII strings
	 */
	int ascii_codepage;
};

int libpff_record_set_initialize(
     libpff_record_set_t **record_set,
     int number_of_entries,
     int ascii_codepage,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_set_free(
     libpff_record_set_t **record_set,
     libcerror_error_t **error );

int libpff_internal_record_set_free(
     libpff_internal_record_set_t **internal_record_set,
     libcerror_error_t **error );

int libpff_record_set_clone(
     libpff_record_set_t **destination_record_set,
     libpff_record_set_t *source_record_set,
     libcerror_error_t **error );

int libpff_record_set_resize(
     libpff_record_set_t *record_set,
     int number_of_entries,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_set_get_number_of_entries(
     libpff_record_set_t *record_set,
     int *number_of_entries,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_set_get_entry_by_index(
     libpff_record_set_t *record_set,
     int entry_index,
     libpff_record_entry_t **record_entry,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_set_get_entry_by_type(
     libpff_record_set_t *record_set,
     uint32_t entry_type,
     uint32_t value_type,
     libpff_record_entry_t **record_entry,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_set_get_entry_by_utf8_name(
     libpff_record_set_t *record_set,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint32_t value_type,
     libpff_record_entry_t **record_entry,
     uint8_t flags,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_record_set_get_entry_by_utf16_name(
     libpff_record_set_t *record_set,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint32_t value_type,
     libpff_record_entry_t **record_entry,
     uint8_t flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_RECORD_SET_H ) */

