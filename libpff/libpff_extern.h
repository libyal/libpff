/*
 * The internal extern definition
 *
 * Copyright (C) 2008-2026, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBPFF_INTERNAL_EXTERN_H )
#define _LIBPFF_INTERNAL_EXTERN_H

#include <common.h>

#if !defined( __CYGWIN__ ) && !defined( _WIN32 ) && defined( __has_attribute )
#if __has_attribute( visibility )
#define LIBPFF_INTERNAL	__attribute__((visibility("hidden"))) extern

#else
#define LIBPFF_INTERNAL	extern

#endif /* __has_attribute( visibility ) */
#else
#define LIBPFF_INTERNAL	extern

#endif /* !defined( __CYGWIN__ ) && !defined( _WIN32 ) && defined( __has_attribute ) */

/* Define HAVE_LOCAL_LIBPFF for local use of libpff
 */
#if !defined( HAVE_LOCAL_LIBPFF )

#include <libpff/extern.h>

#else
#define LIBPFF_EXTERN		/* extern */
#define LIBPFF_EXTERN_VARIABLE	LIBPFF_INTERNAL

#endif /* !defined( HAVE_LOCAL_LIBPFF ) */

#endif /* !defined( _LIBPFF_INTERNAL_EXTERN_H ) */

