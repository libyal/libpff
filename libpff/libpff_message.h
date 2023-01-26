/*
 * Message functions
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

#if !defined( _LIBPFF_MESSAGE_H )
#define _LIBPFF_MESSAGE_H

#include <common.h>
#include <types.h>

#include "libpff_extern.h"
#include "libpff_item.h"
#include "libpff_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libpff_message_initialize_sub_item_attachments(
     libpff_internal_item_t *internal_item,
     libpff_item_descriptor_t *item_descriptor,
     libpff_local_descriptor_value_t *local_descriptor_value,
     libcerror_error_t **error );

int libpff_message_initialize_sub_item_recipients(
     libpff_internal_item_t *internal_item,
     libpff_item_descriptor_t *item_descriptor,
     libpff_local_descriptor_value_t *local_descriptor_value,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_message_get_entry_value_utf8_string_size(
     libpff_item_t *message,
     uint32_t entry_type,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_message_get_entry_value_utf8_string(
     libpff_item_t *message,
     uint32_t entry_type,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_message_get_entry_value_utf16_string_size(
     libpff_item_t *message,
     uint32_t entry_type,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_message_get_entry_value_utf16_string(
     libpff_item_t *message,
     uint32_t entry_type,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_message_get_client_submit_time(
     libpff_item_t *message,
     uint64_t *filetime,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_message_get_delivery_time(
     libpff_item_t *message,
     uint64_t *filetime,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_message_get_creation_time(
     libpff_item_t *message,
     uint64_t *filetime,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_message_get_modification_time(
     libpff_item_t *message,
     uint64_t *filetime,
     libcerror_error_t **error );

int libpff_message_determine_attachment(
     libpff_internal_item_t *internal_item,
     libpff_item_descriptor_t *item_descriptor,
     int attachment_index,
     libcerror_error_t **error );

int libpff_message_determine_attachments(
     libpff_internal_item_t *internal_item,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_message_get_number_of_attachments(
     libpff_item_t *message,
     int *number_of_attachments,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_message_get_attachment(
     libpff_item_t *message,
     int attachment_index,
     libpff_item_t **attachment,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_message_get_attachments(
     libpff_item_t *message,
     libpff_item_t **attachments,
     libcerror_error_t **error );

int libpff_message_determine_recipients(
     libpff_internal_item_t *internal_item,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_message_get_recipients(
     libpff_item_t *message,
     libpff_item_t **recipients,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_message_get_plain_text_body_size(
     libpff_item_t *message,
     size_t *size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_message_get_plain_text_body(
     libpff_item_t *message,
     uint8_t *message_body,
     size_t size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_message_get_rtf_body_size(
     libpff_item_t *message,
     size_t *size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_message_get_rtf_body(
     libpff_item_t *message,
     uint8_t *message_body,
     size_t size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_message_get_html_body_size(
     libpff_item_t *message,
     size_t *size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_message_get_html_body(
     libpff_item_t *message,
     uint8_t *message_body,
     size_t size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_MESSAGE_H ) */

