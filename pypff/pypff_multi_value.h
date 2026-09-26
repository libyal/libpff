/*
 * Python object wrapper of libpff_multi_value_t
 *
 * Copyright (C) 2008-2026, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYPFF_MULTI_VALUE_H )
#define _PYPFF_MULTI_VALUE_H

#include <common.h>
#include <types.h>

#include "pypff_libpff.h"
#include "pypff_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pypff_multi_value pypff_multi_value_t;

struct pypff_multi_value
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libpff multi value
	 */
	libpff_multi_value_t *multi_value;

	/* The parent object
	 */
	PyObject *parent_object;
};

extern PyMethodDef pypff_multi_value_object_methods[];
extern PyTypeObject pypff_multi_value_type_object;

PyObject *pypff_multi_value_new(
           libpff_multi_value_t *multi_value,
           PyObject *parent_object );

int pypff_multi_value_init(
     pypff_multi_value_t *pypff_multi_value );

void pypff_multi_value_free(
      pypff_multi_value_t *pypff_multi_value );

PyObject *pypff_multi_value_get_number_of_values(
           pypff_multi_value_t *pypff_multi_value,
           PyObject *arguments );

PyObject *pypff_multi_value_get_value_type(
           pypff_multi_value_t *pypff_multi_value,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pypff_multi_value_get_value_data(
           pypff_multi_value_t *pypff_multi_value,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pypff_multi_value_get_value_as_datetime(
           pypff_multi_value_t *pypff_multi_value,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pypff_multi_value_get_value_as_integer(
           pypff_multi_value_t *pypff_multi_value,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pypff_multi_value_get_value_as_string(
           pypff_multi_value_t *pypff_multi_value,
           PyObject *arguments,
           PyObject *keywords );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYPFF_MULTI_VALUE_H ) */

