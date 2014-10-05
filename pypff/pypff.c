/*
 * Python bindings module for libpff (pypff)
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

#include "pypff.h"
#include "pypff_error.h"
#include "pypff_file.h"
#include "pypff_file_object_io_handle.h"
#include "pypff_folder.h"
#include "pypff_item.h"
#include "pypff_items.h"
#include "pypff_libcerror.h"
#include "pypff_libcstring.h"
#include "pypff_libpff.h"
#include "pypff_message.h"
#include "pypff_record_entry.h"
#include "pypff_record_entries.h"
#include "pypff_record_set.h"
#include "pypff_record_sets.h"
#include "pypff_python.h"
#include "pypff_unused.h"

#if !defined( LIBPFF_HAVE_BFIO )
LIBPFF_EXTERN \
int libpff_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libpff_error_t **error );
#endif

/* The pypff module methods
 */
PyMethodDef pypff_module_methods[] = {
	{ "get_version",
	  (PyCFunction) pypff_get_version,
	  METH_NOARGS,
	  "get_version() -> String\n"
	  "\n"
	  "Retrieves the version." },

	{ "check_file_signature",
	  (PyCFunction) pypff_check_file_signature,
	  METH_VARARGS | METH_KEYWORDS,
	  "check_file_signature(filename) -> Boolean\n"
	  "\n"
	  "Checks if a file has a Personal Folder Format (PFF) signature." },

	{ "check_file_signature_file_object",
	  (PyCFunction) pypff_check_file_signature_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "check_file_signature_file_object(file_object) -> Boolean\n"
	  "\n"
	  "Checks if a file has a Personal Folder Format (PFF) signature using a file-like object." },

	{ "open",
	  (PyCFunction) pypff_file_new_open,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filename, mode='r') -> Object\n"
	  "\n"
	  "Opens a file." },

	{ "open_file_object",
	  (PyCFunction) pypff_file_new_open_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "open_file_object(file_object, mode='r') -> Object\n"
	  "\n"
	  "Opens a file using a file-like object." },

	/* Sentinel */
	{ NULL,
	  NULL,
	  0,
	  NULL}
};

