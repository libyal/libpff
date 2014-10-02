package jpff;

/*
 * File class
 *
 * Copyright (c) 2010, Joachim Metz <joachim.metz@gmail.com>
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

import jpff.Exception;

/**
 * File class
 * Wraps libpff_file using JNI
 */
public class File
{
	/**
	 * Internal reference of the libpff file
	 */
	protected long libpff_file = 0;

	/* Internal functions
	 */
	private static native void internal_class_initialize();

	private native void internal_initialize();

	private native void internal_free();

	private native void internal_open(
	                     String filename,
	                     int access_flags )
	                     throws jpff.Exception;

	private native void internal_close()
	                     throws jpff.Exception;

	/* External functions
	 */

	/**
	 * Creates a new file
	 */
	public File()
	{
		synchronized(
		 this )
		{
			internal_initialize();
		}
	}

	/**
	 * Destructor
	 */
	protected void finalize()
	{
		synchronized(
		 this )
		{
			internal_free();
		}
	}

	/**
	 * Opens a file
	 *
	 * @param filename the name of the file
	 * @param access_flags the access flags (e.g. ACCESS_FLAGS_READ)
	 */
	public void open(
	             String filename,
	             int access_flags )
	             throws jpff.Exception
	{
		synchronized(
		 this )
		{
			internal_open(
			 filename,
			 access_flags );
		}
	}

	/**
	 * Closes a file
	 */
	public void close()
	             throws jpff.Exception
	{
		synchronized(
		 this )
		{
			internal_close();
		}
	}

	static
	{
		try
		{
			System.loadLibrary(
			 "jpff" );
		}
		catch( Throwable throwable )
		{
			System.err.println(
			 "Unable to load jpff: " + throwable );
		}
		internal_class_initialize();
	}
}

