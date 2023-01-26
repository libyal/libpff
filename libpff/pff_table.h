/*
 * The table definition of a Personal Folder File
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

#if !defined( _PFF_TABLE_H )
#define _PFF_TABLE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pff_table pff_table_t;

struct pff_table
{
	/* The table index offset
	 * The offset is relative to the start of the table
	 * Consists of 2 bytes
	 */
	uint8_t index_offset[ 2 ];

	/* The signature
	 * Consists of 1 byte
	 * Contains: 0xec
	 */
	uint8_t signature;

	/* The table type
	 * Consists of 1 byte
	 * Currently known value types are:
	 * 0x6c
	 * 0x7c
	 * 0x8c
	 * 0x9c
	 * 0xa5
	 * 0xac
	 * 0xbc
	 *
	 * Other types defined by [MS-PST] are:
	 * 0xb5 (considered as the b5 table header)
	 * 0xcc
	 */
	uint8_t type;

	/* The table value reference
	 * Consists of 4 bytes
	 */
	uint8_t value_reference[ 4 ];

        /* Padding
	 * Consists of 4 bytes
	 */
        uint8_t padding1[ 4 ];

	/* The table values
	 */

	/* The table index
	 */
};

typedef struct pff_table_index pff_table_index_t;

struct pff_table_index
{
	/* The number of offsets in the table index
	 * Consists of 2 bytes
	 */
	uint8_t number_of_offsets[ 2 ];

	/* The number of unused offsets in the table index
	 * Consists of 2 bytes
	 */
	uint8_t number_of_unused_offsets[ 2 ];

	/* The table value offsets
	 */
};

typedef struct pff_table_index_item pff_table_index_item_t;

struct pff_table_index_item
{
	/* The start offset of the table value
	 * The offset is relative to the start of the table
	 * Consists of 2 bytes
	 */
	uint8_t start_offset[ 2 ];

	/* The end offset of the table value
	 * The offset is relative to the start of the table
	 * Consists of 2 bytes
	 * Contains the end offset + 1
	 */
	uint8_t end_offset[ 2 ];
};

typedef struct pff_table_header_7c pff_table_header_7c_t;

struct pff_table_header_7c
{
	/* The table header type
	 * Consists of 1 byte
	 * Constant value of 0x7c
	 */
	uint8_t type;

	/* The number of column definitions in the 7c header
	 * Consists of 1 byte
	 */
	uint8_t number_of_column_definitions;

	/* The values array end offset of the 32-bit values
	 * Consists of 2 bytes
	 */
	uint8_t values_array_end_offset_32bit_values[ 2 ];

	/* The values array end offset of the 16-bit values
	 * Consists of 2 bytes
	 */
	uint8_t values_array_end_offset_16bit_values[ 2 ];

	/* The values array end offset of the 8-bit values
	 * Consists of 2 bytes
	 */
	uint8_t values_array_end_offset_8bit_values[ 2 ];

	/* The values array end offset of the cell existence block
	 * Consists of 2 bytes
	 */
	uint8_t values_array_end_offset_cell_existence_block[ 2 ];

	/* The b5 table header index reference
	 * Consists of 4 bytes
	 */
	uint8_t b5_table_header_reference[ 4 ];

	/* The values array index reference
	 * Consists of 4 bytes
	 */
	uint8_t values_array_reference[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];
};

typedef struct pff_table_column_definition_7c pff_table_column_definition_7c_t;

struct pff_table_column_definition_7c
{
	/* The record entry value type
	 * Consists of 2 bytes
	 */
	uint8_t record_entry_value_type[ 2 ];

	/* The record entry type
	 * Consists of 2 bytes
	 */
	uint8_t record_entry_type[ 2 ];

	/* The values array offset
	 * This is an offset relative to the start of a values array
	 * Consists of 2 bytes
	 */
	uint8_t values_array_offset[ 2 ];

	/* The values array size
	 * Consists of 1 byte
	 */
	uint8_t values_array_size;

	/* The values array number
	 * Consists of 1 byte
	 */
	uint8_t values_array_number;
};

typedef struct pff_table_header_ac pff_table_header_ac_t;

struct pff_table_header_ac
{
	/* The table header type
	 * Consists of 1 byte
	 * Constant value of 0xac
	 */
	uint8_t type;

	/* Padding
	 * Consists of 1 byte
	 */
	uint8_t padding1;

	/* The values array end offset of the 32-bit values
	 * Consists of 2 bytes
	 */
	uint8_t values_array_end_offset_32bit_values[ 2 ];

	/* The values array end offset of the 16-bit values
	 * Consists of 2 bytes
	 */
	uint8_t values_array_end_offset_16bit_values[ 2 ];

