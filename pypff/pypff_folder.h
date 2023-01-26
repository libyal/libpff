/*
 * Python object wrapper of libpff_item_t type LIBPFF_ITEM_TYPE_FOLDER
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

#if !defined( _PYPFF_FOLDER_H )
#define _PYPFF_FOLDER_H

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

extern PyMethodDef pypff_folder_object_methods[];
extern PyTypeObject pypff_folder_type_object;

PyObject *pypff_folder_get_name(
           pypff_item_t *pypff_item,
           PyObject *arguments );

PyObject *pypff_folder_get_number_of_sub_folders(
           pypff_item_t *pypff_item,
           PyObject *arguments );

PyObject *pypff_folder_get_sub_folder_by_index(
           PyObject *pypff_item,
           int sub_folder_index );

PyObject *pypff_folder_get_sub_folder(
           pypff_item_t *pypff_item,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pypff_folder_get_sub_folders(
           pypff_item_t *pypff_item,
           PyObject *arguments );

PyObject *pypff_folder_get_number_of_sub_messages(
           pypff_item_t *pypff_item,
           PyObject *arguments );

PyObject *pypff_folder_get_sub_message_by_index(
           PyObject *pypff_item,
           int sub_message_index );

PyObject *pypff_folder_get_sub_message(
           pypff_item_t *pypff_item,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pypff_folder_get_sub_messages(
           pypff_item_t *pypff_item,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYPFF_FOLDER_H ) */

