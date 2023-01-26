/*
 * Record set functions
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
#include <types.h>

#include "libpff_definitions.h"
#include "libpff_libcerror.h"
#include "libpff_libuna.h"
#include "libpff_record_entry.h"
#include "libpff_record_set.h"
#include "libpff_types.h"

/* Creates a record set
 * Make sure the value record_set is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_record_set_initialize(
     libpff_record_set_t **record_set,
     int number_of_entries,
     int ascii_codepage,
     libcerror_error_t **error )
{
	libpff_internal_record_set_t *internal_record_set = NULL;
	libpff_record_entry_t *record_entry               = NULL;
	static char *function                             = "libpff_record_set_initialize";
	int entry_index                                   = 0;

	if( record_set == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record set.",
		 function );

		return( -1 );
	}
	if( *record_set != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid record set value already set.",
		 function );

		return( -1 );
	}
	if( number_of_entries < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of entries value out of bounds.",
		 function );

		return( -1 );
	}
	internal_record_set = memory_allocate_structure(
	                       libpff_internal_record_set_t );

	if( internal_record_set == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create record set.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_record_set,
	     0,
	     sizeof( libpff_internal_record_set_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear record set.",
		 function );

		memory_free(
		 internal_record_set );

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( internal_record_set->entries_array ),
	     number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create entries array.",
		 function );

		goto on_error;
	}
	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
		if( libpff_record_entry_initialize(
		     &record_entry,
		     ascii_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create record entry: %d.",
			 function,
			 entry_index );

			goto on_error;
		}
		if( libcdata_array_set_entry_by_index(
		     internal_record_set->entries_array,
		     entry_index,
		     (intptr_t *) record_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create record entry: %d.",
			 function,
			 entry_index );

			goto on_error;
		}
		record_entry = NULL;
	}
	internal_record_set->ascii_codepage = ascii_codepage;

	*record_set = (libpff_record_set_t *) internal_record_set;

	return( 1 );

on_error:
	if( record_entry != NULL )
	{
		libpff_internal_record_entry_free(
		 (libpff_internal_record_entry_t **) &record_entry,
		 NULL );
	}
	if( internal_record_set != NULL )
	{
		if( internal_record_set->entries_array != NULL )
		{
			libcdata_array_free(
			 &( internal_record_set->entries_array ),
			 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_internal_record_entry_free,
			 NULL );
		}
		memory_free(
		 internal_record_set );
	}
	return( -1 );
}

/* Frees a record set
 * Returns 1 if successful or -1 on error
 */
int libpff_record_set_free(
     libpff_record_set_t **record_set,
     libcerror_error_t **error )
{
	static char *function = "libpff_record_set_free";

	if( record_set == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record set.",
		 function );

		return( -1 );
	}
	if( *record_set != NULL )
	{
		*record_set = NULL;
	}
	return( 1 );
}

/* Frees a record set
 * Returns 1 if successful or -1 on error
 */
int libpff_internal_record_set_free(
     libpff_internal_record_set_t **internal_record_set,
     libcerror_error_t **error )
{
	static char *function = "libpff_internal_record_set_free";
	int result            = 1;

	if( internal_record_set == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record set.",
		 function );

		return( -1 );
	}
	if( *internal_record_set != NULL )
	{
		if( libcdata_array_free(
		     &( ( *internal_record_set )->entries_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_internal_record_entry_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free entries array.",
			 function );

			result = -1;
		}
		memory_free(
		 *internal_record_set );

		*internal_record_set = NULL;
	}
	return( result );
}

