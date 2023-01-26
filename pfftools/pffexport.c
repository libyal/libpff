/*
 * Extracts items from a Personal Folder File (OST, PAB and PST)
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

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "export_handle.h"
#include "log_handle.h"
#include "pffinput.h"
#include "pfftools_getopt.h"
#include "pfftools_libcerror.h"
#include "pfftools_libclocale.h"
#include "pfftools_libcnotify.h"
#include "pfftools_libcpath.h"
#include "pfftools_libpff.h"
#include "pfftools_output.h"
#include "pfftools_signal.h"
#include "pfftools_unused.h"

export_handle_t *pffexport_export_handle = NULL;
libpff_file_t *pffexport_file            = NULL;
int pffexport_abort                      = 0;

/* Prints the executable usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use pffexport to export items stored in a Personal Folder File (OST, PAB\n"
	                 "and PST).\n\n" );

	fprintf( stream, "Usage: pffexport [ -c codepage ] [ -f format ] [ -l logfile ] [ -m mode ]\n"
	                 "                 [ -t target ] [ -dhqvV ] source\n\n" );

	fprintf( stream, "\tsource: the source file\n\n" );

	fprintf( stream, "\t-c:     codepage of ASCII strings, options: ascii, windows-874,\n"
	                 "\t        windows-932, windows-936, windows-949, windows-950,\n"
	                 "\t        windows-1250, windows-1251, windows-1252 (default),\n"
	                 "\t        windows-1253, windows-1254, windows-1255, windows-1256\n"
	                 "\t        windows-1257 or windows-1258\n" );
	fprintf( stream, "\t-d:     dumps the item values in a separate file: ItemValues.txt\n" );
	fprintf( stream, "\t-f:     preferred output format, options: all, html, rtf,\n"
	                 "\t        text (default)\n" );
	fprintf( stream, "\t-h:     shows this help\n" );
	fprintf( stream, "\t-l:     logs information about the exported items\n" );
	fprintf( stream, "\t-m:     export mode, option: all, debug, items (default), recovered.\n"
	                 "\t        'all' exports the (allocated) items, orphan and recovered\n"
	                 "\t        items. 'debug' exports all the (allocated) items, also those\n"
	                 "\t        outside the the root folder. 'items' exports the (allocated)\n"
	                 "\t        items. 'recovered' exports the orphan and recovered items.\n" );
	fprintf( stream, "\t-q:     quiet shows minimal status information\n" );
	fprintf( stream, "\t-t:     specify the basename of the target directory to export to\n"
	                 "\t        (default is the source filename) pffexport will add the\n"
	                 "\t        following suffixes to the basename: .export, .orphans,\n"
	                 "\t        .recovered\n" );
	fprintf( stream, "\t-v:     verbose output to stderr\n" );
	fprintf( stream, "\t-V:     print version\n" );
}

/* Signal handler for pffexport
 */
