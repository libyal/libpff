/*
 * Python object definition of the libpff item types
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

#include "pypff_item_types.h"
#include "pypff_libpff.h"
#include "pypff_python.h"
#include "pypff_unused.h"

PyTypeObject pypff_item_types_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pypff.item_types",
	/* tp_basicsize */
	sizeof( pypff_item_types_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pypff_item_types_free,
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
	"pypff item types object (wraps LIBPFF_ITEM_TYPES)",
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
	0,
	/* tp_members */
	0,
	/* tp_getset */
	0,
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
	(initproc) pypff_item_types_init,
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

/* Initializes the type object
 * Returns 1 if successful or -1 on error
 */
int pypff_item_types_init_type(
     PyTypeObject *type_object )
{
	PyObject *value_object = NULL;

	if( type_object == NULL )
	{
		return( -1 );
	}
	type_object->tp_dict = PyDict_New();

	if( type_object->tp_dict == NULL )
	{
		return( -1 );
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_UNDEFINED );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_UNDEFINED );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "UNDEFINED",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_ACTIVITY );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_ACTIVITY );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "ACTIVITY",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_APPOINTMENT );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_APPOINTMENT );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "APPOINTMENT",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_ATTACHMENT );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_ATTACHMENT );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "ATTACHMENT",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_ATTACHMENTS );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_ATTACHMENTS );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "ATTACHMENTS",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_COMMON );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_COMMON );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "COMMON",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_CONFIGURATION );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_CONFIGURATION );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "CONFIGURATION",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_CONFLICT_MESSAGE );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_CONFLICT_MESSAGE );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "CONFLICT_MESSAGE",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_CONTACT );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_CONTACT );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "CONTACT",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_DISTRIBUTION_LIST );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_DISTRIBUTION_LIST );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "DISTRIBUTION_LIST",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_DOCUMENT );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_DOCUMENT );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "DOCUMENT",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_EMAIL );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_EMAIL );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "EMAIL",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_EMAIL_SMIME );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_EMAIL_SMIME );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "EMAIL_SMIME",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_FAX );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_FAX );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "FAX",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_FOLDER );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_FOLDER );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "FOLDER",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_MEETING );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_MEETING );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "MEETING",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_MMS );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_MMS );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "MMS",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_NOTE );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_NOTE );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "NOTE",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_POSTING_NOTE );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_POSTING_NOTE );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "POSTING_NOTE",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_RECIPIENTS );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_RECIPIENTS );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "RECIPIENTS",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_RSS_FEED );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_RSS_FEED );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "RSS_FEED",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_SHARING );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_SHARING );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "SHARING",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_SMS );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_SMS );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "SMS",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_SUB_ASSOCIATED_CONTENTS );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_SUB_ASSOCIATED_CONTENTS );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "SUB_ASSOCIATED_CONTENTS",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_SUB_FOLDERS );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_SUB_FOLDERS );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "SUB_FOLDERS",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_SUB_MESSAGES );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_SUB_MESSAGES );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "SUB_MESSAGES",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_TASK );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_TASK );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "TASK",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_TASK_REQUEST );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_TASK_REQUEST );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "TASK_REQUEST",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_VOICEMAIL );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_VOICEMAIL );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "VOICEMAIL",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBPFF_ITEM_TYPE_UNKNOWN );
#else
	value_object = PyInt_FromLong(
	                LIBPFF_ITEM_TYPE_UNKNOWN );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "UNKNOWN",
	     value_object ) != 0 )
	{
		goto on_error;
	}
	return( 1 );

on_error:
	if( type_object->tp_dict != NULL )
	{
		Py_DecRef(
		 type_object->tp_dict );

		type_object->tp_dict = NULL;
	}
	return( -1 );
}

/* Creates a new item types object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_item_types_new(
           void )
{
	pypff_item_types_t *definitions_object = NULL;
	static char *function                  = "pypff_item_types_new";

	definitions_object = PyObject_New(
	                      struct pypff_item_types,
	                      &pypff_item_types_type_object );

	if( definitions_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create definitions object.",
		 function );

		goto on_error;
	}
	if( pypff_item_types_init(
	     definitions_object ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize definitions object.",
		 function );

		goto on_error;
	}
	return( (PyObject *) definitions_object );

on_error:
	if( definitions_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) definitions_object );
	}
	return( NULL );
}

/* Initializes an item types object
 * Returns 0 if successful or -1 on error
 */
int pypff_item_types_init(
     pypff_item_types_t *definitions_object )
{
	static char *function = "pypff_item_types_init";

	if( definitions_object == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid definitions object.",
		 function );

		return( -1 );
	}
	return( 0 );
}

/* Frees an item types object
 */
void pypff_item_types_free(
      pypff_item_types_t *definitions_object )
{
	struct _typeobject *ob_type = NULL;
	static char *function       = "pypff_item_types_free";

	if( definitions_object == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid definitions object.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           definitions_object );

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
	ob_type->tp_free(
	 (PyObject*) definitions_object );
}

