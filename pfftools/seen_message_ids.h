/*
 * Seen Message-IDs deduplication table
 *
 * Copyright (C) 2008-2025, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _SEEN_MESSAGE_IDS_H )
#define _SEEN_MESSAGE_IDS_H

#include <common.h>
#include <types.h>

#include "pfftools_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct seen_message_ids_table seen_message_ids_table_t;

int seen_ids_table_load(
     seen_message_ids_table_t **table,
     const char *path,
     libcerror_error_t **error );

void seen_ids_table_free(
      seen_message_ids_table_t **table );

int seen_ids_table_add(
     seen_message_ids_table_t *table,
     char *id,
     libcerror_error_t **error );

int seen_ids_table_contains(
     seen_message_ids_table_t *table,
     const char *id );

int seen_ids_table_save(
     seen_message_ids_table_t *table,
     const char *path,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _SEEN_MESSAGE_IDS_H ) */

