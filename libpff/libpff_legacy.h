/*
 * Legacy functions
 *
 * Copyright (c) 2008-2014, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _LIBPFF_LEGACY_H )
#define _LIBPFF_LEGACY_H

#include <common.h>
#include <types.h>

#include "libpff_extern.h"
#include "libpff_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

LIBPFF_EXTERN \
int libpff_item_get_number_of_sets(
     libpff_item_t *item,
     uint32_t *number_of_sets,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

