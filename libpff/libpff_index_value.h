/*
 * Index value functions
 *
 * Copyright (C) 2008-2016, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBPFF_INDEX_VALUES_H )
#define _LIBPFF_INDEX_VALUES_H

#include <common.h>
#include <types.h>

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
		/* Definition for branch node
		 */
		struct
		{
			/* The back pointer
			 */
			uint64_t back_pointer;
		};

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

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_INDEX_VALUES_H ) */

