/*
 * Codepage functions
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

#if !defined( _LIBPFF_INTERNAL_CODEPAGE_H )
#define _LIBPFF_INTERNAL_CODEPAGE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

/* Define HAVE_LOCAL_LIBPFF for local use of libpff
 * The definitions in <libpff/codepage.h> are copied here
 * for local use of libpff
 */
#if !defined( HAVE_LOCAL_LIBPFF )

#include <libpff/codepage.h>

#else

/* The codepage definitions
 */
enum LIBPFF_CODEPAGES
{
	LIBPFF_CODEPAGE_ASCII		= 20127,

	LIBPFF_CODEPAGE_ISO_8859_1	= 28591,
	LIBPFF_CODEPAGE_ISO_8859_2	= 28592,
	LIBPFF_CODEPAGE_ISO_8859_3	= 28593,
	LIBPFF_CODEPAGE_ISO_8859_4	= 28594,
	LIBPFF_CODEPAGE_ISO_8859_5	= 28595,
	LIBPFF_CODEPAGE_ISO_8859_6	= 28596,
	LIBPFF_CODEPAGE_ISO_8859_7	= 28597,
	LIBPFF_CODEPAGE_ISO_8859_8	= 28598,
	LIBPFF_CODEPAGE_ISO_8859_9	= 28599,
	LIBPFF_CODEPAGE_ISO_8859_10	= 28600,
	LIBPFF_CODEPAGE_ISO_8859_11	= 28601,
	LIBPFF_CODEPAGE_ISO_8859_13	= 28603,
	LIBPFF_CODEPAGE_ISO_8859_14	= 28604,
	LIBPFF_CODEPAGE_ISO_8859_15	= 28605,
	LIBPFF_CODEPAGE_ISO_8859_16	= 28606,

	LIBPFF_CODEPAGE_KOI8_R		= 20866,
	LIBPFF_CODEPAGE_KOI8_U		= 21866,

	LIBPFF_CODEPAGE_WINDOWS_874	= 874,
	LIBPFF_CODEPAGE_WINDOWS_932	= 932,
	LIBPFF_CODEPAGE_WINDOWS_936	= 936,
	LIBPFF_CODEPAGE_WINDOWS_949	= 949,
	LIBPFF_CODEPAGE_WINDOWS_950	= 950,
	LIBPFF_CODEPAGE_WINDOWS_1250	= 1250,
	LIBPFF_CODEPAGE_WINDOWS_1251	= 1251,
	LIBPFF_CODEPAGE_WINDOWS_1252	= 1252,
	LIBPFF_CODEPAGE_WINDOWS_1253	= 1253,
	LIBPFF_CODEPAGE_WINDOWS_1254	= 1254,
	LIBPFF_CODEPAGE_WINDOWS_1255	= 1255,
	LIBPFF_CODEPAGE_WINDOWS_1256	= 1256,
	LIBPFF_CODEPAGE_WINDOWS_1257	= 1257,
	LIBPFF_CODEPAGE_WINDOWS_1258	= 1258
};

#endif /* !defined( HAVE_LOCAL_LIBPFF ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_INTERNAL_CODEPAGE_H ) */

