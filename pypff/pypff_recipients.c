/*
 * Python object wrapper of libpff_item_t type LIBPFF_ITEM_TYPE_RECIPIENTS
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

#include "pypff_error.h"
#include "pypff_item.h"
#include "pypff_libcerror.h"
#include "pypff_libpff.h"
#include "pypff_python.h"
#include "pypff_recipients.h"
#include "pypff_unused.h"

PyMethodDef pypff_recipients_object_methods[] = {

	/* Functions to access the recipients values */
	{ "get_number_of_recipients",
	  (PyCFunction) pypff_item_get_number_of_record_sets,
	  METH_NOARGS,
	  "get_number_of_recipients() -> Integer or None\n"
	  "\n"
	  "Retrieves the number of recipients." },

	{ "get_recipient",
	  (PyCFunction) pypff_recipients_get_recipient,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_recipient(recipient_index) -> Object or None\n"
	  "\n"
	  "Retrieves the recipient specified by the index." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pypff_recipients_object_get_set_definitions[] = {

	{ "number_of_recipients",
	  (getter) pypff_item_get_number_of_record_sets,
	  (setter) 0,
	  "The number of recipients.",
	  NULL },

	{ "recipients",
	  (getter) pypff_item_get_record_sets,
	  (setter) 0,
	  "The recipients.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pypff_recipients_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pypff.recipients",
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
	"pypff recipients object (wraps recipients type libpff_item_t)",
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
	pypff_recipients_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pypff_recipients_object_get_set_definitions,
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

/* Retrieves a specific recipient by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_recipients_get_recipient(
           pypff_item_t *pypff_recipients,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *recipient_object   = NULL;
	static char *function        = "pypff_recipients_get_recipient";
	static char *keyword_list[]  = { "recipient_index", NULL };
	int recipient_index          = 0;

	if( pypff_recipients == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid recipients.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &recipient_index ) == 0 )
	{
		return( NULL );
	}
	recipient_object = pypff_item_get_record_set_by_index(
	                    (PyObject *) pypff_recipients,
	                    recipient_index );

	return( recipient_object );
}

