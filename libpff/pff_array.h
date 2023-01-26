/*
 * The array definition of a Personal Folder File
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

#if !defined( _PFF_ARRAY_H )
#define _PFF_ARRAY_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pff_array pff_array_t;

struct pff_array
{
	/* The array signature
	 * Consists of 1 byte
	 * Contains 0x01
	 */
	uint8_t signature;

	/* The array entries level
	 * Consists of 1 byte
	 */
	uint8_t array_entries_level;

	/* The number of array entries
	 * Consists of 2 bytes
	 */
	uint8_t number_of_entries[ 2 ];

	/* The total data size
	 * Consists of 4 bytes
	 */
	uint8_t total_data_size[ 4 ];

	/* The array entries
	 * Consist of 4 bytes each
	 * Contains a (file) offset index identifier
	 */
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PFF_ARRAY_H ) */