	/* The values array end offset of the 8-bit values
	 * Consists of 2 bytes
	 */
	uint8_t values_array_end_offset_8bit_values[ 2 ];

	/* The values array end offset of the cell existence block
	 * Consists of 2 bytes
	 */
	uint8_t values_array_end_offset_cell_existence_block[ 2 ];

	/* The b5 table header index reference
	 * Consists of 4 bytes
	 */
	uint8_t b5_table_header_reference[ 4 ];

	/* The values array index reference
	 * Consists of 4 bytes
	 */
	uint8_t values_array_reference[ 4 ];

	/* Padding
	 * Consists of 4 bytes
	 */
	uint8_t padding2[ 4 ];

	/* The number of column definitions
	 * Consists of 2 bytes
	 */
	uint8_t number_of_column_definitions[ 2 ];

	/* The column definitions index reference
	 * Consists of 4 bytes
	 */
	uint8_t column_definitions_reference[ 4 ];

	/* Unknown
	 * Consists of 12 bytes
	 * Last 4 bytes possible weak CRC-32 checksum?
	 */
	uint8_t unknown2[ 12 ];
};

typedef struct pff_table_column_definition_ac pff_table_column_definition_ac_t;

struct pff_table_column_definition_ac
{
	/* The record entry value type
	 * Consists of 2 bytes
	 */
	uint8_t record_entry_value_type[ 2 ];

	/* The record entry type
	 * Consists of 2 bytes
	 */
	uint8_t record_entry_type[ 2 ];

	/* The values array offset
	 * This is an offset relative to the start of a values array
	 * Consists of 2 bytes
	 */
	uint8_t values_array_offset[ 2 ];

	/* The values array size
	 * Consists of 2 bytes
	 */
	uint8_t values_array_size[ 2 ];

	/* The values array number
	 * Consists of 2 bytes
	 */
	uint8_t values_array_number[ 2 ];

	/* Padding
	 * Consists of 2 bytes
	 */
	uint8_t padding1[ 2 ];

	/* The record entry values table descriptor
	 * Should refer to a table of type a5
	 * Consists of 4 bytes
	 */
	uint8_t record_entry_values_table_descriptor[ 4 ];
};

typedef struct pff_table_header_b5 pff_table_header_b5_t;

struct pff_table_header_b5
{
	/* The table header type
	 * Consists of 1 byte
	 * Constant value of 0xb5
	 */
	uint8_t type;

	/* The record entry identifier size
	 * Consists of 1 byte
	 */
	uint8_t record_entry_identifier_size;

	/* The record entry value size
	 * Consists of 1 byte
	 */
	uint8_t record_entry_value_size;

	/* The record entries level
	 * Consists of 1 byte
	 */
	uint8_t record_entries_level;

	/* The record entries reference
	 * Consists of 4 bytes
	 */
	uint8_t record_entries_reference[ 4 ];
};

typedef struct pff_table_record_entry_6c pff_table_record_entry_6c_t;

struct pff_table_record_entry_6c
{
	/* The record entry GUID
	 * Consists of 16 bytes
	 */
	uint8_t record_entry_guid[ 16 ];

	/* The values array number
	 * Consists of 2 bytes
	 */
	uint8_t values_array_number[ 2 ];
};

typedef struct pff_table_record_entry_8c pff_table_record_entry_8c_t;

struct pff_table_record_entry_8c
{
	/* Identifier
	 * Consists of 8 bytes
	 */
	uint8_t identifier[ 8 ];

	/* Descriptor identifier
	 * Consists of 4 bytes
	 */
	uint8_t descriptor_identifier[ 4 ];
};

typedef struct pff_table_record_entry_9c pff_table_record_entry_9c_t;

struct pff_table_record_entry_9c
{
	/* The record entry GUID
	 * Consists of 16 bytes
	 */
	uint8_t record_entry_guid[ 16 ];

	/* Descriptor identifier
	 * Consists of 4 bytes
	 */
	uint8_t descriptor_identifier[ 4 ];
};

typedef struct pff_table_record_entry_bc pff_table_record_entry_bc_t;

struct pff_table_record_entry_bc
{
	/* The record entry type
	 * Consists of 2 bytes
	 */
	uint8_t record_entry_type[ 2 ];

	/* The record entry value type
	 * Consists of 2 bytes
	 */
	uint8_t record_entry_value_type[ 2 ];

	/* The record entry value or value reference
	 * The value reference is a descriptor to the table index or local descriptor list
	 * Consists of 4 bytes
	 */
	uint8_t record_entry_value[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PFF_TABLE_H ) */

