/*
 * Python object definition of the libpff item
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
#include "pypff_file.h"
#include "pypff_integer.h"
#include "pypff_item.h"
#include "pypff_items.h"
#include "pypff_libcerror.h"
#include "pypff_libcstring.h"
#include "pypff_libpff.h"
#include "pypff_python.h"
#include "pypff_record_set.h"
#include "pypff_record_sets.h"
#include "pypff_unused.h"

PyMethodDef pypff_item_object_methods[] = {

	/* Functions to access the item values */

	{ "get_number_of_record_sets",
	  (PyCFunction) pypff_item_get_number_of_record_sets,
	  METH_NOARGS,
	  "get_number_of_record_sets() -> Integer\n"
	  "\n"
	  "Retrieves the number of record sets." },

	{ "get_record_set",
	  (PyCFunction) pypff_item_get_record_set,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_record_set(record_set_index) -> Object or None\n"
	  "\n"
	  "Retrieves a specific record set." },

	{ "get_display_name",
	  (PyCFunction) pypff_item_get_display_name,
	  METH_NOARGS,
	  "get_display_name() -> Unicode string or None\n"
	  "\n"
	  "Retrieves the display name." },

	/* Functions to access the sub items */

	{ "get_number_of_sub_items",
	  (PyCFunction) pypff_item_get_number_of_sub_items,
	  METH_NOARGS,
	  "get_number_of_sub_items() -> Integer\n"
	  "\n"
	  "Retrieves the number of sub items." },

	{ "get_sub_item",
	  (PyCFunction) pypff_item_get_sub_item,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_sub_item(sub_item_index) -> Object or None\n"
	  "\n"
	  "Retrieves a specific sub item." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pypff_item_object_get_set_definitions[] = {

	{ "number_of_record_sets",
	  (getter) pypff_item_get_number_of_record_sets,
	  (setter) 0,
	  "The number of record sets.",
	  NULL },

	{ "record_sets",
	  (getter) pypff_item_get_record_sets,
	  (setter) 0,
	  "The record sets",
	  NULL },

	{ "display_name",
	  (getter) pypff_item_get_display_name,
	  (setter) 0,
	  "The display name.",
	  NULL },

	{ "number_of_sub_items",
	  (getter) pypff_item_get_number_of_sub_items,
	  (setter) 0,
	  "The number of sub items.",
	  NULL },

	{ "sub_items",
	  (getter) pypff_item_get_sub_items,
	  (setter) 0,
	  "The items",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pypff_item_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pypff.item",
	/* tp_basicsize */
	sizeof( pypff_item_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pypff_item_free,
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
	"pypff item object (wraps libpff_item_t)",
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
	pypff_item_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pypff_item_object_get_set_definitions,
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
	(initproc) pypff_item_init,
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

/* Creates a new item object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_item_new(
           PyTypeObject *type_object,
           libpff_item_t *item,
           pypff_file_t *file_object )
{
	pypff_item_t *pypff_item = NULL;
	static char *function    = "pypff_item_new";

	if( item == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid item.",
		 function );

		return( NULL );
	}
	pypff_item = PyObject_New(
	              struct pypff_item,
	              type_object );

	if( pypff_item == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize item.",
		 function );

		goto on_error;
	}
	if( pypff_item_init(
	     pypff_item ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize item.",
		 function );

		goto on_error;
	}
	pypff_item->item        = item;
	pypff_item->file_object = file_object;

	Py_IncRef(
	 (PyObject *) pypff_item->file_object );

	return( (PyObject *) pypff_item );

on_error:
	if( pypff_item != NULL )
	{
		Py_DecRef(
		 (PyObject *) pypff_item );
	}
	return( NULL );
}

/* Intializes an item object
 * Returns 0 if successful or -1 on error
 */
int pypff_item_init(
     pypff_item_t *pypff_item )
{
	static char *function = "pypff_item_init";

	if( pypff_item == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid item.",
		 function );

		return( -1 );
	}
	/* Make sure libpff item is set to NULL
	 */
	pypff_item->item = NULL;

	return( 0 );
}

/* Frees an item object
 */
void pypff_item_free(
      pypff_item_t *pypff_item )
{
	libcerror_error_t *error    = NULL;
	struct _typeobject *ob_type = NULL;
	static char *function       = "pypff_item_free";

	if( pypff_item == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid item.",
		 function );

		return;
	}
	if( pypff_item->item == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid item - missing libpff item.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pypff_item );

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
	if( libpff_item_free(
	     &( pypff_item->item ),
	     &error ) != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to free libpff item.",
		 function );

		libcerror_error_free(
		 &error );
	}
	if( pypff_item->file_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) pypff_item->file_object );
	}
	ob_type->tp_free(
	 (PyObject*) pypff_item );
}

