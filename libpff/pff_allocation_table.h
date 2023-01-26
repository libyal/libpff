/*
 * The allocation table definition of a Personal Folder File
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

#if !defined( _PFF_ALLOCATION_TABLE_H )
#define _PFF_ALLOCATION_TABLE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pff_allocation_table_32bit pff_allocation_table_32bit_t;

struct pff_allocation_table_32bit
{
	/* Padding
	 * Consists of 4 bytes
	 * Contains 0x0 values
	 */
	uint8_t padding1[ 4 ];

	/* The allocation table data
	 * Consists of 496 bytes
	 * A bit represents certain amount of allocated data
	 *  64 byte when type is 0x84
	 * 512 byte when type is 0x83
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

typedef struct pff_allocation_table_64bit pff_allocation_table_64bit_t;

struct pff_allocation_table_64bit
{
	/* The allocation table data
	 * Consists of 496 bytes
	 * A bit represents certain amount of allocated data
	 *  64 byte when type is 0x84
	 * 512 byte when type is 0x83
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

typedef struct pff_allocation_table_64bit_4k_page pff_allocation_table_64bit_4k_page_t;

struct pff_allocation_table_64bit_4k_page
{
	/* The allocation table data
	 * Consists of 4072 bytes
	 * A bit represents certain amount of allocated data
	 *  64 byte when type is 0x84
	 * 512 byte when type is 0x83
	 */
	uint8_t data[ 4072 ];

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

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown1[ 8 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PFF_ALLOCATION_TABLE_H ) */

