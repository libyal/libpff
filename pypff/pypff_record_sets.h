/*
 * Python object definition of the record sets sequence and iterator
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

#if !defined( _PYPFF_RECORD_SETS_H )
#define _PYPFF_RECORD_SETS_H

#include <common.h>
#include <types.h>

#include "pypff_item.h"
#include "pypff_libpff.h"
#include "pypff_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pypff_record_sets pypff_record_sets_t;

struct pypff_record_sets
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The item object
	 */
	pypff_item_t *item_object;

	/* The get record set by index callback function
	 */
	PyObject* (*get_record_set_by_index)(
	             pypff_item_t *item_object,
	             int record_set_index );

	/* The (current) record set index
	 */
	int record_set_index;

	/* The number of record sets
	 */
	int number_of_record_sets;
};

extern PyTypeObject pypff_record_sets_type_object;

PyObject *pypff_record_sets_new(
           pypff_item_t *item_object,
           PyObject* (*get_record_set_by_index)(
                        pypff_item_t *item_object,
                        int record_set_index ),
           int number_of_record_sets );

int pypff_record_sets_init(
     pypff_record_sets_t *pypff_record_sets );

void pypff_record_sets_free(
      pypff_record_sets_t *pypff_record_sets );

Py_ssize_t pypff_record_sets_len(
            pypff_record_sets_t *pypff_record_sets );

PyObject *pypff_record_sets_getitem(
           pypff_record_sets_t *pypff_record_sets,
           Py_ssize_t record_set_index );

PyObject *pypff_record_sets_iter(
           pypff_record_sets_t *pypff_record_sets );

PyObject *pypff_record_sets_iternext(
           pypff_record_sets_t *pypff_record_sets );

#if defined( __cplusplus )
}
#endif

#endif

