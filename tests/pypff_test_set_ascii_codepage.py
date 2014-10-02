#!/usr/bin/env python
#
# Python-bindings set ASCII codepage testing program
#
# Copyright (c) 2008-2014, Joachim Metz <joachim.metz@gmail.com>
#
# Refer to AUTHORS for acknowledgements.
#
# This software is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this software.  If not, see <http://www.gnu.org/licenses/>.
#

import sys

import pypff

def main( argc, argv ):
	result = 0

	if argc != 1:
		print "Usage: pypff_test_set_ascii_codepage.py\n"
		return 1

	supported_codepages = [
		"ascii", "cp874", "cp932", "cp936", "cp949", "cp950", "cp1250", "cp1251",
		"cp1252", "cp1253", "cp1254", "cp1255", "cp1256", "cp1257", "cp1258" ]

	unsupported_codepages = [
		"iso-8859-1", "iso-8859-2", "iso-8859-3", "iso-8859-4", "iso-8859-5",
		"iso-8859-6", "iso-8859-7", "iso-8859-8", "iso-8859-9", "iso-8859-10",
		"iso-8859-11", "iso-8859-13", "iso-8859-14", "iso-8859-15", "iso-8859-16",
		"koi8_r", "koi8_u" ]

	pff_file = pypff.file()

	for codepage in supported_codepages:
		print "Testing setting supported ASCII codepage of: %s:\t" %( codepage ),

		try:
			pff_file.ascii_codepage = codepage
			result = 0
		except:
			result = 1

		if result != 0:
			print "(FAIL)"
			return 1
		print "(PASS)"

		print "Testing setting supported ASCII codepage of: %s:\t" %( codepage ),

		try:
			pff_file.set_ascii_codepage( codepage )
			result = 0
		except:
			result = 1

		if result != 0:
			print "(FAIL)"
			return 1
		print "(PASS)"

	for codepage in unsupported_codepages:
		print "Testing setting unsupported ASCII codepage of: %s:\t" %( codepage ),

		result = 1
		try:
			pff_file.ascii_codepage = codepage
		except RuntimeError, exception:
			if exception.message == "pypff_file_set_ascii_codepage_from_string: unable to determine ASCII codepage.":
				result = 0
		except:
			pass

		if result != 0:
			print "(FAIL)"
			return 1
		print "(PASS)"

		print "Testing setting unsupported ASCII codepage of: %s:\t" %( codepage ),

		result = 1
		try:
			pff_file.set_ascii_codepage( codepage )
		except RuntimeError, exception:
			if exception.message == "pypff_file_set_ascii_codepage_from_string: unable to determine ASCII codepage.":
				result = 0
		except:
			pass

		if result != 0:
			print "(FAIL)"
			return 1
		print "(PASS)"

	return 0

if __name__ == "__main__":
	sys.exit( main( len( sys.argv ), sys.argv ) )

