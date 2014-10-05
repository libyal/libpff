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

#if !defined( _PYPFF_MESSAGE_H )
#define _PYPFF_MESSAGE_H

#include <common.h>
#include <types.h>

#include "pypff_file.h"
#include "pypff_item.h"
#include "pypff_libcerror.h"
#include "pypff_libpff.h"
#include "pypff_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern PyMethodDef pypff_message_object_methods[];
extern PyTypeObject pypff_message_type_object;

PyObject *pypff_message_get_subject(
           pypff_item_t *pypff_item,
           PyObject *arguments );

PyObject *pypff_message_get_plain_text_body(
           pypff_item_t *pypff_item,
           PyObject *arguments );

PyObject *pypff_message_get_html_body(
           pypff_item_t *pypff_item,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif

