/*
 * The file header definition of a Personal Folder File
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

#if !defined( _PFF_FILE_HEADER_H )
#define _PFF_FILE_HEADER_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

#define PFF_FILE_HEADER_CONTENT_TYPE_PAB	0x4142
#define PFF_FILE_HEADER_CONTENT_TYPE_PST	0x4d53
#define PFF_FILE_HEADER_CONTENT_TYPE_OST	0x4f53

typedef struct pff_file_header pff_file_header_t;

struct pff_file_header
{
	/* Signature
	 * Consists of 4 bytes
	 * 0x21 0x42 0x24 0x2e
	 */
	uint8_t signature[ 4 ];

	/* Weak CRC-32 checksum of the following 471 bytes
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];

	/* Content type
	 * Consists of 2 bytes
	 * 0x53 0x4d (SM) for PST files
	 * 0x53 0x4f (SO) for OST files
	 */
	uint8_t content_type[ 2 ];

	/* Data version
	 * Consists of 2 byte
	 * 0x000e for 32 bit data type
	 * 0x0015 for 64 bit data type
	 * 0x0017 for 64 bit data type
	 */
	uint8_t data_version[ 2 ];

	/* Content verion
	 * Consists of 2 bytes
	 */
	uint8_t content_version[ 2 ];

	/* Creation platform
	 * Consists of 1 byte
	 */
	uint8_t creation_platform;

	/* Access platform
	 * Consists of 1 byte
	 */
	uint8_t access_platform;

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown2[ 4 ];
};

typedef struct pff_file_header_data_32bit pff_file_header_data_32bit_t;

struct pff_file_header_data_32bit
{
	/* Next index pointer
	 * Consists of 4 bytes
	 */
	uint8_t next_index_pointer[ 4 ];

	/* Next index back pointer
	 * Consists of 4 bytes
	 */
	uint8_t next_index_back_pointer[ 4 ];

	/* Seed value
	 * Consists of 4 bytes
	 */
	uint8_t seed_value[ 4 ];

	/* Descriptors index high water marks
	 * Consists of 128 bytes
	 */
	uint8_t descriptors_index_high_water_marks[ 128 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown5[ 4 ];

	/* File size
	 * Consists of 4 bytes
	 */
	uint8_t file_size[ 4 ];

	/* Last data allocation table offset
	 * Consists of 4 bytes
	 */
	uint8_t last_data_allocation_table_offset[ 4 ];

	/* The total available data size
	 * Consists of 4 bytes
	 */
	uint8_t total_available_data_size[ 4 ];

	/* The total available page size
	 * Consists of 4 bytes
	 */
	uint8_t total_available_page_size[ 4 ];

	/* The back pointer of the root node of the descriptors index
	 * Consists of 4 bytes
	 */
	uint8_t descriptors_index_back_pointer[ 4 ];

	/* The offset of the root node of the descriptors index
	 * Consists of 4 bytes
	 */
	uint8_t descriptors_index_root_node_offset[ 4 ];

	/* The back pointer of the root node of the (file) offsets index
	 * Consists of 4 bytes
	 */
	uint8_t offsets_index_back_pointer[ 4 ];

	/* The offset of the root node of the (file) offsets index
	 * Consists of 4 bytes
	 */
	uint8_t offsets_index_root_node_offset[ 4 ];

	/* Allocation table validation type
	 * Consists of 1 byte
	 */
	uint8_t allocation_table_validation_type;

	/* Unknown
	 * Consists of 3 bytes
	 */
	uint8_t unknown6[ 3 ];

	/* The initial data free map
	 * Consists of 128 bytes
	 */
	uint8_t initial_data_free_map[ 128 ];

	/* The initial page free map
	 * Consists of 128 bytes
	 */
	uint8_t initial_page_free_map[ 128 ];

	/* Sentinel
	 * Consists of 1 byte
	 * 0x80
	 */
	uint8_t sentinel;

	/* The encryption type
	 * Consists of 1 byte
	 */
	uint8_t encryption_type;

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown8[ 2 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown9[ 8 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown10[ 4 ];

	/* Unknown
	 * Consists of 3 bytes
	 */
	uint8_t unknown11[ 3 ];

	/* Unknown
	 * Consists of 1 byte
	 */
	uint8_t unknown12;

	/* Unknown
	 * Consists of 32 bytes
	 */
	uint8_t unknown13[ 32 ];
};

typedef struct pff_file_header_data_64bit pff_file_header_data_64bit_t;

struct pff_file_header_data_64bit
{
	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown3[ 8 ];

	/* Next index back pointer
	 * Consists of 8 bytes
	 */
	uint8_t next_index_back_pointer[ 8 ];

	/* Seed value
	 * Consists of 4 bytes
	 */
	uint8_t seed_value[ 4 ];

	/* Descriptors index high water marks
	 * Consists of 128 bytes
	 */
	uint8_t descriptors_index_high_water_marks[ 128 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown4[ 8 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown5[ 4 ];

	/* File size
	 * Consists of 8 bytes
	 */
	uint8_t file_size[ 8 ];

	/* Last data allocation table offset
	 * Consists of 8 bytes
	 */
	uint8_t last_data_allocation_table_offset[ 8 ];

	/* The total available data size
	 * Consists of 8 bytes
	 */
	uint8_t total_available_data_size[ 8 ];

	/* The total available page size
	 * Consists of 8 bytes
	 */
	uint8_t total_available_page_size[ 8 ];

	/* The back pointer of the root node of the descriptors index
	 * Consists of 8 bytes
	 */
	uint8_t descriptors_index_back_pointer[ 8 ];

	/* The offset of the root node of the descriptors index
	 * Consists of 8 bytes
	 */
	uint8_t descriptors_index_root_node_offset[ 8 ];

	/* The back pointer of the root node of the (file) offsets index
	 * Consists of 8 bytes
	 */
	uint8_t offsets_index_back_pointer[ 8 ];

	/* The offset of the root node of the (file) offsets index
	 * Consists of 8 bytes
	 */
	uint8_t offsets_index_root_node_offset[ 8 ];

	/* Allocation table validation type
	 * Consists of 1 byte
	 */
	uint8_t allocation_table_validation_type;

	/* Unknown
	 * Consists of 3 bytes
	 */
	uint8_t unknown6[ 3 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown7[ 4 ];

	/* The initial data free map
	 * Consists of 128 bytes
	 */
	uint8_t initial_data_free_map[ 128 ];

	/* The initial page free map
	 * Consists of 128 bytes
	 */
	uint8_t initial_page_free_map[ 128 ];

	/* Sentinel
	 * Consists of 1 byte
	 */
	uint8_t sentinel;

	/* The encryption type
	 * Consists of 1 byte
	 */
	uint8_t encryption_type;

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown8[ 2 ];

	/* Next index pointer
	 * Consists of 8 bytes
	 */
	uint8_t next_index_pointer[ 8 ];

	/* Weak CRC-32 checksum of the previous 516 bytes
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];

	/* Unknown
	 * Consists of 3 bytes
	 */
	uint8_t unknown11[ 3 ];

	/* Unknown
	 * Consists of 1 byte
	 */
	uint8_t unknown12;

	/* Unknown
	 * Consists of 32 bytes
	 */
	uint8_t unknown13[ 32 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PFF_FILE_HEADER_H ) */

