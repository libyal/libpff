/*
 * Python object wrapper of libpff_record_entry_t
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

#include "pypff_datetime.h"
#include "pypff_error.h"
#include "pypff_integer.h"
#include "pypff_libcerror.h"
#include "pypff_libpff.h"
#include "pypff_python.h"
#include "pypff_record_entry.h"
#include "pypff_unused.h"

PyMethodDef pypff_record_entry_object_methods[] = {

	{ "get_entry_type",
	  (PyCFunction) pypff_record_entry_get_entry_type,
	  METH_NOARGS,
	  "get_entry_type() -> Integer or None\n"
	  "\n"
	  "Retrieves the entry type." },

	{ "get_value_type",
	  (PyCFunction) pypff_record_entry_get_value_type,
	  METH_NOARGS,
	  "get_value_type() -> Integer or None\n"
	  "\n"
	  "Retrieves the value type." },

	{ "get_data",
	  (PyCFunction) pypff_record_entry_get_data,
	  METH_NOARGS,
	  "get_data() -> Binary string or None\n"
	  "\n"
	  "Retrieves the data." },

	{ "get_data_as_boolean",
	  (PyCFunction) pypff_record_entry_get_data_as_boolean,
	  METH_NOARGS,
	  "get_data_as_boolean() -> Integer or None\n"
	  "\n"
	  "Retrieves the data as a boolean." },

	{ "get_data_as_integer",
	  (PyCFunction) pypff_record_entry_get_data_as_integer,
	  METH_NOARGS,
	  "get_data_as_integer() -> Integer or None\n"
	  "\n"
	  "Retrieves the data as an integer." },

	{ "get_data_as_datetime",
	  (PyCFunction) pypff_record_entry_get_data_as_datetime,
	  METH_NOARGS,
	  "get_data_as_datetime() -> Datetime or None\n"
	  "\n"
	  "Retrieves the data as a datetime object." },

	{ "get_data_as_size",
	  (PyCFunction) pypff_record_entry_get_data_as_size,
	  METH_NOARGS,
	  "get_data_as_size() -> Integer or None\n"
	  "\n"
	  "Retrieves the data as size." },

	{ "get_data_as_floating_point",
	  (PyCFunction) pypff_record_entry_get_data_as_floating_point,
	  METH_NOARGS,
	  "get_data_as_floating_point() -> Float or None\n"
	  "\n"
	  "Retrieves the data as a floating point." },

	{ "get_data_as_string",
	  (PyCFunction) pypff_record_entry_get_data_as_string,
	  METH_NOARGS,
	  "get_data_as_string() -> Unicode string or None\n"
	  "\n"
	  "Retrieves the data as a string." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pypff_record_entry_object_get_set_definitions[] = {

	{ "entry_type",
	  (getter) pypff_record_entry_get_entry_type,
	  (setter) 0,
	  "The entry type.",
	  NULL },

	{ "value_type",
	  (getter) pypff_record_entry_get_value_type,
	  (setter) 0,
	  "The value type.",
	  NULL },

	{ "data",
	  (getter) pypff_record_entry_get_data,
	  (setter) 0,
	  "The data.",
	  NULL },

	{ "data_as_boolean",
	  (getter) pypff_record_entry_get_data_as_boolean,
	  (setter) 0,
	  "The data as a boolean.",
	  NULL },

	{ "data_as_integer",
	  (getter) pypff_record_entry_get_data_as_integer,
	  (setter) 0,
	  "The data as an integer.",
	  NULL },

	{ "data_as_datetime",
	  (getter) pypff_record_entry_get_data_as_datetime,
	  (setter) 0,
	  "The data as a datetime object.",
	  NULL },

	{ "data_as_size",
	  (getter) pypff_record_entry_get_data_as_size,
	  (setter) 0,
	  "The data as size.",
	  NULL },

	{ "data_as_floating_point",
	  (getter) pypff_record_entry_get_data_as_floating_point,
	  (setter) 0,
	  "The data as a floating point.",
	  NULL },

	{ "data_as_string",
	  (getter) pypff_record_entry_get_data_as_string,
	  (setter) 0,
	  "The data as a string.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pypff_record_entry_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pypff.record_entry",
	/* tp_basicsize */
	sizeof( pypff_record_entry_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pypff_record_entry_free,
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
	"pypff record entry object (wraps libpff_record_entry_t)",
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
	pypff_record_entry_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pypff_record_entry_object_get_set_definitions,
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
	(initproc) pypff_record_entry_init,
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

/* Creates a new record entry object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_record_entry_new(
           PyTypeObject *type_object,
           libpff_record_entry_t *record_entry,
           PyObject *parent_object )
{
	pypff_record_entry_t *pypff_record_entry = NULL;
	static char *function                    = "pypff_record_entry_new";

	if( record_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record entry.",
		 function );

		return( NULL );
	}
	pypff_record_entry = PyObject_New(
	                      struct pypff_record_entry,
	                      type_object );

	if( pypff_record_entry == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize record entry.",
		 function );

		goto on_error;
	}
	if( pypff_record_entry_init(
	     pypff_record_entry ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize record entry.",
		 function );

		goto on_error;
	}
	pypff_record_entry->record_entry  = record_entry;
	pypff_record_entry->parent_object = parent_object;

	Py_IncRef(
	 (PyObject *) pypff_record_entry->parent_object );

	return( (PyObject *) pypff_record_entry );

on_error:
	if( pypff_record_entry != NULL )
	{
		Py_DecRef(
		 (PyObject *) pypff_record_entry );
	}
	return( NULL );
}

/* Initializes a record entry object
 * Returns 0 if successful or -1 on error
 */
int pypff_record_entry_init(
     pypff_record_entry_t *pypff_record_entry )
{
	static char *function = "pypff_record_entry_init";

	if( pypff_record_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	/* Make sure libpff record entry is set to NULL
	 */
	pypff_record_entry->record_entry = NULL;

	return( 0 );
}

/* Frees a record entry object
 */
void pypff_record_entry_free(
      pypff_record_entry_t *pypff_record_entry )
{
	struct _typeobject *ob_type = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pypff_record_entry_free";
	int result                  = 0;

	if( pypff_record_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record entry.",
		 function );

		return;
	}
	if( pypff_record_entry->record_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record entry - missing libpff record entry.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pypff_record_entry );

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

	result = libpff_record_entry_free(
	          &( pypff_record_entry->record_entry ),
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to free libpff record entry.",
		 function );

		libcerror_error_free(
		 &error );
	}
	if( pypff_record_entry->parent_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) pypff_record_entry->parent_object );
	}
	ob_type->tp_free(
	 (PyObject*) pypff_record_entry );
}

