/*
 * Export handle
 *
 * Copyright (C) 2008-2015, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _EXPORT_HANDLE_H )
#define _EXPORT_HANDLE_H

#include <common.h>
#include <types.h>

#include "item_file.h"
#include "log_handle.h"
#include "pfftools_libcerror.h"
#include "pfftools_libcstring.h"
#include "pfftools_libfdatetime.h"
#include "pfftools_libfguid.h"
#include "pfftools_libpff.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum EXPORT_MODES
{
	EXPORT_MODE_ALL				= (int) 'a',
	EXPORT_MODE_DEBUG			= (int) 'd',
	EXPORT_MODE_ITEMS			= (int) 'i',
	EXPORT_MODE_ITEMS_NO_ATTACHMENTS	= (int) 'n',
	EXPORT_MODE_RECOVERED			= (int) 'r'
};

enum EXPORT_FORMATS
{
	EXPORT_FORMAT_ALL			= (int) 'a',
	EXPORT_FORMAT_FTK			= (int) 'f',
	EXPORT_FORMAT_HTML			= (int) 'h',
	EXPORT_FORMAT_RTF			= (int) 'r',
	EXPORT_FORMAT_TEXT			= (int) 't'
};

typedef struct export_handle export_handle_t;

struct export_handle
{
	/* The export mode
	 */
	uint8_t export_mode;

	/* Value to indicate item values should be dumped
	 */
	uint8_t dump_item_values;

	/* The preferred export format
	 */
	int preferred_export_format;

	/* The ascii codepage
	 */
	int ascii_codepage;

	/* The target path
	 */
	libcstring_system_character_t *target_path;

	/* The target path size
	 */
	size_t target_path_size;

	/* The items export path
	 */
	libcstring_system_character_t *items_export_path;

	/* The items export path size
	 */
	size_t items_export_path_size;

	/* The orphans export path
	 */
	libcstring_system_character_t *orphans_export_path;

	/* The orphans export path size
	 */
	size_t orphans_export_path_size;

	/* The recovered export path
	 */
	libcstring_system_character_t *recovered_export_path;

	/* The recovered export path size
	 */
	size_t recovered_export_path_size;

	/* The number of items
	 */
	int number_of_items;

	/* The number of orphan items
	 */
	int number_of_orphan_items;

	/* The number of recovered items
	 */
	int number_of_recovered_items;

	/* The number of exported items
	 */
	int number_of_exported_items;

	/* The number of exported orphan items
	 */
	int number_of_exported_orphan_items;

	/* The number of exported recovered items
	 */
	int number_of_exported_recovered_items;

	/* Value to indicate if status information
	 * should be printed to the notify stream
	 */
	uint8_t print_status_information;

	/* The nofication output stream
	 */
	FILE *notify_stream;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int export_handle_initialize(
     export_handle_t **export_handle,
     libcerror_error_t **error );

int export_handle_free(
     export_handle_t **export_handle,
     libcerror_error_t **error );

int export_handle_signal_abort(
     export_handle_t *export_handle,
     libcerror_error_t **error );

int export_handle_set_export_mode(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error );

int export_handle_set_preferred_export_format(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error );

int export_handle_set_ascii_codepage(
     export_handle_t *export_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error );

int export_handle_set_target_path(
     export_handle_t *export_handle,
     const libcstring_system_character_t *target_path,
     libcerror_error_t **error );

int export_handle_set_export_path(
     export_handle_t *export_handle,
     const libcstring_system_character_t *base_path,
     size_t base_path_length,
     const libcstring_system_character_t *suffix,
     size_t suffix_length,
     libcstring_system_character_t **export_path,
     size_t *export_path_size,
     libcerror_error_t **error );

int export_handle_create_items_export_path(
     export_handle_t *export_handle,
     libcerror_error_t **error );

int export_handle_create_orphans_export_path(
     export_handle_t *export_handle,
     libcerror_error_t **error );