/* Retrieves the pypff/libpff version
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_get_version(
           PyObject *self PYPFF_ATTRIBUTE_UNUSED,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	const char *errors           = NULL;
	const char *version_string   = NULL;
	size_t version_string_length = 0;

	PYPFF_UNREFERENCED_PARAMETER( self )
	PYPFF_UNREFERENCED_PARAMETER( arguments )

	Py_BEGIN_ALLOW_THREADS

	version_string = libpff_get_version();

	Py_END_ALLOW_THREADS

	version_string_length = libcstring_narrow_string_length(
	                         version_string );

	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	return( PyUnicode_DecodeUTF8(
	         version_string,
	         (Py_ssize_t) version_string_length,
	         errors ) );
}

/* Checks if the file has a Personal Folder File (PFF) signature
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_check_file_signature(
           PyObject *self PYPFF_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *exception_string    = NULL;
	PyObject *exception_traceback = NULL;
	PyObject *exception_type      = NULL;
	PyObject *exception_value     = NULL;
	PyObject *string_object       = NULL;
	libcerror_error_t *error      = NULL;
	static char *function         = "pypff_check_file_signature";
	static char *keyword_list[]   = { "filename", NULL };
	const char *filename_narrow   = NULL;
	char *error_string            = NULL;
	int result                    = 0;

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	const wchar_t *filename_wide  = NULL;
#else
	PyObject *utf8_string_object  = NULL;
#endif

	PYPFF_UNREFERENCED_PARAMETER( self )

	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. This will also fail if the default encoding is not set correctly. We cannot use "u" here either since that
	 * does not allow us to pass non Unicode string objects and Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|O",
	     keyword_list,
	     &string_object ) == 0 )
	{
		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyUnicode_Type );

	if( result == -1 )
	{
		PyErr_Fetch(
		 &exception_type,
		 &exception_value,
		 &exception_traceback );

		exception_string = PyObject_Repr(
		                    exception_value );

		error_string = PyString_AsString(
		                exception_string );

		if( error_string != NULL )
		{
			PyErr_Format(
		         PyExc_RuntimeError,
			 "%s: unable to determine if string object is of type unicode with error: %s.",
			 function,
			 error_string );
		}
		else
		{
			PyErr_Format(
		         PyExc_RuntimeError,
			 "%s: unable to determine if string object is of type unicode.",
			 function );
		}
		Py_DecRef(
		 exception_string );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		filename_wide = (wchar_t *) PyUnicode_AsUnicode(
		                             string_object );
		Py_BEGIN_ALLOW_THREADS

		result = libpff_check_file_signature_wide(
		          filename_wide,
		          &error );

		Py_END_ALLOW_THREADS
#else
		utf8_string_object = PyUnicode_AsUTF8String(
		                      string_object );

		if( utf8_string_object == NULL )
		{
			PyErr_Fetch(
			 &exception_type,
			 &exception_value,
			 &exception_traceback );

			exception_string = PyObject_Repr(
					    exception_value );

			error_string = PyString_AsString(
					exception_string );

			if( error_string != NULL )
			{
				PyErr_Format(
				 PyExc_RuntimeError,
				 "%s: unable to convert unicode string to UTF-8 with error: %s.",
				 function,
				 error_string );
			}
			else
			{
				PyErr_Format(
				 PyExc_RuntimeError,
				 "%s: unable to convert unicode string to UTF-8.",
				 function );
			}
			Py_DecRef(
			 exception_string );

			return( NULL );
		}
		filename_narrow = PyString_AsString(
				   utf8_string_object );

		Py_BEGIN_ALLOW_THREADS

		result = libpff_check_file_signature(
		          filename_narrow,
		          &error );

		Py_END_ALLOW_THREADS

		Py_DecRef(
		 utf8_string_object );
#endif
		if( result == -1 )
		{
			pypff_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to check file signature.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		if( result != 0 )
		{
			Py_IncRef(
			 (PyObject *) Py_True );

			return( Py_True );
		}
		Py_IncRef(
		 (PyObject *) Py_False );

		return( Py_False );
	}
	PyErr_Clear();

	result = PyObject_IsInstance(
		  string_object,
		  (PyObject *) &PyString_Type );

	if( result == -1 )
	{
		PyErr_Fetch(
		 &exception_type,
		 &exception_value,
		 &exception_traceback );

		exception_string = PyObject_Repr(
				    exception_value );

		error_string = PyString_AsString(
				exception_string );

		if( error_string != NULL )
		{
			PyErr_Format(
		         PyExc_RuntimeError,
			 "%s: unable to determine if string object is of type string with error: %s.",
			 function,
			 error_string );
		}
		else
		{
			PyErr_Format(
		         PyExc_RuntimeError,
			 "%s: unable to determine if string object is of type string.",
			 function );
		}
		Py_DecRef(
		 exception_string );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

		filename_narrow = PyString_AsString(
				   string_object );

		Py_BEGIN_ALLOW_THREADS

		result = libpff_check_file_signature(
		          filename_narrow,
		          &error );

		Py_END_ALLOW_THREADS

		if( result == -1 )
		{
			pypff_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to check file signature.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		if( result != 0 )
		{
			Py_IncRef(
			 (PyObject *) Py_True );

			return( Py_True );
		}
		Py_IncRef(
		 (PyObject *) Py_False );

		return( Py_False );
	}
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type.",
	 function );

	return( NULL );
}

/* Checks if the file has a Personal Folder File (PFF) file signature using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_check_file_signature_file_object(
           PyObject *self PYPFF_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error         = NULL;
	libbfio_handle_t *file_io_handle = NULL;
	PyObject *file_object            = NULL;
	static char *function            = "pypff_check_file_signature_file_object";
	static char *keyword_list[]      = { "file_object", NULL };
	int result                       = 0;

	PYPFF_UNREFERENCED_PARAMETER( self )

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|O",
	     keyword_list,
	     &file_object ) == 0 )
	{
		return( NULL );
	}
	if( pypff_file_object_initialize(
	     &file_io_handle,
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

	result = libpff_check_file_signature_file_io_handle(
	          file_io_handle,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to check file signature.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	if( libbfio_handle_free(
	     &file_io_handle,
	     &error ) != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to free file IO handle.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	if( result != 0 )
	{
		Py_IncRef(
		 (PyObject *) Py_True );

		return( Py_True );
	}
	Py_IncRef(
	 (PyObject *) Py_False );

	return( Py_False );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( NULL );
}

/* Declarations for DLL import/export
 */
