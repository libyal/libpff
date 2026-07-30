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
#include "pypff_integer.h"
#include "pypff_libcerror.h"
#include "pypff_libpff.h"
#include "pypff_name_to_id_map_entry.h"
#include "pypff_python.h"
#include "pypff_unused.h"

PyMethodDef pypff_name_to_id_map_entry_object_methods[] = {

	{ "get_type",
	  (PyCFunction) pypff_name_to_id_map_entry_get_type,
	  METH_NOARGS,
	  "get_type() -> Integer\n"
	  "\n"
	  "Retrieves the name to ID map entry type." },

	{ "get_number",
	  (PyCFunction) pypff_name_to_id_map_entry_get_number,
	  METH_NOARGS,
	  "get_number() -> Integer or None\n"
	  "\n"
	  "Retrieves the numeric name value." },

	{ "get_string",
	  (PyCFunction) pypff_name_to_id_map_entry_get_string,
	  METH_NOARGS,
	  "get_string() -> Unicode string or None\n"
	  "\n"
	  "Retrieves the string name value." },

	{ "get_guid",
	  (PyCFunction) pypff_name_to_id_map_entry_get_guid,
	  METH_NOARGS,
	  "get_guid() -> Bytes\n"
	  "\n"
	  "Retrieves the property-set GUID." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pypff_name_to_id_map_entry_object_get_set_definitions[] = {

	{ "type",
	  (getter) pypff_name_to_id_map_entry_get_type,
	  (setter) 0,
	  "The name to ID map entry type.",
	  NULL },

	{ "number",
	  (getter) pypff_name_to_id_map_entry_get_number,
	  (setter) 0,
	  "The numeric name value.",
	  NULL },

	{ "string",
	  (getter) pypff_name_to_id_map_entry_get_string,
	  (setter) 0,
	  "The string name value.",
	  NULL },

	{ "guid",
	  (getter) pypff_name_to_id_map_entry_get_guid,
	  (setter) 0,
	  "The property-set GUID.",
	  NULL },

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
	static char *function                                  = "pypff_name_to_id_map_entry_new";

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

		return( NULL );
	}
	pypff_name_to_id_map_entry->name_to_id_map_entry = name_to_id_map_entry;
	pypff_name_to_id_map_entry->parent_object         = parent_object;

	if( parent_object != NULL )
	{
		Py_IncRef(
		 parent_object );
	}
	return( (PyObject *) pypff_name_to_id_map_entry );
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
	pypff_name_to_id_map_entry->name_to_id_map_entry = NULL;
	pypff_name_to_id_map_entry->parent_object         = NULL;

	return( 0 );
}

/* Frees a name to ID map entry object
 */
void pypff_name_to_id_map_entry_free(
      pypff_name_to_id_map_entry_t *pypff_name_to_id_map_entry )
{
	struct _typeobject *ob_type = NULL;

	if( pypff_name_to_id_map_entry == NULL )
	{
		return;
	}
	ob_type = Py_TYPE(
	           pypff_name_to_id_map_entry );

	if( pypff_name_to_id_map_entry->parent_object != NULL )
	{
		Py_DecRef(
		 pypff_name_to_id_map_entry->parent_object );
	}
	if( ( ob_type != NULL )
	 && ( ob_type->tp_free != NULL ) )
	{
		ob_type->tp_free(
		 (PyObject *) pypff_name_to_id_map_entry );
	}
}

/* Retrieves the entry type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_name_to_id_map_entry_get_type(
           pypff_name_to_id_map_entry_t *pypff_name_to_id_map_entry,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pypff_name_to_id_map_entry_get_type";
	uint8_t entry_type       = 0;
	int result               = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( ( pypff_name_to_id_map_entry == NULL )
	 || ( pypff_name_to_id_map_entry->name_to_id_map_entry == NULL ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid name to ID map entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_name_to_id_map_entry_get_type(
	          pypff_name_to_id_map_entry->name_to_id_map_entry,
	          &entry_type,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve entry type.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) entry_type );
#else
	integer_object = PyInt_FromLong(
	                  (long) entry_type );
#endif
	return( integer_object );
}

/* Retrieves the numeric name value
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_name_to_id_map_entry_get_number(
           pypff_name_to_id_map_entry_t *pypff_name_to_id_map_entry,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pypff_name_to_id_map_entry_get_number";
	uint32_t number          = 0;
	uint8_t entry_type       = 0;
	int result               = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( ( pypff_name_to_id_map_entry == NULL )
	 || ( pypff_name_to_id_map_entry->name_to_id_map_entry == NULL ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid name to ID map entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_name_to_id_map_entry_get_type(
	          pypff_name_to_id_map_entry->name_to_id_map_entry,
	          &entry_type,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve entry type.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( entry_type != LIBPFF_NAME_TO_ID_MAP_ENTRY_TYPE_NUMERIC )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_name_to_id_map_entry_get_number(
	          pypff_name_to_id_map_entry->name_to_id_map_entry,
	          &number,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve numeric name value.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pypff_integer_unsigned_new_from_64bit(
	                  (uint64_t) number );

	return( integer_object );
}

/* Retrieves the string name value
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_name_to_id_map_entry_get_string(
           pypff_name_to_id_map_entry_t *pypff_name_to_id_map_entry,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *string_object  = NULL;
	libcerror_error_t *error = NULL;
	char *utf8_string        = NULL;
	static char *function    = "pypff_name_to_id_map_entry_get_string";
	size_t utf8_string_size  = 0;
	uint8_t entry_type       = 0;
	int result               = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( ( pypff_name_to_id_map_entry == NULL )
	 || ( pypff_name_to_id_map_entry->name_to_id_map_entry == NULL ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid name to ID map entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_name_to_id_map_entry_get_type(
	          pypff_name_to_id_map_entry->name_to_id_map_entry,
	          &entry_type,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve entry type.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( entry_type != LIBPFF_NAME_TO_ID_MAP_ENTRY_TYPE_STRING )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_name_to_id_map_entry_get_utf8_string_size(
	          pypff_name_to_id_map_entry->name_to_id_map_entry,
	          &utf8_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to determine size of string name value.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( utf8_string_size == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	utf8_string = (char *) PyMem_Malloc(
	                         sizeof( char ) * utf8_string_size );

	if( utf8_string == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create UTF-8 string.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_name_to_id_map_entry_get_utf8_string(
	          pypff_name_to_id_map_entry->name_to_id_map_entry,
	          (uint8_t *) utf8_string,
	          utf8_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve string name value.",
		 function );

		libcerror_error_free(
		 &error );

		PyMem_Free(
		 utf8_string );

		return( NULL );
	}
	string_object = PyUnicode_DecodeUTF8(
	                 utf8_string,
	                 (Py_ssize_t) utf8_string_size - 1,
	                 NULL );

	PyMem_Free(
	 utf8_string );

	return( string_object );
}

/* Retrieves the property-set GUID
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_name_to_id_map_entry_get_guid(
           pypff_name_to_id_map_entry_t *pypff_name_to_id_map_entry,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *bytes_object   = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pypff_name_to_id_map_entry_get_guid";
	uint8_t guid[ 16 ];
	int result               = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( ( pypff_name_to_id_map_entry == NULL )
	 || ( pypff_name_to_id_map_entry->name_to_id_map_entry == NULL ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid name to ID map entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_name_to_id_map_entry_get_guid(
	          pypff_name_to_id_map_entry->name_to_id_map_entry,
	          guid,
	          16,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve property-set GUID.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	bytes_object = PyBytes_FromStringAndSize(
	                (char *) guid,
	                16 );

	return( bytes_object );
}
