/*
 * Table header functions
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

#if !defined( _LIBPFF_TABLE_HEADER_H )
#define _LIBPFF_TABLE_HEADER_H

#include <common.h>
#include <types.h>

#include "libpff_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_table_header libpff_table_header_t;

struct libpff_table_header
{
	/* The type
	 */
	uint8_t type;

	/* The table value reference
	 */
	uint32_t table_value_reference;

	/* The b5 table header reference
	 */
	uint32_t b5_table_header_reference;

	/* The values array reference
	 */
	uint32_t values_array_reference;

	/* The column definitions reference
	 */
	uint32_t column_definitions_reference;

	/* The values array entry size
	 */
	uint16_t values_array_entry_size;

	/* The number of column definitions
	 */
	int number_of_column_definitions;

	/* The column definitions data
	 */
	uint8_t *column_definitions_data;

	/* The column definitions data size
	 */
	size_t column_definitions_data_size;

	/* The record entry identifier size
	 */
	uint8_t record_entry_identifier_size;

	/* The record entry value size
	 */
	uint8_t record_entry_value_size;

	/* The record entries level
	 */
	uint8_t record_entries_level;

	/* The record entries reference
	 */
	uint32_t record_entries_reference;
};

int libpff_table_header_initialize(
     libpff_table_header_t **table_header,
     libcerror_error_t **error );

int libpff_table_header_free(
     libpff_table_header_t **table_header,
     libcerror_error_t **error );

int libpff_table_header_read_data(
     libpff_table_header_t *table_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libpff_table_header_read_6c_data(
     libpff_table_header_t *table_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libpff_table_header_read_7c_data(
     libpff_table_header_t *table_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libpff_table_header_read_9c_data(
     libpff_table_header_t *table_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libpff_table_header_read_ac_data(
     libpff_table_header_t *table_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libpff_table_header_read_b5_data(
     libpff_table_header_t *table_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_TABLE_HEADER_H ) */

