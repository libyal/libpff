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

#include <common.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pypff_error.h"
#include "pypff_name_to_id_map_entry.h"
#include "pypff_unused.h"

PyMethodDef pypff_name_to_id_map_entry_object_methods[] = {

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pypff_name_to_id_map_entry_object_get_set_definitions[] = {

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pypff_name_to_id_map_entry_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pypff.name_to_id_map_entry",
	/* tp_basicsize */
	sizeof( pypff_name_to_id_map_entry_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pypff_name_to_id_map_entry_free,
	/* tp_print */
	0,
	/* tp_getattr */
	0,
	/* tp_setattr */
	0,
	/* tp_compare */
	0,
	/* tp_repr */
	0,
	/* tp_as_number */
	0,
	/* tp_as_sequence */
	0,
	/* tp_as_mapping */
	0,
	/* tp_hash */
	0,
	/* tp_call */
	0,
	/* tp_str */
	0,
	/* tp_getattro */
	0,
	/* tp_setattro */
	0,
	/* tp_as_buffer */
	0,
	/* tp_flags */
	Py_TPFLAGS_DEFAULT,
	/* tp_doc */
	"pypff name to ID map entry object (wraps libpff_name_to_id_map_entry_t)",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	0,
	/* tp_iternext */
	0,
	/* tp_methods */
	pypff_name_to_id_map_entry_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pypff_name_to_id_map_entry_object_get_set_definitions,
	/* tp_base */
	0,
	/* tp_dict */
	0,
	/* tp_descr_get */
	0,
	/* tp_descr_set */
	0,
	/* tp_dictoffset */
	0,
	/* tp_init */
	(initproc) pypff_name_to_id_map_entry_init,
	/* tp_alloc */
	0,
	/* tp_new */
	0,
	/* tp_free */
	0,
	/* tp_is_gc */
	0,
	/* tp_bases */
	NULL,
	/* tp_mro */
	NULL,
	/* tp_cache */
	NULL,
	/* tp_subclasses */
	NULL,
	/* tp_weaklist */
	NULL,
	/* tp_del */
	0
};

/* Creates a new name to ID map entry object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_name_to_id_map_entry_new(
           libpff_name_to_id_map_entry_t *name_to_id_map_entry,
           PyObject *parent_object )
{
	pypff_name_to_id_map_entry_t *pypff_name_to_id_map_entry = NULL;
	static char *function                                    = "pypff_name_to_id_map_entry_new";

	if( name_to_id_map_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid name to ID map entry.",
		 function );

		return( NULL );
	}
	pypff_name_to_id_map_entry = PyObject_New(
	                              struct pypff_name_to_id_map_entry,
	                              &pypff_name_to_id_map_entry_type_object );

	if( pypff_name_to_id_map_entry == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize name to ID map entry.",
		 function );

		goto on_error;
	}
	if( pypff_name_to_id_map_entry_init(
	     pypff_name_to_id_map_entry ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize name to ID map entry.",
		 function );

		goto on_error;
	}
	pypff_name_to_id_map_entry->name_to_id_map_entry = name_to_id_map_entry;
	pypff_name_to_id_map_entry->parent_object        = parent_object;

	Py_IncRef(
	 (PyObject *) pypff_name_to_id_map_entry->parent_object );

	return( (PyObject *) pypff_name_to_id_map_entry );

on_error:
	if( pypff_name_to_id_map_entry != NULL )
	{
		Py_DecRef(
		 (PyObject *) pypff_name_to_id_map_entry );
	}
	return( NULL );
}

/* Initializes a name to ID map entry object
 * Returns 0 if successful or -1 on error
 */
int pypff_name_to_id_map_entry_init(
     pypff_name_to_id_map_entry_t *pypff_name_to_id_map_entry )
{
	static char *function = "pypff_name_to_id_map_entry_init";

	if( pypff_name_to_id_map_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid name to ID map entry.",
		 function );

		return( -1 );
	}
	/* Make sure libpff name to ID map entry is set to NULL
	 */
	pypff_name_to_id_map_entry->name_to_id_map_entry = NULL;

	return( 0 );
}

/* Frees a name to ID map entry object
 */
void pypff_name_to_id_map_entry_free(
      pypff_name_to_id_map_entry_t *pypff_name_to_id_map_entry )
{
	struct _typeobject *ob_type = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pypff_name_to_id_map_entry_free";
	int result                  = 0;

	if( pypff_name_to_id_map_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid name to ID map entry.",
		 function );

		return;
	}
	if( pypff_name_to_id_map_entry->name_to_id_map_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid name to ID map entry - missing libpff name to ID map entry.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pypff_name_to_id_map_entry );

	if( ob_type == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: missing ob_type.",
		 function );

		return;
	}
	if( ob_type->tp_free == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid ob_type - missing tp_free.",
		 function );

		return;
	}
	if( pypff_name_to_id_map_entry->parent_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) pypff_name_to_id_map_entry->parent_object );
	}
	ob_type->tp_free(
	 (PyObject*) pypff_name_to_id_map_entry );
}

