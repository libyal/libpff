/*
 * Python object wrapper of libpff_item_t type LIBPFF_ITEM_TYPE_ATTACHMENT
 *
 * Copyright (C) 2008-2018, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYPFF_ATTACHMENT_H )
#define _PYPFF_ATTACHMENT_H

#include <common.h>
#include <types.h>

#include "pypff_item.h"
#include "pypff_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern PyMethodDef pypff_attachment_object_methods[];
extern PyTypeObject pypff_attachment_type_object;

PyObject *pypff_attachment_get_size(
           pypff_item_t *pypff_item,
           PyObject *arguments );

PyObject *pypff_attachment_read_buffer(
           pypff_item_t *pypff_item,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pypff_attachment_seek_offset(
           pypff_item_t *pypff_item,
           PyObject *arguments,
           PyObject *keywords );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYPFF_ATTACHMENT_H ) */

