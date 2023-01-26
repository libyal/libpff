/*
 * Python object definition of the sequence and iterator object of items
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

#if !defined( _PYPFF_ITEMS_H )
#define _PYPFF_ITEMS_H

#include <common.h>
#include <types.h>

#include "pypff_libpff.h"
#include "pypff_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pypff_items pypff_items_t;

struct pypff_items
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The parent object
	 */
	PyObject *parent_object;

	/* The get item by index callback function
	 */
	PyObject* (*get_item_by_index)(
	             PyObject *parent_object,
	             int index );

	/* The current index
	 */
	int current_index;

	/* The number of items
	 */
	int number_of_items;
};

extern PyTypeObject pypff_items_type_object;

PyObject *pypff_items_new(
           PyObject *parent_object,
           PyObject* (*get_item_by_index)(
                        PyObject *parent_object,
                        int index ),
           int number_of_items );

int pypff_items_init(
     pypff_items_t *sequence_object );

void pypff_items_free(
      pypff_items_t *sequence_object );

Py_ssize_t pypff_items_len(
            pypff_items_t *sequence_object );

PyObject *pypff_items_getitem(
           pypff_items_t *sequence_object,
           Py_ssize_t item_index );

PyObject *pypff_items_iter(
           pypff_items_t *sequence_object );

PyObject *pypff_items_iternext(
           pypff_items_t *sequence_object );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYPFF_ITEMS_H ) */

