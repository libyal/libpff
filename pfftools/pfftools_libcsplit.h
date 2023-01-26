/*
 * The internal libcsplit header
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

#if !defined( _PFFTOOLS_LIBCSPLIT_H )
#define _PFFTOOLS_LIBCSPLIT_H

#include <common.h>

/* Define HAVE_LOCAL_LIBCSPLIT for local use of libcsplit
 */
#if defined( HAVE_LOCAL_LIBCSPLIT )

#include <libcsplit_definitions.h>
#include <libcsplit_narrow_split_string.h>
#include <libcsplit_narrow_string.h>
#include <libcsplit_types.h>
#include <libcsplit_wide_split_string.h>
#include <libcsplit_wide_string.h>

#else

/* If libtool DLL support is enabled set LIBCSPLIT_DLL_IMPORT
 * before including libcsplit.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT )
#define LIBCSPLIT_DLL_IMPORT
#endif

#include <libcsplit.h>

#endif

#endif

