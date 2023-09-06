/*
 * Python object wrapper of libpff_file_t
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
#include <narrow_string.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pypff_attachment.h"
#include "pypff_codepage.h"
#include "pypff_error.h"
#include "pypff_file.h"
#include "pypff_file_object_io_handle.h"
#include "pypff_folder.h"
#include "pypff_integer.h"
#include "pypff_item.h"
#include "pypff_items.h"
#include "pypff_libbfio.h"
#include "pypff_libcerror.h"
#include "pypff_libclocale.h"
#include "pypff_libpff.h"
#include "pypff_message.h"
#include "pypff_python.h"
#include "pypff_unused.h"

#if !defined( LIBPFF_HAVE_BFIO )

LIBPFF_EXTERN \
int libpff_file_open_file_io_handle(
     libpff_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libpff_error_t **error );

#endif /* !defined( LIBPFF_HAVE_BFIO ) */

PyMethodDef pypff_file_object_methods[] = {

	{ "signal_abort",
	  (PyCFunction) pypff_file_signal_abort,
	  METH_NOARGS,
	  "signal_abort() -> None\n"
	  "\n"
	  "Signals the file to abort the current activity." },

	{ "open",
	  (PyCFunction) pypff_file_open,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filename, mode='r') -> None\n"
	  "\n"
	  "Opens a file." },

	{ "open_file_object",
	  (PyCFunction) pypff_file_open_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "open_file_object(file_object, mode='r') -> None\n"
	  "\n"
	  "Opens a file using a file-like object." },

	{ "close",
	  (PyCFunction) pypff_file_close,
	  METH_NOARGS,
	  "close() -> None\n"
	  "\n"
	  "Closes a file." },

	{ "get_size",
	  (PyCFunction) pypff_file_get_size,
	  METH_NOARGS,
	  "get_size() -> Integer or None\n"
	  "\n"
	  "Retrieves the size." },

	{ "get_content_type",
	  (PyCFunction) pypff_file_get_content_type,
	  METH_NOARGS,
	  "get_content_type() -> Integer or None\n"
	  "\n"
	  "Retrieves the content type." },

	{ "get_encryption_type",
	  (PyCFunction) pypff_file_get_encryption_type,
	  METH_NOARGS,
	  "get_encryption_type() -> Integer or None\n"
	  "\n"
	  "Retrieves the encryption type." },

	{ "get_ascii_codepage",
	  (PyCFunction) pypff_file_get_ascii_codepage,
	  METH_NOARGS,
	  "get_ascii_codepage() -> String\n"
	  "\n"
	  "Retrieves the codepage for ASCII strings used in the file." },

	{ "set_ascii_codepage",
	  (PyCFunction) pypff_file_set_ascii_codepage,
	  METH_VARARGS | METH_KEYWORDS,
	  "set_ascii_codepage(codepage) -> None\n"
	  "\n"
	  "Sets the codepage for ASCII strings used in the file.\n"
	  "Expects the codepage to be a string containing a Python codec definition." },

	{ "get_root_item",
	  (PyCFunction) pypff_file_get_root_item,
	  METH_NOARGS,
	  "get_root_item() -> Object or None\n"
	  "\n"
	  "Retrieves the root item." },

	{ "get_message_store",
	  (PyCFunction) pypff_file_get_message_store,
	  METH_NOARGS,
	  "get_message_store() -> Object or None\n"
	  "\n"
	  "Retrieves the message store." },

	{ "get_name_to_id_map",
	  (PyCFunction) pypff_file_get_name_to_id_map,
	  METH_NOARGS,
	  "get_name_to_id_map() -> Object or None\n"
	  "\n"
	  "Retrieves the name to id map." },

	{ "get_root_folder",
	  (PyCFunction) pypff_file_get_root_folder,
	  METH_NOARGS,
	  "get_root_folder() -> Object or None\n"
	  "\n"
	  "Retrieves the root folder." },

	{ "get_number_of_orphan_items",
	  (PyCFunction) pypff_file_get_number_of_orphan_items,
	  METH_NOARGS,
	  "get_number_of_orphan_items() -> Integer or None\n"
	  "\n"
	  "Retrieves the number of orphan items." },

	{ "get_orphan_item",
	  (PyCFunction) pypff_file_get_orphan_item,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_orphan_item(orphan_item_index) -> Object or None\n"
	  "\n"
	  "Retrieves the orphan item specified by the index." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pypff_file_object_get_set_definitions[] = {

	{ "size",
	  (getter) pypff_file_get_size,
	  (setter) 0,
	  "The size.",
	  NULL },

	{ "content_type",
	  (getter) pypff_file_get_content_type,
	  (setter) 0,
	  "The content type.",
	  NULL },

	{ "encryption_type",
	  (getter) pypff_file_get_encryption_type,
	  (setter) 0,
	  "The encryption type.",
	  NULL },

	{ "ascii_codepage",
	  (getter) pypff_file_get_ascii_codepage,
	  (setter) pypff_file_set_ascii_codepage_setter,
	  "The codepage used for ASCII strings in the file.",
	  NULL },

	{ "root_item",
	  (getter) pypff_file_get_root_item,
	  (setter) 0,
	  "The root item.",
	  NULL },

	{ "message_store",
	  (getter) pypff_file_get_message_store,
	  (setter) 0,
	  "The message store.",
	  NULL },

	{ "name_to_id_map",
	  (getter) pypff_file_get_name_to_id_map,
	  (setter) 0,
	  "The name to id map.",
	  NULL },

	{ "root_folder",
	  (getter) pypff_file_get_root_folder,
	  (setter) 0,
	  "The root folder.",
	  NULL },

	{ "number_of_orphan_items",
	  (getter) pypff_file_get_number_of_orphan_items,
	  (setter) 0,
	  "The number of orphan items.",
	  NULL },

	{ "orphan_items",
	  (getter) pypff_file_get_orphan_items,
	  (setter) 0,
	  "The orphan items.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pypff_file_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pypff.file",
	/* tp_basicsize */
	sizeof( pypff_file_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pypff_file_free,
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
	"pypff file object (wraps libpff_file_t)",
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
	pypff_file_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pypff_file_object_get_set_definitions,
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
	(initproc) pypff_file_init,
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

/* Initializes a file object
 * Returns 0 if successful or -1 on error
 */
int pypff_file_init(
     pypff_file_t *pypff_file )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pypff_file_init";

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	pypff_file->file           = NULL;
	pypff_file->file_io_handle = NULL;

	if( libpff_file_initialize(
	     &( pypff_file->file ),
	     &error ) != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize file.",
		 function );

		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 0 );
}

/* Frees a file object
 */
void pypff_file_free(
      pypff_file_t *pypff_file )
{
	struct _typeobject *ob_type = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pypff_file_free";
	int result                  = 0;

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pypff_file );

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
	if( pypff_file->file_io_handle != NULL )
	{
		if( pypff_file_close(
		     pypff_file,
		     NULL ) == NULL )
		{
			return;
		}
	}
	if( pypff_file->file != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libpff_file_free(
		          &( pypff_file->file ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pypff_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free libpff file.",
			 function );

			libcerror_error_free(
			 &error );
		}
	}
	ob_type->tp_free(
	 (PyObject*) pypff_file );
}

/* Signals the file to abort the current activity
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_file_signal_abort(
           pypff_file_t *pypff_file,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pypff_file_signal_abort";
	int result               = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_file_signal_abort(
	          pypff_file->file,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to signal abort.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Opens a file
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_file_open(
           pypff_file_t *pypff_file,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *string_object      = NULL;
	libcerror_error_t *error     = NULL;
	const char *filename_narrow  = NULL;
	static char *function        = "pypff_file_open";
	static char *keyword_list[]  = { "filename", "mode", NULL };
	char *mode                   = NULL;
	int result                   = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	const wchar_t *filename_wide = NULL;
#else
	PyObject *utf8_string_object = NULL;
#endif

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. This will also fail if the default encoding is not set correctly. We cannot use "u" here either since that
	 * does not allow us to pass non Unicode string objects and Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|s",
	     keyword_list,
	     &string_object,
	     &mode ) == 0 )
	{
		return( NULL );
	}
	if( ( mode != NULL )
	 && ( mode[ 0 ] != 'r' ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unsupported mode: %s.",
		 function,
		 mode );

		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyUnicode_Type );

	if( result == -1 )
	{
		pypff_error_fetch_and_raise(
		 PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type unicode.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
#if PY_MAJOR_VERSION >= 3 && PY_MINOR_VERSION >= 3
		filename_wide = (wchar_t *) PyUnicode_AsWideCharString(
		                             string_object,
		                             NULL );
#else
		filename_wide = (wchar_t *) PyUnicode_AsUnicode(
		                             string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libpff_file_open_wide(
		          pypff_file->file,
		          filename_wide,
		          LIBPFF_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS

#if PY_MAJOR_VERSION >= 3 && PY_MINOR_VERSION >= 3
		PyMem_Free(
		 filename_wide );
#endif
#else
		utf8_string_object = PyUnicode_AsUTF8String(
		                      string_object );

		if( utf8_string_object == NULL )
		{
			pypff_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to convert unicode string to UTF-8.",
			 function );

			return( NULL );
		}
#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
		                   utf8_string_object );
#else
		filename_narrow = PyString_AsString(
		                   utf8_string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libpff_file_open(
		          pypff_file->file,
		          filename_narrow,
		          LIBPFF_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS

		Py_DecRef(
		 utf8_string_object );
#endif
		if( result != 1 )
		{
			pypff_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to open file.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyBytes_Type );
#else
	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyString_Type );
#endif
	if( result == -1 )
	{
		pypff_error_fetch_and_raise(
		 PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type string.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
		                   string_object );
#else
		filename_narrow = PyString_AsString(
		                   string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libpff_file_open(
		          pypff_file->file,
		          filename_narrow,
		          LIBPFF_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pypff_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to open file.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type.",
	 function );

	return( NULL );
}

/* Opens a file using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_file_open_file_object(
           pypff_file_t *pypff_file,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *file_object       = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pypff_file_open_file_object";
	static char *keyword_list[] = { "file_object", "mode", NULL };
	char *mode                  = NULL;
	int result                  = 0;

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|s",
	     keyword_list,
	     &file_object,
	     &mode ) == 0 )
	{
		return( NULL );
	}
	if( ( mode != NULL )
	 && ( mode[ 0 ] != 'r' ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unsupported mode: %s.",
		 function,
		 mode );

		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_HasAttrString(
	          file_object,
	          "read" );

	if( result != 1 )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: unsupported file object - missing read attribute.",
		 function );

		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_HasAttrString(
	          file_object,
	          "seek" );

	if( result != 1 )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: unsupported file object - missing seek attribute.",
		 function );

		return( NULL );
	}
	if( pypff_file->file_io_handle != NULL )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: invalid file - file IO handle already set.",
		 function );

		goto on_error;
	}
	if( pypff_file_object_initialize(
	     &( pypff_file->file_io_handle ),
	     file_object,
	     &error ) != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize file IO handle.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_file_open_file_io_handle(
	          pypff_file->file,
	          pypff_file->file_io_handle,
	          LIBPFF_OPEN_READ,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to open file.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );

on_error:
	if( pypff_file->file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &( pypff_file->file_io_handle ),
		 NULL );
	}
	return( NULL );
}

/* Closes a file
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_file_close(
           pypff_file_t *pypff_file,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pypff_file_close";
	int result               = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_file_close(
	          pypff_file->file,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 0 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to close file.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( pypff_file->file_io_handle != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libbfio_handle_free(
		          &( pypff_file->file_io_handle ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pypff_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to free libbfio file IO handle.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Retrieves the size
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_file_get_size(
           pypff_file_t *pypff_file,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pypff_file_get_size";
	size64_t size            = 0;
	int result               = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_file_get_size(
	          pypff_file->file,
	          &size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: failed to retrieve size.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pypff_integer_unsigned_new_from_64bit(
	                  (uint64_t) size );

	return( integer_object );
}

/* Retrieves the content type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_file_get_content_type(
           pypff_file_t *pypff_file,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pypff_file_get_content_type";
	uint8_t content_type     = 0;
	int result               = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_file_get_content_type(
	          pypff_file->file,
	          &content_type,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve content type.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) content_type );
#else
	integer_object = PyInt_FromLong(
	                  (long) content_type );
#endif
	return( integer_object );
}

/* Retrieves the encryption type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_file_get_encryption_type(
           pypff_file_t *pypff_file,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pypff_file_get_encryption_type";
	uint8_t encryption_type  = 0;
	int result               = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_file_get_encryption_type(
	          pypff_file->file,
	          &encryption_type,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve encryption type.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) encryption_type );
#else
	integer_object = PyInt_FromLong(
	                  (long) encryption_type );
#endif
	return( integer_object );
}

/* Retrieves the codepage used for ASCII strings in the file
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_file_get_ascii_codepage(
           pypff_file_t *pypff_file,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *string_object     = NULL;
	libcerror_error_t *error    = NULL;
	const char *codepage_string = NULL;
	static char *function       = "pypff_file_get_ascii_codepage";
	int ascii_codepage          = 0;
	int result                  = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_file_get_ascii_codepage(
	          pypff_file->file,
	          &ascii_codepage,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve ASCII codepage.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	codepage_string = pypff_codepage_to_string(
	                   ascii_codepage );

	if( codepage_string == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unsupported ASCII codepage: %d.",
		 function,
		 ascii_codepage );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromString(
	                 codepage_string );
#else
	string_object = PyString_FromString(
	                 codepage_string );
#endif
	if( string_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to convert codepage string into string object.",
		 function );

		return( NULL );
	}
	return( string_object );
}

/* Sets the codepage used for ASCII strings in the file
 * Returns 1 if successful or -1 on error
 */
int pypff_file_set_ascii_codepage_from_string(
     pypff_file_t *pypff_file,
     const char *codepage_string )
{
	libcerror_error_t *error      = NULL;
	static char *function         = "pypff_file_set_ascii_codepage_from_string";
	size_t codepage_string_length = 0;
	uint32_t feature_flags        = 0;
	int ascii_codepage            = 0;
	int result                    = 0;

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( codepage_string == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid codepage string.",
		 function );

		return( -1 );
	}
	codepage_string_length = narrow_string_length(
	                          codepage_string );

	feature_flags = LIBCLOCALE_CODEPAGE_FEATURE_FLAG_HAVE_WINDOWS;

	if( libclocale_codepage_copy_from_string(
	     &ascii_codepage,
	     codepage_string,
	     codepage_string_length,
	     feature_flags,
	     &error ) != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_RuntimeError,
		 "%s: unable to determine ASCII codepage.",
		 function );

		libcerror_error_free(
		 &error );

		return( -1 );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_file_set_ascii_codepage(
	          pypff_file->file,
	          ascii_codepage,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to set ASCII codepage.",
		 function );

		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 1 );
}

