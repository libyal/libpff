/*
 * Python object wrapper of libpff_item_t
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

#if !defined( _PYPFF_ITEM_H )
#define _PYPFF_ITEM_H

#include <common.h>
#include <types.h>

#include "pypff_libpff.h"
#include "pypff_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pypff_item pypff_item_t;

struct pypff_item
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libpff item
	 */
	libpff_item_t *item;

	/* The libpff record set
	 */
	libpff_record_set_t *record_set;

	/* The parent object
	 */
	PyObject *parent_object;
};

extern PyMethodDef pypff_item_object_methods[];
extern PyTypeObject pypff_item_type_object;

PyObject *pypff_item_new(
           PyTypeObject *type_object,
           libpff_item_t *item,
           PyObject *parent_object );

int pypff_item_init(
     pypff_item_t *pypff_item );

void pypff_item_free(
      pypff_item_t *pypff_item );

PyObject *pypff_item_get_identifier(
           pypff_item_t *pypff_item,
           PyObject *arguments );

PyObject *pypff_item_get_number_of_record_sets(
           pypff_item_t *pypff_item,
           PyObject *arguments );

PyObject *pypff_item_get_record_set_by_index(
           PyObject *pypff_item,
           int record_set_index );

PyObject *pypff_item_get_record_set(
           pypff_item_t *pypff_item,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pypff_item_get_record_sets(
           pypff_item_t *pypff_item,
           PyObject *arguments );

PyObject *pypff_item_get_number_of_entries(
           pypff_item_t *pypff_item,
           PyObject *arguments );

PyObject *pypff_item_get_number_of_sub_items(
           pypff_item_t *pypff_item,
           PyObject *arguments );

PyObject *pypff_item_get_sub_item_by_index(
           PyObject *pypff_item,
           int sub_item_index );

PyObject *pypff_item_get_sub_item(
           pypff_item_t *pypff_item,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pypff_item_get_sub_items(
           pypff_item_t *pypff_item,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYPFF_ITEM_H ) */

