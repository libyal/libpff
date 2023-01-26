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

#if !defined( _PYPFF_FILE_H )
#define _PYPFF_FILE_H

#include <common.h>
#include <types.h>

#include "pypff_libbfio.h"
#include "pypff_libpff.h"
#include "pypff_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pypff_file pypff_file_t;

struct pypff_file
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libpff file
	 */
	libpff_file_t *file;

	/* The libbfio file IO handle
	 */
	libbfio_handle_t *file_io_handle;
};

extern PyMethodDef pypff_file_object_methods[];
extern PyTypeObject pypff_file_type_object;

int pypff_file_init(
     pypff_file_t *pypff_file );

void pypff_file_free(
      pypff_file_t *pypff_file );

PyObject *pypff_file_signal_abort(
           pypff_file_t *pypff_file,
           PyObject *arguments );

PyObject *pypff_file_open(
           pypff_file_t *pypff_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pypff_file_open_file_object(
           pypff_file_t *pypff_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pypff_file_close(
           pypff_file_t *pypff_file,
           PyObject *arguments );

PyObject *pypff_file_get_size(
           pypff_file_t *pypff_file,
           PyObject *arguments );

PyObject *pypff_file_get_content_type(
           pypff_file_t *pypff_file,
           PyObject *arguments );

PyObject *pypff_file_get_encryption_type(
           pypff_file_t *pypff_file,
           PyObject *arguments );

PyObject *pypff_file_get_ascii_codepage(
           pypff_file_t *pypff_file,
           PyObject *arguments );

int pypff_file_set_ascii_codepage_from_string(
     pypff_file_t *pypff_file,
     const char *codepage_string );

PyObject *pypff_file_set_ascii_codepage(
           pypff_file_t *pypff_file,
           PyObject *arguments,
           PyObject *keywords );

int pypff_file_set_ascii_codepage_setter(
     pypff_file_t *pypff_file,
     PyObject *string_object,
     void *closure );

PyObject *pypff_file_get_root_item(
           pypff_file_t *pypff_file,
           PyObject *arguments );

PyObject *pypff_file_get_message_store(
           pypff_file_t *pypff_file,
           PyObject *arguments );

PyObject *pypff_file_get_name_to_id_map(
           pypff_file_t *pypff_file,
           PyObject *arguments );

PyObject *pypff_file_get_root_folder(
           pypff_file_t *pypff_file,
           PyObject *arguments );

PyObject *pypff_file_get_number_of_orphan_items(
           pypff_file_t *pypff_file,
           PyObject *arguments );

PyObject *pypff_file_get_orphan_item_by_index(
           PyObject *pypff_file,
           int orphan_item_index );

PyObject *pypff_file_get_orphan_item(
           pypff_file_t *pypff_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pypff_file_get_orphan_items(
           pypff_file_t *pypff_file,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYPFF_FILE_H ) */

