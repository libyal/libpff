/*
 * File header functions
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

#if !defined( _LIBPFF_FILE_HEADER_H )
#define _LIBPFF_FILE_HEADER_H

#include <common.h>
#include <types.h>

#include "libpff_libbfio.h"
#include "libpff_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_file_header libpff_file_header_t;

struct libpff_file_header
{
	/* The file content type
	 */
	int file_content_type;

	/* The file type
	 */
	uint8_t file_type;

	/* The file size
	 */
	size64_t file_size;

	/* The encryption type
	 */
	uint8_t encryption_type;

	/* The descriptors index root node offset
	 */
	off64_t descriptors_index_root_node_offset;

	/* The descriptors index root node back pointer
	 */
	uint64_t descriptors_index_root_node_back_pointer;

	/* The offsets index root node offset
	 */
	off64_t offsets_index_root_node_offset;

	/* The offsets index root node back pointer
	 */
	uint64_t offsets_index_root_node_back_pointer;
};

int libpff_file_header_initialize(
     libpff_file_header_t **file_header,
     libcerror_error_t **error );

int libpff_file_header_free(
     libpff_file_header_t **file_header,
     libcerror_error_t **error );

int libpff_file_header_read_data(
     libpff_file_header_t *file_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libpff_file_header_read_file_io_handle(
     libpff_file_header_t *file_header,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_FILE_HEADER_H ) */

