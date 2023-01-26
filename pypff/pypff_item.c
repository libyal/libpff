/*
 * Python object wrapper of libpff_item_t
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
#include "pypff_record_set.h"
#include "pypff_record_sets.h"
#include "pypff_unused.h"

PyMethodDef pypff_item_object_methods[] = {

	{ "get_identifier",
	  (PyCFunction) pypff_item_get_identifier,
	  METH_NOARGS,
	  "get_identifier() -> Integer or None\n"
	  "\n"
	  "Retrieves the identifier." },

	{ "get_number_of_record_sets",
	  (PyCFunction) pypff_item_get_number_of_record_sets,
	  METH_NOARGS,
	  "get_number_of_record_sets() -> Integer or None\n"
	  "\n"
	  "Retrieves the number of record sets." },

	{ "get_record_set",
	  (PyCFunction) pypff_item_get_record_set,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_record_set(record_set_index) -> Object or None\n"
	  "\n"
	  "Retrieves the record set specified by the index." },

	{ "get_number_of_entries",
	  (PyCFunction) pypff_item_get_number_of_entries,
	  METH_NOARGS,
	  "get_number_of_entries() -> Integer or None\n"
	  "\n"
	  "Retrieves the number of entries." },

	{ "get_number_of_sub_items",
	  (PyCFunction) pypff_item_get_number_of_sub_items,
	  METH_NOARGS,
	  "get_number_of_sub_items() -> Integer or None\n"
	  "\n"
	  "Retrieves the number of sub items." },

	{ "get_sub_item",
	  (PyCFunction) pypff_item_get_sub_item,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_sub_item(sub_item_index) -> Object or None\n"
	  "\n"
	  "Retrieves the sub item specified by the index." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pypff_item_object_get_set_definitions[] = {

	{ "identifier",
	  (getter) pypff_item_get_identifier,
	  (setter) 0,
	  "The identifier.",
	  NULL },

	{ "number_of_record_sets",
	  (getter) pypff_item_get_number_of_record_sets,
	  (setter) 0,
	  "The number of record sets.",
	  NULL },

	{ "record_sets",
	  (getter) pypff_item_get_record_sets,
	  (setter) 0,
	  "The record sets.",
	  NULL },

	{ "number_of_entries",
	  (getter) pypff_item_get_number_of_entries,
	  (setter) 0,
	  "The number of entries.",
	  NULL },

	{ "number_of_sub_items",
	  (getter) pypff_item_get_number_of_sub_items,
	  (setter) 0,
	  "The number of sub items.",
	  NULL },

	{ "sub_items",
	  (getter) pypff_item_get_sub_items,
	  (setter) 0,
	  "The sub items.",
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
           PyObject *parent_object )
{
	pypff_item_t *pypff_item = NULL;
	static char *function    = "pypff_item_new";

	if( item == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
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
	pypff_item->item          = item;
	pypff_item->parent_object = parent_object;

	Py_IncRef(
	 (PyObject *) pypff_item->parent_object );

	return( (PyObject *) pypff_item );

on_error:
	if( pypff_item != NULL )
	{
		Py_DecRef(
		 (PyObject *) pypff_item );
	}
	return( NULL );
}

/* Initializes an item object
 * Returns 0 if successful or -1 on error
 */
int pypff_item_init(
     pypff_item_t *pypff_item )
{
	static char *function = "pypff_item_init";

	if( pypff_item == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
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
	struct _typeobject *ob_type = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pypff_item_free";
	int result                  = 0;

	if( pypff_item == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid item.",
		 function );

		return;
	}
	if( pypff_item->item == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
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
	Py_BEGIN_ALLOW_THREADS

	result = libpff_item_free(
	          &( pypff_item->item ),
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to free libpff item.",
		 function );

		libcerror_error_free(
		 &error );
	}
	if( pypff_item->parent_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) pypff_item->parent_object );
	}
	ob_type->tp_free(
	 (PyObject*) pypff_item );
}

/* Retrieves the identifier
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_item_get_identifier(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pypff_item_get_identifier";
	uint32_t value_32bit     = 0;
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

	result = libpff_item_get_identifier(
	          pypff_item->item,
	          &value_32bit,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve identifier.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	integer_object = PyLong_FromUnsignedLong(
	                  (unsigned long) value_32bit );

	return( integer_object );
}

/* Retrieves the number of record sets
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_item_get_number_of_record_sets(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object  = NULL;
	libcerror_error_t *error  = NULL;
	static char *function     = "pypff_item_get_number_of_record_sets";
	int number_of_record_sets = 0;
	int result                = 0;

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
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) number_of_record_sets );
#else
	integer_object = PyInt_FromLong(
	                  (long) number_of_record_sets );
#endif
	return( integer_object );
}

/* Retrieves the record set type object
 * Returns a Python type object if successful or NULL on error
 */