/* Sets the codepage used for ASCII strings in the file
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_file_set_ascii_codepage(
           pypff_file_t *pypff_file,
           PyObject *arguments,
           PyObject *keywords )
{
	char *codepage_string       = NULL;
	static char *keyword_list[] = { "codepage", NULL };
	int result                  = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &codepage_string ) == 0 )
	{
		return( NULL );
	}
	result = pypff_file_set_ascii_codepage_from_string(
	          pypff_file,
	          codepage_string );

	if( result != 1 )
	{
		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Sets the codepage used for ASCII strings in the file
 * Returns a Python object if successful or NULL on error
 */
int pypff_file_set_ascii_codepage_setter(
     pypff_file_t *pypff_file,
     PyObject *string_object,
     void *closure PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *utf8_string_object = NULL;
	char *codepage_string        = NULL;
	static char *function        = "pypff_file_set_ascii_codepage_setter";
	int result                   = 0;

	PYPFF_UNREFERENCED_PARAMETER( closure )

	PyErr_Clear();

	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyUnicode_Type );

	if( result == -1 )
	{
		pypff_error_fetch_and_raise(
		 PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type unicode.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		/* The codepage string should only contain ASCII characters.
		 */
		utf8_string_object = PyUnicode_AsUTF8String(
		                      string_object );

		if( utf8_string_object == NULL )
		{
			pypff_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to convert unicode string to UTF-8.",
			 function );

			return( -1 );
		}
#if PY_MAJOR_VERSION >= 3
		codepage_string = PyBytes_AsString(
		                   utf8_string_object );
#else
		codepage_string = PyString_AsString(
		                   utf8_string_object );
#endif
		if( codepage_string == NULL )
		{
			return( -1 );
		}
		result = pypff_file_set_ascii_codepage_from_string(
		          pypff_file,
		          codepage_string );

		if( result != 1 )
		{
			return( -1 );
		}
		return( 0 );
	}
	PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyBytes_Type );
