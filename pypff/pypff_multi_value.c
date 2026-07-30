/*
 * Python object wrapper of libpff_multi_value_t
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

#include "pypff_datetime.h"
#include "pypff_error.h"
#include "pypff_integer.h"
#include "pypff_libcerror.h"
#include "pypff_libpff.h"
#include "pypff_multi_value.h"
#include "pypff_python.h"
#include "pypff_unused.h"

PyMethodDef pypff_multi_value_object_methods[] = {

	{ "get_number_of_values",
	  (PyCFunction) pypff_multi_value_get_number_of_values,
	  METH_NOARGS,
	  "get_number_of_values() -> Integer\n"
	  "\n"
	  "Retrieves the number of values." },

	{ "get_value_type",
	  (PyCFunction) pypff_multi_value_get_value_type,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_value_type(value_index) -> Integer\n"
	  "\n"
	  "Retrieves the type of the value specified by the index." },

	{ "get_value",
	  (PyCFunction) pypff_multi_value_get_value,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_value(value_index) -> Bytes or None\n"
	  "\n"
	  "Retrieves the value specified by the index as bytes." },

	{ "get_value_as_integer",
	  (PyCFunction) pypff_multi_value_get_value_as_integer,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_value_as_integer(value_index) -> Integer\n"
	  "\n"
	  "Retrieves the value specified by the index as an integer." },

	{ "get_value_as_datetime",
	  (PyCFunction) pypff_multi_value_get_value_as_datetime,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_value_as_datetime(value_index) -> Datetime\n"
	  "\n"
	  "Retrieves the value specified by the index as a datetime object." },

	{ "get_value_as_string",
	  (PyCFunction) pypff_multi_value_get_value_as_string,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_value_as_string(value_index) -> Unicode string or None\n"
	  "\n"
	  "Retrieves the value specified by the index as a string." },

	{ "get_value_as_binary_data",
	  (PyCFunction) pypff_multi_value_get_value_as_binary_data,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_value_as_binary_data(value_index) -> Bytes or None\n"
	  "\n"
	  "Retrieves the value specified by the index as binary data." },

	{ "get_value_as_guid",
	  (PyCFunction) pypff_multi_value_get_value_as_guid,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_value_as_guid(value_index) -> Bytes\n"
	  "\n"
	  "Retrieves the value specified by the index as a GUID." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pypff_multi_value_object_get_set_definitions[] = {

	{ "number_of_values",
	  (getter) pypff_multi_value_get_number_of_values,
	  (setter) 0,
	  "The number of values.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pypff_multi_value_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pypff.multi_value",
	/* tp_basicsize */
	sizeof( pypff_multi_value_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pypff_multi_value_free,
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
	"pypff multi value object (wraps libpff_multi_value_t)",
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
	pypff_multi_value_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pypff_multi_value_object_get_set_definitions,
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
	(initproc) pypff_multi_value_init,
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

/* Creates a new multi value object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_multi_value_new(
           libpff_multi_value_t *multi_value,
           PyObject *parent_object )
{
	pypff_multi_value_t *pypff_multi_value = NULL;
	static char *function                  = "pypff_multi_value_new";

	if( multi_value == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid multi value.",
		 function );

		return( NULL );
	}
	pypff_multi_value = PyObject_New(
	                     struct pypff_multi_value,
	                     &pypff_multi_value_type_object );

	if( pypff_multi_value == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize multi value.",
		 function );

		return( NULL );
	}
	pypff_multi_value->multi_value   = multi_value;
	pypff_multi_value->parent_object = parent_object;

	if( parent_object != NULL )
	{
		Py_IncRef(
		 parent_object );
	}
	return( (PyObject *) pypff_multi_value );
}

/* Initializes a multi value object
 * Returns 0 if successful or -1 on error
 */
int pypff_multi_value_init(
     pypff_multi_value_t *pypff_multi_value )
{
	static char *function = "pypff_multi_value_init";

	if( pypff_multi_value == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid multi value.",
		 function );

		return( -1 );
	}
	pypff_multi_value->multi_value   = NULL;
	pypff_multi_value->parent_object = NULL;

	return( 0 );
}

