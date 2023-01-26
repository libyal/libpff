/*
 * Python object definition of the sequence and iterator object of record sets
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

#include <common.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pypff_libcerror.h"
#include "pypff_libpff.h"
#include "pypff_python.h"
#include "pypff_record_set.h"
#include "pypff_record_sets.h"

PySequenceMethods pypff_record_sets_sequence_methods = {
	/* sq_length */
	(lenfunc) pypff_record_sets_len,
	/* sq_concat */
	0,
	/* sq_repeat */
	0,
	/* sq_item */
	(ssizeargfunc) pypff_record_sets_getitem,
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

PyTypeObject pypff_record_sets_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pypff._record_sets",
	/* tp_basicsize */
	sizeof( pypff_record_sets_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pypff_record_sets_free,
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
	&pypff_record_sets_sequence_methods,
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
	"pypff internal sequence and iterator object of record sets",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	(getiterfunc) pypff_record_sets_iter,
	/* tp_iternext */
	(iternextfunc) pypff_record_sets_iternext,
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
	(initproc) pypff_record_sets_init,
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

/* Creates a new record sets object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_record_sets_new(
           PyObject *parent_object,
           PyObject* (*get_item_by_index)(
                        PyObject *parent_object,
                        int index ),
           int number_of_items )
{
	pypff_record_sets_t *record_sets_object = NULL;
	static char *function                   = "pypff_record_sets_new";

	if( parent_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid parent object.",
		 function );

		return( NULL );
	}
	if( get_item_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid get item by index function.",
		 function );

		return( NULL );
	}
	/* Make sure the record sets values are initialized
	 */
	record_sets_object = PyObject_New(
	                      struct pypff_record_sets,
	                      &pypff_record_sets_type_object );

	if( record_sets_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create record sets object.",
		 function );

		goto on_error;
	}
	if( pypff_record_sets_init(
	     record_sets_object ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize record sets object.",
		 function );

		goto on_error;
	}
	record_sets_object->parent_object     = parent_object;
	record_sets_object->get_item_by_index = get_item_by_index;
	record_sets_object->number_of_items   = number_of_items;

	Py_IncRef(
	 (PyObject *) record_sets_object->parent_object );

	return( (PyObject *) record_sets_object );

on_error:
	if( record_sets_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) record_sets_object );
	}
	return( NULL );
}

/* Initializes a record sets object
 * Returns 0 if successful or -1 on error
 */
int pypff_record_sets_init(
     pypff_record_sets_t *record_sets_object )
{
	static char *function = "pypff_record_sets_init";

	if( record_sets_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record sets object.",
		 function );

		return( -1 );
	}
	/* Make sure the record sets values are initialized
	 */
	record_sets_object->parent_object     = NULL;
	record_sets_object->get_item_by_index = NULL;
	record_sets_object->current_index     = 0;
	record_sets_object->number_of_items   = 0;

	return( 0 );
}

/* Frees a record sets object
 */
void pypff_record_sets_free(
      pypff_record_sets_t *record_sets_object )
{
	struct _typeobject *ob_type = NULL;
	static char *function       = "pypff_record_sets_free";

	if( record_sets_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record sets object.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           record_sets_object );

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
	if( record_sets_object->parent_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) record_sets_object->parent_object );
	}
	ob_type->tp_free(
	 (PyObject*) record_sets_object );
}

/* The record sets len() function
 */
Py_ssize_t pypff_record_sets_len(
            pypff_record_sets_t *record_sets_object )
{
	static char *function = "pypff_record_sets_len";

	if( record_sets_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record sets object.",
		 function );

		return( -1 );
	}
	return( (Py_ssize_t) record_sets_object->number_of_items );
}

/* The record sets getitem() function
 */
PyObject *pypff_record_sets_getitem(
           pypff_record_sets_t *record_sets_object,
           Py_ssize_t item_index )
{
	PyObject *record_set_object = NULL;
	static char *function       = "pypff_record_sets_getitem";

	if( record_sets_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record sets object.",
		 function );

		return( NULL );
	}
	if( record_sets_object->get_item_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record sets object - missing get item by index function.",
		 function );

		return( NULL );
	}
	if( record_sets_object->number_of_items < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record sets object - invalid number of items.",
		 function );

		return( NULL );
	}
	if( ( item_index < 0 )
	 || ( item_index >= (Py_ssize_t) record_sets_object->number_of_items ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid invalid item index value out of bounds.",
		 function );

		return( NULL );
	}
	record_set_object = record_sets_object->get_item_by_index(
	                     record_sets_object->parent_object,
	                     (int) item_index );

	return( record_set_object );
}

/* The record sets iter() function
 */
PyObject *pypff_record_sets_iter(
           pypff_record_sets_t *record_sets_object )
{
	static char *function = "pypff_record_sets_iter";

	if( record_sets_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record sets object.",
		 function );

		return( NULL );
	}
	Py_IncRef(
	 (PyObject *) record_sets_object );

	return( (PyObject *) record_sets_object );
}

/* The record sets iternext() function
 */
PyObject *pypff_record_sets_iternext(
           pypff_record_sets_t *record_sets_object )
{
	PyObject *record_set_object = NULL;
	static char *function       = "pypff_record_sets_iternext";

	if( record_sets_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record sets object.",
		 function );

		return( NULL );
	}
	if( record_sets_object->get_item_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record sets object - missing get item by index function.",
		 function );

		return( NULL );
	}
	if( record_sets_object->current_index < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record sets object - invalid current index.",
		 function );

		return( NULL );
	}
	if( record_sets_object->number_of_items < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record sets object - invalid number of items.",
		 function );

		return( NULL );
	}
	if( record_sets_object->current_index >= record_sets_object->number_of_items )
	{
		PyErr_SetNone(
		 PyExc_StopIteration );

		return( NULL );
	}
	record_set_object = record_sets_object->get_item_by_index(
	                     record_sets_object->parent_object,
	                     record_sets_object->current_index );

	if( record_set_object != NULL )
	{
		record_sets_object->current_index++;
	}
	return( record_set_object );
}

