/*
 * Python object definition of the message type libpff item
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

#include "pypff_attachment.h"
#include "pypff_datetime.h"
#include "pypff_error.h"
#include "pypff_integer.h"
#include "pypff_item.h"
#include "pypff_items.h"
#include "pypff_libcerror.h"
#include "pypff_libpff.h"
#include "pypff_message.h"
#include "pypff_python.h"
#include "pypff_unused.h"

PyMethodDef pypff_message_object_methods[] = {

	/* Functions to access the message values */

	{ "get_subject",
	  (PyCFunction) pypff_message_get_subject,
	  METH_NOARGS,
	  "get_subject() -> Unicode string or -None\n"
	  "\n"
	  "Retrieves the subject." },

	{ "get_conversation_topic",
	  (PyCFunction) pypff_message_get_conversation_topic,
	  METH_NOARGS,
	  "get_conversation_topic() -> Unicode string or None\n"
	  "\n"
	  "Retrieves the conversation topic." },

	{ "get_sender_name",
	  (PyCFunction) pypff_message_get_sender_name,
	  METH_NOARGS,
	  "get_sender_name() -> Unicode string or None\n"
	  "\n"
	  "Retrieves the sender name." },

	{ "get_client_submit_time",
	  (PyCFunction) pypff_message_get_client_submit_time,
	  METH_NOARGS,
	  "get_client_submit_time() -> Datetime or None\n"
	  "\n"
	  "Returns the client submit date and time." },

	{ "get_client_submit_time_as_integer",
	  (PyCFunction) pypff_message_get_client_submit_time_as_integer,
	  METH_NOARGS,
	  "get_client_submit_time_as_integer() -> Integer or None\n"
	  "\n"
	  "Returns the client submit date and time as a 64-bit integer containing a FILETIME value." },

	{ "get_delivery_time",
	  (PyCFunction) pypff_message_get_delivery_time,
	  METH_NOARGS,
	  "get_delivery_time() -> Datetime or None\n"
	  "\n"
	  "Returns the delivery date and time." },

	{ "get_delivery_time_as_integer",
	  (PyCFunction) pypff_message_get_delivery_time_as_integer,
	  METH_NOARGS,
	  "get_delivery_time_as_integer() -> Integer or None\n"
	  "\n"
	  "Returns the delivery date and time as a 64-bit integer containing a FILETIME value." },

	{ "get_creation_time",
	  (PyCFunction) pypff_message_get_creation_time,
	  METH_NOARGS,
	  "get_creation_time() -> Datetime or None\n"
	  "\n"
	  "Returns the creation date and time." },

	{ "get_creation_time_as_integer",
	  (PyCFunction) pypff_message_get_creation_time_as_integer,
	  METH_NOARGS,
	  "get_creation_time_as_integer() -> Integer or None\n"
	  "\n"
	  "Returns the creation date and time as a 64-bit integer containing a FILETIME value." },

	{ "get_modification_time",
	  (PyCFunction) pypff_message_get_modification_time,
	  METH_NOARGS,
	  "get_modification_time() -> Datetime or None\n"
	  "\n"
	  "Returns the modification date and time." },

	{ "get_modification_time_as_integer",
	  (PyCFunction) pypff_message_get_modification_time_as_integer,
	  METH_NOARGS,
	  "get_modification_time_as_integer() -> Integer or None\n"
	  "\n"
	  "Returns the modification date and time as a 64-bit integer containing a FILETIME value." },

	{ "get_transport_headers",
	  (PyCFunction) pypff_message_get_transport_headers,
	  METH_NOARGS,
	  "get_transport_headers() -> String or None\n"
	  "\n"
	  "Retrieves the transport headers." },

	{ "get_plain_text_body",
	  (PyCFunction) pypff_message_get_plain_text_body,
	  METH_NOARGS,
	  "get_plain_text_body() -> String or None\n"
	  "\n"
	  "Retrieves the plain-text body." },

	{ "get_rtf_body",
	  (PyCFunction) pypff_message_get_rtf_body,
	  METH_NOARGS,
	  "get_rtf_body() -> String or None\n"
	  "\n"
	  "Retrieves the RTF body." },

	{ "get_html_body",
	  (PyCFunction) pypff_message_get_html_body,
	  METH_NOARGS,
	  "get_html_body() -> String or None\n"
	  "\n"
	  "Retrieves the HTML body." },

	/* Functions to access the attachments */

	{ "get_number_of_attachments",
	  (PyCFunction) pypff_message_get_number_of_attachments,
	  METH_NOARGS,
	  "get_number_of_attachments() -> Integer\n"
	  "\n"
	  "Retrieves the number of attachments." },

	{ "get_attachment",
	  (PyCFunction) pypff_message_get_attachment,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_attachment(attachment_index) -> Object or None\n"
	  "\n"
	  "Retrieves a specific attachment." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pypff_message_object_get_set_definitions[] = {

	{ "subject",
	  (getter) pypff_message_get_subject,
	  (setter) 0,
	  "The subject.",
	  NULL },

	{ "conversation_topic",
	  (getter) pypff_message_get_conversation_topic,
	  (setter) 0,
	  "The conversation topic.",
	  NULL },

/* TODO conversation index */

	{ "sender_name",
	  (getter) pypff_message_get_sender_name,
	  (setter) 0,
	  "The sender name.",
	  NULL },

	{ "client_submit_time",
	  (getter) pypff_message_get_client_submit_time,
	  (setter) 0,
	  "The client submit date and time.",
	  NULL },

	{ "delivery_time",
	  (getter) pypff_message_get_delivery_time,
	  (setter) 0,
	  "The delivery date and time.",
	  NULL },

	{ "creation_time",
	  (getter) pypff_message_get_creation_time,
	  (setter) 0,
	  "The creation date and time.",
	  NULL },

	{ "modification_time",
	  (getter) pypff_message_get_modification_time,
	  (setter) 0,
	  "The modification date and time.",
	  NULL },

	{ "transport_headers",
	  (getter) pypff_message_get_transport_headers,
	  (setter) 0,
	  "The transport headers.",
	  NULL },

	{ "plain_text_body",
	  (getter) pypff_message_get_plain_text_body,
	  (setter) 0,
	  "The plain text body.",
	  NULL },

	{ "rtf_body",
	  (getter) pypff_message_get_rtf_body,
	  (setter) 0,
	  "The RTF body.",
	  NULL },

	{ "html_body",
	  (getter) pypff_message_get_html_body,
	  (setter) 0,
	  "The HTML body.",
	  NULL },

	{ "number_of_attachments",
	  (getter) pypff_message_get_number_of_attachments,
	  (setter) 0,
	  "The number of attachments.",
	  NULL },

	{ "attachments",
	  (getter) pypff_message_get_attachments,
	  (setter) 0,
	  "The attachments",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pypff_message_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

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

	result = libpff_message_get_entry_value_utf8_string_size(
	          pypff_item->item,
	          LIBPFF_ENTRY_TYPE_MESSAGE_SUBJECT,
	          &value_string_size,
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
		 PyExc_MemoryError,
		 "%s: unable to create subject.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_message_get_entry_value_utf8_string(
		  pypff_item->item,
		  LIBPFF_ENTRY_TYPE_MESSAGE_SUBJECT,
		  value_string,
		  value_string_size,
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
				 NULL );
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
				 NULL );
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

/* Retrieves the conversation topic
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_message_get_conversation_topic(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *string_object  = NULL;
	uint8_t *value_string    = NULL;
	static char *function    = "pypff_message_get_conversation_topic";
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

	result = libpff_message_get_entry_value_utf8_string_size(
	          pypff_item->item,
	          LIBPFF_ENTRY_TYPE_MESSAGE_CONVERSATION_TOPIC,
	          &value_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve conversation topic size.",
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
		 PyExc_MemoryError,
		 "%s: unable to create conversation topic.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_message_get_entry_value_utf8_string(
		  pypff_item->item,
		  LIBPFF_ENTRY_TYPE_MESSAGE_CONVERSATION_TOPIC,
		  value_string,
		  value_string_size,
		  &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve conversation topic.",
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

	return( string_object );

on_error:
	if( value_string != NULL )
	{
		PyMem_Free(
		 value_string );
	}
	return( NULL );
}

/* Retrieves the sender name
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_message_get_sender_name(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *string_object  = NULL;
	uint8_t *value_string    = NULL;
	static char *function    = "pypff_message_get_sender_name";
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

	result = libpff_message_get_entry_value_utf8_string_size(
	          pypff_item->item,
	          LIBPFF_ENTRY_TYPE_MESSAGE_SENDER_NAME,
	          &value_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve sender name size.",
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
		 PyExc_MemoryError,
		 "%s: unable to create sender name.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_message_get_entry_value_utf8_string(
		  pypff_item->item,
		  LIBPFF_ENTRY_TYPE_MESSAGE_SENDER_NAME,
		  value_string,
		  value_string_size,
		  &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve sender name.",
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

	return( string_object );

on_error:
	if( value_string != NULL )
	{
		PyMem_Free(
		 value_string );
	}
	return( NULL );
}

/* Retrieves the client submit date and time
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_message_get_client_submit_time(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *date_time_object = NULL;
	libcerror_error_t *error   = NULL;
	static char *function      = "pypff_message_get_client_submit_time";
	uint64_t filetime          = 0;
	int result                 = 0;

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

	result = libpff_message_get_client_submit_time(
	          pypff_item->item,
	          &filetime,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve message client submit time.",
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
	date_time_object = pypff_datetime_new_from_filetime(
	                    filetime );

	return( date_time_object );
}

/* Retrieves the client submit date and time as an integer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_message_get_client_submit_time_as_integer(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pypff_message_get_client_submit_time_as_integer";
	uint64_t filetime        = 0;
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

	result = libpff_message_get_client_submit_time(
	          pypff_item->item,
	          &filetime,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve message client submit time.",
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
	integer_object = pypff_integer_unsigned_new_from_64bit(
	                  (uint64_t) filetime );

	return( integer_object );
}

/* Retrieves the delivery date and time
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_message_get_delivery_time(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *date_time_object = NULL;
	libcerror_error_t *error   = NULL;
	static char *function      = "pypff_message_get_delivery_time";
	uint64_t filetime          = 0;
	int result                 = 0;

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

	result = libpff_message_get_delivery_time(
	          pypff_item->item,
	          &filetime,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve message delivery time.",
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
	date_time_object = pypff_datetime_new_from_filetime(
	                    filetime );

	return( date_time_object );
}

/* Retrieves the delivery date and time as an integer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_message_get_delivery_time_as_integer(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pypff_message_get_delivery_time_as_integer";
	uint64_t filetime        = 0;
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

	result = libpff_message_get_delivery_time(
	          pypff_item->item,
	          &filetime,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve message delivery time.",
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
	integer_object = pypff_integer_unsigned_new_from_64bit(
	                  (uint64_t) filetime );

	return( integer_object );
}

/* Retrieves the creation date and time
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_message_get_creation_time(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *date_time_object = NULL;
	libcerror_error_t *error   = NULL;
	static char *function      = "pypff_message_get_creation_time";
	uint64_t filetime          = 0;
	int result                 = 0;

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

	result = libpff_message_get_creation_time(
	          pypff_item->item,
	          &filetime,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve message creation time.",
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
	date_time_object = pypff_datetime_new_from_filetime(
	                    filetime );

	return( date_time_object );
}

/* Retrieves the creation date and time as an integer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_message_get_creation_time_as_integer(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pypff_message_get_creation_time_as_integer";
	uint64_t filetime        = 0;
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

	result = libpff_message_get_creation_time(
	          pypff_item->item,
	          &filetime,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve message creation time.",
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
	integer_object = pypff_integer_unsigned_new_from_64bit(
	                  (uint64_t) filetime );

	return( integer_object );
}

/* Retrieves the modification date and time
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_message_get_modification_time(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *date_time_object = NULL;
	libcerror_error_t *error   = NULL;
	static char *function      = "pypff_message_get_modification_time";
	uint64_t filetime          = 0;
	int result                 = 0;

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

	result = libpff_message_get_modification_time(
	          pypff_item->item,
	          &filetime,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve message modification time.",
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
	date_time_object = pypff_datetime_new_from_filetime(
	                    filetime );

	return( date_time_object );
}

/* Retrieves the modification date and time as an integer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_message_get_modification_time_as_integer(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pypff_message_get_modification_time_as_integer";
	uint64_t filetime        = 0;
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

	result = libpff_message_get_modification_time(
	          pypff_item->item,
	          &filetime,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve message modification time.",
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
	integer_object = pypff_integer_unsigned_new_from_64bit(
	                  (uint64_t) filetime );

	return( integer_object );
}

/* Retrieves the transport headers
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_message_get_transport_headers(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *string_object  = NULL;
	uint8_t *value_string    = NULL;
	static char *function    = "pypff_message_get_transport_headers";
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

	result = libpff_message_get_entry_value_utf8_string_size(
	          pypff_item->item,
	          LIBPFF_ENTRY_TYPE_MESSAGE_TRANSPORT_HEADERS,
	          &value_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve transport headers size.",
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
		 PyExc_MemoryError,
		 "%s: unable to create transport headers.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_message_get_entry_value_utf8_string(
		  pypff_item->item,
	          LIBPFF_ENTRY_TYPE_MESSAGE_TRANSPORT_HEADERS,
		  value_string,
		  value_string_size,
		  &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve transport headers.",
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
		 PyExc_MemoryError,
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
	/* Do not write the end-of-string byte
	 */
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
			 (char *) value_string,
			 (Py_ssize_t) value_string_size - 1 );
#else
	string_object = PyString_FromStringAndSize(
			 (char *) value_string,
			 (Py_ssize_t) value_string_size - 1 );
#endif
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

/* Retrieves the RTF body
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_message_get_rtf_body(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *string_object  = NULL;
	uint8_t *value_string    = NULL;
	static char *function    = "pypff_message_get_rtf_body";
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

	result = libpff_message_get_rtf_body_size(
	          pypff_item->item,
	          &value_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve RTF body size.",
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
		 PyExc_MemoryError,
		 "%s: unable to create RTF body.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_message_get_rtf_body(
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
		 "%s: unable to retrieve RTF body.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Do not write the end-of-string byte
	 */
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
			 (char *) value_string,
			 (Py_ssize_t) value_string_size - 1 );