PyTypeObject *pypff_item_get_record_set_type_object(
               libpff_record_set_t *record_set PYPFF_ATTRIBUTE_UNUSED )
{
	PYPFF_UNREFERENCED_PARAMETER( record_set )

	return( &pypff_record_set_type_object );
}

/* Retrieves a specific record set by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_item_get_record_set_by_index(
           PyObject *pypff_item,
           int record_set_index )
{
	PyObject *record_set_object     = NULL;
	PyTypeObject *type_object       = NULL;
	libcerror_error_t *error        = NULL;
	libpff_record_set_t *record_set = NULL;
	static char *function           = "pypff_item_get_record_set_by_index";
	int result                      = 0;

	if( pypff_item == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid item.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_item_get_record_set_by_index(
	          ( (pypff_item_t *) pypff_item )->item,
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
	type_object = pypff_item_get_record_set_type_object(
	               record_set );

	if( type_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to retrieve record set type object.",
		 function );

		goto on_error;
	}
	record_set_object = pypff_record_set_new(
	                     type_object,
	                     record_set,
	                     (PyObject *) pypff_item );

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
		libpff_record_set_free(
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
	                     (PyObject *) pypff_item,
	                     record_set_index );

	return( record_set_object );
}

/* Retrieves a sequence and iterator object for the record sets
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_item_get_record_sets(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *sequence_object = NULL;
	libcerror_error_t *error  = NULL;
	static char *function     = "pypff_item_get_record_sets";
	int number_of_record_sets = 0;
	int result                = 0;

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
	sequence_object = pypff_record_sets_new(
	                   (PyObject *) pypff_item,
	                   &pypff_item_get_record_set_by_index,
	                   number_of_record_sets );

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

/* Retrieves the number of entries
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_item_get_number_of_entries(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pypff_item_get_number_of_entries";
	uint32_t value_32bit     = 0;
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

	result = libpff_item_get_number_of_entries(
	          pypff_item->item,
	          &value_32bit,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of entries.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	integer_object = PyLong_FromUnsignedLong(
	                  (unsigned long) value_32bit );

	return( integer_object );
}

/* Retrieves the number of sub items
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_item_get_number_of_sub_items(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pypff_item_get_number_of_sub_items";
	int number_of_sub_items  = 0;
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

/* Retrieves the item type object
 * Returns a Python type object if successful or NULL on error
 */
PyTypeObject *pypff_item_get_item_type_object(
               libpff_item_t *item )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pypff_item_get_sub_item_by_index";
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

		case LIBPFF_ITEM_TYPE_FOLDER:
			return( &pypff_folder_type_object );

		case LIBPFF_ITEM_TYPE_ATTACHMENT:
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

/* Retrieves a specific sub item by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_item_get_sub_item_by_index(
           PyObject *pypff_item,
           int sub_item_index )
{
	PyObject *item_object     = NULL;
	PyTypeObject *type_object = NULL;
	libcerror_error_t *error  = NULL;
	libpff_item_t *sub_item   = NULL;
	static char *function     = "pypff_item_get_sub_item_by_index";
	int result                = 0;

	if( pypff_item == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid item.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_item_get_sub_item(
	          ( (pypff_item_t *) pypff_item )->item,
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
	type_object = pypff_item_get_item_type_object(
	               sub_item );

	if( type_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to retrieve item type object.",
		 function );

		goto on_error;
	}
	item_object = pypff_item_new(
	               type_object,
	               sub_item,
	               ( (pypff_item_t *) pypff_item )->parent_object );

	if( item_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create sub item object.",
		 function );

		goto on_error;
	}
	return( item_object );

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
	PyObject *item_object       = NULL;
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
	item_object = pypff_item_get_sub_item_by_index(
	               (PyObject *) pypff_item,
	               sub_item_index );

	return( item_object );
}

/* Retrieves a sequence and iterator object for the sub items
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_item_get_sub_items(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *sequence_object = NULL;
	libcerror_error_t *error  = NULL;
	static char *function     = "pypff_item_get_sub_items";
	int number_of_sub_items   = 0;
	int result                = 0;

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
	sequence_object = pypff_items_new(
	                   (PyObject *) pypff_item,
	                   &pypff_item_get_sub_item_by_index,
	                   number_of_sub_items );

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

