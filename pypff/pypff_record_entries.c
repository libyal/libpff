/*
 * Python object definition of the record entriess sequence and iterator
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

#include <common.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pypff_libcerror.h"
#include "pypff_libpff.h"
#include "pypff_python.h"
#include "pypff_record_entry.h"
#include "pypff_record_entries.h"
#include "pypff_record_set.h"

PySequenceMethods pypff_record_entries_sequence_methods = {
	/* sq_length */
	(lenfunc) pypff_record_entries_len,
	/* sq_concat */
	0,
	/* sq_repeat */
	0,
	/* sq_item */
	(ssizeargfunc) pypff_record_entries_getitem,
	/* sq_slice */
	0,
	/* sq_ass_item */
	0,
	/* sq_ass_slice */
	0,
	/* sq_contains */
	0,
	/* sq_inplace_concat */
	0,
	/* sq_inplace_repeat */
	0
};

PyTypeObject pypff_record_entries_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pypff._record_entries",
	/* tp_basicsize */
	sizeof( pypff_record_entries_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pypff_record_entries_free,
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
	&pypff_record_entries_sequence_methods,
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
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_ITER,
	/* tp_doc */
	"internal pypff record entries sequence and iterator object",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	(getiterfunc) pypff_record_entries_iter,
	/* tp_iternext */
	(iternextfunc) pypff_record_entries_iternext,
	/* tp_methods */
	0,
	/* tp_members */
	0,
	/* tp_getset */
	0,
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
	(initproc) pypff_record_entries_init,
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

/* Creates a new record entries object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_record_entries_new(
           pypff_record_set_t *record_set_object,
           PyObject* (*get_record_entry_by_index)(
                        pypff_record_set_t *record_set_object,
                        int record_entry_index ),
           int number_of_record_entries )
{
	pypff_record_entries_t *pypff_record_entries = NULL;
	static char *function                        = "pypff_record_entries_new";

	if( record_set_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record set object.",
		 function );

		return( NULL );
	}
	if( get_record_entry_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid get record entry by index function.",
		 function );

		return( NULL );
	}
	/* Make sure the record entries values are initialized
	 */
	pypff_record_entries = PyObject_New(
	                        struct pypff_record_entries,
	                        &pypff_record_entries_type_object );

	if( pypff_record_entries == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize record entries.",
		 function );

		goto on_error;
	}
	if( pypff_record_entries_init(
	     pypff_record_entries ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize record entries.",
		 function );

		goto on_error;
	}
	pypff_record_entries->record_set_object         = record_set_object;
	pypff_record_entries->get_record_entry_by_index = get_record_entry_by_index;
	pypff_record_entries->number_of_record_entries  = number_of_record_entries;

	Py_IncRef(
	 (PyObject *) pypff_record_entries->record_set_object );

	return( (PyObject *) pypff_record_entries );

on_error:
	if( pypff_record_entries != NULL )
	{
		Py_DecRef(
		 (PyObject *) pypff_record_entries );
	}
	return( NULL );
}

/* Intializes a record entries object
 * Returns 0 if successful or -1 on error
 */
int pypff_record_entries_init(
     pypff_record_entries_t *pypff_record_entries )
{
	static char *function = "pypff_record_entries_init";

	if( pypff_record_entries == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record entries.",
		 function );

		return( -1 );
	}
	/* Make sure the record entries values are initialized
	 */
	pypff_record_entries->record_set_object         = NULL;
	pypff_record_entries->get_record_entry_by_index = NULL;
	pypff_record_entries->record_entry_index        = 0;
	pypff_record_entries->number_of_record_entries  = 0;

	return( 0 );
}

/* Frees a record entries object
 */
void pypff_record_entries_free(
      pypff_record_entries_t *pypff_record_entries )
{
	struct _typeobject *ob_type = NULL;
	static char *function       = "pypff_record_entries_free";

	if( pypff_record_entries == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record entries.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pypff_record_entries );

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
	if( pypff_record_entries->record_set_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) pypff_record_entries->record_set_object );
	}
	ob_type->tp_free(
	 (PyObject*) pypff_record_entries );
}

/* The record entries len() function
 */
Py_ssize_t pypff_record_entries_len(
            pypff_record_entries_t *pypff_record_entries )
{
	static char *function = "pypff_record_entries_len";

	if( pypff_record_entries == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record entries.",
		 function );

		return( -1 );
	}
	return( (Py_ssize_t) pypff_record_entries->number_of_record_entries );
}

/* The record entries getitem() function
 */
PyObject *pypff_record_entries_getitem(
           pypff_record_entries_t *pypff_record_entries,
           Py_ssize_t record_entry_index )
{
	PyObject *record_entry_object = NULL;
	static char *function         = "pypff_record_entries_getitem";

	if( pypff_record_entries == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record entries.",
		 function );

		return( NULL );
	}
	if( pypff_record_entries->get_record_entry_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record entries - missing get record entry by index function.",
		 function );

		return( NULL );
	}
	if( pypff_record_entries->number_of_record_entries < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record entries - invalid number of record entries.",
		 function );

		return( NULL );
	}
	if( ( record_entry_index < 0 )
	 || ( record_entry_index >= (Py_ssize_t) pypff_record_entries->number_of_record_entries ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid invalid record entry index value out of bounds.",
		 function );

		return( NULL );
	}
	record_entry_object = pypff_record_entries->get_record_entry_by_index(
	                       pypff_record_entries->record_set_object,
	                       (int) record_entry_index );

	return( record_entry_object );
}

/* The record entries iter() function
 */
PyObject *pypff_record_entries_iter(
           pypff_record_entries_t *pypff_record_entries )
{
	static char *function = "pypff_record_entries_iter";

	if( pypff_record_entries == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record entries.",
		 function );

		return( NULL );
	}
	Py_IncRef(
	 (PyObject *) pypff_record_entries );

	return( (PyObject *) pypff_record_entries );
}

/* The record entries iternext() function
 */
PyObject *pypff_record_entries_iternext(
           pypff_record_entries_t *pypff_record_entries )
{
	PyObject *record_entry_object = NULL;
	static char *function         = "pypff_record_entries_iternext";

	if( pypff_record_entries == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record entries.",
		 function );

		return( NULL );
	}
	if( pypff_record_entries->get_record_entry_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record entries - missing get record entry by index function.",
		 function );

		return( NULL );
	}
	if( pypff_record_entries->record_entry_index < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record entries - invalid record entry index.",
		 function );

		return( NULL );
	}
	if( pypff_record_entries->number_of_record_entries < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record entries - invalid number of record entries.",
		 function );

		return( NULL );
	}
	if( pypff_record_entries->record_entry_index >= pypff_record_entries->number_of_record_entries )
	{
		PyErr_SetNone(
		 PyExc_StopIteration );

		return( NULL );
	}
	record_entry_object = pypff_record_entries->get_record_entry_by_index(
	                       pypff_record_entries->record_set_object,
	                       pypff_record_entries->record_entry_index );

	if( record_entry_object != NULL )
	{
		pypff_record_entries->record_entry_index++;
	}
	return( record_entry_object );
}

