/*
 * Python object wrapper of libpff_record_set_t
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

#include "pypff_error.h"
#include "pypff_libcerror.h"
#include "pypff_libpff.h"
#include "pypff_python.h"
#include "pypff_record_entries.h"
#include "pypff_record_entry.h"
#include "pypff_record_set.h"
#include "pypff_unused.h"

PyMethodDef pypff_record_set_object_methods[] = {

	{ "get_number_of_entries",
	  (PyCFunction) pypff_record_set_get_number_of_entries,
	  METH_NOARGS,
	  "get_number_of_entries() -> Integer or None\n"
	  "\n"
	  "Retrieves the number of entries." },

	{ "get_entry",
	  (PyCFunction) pypff_record_set_get_entry,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_entry(entry_index) -> Object or None\n"
	  "\n"
	  "Retrieves the entry specified by the index." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pypff_record_set_object_get_set_definitions[] = {

	{ "number_of_entries",
	  (getter) pypff_record_set_get_number_of_entries,
	  (setter) 0,
	  "The number of entries.",
	  NULL },

	{ "entries",
	  (getter) pypff_record_set_get_entries,
	  (setter) 0,
	  "The entries.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pypff_record_set_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pypff.record_set",
	/* tp_basicsize */
	sizeof( pypff_record_set_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pypff_record_set_free,
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
	"pypff record set object (wraps libpff_record_set_t)",
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
	pypff_record_set_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pypff_record_set_object_get_set_definitions,
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
	(initproc) pypff_record_set_init,
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

/* Creates a new record set object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_record_set_new(
           PyTypeObject *type_object,
           libpff_record_set_t *record_set,
           PyObject *parent_object )
{
	pypff_record_set_t *pypff_record_set = NULL;
	static char *function                = "pypff_record_set_new";

	if( record_set == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record set.",
		 function );

		return( NULL );
	}
	pypff_record_set = PyObject_New(
	                    struct pypff_record_set,
	                    type_object );

	if( pypff_record_set == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize record set.",
		 function );

		goto on_error;
	}
	if( pypff_record_set_init(
	     pypff_record_set ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize record set.",
		 function );

		goto on_error;
	}
	pypff_record_set->record_set    = record_set;
	pypff_record_set->parent_object = parent_object;

	Py_IncRef(
	 (PyObject *) pypff_record_set->parent_object );

	return( (PyObject *) pypff_record_set );

on_error:
	if( pypff_record_set != NULL )
	{
		Py_DecRef(
		 (PyObject *) pypff_record_set );
	}
	return( NULL );
}

/* Initializes a record set object
 * Returns 0 if successful or -1 on error
 */
int pypff_record_set_init(
     pypff_record_set_t *pypff_record_set )
{
	static char *function = "pypff_record_set_init";

	if( pypff_record_set == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record set.",
		 function );

		return( -1 );
	}
	/* Make sure libpff record set is set to NULL
	 */
	pypff_record_set->record_set = NULL;

	return( 0 );
}

/* Frees a record set object
 */
void pypff_record_set_free(
      pypff_record_set_t *pypff_record_set )
{
	struct _typeobject *ob_type = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pypff_record_set_free";
	int result                  = 0;

	if( pypff_record_set == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record set.",
		 function );

		return;
	}
	if( pypff_record_set->record_set == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record set - missing libpff record set.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pypff_record_set );

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
	Py_BEGIN_ALLOW_THREADS

	result = libpff_record_set_free(
	          &( pypff_record_set->record_set ),
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to free libpff record set.",
		 function );

		libcerror_error_free(
		 &error );
	}
	if( pypff_record_set->parent_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) pypff_record_set->parent_object );
	}
	ob_type->tp_free(
	 (PyObject*) pypff_record_set );
}

/* Retrieves the number of entries
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_record_set_get_number_of_entries(
           pypff_record_set_t *pypff_record_set,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pypff_record_set_get_number_of_entries";
	int number_of_entries    = 0;
	int result               = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_record_set == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record set.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_record_set_get_number_of_entries(
	          pypff_record_set->record_set,
	          &number_of_entries,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of entries.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) number_of_entries );
#else
	integer_object = PyInt_FromLong(
	                  (long) number_of_entries );
#endif
	return( integer_object );
}

/* Retrieves the record entry type object
 * Returns a Python type object if successful or NULL on error
 */
PyTypeObject *pypff_record_set_get_record_entry_type_object(
               libpff_record_entry_t *record_entry PYPFF_ATTRIBUTE_UNUSED )
{
	PYPFF_UNREFERENCED_PARAMETER( record_entry )

	return( &pypff_record_entry_type_object );
}

/* Retrieves a specific entry by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_record_set_get_entry_by_index(
           PyObject *pypff_record_set,
           int entry_index )
{
	PyObject *entry_object       = NULL;
	PyTypeObject *type_object    = NULL;
	libcerror_error_t *error     = NULL;
	libpff_record_entry_t *entry = NULL;
	static char *function        = "pypff_record_set_get_entry_by_index";
	int result                   = 0;

	if( pypff_record_set == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record set.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_record_set_get_entry_by_index(
	          ( (pypff_record_set_t *) pypff_record_set )->record_set,
	          entry_index,
	          &entry,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve entry: %d.",
		 function,
		 entry_index );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	type_object = pypff_record_set_get_record_entry_type_object(
	               entry );

	if( type_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to retrieve record entry type object.",
		 function );

		goto on_error;
	}
	entry_object = pypff_record_entry_new(
	                type_object,
	                entry,
	                (PyObject *) pypff_record_set );

	if( entry_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create record entry object.",
		 function );

		goto on_error;
	}
	return( entry_object );

on_error:
	if( entry != NULL )
	{
		libpff_record_entry_free(
		 &entry,
		 NULL );
	}
	return( NULL );
}

/* Retrieves a specific entry
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_record_set_get_entry(
           pypff_record_set_t *pypff_record_set,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *entry_object      = NULL;
	static char *keyword_list[] = { "entry_index", NULL };
	int entry_index             = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &entry_index ) == 0 )
	{
		return( NULL );
	}
	entry_object = pypff_record_set_get_entry_by_index(
	                (PyObject *) pypff_record_set,
	                entry_index );

	return( entry_object );
}

/* Retrieves a sequence and iterator object for the entries
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_record_set_get_entries(
           pypff_record_set_t *pypff_record_set,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *sequence_object = NULL;
	libcerror_error_t *error  = NULL;
	static char *function     = "pypff_record_set_get_entries";
	int number_of_entries     = 0;
	int result                = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_record_set == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record set.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_record_set_get_number_of_entries(
	          pypff_record_set->record_set,
	          &number_of_entries,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of entries.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	sequence_object = pypff_record_entries_new(
	                   (PyObject *) pypff_record_set,
	                   &pypff_record_set_get_entry_by_index,
	                   number_of_entries );

	if( sequence_object == NULL )
	{
		pypff_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to create sequence object.",
		 function );

		return( NULL );
	}
	return( sequence_object );
}