/* Clones a record set
 * Make sure the value destination_record_set is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libpff_record_set_clone(
     libpff_record_set_t **destination_record_set,
     libpff_record_set_t *source_record_set,
     libcerror_error_t **error )
{
	libpff_internal_record_set_t *internal_destination_record_set = NULL;
	libpff_internal_record_set_t *internal_source_record_set      = NULL;
	static char *function                                         = "libpff_record_set_clone";

	if( destination_record_set == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination record set.",
		 function );

		return( -1 );
	}
	if( *destination_record_set != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination record set value already set.",
		 function );

		return( -1 );
	}
	if( source_record_set == NULL )
	{
		*destination_record_set = NULL;

		return( 1 );
	}
	internal_source_record_set = (libpff_internal_record_set_t *) source_record_set;

	internal_destination_record_set = memory_allocate_structure(
	                                   libpff_internal_record_set_t );

	if( internal_destination_record_set == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination record set.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_destination_record_set,
	     0,
	     sizeof( libpff_internal_record_set_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear destination record set.",
		 function );

		memory_free(
		 internal_destination_record_set );

		return( -1 );
	}
	if( libcdata_array_clone(
	     &( internal_destination_record_set->entries_array ),
	     internal_source_record_set->entries_array,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_internal_record_entry_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libpff_record_entry_clone,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to clone entries array.",
		 function );

		goto on_error;
	}
	*destination_record_set = (libpff_record_set_t *) internal_destination_record_set;

	return( 1 );

on_error:
	if( internal_destination_record_set != NULL )
	{
		libpff_internal_record_set_free(
		 &internal_destination_record_set,
		 NULL );
	}
	return( -1 );
}

/* Resizes the record set
 * Returns 1 if successful or -1 on error
 */
int libpff_record_set_resize(
     libpff_record_set_t *record_set,
     int number_of_entries,
     libcerror_error_t **error )
{
	libpff_internal_record_set_t *internal_record_set = NULL;
	libpff_record_entry_t *record_entry               = NULL;
	static char *function                             = "libpff_record_set_resize";
	int entry_index                                   = 0;
	int last_number_of_entries                        = 0;

	if( record_set == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record set.",
		 function );

		return( -1 );
	}
	internal_record_set = (libpff_internal_record_set_t *) record_set;

	if( libcdata_array_get_number_of_entries(
	     internal_record_set->entries_array,
	     &last_number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries.",
		 function );

		return( -1 );
	}
	if( libcdata_array_resize(
	     internal_record_set->entries_array,
	     number_of_entries,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libpff_internal_record_entry_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize entries array.",
		 function );

		return( -1 );
	}
	for( entry_index = last_number_of_entries;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
		if( libpff_record_entry_initialize(
		     &record_entry,
		     internal_record_set->ascii_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create record entry: %d.",
			 function,
			 entry_index );

			goto on_error;
		}
		if( libcdata_array_set_entry_by_index(
		     internal_record_set->entries_array,
		     entry_index,
		     (intptr_t *) record_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create record entry: %d.",
			 function,
			 entry_index );

			goto on_error;
		}
		record_entry = NULL;
	}
	return( 1 );

