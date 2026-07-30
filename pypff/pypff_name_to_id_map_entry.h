/*
 * Python object wrapper of libpff_name_to_id_map_entry_t
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

#if !defined( _PYPFF_NAME_TO_ID_MAP_ENTRY_H )
#define _PYPFF_NAME_TO_ID_MAP_ENTRY_H

#include <common.h>
#include <types.h>

#include "pypff_libpff.h"
#include "pypff_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pypff_name_to_id_map_entry pypff_name_to_id_map_entry_t;

struct pypff_name_to_id_map_entry
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libpff name to ID map entry
	 */
	libpff_name_to_id_map_entry_t *name_to_id_map_entry;

	/* The parent object
	 */
	PyObject *parent_object;
};

extern PyMethodDef pypff_name_to_id_map_entry_object_methods[];
extern PyTypeObject pypff_name_to_id_map_entry_type_object;

PyObject *pypff_name_to_id_map_entry_new(
           libpff_name_to_id_map_entry_t *name_to_id_map_entry,
           PyObject *parent_object );

int pypff_name_to_id_map_entry_init(
     pypff_name_to_id_map_entry_t *pypff_name_to_id_map_entry );

void pypff_name_to_id_map_entry_free(
      pypff_name_to_id_map_entry_t *pypff_name_to_id_map_entry );

PyObject *pypff_name_to_id_map_entry_get_type(
           pypff_name_to_id_map_entry_t *pypff_name_to_id_map_entry,
           PyObject *arguments );

PyObject *pypff_name_to_id_map_entry_get_number(
           pypff_name_to_id_map_entry_t *pypff_name_to_id_map_entry,
           PyObject *arguments );

PyObject *pypff_name_to_id_map_entry_get_string(
           pypff_name_to_id_map_entry_t *pypff_name_to_id_map_entry,
           PyObject *arguments );

PyObject *pypff_name_to_id_map_entry_get_guid(
           pypff_name_to_id_map_entry_t *pypff_name_to_id_map_entry,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYPFF_NAME_TO_ID_MAP_ENTRY_H ) */
