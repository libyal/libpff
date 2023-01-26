/*
 * Attached file IO handle functions
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

#if !defined( _LIBPFF_ATTACHED_FILE_IO_HANDLE_H )
#define _LIBPFF_ATTACHED_FILE_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include "libpff_libcerror.h"
#include "libpff_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_attached_file_io_handle libpff_attached_file_io_handle_t;

struct libpff_attached_file_io_handle
{
	/* The attachment item
	 */
	libpff_item_t *attachment;

	/* Value to indicate the IO handle is open
	 */
	uint8_t is_open;

	/* The current access flags
	 */
	int access_flags;
};

int libpff_attached_file_io_handle_initialize(
     libpff_attached_file_io_handle_t **io_handle,
     libpff_item_t *attachment,
     libcerror_error_t **error );

int libpff_attached_file_io_handle_free(
     libpff_attached_file_io_handle_t **io_handle,
     libcerror_error_t **error );

int libpff_attached_file_io_handle_clone(
     libpff_attached_file_io_handle_t **destination_io_handle,
     libpff_attached_file_io_handle_t *source_io_handle,
     libcerror_error_t **error );

int libpff_attached_file_io_handle_open(
     libpff_attached_file_io_handle_t *io_handle,
     int flags,
     libcerror_error_t **error );

int libpff_attached_file_io_handle_close(
     libpff_attached_file_io_handle_t *io_handle,
     libcerror_error_t **error );

ssize_t libpff_attached_file_io_handle_read(
         libpff_attached_file_io_handle_t *io_handle,
         uint8_t *buffer,
         size_t size,
         libcerror_error_t **error );

ssize_t libpff_attached_file_io_handle_write(
         libpff_attached_file_io_handle_t *io_handle,
         const uint8_t *buffer,
         size_t size,
         libcerror_error_t **error );

off64_t libpff_attached_file_io_handle_seek_offset(
         libpff_attached_file_io_handle_t *io_handle,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

int libpff_attached_file_io_handle_exists(
     libpff_attached_file_io_handle_t *io_handle,
     libcerror_error_t **error );

int libpff_attached_file_io_handle_is_open(
     libpff_attached_file_io_handle_t *io_handle,
     libcerror_error_t **error );

int libpff_attached_file_io_handle_get_size(
     libpff_attached_file_io_handle_t *io_handle,
     size64_t *size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_ATTACHED_FILE_IO_HANDLE_H ) */