#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif

/* Initializes the pypff module
 */
PyMODINIT_FUNC initpypff(
                void )
{
	PyObject *module                         = NULL;
	PyTypeObject *file_type_object           = NULL;
	PyTypeObject *folder_type_object         = NULL;
	PyTypeObject *item_type_object           = NULL;
	PyTypeObject *items_type_object          = NULL;
	PyTypeObject *message_type_object        = NULL;
	PyTypeObject *record_entry_type_object   = NULL;
	PyTypeObject *record_entries_type_object = NULL;
	PyTypeObject *record_set_type_object     = NULL;
	PyTypeObject *record_sets_type_object    = NULL;
	PyGILState_STATE gil_state               = 0;

	/* Create the module
	 * This function must be called before grabbing the GIL
	 * otherwise the module will segfault on a version mismatch
	 */
	module = Py_InitModule3(
	          "pypff",
	          pypff_module_methods,
	          "Python libpff module (pypff)." );

	PyEval_InitThreads();

	gil_state = PyGILState_Ensure();

	/* Setup the file type object
	 */
	pypff_file_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pypff_file_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pypff_file_type_object );

	file_type_object = &pypff_file_type_object;

	PyModule_AddObject(
	 module,
	 "file",
	 (PyObject *) file_type_object );

	/* Setup the folder type object
	 */
	pypff_folder_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pypff_folder_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pypff_folder_type_object );

	folder_type_object = &pypff_folder_type_object;

	PyModule_AddObject(
	 module,
	 "folder",
	 (PyObject *) folder_type_object );

	/* Setup the item type object
	 */
	pypff_item_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pypff_item_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pypff_item_type_object );

	item_type_object = &pypff_item_type_object;

	PyModule_AddObject(
	 module,
	 "item",
	 (PyObject *) item_type_object );

	/* Setup the items type object
	 */
	pypff_items_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pypff_items_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pypff_items_type_object );

	items_type_object = &pypff_items_type_object;

	PyModule_AddObject(
	 module,
	"_items",
	(PyObject *) items_type_object );

	/* Setup the message type object
	 */
	pypff_message_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pypff_message_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pypff_message_type_object );

	message_type_object = &pypff_message_type_object;

	PyModule_AddObject(
	 module,
	 "message",
	 (PyObject *) message_type_object );

	/* Setup the record entry type object
	 */
	pypff_record_entry_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pypff_record_entry_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pypff_record_entry_type_object );

	record_entry_type_object = &pypff_record_entry_type_object;

	PyModule_AddObject(
	 module,
	 "record_entry",
	 (PyObject *) record_entry_type_object );

	/* Setup the record entries type object
	 */
	pypff_record_entries_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pypff_record_entries_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pypff_record_entries_type_object );

	record_entries_type_object = &pypff_record_entries_type_object;

	PyModule_AddObject(
	 module,
	"_record_entries",
	(PyObject *) record_entries_type_object );

	/* Setup the record set type object
	 */
	pypff_record_set_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pypff_record_set_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pypff_record_set_type_object );

	record_set_type_object = &pypff_record_set_type_object;

	PyModule_AddObject(
	 module,
	 "record_set",
	 (PyObject *) record_set_type_object );

	/* Setup the record sets type object
	 */
	pypff_record_sets_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pypff_record_sets_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pypff_record_sets_type_object );

	record_sets_type_object = &pypff_record_sets_type_object;

	PyModule_AddObject(
	 module,
	"_record_sets",
	(PyObject *) record_sets_type_object );

on_error:
	PyGILState_Release(
	 gil_state );
}

