/*
 * Input/Output (IO) handle functions
 *
 * Copyright (C) 2008-2020, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBPFF_IO_HANDLE2_H )
#define _LIBPFF_IO_HANDLE2_H

#include <common.h>
#include <types.h>

#include "libpff_descriptors_index.h"
#include "libpff_libbfio.h"
#include "libpff_libfcache.h"
#include "libpff_libfdata.h"
#include "libpff_io_handle.h"
#include "libpff_offsets_index.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libpff_io_handle_read_descriptor_data_list(
     libpff_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libpff_offsets_index_t *offsets_index,
     uint32_t descriptor_identifier,
     uint64_t data_identifier,
     uint8_t recovered,
     int recovered_value_index,
     libfdata_list_t **descriptor_data_list,
     libfcache_cache_t **descriptor_data_cache,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_IO_HANDLE2_H ) */