on_error:
	if( record_entry != NULL )
	{
		libpff_internal_record_entry_free(
		 (libpff_internal_record_entry_t **) &record_entry,
		 NULL );
	}
	if( last_number_of_entries != 0 )
	{
		libcdata_array_resize(
		 internal_record_set->entries_array,
		 last_number_of_entries,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libpff_internal_record_entry_free,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the number of entries in the record set
 * Returns 1 if successful or -1 on error
 */
int libpff_record_set_get_number_of_entries(
     libpff_record_set_t *record_set,
     int *number_of_entries,
     libcerror_error_t **error )
{
	libpff_internal_record_set_t *internal_record_set = NULL;
	static char *function                             = "libpff_record_set_get_number_of_entries";

	if( record_set == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record set.",
		 function );

		return( -1 );
	}
	internal_record_set = (libpff_internal_record_set_t *) record_set;

	if( libcdata_array_get_number_of_entries(
	     internal_record_set->entries_array,
	     number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific entry from the record set
 * Returns 1 if successful or -1 on error
 */
int libpff_record_set_get_entry_by_index(
     libpff_record_set_t *record_set,
     int entry_index,
     libpff_record_entry_t **record_entry,
     libcerror_error_t **error )
{
	libpff_internal_record_set_t *internal_record_set = NULL;
	static char *function                             = "libpff_record_set_get_entry_by_index";

	if( record_set == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record set.",
		 function );

		return( -1 );
	}
	internal_record_set = (libpff_internal_record_set_t *) record_set;

	if( libcdata_array_get_entry_by_index(
	     internal_record_set->entries_array,
	     entry_index,
	     (intptr_t **) record_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create record entry: %d.",
		 function,
		 entry_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the record entry matching the entry and value type pair from the record set
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE flag is set
 * the value type is ignored and set. The default behavior is a strict
 * matching of the value type. In this case the value type must be filled
 * with the corresponding value type
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP is set
 * the name to identifier mapping is ignored. The default behavior is
 * to use the mapped entry value. In this case named properties are not
 * retrieved.
 *
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_record_set_get_entry_by_type(
     libpff_record_set_t *record_set,
     uint32_t entry_type,
     uint32_t value_type,
     libpff_record_entry_t **record_entry,
     uint8_t flags,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	libpff_internal_record_set_t *internal_record_set     = NULL;
	static char *function                                 = "libpff_record_set_get_entry_by_type";
	int entry_index                                       = 0;
	int number_of_entries                                 = 0;
	int result                                            = 0;

	if( record_set == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record set.",
		 function );

		return( -1 );
	}
	internal_record_set = (libpff_internal_record_set_t *) record_set;

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	if( *record_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid record entry value already set.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     internal_record_set->entries_array,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries.",
		 function );

		return( -1 );
	}
	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     internal_record_set->entries_array,
		     entry_index,
		     (intptr_t **) &internal_record_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record entry: %d.",
			 function,
			 entry_index );

			return( -1 );
		}
		if( internal_record_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing data record entry: %d.",
			 function,
			 entry_index );

			return( -1 );
		}
		result = 0;

		/* Skip tables entries that do not contain a MAPI identifier
		 */
		if( internal_record_entry->identifier.format != LIBPFF_RECORD_ENTRY_IDENTIFIER_FORMAT_MAPI_PROPERTY )
		{
			continue;
		}
		/* Ignore the name to identifier map
		 */
		if( ( flags & LIBPFF_ENTRY_VALUE_FLAG_IGNORE_NAME_TO_ID_MAP ) != 0 )
		{
			if( internal_record_entry->identifier.entry_type == entry_type )
			{
				result = 1;
			}
		}
		/* Mapped properties must be accessed through their mapped entry type value
		 */
		else if( internal_record_entry->name_to_id_map_entry != NULL )
		{
			if( internal_record_entry->name_to_id_map_entry->type == LIBPFF_NAME_TO_ID_MAP_ENTRY_TYPE_NUMERIC )
			{
				if( internal_record_entry->name_to_id_map_entry->numeric_value == entry_type )
				{
					result = 1;
				}
			}
		}
		/* Other properties are accessed through their entry type value
		 */
		else if( internal_record_entry->identifier.entry_type == entry_type )
		{
			result = 1;
		}
		if( result != 0 )
		{
			if( ( ( flags & LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE ) != 0 )
			 || ( internal_record_entry->identifier.value_type == value_type ) )
			{
				*record_entry = (libpff_record_entry_t *) internal_record_entry;

				return( 1 );
			}
		}
	}
	*record_entry = NULL;

	return( 0 );
}

/* Retrieves the record entry matching the UTF-8 encoded name from the record set.
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE flag is set
 * the value type is ignored and set. The default behavior is a strict
 * matching of the value type. In this case the value type must be filled
 * with the corresponding value type
 *
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_record_set_get_entry_by_utf8_name(
     libpff_record_set_t *record_set,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint32_t value_type,
     libpff_record_entry_t **record_entry,
     uint8_t flags,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	libpff_internal_record_set_t *internal_record_set     = NULL;
	static char *function                                 = "libpff_record_set_get_entry_by_utf8_name";
	int entry_index                                       = 0;
	int number_of_entries                                 = 0;
	int result                                            = 0;

	if( record_set == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record set.",
		 function );

		return( -1 );
	}
	internal_record_set = (libpff_internal_record_set_t *) record_set;

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     internal_record_set->entries_array,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries.",
		 function );

		return( -1 );
	}
	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     internal_record_set->entries_array,
		     entry_index,
		     (intptr_t **) &internal_record_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record entry: %d.",
			 function,
			 entry_index );

			return( -1 );
		}
		if( internal_record_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing data record entry: %d.",
			 function,
			 entry_index );

			return( -1 );
		}
		/* There is a match
		 * if there is a name-to-id map entry
		 *   and the name-to-id map entry is a string
		 *   and the entry name string matches the one in the name-to-id map
		 */
		if( ( internal_record_entry->name_to_id_map_entry != NULL )
		 && ( internal_record_entry->name_to_id_map_entry->type == LIBPFF_NAME_TO_ID_MAP_ENTRY_TYPE_STRING ) )
		{
			if( internal_record_entry->name_to_id_map_entry->is_ascii_string == 0 )
			{
				result = libuna_utf8_string_compare_with_utf16_stream(
				          utf8_string,
				          utf8_string_length + 1,
				          internal_record_entry->name_to_id_map_entry->string_value,
				          internal_record_entry->name_to_id_map_entry->value_size,
				          LIBPFF_ENDIAN_LITTLE,
				          error );
			}
			else
			{
				result = libuna_utf8_string_compare_with_byte_stream(
				          utf8_string,
				          utf8_string_length + 1,
				          internal_record_entry->name_to_id_map_entry->string_value,
				          internal_record_entry->name_to_id_map_entry->value_size,
				          LIBUNA_CODEPAGE_ASCII,
				          error );
			}
			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to compare UTF-8 string with name to id map entry.",
				 function );

				return( -1 );
			}
			else if( result != 0 )
			{
				if( ( ( flags & LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE ) != 0 )
				 || ( internal_record_entry->identifier.value_type == value_type ) )
				{
					*record_entry = (libpff_record_entry_t *) internal_record_entry;

					return( 1 );
				}
			}
		}
	}
	*record_entry = NULL;

	return( 0 );
}

