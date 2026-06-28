#!/usr/bin/env python3
#
# Python-bindings record_set type test script
#
# Copyright (C) 2008-2026, Joachim Metz <joachim.metz@gmail.com>
#
# Refer to AUTHORS for acknowledgements.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

import argparse
import os
import sys
import unittest

import pypff


class RecordSetTypeTests(unittest.TestCase):
    """Tests the record_set type."""

    def test_get_number_of_entries(self):
        """Tests the get_number_of_entries function and number_of_entries property."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            root_folder = pff_file.get_root_folder()
            if not root_folder:
                raise unittest.SkipTest("missing root folder")

            if not root_folder.number_of_record_sets:
                raise unittest.SkipTest("missing record sets")

            record_set = root_folder.get_record_set(0)
            if not record_set:
                raise unittest.SkipTest("missing record set")

            number_of_entries = record_set.get_number_of_entries()
            self.assertIsNotNone(number_of_entries)

            self.assertIsNotNone(record_set.number_of_entries)

        finally:
            pff_file.close()

    def test_get_entry(self):
        """Tests the get_entry function."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            root_folder = pff_file.get_root_folder()
            if not root_folder:
                raise unittest.SkipTest("missing root folder")

            if not root_folder.number_of_record_sets:
                raise unittest.SkipTest("missing record sets")

            record_set = root_folder.get_record_set(0)
            if not record_set:
                raise unittest.SkipTest("missing record set")

            if not record_set.number_of_entries:
                raise unittest.SkipTest("missing record entries")

            entry = record_set.get_entry(0)
            self.assertIsNotNone(entry)

        finally:
            pff_file.close()

    # TODO: add tests for entries

    def test_get_entry_by_type(self):
        """Tests the get_entry_by_type function."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            root_folder = pff_file.get_root_folder()
            if not root_folder:
                raise unittest.SkipTest("missing root folder")

            if not root_folder.number_of_record_sets:
                raise unittest.SkipTest("missing record sets")

            record_set = root_folder.get_record_set(0)
            if not record_set:
                raise unittest.SkipTest("missing record set")

            if not record_set.number_of_entries:
                raise unittest.SkipTest("missing record entries")

            _ = record_set.get_entry_by_type(0x3007, 0x0040)

            _ = record_set.get_entry_by_type(0x3008)

        finally:
            pff_file.close()



if __name__ == "__main__":
    argument_parser = argparse.ArgumentParser()

    argument_parser.add_argument(
        "source",
        nargs="?",
        action="store",
        metavar="PATH",
        default=None,
        help="path of the source file.",
    )
    options, unknown_options = argument_parser.parse_known_args()
    unknown_options.insert(0, sys.argv[0])

    setattr(unittest, "source", options.source)

    unittest.main(argv=unknown_options, verbosity=2)