/* Frees a multi value object
 */
void pypff_multi_value_free(
      pypff_multi_value_t *pypff_multi_value )
{
	struct _typeobject *ob_type = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pypff_multi_value_free";
	int result                  = 0;

	if( pypff_multi_value == NULL )
	{
		return;
	}
	ob_type = Py_TYPE(
	           pypff_multi_value );

	if( pypff_multi_value->multi_value != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libpff_multi_value_free(
		          &( pypff_multi_value->multi_value ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pypff_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to free libpff multi value.",
			 function );

			libcerror_error_free(
			 &error );
		}
	}
	if( pypff_multi_value->parent_object != NULL )
	{
		Py_DecRef(
		 pypff_multi_value->parent_object );
	}
	if( ( ob_type != NULL )
	 && ( ob_type->tp_free != NULL ) )
	{
		ob_type->tp_free(
		 (PyObject *) pypff_multi_value );
	}
}

/* Retrieves the number of values
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_multi_value_get_number_of_values(
           pypff_multi_value_t *pypff_multi_value,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pypff_multi_value_get_number_of_values";
	int number_of_values     = 0;
	int result               = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( ( pypff_multi_value == NULL )
	 || ( pypff_multi_value->multi_value == NULL ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid multi value.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_multi_value_get_number_of_values(
	          pypff_multi_value->multi_value,
	          &number_of_values,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of values.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) number_of_values );
#else
	integer_object = PyInt_FromLong(
	                  (long) number_of_values );
#endif
	return( integer_object );
}

/* Retrieves the type of a specific value
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_multi_value_get_value_type(
           pypff_multi_value_t *pypff_multi_value,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *integer_object     = NULL;
	libcerror_error_t *error     = NULL;
	uint8_t *value_data          = NULL;
	static char *function        = "pypff_multi_value_get_value_type";
	static char *keyword_list[]  = { "value_index", NULL };
	size_t value_data_size       = 0;
	uint32_t value_type          = 0;
	int value_index              = 0;
	int result                   = 0;

	if( ( pypff_multi_value == NULL )
	 || ( pypff_multi_value->multi_value == NULL ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid multi value.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &value_index ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_multi_value_get_value(
	          pypff_multi_value->multi_value,
	          value_index,
	          &value_type,
	          &value_data,
	          &value_data_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value: %d type.",
		 function,
		 value_index );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pypff_integer_unsigned_new_from_64bit(
	                  (uint64_t) value_type );

	return( integer_object );
}

/* Retrieves a specific value as bytes
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_multi_value_get_value(
           pypff_multi_value_t *pypff_multi_value,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *bytes_object       = NULL;
	libcerror_error_t *error     = NULL;
	uint8_t *value_data          = NULL;
	static char *function        = "pypff_multi_value_get_value";
	static char *keyword_list[]  = { "value_index", NULL };
	size_t value_data_size       = 0;
	uint32_t value_type          = 0;
	int value_index              = 0;
	int result                   = 0;

	if( ( pypff_multi_value == NULL )
	 || ( pypff_multi_value->multi_value == NULL ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid multi value.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &value_index ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_multi_value_get_value(
	          pypff_multi_value->multi_value,
	          value_index,
	          &value_type,
	          &value_data,
	          &value_data_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value: %d.",
		 function,
		 value_index );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( ( value_data == NULL )
	 || ( value_data_size == 0 ) )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	bytes_object = PyBytes_FromStringAndSize(
	                (char *) value_data,
	                (Py_ssize_t) value_data_size );

	return( bytes_object );
}

/* Retrieves a specific value as an integer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_multi_value_get_value_as_integer(
           pypff_multi_value_t *pypff_multi_value,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *integer_object     = NULL;
	libcerror_error_t *error     = NULL;
	uint8_t *value_data          = NULL;
	static char *function        = "pypff_multi_value_get_value_as_integer";
	static char *keyword_list[]  = { "value_index", NULL };
	size_t value_data_size       = 0;
	uint64_t value_64bit         = 0;
	uint32_t value_32bit         = 0;
	uint32_t value_type          = 0;
	int value_index              = 0;
	int result                   = 0;

	if( ( pypff_multi_value == NULL )
	 || ( pypff_multi_value->multi_value == NULL ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid multi value.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &value_index ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_multi_value_get_value(
	          pypff_multi_value->multi_value,
	          value_index,
	          &value_type,
	          &value_data,
	          &value_data_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value: %d type.",
		 function,
		 value_index );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	switch( value_type )
	{
		case LIBPFF_VALUE_TYPE_INTEGER_16BIT_SIGNED:
		case LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED:
			Py_BEGIN_ALLOW_THREADS

			result = libpff_multi_value_get_value_32bit(
			          pypff_multi_value->multi_value,
			          value_index,
			          &value_32bit,
			          &error );

			Py_END_ALLOW_THREADS

#if PY_MAJOR_VERSION >= 3
			if( value_type == LIBPFF_VALUE_TYPE_INTEGER_16BIT_SIGNED )
			{
				integer_object = PyLong_FromLong(
				                  (long) ( (int16_t) value_32bit ) );
			}
			else
			{
				integer_object = PyLong_FromLong(
				                  (long) ( (int32_t) value_32bit ) );
			}
#else
			if( value_type == LIBPFF_VALUE_TYPE_INTEGER_16BIT_SIGNED )
			{
				integer_object = PyInt_FromLong(
				                  (long) ( (int16_t) value_32bit ) );
			}
			else
			{
				integer_object = PyInt_FromLong(
				                  (long) ( (int32_t) value_32bit ) );
			}
#endif
			break;

		case LIBPFF_VALUE_TYPE_INTEGER_64BIT_SIGNED:
		case LIBPFF_VALUE_TYPE_FILETIME:
		case LIBPFF_VALUE_TYPE_FLOATINGTIME:
			Py_BEGIN_ALLOW_THREADS

			result = libpff_multi_value_get_value_64bit(
			          pypff_multi_value->multi_value,
			          value_index,
			          &value_64bit,
			          &error );

			Py_END_ALLOW_THREADS

			if( value_type == LIBPFF_VALUE_TYPE_INTEGER_64BIT_SIGNED )
			{
				integer_object = pypff_integer_signed_new_from_64bit(
				                  (int64_t) value_64bit );
			}
			else
			{
				integer_object = pypff_integer_unsigned_new_from_64bit(
				                  value_64bit );
			}
			break;

		default:
			PyErr_Format(
			 PyExc_IOError,
			 "%s: value is not an integer type.",
			 function );

			return( NULL );
	}
	if( result != 1 )
	{
		Py_XDECREF(
		 integer_object );

		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value: %d as an integer.",
		 function,
		 value_index );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	return( integer_object );
}

/* Retrieves a specific value as a datetime object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_multi_value_get_value_as_datetime(
           pypff_multi_value_t *pypff_multi_value,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *datetime_object    = NULL;
	libcerror_error_t *error     = NULL;
	uint8_t *value_data          = NULL;
	static char *function        = "pypff_multi_value_get_value_as_datetime";
	static char *keyword_list[]  = { "value_index", NULL };
	size_t value_data_size       = 0;
	uint64_t filetime            = 0;
	uint32_t value_type          = 0;
	int value_index              = 0;
	int result                   = 0;

	if( ( pypff_multi_value == NULL )
	 || ( pypff_multi_value->multi_value == NULL ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid multi value.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &value_index ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_multi_value_get_value(
	          pypff_multi_value->multi_value,
	          value_index,
	          &value_type,
	          &value_data,
	          &value_data_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value: %d type.",
		 function,
		 value_index );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( value_type != LIBPFF_VALUE_TYPE_FILETIME )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: value is not a datetime type.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_multi_value_get_value_filetime(
	          pypff_multi_value->multi_value,
	          value_index,
	          &filetime,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value: %d as a datetime.",
		 function,
		 value_index );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	datetime_object = pypff_datetime_new_from_filetime(
	                   filetime );

	return( datetime_object );
}

/* Retrieves a specific value as a string
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_multi_value_get_value_as_string(
           pypff_multi_value_t *pypff_multi_value,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *string_object      = NULL;
	libcerror_error_t *error     = NULL;
	char *utf8_string            = NULL;
	static char *function        = "pypff_multi_value_get_value_as_string";
	static char *keyword_list[]  = { "value_index", NULL };
	size_t utf8_string_size      = 0;
	int value_index              = 0;
	int result                   = 0;

	if( ( pypff_multi_value == NULL )
	 || ( pypff_multi_value->multi_value == NULL ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid multi value.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &value_index ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_multi_value_get_value_utf8_string_size(
	          pypff_multi_value->multi_value,
	          value_index,
	          &utf8_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to determine size of value: %d as UTF-8 string.",
		 function,
		 value_index );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( ( result == 0 )
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

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_multi_value_get_value_utf8_string(
	          pypff_multi_value->multi_value,
	          value_index,
	          (uint8_t *) utf8_string,
	          utf8_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value: %d as UTF-8 string.",
		 function,
		 value_index );

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

/* Retrieves a specific value as binary data
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_multi_value_get_value_as_binary_data(
           pypff_multi_value_t *pypff_multi_value,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *bytes_object       = NULL;
	libcerror_error_t *error     = NULL;
	uint8_t *binary_data         = NULL;
	static char *function        = "pypff_multi_value_get_value_as_binary_data";
	static char *keyword_list[]  = { "value_index", NULL };
	size_t binary_data_size      = 0;
	int value_index              = 0;
	int result                   = 0;

	if( ( pypff_multi_value == NULL )
	 || ( pypff_multi_value->multi_value == NULL ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid multi value.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &value_index ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_multi_value_get_value_binary_data_size(
	          pypff_multi_value->multi_value,
	          value_index,
	          &binary_data_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to determine size of value: %d as binary data.",
		 function,
		 value_index );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( ( result == 0 )
	 || ( binary_data_size == 0 ) )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	binary_data = (uint8_t *) PyMem_Malloc(
	                           sizeof( uint8_t ) * binary_data_size );

	if( binary_data == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create binary data.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_multi_value_get_value_binary_data(
	          pypff_multi_value->multi_value,
	          value_index,
	          binary_data,
	          binary_data_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value: %d as binary data.",
		 function,
		 value_index );

		libcerror_error_free(
		 &error );

		PyMem_Free(
		 binary_data );

		return( NULL );
	}
	bytes_object = PyBytes_FromStringAndSize(
	                (char *) binary_data,
	                (Py_ssize_t) binary_data_size );

	PyMem_Free(
	 binary_data );

	return( bytes_object );
}

/* Retrieves a specific value as a GUID
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_multi_value_get_value_as_guid(
           pypff_multi_value_t *pypff_multi_value,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *bytes_object       = NULL;
	libcerror_error_t *error     = NULL;
	static char *function        = "pypff_multi_value_get_value_as_guid";
	static char *keyword_list[]  = { "value_index", NULL };
	uint8_t guid[ 16 ];
	int value_index              = 0;
	int result                   = 0;

	if( ( pypff_multi_value == NULL )
	 || ( pypff_multi_value->multi_value == NULL ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid multi value.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &value_index ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_multi_value_get_value_guid(
	          pypff_multi_value->multi_value,
	          value_index,
	          guid,
	          16,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value: %d as a GUID.",
		 function,
		 value_index );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	bytes_object = PyBytes_FromStringAndSize(
	                (char *) guid,
	                16 );

	return( bytes_object );
}
