/*
 * Reference descriptor functions
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

#if !defined( _LIBPFF_REFERENCE_DESCRIPTOR_H )
#define _LIBPFF_REFERENCE_DESCRIPTOR_H

#include <common.h>
#include <types.h>

#include "libpff_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

struct libpff_reference_descriptor
{
	/* The (reference) value
	 */
	uint32_t value;
};

typedef struct libpff_reference_descriptor libpff_reference_descriptor_t;

int libpff_reference_descriptor_initialize(
     libpff_reference_descriptor_t **reference_descriptor,
     uint32_t value,
     libcerror_error_t **error );

int libpff_reference_descriptor_free(
     libpff_reference_descriptor_t **reference_descriptor,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_REFERENCE_DESCRIPTOR_H ) */

