/*
 * Python bindings module for libpff (pypff)
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

#include "pypff.h"
#include "pypff_attachment.h"
#include "pypff_error.h"
#include "pypff_file.h"
#include "pypff_file_object_io_handle.h"
#include "pypff_folder.h"
#include "pypff_item.h"
#include "pypff_items.h"
#include "pypff_libbfio.h"
#include "pypff_libcerror.h"
#include "pypff_libpff.h"
#include "pypff_message.h"
#include "pypff_python.h"
#include "pypff_record_entries.h"
#include "pypff_record_entry.h"
#include "pypff_record_set.h"
#include "pypff_record_sets.h"
#include "pypff_unused.h"

#if !defined( LIBPFF_HAVE_BFIO )

LIBPFF_EXTERN \
int libpff_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libpff_error_t **error );

#endif /* !defined( LIBPFF_HAVE_BFIO ) */

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
	  "Checks if a file has a Personal Folder File (PFF) signature." },

	{ "check_file_signature_file_object",
	  (PyCFunction) pypff_check_file_signature_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "check_file_signature_file_object(file_object) -> Boolean\n"
	  "\n"
	  "Checks if a file has a Personal Folder File (PFF) signature using a file-like object." },

	{ "open",
	  (PyCFunction) pypff_open_new_file,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filename, mode='r') -> Object\n"
	  "\n"
	  "Opens a file." },

	{ "open_file_object",
	  (PyCFunction) pypff_open_new_file_with_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "open_file_object(file_object, mode='r') -> Object\n"
	  "\n"
	  "Opens a file using a file-like object." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

/* Retrieves the pypff/libpff version
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_get_version(
           PyObject *self PYPFF_ATTRIBUTE_UNUSED,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	const char *version_string   = NULL;
	size_t version_string_length = 0;

	PYPFF_UNREFERENCED_PARAMETER( self )
	PYPFF_UNREFERENCED_PARAMETER( arguments )

	Py_BEGIN_ALLOW_THREADS

	version_string = libpff_get_version();

	Py_END_ALLOW_THREADS

	version_string_length = narrow_string_length(
	                         version_string );

	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	return( PyUnicode_DecodeUTF8(
	         version_string,
	         (Py_ssize_t) version_string_length,
	         NULL ) );
}

/* Checks if a file has a Personal Folder File (PFF) signature
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_check_file_signature(
           PyObject *self PYPFF_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *string_object     = NULL;
	libcerror_error_t *error    = NULL;
	const char *filename_narrow = NULL;
	static char *function       = "pypff_check_file_signature";
	static char *keyword_list[] = { "filename", NULL };
	int result                  = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	const wchar_t *filename_wide = NULL;
#else
	PyObject *utf8_string_object = NULL;
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
	     "O|",
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
		pypff_error_fetch_and_raise(
		 PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type Unicode.",
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

		result = libpff_check_file_signature_wide(
		          filename_wide,
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
			 "%s: unable to convert Unicode string to UTF-8.",
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

		result = libpff_check_file_signature(
		          filename_narrow,
		          &error );

		Py_END_ALLOW_THREADS

		Py_DecRef(
		 utf8_string_object );

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

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

/* Checks if a file has a Personal Folder File (PFF) signature using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_check_file_signature_file_object(
           PyObject *self PYPFF_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *file_object            = NULL;
	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	static char *function            = "pypff_check_file_signature_file_object";
	static char *keyword_list[]      = { "file_object", NULL };
	int result                       = 0;

	PYPFF_UNREFERENCED_PARAMETER( self )

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|",
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

/* Creates a new file object and opens it
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_open_new_file(
           PyObject *self PYPFF_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	pypff_file_t *pypff_file = NULL;
	static char *function    = "pypff_open_new_file";

	PYPFF_UNREFERENCED_PARAMETER( self )

	/* PyObject_New does not invoke tp_init
	 */
	pypff_file = PyObject_New(
	              struct pypff_file,
	              &pypff_file_type_object );

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create file.",
		 function );

		goto on_error;
	}
	if( pypff_file_init(
	     pypff_file ) != 0 )
	{
		goto on_error;
	}
	if( pypff_file_open(
	     pypff_file,
	     arguments,
	     keywords ) == NULL )
	{
		goto on_error;
	}
	return( (PyObject *) pypff_file );

on_error:
	if( pypff_file != NULL )
	{
		Py_DecRef(
		 (PyObject *) pypff_file );
	}
	return( NULL );
}

