/*
 * Python object definition of the libpff record entry
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

#include "pypff_error.h"
#include "pypff_integer.h"
#include "pypff_libcerror.h"
#include "pypff_libcstring.h"
#include "pypff_libpff.h"
#include "pypff_python.h"
#include "pypff_unused.h"
#include "pypff_record_entry.h"
#include "pypff_record_set.h"

PyMethodDef pypff_record_entry_object_methods[] = {

	/* Functions to access the record entry values */

	{ "get_entry_type",
	  (PyCFunction) pypff_record_entry_get_entry_type,
	  METH_NOARGS,
	  "get_entry_type() -> Integer\n"
	  "\n"
	  "Retrieves the entry type." },

	{ "get_value_type",
	  (PyCFunction) pypff_record_entry_get_value_type,
	  METH_NOARGS,
	  "get_value_type() -> Integer\n"
	  "\n"
	  "Retrieves the value type." },

	{ "get_value_data",
	  (PyCFunction) pypff_record_entry_get_value_data,
	  METH_NOARGS,
	  "get_value_data -> String or None\n"
	  "\n"
	  "Retrieves the value data as a binary string." },

	/* TODO get_value_data_as_boolean */

	{ "get_value_data_as_integer",
	  (PyCFunction) pypff_record_entry_get_value_data_as_integer,
	  METH_NOARGS,
	  "get_value_data_as_integer -> Integer\n"
	  "\n"
	  "Retrieves the value data as an integer ." },

	{ "get_value_data_as_string",
	  (PyCFunction) pypff_record_entry_get_value_data_as_string,
	  METH_NOARGS,
	  "get_value_data_as_string -> Unicode string or None\n"
	  "\n"
	  "Retrieves the value data as a string." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pypff_record_entry_object_get_entry_definitions[] = {

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

	{ "value_data",
	  (getter) pypff_record_entry_get_value_data,
	  (setter) 0,
	  "The value data.",
	  NULL },

	{ "value_data_as_integer",
	  (getter) pypff_record_entry_get_value_data_as_integer,
	  (setter) 0,
	  "The value data represented as an integer.",
	  NULL },

	{ "value_data_as_string",
	  (getter) pypff_record_entry_get_value_data_as_string,
	  (setter) 0,
	  "The value data represented as a string.",
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
	pypff_record_entry_object_get_entry_definitions,
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
           pypff_record_set_t *record_set_object )
{
	pypff_record_entry_t *pypff_record_entry = NULL;
	static char *function                    = "pypff_record_entry_new";

	if( record_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
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
	pypff_record_entry->record_entry      = record_entry;
	pypff_record_entry->record_set_object = record_set_object;

	Py_IncRef(
	 (PyObject *) pypff_record_entry->record_set_object );

	return( (PyObject *) pypff_record_entry );

on_error:
	if( pypff_record_entry != NULL )
	{
		Py_DecRef(
		 (PyObject *) pypff_record_entry );
	}
	return( NULL );
}

/* Intializes a record entry object
 * Returns 0 if successful or -1 on error
 */
int pypff_record_entry_init(
     pypff_record_entry_t *pypff_record_entry )
{
	static char *function = "pypff_record_entry_init";

	if( pypff_record_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
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
	libcerror_error_t *error    = NULL;
	struct _typeobject *ob_type = NULL;
	static char *function       = "pypff_record_entry_free";

	if( pypff_record_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid record entry.",
		 function );

		return;
	}
	if( pypff_record_entry->record_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
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
	if( libpff_record_entry_free(
	     &( pypff_record_entry->record_entry ),
	     &error ) != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to free libpff record entry.",
		 function );

		libcerror_error_free(
		 &error );
	}
	if( pypff_record_entry->record_set_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) pypff_record_entry->record_set_object );
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
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pypff_record_entry_get_entry_type";
	uint32_t entry_type      = 0;
	int result               = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_record_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid record entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_record_entry_get_entry_type(
	          pypff_record_entry->record_entry,
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
	integer_object = pypff_integer_unsigned_new_from_64bit(
	                  (uint64_t) entry_type );

	return( integer_object );
}

/* Retrieves the value type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_record_entry_get_value_type(
           pypff_record_entry_t *pypff_record_entry,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pypff_record_entry_get_value_type";
	uint32_t value_type      = 0;
	int result               = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_record_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
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
	integer_object = pypff_integer_unsigned_new_from_64bit(
	                  (uint64_t) value_type );

	return( integer_object );
}

/* Retrieves the value data
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_record_entry_get_value_data(
           pypff_record_entry_t *pypff_record_entry,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *string_object  = NULL;
	uint8_t *value_data      = NULL;
	static char *function    = "pypff_record_entry_get_value_data";
	size_t value_data_size   = 0;
	int result               = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_record_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid value.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_record_entry_get_value_data_size(
	          pypff_record_entry->record_entry,
	          &value_data_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value data size.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( ( result == 0 )
	      || ( value_data_size == 0 ) )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	value_data = (uint8_t *) PyMem_Malloc(
	                          sizeof( uint8_t ) * value_data_size );

	if( value_data == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to create value data.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_record_entry_copy_value_data(
		  pypff_record_entry->record_entry,
		  value_data,
		  value_data_size,
		  &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value data.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
			 (char *) value_data,
			 (Py_ssize_t) value_data_size );
#else
	string_object = PyString_FromStringAndSize(
			 (char *) value_data,
			 (Py_ssize_t) value_data_size );
#endif
	PyMem_Free(
	 value_data );

	return( string_object );

on_error:
	if( value_data != NULL )
	{
		PyMem_Free(
		 value_data );
	}
	return( NULL );
}

/* Retrieves the value data represented as an integer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_record_entry_get_value_data_as_integer(
           pypff_record_entry_t *pypff_record_entry,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pypff_record_entry_get_value_data_as_integer";
	uint64_t value_64bit     = 0;
	int64_t integer_value    = 0;
	uint32_t value_32bit     = 0;
	uint32_t value_type      = 0;
	uint16_t value_16bit     = 0;
	int result               = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_record_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid value.",
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

			result = libpff_record_entry_get_value_16bit(
				  pypff_record_entry->record_entry,
				  &value_16bit,
				  &error );

			Py_END_ALLOW_THREADS

			/* Interpret the 16-bit value as signed
			 */
			integer_value = (int16_t) value_16bit;

			break;

		case LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED:
			Py_BEGIN_ALLOW_THREADS

			result = libpff_record_entry_get_value_32bit(
				  pypff_record_entry->record_entry,
				  &value_32bit,
				  &error );

			Py_END_ALLOW_THREADS

			/* Interpret the 32-bit value as signed
			 */
			integer_value = (int32_t) value_32bit;

			break;

		case LIBPFF_VALUE_TYPE_INTEGER_64BIT_SIGNED:
			Py_BEGIN_ALLOW_THREADS

			result = libpff_record_entry_get_value_64bit(
				  pypff_record_entry->record_entry,
				  &value_64bit,
				  &error );

			Py_END_ALLOW_THREADS

			/* Interpret the 64-bit value as signed
			 */
			integer_value = (int64_t) value_64bit;

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
	integer_object = pypff_integer_signed_new_from_64bit(
	                  integer_value );

	return( integer_object );
}

/* Retrieves the value data represented as a string
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_record_entry_get_value_data_as_string(
           pypff_record_entry_t *pypff_record_entry,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *string_object  = NULL;
	const char *errors       = NULL;
	uint8_t *value_string    = NULL;
	static char *function    = "pypff_record_entry_get_value_data_as_string";
	size_t value_string_size = 0;
	uint32_t value_type      = 0;
	int result               = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_record_entry == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid value.",
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
	if( ( value_type != LIBPFF_VALUE_TYPE_STRING_ASCII )
	 && ( value_type != LIBPFF_VALUE_TYPE_STRING_UNICODE ) )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: value is not a string type.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_record_entry_get_value_utf8_string_size(
	          pypff_record_entry->record_entry,
	          &value_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value string size.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( ( result == 0 )
	      || ( value_string_size == 0 ) )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	value_string = (uint8_t *) PyMem_Malloc(
	                            sizeof( uint8_t ) * value_string_size );

	if( value_string == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to create value string.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_record_entry_get_value_utf8_string(
		  pypff_record_entry->record_entry,
		  value_string,
		  value_string_size,
		  &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value string.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	string_object = PyUnicode_DecodeUTF8(
			 (char *) value_string,
			 (Py_ssize_t) value_string_size - 1,
			 errors );

	PyMem_Free(
	 value_string );

	return( string_object );

on_error:
	if( value_string != NULL )
	{
		PyMem_Free(
		 value_string );
	}
	return( NULL );
}

