/*
 * The index node definition of a Personal Folder File
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

#if !defined( _PFF_INDEX_NODE_H )
#define _PFF_INDEX_NODE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pff_index_node_32bit_footer pff_index_node_32bit_footer_t;

struct pff_index_node_32bit_footer
{
	/* The number of node entries
	 * Consists of 1 byte
	 */
	uint8_t number_of_entries;

	/* The maximum number of node entries
	 * Consists of 1 byte
	 */
	uint8_t maximum_number_of_entries;

	/* The entry size
	 * Consists of 1 byte
	 */
	uint8_t entry_size;

	/* The level of the node
	 * Consists of 1 byte
	 */
	uint8_t level;

	/* Type indicator
	 * Consists of 1 byte
	 */
	uint8_t type;

	/* Copy of type indicator
	 * Consists of 1 byte
	 */
	uint8_t type_copy;

	/* Signature
	 * Consists of 2 bytes
	 */
	uint8_t signature[ 2 ];

	/* The back pointer value
	 * Consists of 4 bytes
	 */
	uint8_t back_pointer[ 4 ];

        /* A weak CRC-32 checksum of the first 500 bytes
 	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];
};

typedef struct pff_index_node_branch_entry_32bit pff_index_node_branch_entry_32bit_t;

struct pff_index_node_branch_entry_32bit
{
	/* The first identifier of the sub node
	 * Consists of 4 bytes
	 */
	uint8_t identifier[ 4 ];

	/* The back pointer
	 * Consists of 4 bytes
	 */
	uint8_t back_pointer[ 4 ];

	/* The file offset of the index sub node
	 * Consists of 4 bytes
	 */
	uint8_t file_offset[ 4 ];
};

typedef struct pff_index_node_descriptor_entry_32bit pff_index_node_descriptor_entry_32bit_t;

struct pff_index_node_descriptor_entry_32bit
{
	/* The descriptor identifier
	 * Consists of 4 bytes
	 */
	uint8_t identifier[ 4 ];

	/* The data offset identifier
	 * Consists of 4 bytes
	 */
	uint8_t data_identifier[ 4 ];

	/* The local descriptors offset identifier
	 * Consists of 4 bytes
	 */
	uint8_t local_descriptors_identifier[ 4 ];

	/* The parent descriptor identifier
	 * Consists of 4 bytes
	 */
	uint8_t parent_identifier[ 4 ];
};

typedef struct pff_index_node_offset_entry_32bit pff_index_node_offset_entry_32bit_t;

struct pff_index_node_offset_entry_32bit
{
	/* The offset identifier
	 * Consists of 4 bytes
	 */
	uint8_t identifier[ 4 ];

	/* The file offset of the index sub node
	 * Consists of 4 bytes
	 */
	uint8_t file_offset[ 4 ];

	/* The data size
	 * Consists of 2 bytes
	 */
	uint8_t data_size[ 2 ];

	/* The reference count
	 * Consists of 2 bytes
	 */
	uint8_t reference_count[ 2 ];
};

typedef struct pff_index_node_64bit_footer pff_index_node_64bit_footer_t;

struct pff_index_node_64bit_footer
{
	/* The number of node entries
	 * Consists of 1 byte
	 */
	uint8_t number_of_entries;

	/* The maximum number of node entries
	 * Consists of 1 byte
	 */
	uint8_t maximum_number_of_entries;

	/* The entry size
	 * Consists of 1 byte
	 */
	uint8_t entry_size;

	/* The level of the node
	 * Consists of 1 byte
	 */
	uint8_t level;

	/* Padding
	 * Consists of 4 bytes
	 */
	uint8_t padding1[ 4 ];

	/* Type indicator
	 * Consists of 1 byte
	 */
	uint8_t type;

	/* Copy of type indicator
	 * Consists of 1 byte
	 */
	uint8_t type_copy;

	/* Signature
	 * Consists of 2 bytes
	 */
	uint8_t signature[ 2 ];

        /* A weak CRC-32 checksum of the first 492 bytes
 	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];

	/* The back pointer value
	 * Consists of 8 bytes
	 */
	uint8_t back_pointer[ 8 ];
};

typedef struct pff_index_node_branch_entry_64bit pff_index_node_branch_entry_64bit_t;

struct pff_index_node_branch_entry_64bit
{
	/* The first identifier of the sub node
	 * Consists of 8 bytes
	 */
	uint8_t identifier[ 8 ];

	/* The back pointer
	 * Consists of 8 bytes
	 */
	uint8_t back_pointer[ 8 ];

	/* The file offset of the index sub node
	 * Consists of 8 bytes
	 */
	uint8_t file_offset[ 8 ];
};

typedef struct pff_index_node_descriptor_entry_64bit pff_index_node_descriptor_entry_64bit_t;

struct pff_index_node_descriptor_entry_64bit
{
	/* The descriptor identifier
	 * Consists of 8 bytes
	 * Only the lower 32-bits are used
	 */
	uint8_t identifier[ 8 ];

	/* The data offset identifier
	 * Consists of 8 bytes
	 */
	uint8_t data_identifier[ 8 ];

	/* The local descriptors offset identifier
	 * Consists of 8 bytes
	 */
	uint8_t local_descriptors_identifier[ 8 ];

	/* The parent descriptor identifier
	 * Consists of 4 bytes
	 */
	uint8_t parent_identifier[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];
};

typedef struct pff_index_node_offset_entry_64bit pff_index_node_offset_entry_64bit_t;

struct pff_index_node_offset_entry_64bit
{
	/* The offset identifier
	 * Consists of 8 bytes
	 */
	uint8_t identifier[ 8 ];

	/* The file offset of the index sub node
	 * Consists of 8 bytes
	 */
	uint8_t file_offset[ 8 ];

	/* The data size
	 * Consists of 2 bytes
	 */
	uint8_t data_size[ 2 ];

	/* The reference count
	 * Consists of 2 bytes
	 */
	uint8_t reference_count[ 2 ];

	/* The file offset of the data allocation table
	 * Consists of 4 bytes
	 */
	uint8_t data_allocation_table_file_offset[ 4 ];
};

typedef struct pff_index_node_64bit_4k_page_footer pff_index_node_64bit_4k_page_footer_t;

struct pff_index_node_64bit_4k_page_footer
{
	/* The number of node entries
	 * Consists of 2 bytes
	 */
	uint8_t number_of_entries[ 2 ];

	/* The maximum number of node entries
	 * Consists of 2 bytes
	 */
	uint8_t maximum_number_of_entries[ 2 ];

	/* The entry size
	 * Consists of 1 byte
	 */
	uint8_t entry_size;

	/* The level of the node
	 * Consists of 1 byte
	 */
	uint8_t level;

	/* Padding
	 * Consists of 10 bytes
	 */
	uint8_t padding1[ 10 ];

	/* Type indicator
	 * Consists of 1 byte
	 */
	uint8_t type;

	/* Copy of type indicator
	 * Consists of 1 byte
	 */
	uint8_t type_copy;

	/* Signature
	 * Consists of 2 bytes
	 */
	uint8_t signature[ 2 ];

        /* A weak CRC-32 checksum of the first 4074 bytes
 	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];

	/* The back pointer value
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

#endif /* !defined( _PFF_INDEX_NODE_H ) */