/* Creates a new file object and opens it using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_open_new_file_with_file_object(
           PyObject *self PYPFF_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	pypff_file_t *pypff_file = NULL;
	static char *function    = "pypff_open_new_file_with_file_object";

	PYPFF_UNREFERENCED_PARAMETER( self )

	/* PyObject_New does not invoke tp_init
	 */
	pypff_file = PyObject_New(
	              struct pypff_file,
	              &pypff_file_type_object );

	if( pypff_file == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create file.",
		 function );

		goto on_error;
	}
	if( pypff_file_init(
	     pypff_file ) != 0 )
	{
		goto on_error;
	}
	if( pypff_file_open_file_object(
	     pypff_file,
	     arguments,
	     keywords ) == NULL )
	{
		goto on_error;
	}
	return( (PyObject *) pypff_file );

on_error:
	if( pypff_file != NULL )
	{
		Py_DecRef(
		 (PyObject *) pypff_file );
	}
	return( NULL );
}

#if PY_MAJOR_VERSION >= 3

/* The pypff module definition
 */
PyModuleDef pypff_module_definition = {
	PyModuleDef_HEAD_INIT,

	/* m_name */
	"pypff",
	/* m_doc */
	"Python libpff module (pypff).",
	/* m_size */
	-1,
	/* m_methods */
	pypff_module_methods,
	/* m_reload */
	NULL,
	/* m_traverse */
	NULL,
	/* m_clear */
	NULL,
	/* m_free */
	NULL,
};

#endif /* PY_MAJOR_VERSION >= 3 */

/* Initializes the pypff module
 */
#if PY_MAJOR_VERSION >= 3
PyMODINIT_FUNC PyInit_pypff(
                void )
#else
PyMODINIT_FUNC initpypff(
                void )
#endif
{
	PyObject *module           = NULL;
	PyGILState_STATE gil_state = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libpff_notify_set_stream(
	 stderr,
	 NULL );
	libpff_notify_set_verbose(
	 1 );
#endif

	/* Create the module
	 * This function must be called before grabbing the GIL
	 * otherwise the module will segfault on a version mismatch
	 */
#if PY_MAJOR_VERSION >= 3
	module = PyModule_Create(
	          &pypff_module_definition );
#else
	module = Py_InitModule3(
	          "pypff",
	          pypff_module_methods,
	          "Python libpff module (pypff)." );
#endif
	if( module == NULL )
	{
#if PY_MAJOR_VERSION >= 3
		return( NULL );
#else
		return;
#endif
	}
#if PY_VERSION_HEX < 0x03070000
	PyEval_InitThreads();
#endif
	gil_state = PyGILState_Ensure();

	/* Setup the attachment type object
	 */
	pypff_attachment_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pypff_attachment_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pypff_attachment_type_object );

	PyModule_AddObject(
	 module,
	 "attachment",
	 (PyObject *) &pypff_attachment_type_object );

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

	PyModule_AddObject(
	 module,
	 "file",
	 (PyObject *) &pypff_file_type_object );

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

	PyModule_AddObject(
	 module,
	 "folder",
	 (PyObject *) &pypff_folder_type_object );

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

	PyModule_AddObject(
	 module,
	 "item",
	 (PyObject *) &pypff_item_type_object );

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

	PyModule_AddObject(
	 module,
	 "items",
	 (PyObject *) &pypff_items_type_object );

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

	PyModule_AddObject(
	 module,
	 "message",
	 (PyObject *) &pypff_message_type_object );

	/* Setup the record_entries type object
	 */
	pypff_record_entries_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pypff_record_entries_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pypff_record_entries_type_object );

	PyModule_AddObject(
	 module,
	 "record_entries",
	 (PyObject *) &pypff_record_entries_type_object );

	/* Setup the record_entry type object
	 */
	pypff_record_entry_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pypff_record_entry_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pypff_record_entry_type_object );

	PyModule_AddObject(
	 module,
	 "record_entry",
	 (PyObject *) &pypff_record_entry_type_object );

	/* Setup the record_set type object
	 */
	pypff_record_set_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pypff_record_set_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pypff_record_set_type_object );

	PyModule_AddObject(
	 module,
	 "record_set",
	 (PyObject *) &pypff_record_set_type_object );

	/* Setup the record_sets type object
	 */
	pypff_record_sets_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pypff_record_sets_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pypff_record_sets_type_object );

	PyModule_AddObject(
	 module,
	 "record_sets",
	 (PyObject *) &pypff_record_sets_type_object );

	PyGILState_Release(
	 gil_state );

#if PY_MAJOR_VERSION >= 3
	return( module );
#else
	return;
#endif

on_error:
	PyGILState_Release(
	 gil_state );

#if PY_MAJOR_VERSION >= 3
	return( NULL );
#else
	return;
#endif
}