#else
	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyString_Type );
#endif
	if( result == -1 )
	{
		pypff_error_fetch_and_raise(
		 PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type string.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
#if PY_MAJOR_VERSION >= 3
		codepage_string = PyBytes_AsString(
		                   string_object );
#else
		codepage_string = PyString_AsString(
		                   string_object );
#endif
		if( codepage_string == NULL )
		{
			return( -1 );
		}
		result = pypff_file_set_ascii_codepage_from_string(
		          pypff_file,
		          codepage_string );

		if( result != 1 )
		{
			return( -1 );
		}
		return( 0 );
	}
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type.",
	 function );

	return( -1 );
}

/* Retrieves the item type object
 * Returns a Python type object if successful or NULL on error
 */
PyTypeObject *pypff_file_get_item_type_object(
               libpff_item_t *item )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pypff_file_get_item_type_object";
	uint8_t item_type        = 0;
	int result               = 0;

	if( item == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid item.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_item_get_type(
	          item,
	          &item_type,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve item type.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	switch( item_type )
	{
		case LIBPFF_ITEM_TYPE_ACTIVITY:
		case LIBPFF_ITEM_TYPE_APPOINTMENT:
		case LIBPFF_ITEM_TYPE_COMMON:
		case LIBPFF_ITEM_TYPE_CONFIGURATION:
		case LIBPFF_ITEM_TYPE_CONFLICT_MESSAGE:
		case LIBPFF_ITEM_TYPE_CONTACT:
		case LIBPFF_ITEM_TYPE_DISTRIBUTION_LIST:
		case LIBPFF_ITEM_TYPE_DOCUMENT:
		case LIBPFF_ITEM_TYPE_EMAIL:
		case LIBPFF_ITEM_TYPE_EMAIL_SMIME:
		case LIBPFF_ITEM_TYPE_FAX:
		case LIBPFF_ITEM_TYPE_MEETING:
		case LIBPFF_ITEM_TYPE_MMS:
		case LIBPFF_ITEM_TYPE_NOTE:
		case LIBPFF_ITEM_TYPE_POSTING_NOTE:
		case LIBPFF_ITEM_TYPE_RSS_FEED:
		case LIBPFF_ITEM_TYPE_SHARING:
		case LIBPFF_ITEM_TYPE_SMS:
		case LIBPFF_ITEM_TYPE_TASK:
		case LIBPFF_ITEM_TYPE_TASK_REQUEST:
		case LIBPFF_ITEM_TYPE_VOICEMAIL:
			return( &pypff_message_type_object );

		case LIBPFF_ITEM_TYPE_ATTACHMENT:
			return( &pypff_attachment_type_object );

		case LIBPFF_ITEM_TYPE_FOLDER:
			return( &pypff_folder_type_object );

		case LIBPFF_ITEM_TYPE_ATTACHMENTS:
		case LIBPFF_ITEM_TYPE_RECIPIENTS:
		case LIBPFF_ITEM_TYPE_SUB_ASSOCIATED_CONTENTS:
		case LIBPFF_ITEM_TYPE_SUB_FOLDERS:
		case LIBPFF_ITEM_TYPE_SUB_MESSAGES:
		default:
			break;
	}
	return( &pypff_item_type_object );
}

/* Retrieves the root item
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_file_get_root_item(
           pypff_file_t *pypff_file,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *item_object    = NULL;
	libcerror_error_t *error = NULL;
	libpff_item_t *root_item = NULL;
	static char *function    = "pypff_file_get_root_item";
	int result               = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_file_get_root_item(
	          pypff_file->file,
	          &root_item,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve root item.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Note that libpff_item_get_type will fail on the root item
	 */
	item_object = pypff_item_new(
	               &pypff_folder_type_object,
	               root_item,
	               (PyObject *) pypff_file );

	if( item_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create item object.",
		 function );

		goto on_error;
	}
	return( item_object );