/* Retrieves the record entry matching the UTF-16 encoded name from the record set.
 *
 * When the LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE flag is set
 * the value type is ignored and set. The default behavior is a strict
 * matching of the value type. In this case the value type must be filled
 * with the corresponding value type
 *
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libpff_record_set_get_entry_by_utf16_name(
     libpff_record_set_t *record_set,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint32_t value_type,
     libpff_record_entry_t **record_entry,
     uint8_t flags,
     libcerror_error_t **error )
{
	libpff_internal_record_entry_t *internal_record_entry = NULL;
	libpff_internal_record_set_t *internal_record_set     = NULL;
	static char *function                                 = "libpff_record_set_get_entry_by_utf16_name";
	int entry_index                                       = 0;
	int number_of_entries                                 = 0;
	int result                                            = 0;

	if( record_set == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record set.",
		 function );

		return( -1 );
	}
	internal_record_set = (libpff_internal_record_set_t *) record_set;

	if( record_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record entry.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     internal_record_set->entries_array,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries.",
		 function );

		return( -1 );
	}
	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     internal_record_set->entries_array,
		     entry_index,
		     (intptr_t **) &internal_record_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve record entry: %d.",
			 function,
			 entry_index );

			return( -1 );
		}
		if( internal_record_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing data record entry: %d.",
			 function,
			 entry_index );

			return( -1 );
		}
		/* There is a match
		 * if there is a name-to-id map entry
		 *   and the name-to-id map entry is a string
		 *   and the entry name string matches the one in the name-to-id map
		 */
		if( ( internal_record_entry->name_to_id_map_entry != NULL )
		 && ( internal_record_entry->name_to_id_map_entry->type == LIBPFF_NAME_TO_ID_MAP_ENTRY_TYPE_STRING ) )
		{
			if( internal_record_entry->name_to_id_map_entry->is_ascii_string == 0 )
			{
				result = libuna_utf16_string_compare_with_utf16_stream(
				          utf16_string,
				          utf16_string_length + 1,
				          internal_record_entry->name_to_id_map_entry->string_value,
				          internal_record_entry->name_to_id_map_entry->value_size,
				          LIBPFF_ENDIAN_LITTLE,
				          error );
			}
			else
			{
				result = libuna_utf16_string_compare_with_byte_stream(
				          utf16_string,
				          utf16_string_length + 1,
				          internal_record_entry->name_to_id_map_entry->string_value,
				          internal_record_entry->name_to_id_map_entry->value_size,
				          LIBUNA_CODEPAGE_ASCII,
				          error );
			}
			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to compare UTF-16 string with name to id map entry.",
				 function );

				return( -1 );
			}
			else if( result != 0 )
			{
				if( ( ( flags & LIBPFF_ENTRY_VALUE_FLAG_MATCH_ANY_VALUE_TYPE ) != 0 )
				 || ( internal_record_entry->identifier.value_type == value_type ) )
				{
					*record_entry = (libpff_record_entry_t *) internal_record_entry;

					return( 1 );
				}
			}
		}
	}
	*record_entry = NULL;

	return( 0 );
}