#else
	string_object = PyString_FromStringAndSize(
			 (char *) value_string,
			 (Py_ssize_t) value_string_size - 1 );
#endif
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

/* Retrieves the HTML body
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_message_get_html_body(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *string_object  = NULL;
	uint8_t *value_string    = NULL;
	static char *function    = "pypff_message_get_html_body";
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

	result = libpff_message_get_html_body_size(
	          pypff_item->item,
	          &value_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve HTML body size.",
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
		 PyExc_MemoryError,
		 "%s: unable to create HTML body.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libpff_message_get_html_body(
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
		 "%s: unable to retrieve HTML body.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Do not write the end-of-string byte
	 */
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
			 (char *) value_string,
			 (Py_ssize_t) value_string_size - 1 );
#else
	string_object = PyString_FromStringAndSize(
			 (char *) value_string,
			 (Py_ssize_t) value_string_size - 1 );
#endif
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

/* Retrieves the number of attachments
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_message_get_number_of_attachments(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object  = NULL;
	libcerror_error_t *error  = NULL;
	static char *function     = "pypff_message_get_number_of_attachments";
	int number_of_attachments = 0;
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

	result = libpff_message_get_number_of_attachments(
	          pypff_item->item,
	          &number_of_attachments,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of attachments.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) number_of_attachments );
#else
	integer_object = PyInt_FromLong(
	                  (long) number_of_attachments );
#endif
	return( integer_object );
}

/* Retrieves a specific attachment by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_message_get_attachment_by_index(
           PyObject *pypff_item,
           int attachment_index )
{
	libcerror_error_t *error  = NULL;
	libpff_item_t *sub_item   = NULL;
	PyObject *sub_item_object = NULL;
	static char *function     = "pypff_message_get_attachment_by_index";
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

	result = libpff_message_get_attachment(
	          ( (pypff_item_t *) pypff_item )->item,
	          attachment_index,
	          &sub_item,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve attachment: %d.",
		 function,
		 attachment_index );

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
		 "%s: unable to retrieve attachment: %d type.",
		 function,
		 attachment_index );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	sub_item_object = pypff_item_new(
	                   &pypff_attachment_type_object,
	                   sub_item,
	                   (PyObject *) ( (pypff_item_t *) pypff_item )->parent_object );

	if( sub_item_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create attachment object.",
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

/* Retrieves a specific attachment
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_message_get_attachment(
           pypff_item_t *pypff_item,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *sub_item_object   = NULL;
	static char *keyword_list[] = { "attachment_index", NULL };
	int attachment_index        = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &attachment_index ) == 0 )
	{
		return( NULL );
	}
	sub_item_object = pypff_message_get_attachment_by_index(
	                   (PyObject *) pypff_item,
	                   attachment_index );

	return( sub_item_object );
}

/* Retrieves an items sequence and iterator object for the attachments
 * Returns a Python object if successful or NULL on error
 */
PyObject *pypff_message_get_attachments(
           pypff_item_t *pypff_item,
           PyObject *arguments PYPFF_ATTRIBUTE_UNUSED )
{
	PyObject *sub_items_object = NULL;
	libcerror_error_t *error   = NULL;
	static char *function      = "pypff_message_get_attachments";
	int number_of_attachments  = 0;
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

	result = libpff_message_get_number_of_attachments(
	          pypff_item->item,
	          &number_of_attachments,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pypff_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of attachments.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	sub_items_object = pypff_items_new(
	                    (PyObject *) pypff_item,
	                    &pypff_message_get_attachment_by_index,
	                    number_of_attachments );

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