on_error:
	if( root_item != NULL )
	{
		libpff_item_free(
		 &root_item,
		 NULL );
	}
	return( NULL );
}

/* Retrieves the message store
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_file_get_message_store(
           pypff_file_t *pypff_file,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *message_store_object = NULL;
	PyTypeObject *type_object      = NULL;
	libcerror_error_t *error       = NULL;
	libpff_item_t *message_store   = NULL;
	static char *function          = "pypff_file_get_message_store";
	int result                     = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_file_get_message_store(
	          pypff_file->file,
	          &message_store,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve message store.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	type_object = pypff_file_get_item_type_object(
	               message_store );

	if( type_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to retrieve message store type object.",
		 function );

		goto on_error;
	}
	message_store_object = pypff_item_new(
	                        type_object,
	                        message_store,
	                        (PyObject *) pypff_file );

	if( message_store_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create message store object.",
		 function );

		goto on_error;
	}
	return( message_store_object );

on_error:
	if( message_store != NULL )
	{
		libpff_item_free(
		 &message_store,
		 NULL );
	}
	return( NULL );
}

/* Retrieves the name to id map
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_file_get_name_to_id_map(
           pypff_file_t *pypff_file,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *name_to_id_map_object = NULL;
	PyTypeObject *type_object       = NULL;
	libcerror_error_t *error        = NULL;
	libpff_item_t *name_to_id_map   = NULL;
	static char *function           = "pypff_file_get_name_to_id_map";
	int result                      = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_file_get_name_to_id_map(
	          pypff_file->file,
	          &name_to_id_map,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve name to id map.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	type_object = pypff_file_get_item_type_object(
	               name_to_id_map );

	if( type_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to retrieve name to id map type object.",
		 function );

		goto on_error;
	}
	name_to_id_map_object = pypff_item_new(
	                         type_object,
	                         name_to_id_map,
	                         (PyObject *) pypff_file );

	if( name_to_id_map_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create name to id map object.",
		 function );

		goto on_error;
	}
	return( name_to_id_map_object );

on_error:
	if( name_to_id_map != NULL )
	{
		libpff_item_free(
		 &name_to_id_map,
		 NULL );
	}
	return( NULL );
}

/* Retrieves the root folder
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_file_get_root_folder(
           pypff_file_t *pypff_file,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *folder_object    = NULL;
	PyTypeObject *type_object  = NULL;
	libcerror_error_t *error   = NULL;
	libpff_item_t *root_folder = NULL;
	static char *function      = "pypff_file_get_root_folder";
	int result                 = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_file_get_root_folder(
	          pypff_file->file,
	          &root_folder,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve root folder.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	type_object = pypff_file_get_item_type_object(
	               root_folder );

	if( type_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to retrieve root folder type object.",
		 function );

		goto on_error;
	}
	folder_object = pypff_item_new(
	                 type_object,
	                 root_folder,
	                 (PyObject *) pypff_file );

	if( folder_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create folder object.",
		 function );

		goto on_error;
	}
	return( folder_object );

on_error:
	if( root_folder != NULL )
	{
		libpff_item_free(
		 &root_folder,
		 NULL );
	}
	return( NULL );
}

/* Retrieves the number of orphan items
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_file_get_number_of_orphan_items(
           pypff_file_t *pypff_file,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object   = NULL;
	libcerror_error_t *error   = NULL;
	static char *function      = "pypff_file_get_number_of_orphan_items";
	int number_of_orphan_items = 0;
	int result                 = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_file_get_number_of_orphan_items(
	          pypff_file->file,
	          &number_of_orphan_items,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of orphan items.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) number_of_orphan_items );
#else
	integer_object = PyInt_FromLong(
	                  (long) number_of_orphan_items );
#endif
	return( integer_object );
}

/* Retrieves a specific orphan item by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_file_get_orphan_item_by_index(
           PyObject *pypff_file,
           int orphan_item_index )
{
	PyObject *orphan_item_object = NULL;
	PyTypeObject *type_object    = NULL;
	libcerror_error_t *error     = NULL;
	libpff_item_t *orphan_item   = NULL;
	static char *function        = "pypff_file_get_orphan_item_by_index";
	int result                   = 0;

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_file_get_orphan_item_by_index(
	          ( (pypff_file_t *) pypff_file )->file,
	          orphan_item_index,
	          &orphan_item,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve orphan item: %d.",
		 function,
		 orphan_item_index );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	type_object = pypff_file_get_item_type_object(
	               orphan_item );

	if( type_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to retrieve item type object.",
		 function );

		goto on_error;
	}
	orphan_item_object = pypff_item_new(
	                      type_object,
	                      orphan_item,
	                      (PyObject *) pypff_file );

	if( orphan_item_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create item object.",
		 function );

		goto on_error;
	}
	return( orphan_item_object );

on_error:
	if( orphan_item != NULL )
	{
		libpff_item_free(
		 &orphan_item,
		 NULL );
	}
	return( NULL );
}

/* Retrieves a specific orphan item
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_file_get_orphan_item(
           pypff_file_t *pypff_file,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *orphan_item_object = NULL;
	static char *keyword_list[]  = { "orphan_item_index", NULL };
	int orphan_item_index        = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &orphan_item_index ) == 0 )
	{
		return( NULL );
	}
	orphan_item_object = pypff_file_get_orphan_item_by_index(
	                      (PyObject *) pypff_file,
	                      orphan_item_index );

	return( orphan_item_object );
}

/* Retrieves a sequence and iterator object for the orphan items
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_file_get_orphan_items(
           pypff_file_t *pypff_file,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *sequence_object  = NULL;
	libcerror_error_t *error   = NULL;
	static char *function      = "pypff_file_get_orphan_items";
	int number_of_orphan_items = 0;
	int result                 = 0;

	PYPFF_UNREFERENCED_PARAMETER( arguments )

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_file_get_number_of_orphan_items(
	          pypff_file->file,
	          &number_of_orphan_items,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of orphan items.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	sequence_object = pypff_items_new(
	                   (PyObject *) pypff_file,
	                   &pypff_file_get_orphan_item_by_index,
	                   number_of_orphan_items );

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

