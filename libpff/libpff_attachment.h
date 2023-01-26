/*
 * Attachment functions
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

#if !defined( _LIBPFF_ATTACHMENT_H )
#define _LIBPFF_ATTACHMENT_H

#include <common.h>
#include <types.h>

#include "libpff_extern.h"
#include "libpff_item.h"
#include "libpff_libbfio.h"
#include "libpff_libcerror.h"
#include "libpff_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

LIBPFF_EXTERN \
int libpff_attachment_get_type(
     libpff_item_t *attachment,
     int *attachment_type,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_attachment_get_data_size(
     libpff_item_t *attachment,
     size64_t *size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
ssize_t libpff_attachment_data_read_buffer(
         libpff_item_t *attachment,
         uint8_t *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBPFF_EXTERN \
off64_t libpff_attachment_data_seek_offset(
         libpff_item_t *attachment,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_attachment_get_data_file_io_handle(
     libpff_item_t *attachment,
     libbfio_handle_t **file_io_handle,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_attachment_get_item(
     libpff_item_t *attachment,
     libpff_item_t **attached_item,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_ATTACHMENT_H ) */

