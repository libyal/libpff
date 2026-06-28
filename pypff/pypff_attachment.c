/*
 * Python object wrapper of libpff_item_t type LIBPFF_ITEM_TYPE_ATTACHMENT
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

#include "pypff_attachment.h"
#include "pypff_error.h"
#include "pypff_integer.h"
#include "pypff_item.h"
#include "pypff_libcerror.h"
#include "pypff_libpff.h"
#include "pypff_python.h"
#include "pypff_unused.h"

PyMethodDef pypff_attachment_object_methods[] = {

	/* Functions to access the attachment values */

	{ "get_long_filename",
	  (PyCFunction) pypff_attachment_get_long_filename,
	  METH_NOARGS,
	  "get_long_filename() -> Unicode string or None\n"
	  "\n"
	  "Retrieves the long filename." },

	{ "get_size",
	  (PyCFunction) pypff_attachment_get_size,
	  METH_NOARGS,
	  "get_size() -> Integer or None\n"
	  "\n"
	  "Retrieves the attachment data size." },

	{ "read_buffer",
	  (PyCFunction) pypff_attachment_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_buffer(size) -> String\n"
	  "\n"
	  "Reads a buffer of attachment data." },

	{ "seek_offset",
	  (PyCFunction) pypff_attachment_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "seek_offset(offset, whence) -> None\n"
	  "\n"
	  "Seeks an offset within the attachment data." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pypff_attachment_object_get_set_definitions[] = {

	{ "long_filename",
	  (getter) pypff_attachment_get_long_filename,
	  (setter) 0,
	  "The long filename.",
	  NULL },

	{ "size",
	  (getter) pypff_attachment_get_size,
	  (setter) 0,
	  "The data size.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pypff_attachment_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pypff.attachment",
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
	"pypff attachment object (wraps attachment type libpff_item_t)",
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
	pypff_attachment_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pypff_attachment_object_get_set_definitions,
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

/* Retrieves the long filename
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_attachment_get_long_filename(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error            = NULL;
	libpff_record_entry_t *record_entry = NULL;
	PyObject *string_object             = NULL;
	uint8_t *value_string               = NULL;
	static char *function               = "pypff_attachment_get_long_filename";
	size_t value_string_size            = 0;
	int result                          = 0;

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

	result = libpff_record_set_get_entry_by_type(
		  pypff_item->record_set,
		  LIBPFF_ENTRY_TYPE_ATTACHMENT_FILENAME_LONG,
		  0,
		  &record_entry,
		  LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE,
		  &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve long filename record entry.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( result == 1 )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libpff_record_entry_get_data_as_utf8_string_size(
			  record_entry,
			  &value_string_size,
			  &error );

		Py_END_ALLOW_THREADS

		if( result == -1 )
		{
			pypff_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to retrieve long filename UTF-8 string size.",
			 function );

			libcerror_error_free(
			 &error );

			goto on_error;
		}
		else if( value_string_size > 0 )
		{
			value_string = (uint8_t *) PyMem_Malloc(
						    sizeof( uint8_t ) * value_string_size );

			if( value_string == NULL )
			{
				PyErr_Format(
				 PyExc_MemoryError,
				 "%s: unable to create long filename.",
				 function );

				goto on_error;
			}
			Py_BEGIN_ALLOW_THREADS

			result = libpff_record_entry_get_data_as_utf8_string(
				  record_entry,
				  value_string,
				  value_string_size,
				  &error );

			Py_END_ALLOW_THREADS

			if( result != 1 )
			{
				pypff_error_raise(
				 error,
				 PyExc_IOError,
				 "%s: unable to retrieve long filename UTF-8 string.",
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
					 NULL );

			PyMem_Free(
			 value_string );

			value_string = NULL;
		}
		Py_BEGIN_ALLOW_THREADS

		result = libpff_record_entry_free(
			  &record_entry,
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

			goto on_error;
		}
	}
	if( string_object == NULL )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	return( string_object );

on_error:
	if( value_string != NULL )
	{
		PyMem_Free(
		 value_string );
	}
	if( record_entry != NULL )
	{
		libpff_record_entry_free(
		 &record_entry,
		 NULL );
	}
	return( NULL );
}

/* Retrieves the size
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_attachment_get_size(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pypff_attachment_get_size";
	size64_t size            = 0;
	int result               = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_item == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid item.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_attachment_get_data_size(
	          pypff_item->item,
	          &size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: failed to retrieve data size.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pypff_integer_unsigned_new_from_64bit(
	                  (uint64_t) size );

	return( integer_object );
}

/* Reads (attachment) data at the current offset into a buffer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_attachment_read_buffer(
           pypff_item_t *pypff_item,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	PyObject *string_object     = NULL;
	static char *function       = "pypff_attachment_read_buffer";
	static char *keyword_list[] = { "size", NULL };
	char *buffer                = NULL;
	ssize_t read_count          = 0;
	int read_size               = -1;

	if( pypff_item == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pypff item.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|i",
	     keyword_list,
	     &read_size ) == 0 )
	{
		return( NULL );
	}
	if( read_size < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value less than zero.",
		 function );

		return( NULL );
	}
	/* Make sure the data fits into the memory buffer
	 */
	if( read_size > INT_MAX )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value exceeds maximum.",
		 function );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
	                 NULL,
	                 read_size );

	buffer = PyBytes_AsString(
	          string_object );
#else
	string_object = PyString_FromStringAndSize(
	                 NULL,
	                 read_size );

	buffer = PyString_AsString(
	          string_object );
#endif
	Py_BEGIN_ALLOW_THREADS

	read_count = libpff_attachment_data_read_buffer(
	              pypff_item->item,
	              (uint8_t *) buffer,
	              (size_t) read_size,
	              &error );

	Py_END_ALLOW_THREADS

	if( read_count <= -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to read data.",
		 function );

		libcerror_error_free(
		 &error );

		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	/* Need to resize the string here in case read_size was not fully read.
	 */
#if PY_MAJOR_VERSION >= 3
	if( _PyBytes_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#else
	if( _PyString_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#endif
	{
		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	return( string_object );
}

/* Seeks a certain offset in the (attachment) data
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_attachment_seek_offset(
           pypff_item_t *pypff_item,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	static char *function       = "pypff_attachment_seek_offset";
	static char *keyword_list[] = { "offset", "whence", NULL };
	off64_t offset              = 0;
	int whence                  = 0;

	if( pypff_item == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pypff item.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "L|i",
	     keyword_list,
	     &offset,
	     &whence ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	offset = libpff_attachment_data_seek_offset(
	          pypff_item->item,
	          offset,
	          whence,
	          &error );

	Py_END_ALLOW_THREADS

 	if( offset == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to seek offset.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