/* Retrieves the entry type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_record_entry_get_entry_type(
           pypff_record_entry_t *pypff_record_entry,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pypff_record_entry_get_entry_type";
	uint32_t value_32bit     = 0;
	int result               = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_record_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_record_entry_get_entry_type(
	          pypff_record_entry->record_entry,
	          &value_32bit,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
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
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	integer_object = PyLong_FromUnsignedLong(
	                  (unsigned long) value_32bit );

	return( integer_object );
}

/* Retrieves the value type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_record_entry_get_value_type(
           pypff_record_entry_t *pypff_record_entry,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pypff_record_entry_get_value_type";
	uint32_t value_32bit     = 0;
	int result               = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_record_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_record_entry_get_value_type(
	          pypff_record_entry->record_entry,
	          &value_32bit,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value type.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	integer_object = PyLong_FromUnsignedLong(
	                  (unsigned long) value_32bit );

	return( integer_object );
}

/* Retrieves the data
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_record_entry_get_data(
           pypff_record_entry_t *pypff_record_entry,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *bytes_object   = NULL;
	libcerror_error_t *error = NULL;
	char *data               = NULL;
	static char *function    = "pypff_record_entry_get_data";
	size_t data_size         = 0;
	int result               = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_record_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_record_entry_get_data_size(
	          pypff_record_entry->record_entry,
	          &data_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve data size.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( ( result == 0 )
	      || ( data_size == 0 ) )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	data = (char *) PyMem_Malloc(
	                 sizeof( char ) * data_size );

	if( data == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create data.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_record_entry_get_data(
	          pypff_record_entry->record_entry,
	          (uint8_t *) data,
	          data_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve data.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* This is a binary string so include the full size
	 */
