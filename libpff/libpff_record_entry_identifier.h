/*
 * Record entry identifier functions
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

#if !defined( _LIBPFF_RECORD_ENTRY_IDENTIFIER_H )
#define _LIBPFF_RECORD_ENTRY_IDENTIFIER_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libpff_record_entry_identifier libpff_record_entry_identifier_t;

struct libpff_record_entry_identifier
{
	/* The entry identifier format
	 */
	uint8_t format;

	/* The entry identifier
	 */
	union
	{
		/* The MAPI property based entry identifier
		 */
		struct
		{
			/* The entry type
			 */
			uint32_t entry_type;

			/* The value type
			 */
			uint32_t value_type;
		};
		/* The GUID based entry identifier
		 */
		uint8_t guid[ 16 ];

		/* The PRQ_ID_SECURE4 based entry identifier
		 */
		uint64_t secure4;
	};
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_RECORD_ENTRY_IDENTIFIER_H ) */

