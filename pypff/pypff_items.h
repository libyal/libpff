/*
 * Python object definition of the items sequence and iterator
 *
 * Copyright (C) 2008-2015, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYPFF_ITEMS_H )
#define _PYPFF_ITEMS_H

#include <common.h>
#include <types.h>

#include "pypff_item.h"
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

	/* The item object
	 */
	pypff_item_t *item_object;

	/* The get item by index callback function
	 */
	PyObject* (*get_item_by_index)(
	             pypff_item_t *item_object,
	             int item_index );

	/* The (current) item index
	 */
	int item_index;

	/* The number of items
	 */
	int number_of_items;
};

extern PyTypeObject pypff_items_type_object;

PyObject *pypff_items_new(
           pypff_item_t *item_object,
           PyObject* (*get_item_by_index)(
                        pypff_item_t *item_object,
                        int item_index ),
           int number_of_items );

int pypff_items_init(
     pypff_items_t *pypff_items );

void pypff_items_free(
      pypff_items_t *pypff_items );

Py_ssize_t pypff_items_len(
            pypff_items_t *pypff_items );

PyObject *pypff_items_getitem(
           pypff_items_t *pypff_items,
           Py_ssize_t item_index );

PyObject *pypff_items_iter(
           pypff_items_t *pypff_items );

PyObject *pypff_items_iternext(
           pypff_items_t *pypff_items );

#if defined( __cplusplus )
}
#endif

#endif

