/*
 * Python object wrapper of libpff_item_t type LIBPFF_ITEM_TYPE_RECIPIENTS
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

#if !defined( _PYPFF_RECIPIENTS_H )
#define _PYPFF_RECIPIENTS_H

#include <common.h>
#include <types.h>

#include "pypff_item.h"
#include "pypff_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern PyMethodDef pypff_recipients_object_methods[];
extern PyTypeObject pypff_recipients_type_object;

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYPFF_RECIPIENTS_H ) */

