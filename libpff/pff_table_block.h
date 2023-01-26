/*
 * The table block definition of a Personal Folder File
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

#if !defined( _PFF_TABLE_BLOCK_H )
#define _PFF_TABLE_BLOCK_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pff_table_block pff_table_block_t;

struct pff_table_block
{
	/* The index offset
	 * Consists of 2 bytes
	 * The offset is relative to the start of the table block
	 */
	uint8_t index_offset[ 2 ];

	/* The table block values
	 */

	/* The table block index
	 */
};

typedef struct pff_table_block_index pff_table_block_index_t;

struct pff_table_block_index
{
	/* The number of offsets in the table index
	 * Consists of 2 bytes
	 */
	uint8_t number_of_offsets[ 2 ];

	/* The number of unused offsets in the table index
	 * Consists of 2 bytes
	 */
	uint8_t number_of_unused_offsets[ 2 ];

	/* The offsets array
	 * Each index offset consists of 2 bytes
	 */
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PFF_TABLE_BLOCK_H ) */

