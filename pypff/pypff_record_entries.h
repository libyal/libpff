/*
 * Python object definition of the record entries sequence and iterator
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

#if !defined( _PYPFF_RECORD_ENTRIES_H )
#define _PYPFF_RECORD_ENTRIES_H

#include <common.h>
#include <types.h>

#include "pypff_libpff.h"
#include "pypff_record_set.h"
#include "pypff_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pypff_record_entries pypff_record_entries_t;

struct pypff_record_entries
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The record set object
	 */
	pypff_record_set_t *record_set_object;

	/* The get record entry by index callback function
	 */
	PyObject* (*get_record_entry_by_index)(
	             pypff_record_set_t *record_set_object,
	             int record_entry_index );

	/* The (current) record entry index
	 */
	int record_entry_index;

	/* The number of record entries
	 */
	int number_of_record_entries;
};

extern PyTypeObject pypff_record_entries_type_object;

PyObject *pypff_record_entries_new(
           pypff_record_set_t *record_set_object,
           PyObject* (*get_record_entry_by_index)(
                        pypff_record_set_t *record_set_object,
                        int record_entry_index ),
           int number_of_record_entries );

int pypff_record_entries_init(
     pypff_record_entries_t *pypff_record_entries );

void pypff_record_entries_free(
      pypff_record_entries_t *pypff_record_entries );

Py_ssize_t pypff_record_entries_len(
            pypff_record_entries_t *pypff_record_entries );

PyObject *pypff_record_entries_getitem(
           pypff_record_entries_t *pypff_record_entries,
           Py_ssize_t record_entry_index );

PyObject *pypff_record_entries_iter(
           pypff_record_entries_t *pypff_record_entries );

PyObject *pypff_record_entries_iternext(
           pypff_record_entries_t *pypff_record_entries );

#if defined( __cplusplus )
}
#endif

#endif

