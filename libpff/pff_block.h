/*
 * The block definition of a Personal Folder File
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

#if !defined( _PFF_BLOCK_H )
#define _PFF_BLOCK_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pff_block_footer_32bit pff_block_footer_32bit_t;

struct pff_block_footer_32bit
{
	/* The block data size
	 * Consists of 2 bytes
	 */
	uint8_t data_size[ 2 ];

	/* The block signature
	 * Consists of 2 bytes
	 */
	uint8_t signature[ 2 ];

	/* The block back pointer
	 * Consists of 4 bytes
	 */
	uint8_t back_pointer[ 4 ];

	/* The block data CRC-32 checksum
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];
};

typedef struct pff_block_footer_64bit pff_block_footer_64bit_t;

struct pff_block_footer_64bit
{
	/* The block data size
	 * Consists of 2 bytes
	 */
	uint8_t data_size[ 2 ];

	/* The block signature
	 * Consists of 2 bytes
	 */
	uint8_t signature[ 2 ];

	/* The block data CRC-32 checksum
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];

	/* The block back pointer
	 * Consists of 8 bytes
	 */
	uint8_t back_pointer[ 8 ];
};

typedef struct pff_block_footer_64bit_4k_page pff_block_footer_64bit_4k_page_t;

struct pff_block_footer_64bit_4k_page
{
	/* The block data size
	 * Consists of 2 bytes
	 */
	uint8_t data_size[ 2 ];

	/* The block signature
	 * Consists of 2 bytes
	 */
	uint8_t signature[ 2 ];

	/* The block data CRC-32 checksum
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];

	/* The block back pointer
	 * Consists of 8 bytes
	 */
	uint8_t back_pointer[ 8 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown1[ 2 ];

	/* The uncompressed block data size
	 * Consists of 2 bytes
	 */
	uint8_t uncompressed_data_size[ 2 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown2[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PFF_BLOCK_H ) */

