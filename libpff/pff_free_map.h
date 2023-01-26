/*
 * The free map definition of a Personal Folder File
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

#if !defined( _PFF_FREE_MAP_H )
#define _PFF_FREE_MAP_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pff_free_map_32bit pff_free_map_32bit_t;

struct pff_free_map_32bit
{
	/* Padding
	 * Consists of 4 bytes
	 * Contains 0x0 values
	 */
	uint8_t padding1[ 4 ];

	/* The free map data
	 * Consists of 496 bytes
	 */
	uint8_t data[ 496 ];

	/* The type
	 * Consists of 1 byte
	 */
	uint8_t type;

	/* The type copy
	 * Consists of 1 byte
	 */
	uint8_t type_copy;

	/* The signature
	 * Consists of 2 bytes
	 * Contains 0x0 values
	 */
	uint8_t signature[ 2 ];

	/* The back pointer
	 * Consists of 4 bytes
	 */
	uint8_t back_pointer[ 4 ];

	/* A weak CRC-32 checksum of the 496 bytes of table data
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];
};

typedef struct pff_free_map_64bit pff_free_map_64bit_t;

struct pff_free_map_64bit
{
	/* The free map data
	 * Consists of 496 bytes
	 */
	uint8_t data[ 496 ];

	/* The type
	 * Consists of 1 byte
	 */
	uint8_t type;

	/* The type copy
	 * Consists of 1 byte
	 */
	uint8_t type_copy;

	/* The signature
	 * Consists of 2 bytes
	 * Contains 0x0 values
	 */
	uint8_t signature[ 2 ];

	/* A weak CRC-32 checksum of the 496 bytes of table data
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];

	/* The back pointer
	 * Consists of 8 bytes
	 */
	uint8_t back_pointer[ 8 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PFF_FREE_MAP_H ) */

