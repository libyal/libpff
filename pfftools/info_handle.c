/*
 * Info handle
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

#include <common.h>
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "info_handle.h"
#include "pffinput.h"
#include "pfftools_libcerror.h"
#include "pfftools_libclocale.h"
#include "pfftools_libpff.h"

#define INFO_HANDLE_NOTIFY_STREAM	stdout

/* Creates an info handle
 * Make sure the value info_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int info_handle_initialize(
     info_handle_t **info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_initialize";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid info handle value already set.",
		 function );

		return( -1 );
	}
	*info_handle = memory_allocate_structure(
	                info_handle_t );

	if( *info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create info handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *info_handle,
	     0,
	     sizeof( info_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear info handle.",
		 function );

		goto on_error;
	}
	if( libpff_file_initialize(
	     &( ( *info_handle )->input_file ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize input file.",
		 function );

		goto on_error;
	}
	( *info_handle )->notify_stream = INFO_HANDLE_NOTIFY_STREAM;

	return( 1 );

on_error:
	if( *info_handle != NULL )
	{
		memory_free(
		 *info_handle );

		*info_handle = NULL;
	}
	return( -1 );
}

/* Frees an info handle
 * Returns 1 if successful or -1 on error
 */
int info_handle_free(
     info_handle_t **info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_free";
	int result            = 1;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle != NULL )
	{
		if( ( *info_handle )->input_file != NULL )
		{
			if( libpff_file_free(
			     &( ( *info_handle )->input_file ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free input file.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *info_handle );

		*info_handle = NULL;
	}
	return( result );
}

/* Signals the info handle to abort
 * Returns 1 if successful or -1 on error
 */
int info_handle_signal_abort(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_signal_abort";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_file != NULL )
	{
		if( libpff_file_signal_abort(
		     info_handle->input_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal input file to abort.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Sets the ascii codepage
 * Returns 1 if successful or -1 on error
 */
int info_handle_set_ascii_codepage(
     info_handle_t *info_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function  = "info_handle_set_ascii_codepage";
	size_t string_length   = 0;
	uint32_t feature_flags = 0;
	int result             = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	feature_flags = LIBCLOCALE_CODEPAGE_FEATURE_FLAG_HAVE_KOI8
	              | LIBCLOCALE_CODEPAGE_FEATURE_FLAG_HAVE_WINDOWS;

	string_length = system_string_length(
	                 string );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libclocale_codepage_copy_from_string_wide(
	          &( info_handle->ascii_codepage ),
	          string,
	          string_length,
	          feature_flags,
	          error );
#else
	result = libclocale_codepage_copy_from_string(
	          &( info_handle->ascii_codepage ),
	          string,
	          string_length,
	          feature_flags,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine ASCII codepage.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Opens the input of the info handle
 * Returns 1 if successful or -1 on error
 */
int info_handle_open_input(
     info_handle_t *info_handle,
     const system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function = "info_handle_open_input";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libpff_file_open_wide(
	     info_handle->input_file,
	     filename,
	     LIBPFF_OPEN_READ,
	     error ) != 1 )
#else
	if( libpff_file_open(
	     info_handle->input_file,
	     filename,
	     LIBPFF_OPEN_READ,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open input file.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Closes the info handle
 * Returns the 0 if succesful or -1 on error
 */
int info_handle_close(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_close";
	int result            = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->root_item != NULL )
	{
		if( libpff_item_free(
		     &( info_handle->root_item ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free root item.",
			 function );

			result = -1;
		}
	}
	if( libpff_file_close(
	     info_handle->input_file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close input file.",
		 function );

		result = -1;
	}
	return( result );
}

/* Prints the file information to a stream
 * Returns 1 if successful or -1 on error
 */
int info_handle_file_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function     = "info_handle_file_fprint";
	size64_t file_size        = 0;
	uint8_t file_content_type = 0;
	uint8_t file_type         = 0;
	uint8_t encryption_type   = 0;
	int is_corrupted          = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libpff_file_get_size(
	     info_handle->input_file,
	     &file_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size.",
		 function );

		return( -1 );
	}
	if( libpff_file_get_content_type(
	     info_handle->input_file,
	     &file_content_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file content type.",
		 function );

		return( -1 );
	}
	if( libpff_file_get_type(
	     info_handle->input_file,
	     &file_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file type.",
		 function );

		return( -1 );
	}
	if( libpff_file_get_encryption_type(
	     info_handle->input_file,
	     &encryption_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve encryption type.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "Personal Folder File information:\n" );

	fprintf(
	 info_handle->notify_stream,
	 "\tFile size:\t\t%" PRIu64 " bytes\n",
	 file_size );

	fprintf(
	 info_handle->notify_stream,
	 "\tFile content type:\t" );

	if( file_content_type == LIBPFF_FILE_CONTENT_TYPE_PAB )
	{
		fprintf(
		 info_handle->notify_stream,
		 "Personal Address Book (PAB)\n" );
	}
	else if( file_content_type == LIBPFF_FILE_CONTENT_TYPE_PST )
	{
		fprintf(
		 info_handle->notify_stream,
		 "Personal Storage Tables (PST)\n" );
	}
	else if( file_content_type == LIBPFF_FILE_CONTENT_TYPE_OST )
	{
		fprintf(
		 info_handle->notify_stream,
		 "Offline Storage Tables (OST)\n" );
	}
	else
	{
		fprintf(
		 info_handle->notify_stream,
		 "unknown: 0x02%" PRIx8 "\n",
		 file_content_type );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tFile type:\t\t" );

	if( file_type == LIBPFF_FILE_TYPE_32BIT )
	{
		fprintf(
		 info_handle->notify_stream,
		 "32-bit\n" );
	}
	else if( file_type == LIBPFF_FILE_TYPE_64BIT )
	{
		fprintf(
		 info_handle->notify_stream,
		 "64-bit\n" );
	}
	else if( file_type == LIBPFF_FILE_TYPE_64BIT_4K_PAGE )
	{
		fprintf(
		 info_handle->notify_stream,
		 "64-bit with 4k page\n" );
	}
	else
	{
		fprintf(
		 info_handle->notify_stream,
		 "unknown: 0x02%" PRIx8 "\n",
		 file_type );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tEncryption type:\t" );

	if( encryption_type == LIBPFF_ENCRYPTION_TYPE_NONE )
	{
		fprintf(
		 info_handle->notify_stream,
		 "none\n" );
	}
	else if( encryption_type == LIBPFF_ENCRYPTION_TYPE_COMPRESSIBLE )
	{
		fprintf(
		 info_handle->notify_stream,
		 "compressible\n" );
	}
	else if( encryption_type == LIBPFF_ENCRYPTION_TYPE_HIGH )
	{
		fprintf(
		 info_handle->notify_stream,
		 "high\n" );
	}
	else
	{
		fprintf(
		 info_handle->notify_stream,
		 "unknown: 0x02%" PRIx8 "\n",
		 encryption_type );
	}
	is_corrupted = libpff_file_is_corrupted(
	                info_handle->input_file,
	                error );

	if( is_corrupted == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if file is corrupted.",
		 function );

		return( -1 );
	}
	if( is_corrupted != 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tIs corrupted\n" );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	return( 1 );
}

/* Prints the message store to a stream
 * Returns 1 if successful or -1 on error
 */
int info_handle_message_store_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	libpff_item_t *message_store        = NULL;
	libpff_record_entry_t *record_entry = NULL;
	libpff_record_set_t *record_set     = NULL;
	static char *function               = "info_handle_message_store_fprint";
	uint32_t password_checksum          = 0;
	uint32_t valid_folder_mask          = 0;
	int result                          = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	result = libpff_file_get_message_store(
	          info_handle->input_file,
	          &message_store,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve message store.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 1 );
	}
	if( libpff_item_get_record_set_by_index(
	     message_store,
	     0,
	     &record_set,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record set.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "Message store:\n" );

	result = libpff_record_set_get_entry_by_type(
		  record_set,
		  LIBPFF_ENTRY_TYPE_MESSAGE_STORE_VALID_FOLDER_MASK,
		  LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED,
		  &record_entry,
		  0,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve valid folder mask record entry.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libpff_record_entry_get_data_as_32bit_integer(
		     record_entry,
		     &valid_folder_mask,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve 32-bit integer value.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tFolders:\t\t" );

		if( ( valid_folder_mask & LIBPFF_VALID_FOLDER_MASK_SUBTREE ) != 0 )
		{
			fprintf(
			 info_handle->notify_stream,
			 "Subtree, " );
		}
		if( ( valid_folder_mask & LIBPFF_VALID_FOLDER_MASK_INBOX ) != 0 )
		{
			fprintf(
			 info_handle->notify_stream,
			 "Inbox, " );
		}
		if( ( valid_folder_mask & LIBPFF_VALID_FOLDER_MASK_OUTBOX ) != 0 )
		{
			fprintf(
			 info_handle->notify_stream,
			 "Outbox, " );
		}
		if( ( valid_folder_mask & LIBPFF_VALID_FOLDER_MASK_WASTEBOX ) != 0 )
		{
			fprintf(
			 info_handle->notify_stream,
			 "Wastbox, " );
		}
		if( ( valid_folder_mask & LIBPFF_VALID_FOLDER_MASK_SENTMAIL ) != 0 )
		{
			fprintf(
			 info_handle->notify_stream,
			 "Sentmail, " );
		}
		if( ( valid_folder_mask & LIBPFF_VALID_FOLDER_MASK_VIEWS ) != 0 )
		{
			fprintf(
			 info_handle->notify_stream,
			 "Views, " );
		}
		if( ( valid_folder_mask & LIBPFF_VALID_FOLDER_MASK_COMMON_VIEWS ) != 0 )
		{
			fprintf(
			 info_handle->notify_stream,
			 "Common views, " );
		}
		if( ( valid_folder_mask & LIBPFF_VALID_FOLDER_MASK_FINDER ) != 0 )
		{
			fprintf(
			 info_handle->notify_stream,
			 "Finder" );
		}
		fprintf(
		 info_handle->notify_stream,
		 "\n" );

		if( libpff_record_entry_free(
		     &record_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free record entry.",
			 function );

			goto on_error;
		}
	}
	result = libpff_record_set_get_entry_by_type(
		  record_set,
		  LIBPFF_ENTRY_TYPE_MESSAGE_STORE_PASSWORD_CHECKSUM,
		  LIBPFF_VALUE_TYPE_INTEGER_32BIT_SIGNED,
		  &record_entry,
		  0,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve password checksum record entry.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libpff_record_entry_get_data_as_32bit_integer(
		     record_entry,
		     &password_checksum,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve 32-bit integer value.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tPassword checksum:\t" );

		if( ( password_checksum == 0x00000000UL )
		 || ( password_checksum == 0xffffffffUL ) )
		{
			fprintf(
			 info_handle->notify_stream,
			 "N/A" );
		}
		else
		{
			fprintf(
			 info_handle->notify_stream,
			 "0x%08" PRIx32 "",
			 password_checksum );
		}
		fprintf(
		 info_handle->notify_stream,
		 "\n" );

		if( libpff_record_entry_free(
		     &record_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free record entry.",
			 function );

			goto on_error;
		}
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	if( libpff_record_set_free(
	     &record_set,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free record set.",
		 function );

		goto on_error;
	}
	if( libpff_item_free(
	     &message_store,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free message store item.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( record_entry != NULL )
	{
		libpff_record_entry_free(
		 &record_entry,
		 NULL );
	}
	if( record_set != NULL )
	{
		libpff_record_set_free(
		 &record_set,
		 NULL );
	}
	if( message_store != NULL )
	{
		libpff_item_free(
		 &message_store,
		 NULL );
	}
	return( -1 );
}

/* Prints the unallocaated blocks to a stream
 * Returns 1 if successful or -1 on error
 */
int info_handle_unallocated_blocks_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function            = "info_handle_unallocated_blocks_fprint";
	off64_t offset                   = 0;
	size64_t size                    = 0;
	int block_iterator               = 0;
	int number_of_unallocated_blocks = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libpff_file_get_number_of_unallocated_blocks(
	     info_handle->input_file,
	     LIBPFF_UNALLOCATED_BLOCK_TYPE_PAGE,
	     &number_of_unallocated_blocks,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of unallocated blocks.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "Unallocated page blocks:\n" );

	if( number_of_unallocated_blocks > 0 )
	{
		for( block_iterator = 0;
		     block_iterator < number_of_unallocated_blocks;
		     block_iterator++ )
		{
			if( libpff_file_get_unallocated_block(
			     info_handle->input_file,
			     LIBPFF_UNALLOCATED_BLOCK_TYPE_PAGE,
			     block_iterator,
			     &offset,
			     &size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve unallocated block: %d.",
				 function,
				 block_iterator + 1 );

				return( -1 );
			}
			fprintf(
			 info_handle->notify_stream,
			 "\t%08" PRIu64 " (0x%08" PRIx64 ") - %08" PRIu64 " (0x%08" PRIx64 ") size: %" PRIu64 "\n",
			 offset,
			 offset,
			 offset + size,
			 offset + size,
			 size );
		}
	}
	else
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tN/A\n" );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	if( libpff_file_get_number_of_unallocated_blocks(
	     info_handle->input_file,
	     LIBPFF_UNALLOCATED_BLOCK_TYPE_DATA,
	     &number_of_unallocated_blocks,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of unallocated blocks.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "Unallocated data blocks:\n" );

	if( number_of_unallocated_blocks > 0 )
	{
		for( block_iterator = 0;
		     block_iterator < number_of_unallocated_blocks;
		     block_iterator++ )
		{
			if( libpff_file_get_unallocated_block(
			     info_handle->input_file,
			     LIBPFF_UNALLOCATED_BLOCK_TYPE_DATA,
			     block_iterator,
			     &offset,
			     &size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve unallocated block: %d.",
				 function, ( block_iterator + 1 ) );

				return( -1 );
			}
			fprintf(
			 info_handle->notify_stream,
			 "\t%08" PRIu64 " (0x%08" PRIx64 ") - %08" PRIu64 " (0x%08" PRIx64 ") size: %" PRIu64 "\n",
			 offset,
			 offset,
			 offset + size,
			 offset + size,
			 size );
		}
	}
	else
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tN/A\n" );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	return( 1 );
}

