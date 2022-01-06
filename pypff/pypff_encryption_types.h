/*
 * Python object definition of the libpff encryption types
 *
 * Copyright (C) 2008-2022, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYPFF_ENCRYPTION_TYPES_H )
#define _PYPFF_ENCRYPTION_TYPES_H

#include <common.h>
#include <types.h>

#include "pypff_libpff.h"
#include "pypff_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pypff_encryption_types pypff_encryption_types_t;

struct pypff_encryption_types
{
	/* Python object initialization
	 */
	PyObject_HEAD
};

extern PyTypeObject pypff_encryption_types_type_object;

int pypff_encryption_types_init_type(
     PyTypeObject *type_object );

PyObject *pypff_encryption_types_new(
           void );

int pypff_encryption_types_init(
     pypff_encryption_types_t *definitions_object );

void pypff_encryption_types_free(
      pypff_encryption_types_t *definitions_object );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYPFF_ENCRYPTION_TYPES_H ) */

