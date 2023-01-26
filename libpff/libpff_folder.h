/*
 * Folder functions
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

#if !defined( _LIBPFF_FOLDER_H )
#define _LIBPFF_FOLDER_H

#include <common.h>
#include <types.h>

#include "libpff_extern.h"
#include "libpff_item.h"
#include "libpff_libcerror.h"
#include "libpff_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

LIBPFF_EXTERN \
int libpff_folder_get_type(
     libpff_item_t *folder,
     uint8_t *type,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_folder_get_utf8_name_size(
     libpff_item_t *folder,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_folder_get_utf8_name(
     libpff_item_t *folder,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_folder_get_utf16_name_size(
     libpff_item_t *folder,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_folder_get_utf16_name(
     libpff_item_t *folder,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libpff_folder_determine_sub_folders(
     libpff_internal_item_t *internal_item,
     libcerror_error_t **error );

int libpff_folder_determine_sub_messages(
     libpff_internal_item_t *internal_item,
     libcerror_error_t **error );

int libpff_folder_determine_sub_associated_contents(
     libpff_internal_item_t *internal_item,
     libcerror_error_t **error );

int libpff_folder_determine_unknowns(
     libpff_internal_item_t *internal_item,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_folder_get_number_of_sub_folders(
     libpff_item_t *folder,
     int *number_of_sub_folders,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_folder_get_sub_folder(
     libpff_item_t *folder,
     int sub_folder_index,
     libpff_item_t **sub_folder,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_folder_get_sub_folder_by_utf8_name(
     libpff_item_t *folder,
     uint8_t *utf8_sub_folder_name,
     size_t utf8_sub_folder_name_size,
     libpff_item_t **sub_folder,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_folder_get_sub_folder_by_utf16_name(
     libpff_item_t *folder,
     uint16_t *utf16_sub_folder_name,
     size_t utf16_sub_folder_name_size,
     libpff_item_t **sub_folder,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_folder_get_sub_folders(
     libpff_item_t *item,
     libpff_item_t **sub_folders,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_folder_get_number_of_sub_messages(
     libpff_item_t *folder,
     int *number_of_sub_messages,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_folder_get_sub_message(
     libpff_item_t *folder,
     int sub_message_index,
     libpff_item_t **sub_message,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_folder_get_sub_message_by_utf8_name(
     libpff_item_t *folder,
     uint8_t *utf8_sub_message_name,
     size_t utf8_sub_message_name_size,
     libpff_item_t **sub_message,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_folder_get_sub_message_by_utf16_name(
     libpff_item_t *folder,
     uint16_t *utf16_sub_message_name,
     size_t utf16_sub_message_name_size,
     libpff_item_t **sub_message,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_folder_get_sub_messages(
     libpff_item_t *item,
     libpff_item_t **sub_messages,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_folder_get_number_of_sub_associated_contents(
     libpff_item_t *folder,
     int *number_of_sub_associated_contents,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_folder_get_sub_associated_content(
     libpff_item_t *folder,
     int sub_associated_content_index,
     libpff_item_t **sub_associated_content,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_folder_get_sub_associated_contents(
     libpff_item_t *item,
     libpff_item_t **sub_associated_contents,
     libcerror_error_t **error );

LIBPFF_EXTERN \
int libpff_folder_get_unknowns(
     libpff_item_t *folder,
     libpff_item_t **unknowns,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBPFF_FOLDER_H ) */