void pffexport_signal_handler(
      pfftools_signal_t signal PFFTOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function   = "pffexport_signal_handler";

	PFFTOOLS_UNREFERENCED_PARAMETER( signal )

	pffexport_abort = 1;

	if( pffexport_export_handle != NULL )
	{
		if( export_handle_signal_abort(
		     pffexport_export_handle,
		     &error ) != 1 )
		{
			libcnotify_printf(
			 "%s: unable to signal export handle to abort.\n",
			 function );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
	}
	if( pffexport_file != NULL )
	{
		if( libpff_file_signal_abort(
		     pffexport_file,
		     &error ) != 1 )
		{
			libcnotify_printf(
			 "%s: unable to signal file to abort.\n",
			 function );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
#if defined( WINAPI ) && !defined( __CYGWIN__ )
	if( _close(
	     0 ) != 0 )
#else
	if( close(
	     0 ) != 0 )
#endif
	{
		libcnotify_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libcerror_error_t *error                           = NULL;
	log_handle_t *log_handle                           = NULL;
	system_character_t *log_filename                   = NULL;
	system_character_t *option_ascii_codepage          = NULL;
	system_character_t *option_export_mode             = NULL;
	system_character_t *option_preferred_export_format = NULL;
	system_character_t *option_target_path             = NULL;
	system_character_t *path_separator                 = NULL;
	system_character_t *source                         = NULL;
	char *program                                      = "pffexport";
	system_integer_t option                            = 0;
	size_t source_length                               = 0;
	uint8_t dump_item_values                           = 0;
	uint8_t print_status_information                   = 1;
	int result                                         = 0;
	int verbose                                        = 0;

	libcnotify_stream_set(
	 stderr,
	 NULL );
	libcnotify_verbose_set(
	 1 );

	if( libclocale_initialize(
	     "pfftools",
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize locale values.\n" );

		goto on_error;
	}
	if( pfftools_output_initialize(
	     _IONBF,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize output settings.\n" );

		goto on_error;
	}
	pfftools_output_version_fprint(
	 stdout,
	 program );

	while( ( option = pfftools_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "c:df:hl:m:qt:vV" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM "\n",
				 argv[ optind - 1 ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (system_integer_t) 'c':
				option_ascii_codepage = optarg;

				break;

			case (system_integer_t) 'd':
				dump_item_values = 1;

				break;

			case (system_integer_t) 'f':
				option_preferred_export_format = optarg;

				break;

			case (system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'l':
				log_filename = optarg;

				break;

			case (system_integer_t) 'm':
				option_export_mode = optarg;

				break;

			case (system_integer_t) 'q':
				print_status_information = 0;

				break;

			case (system_integer_t) 't':
				option_target_path = optarg;

				break;

			case (system_integer_t) 'v':
				verbose = 1;

				break;

			case (system_integer_t) 'V':
				pfftools_output_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing source file.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	source = argv[ optind ];

	if( option_target_path == NULL )
	{
		source_length = system_string_length(
		                 source );

		path_separator = system_string_search_character_reverse(
		                  source,
		                  (system_character_t) LIBCPATH_SEPARATOR,
		                  source_length );

		if( path_separator == NULL )
		{
			path_separator = source;
		}
		else
		{
			path_separator++;
		}
		option_target_path = path_separator;
	}
	libcnotify_verbose_set(
	 verbose );
	libpff_notify_set_stream(
	 stderr,
	 NULL );
	libpff_notify_set_verbose(
	 verbose );

	if( export_handle_initialize(
	     &pffexport_export_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create export handle.\n" );

		goto on_error;
	}
	pffexport_export_handle->print_status_information = print_status_information;

	if( option_export_mode != NULL )
	{
		result = export_handle_set_export_mode(
			  pffexport_export_handle,
			  option_export_mode,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set export mode.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stderr,
			 "Unsupported export mode defaulting to: items.\n" );
		}
	}
	pffexport_export_handle->dump_item_values = dump_item_values;

	if( option_preferred_export_format != NULL )
	{
		result = export_handle_set_preferred_export_format(
			  pffexport_export_handle,
			  option_preferred_export_format,
			  &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set preferred export format.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stderr,
			 "Unsupported preferred export format defaulting to: text.\n" );
		}
	}
	if( option_ascii_codepage != NULL )
	{
		result = export_handle_set_ascii_codepage(
		          pffexport_export_handle,
		          option_ascii_codepage,
		          &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set ASCII codepage in export handle.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stderr,
			 "Unsupported ASCII codepage defaulting to: windows-1252.\n" );
		}
	}
	if( export_handle_set_target_path(
	     pffexport_export_handle,
	     option_target_path,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to set target path.\n" );

		goto on_error;
	}
	result = export_handle_create_items_export_path(
	          pffexport_export_handle,
	          &error );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to create items export path.\n" );

		goto on_error;
	}
	else if( result == 0 )
	{
		fprintf(
		 stderr,
		 "%" PRIs_SYSTEM " already exists.\n",
		 pffexport_export_handle->items_export_path );

		goto on_error;
	}
	result = export_handle_create_orphans_export_path(
	          pffexport_export_handle,
	          &error );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to create orphans export path.\n" );

		goto on_error;
	}
	else if( result == 0 )
	{
		fprintf(
		 stderr,
		 "%" PRIs_SYSTEM " already exists.\n",
		 pffexport_export_handle->orphans_export_path );

		goto on_error;
	}
	result = export_handle_create_recovered_export_path(
	          pffexport_export_handle,
	          &error );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to create recovered export path.\n" );

		goto on_error;
	}
	else if( result == 0 )
	{
		fprintf(
		 stderr,
		 "%" PRIs_SYSTEM " already exists.\n",
		 pffexport_export_handle->recovered_export_path );

		goto on_error;
	}
	if( log_handle_initialize(
	     &log_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create log handle.\n" );

		goto on_error;
	}
	if( log_handle_open(
	     log_handle,
	     log_filename,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open log file: %" PRIs_SYSTEM ".\n",
		 log_filename );

		goto on_error;
	}
	if( libpff_file_initialize(
	     &pffexport_file,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create file.\n" );

		goto on_error;
	}
	if( libpff_file_set_ascii_codepage(
	     pffexport_file,
	     pffexport_export_handle->ascii_codepage,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to set ASCII codepage.\n" );

		goto on_error;
	}
	if( pfftools_signal_attach(
	     pffexport_signal_handler,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to attach signal handler.\n" );

		goto on_error;
	}
	fprintf(
	 stdout,
	 "Opening file.\n" );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libpff_file_open_wide(
	     pffexport_file,
	     source,
	     LIBPFF_OPEN_READ,
	     &error ) != 1 )
#else
	if( libpff_file_open(
	     pffexport_file,
	     source,
	     LIBPFF_OPEN_READ,
	     &error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Error opening file: %" PRIs_SYSTEM ".\n",
		 source );

		goto on_error;
	}
	if( export_handle_export_file(
	     pffexport_export_handle,
	     pffexport_file,
	     log_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to export file.\n" );

		goto on_error;
	}
	if( libpff_file_close(
	     pffexport_file,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close file.\n" );

		goto on_error;
	}
	if( pfftools_signal_detach(
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to detach signal handler.\n" );

		goto on_error;
	}
	if( libpff_file_free(
	     &pffexport_file,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free file.\n" );

		goto on_error;
	}
	if( log_handle_close(
	     log_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close log file.\n" );

		goto on_error;
	}
	if( log_handle_free(
	     &log_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free log handle.\n" );

		goto on_error;
	}
	if( export_handle_free(
	     &pffexport_export_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free export handle.\n" );

		goto on_error;
	}
	if( pffexport_abort != 0 )
	{
		fprintf(
		 stdout,
		 "Export aborted.\n" );

		return( EXIT_FAILURE );
	}
	fprintf(
	 stdout,
	 "Export completed.\n" );

	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( pffexport_file != NULL )
	{
		libpff_file_close(
		 pffexport_file,
		 NULL );
		libpff_file_free(
		 &pffexport_file,
		 NULL );
	}
	if( log_handle != NULL )
	{
		log_handle_close(
		 log_handle,
		 NULL );
		log_handle_free(
		 &log_handle,
		 NULL );
	}
	if( pffexport_export_handle != NULL )
	{
		export_handle_free(
		 &pffexport_export_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

