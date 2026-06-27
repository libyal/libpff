#!/usr/bin/env python3
#
# Python-bindings item type test script
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


class ItemTypeTests(unittest.TestCase):
    """Tests the item type."""

    def test_get_identifier(self):
        """Tests the get_identifier function and identifier property."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            root_folder = pff_file.get_root_folder()
            if not root_folder:
                raise unittest.SkipTest("missing root folder")

            identifier = root_folder.get_identifier()
            self.assertIsNotNone(identifier)

            self.assertIsNotNone(root_folder.identifier)

        finally:
            pff_file.close()

    def test_get_number_of_record_sets(self):
        """Tests the get_number_of_record_sets function and number_of_record_sets property."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            root_folder = pff_file.get_root_folder()
            if not root_folder:
                raise unittest.SkipTest("missing root folder")

            number_of_record_sets = root_folder.get_number_of_record_sets()
            self.assertIsNotNone(number_of_record_sets)

            self.assertIsNotNone(root_folder.number_of_record_sets)

        finally:
            pff_file.close()

    def test_get_record_set(self):
        """Tests the get_record_set function."""
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
            self.assertIsNotNone(record_set)

        finally:
            pff_file.close()

    # TODO: add tests for record_sets

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

            number_of_entries = root_folder.get_number_of_entries()
            self.assertIsNotNone(number_of_entries)

            self.assertIsNotNone(root_folder.number_of_entries)

        finally:
            pff_file.close()

    def test_get_number_of_sub_items(self):
        """Tests the get_number_of_sub_items function and number_of_sub_items property."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            root_folder = pff_file.get_root_folder()
            if not root_folder:
                raise unittest.SkipTest("missing root folder")

            number_of_sub_items = root_folder.get_number_of_sub_items()
            self.assertIsNotNone(number_of_sub_items)

            self.assertIsNotNone(root_folder.number_of_sub_items)

        finally:
            pff_file.close()

    def test_get_sub_item(self):
        """Tests the get_sub_item function."""
        test_source = getattr(unittest, "source", None)
        if not test_source:
            raise unittest.SkipTest("missing source")

        pff_file = pypff.file()

        pff_file.open(test_source)

        try:
            root_folder = pff_file.get_root_folder()
            if not root_folder:
                raise unittest.SkipTest("missing root folder")

            if not root_folder.number_of_sub_items:
                raise unittest.SkipTest("missing sub items")

            sub_item = root_folder.get_sub_item(0)
            self.assertIsNotNone(sub_item)

        finally:
            pff_file.close()

    # TODO: add tests for sub_items


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
