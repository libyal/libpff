/*
 * Local descriptor value functions
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

#if !defined( _LIBPFF_LOCAL_DESCRIPTOR_VALUE_H )
#define _LIBPFF_LOCAL_DESCRIPTOR_VALUE_H

#include <common.h>
#include <types.h>

#include "libpff_io_handle.h"
#include "libpff_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_local_descriptor_value libpff_local_descriptor_value_t;

struct libpff_local_descriptor_value
{
	/* The identifier value
	 */
	uint64_t identifier;

	union
	{
		/* The branch node sub node identifier value
		 * Refers to a value in the file offset index
		 */
		uint64_t sub_node_identifier;

		struct
		{
			/* The data identifier value
			 * Refers to a value in the file offset index
			 */
			uint64_t data_identifier;

			/* The leaf node sub node identifier value
			 * Refers to a value in the file offset index
			 */
			uint64_t local_descriptors_identifier;
		};
	};

	/* TODO remove after refactoring
	 */
	uint8_t level;
};

int libpff_local_descriptor_value_initialize(
     libpff_local_descriptor_value_t **local_descriptor_value,
     libcerror_error_t **error );

int libpff_local_descriptor_value_free(
     libpff_local_descriptor_value_t **local_descriptor_value,
     libcerror_error_t **error );

int libpff_local_descriptor_value_read_data(
     libpff_local_descriptor_value_t *local_descriptor_value,
     libpff_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_LOCAL_DESCRIPTOR_VALUE_H ) */

