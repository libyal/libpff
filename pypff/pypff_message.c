/*
 * Python object definition of the message type libpff item
 *
 * Copyright (c) 2008-2014, Joachim Metz <joachim.metz@gmail.com>
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
#include "pypff_item.h"
#include "pypff_libcerror.h"
#include "pypff_libcstring.h"
#include "pypff_libpff.h"
#include "pypff_message.h"
#include "pypff_python.h"
#include "pypff_unused.h"

PyMethodDef pypff_message_object_methods[] = {

	/* Functions to access the message values */

	{ "get_subject",
	  (PyCFunction) pypff_message_get_subject,
	  METH_NOARGS,
	  "get_subject() -> Unicode string or None\n"
	  "\n"
	  "Retrieves the subject." },

	{ "get_plain_text_body",
	  (PyCFunction) pypff_message_get_plain_text_body,
	  METH_NOARGS,
	  "get_plain_text_body() -> String or None\n"
	  "\n"
	  "Retrieves the plain-text body." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pypff_message_object_get_set_definitions[] = {

	{ "subject",
	  (getter) pypff_message_get_subject,
	  (setter) 0,
	  "The subject.",
	  NULL },

	{ "plain_text_body",
	  (getter) pypff_message_get_plain_text_body,
	  (setter) 0,
	  "The plain text body.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pypff_message_type_object = {
	PyObject_HEAD_INIT( NULL )

	/* ob_size */
	0,
	/* tp_name */
	"pypff.message",
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
	"pypff message object (wraps message type libpff_item_t)",
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
	pypff_message_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pypff_message_object_get_set_definitions,
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

/* Retrieves the subject
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_message_get_subject(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *string_object  = NULL;
	const char *errors       = NULL;
	uint8_t *value_string    = NULL;
	static char *function    = "pypff_message_get_subject";
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
	          LIBPFF_ENTRY_TYPE_MESSAGE_SUBJECT,
	          &value_string_size,
	          0,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve subject size.",
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
		 "%s: unable to create subject.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_item_get_entry_value_utf8_string(
		  pypff_item->item,
		  0,
		  LIBPFF_ENTRY_TYPE_MESSAGE_SUBJECT,
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
		 "%s: unable to retrieve subject.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Ignore the subject control codes for now
	 */
	if( value_string[ 0 ] < 0x20 )
	{
		/* Pass the string length to PyUnicode_DecodeUTF8
		 * otherwise it makes the end of string character is part
		 * of the string
		 */
		string_object = PyUnicode_DecodeUTF8(
				 (char *) &( value_string[ 2 ] ),
				 (Py_ssize_t) value_string_size - 3,
				 errors );
	}
	else
	{
		/* Pass the string length to PyUnicode_DecodeUTF8
		 * otherwise it makes the end of string character is part
		 * of the string
		 */
		string_object = PyUnicode_DecodeUTF8(
				 (char *) value_string,
				 (Py_ssize_t) value_string_size - 1,
				 errors );
	}
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

/* Retrieves the plain text body
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_message_get_plain_text_body(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *string_object  = NULL;
	const char *errors       = NULL;
	uint8_t *value_string    = NULL;
	static char *function    = "pypff_message_get_plain_text_body";
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

	result = libpff_message_get_plain_text_body_size(
	          pypff_item->item,
	          &value_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve plain text body size.",
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
		 "%s: unable to create plain text body.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_message_get_plain_text_body(
		  pypff_item->item,
		  value_string,
		  value_string_size,
		  &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve plain text body.",
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

