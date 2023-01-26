/*
 * Item descriptor functions
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

#if !defined( _LIBPFF_ITEM_DESCRIPTOR_H )
#define _LIBPFF_ITEM_DESCRIPTOR_H

#include <common.h>
#include <types.h>

#include "libpff_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_item_descriptor libpff_item_descriptor_t;

struct libpff_item_descriptor
{
	/* The descriptor identifier value
	 */
	uint32_t descriptor_identifier;

	/* The data identifier value
	 */
	uint64_t data_identifier;

	/* The local descriptors identifier value
	 */
	uint64_t local_descriptors_identifier;

	/* Value to indicate if the item was recovered
	 */
	uint8_t recovered;

	/* The value index of the recovered data identifier
	 */
	int recovered_data_identifier_value_index;

	/* The value index of the recovered local descriptors identifier
	 */
	int recovered_local_descriptors_identifier_value_index;
};

int libpff_item_descriptor_initialize(
     libpff_item_descriptor_t **item_descriptor,
     uint32_t descriptor_identifier,
     uint64_t data_identifier,
     uint64_t local_descriptors_identifier,
     uint8_t recovered,
     libcerror_error_t **error );

int libpff_item_descriptor_free(
     libpff_item_descriptor_t **item_descriptor,
     libcerror_error_t **error );

int libpff_item_descriptor_clone(
     libpff_item_descriptor_t **destination_item_descriptor,
     libpff_item_descriptor_t *source_item_descriptor,
     libcerror_error_t **error );

int libpff_item_descriptor_compare(
     libpff_item_descriptor_t *first_item_descriptor,
     libpff_item_descriptor_t *second_item_descriptor,
     libcerror_error_t **error );

int libpff_item_descriptor_get_descriptor_identifier(
     libpff_item_descriptor_t *item_descriptor,
     uint32_t *identifier,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_ITEM_DESCRIPTOR_H ) */