/* Retrieves the number of record sets
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_item_get_number_of_record_sets(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error  = NULL;
	PyObject *integer_object  = NULL;
	static char *function     = "pypff_item_get_number_of_record_sets";
	int number_of_record_sets = 0;
	int result                = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_item == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid item.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_item_get_number_of_record_sets(
	          pypff_item->item,
	          &number_of_record_sets,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of record sets.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pypff_integer_unsigned_new_from_64bit(
	                  (uint64_t) number_of_record_sets );

	return( integer_object );
}

/* Retrieves a specific record set by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_item_get_record_set_by_index(
           pypff_item_t *pypff_item,
           int record_set_index )
{
	libcerror_error_t *error        = NULL;
	libpff_record_set_t *record_set = NULL;
	PyObject *record_set_object     = NULL;
	static char *function           = "pypff_item_get_record_set_by_index";
	uint8_t record_set_type         = 0;
	int result                      = 0;

	if( pypff_item == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid item.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_item_get_record_set_by_index(
	          pypff_item->item,
	          record_set_index,
	          &record_set,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve record set: %d.",
		 function,
		 record_set_index );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_item_get_type(
	          record_set,
	          &record_set_type,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve record set: %d type.",
		 function,
		 record_set_index );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	record_set_object = pypff_item_new(
	                     &pypff_item_type_object,
	                     record_set,
	                     pypff_item->file_object );

	if( record_set_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create record set object.",
		 function );

		goto on_error;
	}
	return( record_set_object );

on_error:
	if( record_set != NULL )
	{
		libpff_item_free(
		 &record_set,
		 NULL );
	}
	return( NULL );
}

/* Retrieves a specific record set
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_item_get_record_set(
           pypff_item_t *pypff_item,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *record_set_object = NULL;
	static char *keyword_list[] = { "record_set_index", NULL };
	int record_set_index        = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &record_set_index ) == 0 )
	{
		return( NULL );
	}
	record_set_object = pypff_item_get_record_set_by_index(
	                     pypff_item,
	                     record_set_index );

	return( record_set_object );
}

/* Retrieves an record sets sequence and iterator object for the record sets
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_item_get_record_sets(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error     = NULL;
	PyObject *record_sets_object = NULL;
	static char *function        = "pypff_item_get_record_sets";
	int number_of_record_sets    = 0;
	int result                   = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_item == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid item.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_item_get_number_of_record_sets(
	          pypff_item->item,
	          &number_of_record_sets,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of record sets.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	record_sets_object = pypff_record_sets_new(
	                      pypff_item,
	                      &pypff_item_get_record_set_by_index,
	                      number_of_record_sets );

	if( record_sets_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create record sets object.",
		 function );

		return( NULL );
	}
	return( record_sets_object );
}

/* Retrieves the display name
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_item_get_display_name(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *string_object  = NULL;
	const char *errors       = NULL;
	uint8_t *value_string    = NULL;
	static char *function    = "pypff_item_get_display_name";
	size_t value_string_size = 0;
	int result               = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_item == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid item.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_item_get_entry_value_utf8_string_size(
	          pypff_item->item,
	          0,
	          LIBPFF_ENTRY_TYPE_DISPLAY_NAME,
	          &value_string_size,
	          0,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve display name size.",
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
		 "%s: unable to create display name.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_item_get_entry_value_utf8_string(
		  pypff_item->item,
		  0,
		  LIBPFF_ENTRY_TYPE_DISPLAY_NAME,
		  value_string,
		  value_string_size,
		  0,
		  &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve display name.",
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

/* Retrieves the number of sub items
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_item_get_number_of_sub_items(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pypff_item_get_number_of_sub_items";
	int number_of_sub_items  = 0;
	int result               = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_item == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid item.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_item_get_number_of_sub_items(
	          pypff_item->item,
	          &number_of_sub_items,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of sub items.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) number_of_sub_items );
#else
	integer_object = PyInt_FromLong(
	                  (long) number_of_sub_items );
#endif
	return( integer_object );
}

/* Retrieves a specific sub item by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_item_get_sub_item_by_index(
           pypff_item_t *pypff_item,
           int sub_item_index )
{
	libcerror_error_t *error  = NULL;
	libpff_item_t *sub_item   = NULL;
	PyObject *sub_item_object = NULL;
	static char *function     = "pypff_item_get_sub_item_by_index";
	uint8_t sub_item_type     = 0;
	int result                = 0;

	if( pypff_item == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid item.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_item_get_sub_item(
	          pypff_item->item,
	          sub_item_index,
	          &sub_item,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve sub item: %d.",
		 function,
		 sub_item_index );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_item_get_type(
	          sub_item,
	          &sub_item_type,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve sub item: %d type.",
		 function,
		 sub_item_index );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	sub_item_object = pypff_item_new(
	                   &pypff_item_type_object,
	                   sub_item,
	                   pypff_item->file_object );

	if( sub_item_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create sub item object.",
		 function );

		goto on_error;
	}
	return( sub_item_object );

on_error:
	if( sub_item != NULL )
	{
		libpff_item_free(
		 &sub_item,
		 NULL );
	}
	return( NULL );
}

/* Retrieves a specific sub item
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_item_get_sub_item(
           pypff_item_t *pypff_item,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *sub_item_object   = NULL;
	static char *keyword_list[] = { "sub_item_index", NULL };
	int sub_item_index          = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &sub_item_index ) == 0 )
	{
		return( NULL );
	}
	sub_item_object = pypff_item_get_sub_item_by_index(
	                   pypff_item,
	                   sub_item_index );

	return( sub_item_object );
}

/* Retrieves an items sequence and iterator object for the items
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_item_get_sub_items(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error   = NULL;
	PyObject *sub_items_object = NULL;
	static char *function      = "pypff_item_get_sub_items";
	int number_of_sub_items    = 0;
	int result                 = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_item == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid item.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_item_get_number_of_sub_items(
	          pypff_item->item,
	          &number_of_sub_items,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of sub items.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	sub_items_object = pypff_items_new(
	                    pypff_item,
	                    &pypff_item_get_sub_item_by_index,
	                    number_of_sub_items );

	if( sub_items_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create sub items object.",
		 function );

		return( NULL );
	}
	return( sub_items_object );
}