int export_handle_create_recovered_export_path(
     export_handle_t *export_handle,
     libcerror_error_t **error );

/* Item generic export functions
 */
int export_handle_create_default_item_directory(
     export_handle_t *export_handle,
     int item_index,
     const libcstring_system_character_t *item_prefix,
     size_t item_prefix_length,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     libcstring_system_character_t **item_directory_path,
     size_t *item_directory_path_size,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_create_item_file(
     export_handle_t *export_handle,
     const libcstring_system_character_t *path,
     size_t path_length,
     const libcstring_system_character_t *filename,
     size_t filename_length,
     item_file_t **item_file,
     libcerror_error_t **error );

int export_handle_export_item(
     export_handle_t *export_handle,
     libpff_item_t *item,
     int item_index,
     int number_of_items,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_record_set_to_item_file(
     export_handle_t *export_handle,
     libpff_record_set_t *record_set,
     int record_set_index,
     item_file_t *item_file,
     libcerror_error_t **error );

int export_handle_export_record_entry_to_item_file(
     export_handle_t *export_handle,
     libpff_record_entry_t *record_entry,
     int record_set_index,
     int record_entry_index,
     item_file_t *item_file,
     libcerror_error_t **error );

int export_handle_export_item_values(
     export_handle_t *export_handle,
     libpff_item_t *item,
     const libcstring_system_character_t *item_values_filename,
     size_t item_values_filename_length,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_sub_items(
     export_handle_t *export_handle,
     libpff_item_t *item,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

/* Message item export functions
 */
int export_handle_export_message_header(
     export_handle_t *export_handle,
     libpff_item_t *message,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_message_flags_to_item_file(
     export_handle_t *export_handle,
     libpff_item_t *message,
     const libcstring_system_character_t *description,
     item_file_t *item_file,
     libcerror_error_t **error );

int export_handle_export_message_importance_to_item_file(
     export_handle_t *export_handle,
     libpff_item_t *message,
     const libcstring_system_character_t *description,
     item_file_t *item_file,
     libcerror_error_t **error );

int export_handle_export_message_priority_to_item_file(
     export_handle_t *export_handle,
     libpff_item_t *message,
     const libcstring_system_character_t *description,
     item_file_t *item_file,
     libcerror_error_t **error );

int export_handle_export_message_sensitivity_item_file(
     export_handle_t *export_handle,
     libpff_item_t *message,
     const libcstring_system_character_t *description,
     item_file_t *item_file,
     libcerror_error_t **error );

int export_handle_export_message_status_to_item_file(
     export_handle_t *export_handle,
     libpff_item_t *message,
     const libcstring_system_character_t *description,
     item_file_t *item_file,
     libcerror_error_t **error );

int export_handle_export_message_subject_to_item_file(
     export_handle_t *export_handle,
     libpff_item_t *message,
     const libcstring_system_character_t *description,
     item_file_t *item_file,
     libcerror_error_t **error );

int export_handle_export_message_header_to_item_file(
     export_handle_t *export_handle,
     libpff_item_t *message,
     item_file_t *item_file,
     libcerror_error_t **error );

int export_handle_export_message_body(
     export_handle_t *export_handle,
     libpff_item_t *message,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_message_body_html(
     export_handle_t *export_handle,
     libpff_item_t *message,
     size_t message_html_body_size,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_message_body_html_to_item_file(
     export_handle_t *export_handle,
     libpff_item_t *message,
     size_t message_html_body_size,
     item_file_t *item_file,
     libcerror_error_t **error );

int export_handle_export_message_body_rtf(
     export_handle_t *export_handle,
     libpff_item_t *message,
     size_t message_rtf_body_size,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_message_body_rtf_to_item_file(
     export_handle_t *export_handle,
     libpff_item_t *message,
     size_t message_rtf_body_size,
     item_file_t *item_file,
     libcerror_error_t **error );

int export_handle_export_message_body_plain_text(
     export_handle_t *export_handle,
     libpff_item_t *message,
     size_t plain_text_body_size,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_message_body_plain_text_to_item_file(
     export_handle_t *export_handle,
     libpff_item_t *message,
     size_t plain_text_body_size,
     item_file_t *item_file,
     libcerror_error_t **error );

int export_handle_export_message_conversation_index(
     export_handle_t *export_handle,
     libpff_item_t *message,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_message_conversation_index_to_item_file(
     export_handle_t *export_handle,
     libpff_item_t *message,
     item_file_t *item_file,
     libcerror_error_t **error );

int export_handle_export_message_transport_headers(
     export_handle_t *export_handle,
     libpff_item_t *message,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_message_transport_headers_to_item_file(
     export_handle_t *export_handle,
     libpff_item_t *message,
     size_t message_transport_headers_size,
     item_file_t *item_file,
     libcerror_error_t **error );

int export_handle_export_attachments(
     export_handle_t *export_handle,
     libpff_item_t *item,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_attachment(
     export_handle_t *export_handle,
     libpff_item_t *attachment,
     int attachment_index,
     int number_of_attachments,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_attachment_data(
     export_handle_t *export_handle,
     libpff_item_t *attachment,
     int attachment_index,
     int number_of_attachments,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_attachment_item(
     export_handle_t *export_handle,
     libpff_item_t *attachment,
     int attachment_index,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_recipients(
     export_handle_t *export_handle,
     libpff_item_t *item,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_recipient_type_to_item_file(
     export_handle_t *export_handle,
     libpff_item_t *recipients,
     int recipient_index,
     const libcstring_system_character_t *description,
     item_file_t *item_file,
     libcerror_error_t **error );

int export_handle_export_recipients_to_item_file(
     export_handle_t *export_handle,
     libpff_item_t *recipients,
     int number_of_recipients,
     item_file_t *item_file,
     log_handle_t *log_handle,
     libcerror_error_t **error );

/* Item specific export functions
 */
int export_handle_export_activity(
     export_handle_t *export_handle,
     libpff_item_t *activity,
     int activity_index,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_appointment(
     export_handle_t *export_handle,
     libpff_item_t *appointment,
     int appointment_index,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_contact(
     export_handle_t *export_handle,
     libpff_item_t *contact,
     int contact_index,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_distribution_list(
     export_handle_t *export_handle,
     libpff_item_t *distribution_list,
     int distribution_list_index,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_document(
     export_handle_t *export_handle,
     libpff_item_t *document,
     int document_index,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_email(
     export_handle_t *export_handle,
     libpff_item_t *email,
     int email_index,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_email_ftk(
     export_handle_t *export_handle,
     libpff_item_t *email,
     size_t email_html_body_size,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_meeting(
     export_handle_t *export_handle,
     libpff_item_t *meeting,
     int meeting_index,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_note(
     export_handle_t *export_handle,
     libpff_item_t *note,
     int note_index,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_rss_feed(
     export_handle_t *export_handle,
     libpff_item_t *rss_feed,
     int rss_feed_index,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_task(
     export_handle_t *export_handle,
     libpff_item_t *task,
     int task_index,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

/* Folder export functions
 */
int export_handle_export_folder(
     export_handle_t *export_handle,
     libpff_item_t *folder,
     int folder_index,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_sub_folders(
     export_handle_t *export_handle,
     libpff_item_t *folder,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_sub_messages(
     export_handle_t *export_handle,
     libpff_item_t *folder,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_unknowns(
     export_handle_t *export_handle,
     libpff_item_t *folder,
     libcerror_error_t **error );

/* File export functions
 */
int export_handle_export_items(
     export_handle_t *export_handle,
     libpff_file_t *file,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_orphan_items(
     export_handle_t *export_handle,
     libpff_file_t *file,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_recovered_items(
     export_handle_t *export_handle,
     libpff_file_t *file,
     const libcstring_system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_file(
     export_handle_t *export_handle,
     libpff_file_t *file,
     log_handle_t *log_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

