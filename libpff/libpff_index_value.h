/*
 * Index value functions
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

#if !defined( _LIBPFF_INDEX_VALUE_H )
#define _LIBPFF_INDEX_VALUE_H

#include <common.h>
#include <types.h>

#include "libpff_io_handle.h"
#include "libpff_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_index_value libpff_index_value_t;

struct libpff_index_value
{
	/* The identifier value
	 */
	uint64_t identifier;

	union
	{
		/* Definition for offset index leaf node
		 */
		struct
		{
			/* The file offset
			 */
			off64_t file_offset;

			/* The data size
			 */
			size32_t data_size;

			/* The reference count
			 */
			uint16_t reference_count;
		};

		/* Definition for a descriptor index leaf node
		 */
		struct
		{
			/* The data identifier
			 * Refers to a value in the file offset index
			 */
			uint64_t data_identifier;

			/* The local descriptors identifier
			 * Refers to a value in the file offset index
			 */
			uint64_t local_descriptors_identifier;

			/* The parent identifier
			 * Refers to a value in descriptor index
			 */
			uint32_t parent_identifier;
		};
	};
};

int libpff_index_value_initialize(
     libpff_index_value_t **index_value,
     libcerror_error_t **error );

int libpff_index_value_free(
     libpff_index_value_t **index_value,
     libcerror_error_t **error );

int libpff_index_value_compare(
     libpff_index_value_t *first_index_value,
     libpff_index_value_t *second_index_value,
     libcerror_error_t **error );

int libpff_index_value_read_data(
     libpff_index_value_t *index_value,
     libpff_io_handle_t *io_handle,
     uint8_t index_node_type,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_INDEX_VALUE_H ) */

