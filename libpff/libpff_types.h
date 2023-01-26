/*
 * The internal type definitions
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

#if !defined( _LIBPFF_INTERNAL_TYPES_H )
#define _LIBPFF_INTERNAL_TYPES_H

#include <common.h>
#include <types.h>

/* Define HAVE_LOCAL_LIBPFF for local use of libpff
 * The definitions in <libpff/types.h> are copied here
 * for local use of libpff
 */
#if defined( HAVE_LOCAL_LIBPFF )

/* The following type definitions hide internal data structures
 */
#if defined( HAVE_DEBUG_OUTPUT ) && !defined( WINAPI )
typedef struct libpff_file {}			libpff_file_t;
typedef struct libpff_item {}			libpff_item_t;
typedef struct libpff_multi_value {}		libpff_multi_value_t;
typedef struct libpff_name_to_id_map_entry {}	libpff_name_to_id_map_entry_t;
typedef struct libpff_record_entry {}		libpff_record_entry_t;
typedef struct libpff_record_set {}		libpff_record_set_t;

#else
typedef intptr_t libpff_file_t;
typedef intptr_t libpff_item_t;
typedef intptr_t libpff_multi_value_t;
typedef intptr_t libpff_name_to_id_map_entry_t;
typedef intptr_t libpff_record_entry_t;
typedef intptr_t libpff_record_set_t;

#endif /* defined( HAVE_DEBUG_OUTPUT ) && !defined( WINAPI ) */

#endif /* defined( HAVE_LOCAL_LIBPFF ) */

/* The largest primary (or scalar) available
 * supported by a single load and store instruction
 */
typedef unsigned long int libpff_aligned_t;

#endif /* !defined( _LIBPFF_INTERNAL_TYPES_H ) */

