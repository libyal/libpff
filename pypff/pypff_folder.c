/*
 * Python object wrapper of libpff_item_t type LIBPFF_ITEM_TYPE_FOLDER
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
#include "pypff_folder.h"
#include "pypff_item.h"
#include "pypff_items.h"
#include "pypff_libcerror.h"
#include "pypff_libpff.h"
#include "pypff_message.h"
#include "pypff_python.h"
#include "pypff_unused.h"

PyMethodDef pypff_folder_object_methods[] = {

	/* Functions to access the folder values */

	{ "get_name",
	  (PyCFunction) pypff_folder_get_name,
	  METH_NOARGS,
	  "get_name() -> Unicode string or None\n"
	  "\n"
	  "Retrieves the name." },

	/* Functions to access the sub folders */

	{ "get_number_of_sub_folders",
	  (PyCFunction) pypff_folder_get_number_of_sub_folders,
	  METH_NOARGS,
	  "get_number_of_sub_folders() -> Integer\n"
	  "\n"
	  "Retrieves the number of sub folders." },

	{ "get_sub_folder",
	  (PyCFunction) pypff_folder_get_sub_folder,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_sub_folder(sub_folder_index) -> Object or None\n"
	  "\n"
	  "Retrieves a specific sub folder." },

	/* Functions to access the sub messages */

	{ "get_number_of_sub_messages",
	  (PyCFunction) pypff_folder_get_number_of_sub_messages,
	  METH_NOARGS,
	  "get_number_of_sub_messages() -> Integer\n"
	  "\n"
	  "Retrieves the number of sub messages." },

	{ "get_sub_message",
	  (PyCFunction) pypff_folder_get_sub_message,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_sub_message(sub_message_index) -> Object or None\n"
	  "\n"
	  "Retrieves a specific sub message." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pypff_folder_object_get_set_definitions[] = {

	{ "name",
	  (getter) pypff_folder_get_name,
	  (setter) 0,
	  "The name.",
	  NULL },

	{ "number_of_sub_folders",
	  (getter) pypff_folder_get_number_of_sub_folders,
	  (setter) 0,
	  "The number of sub folders.",
	  NULL },

	{ "sub_folders",
	  (getter) pypff_folder_get_sub_folders,
	  (setter) 0,
	  "The sub folders",
	  NULL },

	{ "number_of_sub_messages",
	  (getter) pypff_folder_get_number_of_sub_messages,
	  (setter) 0,
	  "The number of sub messages.",
	  NULL },

	{ "sub_messages",
	  (getter) pypff_folder_get_sub_messages,
	  (setter) 0,
	  "The sub messages",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pypff_folder_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pypff.folder",
	/* tp_basicsize */
	sizeof( pypff_item_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	0,
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
	"pypff folder object (wraps folder type libpff_item_t)",
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
	pypff_folder_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pypff_folder_object_get_set_definitions,
	/* tp_base */
	&pypff_item_type_object,
	/* tp_dict */
	0,
	/* tp_descr_get */
	0,
	/* tp_descr_set */
	0,
	/* tp_dictoffset */
	0,
	/* tp_init */
	0,
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

/* Retrieves the name
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_folder_get_name(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *string_object  = NULL;
	libcerror_error_t *error = NULL;
	uint8_t *utf8_string     = NULL;
	static char *function    = "pypff_folder_get_name";
	size_t utf8_string_size  = 0;
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

	result = libpff_folder_get_utf8_name_size(
	          pypff_item->item,
	          &utf8_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve size of UTF-8 name.",
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
	utf8_string = (uint8_t *) PyMem_Malloc(
	                           sizeof( uint8_t ) * utf8_string_size );

	if( utf8_string == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create name.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_folder_get_utf8_name(
		  pypff_item->item,
		  utf8_string,
		  utf8_string_size,
		  &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve UTF-8 name.",
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
			 (char *) utf8_string,
			 (Py_ssize_t) utf8_string_size - 1,
			 NULL );

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

/* Retrieves the number of sub folders
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_folder_get_number_of_sub_folders(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error  = NULL;
	PyObject *integer_object  = NULL;
	static char *function     = "pypff_folder_get_number_of_sub_folders";
	int number_of_sub_folders = 0;
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

	result = libpff_folder_get_number_of_sub_folders(
	          pypff_item->item,
	          &number_of_sub_folders,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of sub folders.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) number_of_sub_folders );
#else
	integer_object = PyInt_FromLong(
	                  (long) number_of_sub_folders );
#endif
	return( integer_object );
}

/* Retrieves a specific sub folder by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_folder_get_sub_folder_by_index(
           PyObject *pypff_item,
           int sub_folder_index )
{
	libcerror_error_t *error  = NULL;
	libpff_item_t *sub_item   = NULL;
	PyObject *sub_item_object = NULL;
	static char *function     = "pypff_folder_get_sub_folder_by_index";
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

	result = libpff_folder_get_sub_folder(
	          ( (pypff_item_t *) pypff_item )->item,
	          sub_folder_index,
	          &sub_item,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve sub folder: %d.",
		 function,
		 sub_folder_index );

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
		 "%s: unable to retrieve sub folder: %d type.",
		 function,
		 sub_folder_index );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	sub_item_object = pypff_item_new(
	                   &pypff_folder_type_object,
	                   sub_item,
	                   (PyObject *) ( (pypff_item_t *) pypff_item )->parent_object );

	if( sub_item_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create sub folder object.",
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

/* Retrieves a specific sub folder
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_folder_get_sub_folder(
           pypff_item_t *pypff_item,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *sub_item_object   = NULL;
	static char *keyword_list[] = { "sub_folder_index", NULL };
	int sub_folder_index        = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &sub_folder_index ) == 0 )
	{
		return( NULL );
	}
	sub_item_object = pypff_folder_get_sub_folder_by_index(
	                   (PyObject *) pypff_item,
	                   sub_folder_index );

	return( sub_item_object );
}

/* Retrieves an items sequence and iterator object for the sub folders
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_folder_get_sub_folders(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error   = NULL;
	PyObject *sub_items_object = NULL;
	static char *function      = "pypff_folder_get_sub_folders";
	int number_of_sub_folders  = 0;
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

	result = libpff_folder_get_number_of_sub_folders(
	          pypff_item->item,
	          &number_of_sub_folders,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of sub folders.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	sub_items_object = pypff_items_new(
	                    (PyObject *) pypff_item,
	                    &pypff_folder_get_sub_folder_by_index,
	                    number_of_sub_folders );

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

/* Retrieves the number of sub messages
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_folder_get_number_of_sub_messages(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error   = NULL;
	PyObject *integer_object   = NULL;
	static char *function      = "pypff_folder_get_number_of_sub_messages";
	int number_of_sub_messages = 0;
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

	result = libpff_folder_get_number_of_sub_messages(
	          pypff_item->item,
	          &number_of_sub_messages,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of sub messages.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) number_of_sub_messages );
#else
	integer_object = PyInt_FromLong(
	                  (long) number_of_sub_messages );
#endif
	return( integer_object );
}

/* Retrieves a specific sub message by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_folder_get_sub_message_by_index(
           PyObject *pypff_item,
           int sub_message_index )
{
	libcerror_error_t *error  = NULL;
	libpff_item_t *sub_item   = NULL;
	PyObject *sub_item_object = NULL;
	static char *function     = "pypff_folder_get_sub_message_by_index";
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

	result = libpff_folder_get_sub_message(
	          ( (pypff_item_t *) pypff_item )->item,
	          sub_message_index,
	          &sub_item,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve sub message: %d.",
		 function,
		 sub_message_index );

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
		 "%s: unable to retrieve sub message: %d type.",
		 function,
		 sub_message_index );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	sub_item_object = pypff_item_new(
	                   &pypff_message_type_object,
	                   sub_item,
	                   (PyObject *) ( (pypff_item_t *) pypff_item )->parent_object );

	if( sub_item_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create sub message object.",
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

/* Retrieves a specific sub message
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_folder_get_sub_message(
           pypff_item_t *pypff_item,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *sub_item_object   = NULL;
	static char *keyword_list[] = { "sub_message_index", NULL };
	int sub_message_index       = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &sub_message_index ) == 0 )
	{
		return( NULL );
	}
	sub_item_object = pypff_folder_get_sub_message_by_index(
	                   (PyObject *) pypff_item,
	                   sub_message_index );

	return( sub_item_object );
}

/* Retrieves an items sequence and iterator object for the sub messages
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_folder_get_sub_messages(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error   = NULL;
	PyObject *sub_items_object = NULL;
	static char *function      = "pypff_folder_get_sub_messages";
	int number_of_sub_messages = 0;
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

	result = libpff_folder_get_number_of_sub_messages(
	          pypff_item->item,
	          &number_of_sub_messages,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of sub messages.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	sub_items_object = pypff_items_new(
	                    (PyObject *) pypff_item,
	                    &pypff_folder_get_sub_message_by_index,
	                    number_of_sub_messages );

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

