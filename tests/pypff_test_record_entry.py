#!/usr/bin/env python3
#
# Python-bindings record_entry type test script
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


class RecordEntryTypeTests(unittest.TestCase):
    """Tests the record_entry type."""

    def test_get_entry_type(self):
        """Tests the get_entry_type function and entry_type property."""
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

            record_entry = record_set.get_entry(0)
            if not record_entry:
                raise unittest.SkipTest("missing record entry")

            entry_type = record_entry.get_entry_type()
            self.assertIsNotNone(entry_type)

            self.assertIsNotNone(record_entry.entry_type)

        finally:
            pff_file.close()

    def test_get_value_type(self):
        """Tests the get_value_type function and value_type property."""
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

            record_entry = record_set.get_entry(0)
            if not record_entry:
                raise unittest.SkipTest("missing record entry")

            value_type = record_entry.get_value_type()
            self.assertIsNotNone(value_type)

            self.assertIsNotNone(record_entry.value_type)

        finally:
            pff_file.close()

    def test_get_data(self):
        """Tests the get_data function and data property."""
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

            record_entry = record_set.get_entry(0)
            if not record_entry:
                raise unittest.SkipTest("missing record entry")

            _ = record_entry.get_data()

            _ = record_entry.data

        finally:
            pff_file.close()

    # TODO: add tests for data_as_boolean
    # TODO: add tests for data_as_integer
    # TODO: add tests for data_as_datetime
    # TODO: add tests for data_as_size
    # TODO: add tests for data_as_floating_point
    # TODO: add tests for data_as_string


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
