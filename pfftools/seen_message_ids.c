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

#include <common.h>
#include <file_stream.h>
#include <memory.h>
#include <narrow_string.h>
#include <types.h>

#include <errno.h>

#include "seen_message_ids.h"
#include "pfftools_libcerror.h"

/* Number of hash buckets for the seen-Message-ID deduplication table */
#define SEEN_IDS_BUCKETS 1024

/* Linked list node tracking a single seen Message-ID value */
typedef struct seen_message_id seen_message_id_t;

struct seen_message_id
{
	char *id;
	seen_message_id_t *next;
};

/* Hash table mapping Message-ID strings to chain heads */
struct seen_message_ids_table
{
	seen_message_id_t *buckets[ SEEN_IDS_BUCKETS ];
};

/* djb2 hash of a NUL-terminated string, bucketed to SEEN_IDS_BUCKETS
 * Returns bucket index in [0, SEEN_IDS_BUCKETS)
 */
static uint32_t seen_ids_hash(
                 const char *id )
{
	uint32_t h = 5381;

	while( *id != 0 )
	{
		h = ( ( h << 5 ) + h ) ^ (uint8_t) *id++;
	}
	return( h % SEEN_IDS_BUCKETS );
}

/* Releases the hash table and all nodes
 * Sets the table pointer to NULL on return
 */
void seen_ids_table_free(
      seen_message_ids_table_t **table )
{
	seen_message_id_t *current = NULL;
	seen_message_id_t *next    = NULL;
	uint32_t bucket_index      = 0;

	if( ( table == NULL ) || ( *table == NULL ) )
	{
		return;
	}
	for( bucket_index = 0;
	     bucket_index < SEEN_IDS_BUCKETS;
	     bucket_index++ )
	{
		current = ( *table )->buckets[ bucket_index ];

		while( current != NULL )
		{
			next = current->next;

			if( current->id != NULL )
			{
				memory_free(
				 current->id );
			}
			memory_free(
			 current );

			current = next;
		}
	}
	memory_free(
	 *table );

	*table = NULL;
}

/* Prepends a Message-ID string into its hash bucket
 * Ownership of id is transferred to the table
 * Returns 1 if successful or -1 on error
 */
int seen_ids_table_add(
     seen_message_ids_table_t *table,
     char *id,
     libcerror_error_t **error )
{
	seen_message_id_t *node = NULL;
	uint32_t bucket_index   = 0;
	static char *function   = "seen_ids_table_add";

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	node = (seen_message_id_t *) memory_allocate(
	                              sizeof( seen_message_id_t ) );

	if( node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to allocate list node.",
		 function );

		return( -1 );
	}
	bucket_index    = seen_ids_hash( id );
	node->id        = id;
	node->next      = table->buckets[ bucket_index ];
	table->buckets[ bucket_index ] = node;

	return( 1 );
}

/* Tests whether a Message-ID string is in the table
 * Returns 1 if found, 0 if not found
 */
int seen_ids_table_contains(
     seen_message_ids_table_t *table,
     const char *id )
{
	seen_message_id_t *current = NULL;
	uint32_t bucket_index      = 0;
	size_t id_len              = 0;

	if( table == NULL )
	{
		return( 0 );
	}
	bucket_index = seen_ids_hash( id );
	current      = table->buckets[ bucket_index ];
	id_len       = narrow_string_length( id );

	while( current != NULL )
	{
		if( ( narrow_string_length( current->id ) == id_len )
		 && ( narrow_string_compare(
		       current->id,
		       id,
		       id_len ) == 0 ) )
		{
			return( 1 );
		}
		current = current->next;
	}
	return( 0 );
}

/* Populates a new table from a newline-delimited file
 * Absence of the file is not an error on first run
 * Returns 1 if successful or -1 on error
 */
int seen_ids_table_load(
     seen_message_ids_table_t **table,
     const char *path,
     libcerror_error_t **error )
{
	FILE *file            = NULL;
	char *line            = NULL;
	char *newline         = NULL;
	char *id              = NULL;
	size_t id_length      = 0;
	static char *function = "seen_ids_table_load";
	static const size_t line_size = 4096;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	*table = (seen_message_ids_table_t *) memory_allocate(
	                                       sizeof( seen_message_ids_table_t ) );

	if( *table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to allocate hash table.",
		 function );

		return( -1 );
	}
	memory_set(
	 *table,
	 0,
	 sizeof( seen_message_ids_table_t ) );

	line = (char *) memory_allocate(
	                 sizeof( char ) * line_size );

	if( line == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to allocate line buffer.",
		 function );

		return( -1 );
	}
	file = file_stream_open(
	        path,
	        "r" );

	if( file == NULL )
	{
		if( errno != ENOENT )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open dedup file for reading: %s.",
			 function,
			 path );

			memory_free(
			 line );

			return( -1 );
		}
		/* file absent on first run; not an error */
		memory_free(
		 line );

		return( 1 );
	}
	while( file_stream_get_string(
	        file,
	        line,
	        (int) line_size ) != NULL )
	{
		newline = narrow_string_search_character(
		           line,
		           (int) '\n',
		           narrow_string_length( line ) );

		if( newline != NULL )
		{
			*newline = 0;
		}
		else if( feof( file ) == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: dedup file line exceeds buffer; state may be incomplete.",
			 function );

			file_stream_close(
			 file );

			memory_free(
			 line );

			return( -1 );
		}
		id_length = narrow_string_length(
		             line );

		if( id_length == 0 )
		{
			continue;
		}
		id = (char *) memory_allocate(
		               sizeof( char ) * ( id_length + 1 ) );

		if( id == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to allocate id string.",
			 function );

			file_stream_close(
			 file );

			memory_free(
			 line );

			return( -1 );
		}
		narrow_string_copy(
		 id,
		 line,
		 id_length + 1 );

		if( seen_ids_table_add(
		     *table,
		     id,
		     error ) != 1 )
		{
			memory_free(
			 id );

			file_stream_close(
			 file );

			memory_free(
			 line );

			return( -1 );
		}
	}
	file_stream_close(
	 file );

	memory_free(
	 line );

	return( 1 );
}

/* Writes all table entries to a newline-delimited file
 * Returns 1 if successful or -1 on error
 */
int seen_ids_table_save(
     seen_message_ids_table_t *table,
     const char *path,
     libcerror_error_t **error )
{
	seen_message_id_t *current = NULL;
	FILE *file                 = NULL;
	uint32_t bucket_index      = 0;
	static char *function      = "seen_ids_table_save";

	if( path == NULL )
	{
		return( 1 );
	}
	file = file_stream_open(
	        path,
	        "w" );

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open dedup file for writing: %s.",
		 function,
		 path );

		return( -1 );
	}
	if( table != NULL )
	{
		for( bucket_index = 0;
		     bucket_index < SEEN_IDS_BUCKETS;
		     bucket_index++ )
		{
			current = table->buckets[ bucket_index ];

			while( current != NULL )
			{
				file_stream_write(
				 file,
				 current->id,
				 narrow_string_length( current->id ) );

				file_stream_write(
				 file,
				 "\n",
				 1 );

				current = current->next;
			}
		}
	}
	file_stream_close(
	 file );

	return( 1 );
}