#if PY_MAJOR_VERSION >= 3
	bytes_object = PyBytes_FromStringAndSize(
	                data,
	                (Py_ssize_t) data_size );
#else
	bytes_object = PyString_FromStringAndSize(
	                data,
	                (Py_ssize_t) data_size );
#endif
	if( bytes_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to convert data into Bytes object.",
		 function );

		goto on_error;
	}
	PyMem_Free(
	 data );

	return( bytes_object );

on_error:
	if( data != NULL )
	{
		PyMem_Free(
		 data );
	}
	return( NULL );
}

/* Retrieves the data as a boolean value
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_record_entry_get_data_as_boolean(
           pypff_record_entry_t *pypff_record_entry,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pypff_record_entry_get_data_as_boolean";
	uint8_t value_boolean    = 0;
	int result               = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_record_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_record_entry_get_data_as_boolean(
	          pypff_record_entry->record_entry,
	          &value_boolean,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve boolean value.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( value_boolean != 0x00 )
	{
		Py_IncRef(
		 Py_True );

		return( Py_True );
	}
	Py_IncRef(
	 Py_False );

	return( Py_False );
}

/* Retrieves the data as an integer value
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_record_entry_get_data_as_integer(
           pypff_record_entry_t *pypff_record_entry,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pypff_record_entry_get_data_as_integer";
	uint64_t value_64bit     = 0;
	uint32_t value_32bit     = 0;
	uint32_t value_type      = 0;
	uint16_t value_16bit     = 0;
	int result               = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_record_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_record_entry_get_value_type(
	          pypff_record_entry->record_entry,
	          &value_type,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value type.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	switch( value_type )
	{
		case LIBPFF_VALUE_TYPE_INTEGER_16BIT_SIGNED:
			Py_BEGIN_ALLOW_THREADS

			result = libpff_record_entry_get_data_as_16bit_integer(
			          pypff_record_entry->record_entry,
			          &value_16bit,
			          &error );

			Py_END_ALLOW_THREADS

#if PY_MAJOR_VERSION >= 3
			integer_object = PyLong_FromLong(
			                  (long) ( (int16_t) value_16bit ) );
#else
			integer_object = PyInt_FromLong(
			                  (long) ( (int16_t) value_16bit ) );
#endif
			break;

		case LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED:
			Py_BEGIN_ALLOW_THREADS

			result = libpff_record_entry_get_data_as_32bit_integer(
			          pypff_record_entry->record_entry,
			          &value_32bit,
			          &error );

			Py_END_ALLOW_THREADS

#if PY_MAJOR_VERSION >= 3
			integer_object = PyLong_FromLong(
			                  (long) ( (int32_t) value_32bit ) );
#else
			integer_object = PyInt_FromLong(
			                  (long) ( (int32_t) value_32bit ) );
#endif
			break;

		case LIBPFF_VALUE_TYPE_INTEGER_64BIT_SIGNED:
			Py_BEGIN_ALLOW_THREADS

			result = libpff_record_entry_get_data_as_64bit_integer(
			          pypff_record_entry->record_entry,
			          &value_64bit,
			          &error );

			Py_END_ALLOW_THREADS

			integer_object = pypff_integer_signed_new_from_64bit(
			                  (int64_t) value_64bit );

			break;

		case LIBPFF_VALUE_TYPE_FILETIME:
			Py_BEGIN_ALLOW_THREADS

			result = libpff_record_entry_get_data_as_filetime(
			          pypff_record_entry->record_entry,
			          &value_64bit,
			          &error );

			Py_END_ALLOW_THREADS

			integer_object = pypff_integer_unsigned_new_from_64bit(
			                  value_64bit );
			break;

		case LIBPFF_VALUE_TYPE_FLOATINGTIME:
			Py_BEGIN_ALLOW_THREADS

			result = libpff_record_entry_get_data_as_floatingtime(
			          pypff_record_entry->record_entry,
			          &value_64bit,
			          &error );

			Py_END_ALLOW_THREADS

			integer_object = pypff_integer_unsigned_new_from_64bit(
			                  value_64bit );
			break;

		default:
			PyErr_Format(
			 PyExc_IOError,
			 "%s: value is not an integer type.",
			 function );

			return( NULL );
	}
	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve integer value.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	return( integer_object );
}

/* Retrieves the data as an datetime value
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_record_entry_get_data_as_datetime(
           pypff_record_entry_t *pypff_record_entry,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *datetime_object = NULL;
	libcerror_error_t *error  = NULL;
	static char *function     = "pypff_record_entry_get_data_as_datetime";
	uint64_t value_64bit      = 0;
	uint32_t value_type       = 0;
	int result                = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_record_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_record_entry_get_value_type(
	          pypff_record_entry->record_entry,
	          &value_type,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value type.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	switch( value_type )
	{
		case LIBPFF_VALUE_TYPE_FILETIME:
			Py_BEGIN_ALLOW_THREADS

			result = libpff_record_entry_get_data_as_filetime(
			          pypff_record_entry->record_entry,
			          &value_64bit,
			          &error );

			Py_END_ALLOW_THREADS

			datetime_object = pypff_datetime_new_from_filetime(
			                   value_64bit );
			break;

		case LIBPFF_VALUE_TYPE_FLOATINGTIME:
			Py_BEGIN_ALLOW_THREADS

			result = libpff_record_entry_get_data_as_floatingtime(
			          pypff_record_entry->record_entry,
			          &value_64bit,
			          &error );

			Py_END_ALLOW_THREADS

			datetime_object = pypff_datetime_new_from_floatingtime(
			                   value_64bit );
			break;

		default:
			PyErr_Format(
			 PyExc_IOError,
			 "%s: value is not an datetime type.",
			 function );

			return( NULL );
	}
	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve datetime value.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	return( datetime_object );
}

/* Retrieves the data as size
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_record_entry_get_data_as_size(
           pypff_record_entry_t *pypff_record_entry,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pypff_record_entry_get_data_as_size";
	size64_t data_as_size    = 0;
	int result               = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_record_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_record_entry_get_data_as_size(
	          pypff_record_entry->record_entry,
	          &data_as_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: failed to retrieve data as size.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pypff_integer_unsigned_new_from_64bit(
	                  (uint64_t) data_as_size );

	return( integer_object );
}

/* Retrieves the data as an floating-point value
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_record_entry_get_data_as_floating_point(
           pypff_record_entry_t *pypff_record_entry,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *float_object      = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pypff_record_entry_get_data_as_floating_point";
	double floating_point_value = 0;
	int result                  = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_record_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_record_entry_get_data_as_floating_point(
	          pypff_record_entry->record_entry,
	          &floating_point_value,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve data as floating-point value.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	float_object = PyFloat_FromDouble(
	                floating_point_value );

	return( float_object );
}

/* Retrieves the data as a string
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_record_entry_get_data_as_string(
           pypff_record_entry_t *pypff_record_entry,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *string_object  = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pypff_value_get_data_as_string";
	char *utf8_string        = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_record_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_record_entry_get_data_as_utf8_string_size(
	          pypff_record_entry->record_entry,
	          &utf8_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to determine size of data as UTF-8 string.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( ( result == 0 )
	      || ( utf8_string_size == 0 ) )
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

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_record_entry_get_data_as_utf8_string(
	          pypff_record_entry->record_entry,
	          (uint8_t *) utf8_string,
	          utf8_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve data as UTF-8 string.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Pass the string length to PyUnicode_DecodeUTF8 otherwise it makes
	 * the end of string character is part of the string
	 */
	string_object = PyUnicode_DecodeUTF8(
	                 utf8_string,
	                 (Py_ssize_t) utf8_string_size - 1,
	                 NULL );

	if( string_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to convert UTF-8 string into Unicode object.",
		 function );

		goto on_error;
	}
	PyMem_Free(
	 utf8_string );

	return( string_object );

on_error:
	if( utf8_string != NULL )
	{
		PyMem_Free(
		 utf8_string );
	}
	return( NULL );
}

