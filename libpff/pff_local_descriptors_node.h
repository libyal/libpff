/*
 * The local descriptors node definition of a Personal Folder File
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

#if !defined( _PFF_LOCAL_DESCRIPTORS_NODE_H )
#define _PFF_LOCAL_DESCRIPTORS_NODE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pff_local_descriptors_node_32bit pff_local_descriptors_node_32bit_t;

struct pff_local_descriptors_node_32bit
{
	/* The local descriptors signature
	 * Consists of 1 byte
	 * Contains 0x02
	 */
	uint8_t signature;

	/* The local descriptors node (indirection) level
	 * Consists of 1 byte
	 */
	uint8_t level;

	/* The number of local descriptors entries
	 * Consists of 2 bytes
	 */
	uint8_t number_of_entries[ 2 ];

	/* The local descriptors node entries
	 */
};

typedef struct pff_local_descriptor_branch_node_entry_type_32bit pff_local_descriptor_branch_node_entry_type_32bit_t;

struct pff_local_descriptor_branch_node_entry_type_32bit
{
	/* The identifier
	 * Consists of 4 bytes
	 */
	uint8_t identifier[ 4 ];

	/* The local descriptors sub node identifier
	 * Consists of 4 bytes
	 * Refers to an identifier in the (file) offset index
	 */
	uint8_t sub_node_identifier[ 4 ];
};

typedef struct pff_local_descriptor_leaf_node_entry_type_32bit pff_local_descriptor_leaf_node_entry_type_32bit_t;

struct pff_local_descriptor_leaf_node_entry_type_32bit
{
	/* The identifier
	 * Consists of 4 bytes
	 */
	uint8_t identifier[ 4 ];

	/* The data identifier
	 * Consists of 4 bytes
	 * Refers to an identifier in the (file) offset index
	 */
	uint8_t data_identifier[ 4 ];

	/* The local descriptors identifier
	 * Consists of 4 bytes
	 * Refers to an identifier in the (file) offset index
	 */
	uint8_t local_descriptors_identifier[ 4 ];
};

typedef struct pff_local_descriptors_node_64bit pff_local_descriptors_node_64bit_t;

struct pff_local_descriptors_node_64bit
{
	/* The local descriptors signature
	 * Consists of 1 byte
	 * Contains 0x02
	 */
	uint8_t signature;

	/* The local descriptors node (indirection) level
	 * Consists of 1 byte
	 */
	uint8_t level;

	/* The number of local descriptors entries
	 * Consists of 2 bytes
	 */
	uint8_t number_of_entries[ 2 ];

	/* Padding
	 * Consists of 4 bytes
	 */
	uint8_t padding1[ 4 ];

	/* The local descriptors node entries
	 */
};

typedef struct pff_local_descriptor_branch_node_entry_type_64bit pff_local_descriptor_branch_node_entry_type_64bit_t;

struct pff_local_descriptor_branch_node_entry_type_64bit
{
	/* The identifier
	 * Consists of 8 bytes
	 * Only the lower 32-bits are used
	 */
	uint8_t identifier[ 8 ];

	/* The local descriptors sub node identifier
	 * Consists of 8 bytes
	 * Refers to an identifier in the (file) offset index
	 */
	uint8_t sub_node_identifier[ 8 ];
};

typedef struct pff_local_descriptor_leaf_node_entry_type_64bit pff_local_descriptor_leaf_node_entry_type_64bit_t;

struct pff_local_descriptor_leaf_node_entry_type_64bit
{
	/* The identifier
	 * Consists of 8 bytes
	 * Only the lower 32-bits are used
	 */
	uint8_t identifier[ 8 ];

	/* The data identifier
	 * Refers to an identifier in the (file) offset index
	 * Consists of 8 bytes
	 */
	uint8_t data_identifier[ 8 ];

	/* The local descriptors identifier
	 * Consists of 8 bytes
	 * Refers to an identifier in the (file) offset index
	 */
	uint8_t local_descriptors_identifier[ 8 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PFF_LOCAL_DESCRIPTORS_NODE_H ) */

